modded class ModsMenuDetailed
{
	ref MainMenu m_MainMenu;
	ref ExpansionNewsfeed m_ExpansionNewsfeed;
	
	override bool OnMouseButtonUp(Widget w, int x, int y, int button)
	{
		super.OnMouseButtonUp(w,x,y,button);
		
		if( w == m_CloseButton )
		{
			m_MainMenu.ShowNewsfeed(true);
			return true;
		}
		return false;
	}
	
	void SetMainMenu(MainMenu menu)
	{
		m_MainMenu = menu;
	}
}