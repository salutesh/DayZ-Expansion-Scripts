/**
 * ExpansionUIManager.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionUIManager
{
	private ref ExpansionScriptViewMenuBase m_CurrentMenu;
	private ref map<string, ExpansionScriptViewMenuBase> m_ActiveMenus;
	
	void ExpansionUIManager()
	{	
		m_CurrentMenu = NULL;
		m_ActiveMenus = new map<string, ExpansionScriptViewMenuBase>;
	}
	
	void SetMenu(ExpansionScriptViewMenuBase view)
	{
		if (m_CurrentMenu)
			m_CurrentMenu = null;
		
		m_CurrentMenu = view;
	}
	
	ExpansionScriptViewMenuBase GetMenu()
	{
		return m_CurrentMenu;
	}
	
	void CloseMenu()
	{
		if (m_CurrentMenu)
		{
			if (m_CurrentMenu.IsVisible())
				m_CurrentMenu.Hide();

			Print("[EXPANSION UI MANAGER]: Removing menu " + m_CurrentMenu + " from UI manager.");
			DestroySVMenu(m_CurrentMenu);
			m_CurrentMenu = NULL;
		}
	}
	
	void CloseAll()
	{
		m_ActiveMenus.Clear();
	}
	
	ExpansionScriptViewMenuBase CreateSVMenu(string viewName)
	{
		ExpansionScriptViewMenuBase viewMenu;
		if (m_ActiveMenus.Find(viewName, viewMenu))
			m_ActiveMenus.Remove(viewName);

		viewMenu = CreateMenuInstance(viewName);

		if (viewMenu)
		{
			SetMenu(viewMenu);
			m_ActiveMenus.Insert(viewName, viewMenu);
			viewMenu.Show();
			Print("[EXPANSION UI MANAGER]: Created menu " + viewMenu + ".");
		}
		
		return viewMenu;
	}
	
	void DestroySVMenu(ExpansionScriptViewMenuBase menu, bool doDelete = true)
	{
		ExpansionScriptViewMenuBase viewMenu;
		if (m_ActiveMenus.Find(menu.ClassName(), viewMenu))
		{
			m_ActiveMenus.Remove(menu.ClassName());
		}
	}
	
	map<string, ExpansionScriptViewMenuBase> GetActiveMenus()
	{
		return m_ActiveMenus;
	}
	
	ExpansionScriptViewMenuBase CreateMenuInstance(string className)
	{
		return ExpansionScriptViewMenuBase.Cast(className.ToType().Spawn());
	}
};
