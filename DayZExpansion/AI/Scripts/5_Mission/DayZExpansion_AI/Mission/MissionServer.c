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
#ifdef EXTRACE_DIAG
	bool m_eAI_PlayerConnected;
#endif

	// ------------------------------------------------------------
	// MissionServer Constructor
	// ------------------------------------------------------------
	void MissionServer()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GLOBAL, this, "MissionServer");
#endif

		ExpansionHumanLoadout.Init();

		PlayerBase.Expansion_RegisterInventoryItemType(ItemCompass);
		PlayerBase.Expansion_RegisterInventoryItemType(ItemMap);
		PlayerBase.Expansion_RegisterInventoryItemType(GPSReceiver);
	#ifdef EXPANSIONMODNAVIGATION
		PlayerBase.Expansion_RegisterInventoryItemType(ExpansionGPS);
	#endif
	}

	override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
	{
		super.InvokeOnConnect(player, identity);

	#ifdef EXTRACE_DIAG
		m_eAI_PlayerConnected = true;
	#endif

		//! Get players. Vanilla does this in TickScheduler (which we override),
		//! but it's more efficient to do it only when player count actually changes.
		//! We don't use this array, this is just to avoid potential compat issues
		//! with 3rd party mods that may expect it to be filled
		eAI_GetPlayers();

		eAIDynamicPatrol.LoadBalancing_Schedule();

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

	override void OnMissionStart()
	{
		super.OnMissionStart();

		Expansion_CreateAINoGoAreas();
	}

	void Expansion_CreateAINoGoAreas()
	{
		auto excludedAreas = GetExpansionSettings().GetAILocation().NoGoAreas;

		foreach (auto areaConfig: excludedAreas)
		{
			ExpansionAINoGoArea area;
			if (Class.CastTo(area, g_Game.CreateObjectEx("ExpansionAINoGoArea", areaConfig.Position, ECE_NONE)))
				area.Expansion_Init(areaConfig);
		}
	}

	override void OnMissionLoaded()
	{
		super.OnMissionLoaded();

		//! Init patrols late so mapping already loaded
        g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(ExpansionAIPatrolManager.InitPatrols, 10000, false);
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

		eAIDynamicPatrol.LoadBalancing_Schedule();
	}

#ifdef EXTRACE_DIAG
	override void TickScheduler(float timeslice)
	{
		EXTrace trace;
		if (m_eAI_PlayerConnected)
			trace = EXTrace.Profile(EXTrace.AI_PROFILE, this, "TickScheduler");

		super.TickScheduler(timeslice);
	}
#endif

	override void eAI_GetPlayers()
	{
		g_Game.GetWorld().GetPlayerList(m_Players);
	}
};
