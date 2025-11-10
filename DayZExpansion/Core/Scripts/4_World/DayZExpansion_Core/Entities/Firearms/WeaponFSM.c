/**
 * WeaponFSM.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class WeaponFSM
{
	string m_Expansion_ValidateAndRepairQueued;

	//! This is just here so that our custom ErrorEx string including owner is used,
	//! and queued calls to Expansion_ValidateAndRepair are removed.
	//! Other than that, this is exactly equivalent to vanilla ValidateAndRepairHelper
	override protected bool ValidateAndRepairHelper(Weapon_Base weapon, string name, bool stateCondition, bool gunCondition, WeaponEventBase e1, WeaponEventBase e2, out WeaponStableState state)
	{
		if (LogManager.IsWeaponLogEnable()) { wpnDebugPrint("[wpnfsm] " + weapon.GetDebugName(weapon) + " ValidateAndRepair - " + name + " - " + m_State + " - state: " + stateCondition + " & weapon: " + gunCondition); }

		if (m_Expansion_ValidateAndRepairQueued)
		{
			g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(weapon.Expansion_ValidateAndRepair);
			m_Expansion_ValidateAndRepairQueued = "";
		}

		if (stateCondition != gunCondition)
		{
			WeaponStableState repairedState;
			
			// Seeing this message is not TOO bad, it is mostly informative
			// It is simply being listed in the logs to identify how much the FSM state and weapon state still desyncs
			// Which can be because of a myriad of causes, such as incorrectly set up transitions
			// Or simply certain timings of certain actions or interrupts lined up perfectly, which can have repro rates such as 1/300
			ErrorEx(string.Format("[wpnfsm] ValidateAndRepair Attempting to repair: %1 (owner %2) - %3 - %4 - state: %5 != weapon: %6",
				weapon.GetDebugName(weapon), weapon.GetHierarchyRootPlayer(), name, m_State, stateCondition, gunCondition), ErrorExSeverity.INFO);
			
			if (e1 && e2)
				repairedState = ValidateAndRepairStateFinder(gunCondition, e1, e2, state);
			
			if (repairedState)
			{
				Terminate();
				m_State = repairedState;
				Start(null, true);
				state = repairedState;
				weapon.SyncSelectionState(state.HasBullet(), state.HasMagazine());
				repairedState.SyncAnimState();

				if (LogManager.IsWeaponLogEnable()) { wpnDebugPrint("[wpnfsm] " + weapon.GetDebugName(weapon) + " ValidateAndRepair - " + name + " - Result - " + m_State); }
				return true;
			}
			else
			{
				// Last ditch effort
				if (m_SynchronizeAttempts < MAX_SYNCHRONIZE_ATTEMPTS)
				{
					int currentTime = g_Game.GetTime();
					int timeDiff = currentTime - m_LastSynchronizeTime;
					
					// Careful with calling synchronize
					if (timeDiff > MIN_SYNCHRONIZE_INTERVAL)
					{
						// If a lot of time passed since last attempt
						// There is a possibility the weapon was fixed for a period
						if (timeDiff > RESET_SYNCHRONIZE_THRESHOLD)
							m_SynchronizeAttempts = 0;
						
						// Only call this on server or in SP
						// Synchronize will ask the server for its FSM state anyways
						if (g_Game.IsServer())
							weapon.RandomizeFSMState();
						
						weapon.Synchronize();
							
						++m_SynchronizeAttempts;
						m_LastSynchronizeTime = currentTime;
					}
				}
				else
				{
					OnFailThresholdBreached(weapon, name, stateCondition, gunCondition);
				}
			}
		}
		
		return false;
	}

	bool Expansion_ValidateAndRepair(string name)
	{
		m_Expansion_ValidateAndRepairQueued = "";

		WeaponStableState state = WeaponStableState.Cast(m_State);				
		Weapon_Base weapon;
		Man owner;
		if (state)
		{		
			weapon = state.m_weapon;
			owner = weapon.GetHierarchyRootPlayer();

			if (super.Internal_ValidateAndRepair())
			{
				EXError.Info(this, string.Format("Repaired state %1 of %2 (owner %3) - %4", m_State, weapon, owner, name));
				return true;
			}
		#ifdef DIAG_DEVELOPER
			else if (state.IsRepairEnabled())
			{
				EXError.Info(this, string.Format("Validated state %1 of %2 (owner %3) - %4", m_State, weapon, owner, name));
			}
			else
			{
				EXError.Info(this, string.Format("Skipped validating state %1 of %2 (owner %3) - %4", m_State, weapon, owner, name));
			}
		#endif
		}
	#ifdef DIAG_DEVELOPER
		else
		{
			WeaponStateBase baseState = WeaponStateBase.Cast(m_State);
			if (baseState)
			{
				weapon = baseState.m_weapon;
				owner = weapon.GetHierarchyRootPlayer();
			}

			EXError.Info(this, string.Format("Skipped validating unstable state %1 of %2 (owner %3) - %4", m_State, weapon, owner, name));
		}
	#endif

		return false;
	}
}
