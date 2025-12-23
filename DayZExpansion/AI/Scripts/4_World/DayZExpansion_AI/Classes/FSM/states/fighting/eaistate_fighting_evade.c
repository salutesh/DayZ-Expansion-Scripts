class eAIState_Fighting_Evade: eAIState
{
	override void OnEntry(string Event, ExpansionState From)
	{
		unit.eAI_ForceSideStep(Math.RandomFloat(0.3, 0.5), null, 0.0, false);
		unit.OverrideMovementSpeed(true, 3);
		auto target = unit.GetTarget();
		if (target)
			unit.OverrideTargetPosition(target);
	}

	override void OnExit(string Event, bool Aborted, ExpansionState To)
	{
		unit.OverrideMovementSpeed(false, 0);
	}

	override int OnUpdate(float DeltaTime, int SimulationPrecision)
	{
		if (unit.eAI_IsSideStepping())
			return CONTINUE;
		
		return EXIT;
	}

	int Guard()
	{
		int missionTime = g_Game.GetTime();
		
		Weapon_Base aiWeapon;
		if (Class.CastTo(aiWeapon, unit.GetHumanInventory().GetEntityInHands()) && missionTime - unit.m_eAI_LastEvadeTime < Math.RandomInt(2500, 5000)) return eAITransition.FAIL;
		
		auto target = unit.GetTarget();
		if (!target) return eAITransition.FAIL;
		
		auto targetPlayer = DayZPlayerImplement.Cast(target.GetEntity());
		if (!targetPlayer) return eAITransition.FAIL;
		
		if (!targetPlayer.IsRaised()) return eAITransition.FAIL;  //! Ignore if target player isn't raised
		
		if (!target.m_LOS) return eAITransition.FAIL;
		
		auto weapon = Weapon_Base.Cast(targetPlayer.GetHumanInventory().GetEntityInHands());
		if (!weapon) return eAITransition.FAIL;  //! Ignore if target player doesn't have a firearm
		
		vector fromTargetDirection = vector.Direction(targetPlayer.GetPosition(), unit.GetPosition()).Normalized();
		float fromTargetDot = vector.Dot(targetPlayer.Expansion_GetAimDirection(), fromTargetDirection);
		if (fromTargetDot < 0.97) return eAITransition.FAIL;  //! Ignore if target player isn't aiming at us
		
		if (Math.RandomInt(0, 5) > 0) return eAITransition.FAIL;  //! 1 in 5 chance to evade
		
		if (unit.eAI_IsDangerousAltitude()) return eAITransition.FAIL;  //! Don't evade if high above ground
		
		if (unit.m_eAI_IsOnLadder) return eAITransition.FAIL;
		
		unit.m_eAI_LastEvadeTime = missionTime;
		
		return eAITransition.SUCCESS;
	}
};
