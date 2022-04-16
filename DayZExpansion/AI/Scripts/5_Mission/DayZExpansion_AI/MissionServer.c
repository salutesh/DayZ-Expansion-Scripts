modded class MissionServer
{
	void MissionServer()
	{
		GetDayZGame().eAICreateManager();

		CF_Log.Info("eAI - Loaded Server Mission");
	}

	override void InvokeOnConnect(PlayerBase player, PlayerIdentity identity)
	{
		super.InvokeOnConnect(player, identity);

		GetDayZGame().eAIManagerGet().InvokeOnConnect(player, identity);
	}
};