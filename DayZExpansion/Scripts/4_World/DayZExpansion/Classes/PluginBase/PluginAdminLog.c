/**
 * PluginAdminLog.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class PluginAdminLog
{		
	ref ExpansionKillFeedModule m_KillfeedModule;
		
	// ------------------------------------------------------------
	// PluginAdminLog PlayerKilled
	// ------------------------------------------------------------	
	override void PlayerKilled( PlayerBase player, Object source )  // PlayerBase.c   
	{
		#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
		EXLogPrint("PluginAdminLog::PlayerKilled - Start");
		#endif
		
		super.PlayerKilled( player, source );
		
		if ( GetExpansionSettings().GetNotification().EnableKillFeed && player.GetIdentity() )
		{
			if (!m_KillfeedModule)
				m_KillfeedModule = ExpansionKillFeedModule.Cast(CF_ModuleCoreManager.Get(ExpansionKillFeedModule));
			
			if ( player && source && !m_KillfeedModule.WasHitCheckDone() )
			{
				if ( player == source )	// deaths not caused by another object (starvation, dehydration)
				{
					if (!m_KillfeedModule.OnKilledByCondition( player, source ) )
					{
						m_KillfeedModule.OnPlayerSuicide(player);
					}
				}
				else if ( source.IsWeapon() || source.IsMeleeWeapon() )  // player with weapon
				{	
					m_KillfeedModule.OnKilledByWeapon( player, source );
				}
				else if ( player != source && source.IsInherited( PlayerBase ) ) // player with no weapon
				{			
					m_KillfeedModule.OnKilledByPlayer( player, source );
				}
				else if ( source.IsInherited( ZombieBase ) ) // zombie
				{
					m_KillfeedModule.OnKilledByZombie( player, source );
				}
				else if ( source.IsInherited( AnimalBase ) ) // animal
				{	
					m_KillfeedModule.OnKilledByAnimal( player, source );
				}
			}
		}
		
		#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
		EXLogPrint("PluginAdminLog::PlayerKilled - End");
		#endif
	}
}