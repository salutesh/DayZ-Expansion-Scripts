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

#ifdef EXPANSIONUI
class ExpansionUIManager
{
	private ref ExpansionScriptViewMenuBase m_CurrentMenu;
	private ref map<string, ExpansionScriptViewMenuBase> m_ActiveMenus;
	
	void ExpansionUIManager()
	{	
		m_CurrentMenu = NULL;
		m_ActiveMenus = new map<string, ExpansionScriptViewMenuBase>;
	}
	
	protected void SetMenu(ExpansionScriptViewMenuBase view)
	{
		if (m_CurrentMenu)
			m_CurrentMenu.Destroy();
		
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
	
	/**
	 * @brief Close all Expansion menus
	 * 
	 * @param includeVanilla If true, close all vanilla menus as well (may include pause menu and menus of other mods, use with care!)
	 */
	void CloseAll(bool includeVanilla = false)
	{
		CloseAllScriptViews();
		CloseAllUIScriptedMenus(includeVanilla);
	}
	
	/**
	 * @brief Close all Expansion ScriptView menus
	 */
	protected void CloseAllScriptViews()
	{
		foreach (ExpansionScriptViewMenuBase menu: m_ActiveMenus)
		{
			if (menu.IsVisible())
				menu.Hide();
			
			menu.Destroy();
		}

		m_ActiveMenus.Clear();
		m_CurrentMenu = NULL;
	}

	/**
	 * @brief Close all Expansion UIScripted menus
	 */
	protected void CloseAllUIScriptedMenus(bool includeVanilla = false)
	{
		UIMenuPanel menu = g_Game.GetUIManager().GetMenu();
		while (menu)
		{
			if (menu.GetParentMenu())
			{
				menu = menu.GetParentMenu();
			}
			else
			{
				if (includeVanilla || (menu.GetID() >= MENU_EXPANSION_MENU_START && menu.GetID() < MENU_EXPANSION_MENU_END))
					menu.Close();

				break;
			}
		}
	}

	ExpansionScriptViewMenuBase CreateSVMenu(string viewName)
	{
		ExpansionScriptViewMenuBase viewMenu;
		if (m_ActiveMenus.Find(viewName, viewMenu))
		{
			m_ActiveMenus.Remove(viewName);
			viewMenu.Destroy();
		}

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
	
	protected void DestroySVMenu(ExpansionScriptViewMenuBase menu, bool doDelete = true)
	{
		ExpansionScriptViewMenuBase viewMenu;
		if (m_ActiveMenus.Find(menu.ClassName(), viewMenu))
		{
			m_ActiveMenus.Remove(menu.ClassName());
			if (doDelete)
				viewMenu.Destroy();
		}
	}
	
	map<string, ExpansionScriptViewMenuBase> GetActiveMenus()
	{
		return m_ActiveMenus;
	}
	
	protected ExpansionScriptViewMenuBase CreateMenuInstance(string className)
	{
		return ExpansionScriptViewMenuBase.Cast(className.ToType().Spawn());
	}
};
#endif
