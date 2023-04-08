#ifdef DIAG
modded class PluginDiagMenu
{
	protected string m_ExpansionTextDiagsAIMenu         = "EXP AI";
    
	// AI
	protected int m_ExpansionDiagsRootMenu_AI;
		protected int m_ExpansionDiagsEnum_AIClimb;
		protected int m_ExpansionDiagsEnum_AIVehicle;
	
	override protected void RegisterModdedDiagsIDs()
	{
		super.RegisterModdedDiagsIDs();
		
		// AI
		m_ExpansionDiagsRootMenu_AI = GetModdedDiagID();
			m_ExpansionDiagsEnum_AIClimb = GetModdedDiagID();
			m_ExpansionDiagsEnum_AIVehicle = GetModdedDiagID();
	}
	
	override protected void RegisterModdedDiags()
	{
		super.RegisterModdedDiags();

		// AI
		DiagMenu.RegisterMenu(m_ExpansionDiagsRootMenu_AI, m_ExpansionTextDiagsAIMenu, GetModdedRootMenu());
		{
			DiagMenu.RegisterRange(m_ExpansionDiagsEnum_AIClimb, "", "AI CLIMB", m_ExpansionDiagsRootMenu_AI,"0,0x10,0,0x01");
			DiagMenu.RegisterBool(m_ExpansionDiagsEnum_AIVehicle, "", "AI VEHICLE", m_ExpansionDiagsRootMenu_AI);
		}
	}
};
#endif