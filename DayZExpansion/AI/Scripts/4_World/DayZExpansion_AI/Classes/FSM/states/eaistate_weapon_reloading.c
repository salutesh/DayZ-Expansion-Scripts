class eAIState_Weapon_Reloading: eAIState
{
	override void OnEntry(string Event, ExpansionState From)
	{
		if (unit.GetEmoteManager().IsEmotePlaying())
			unit.GetEmoteManager().ServerRequestEmoteCancel();
	}

	int GuardEx(int lastAttemptTime, inout Weapon_Base weapon, inout Magazine magazine)
	{
		if (g_Game.GetTime() - lastAttemptTime < 1000) return eAITransition.FAIL;
		
		if (unit.IsClimbing() || unit.IsFalling() || unit.IsFighting() || !unit.GetCommand_Move()) return eAITransition.FAIL;
		
		if (unit.IsRestrained()) return eAITransition.FAIL;
		if (unit.IsUnconscious()) return eAITransition.FAIL;
		if (unit.IsSwimming()) return eAITransition.FAIL;
		
		if (!Class.CastTo(weapon, unit.GetItemInHands())) return eAITransition.FAIL;
		
		//! Allow sub-FSM to handle destroyed weapon so it gets dropped
		if (weapon.IsDamageDestroyed()) return eAITransition.SUCCESS;
		
		magazine = null;
		
		if (weapon.Expansion_IsChambered())
		{
			if (!unit.eAI_IsSafeToFillMag()) return eAITransition.FAIL;
			
			int mi = weapon.GetCurrentMuzzle();
			
			if (weapon.HasInternalMagazine(mi))
			{
				//! If this is a gun with internal mag and we are not in combat, reload when mag is below 60% capacity
				//! even if chamber is not empty
				if (weapon.GetInternalMagazineCartridgeCount(mi) >= weapon.GetInternalMagazineMaxCartridgeCount(mi) * 0.6)
					return eAITransition.FAIL;
				
				if (!unit.eAI_HasAmmoForFirearm(weapon, magazine))
					return eAITransition.FAIL;
			}
			else
			{
				//! If this is a gun with attached mag and we are not in combat, reload when mag is empty
				//! even if chamber is not empty
				Magazine currentMag = weapon.GetMagazine(mi);
				if (currentMag)
				{
					int ammoCount = currentMag.GetAmmoCount();
					
					//! Don't reload if attached mag is still at 40% capacity or higher
					if (ammoCount >= currentMag.GetAmmoMax() * 0.4)
						return eAITransition.FAIL;
					
					if (!unit.eAI_HasAmmoForFirearm(weapon, magazine))
						return eAITransition.FAIL;
					
					//! Don't swap magazine if attached mag is not empty and not unlimited reload
					if (!magazine.IsAmmoPile() && ammoCount > 0 && !unit.eAI_IsTargetUnlimitedReload())
						return eAITransition.FAIL;
				}
				else
				{
					if (!unit.eAI_HasAmmoForFirearm(weapon, magazine))
						return eAITransition.FAIL;
					
					if (magazine.IsAmmoPile())
						return eAITransition.FAIL;
				}
			}
		}
		
		if (unit.GetWeaponManager().CanUnjam(weapon)) return eAITransition.FAIL;
		
		// don't move to the state if the action manager is operating
		if (!unit.GetActionManager() || unit.GetActionManager().GetRunningAction()) return eAITransition.FAIL;
		
		return eAITransition.SUCCESS;
	}
};
