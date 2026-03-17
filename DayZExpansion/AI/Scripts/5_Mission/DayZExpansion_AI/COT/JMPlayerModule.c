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

#ifdef COT_ANYSPECTATE
	override private void Server_StartSpectating(Object spectateObject, PlayerIdentity ident)
	{
		eAI_RemoveSpectator(ident);

		super.Server_StartSpectating(spectateObject, ident);
	}

	override void Server_OnStartSpectating(Object spectateObject, PlayerIdentity ident)
	{
		PlayerBase spectator = PlayerBase.Cast(ident.GetPlayer());
		eAIBase ai;
		if (spectator && Class.CastTo(ai, spectateObject))
			ai.m_eAI_Spectators.Insert(spectator);
	}
#endif

	void StartSpectatingAI(eAIBase ai, PlayerIdentity ident)
	{
	#ifdef SERVER
	#ifdef COT_ANYSPECTATE
		Server_StartSpectating(ai, ident);
	#else
		PlayerBase playerSpectator = eAI_RemoveSpectator(ident);
		if (!playerSpectator)
			return;

		if (m_Spectators[ident.GetId()] != playerSpectator)
		{
			playerSpectator.COT_RememberVehicle();
			playerSpectator.SetLastPosition();

			m_Spectators[ident.GetId()] = playerSpectator;
		}

		playerSpectator.m_JM_SpectatedPlayer = ai;
		playerSpectator.m_JM_CameraPosition = vector.Zero;

		playerSpectator.COT_TempDisableOnSelectPlayer();

		g_Game.SelectPlayer(ident, null);

		vector position = ai.GetBonePositionWS(ai.GetBoneIndexByName("Head"));
		g_Game.SelectSpectator(ident, "JMSpectatorCamera", position);

		playerSpectator.COTSetGodMode(true, false);  //! Enable godmode and remember previous state of GetAllowDamage
		playerSpectator.COTUpdateSpectatorPosition();

		ScriptRPC rpc = new ScriptRPC();
		int networkLow, networkHigh;
		ai.GetNetworkID(networkLow, networkHigh);
		rpc.Write(networkLow);
		rpc.Write(networkHigh);
		rpc.Send(null, JMPlayerModuleRPC.StartSpectating, true, ident);

		ai.m_eAI_Spectators.Insert(playerSpectator);

		GetCommunityOnlineToolsBase().Log(ident, "Spectating AI " + ai);
	#endif
	#else
		if (!g_Game.IsMultiplayer() || !g_Game.GetPlayer() || !ai)
			return;

		m_SpectatorClient = PlayerBase.Cast(g_Game.GetPlayer());
		m_SpectatorClient.COT_TempDisableOnSelectPlayer();
		m_SpectatorClient.COT_RememberVehicle();
	#endif
	}

	PlayerBase eAI_RemoveSpectator(PlayerIdentity ident)
	{
		PlayerBase spectator = PlayerBase.Cast(ident.GetPlayer());
		eAIBase ai;
	#ifdef COT_ANYSPECTATE
		if (spectator && Class.CastTo(ai, spectator.m_JM_SpectatedObject))
	#else
		if (spectator && Class.CastTo(ai, spectator.m_JM_SpectatedPlayer))
	#endif
			ai.m_eAI_Spectators.RemoveItem(spectator);

		return spectator;
	}

	override private void Server_EndSpectating(PlayerIdentity ident)
	{
		eAI_RemoveSpectator(ident);

		super.Server_EndSpectating(ident);
	}
}
#endif
