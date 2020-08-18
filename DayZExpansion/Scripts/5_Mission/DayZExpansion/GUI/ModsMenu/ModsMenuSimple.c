modded class ModsMenuSimple
{
	ref MainMenu m_MainMenu;
	ref ExpansionNewsfeed m_ExpansionNewsfeed;
	
	override void Select( ModInfo mod )
	{
		super.Select(mod);
		
		m_MainMenu.ShowNewsfeed(false);
	}
	
	void SetMainMenu(MainMenu menu)
	{
		m_MainMenu = menu;
	}
}