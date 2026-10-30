#ifdef DIAG_DEVELOPER
modded class PluginDiagMenu
{
	protected int m_Expansion_Core_RootMenuID;
	static int EXPANSION_SHOW_ACTION_CLASSNAME;
	
	override protected void RegisterModdedDiagsIDs()
	{
		super.RegisterModdedDiagsIDs();

		m_Expansion_Core_RootMenuID = GetModdedDiagID();
		EXPANSION_SHOW_ACTION_CLASSNAME = GetModdedDiagID();
	}

	override protected void RegisterModdedDiags()
	{
		super.RegisterModdedDiags();

		DiagMenu.RegisterMenu(m_Expansion_Core_RootMenuID, "EXP CORE", GetModdedRootMenu());
		DiagMenu.RegisterBool(EXPANSION_SHOW_ACTION_CLASSNAME, "", "Show action classname", m_Expansion_Core_RootMenuID);
	}
}
#endif
