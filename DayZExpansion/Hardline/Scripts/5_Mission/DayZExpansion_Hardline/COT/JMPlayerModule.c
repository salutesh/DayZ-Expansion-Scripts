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
		GetPermissionsManager().RegisterPermission("Admin.Player.Set.ExpansionReputation");
	}

	override int GetRPCMax()
	{
		return Expansion_JMPlayerModuleRPC.EX_COUNT; // needs to have an int higher than the useable ones
	}

#ifdef CF_BUGFIX_REF
	override void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx)
#else
	override void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ref ParamsReadContext ctx)
#endif
	{
		super.OnRPC(sender, target, rpc_type, ctx);

		switch (rpc_type)
		{
			case Expansion_JMPlayerModuleRPC.SetReputation:
				RPC_SetExpansionReputation(ctx, sender, target);
			break;
		}
	}

	void SetExpansionReputation(int reputation, array<string> guids)
	{
		if (IsMissionHost())
		{
			Exec_SetExpansionReputation(reputation, guids, null);
			return;
		}

		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(reputation);
		rpc.Write(guids);
		rpc.Send(null, Expansion_JMPlayerModuleRPC.SetReputation, true, null);
	}

	private void Exec_SetExpansionReputation(int reputation, array<string> guids, PlayerIdentity ident, JMPlayerInstance instance = null)
	{
		array<JMPlayerInstance> players = GetPermissionsManager().GetPlayers(guids);

		for (int i = 0; i < players.Count(); i++)
		{
			PlayerBase player = PlayerBase.Cast(players[i].PlayerObject);
			if (player == null)
				continue;

			player.Expansion_SetReputation(reputation);

			GetCommunityOnlineToolsBase().Log(ident, "Set Reputation To " + reputation + " [guid=" + players[i].GetGUID() + "]");

			SendWebhook("Set", instance, "Set " + players[i].FormatSteamWebhook() + " reputation to " + reputation);

			players[i].Update();
		}
	}

	private void RPC_SetExpansionReputation(ParamsReadContext ctx, PlayerIdentity senderRPC, Object target)
	{
		int reputation;
		if (!ctx.Read(reputation))
			return;

		array<string> guids;
		if (!ctx.Read(guids))
			return;

		JMPlayerInstance instance;
		if (!GetPermissionsManager().HasPermission("Admin.Player.Set.ExpansionReputation", senderRPC, instance))
			return;

		Exec_SetExpansionReputation(reputation, guids, senderRPC, instance);
	}
}
#endif
