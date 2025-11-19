class eAIState_Fighting_Positioning: eAIState
{
	vector m_Position;
	float m_Time;
	float m_MovementDirection;

	override void OnEntry(string Event, ExpansionState From)
	{
		m_Time = 0;
				
		if (unit.eAI_ShouldGetUp())
			unit.Expansion_GetUp();
	}

	override void OnExit(string Event, bool Aborted, ExpansionState To)
	{
		unit.OverrideMovementDirection(false, 0);
	}

	override int OnUpdate(float DeltaTime, int SimulationPrecision)
	{
		if (unit.eAI_IsChangingStance())
			return CONTINUE;
		
		bool wantsLower = false;
		bool wantsRaise = false;
		
		float speed = unit.Expansion_GetMovementSpeed();
		
		int timeSinceLastFire = g_Game.GetTime() - unit.m_eAI_LastFireTime;
		if (unit.m_eAI_LastFireTime > 0 && timeSinceLastFire > unit.m_eAI_TimeBetweenFiring && speed > 0)
			wantsLower = true;
		
		auto target = unit.GetTarget();
		if (target)
		{
			m_Position = target.GetPosition(false);
			unit.m_eAI_DistanceToTargetSq = target.GetDistanceSq(true);
			
			bool shouldBeMeleeing = false;
			
			auto hands = unit.GetHumanInventory().GetEntityInHands();
			auto targetEntity = target.GetEntity();
			auto itemTarget = ItemBase.Cast(targetEntity);
			if (itemTarget && !itemTarget.Expansion_IsMechanicalTrap() && (!itemTarget.Expansion_IsExplosive() || target.GetCachedThreat(true) > 0.4))
			{
				wantsLower = true;
			}
			else if (target.IsCreature() && !target.IsAcuteDanger() && targetEntity.IsDamageDestroyed())
			{
				//! We are hunting for food
				wantsLower = true;
			}
			else if (!hands)
			{
				shouldBeMeleeing = true;
			}
			else if (hands.IsWeapon())
			{
				if (unit.m_eAI_DistanceToTargetSq <= unit.m_eMeleeCombat.eAI_GetRangeSq())
					shouldBeMeleeing = true;
				else if (unit.m_eAI_QueuedShots == 0 && Math.RandomFloat(0.0, 1.0) > 0.4)
					unit.eAI_SelectFireMode(target, unit.m_eAI_DistanceToTargetSq, Weapon_Base.Cast(hands));
			}
			else if (hands.IsMeleeWeapon())
			{
				shouldBeMeleeing = true;
			}
			
			float minDist;
			float minDistSq;
			
			if (shouldBeMeleeing)
			{
				float rangeSq = unit.m_eMeleeCombat.eAI_GetRangeSq();
				
				float raiseRangeSqMult = 1;
				float lowerRangeSqMult = 1;
				
				//! Only adjust range multipliers if not a creature (animal) since animal attacks cannot be blocked
				//! so we want to raise later and lower earlier
				if (!target.IsCreature() && ((!unit.IsRaised() && Math.RandomInt(0, 5) == 2) || (unit.IsRaised() && !unit.IsWeaponRaiseCompleted())))
				{
					if (!unit.IsSprintFull() && (target.IsRaised() || target.IsFighting()))
					{
						//! (range * n)^2 == range^2 * n^2
						raiseRangeSqMult = 2.25;  //! 1.5^2
						lowerRangeSqMult = 9;  //! 3^2
					}
				}
				
				if (unit.m_eAI_DistanceToTargetSq <= rangeSq * raiseRangeSqMult || target.IsUnconscious())
					wantsRaise = true;
				else if (unit.m_eAI_DistanceToTargetSq > rangeSq * lowerRangeSqMult)
					wantsLower = true;
				
				if (target.IsUnconscious())
					minDist = 2.0;
				else
					minDist = 1.0;
			}
			else if (!wantsLower)
			{
				minDist = 2.0;
				if (unit.m_eAI_IsInCover)
					wantsRaise = true;
			}
			
			minDistSq = minDist * minDist;
			
			if (targetEntity && !itemTarget && (unit.m_eAI_DistanceToTargetSq <= minDistSq || (unit.m_eAI_PositionIsFinal && unit.eAI_IsUnreachable(minDist, m_Position))))
			{
				m_Time += DeltaTime;
				if (!m_MovementDirection || m_Time > Math.RandomIntInclusive(1, 3))
				{
					if (Math.RandomIntInclusive(0, 1))
						m_MovementDirection = Math.RandomFloat(135, 180);
					else
						m_MovementDirection = Math.RandomFloat(-135, -180);
				}
				unit.OverrideMovementDirection(true, m_MovementDirection);
				unit.OverrideMovementSpeed(true, 2);
			}
			else
			{
				unit.OverrideMovementDirection(false, 0);
				if (itemTarget && itemTarget.Expansion_IsExplosive() && wantsLower && unit.m_eAI_DistanceToTargetSq < target.GetMinDistanceSq())
					unit.OverrideMovementSpeed(true, 3);
				else 
					unit.OverrideMovementSpeed(false, 0);
				m_Time = 0;
				m_MovementDirection = 0;
			}
			
			if ((!itemTarget && ((hands && hands.IsWeapon()) || target.GetMinDistance() > 0.0 || !target.CanMeleeIfClose())) || (itemTarget && itemTarget.Expansion_IsDanger()))
			{
				unit.OverrideTargetPosition(target);
			}
			else
			{
				unit.OverrideTargetPosition(m_Position);
			}
			
			if (hands && (hands.IsWeapon() || (unit.m_eAI_DistanceToTargetSq <= 100.0 && !hands.IsInherited(Torch))))
			{
				if (hands.HasEnergyManager() && !hands.GetCompEM().IsWorking() && hands.GetCompEM().CanSwitchOn())
				{
					hands.GetCompEM().SwitchOn();
					unit.m_eAI_TypeSwitchedOnDuringCombat = hands.GetType();
					unit.eAI_ForceVisibilityUpdate();
				}
			}
		}
		else
		{
			unit.OverrideMovementDirection(false, 0);
			unit.OverrideMovementSpeed(false, 0);
			unit.OverrideTargetPosition(unit.GetPosition() + unit.GetDirection() * speed * 0.333333);
		}
		
		if (wantsRaise && unit.CanRaiseWeapon())
		{
			unit.RaiseWeapon(true);
		}
		else if (wantsLower || !unit.CanRaiseWeapon())
		{
			unit.RaiseWeapon(false);
			unit.eAI_AdjustStance(unit.m_eAI_LastFireTime, timeSinceLastFire, unit.m_eAI_TimeBetweenFiringAndGettingUp);
		}
		
		return EXIT;
	}
};
