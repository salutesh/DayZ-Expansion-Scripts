/**
 * ExpansionSupply.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionSupplySignal: M18SmokeGrenade_Purple
{
	override void Unpin()
	{
		super.Unpin();

		if ( IsMissionHost() )
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(Expansion_CallAirdrop, m_FuseDelay * 1000);
		}
	}

	void Expansion_CallAirdrop()
	{
		ExpansionMissionModule.s_Instance.CallAirdrop(GetPosition());
	}
};
