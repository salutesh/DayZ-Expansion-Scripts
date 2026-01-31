#ifdef DIAG_DEVELOPER
modded class PluginDiagMenuClient
{
	override protected void BindCallbacks()
	{
		super.BindCallbacks();

		DiagMenu.BindCallback(m_ExpansionDiagsEnum_AIClimb, ExpansionAI_Climb);
		DiagMenu.BindCallback(m_ExpansionDiagsEnum_AIVehicle, ExpansionAI_Vehicle);
		DiagMenu.BindCallback(m_ExpansionDiagsEnum_AIServerStats, ExpansionAI_ServerStats);
	}

	static void ExpansionAI_Climb(float value)
	{
		DayZPlayerImplement player;
		Class.CastTo(player, g_Game.GetPlayer());
		
        player.DEBUG_EXPANSION_AI_CLIMB = value;
	}

	static void ExpansionAI_Vehicle(bool state)
	{
		DayZPlayerImplement player;
		Class.CastTo(player, g_Game.GetPlayer());
		
        player.DEBUG_EXPANSION_AI_VEHICLE = state;
	}

	static void ExpansionAI_ServerStats(bool state)
	{
        ExpansionWorld.s_DebugMonitor_ShowServerStats = state;

        auto rpc = GetDayZGame().GetExpansionGame().m_RPCManager.CreateRPC("RPC_DebugMonitor_ShowServerStats");
        rpc.Write(state);
        rpc.Expansion_Send(true);
	}
};
#endif