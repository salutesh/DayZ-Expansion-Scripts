
class ExpansionJacobianEntry
{
	vector m_linearJointAxis;
	vector m_aJ;
	vector m_bJ;
	vector m_0MinvJt;
	vector m_1MinvJt;

	float m_Adiag;

	void ExpansionJacobianEntry( Matrix3 world2A,
								 Matrix3 world2B,
								 vector rel_pos1,
								 vector rel_pos2,
								 vector jointAxis,
								 vector inertiaInvA,
								 float massInvA,
								 vector inertiaInvB,
								 float massInvB )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionJacobianEntry::ExpansionJacobianEntry - Start");
		#endif
		
		m_linearJointAxis = jointAxis;

		m_aJ = world2A.Multiply( VectorHelper.Multiply( rel_pos1, m_linearJointAxis ) );
		m_bJ = world2B.Multiply( VectorHelper.Multiply( rel_pos2, -m_linearJointAxis ) );

		m_0MinvJt = VectorHelper.Multiply( inertiaInvA, m_aJ );
		m_1MinvJt = VectorHelper.Multiply( inertiaInvB, m_bJ );

		float dAJ = vector.Dot( m_0MinvJt, m_aJ );
		float dBJ = vector.Dot( m_1MinvJt, m_bJ );
		m_Adiag = massInvA + dAJ + massInvB + dBJ;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionJacobianEntry::ExpansionJacobianEntry - End");
		#endif
	}

	float GetDiagonal()
	{
		return m_Adiag;
	}

	float GetRelativeVelocity( vector linvelA,
							  vector angvelA,
							  vector linvelB,
							  vector angvelB )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionJacobianEntry::GetRelativeVelocity - Start");
		#endif
		
		vector linrel = linvelA - linvelB;

		vector angvela = VectorHelper.Multiply( angvelA, m_aJ );
		vector angvelb = VectorHelper.Multiply( angvelB, m_bJ );

		linrel = VectorHelper.Multiply( linrel, m_linearJointAxis );

		angvela += angvelb + linrel;

		float rel_vel = angvela[0] + angvela[1] + angvela[2];

		rel_vel += 0.00001;

		#ifdef EXPANSIONEXPRINT
		EXPrint( "ExpansionJacobianEntry::GetRelativeVelocity - Return: " + rel_vel.ToString() );
		#endif
		
		return rel_vel;
	}
}

