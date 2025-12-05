class eAIState_Fighting_Melee: eAIState
{
	float m_Time;
	eAITarget m_Target;
	float m_MovementDirection;
	bool m_IsFighting;

	override void OnEntry(string Event, ExpansionState From)
	{
		m_Time = 0;
		m_MovementDirection = 0;
		
		unit.m_eAI_LastFireTime = g_Game.GetTime();
		
		unit.Expansion_GetUp(true);
	}

	override void OnExit(string Event, bool Aborted, ExpansionState To)
	{
		unit.OverrideMovementDirection(false, 0);
		unit.Notify_Melee(false);
		
		if (m_Target)
		{
			m_Target.m_SkipMelee = false;
		}
	}

	override int OnUpdate(float DeltaTime, int SimulationPrecision)
	{
		if (unit.eAI_IsChangingStance())
			return CONTINUE;
		
		if (!m_Target)
			return EXIT;
		
		auto lowPosition = m_Target.GetPosition(false);
		
		m_Time += DeltaTime;
		if (m_Target.IsMechanicalTrap())
			unit.OverrideTargetPosition(m_Target);
		else
			unit.OverrideTargetPosition(lowPosition);
		
		unit.m_eAI_DistanceToTargetSq = m_Target.GetDistanceSq(true);
		
		if (unit.IsFighting())
		{
			m_IsFighting = true;
			
			//! If we are already meleeing, wait until raised again to avoid awkward animation
			if (!unit.IsRaised() || !unit.IsWeaponRaiseCompleted())
			{
				unit.RaiseWeapon(true);
			
				if (m_Time < 0.5)
				{
					//! waiting for the weapon to be raised
					return CONTINUE;
				}
			}
		}
		else
		{
			if (m_IsFighting)
			{
				m_IsFighting = false;
				m_Time = 0;
			}
		}
		
		vector direction = vector.Direction(unit.GetPosition(), lowPosition).Normalized();
		float dot = vector.Dot(unit.GetDirection(), direction);
		
		//! 1 in 5 chance to move backwards if target is player and fighting
		if (m_Target.IsPlayer() && (m_MovementDirection || (m_Target.IsFighting() && Math.RandomInt(0, 5) == 2)) && dot >= 0.5 && m_Time < Math.RandomFloat(0.5, 1.0))
		{
			if (!m_MovementDirection)
			{
				if (Math.RandomIntInclusive(0, 1))
					m_MovementDirection = Math.RandomFloat(135, 180);
				else
					m_MovementDirection = Math.RandomFloat(-135, -180);
			}
			unit.OverrideMovementDirection(true, m_MovementDirection);
			unit.OverrideMovementSpeed(true, 2);
			
			return EXIT;
		}
		
		if (dot < 0.866 && (!m_Target.IsInanimate() || m_Target.IsMechanicalTrap()) && !m_Target.GetEntity().GetParent())
		{
			if (m_Time < Math.RandomFloat(1.5, 3.0))
			{
				if (unit.m_eAI_DistanceToTargetSq < 1.0)
				{
					if (Math.RandomIntInclusive(0, 1))
						m_MovementDirection = Math.RandomFloat(135, 180);
					else
						m_MovementDirection = Math.RandomFloat(-135, -180);
					
					unit.OverrideMovementDirection(true, m_MovementDirection);
					unit.OverrideMovementSpeed(true, 2);
				}
				
				//! waiting for unit to face target
				return EXIT;
			}
		}
		
		unit.Notify_Melee();
		m_Time = 0;
		m_MovementDirection = 0;
		unit.OverrideMovementDirection(false, 0);
		
		return EXIT;
	}

	int Guard()
	{
		if (unit.IsRestrained()) return eAITransition.FAIL;
		
		if (!unit.CanRaiseWeapon() || !unit.eAI_HasLOS()) return eAITransition.FAIL;
		
		m_Target = unit.GetTarget();
		if (!m_Target) return eAITransition.FAIL;

		EntityAI targetEntity = m_Target.GetEntity();
		if (!targetEntity || targetEntity.IsDamageDestroyed()) return eAITransition.FAIL;
		
		if (!m_Target.IsMeleeViable() || m_Target.GetThreat() < 0.4) return eAITransition.FAIL;
		
		if (m_Target.ShouldAvoid()) return eAITransition.FAIL;
		
		if (unit.m_eAI_HasProjectileWeaponInHands)
		{
			if (unit.eAI_SkipMelee("skipping melee", m_Target, false))
				return eAITransition.FAIL;
		}
		
		return eAITransition.SUCCESS;
	}
};
