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
		GetPermissionsManager().RegisterPermission("Admin.Player.Set.ExpansionEntityStorage");
	}

	override int GetRPCMax()
	{
		return Expansion_JMPlayerModuleRPC.EX_COUNT; // needs to have an int higher than the useable ones
	}

	override void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ref ParamsReadContext ctx)
	{
		super.OnRPC(sender, target, rpc_type, ctx);

		switch (rpc_type)
		{
			case Expansion_JMPlayerModuleRPC.RetriveEntityStorageObj:
				RPC_RetriveEntityStorageObj(ctx, sender, target);
			break;
		}
	}

	void RetriveEntityStorageObj(string entityID)
	{
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(entityID);
		rpc.Send(null, Expansion_JMPlayerModuleRPC.RetriveEntityStorageObj, true, null);
	}

	private void Exec_RetriveEntityStorageObj(string entityID, PlayerIdentity ident, JMPlayerInstance instance = null)
	{
		JMPlayerInstance jmPlayer = GetPermissionsManager().GetPlayer(ident.GetId());
		PlayerBase player = PlayerBase.Cast(PlayerBase.ExpansionGetPlayerByIdentity(ident));
		if (player == null)
			return;
		
		EntityAI restoredEntity;
		if (!ExpansionEntityStorageModule.RestoreFromFile(GetEntityStorageDataDirectory() + entityID + ".bin", restoredEntity, null, player))
		{
			Error(ToString() + "::Exec_RetriveEntityStorageObj - Could not restore entity storage item from file " + entityID);
			return;
		}
		
		if (jmPlayer)
			GetCommunityOnlineToolsBase().Log(ident, "Restore entity storage object " + entityID + " [guid=" + jmPlayer.GetGUID() + "]");
	}

	private void RPC_RetriveEntityStorageObj(ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target)
	{
		string entityID;
		if (!ctx.Read(entityID))
			return;

		JMPlayerInstance instance;
		if (!GetPermissionsManager().HasPermission("Admin.Player.Set.ExpansionEntityStorage", senderRPC, instance))
			return;

		Exec_RetriveEntityStorageObj(entityID, senderRPC, instance);
	}
	
	string GetEntityStorageDataDirectory()
	{
		int instance_id = GetGame().ServerConfigGetInt("instanceId");
		return "$mission:storage_" + instance_id + "\\expansion\\entitystorage\\";
	}
}
#endif
