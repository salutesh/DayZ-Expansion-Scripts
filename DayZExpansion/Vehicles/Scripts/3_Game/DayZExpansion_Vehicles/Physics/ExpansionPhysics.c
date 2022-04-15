/**
 * ExpansionPhysics.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionPhysics
{
	static float CalculateBuoyancyAtPosition(vector position, float offset, float objectMass, float objectDensity, vector velocity, out bool isAboveWater)
	{
		float resultForce = 0;
		float downForce = 0.5;

		float depth = GetGame().GetWaterDepth(position) + offset;

		float forceFactor = 1.0 - (depth / objectDensity);
		forceFactor = -forceFactor;
		if (forceFactor > 0)
		{
			resultForce = 10.0 * objectMass * (forceFactor - velocity[1] * 0.5);
			resultForce += -downForce * objectMass;

			isAboveWater = false;
		}
		else
		{
			isAboveWater = true;
		}

		return resultForce;
	}

	static vector GetVelocityInLocal(IEntity ent, vector relPos, vector linearVelocity, vector angularVelocity)
	{
		return linearVelocity + (angularVelocity * relPos);
	}

	static float ResolveSingleBilateral(Object body1,
										vector pos1,
										vector rel_vel_at_pos,
										Object body2,
										vector pos2,
										vector normal)
	{
		float normalLenSqr = normal.LengthSq();
		if (normalLenSqr > 1.1)
		{
			return 0;
		}

		vector vel;
		vector invInertiaDiagLocal1 = dBodyGetInvInertiaDiagLocal(body1);
		vector linearVelocity1 = GetVelocity(body1);
		vector angularVelocity1 = dBodyGetAngularVelocity(body1);
		vector linearVelocity2;
		vector angularVelocity2;
		vector orientation2[3];
		float invMass2 = 0;

		vector rel_pos1 = pos1;
		vector rel_pos2 = "0 0 0";

		vector vel1 = rel_vel_at_pos;
		vector orientation1[3];
		body1.GetTransform(orientation1);
		vector invInertiaDiagLocal2;
		float invMass1 = 1.0 / dBodyGetMass(body1);

		if (body2) // if is a dynamic body
		{
			rel_pos2 = pos2;
			vector vel2 = dBodyGetVelocityAt(body2, body2.GetGlobalPos(rel_pos2));

			body2.GetTransform(orientation2);

			invInertiaDiagLocal2 = dBodyGetInvInertiaDiagLocal(body2);
			invMass2 = 1.0 / dBodyGetMass(body2);

			linearVelocity2 = GetVelocity(body2);
			angularVelocity2 = dBodyGetAngularVelocity(body2);

			vel = vel1 - vel2;
		}
		else
		{
			Math3D.MatrixIdentity3(orientation2);
			vel = vel1;
		}

		//Matrix3.Transpose(orientation1, orientation1);
		//Matrix3.Transpose(orientation2, orientation2);

		ExpansionJacobianEntry jac = new ExpansionJacobianEntry(orientation1, orientation2, rel_pos1, rel_pos2, normal, invInertiaDiagLocal1, invMass1, invInertiaDiagLocal2, invMass2);

		float jacDiagAB = jac.GetDiagonal();
		float jacDiagABInv = 1.0 / jacDiagAB;

		float rel_vel = jac.GetRelativeVelocity(linearVelocity1.Multiply3(orientation1), angularVelocity1.Multiply3(orientation1), linearVelocity2.Multiply3(orientation2), angularVelocity2.Multiply3(orientation2));

		rel_vel = vector.Dot(normal, vel);

		float contactDamping = 10.0;

		float res = -contactDamping * rel_vel * jacDiagABInv;

		return res;
	}

	static float ComputeImpulseDenominator(vector pos, vector normal, Matrix3 invInertiaWS, float invMass)
	{
		vector c0 = pos * normal;

		vector vec = c0.Multiply3(invInertiaWS.data) * pos;

		return invMass + vector.Dot(normal, vec);
	}

	static void IntegrateTransform(vector pTransform0[4], vector pLinVel, vector pAngVel, float pDt, inout vector pResult[4])
	{
		/*
		vector futureAngularVelocity = pAngVel * pDt;

		pResult[0][0] = 0.0;
		pResult[1][1] = 0.0;
		pResult[2][2] = 0.0;

		pResult[0][1] = -futureAngularVelocity[2];
		pResult[1][0] = futureAngularVelocity[2];
		pResult[2][0] = -futureAngularVelocity[1];
		pResult[0][2] = futureAngularVelocity[1];
		pResult[1][2] = -futureAngularVelocity[0];
		pResult[2][1] = futureAngularVelocity[0];

		Math3D.MatrixMultiply3(pResult, pTransform0, pResult);

		pResult[0] = pTransform0[0] + pResult[0];
		pResult[1] = pTransform0[1] + pResult[1];
		pResult[2] = pTransform0[2] + pResult[2];

		pResult[0].Normalize();
		pResult[1].Normalize();
		pResult[2].Normalize();

		pResult[3] = pTransform0[3] + (pLinVel * pDt);
		*/

		///*
		vector m1[];
		Math3D.YawPitchRollMatrix(pAngVel * pDt * Math.RAD2DEG, m1);
		Math3D.MatrixMultiply3(pTransform0, m1, pResult);

		pResult[3] = pTransform0[3] + (pLinVel * pDt);
		//*/
	}

	static void CalculateVelocity(vector pTransform0[4], vector pTransform1[4], float pDt, out vector pLinVel, out vector pAngVel)
	{
		float invDt = 1.0;
		if (pDt != 0.0)
			invDt = 1.0 / pDt;

		pLinVel = (pTransform1[3] - pTransform0[3]) * invDt;

		CalculateAngularVelocity(pTransform0, pTransform1, pAngVel);

		pAngVel = pAngVel * invDt;
	}

	static void CalculateAngularVelocity(vector pTransform0[4], vector pTransform1[4], out vector pAngVel)
	{
		vector res[3];
		Math3D.MatrixInvMultiply3(pTransform0, pTransform1, res);

		float q[4];
		Math3D.MatrixToQuat(res, q);

		float angle = 2.0 * Math.Acos(q[3]);
		pAngVel = Vector(q[0], q[1], q[2]);
		float len = pAngVel.Length();
		if (len < 0.00001)
		{
			pAngVel = "0 0 0";
		}
		else
		{
			pAngVel[0] = pAngVel[0] * angle / len;
			pAngVel[1] = pAngVel[1] * angle / len;
			pAngVel[2] = pAngVel[2] * angle / len;
		}
	}

	static void MatrixToQuat(vector mat[4], out float q[4])
	{
		float trace = mat[0][0] + mat[1][1] + mat[2][2];

		float s;
		int i;
		int j;
		int k;

		if (trace > 0.0)
		{
			s = Math.Sqrt(trace + 1.0);
			q[3] = (s * 0.5);
			s = 0.5 / s;

			q[0] = ((mat[2][1] - mat[1][2]) * s);
			q[1] = ((mat[0][2] - mat[2][0]) * s);
			q[2] = ((mat[1][0] - mat[0][1]) * s);
			return;
		}

		if (mat[0][0] < mat[1][1])
		{
			i = 1;
			if (mat[1][1] < mat[2][2])
				i = 2;
		}
		else if (mat[0][0] < mat[2][2])
			i = 2;

		j = (i + 1) % 3;
		k = (i + 2) % 3;

		s = Math.Sqrt(mat[i][i] - mat[j][j] - mat[k][k] + 1.0);
		q[i] = s * 0.5;
		s = 0.5 / s;

		q[3] = (mat[k][j] - mat[j][k]) * s;
		q[j] = (mat[j][i] + mat[i][j]) * s;
		q[k] = (mat[k][i] + mat[i][k]) * s;
	}

	static void QuatAxis(vector axis, out float qR[4])
	{
		float halfA = 0.5 * axis.Normalize();
		float s = Math.Sin(halfA);

		qR[0] = s * axis[0];
		qR[1] = s * axis[1];
		qR[2] = s * axis[2];
		qR[3] = Math.Cos(halfA);

		float SquareSum = qR[0] * qR[0] + qR[1] * qR[1] + qR[2] * qR[2] + qR[3] * qR[3];

		if (SquareSum > 0.0)
		{
			float Scale = 1.0 / Math.Sqrt(SquareSum);

			qR[0] = qR[0] * Scale;
			qR[1] = qR[1] * Scale;
			qR[2] = qR[2] * Scale;
			qR[3] = qR[3] * Scale;
		}
	}

	static void QuatAxis(vector axis, float angle, out float qR[4])
	{
		float halfA = 0.5 * angle;
		float s = Math.Sin(halfA);

		qR[0] = s * axis[0];
		qR[1] = s * axis[1];
		qR[2] = s * axis[2];
		qR[3] = Math.Cos(halfA);

		float SquareSum = qR[0] * qR[0] + qR[1] * qR[1] + qR[2] * qR[2] + qR[3] * qR[3];

		if (SquareSum > 0.0)
		{
			float Scale = 1.0 / Math.Sqrt(SquareSum);

			qR[0] = qR[0] * Scale;
			qR[1] = qR[1] * Scale;
			qR[2] = qR[2] * Scale;
			qR[3] = qR[3] * Scale;
		}
	}

	static void QuatToAxisAndAngle(float q[4], out vector axis, out float angle)
	{
		axis[0] = q[0];
		axis[1] = q[1];
		axis[2] = q[2];
		angle = 2 * Math.Acos(q[3]);
	}

	static void QuatToAxis(float q[4], out vector axis)
	{
		float scale = 2.0 * Math.Acos(q[3]);
		axis[0] = q[0] * scale;
		axis[1] = q[1] * scale;
		axis[2] = q[2] * scale;
	}

	static void QuatNormalize(float q1[4], out float qR[4])
	{
		float SquareSum = q1[0] * q1[0] + q1[1] * q1[1] + q1[2] * q1[2] + q1[3] * q1[3];

		if (SquareSum > 0.0)
		{
			float Scale = 1.0 / Math.Sqrt(SquareSum);

			qR[0] = q1[0] * Scale;
			qR[1] = q1[1] * Scale;
			qR[2] = q1[2] * Scale;
			qR[3] = q1[3] * Scale;
		}
	}

	static void QuatInverse(float q1[4], out float qR[4])
	{
		qR[0] = -q1[0];
		qR[1] = -q1[1];
		qR[2] = -q1[2];
		qR[3] = q1[3];
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
		}
		else
		{
			qR[0] = -q2[0];
			qR[1] = -q2[1];
			qR[2] = -q2[2];
			qR[3] = -q2[3];
		}
	}

	static void QuatSlerp(out float qR[4], float q1[4], float q2[4], float Slerp)
	{
		// Get cosine of angle between quats.
		float RawCosom = q1[0] * q2[0] + q1[1] * q2[1] + q1[2] * q2[2] + q1[3] * q2[3];

		// Unaligned quats - compensate, results in taking shorter route.
		float Cosom = RawCosom;
		if (RawCosom < 0)
			Cosom = -RawCosom;

		float Scale0, Scale1;
		if (Cosom < 0.9999)
		{
			float Omega = Math.Acos(Cosom);
			float InvSin = 1.0 / Math.Sin(Omega);
			Scale0 = Math.Sin((1.0 - Slerp) * Omega) * InvSin;
			Scale1 = Math.Sin(Slerp * Omega) * InvSin;
		}
		else
		{
			// Use linear interpolation.
			Scale0 = 1.0 - Slerp;
			Scale1 = Slerp;
		}

		// In keeping with our flipped Cosom:
		if (RawCosom < 0)
			Scale1 = -Scale1;

		qR[0] = Scale0 * q1[0] + Scale1 * q2[0];
		qR[1] = Scale0 * q1[1] + Scale1 * q2[1];
		qR[2] = Scale0 * q1[2] + Scale1 * q2[2];
		qR[3] = Scale0 * q1[3] + Scale1 * q2[3];

		float SquareSum = qR[0] * qR[0] + qR[1] * qR[1] + qR[2] * qR[2] + qR[3] * qR[3];

		if (SquareSum > 0.0)
		{
			float Scale = 1.0 / Math.Sqrt(SquareSum);

			qR[0] = qR[0] * Scale;
			qR[1] = qR[1] * Scale;
			qR[2] = qR[2] * Scale;
			qR[3] = qR[3] * Scale;
		}
	}
};
