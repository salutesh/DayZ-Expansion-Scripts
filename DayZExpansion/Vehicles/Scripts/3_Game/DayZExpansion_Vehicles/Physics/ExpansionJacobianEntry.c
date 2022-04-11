/**
 * ExpansionJacobianEntry.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
 */

class ExpansionJacobianEntry
{
	vector m_linearJointAxis;
	vector m_aJ;
	vector m_bJ;
	vector m_0MinvJt;
	vector m_1MinvJt;

	float m_Adiag;

	void ExpansionJacobianEntry(vector world2A[3], vector world2B[3], vector rel_pos1, vector rel_pos2, vector jointAxis, vector inertiaInvA, float massInvA, vector inertiaInvB, float massInvB)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_7(ExpansionTracing.VEHICLES, this, "ExpansionJacobianEntry").Add(rel_pos1).Add(rel_pos2).Add(jointAxis).Add(inertiaInvA).Add(massInvA).Add(inertiaInvB).Add(massInvB);
#endif

		m_linearJointAxis = jointAxis;

		m_aJ = rel_pos1 * m_linearJointAxis;
		m_bJ = rel_pos2 * -m_linearJointAxis;
		m_aJ = m_aJ.InvMultiply3(world2A);
		m_bJ = m_bJ.InvMultiply3(world2B);

		m_0MinvJt = VectorHelper.Multiply(inertiaInvA, m_aJ);
		m_1MinvJt = VectorHelper.Multiply(inertiaInvB, m_bJ);

		float dAJ = vector.Dot(m_0MinvJt, m_aJ);
		float dBJ = vector.Dot(m_1MinvJt, m_bJ);
		m_Adiag = massInvA + dAJ + massInvB + dBJ;
	}

	float GetDiagonal()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetDiagonal");
#endif

		return m_Adiag;
	}

	float GetRelativeVelocity(vector linvelA, vector angvelA, vector linvelB, vector angvelB)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_4(ExpansionTracing.VEHICLES, this, "GetRelativeVelocity").Add(linvelA).Add(angvelA).Add(linvelB).Add(angvelB);
#endif

		vector linrel = VectorHelper.Multiply(linvelA - linvelB, m_linearJointAxis);

		vector angvela = VectorHelper.Multiply(angvelA, m_aJ);
		vector angvelb = VectorHelper.Multiply(angvelB, m_bJ);

		angvela = angvela + angvelb + linrel;

		float rel_vel = angvela[0] + angvela[1] + angvela[2];

		rel_vel += 0.00001;

		return rel_vel;
	}
};
