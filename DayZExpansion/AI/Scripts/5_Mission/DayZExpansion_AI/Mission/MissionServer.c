/**
 * MissionServer.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		MissionServer
 * @brief		
 **/
modded class MissionServer
{	
	// ------------------------------------------------------------
	// MissionServer Constructor
	// ------------------------------------------------------------
	void MissionServer()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GLOBAL, this, "MissionServer");
#endif

		ExpansionHumanLoadout.Init();
	}

	override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
	{
		super.InvokeOnConnect(player, identity);

		if (player.GetGroup())
			return;

		//! ToDo: Need to add a setting for that maybe?!
		string factionName = GetExpansionSettings().GetAI().PlayerFactions.GetRandomElement();
		if (factionName)
		{
			eAIFaction faction = eAIFaction.Create(factionName);
			if (faction)
			{
				if (faction.IsInvincible() || faction.IsObserver() || faction.IsPassive())
				{
					EXError.Warn(player, "Cannot set faction to invincible, observer or passive", {});
				}
				else
				{
					EXTrace.Print(EXTrace.AI, player, "Setting faction " + faction.ToString());
					eAIGroup group = eAIGroup.GetGroupByLeader(player, true, faction);
				}
			}
		}
	}

	override void SyncRespawnModeInfo(PlayerIdentity identity)
	{
		if (!identity)
		{
			EXTrace.Print(EXTrace.AI, this, "SyncRespawnModeInfo - NULL identity - return");
			return;
		}

		super.SyncRespawnModeInfo(identity);
	}

	override void OnMissionLoaded()
	{
		super.OnMissionLoaded();

		//! Init patrols late so mapping already loaded
        GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ExpansionAIPatrolManager.InitPatrols, 10000, false);
		//! According to the community this fix the issue of zombies not aggroing
		//! Dont ask how, dont ask why
	}

	override void PlayerDisconnected( PlayerBase player, PlayerIdentity identity, string uid )
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.AI, this);
#endif

		super.PlayerDisconnected( player, identity, uid );

		if (PlayerBase.Expansion_GetOnlinePlayersCount() == 0)
			eAIGroup.SaveAllPersistentGroups();
	}
};
