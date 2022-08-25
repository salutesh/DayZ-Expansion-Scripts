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
	private int m_Humanity;
	
	bool IsHero()
	{
		return m_Humanity >= GetExpansionSettings().GetHardline().RankScout;
	}
	
	bool IsBandit()
	{
		return m_Humanity <= GetExpansionSettings().GetHardline().RankKleptomaniac;
	}
	
	void SetHumanity(int humanity)
	{
		m_Humanity = humanity;
	}
	
	int GetHumanity()
	{
		return m_Humanity;
	}
	
   // ------------------------------------------------------------
	// Override EEKilled
	// ------------------------------------------------------------
	override void EEKilled(Object killer)
	{
		if  (GetExpansionSettings().GetHardline().UseHumanity)
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