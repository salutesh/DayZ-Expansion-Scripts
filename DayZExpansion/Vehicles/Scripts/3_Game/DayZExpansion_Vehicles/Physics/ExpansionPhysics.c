/**
 * ExpansionPhysics.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionPhysics
{
	static float CalculateBuoyancyAtPosition( vector position, float offset, float objectMass, float objectDensity, vector velocity, out bool isAboveWater )
	{
		float resultForce = 0;
		float downForce = 0.5;

		float depth = GetGame().GetWaterDepth( position ) + offset;

		float forceFactor = 1.0 - ( depth / objectDensity );
		forceFactor = -forceFactor;
		if ( forceFactor > 0 )
		{
			resultForce = 10.0 * objectMass * ( forceFactor - velocity[1] * 0.5 );
			resultForce += -downForce * objectMass;
			
			isAboveWater = false;
		} else
		{
			isAboveWater = true;
		}

		return resultForce;
	}

	static vector GetVelocityInLocal( IEntity ent, vector relPos, vector linearVelocity, vector angularVelocity )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionPhysics::GetVelocityInLocal - Return: " + linearVelocity + ( angularVelocity * relPos ).ToString() );
		#endif
		return linearVelocity + ( angularVelocity * relPos );
	}

	static float ResolveSingleBilateral( Object body1,
										 vector pos1,
										 vector rel_vel_at_pos,
										 Object body2,
										 vector pos2,
										 vector normal )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionPhysics::ResolveSingleBilateral - Start");
		#endif
		
		float normalLenSqr = normal.LengthSq();
		if ( normalLenSqr > 1.1 )
		{
			return 0;
		}

		vector vel;
		vector invInertiaDiagLocal1;
		vector linearVelocity1 = GetVelocity( body1 );
		vector angularVelocity1 = dBodyGetAngularVelocity( body1 );
		vector linearVelocity2;
		vector angularVelocity2;
		Matrix3 invOrientation2 = new Matrix3;
		float invMass2 = 0;

		vector rel_pos1 = pos1 - dBodyGetCenterOfMass( body1 );
		vector rel_pos2 = "0 0 0";

		vector vel1 = rel_vel_at_pos;
		Transform trans1 = Transform.GetObject( body1 );
		Matrix3 invOrientation1 = trans1.GetBasis(); // .Transpose();
		vector invInertiaDiagLocal2 = dBodyGetInvInertiaDiagLocal( body1 );
		float invMass1 = 1.0 / dBodyGetMass( body1 );
		
		if ( body2 ) // if is a dynamic body
		{
			rel_pos2 = pos2 - dBodyGetCenterOfMass( body2 );
			vector vel2 = dBodyGetVelocityAt( body2, body2.GetGlobalPos( rel_pos2 ) );

			invOrientation2 = Transform.GetObject( body2 ).GetBasis().Transpose();
			invInertiaDiagLocal2 = dBodyGetInvInertiaDiagLocal( body2 );
			invMass2 = 1.0 / dBodyGetMass( body2 );
			
			linearVelocity2 = GetVelocity( body2 );
			angularVelocity2 = dBodyGetAngularVelocity( body2 );
			
			vel = vel1 - vel2;
		} else
		{
			vel = vel1;
		}

		ExpansionJacobianEntry jac = new ExpansionJacobianEntry( invOrientation1, invOrientation2, rel_pos1, rel_pos2, normal, invInertiaDiagLocal1, invMass1, invInertiaDiagLocal2, invMass2 );

		float jacDiagAB = jac.GetDiagonal();
		float jacDiagABInv = 1.0 / jacDiagAB;
		
		float rel_vel =	jac.GetRelativeVelocity( 
						linearVelocity1, 
						invOrientation1.Multiply( angularVelocity1 ), 
						linearVelocity2, 
						invOrientation2.Multiply( angularVelocity2 ) );
		rel_vel *= vector.Dot( normal, vel );

		float contactDamping = 0.2;

		#ifdef EXPANSIONEXPRINT
		EXPrint( "ExpansionPhysics::ResolveSingleBilateral - Return: " + (contactDamping * rel_vel * jacDiagABInv).ToString() );
		#endif

		return contactDamping * rel_vel * jacDiagABInv;
	}

	static float ComputeImpulseDenominator( vector pos, vector normal, Matrix3 invInertiaWS, float invMass )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "ExpansionPhysics::ComputeImpulseDenominator - Start");
		#endif
		
		vector c0 = pos * normal;

		vector vec = c0.Multiply3( invInertiaWS.data ) * pos;

		#ifdef EXPANSIONEXPRINT
		EXPrint( "ExpansionPhysics::ComputeImpulseDenominator - Return: " + (invMass + vector.Dot( normal, vec )).ToString() );
		#endif

		return invMass + vector.Dot( normal, vec );
	}

	static void IntegrateTransform( vector curTrans[4], vector linVel, vector angVel, float timestep, inout vector predictedTrans[4] )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "ExpansionPhysics::IntegrateTransform - Start");
		#endif

		vector m1[];
		Math3D.YawPitchRollMatrix( angVel * timestep, m1 );
		Math3D.MatrixMultiply3( curTrans, m1, predictedTrans );

		predictedTrans[3] = curTrans[3] + ( linVel * timestep );

		#ifdef EXPANSIONEXPRINT
		EXPrint( "ExpansionPhysics::IntegrateTransform - End");
		#endif
	}

	static void CalculateVelocity( vector pTransform0[4], vector pTransform1[4], float pDt, out vector pLinVel, out vector pAngVel )
	{
		float invDt = 1.0;
		if ( pDt != 0.0 )
			invDt = 1.0 / pDt;
			
		pLinVel = ( pTransform1[3] - pTransform0[3] ) * invDt;

		vector axis;
		float angle;
		CalculateAngularVelocity(pTransform0, pTransform1, axis, angle);

		pAngVel = axis * angle * invDt;
	}

	static void CalculateAngularVelocity(vector pTransform0[4], vector pTransform1[4], out vector pAxis, out float pAngle)
	{
		vector m[3];
		Math3D.MatrixInvMultiply3(pTransform1, pTransform0, m);
		
		float quatR[4];
		Math3D.MatrixToQuat(m, quatR);

		pAngle = 2.0 * Math.Acos(quatR[3]);
		pAxis = Vector(quatR[0], quatR[1], quatR[2]);
		
		float len = pAxis.Length();
		if (len < 0.000001)
		{
			pAxis = "0 0 1";
		} else
		{
			pAxis[0] = pAxis[0] / len;
			pAxis[1] = pAxis[1] / len;
			pAxis[2] = pAxis[2] / len;
		}
	}
	
	static void QuatInverse(float q1[4], out float qR[4])
	{
		qR[0] = -q1[0];
		qR[1] = -q1[1];
		qR[2] = -q1[2];
		qR[3] = -q1[3];
	}
	
	static void QuatAdd(float q1[4], float q2[4], out float qR[4])
	{
		qR[0] = q1[0] + q2[0];
		qR[1] = q1[1] + q2[1];
		qR[2] = q1[2] + q2[2];
		qR[3] = q1[3] + q2[3];
	}
	
	static void QuatSub(float q1[4], float q2[4], out float qR[4])
	{
		qR[0] = q1[0] - q2[0];
		qR[1] = q1[1] - q2[1];
		qR[2] = q1[2] - q2[2];
		qR[3] = q1[3] - q2[3];
	}
	
	static void QuatMultiply(float q1[4], float q2[4], out float qR[4])
	{
		qR[0] = (q1[3] * q2[0]) + (q1[0] * q2[3]) + (q1[1] * q2[2]) - (q1[2] * q2[1]);
		qR[1] = (q1[3] * q2[1]) + (q1[1] * q2[3]) + (q1[2] * q2[0]) - (q1[0] * q2[2]);
		qR[2] = (q1[3] * q2[2]) + (q1[2] * q2[3]) + (q1[0] * q2[1]) - (q1[1] * q2[0]);
		qR[3] = (q1[3] * q2[3]) - (q1[0] * q2[0]) - (q1[1] * q2[1]) - (q1[2] * q2[2]);
	}
	
	static float QuatDot(float q1[4], float q2[4])
	{
		return (q1[0] * q2[0]) + (q1[1] * q2[1]) + (q1[2] * q2[2]) + (q1[3] * q2[3]);
	}
	
	static void QuatNearest(float q1[4], float q2[4], out float qR[4])
	{
		float diff[4];
		float sum[4];
		
		QuatSub(q1, q2, diff);
		QuatAdd(q1, q2, sum);
		
		if (QuatDot(diff, diff) < QuatDot(sum, sum))
		{
			qR[0] = q2[0];
			qR[1] = q2[1];
			qR[2] = q2[2];
			qR[3] = q2[3];
		} else
		{
			qR[0] = -q2[0];
			qR[1] = -q2[1];
			qR[2] = -q2[2];
			qR[3] = -q2[3];
		}
	}
};