/**
 * ExpansionEmoteManager.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class EmoteManager
{
	override protected void CommitSuicide()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GLOBAL, this, "CommitSuicide");
#endif

		if (m_Player)
		{
			if (m_Player.Expansion_IsInSafeZone())
			{
				return;
			}
		}

		super.CommitSuicide( );
	}

	override void CreateBleedingEffect(int Callback_ID)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GLOBAL, this, "CreateBleedingEffect");
#endif

		if (m_Player)
		{
			if (m_Player.Expansion_IsInSafeZone())
			{				
				return;
			}
		}
		
		super.CreateBleedingEffect( Callback_ID );
	}

	int Expansion_GetCurrentGesture()
	{
		return m_CurrentGestureID;
	}
};