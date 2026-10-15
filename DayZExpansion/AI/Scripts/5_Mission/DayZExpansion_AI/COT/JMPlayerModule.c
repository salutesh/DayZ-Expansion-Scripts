/**
 * JMPlayerModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef JM_COT
modded class JMPlayerModule
{
	void JMPlayerModule()
	{
		GetPermissionsManager().RegisterPermission("Admin.Player.Set.ExpansionFaction");
	}

	override int GetRPCMax()
	{
		return Expansion_JMPlayerModuleRPC.EX_COUNT; // needs to have an int higher than the useable ones
	}

	override void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(sender, target, rpc_type, ctx);

		switch (rpc_type)
		{
			case Expansion_JMPlayerModuleRPC.SetFaction:
				RPC_SetExpansionFaction(ctx, sender, target);
			break;
		}
	}

	void SetExpansionFaction(string factionName, array<string> guids)
	{
		int factionTypeID;

		if (factionName)
		{
			typename factionType = eAIFaction.GetType(factionName);
			if (!factionType)
			{
				Error("Invalid faction name " + factionName);
				return;
			}

			factionTypeID = eAIRegisterFaction.s_FactionIDs[factionType];
			if (!factionTypeID)
				return;
		}

		if (IsMissionHost())
		{
			Exec_SetExpansionFaction(factionTypeID, guids, null);
			return;
		}

		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(factionTypeID);
		rpc.Write(guids);
		rpc.Send(null, Expansion_JMPlayerModuleRPC.SetFaction, true, null);
	}

	private void Exec_SetExpansionFaction(int factionTypeID, array<string> guids, PlayerIdentity ident, JMPlayerInstance instance = null)
	{
		typename factionType;

		if (factionTypeID)
			factionType = eAIFaction.GetTypeByID(factionTypeID);

		array<JMPlayerInstance> players = GetPermissionsManager().GetPlayers(guids);

		for (int i = 0; i < players.Count(); i++)
		{
			PlayerBase player = PlayerBase.Cast(players[i].PlayerObject);
			if (player == null)
				continue;

			if (!factionTypeID)
			{
				player.SetGroup(null);

				GetCommunityOnlineToolsBase().Log(ident, "Set Faction To NONE [guid=" + players[i].GetGUID() + "]");

				SendWebhook("Set", instance, "Set " + players[i].FormatSteamWebhook() + " faction to NONE");

				players[i].Update();
			}
			else if (factionType)
			{
				eAIFaction faction = eAIFaction.Cast(factionType.Spawn());
				if (faction)
				{
					if (player.GetGroup())
					{
						player.GetGroup().SetFaction(faction);
					}
					else
					{
						eAIGroup group = eAIGroup.GetGroupByLeader(player, true, faction);
					}

					GetCommunityOnlineToolsBase().Log(ident, "Set Faction To " + faction.GetName() + " [guid=" + players[i].GetGUID() + "]");

					SendWebhook("Set", instance, "Set " + players[i].FormatSteamWebhook() + " faction to " + faction.GetName());

					players[i].Update();
				}
			}
		}
	}

	private void RPC_SetExpansionFaction(ParamsReadContext ctx, PlayerIdentity senderRPC, Object target)
	{
		int factionTypeID;
		if (!ctx.Read(factionTypeID))
			return;

		array<string> guids;
		if (!ctx.Read(guids))
			return;

		JMPlayerInstance instance;
		if (!GetPermissionsManager().HasPermission("Admin.Player.Set.ExpansionFaction", senderRPC, instance))
			return;

		Exec_SetExpansionFaction(factionTypeID, guids, senderRPC, instance);
	}

#ifndef COT_SPECTATOR_TRACKING
	override private void Server_StartSpectating(Object spectateObject, PlayerIdentity ident)
	{
		eAI_RemoveSpectator(ident);

		super.Server_StartSpectating(spectateObject, ident);
	}

	override void Server_OnStartSpectating(Object spectateObject, PlayerIdentity ident)
	{
		super.Server_OnStartSpectating(spectateObject, ident);

		PlayerBase spectator = PlayerBase.Cast(ident.GetPlayer());
		eAIBase ai;
		if (spectator && Class.CastTo(ai, spectateObject))
		{
			if (!ai.m_eAI_Spectators)
				ai.m_eAI_Spectators = new set<PlayerBase>;

			ai.m_eAI_Spectators.Insert(spectator);
		}
	}
#endif

	void StartSpectatingAI(eAIBase ai, PlayerIdentity ident)
	{
	#ifdef SERVER
		Server_StartSpectating(ai, ident);
	#else
		if (!g_Game.IsMultiplayer() || !g_Game.GetPlayer() || !ai)
			return;

		m_SpectatorClient = PlayerBase.Cast(g_Game.GetPlayer());
		m_SpectatorClient.COT_TempDisableOnSelectPlayer();
		m_SpectatorClient.COT_RememberVehicle();
	#endif
	}

#ifdef COT_SPECTATOR_TRACKING
	[Obsolete("No replacement")]
#endif
	PlayerBase eAI_RemoveSpectator(PlayerIdentity ident)
	{
		PlayerBase spectator = PlayerBase.Cast(ident.GetPlayer());

	#ifndef COT_SPECTATOR_TRACKING
		eAIBase ai;
		if (spectator && Class.CastTo(ai, spectator.m_JM_SpectatedObject) && ai.m_eAI_Spectators)
			ai.m_eAI_Spectators.RemoveItem(spectator);
	#endif

		return spectator;
	}

#ifndef COT_SPECTATOR_TRACKING
	override private void Server_EndSpectating(PlayerIdentity ident)
	{
		eAI_RemoveSpectator(ident);

		super.Server_EndSpectating(ident);
	}
#endif
}
#endif
