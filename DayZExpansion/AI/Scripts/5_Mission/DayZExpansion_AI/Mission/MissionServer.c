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

		ExpansionAIPatrolManager.InitPatrols();
	}

	override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
	{
		super.InvokeOnConnect(player, identity);

		string factionName = GetExpansionSettings().GetAI().PlayerFactions.GetRandomElement();
		if (factionName)
		{
			eAIFaction faction = eAIFaction.Create(factionName);
			if (faction)
			{
				EXTrace.Print(EXTrace.AI, player, "Setting faction " + faction.ToString());
				eAIGroup group = eAIGroup.GetGroupByLeader(player, true, faction);
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
};
