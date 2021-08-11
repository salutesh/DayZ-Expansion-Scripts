/**
 * ExpansionEmoteManager.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class EmoteManager
{
	override protected void CommitSuicide()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("EmoteManager::CommitSuicide Start");
		#endif

		if (m_Player)
		{
			if (m_Player.IsInSafeZone())
			{
				#ifdef EXPANSIONEXPRINT
				EXPrint("EmoteManager::CommitSuicide End");
				#endif

				return;
			}
		}

		super.CommitSuicide( );

		#ifdef EXPANSIONEXPRINT
		EXPrint("EmoteManager::CommitSuicide End");
		#endif
	}

	override void CreateBleedingEffect(int Callback_ID)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("EmoteManager::CreateBleedingEffect Start");
		#endif

		if (m_Player)
		{
			if (m_Player.IsInSafeZone())
			{
				#ifdef EXPANSIONEXPRINT
				EXPrint("EmoteManager::CreateBleedingEffect End");
				#endif
				
				return;
			}
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("EmoteManager::CreateBleedingEffect End");
		#endif
		
		super.CreateBleedingEffect( Callback_ID );
	}
};