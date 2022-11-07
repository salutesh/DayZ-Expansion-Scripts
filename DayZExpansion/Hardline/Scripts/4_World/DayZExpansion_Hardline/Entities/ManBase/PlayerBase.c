/**
 * PlayerBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class PlayerBase
{
	private int m_Reputation;
	
	void SetReputation(int reputation)
	{
		m_Reputation = reputation;
	}
	
	int GetReputation()
	{
		return m_Reputation;
	}
	
   // ------------------------------------------------------------
	// Override EEKilled
	// ------------------------------------------------------------
	override void EEKilled(Object killer)
	{
		if  (GetExpansionSettings().GetHardline().UseReputation)
		{	
			ExpansionHardlineModule hardlineModule = ExpansionHardlineModule.Cast(CF_ModuleCoreManager.Get(ExpansionHardlineModule));
			if (hardlineModule)
			{
				hardlineModule.OnEntityKilled(this, killer);
			}
		}

		super.EEKilled(killer);
	}
};