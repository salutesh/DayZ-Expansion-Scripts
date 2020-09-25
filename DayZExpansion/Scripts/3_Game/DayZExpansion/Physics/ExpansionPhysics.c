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

		vector vec = VectorHelper.Multiply( c0, invInertiaWS ) * pos;

		#ifdef EXPANSIONEXPRINT
		EXPrint( "ExpansionPhysics::ComputeImpulseDenominator - Return: " + (invMass + vector.Dot( normal, vec )).ToString() );
		#endif

		return invMass + vector.Dot( normal, vec );
	}

	static void IntegrateTransform( Transform curTrans, vector linVel, vector angVel, float timestep, inout Transform predictedTrans )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "ExpansionPhysics::IntegrateTransform - Start");
		#endif

		//! TODO: Optimize, we can use native methods. Although the code readability may lower
		
		predictedTrans.data[3] = curTrans.data[3] + ( linVel * timestep );

		Quaternion predictedRot = curTrans.GetRotation();
		predictedRot.AddSelf( VectorHelper.Multiply( angVel, predictedRot ).MultiplySelf( timestep * 0.5 ) );

		predictedTrans.SetRotation( predictedRot );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint( "ExpansionPhysics::IntegrateTransform - End");
		#endif
	}
};