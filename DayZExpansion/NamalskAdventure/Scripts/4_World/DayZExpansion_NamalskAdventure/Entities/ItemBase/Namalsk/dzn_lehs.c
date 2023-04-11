/**
 * dzn_lehs.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef NAMALSK_SURVIVAL
modded class dzn_lehs
{
	protected ExpansionCircuitBoardBase m_CircuitBoard;
	
	override void RefreshLEHSState(EntityAI item, bool state)
	{
		super.RefreshLEHSState(item, state);
		
		if (ExpansionCircuitBoardBase.Cast(item))
		{
			if (state)
			{
				m_CircuitBoard = ExpansionCircuitBoardBase.Cast(item);
			}
			else
			{
				m_CircuitBoard = null;
			}
		}
	}
	
	ExpansionCircuitBoardBase GetCircuitBoard()
	{
		return m_CircuitBoard;
	}
	
	override void EEHealthLevelChanged(int oldLevel, int newLevel, string zone)
	{
		super.EEHealthLevelChanged(oldLevel, newLevel, zone);
		
		//! Only on server
		if (GetGame().IsClient() || !GetGame().IsMultiplayer())
			return;
		
		//! Destroy attached circuit board if suit health state reached STATE_RUINED. 
		if (newLevel == GameConstants.STATE_RUINED)
		{
			if (GetCircuitBoard())
			{
				int health = GetCircuitBoard().GetHealth();
				GetCircuitBoard().AddHealth("", "", -health);
			}
		}
	}
};

modded class dzn_lehs_helmet
{
	bool HasCircuitBoard()
	{
		if (GetSuit() && !IsRuined())
		{
			if (GetSuit().GetCircuitBoard())
			{
				if (!GetSuit().GetCircuitBoard().IsRuined())
				{
					return true;
				}
			}
		}

		return false;
	}
};
#endif