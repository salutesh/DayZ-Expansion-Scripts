class ExpansionState_Fighting_FireWeapon: eAIState
{
	float m_Time;
	eAITarget m_Target;
	Weapon_Base m_Weapon;

	override void OnEntry(string Event, ExpansionState From)
	{
		unit.RaiseWeapon(true);
		
		m_Time = 0;
		
		unit.m_eAI_LastFireTime = GetGame().GetTime();
		
		bool adjustStance = unit.eAI_AdjustStance(m_Weapon, unit.m_eAI_DistanceToTargetSq);
		
		#ifdef DIAG
		if (adjustStance)
			EXTrace.Print(EXTrace.AI, unit, "eAI_AdjustStance " + typename.EnumToString(eAIStance, unit.eAI_GetStance()));
		#endif
	}

	override int OnUpdate(float DeltaTime, int SimulationPrecision)
	{
		if (unit.eAI_IsChangingStance())
			return CONTINUE;
		
		if (!m_Target)
			return EXIT;
		
		auto lowPosition = m_Target.GetPosition(false);
		auto aimPosition = lowPosition + m_Target.GetAimOffset();
		
		m_Time += DeltaTime;
		unit.OverrideTargetPosition(m_Target);
		
		if (!unit.IsRaised() || !unit.IsWeaponRaiseCompleted())
		{
			unit.RaiseWeapon(true);
		
			if (m_Time >= 0.5)
			{
				m_Time = 0;
				return EXIT;
			}
		
			// waiting for the weapon to be raised
			return CONTINUE;
		}
		
		auto neck = unit.GetBonePositionWS(unit.GetBoneIndexByName("neck"));
		auto direction = vector.Direction(neck, aimPosition).Normalized();
		auto weapon = unit.GetHumanInventory().GetEntityInHands();
		
		float threshold;
		if (weapon && weapon.ShootsExplosiveAmmo())
			threshold = 0.96875;
		else
			threshold = 0.875;
		
		if (vector.Dot(unit.GetAimDirection(), direction) < threshold)
		{
			if (m_Time >= 0.5)
			{
				m_Time = 0;
				return EXIT;
			}
		
			// waiting for unit to face target
			return CONTINUE;
		}
		
		unit.TryFireWeapon();
		
		return EXIT;
	}
};
