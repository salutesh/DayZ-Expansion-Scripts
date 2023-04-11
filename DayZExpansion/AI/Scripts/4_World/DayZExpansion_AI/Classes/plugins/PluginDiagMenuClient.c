#ifdef DIAG
modded class PluginDiagMenuClient
{
	override protected void BindCallbacks()
	{
		super.BindCallbacks();

		DiagMenu.BindCallback(m_ExpansionDiagsEnum_AIClimb, ExpansionAI_Climb);
		DiagMenu.BindCallback(m_ExpansionDiagsEnum_AIVehicle, ExpansionAI_Vehicle);
	}

	static void ExpansionAI_Climb(float value)
	{
		DayZPlayerImplement player;
		Class.CastTo(player, GetGame().GetPlayer());
		
        player.DEBUG_EXPANSION_AI_CLIMB = value;
	}

	static void ExpansionAI_Vehicle(bool state)
	{
		DayZPlayerImplement player;
		Class.CastTo(player, GetGame().GetPlayer());
		
        player.DEBUG_EXPANSION_AI_VEHICLE = state;
	}
};
#endif