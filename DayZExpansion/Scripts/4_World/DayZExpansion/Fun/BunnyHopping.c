
modded class ExpansionHumanCommandFall
{
	
	void ExpansionHumanCommandFall(Human pHuman, float pYVelocity, ExpansionHumanST pTable)
	{		
		Class.CastTo( m_Player, pHuman );
		m_Table = pTable;
		m_JumpVelocity = pYVelocity;

		m_Input = m_Player.GetInputController();
		m_IsFalling = true;

		m_LandType = -1;
	}
	
	
	override void PreAnimUpdate( float pDt )
	{

		super.PreAnimUpdate(pDt);

		if ( m_IsJumping )
		{
			if ( m_Time > 1 )
			{
				m_Time = 0;
				m_IsJumping = false;
			}
		}
	}
	
	override void PrePhysUpdate(float pDt)
	{
		super.PrePhysUpdate(pDt);
		
		if (m_IsJumping && m_JumpVelocity > 0)
		{
			PrePhys_SetTranslation(vector.Zero);

			float mass = dBodyGetMass(m_Player);
			vector impulse = Vector(0, m_JumpVelocity - GetVelocity(m_Player)[1], 0);

			IEntity parent = m_Player.GetParent();
			if (parent)
			{
				vector parentVel = GetVelocity(parent);
				if (parentVel[1] < 0)
					parentVel[1] = 0;

				// impulse += parentVel;
			}

			dBodyApplyImpulse(m_Player, impulse * mass);

			m_JumpVelocity = 0;
		}
	}

	
	
}

