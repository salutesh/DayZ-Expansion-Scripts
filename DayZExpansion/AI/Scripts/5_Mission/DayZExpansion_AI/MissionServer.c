modded class MissionServer
{
	void MissionServer()
	{
		CF_Log.Info("eAI - Loaded Server Mission");
	}

	override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
	{
		super.InvokeOnConnect(player, identity);

		GetDayZGame().GetExpansionGame().InvokeOnConnect(player, identity);
	}
};