/**
 * ExpansionUIManager.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/
#ifdef DABS_FRAMEWORK
class ExpansionUIManager
{
	ref ExpansionScriptViewMenu m_CurrentMenu;
	ref map<int, string> m_Menus;
	ref map<int, ExpansionScriptViewMenu> m_ActiveMenus;
	
	void ExpansionUIManager()
	{	
		m_CurrentMenu = NULL;
		m_Menus = new map<int, string>;
		m_ActiveMenus = new map<int, ExpansionScriptViewMenu>;
	}
	
	void SetMenu(ExpansionScriptViewMenu view)
	{
		if (m_CurrentMenu)
		{
			delete m_CurrentMenu;
			m_CurrentMenu = NULL;
		}
		
		m_CurrentMenu = view;
	}
	
	ref ExpansionScriptViewMenu GetMenu()
	{
		return m_CurrentMenu;
	}
	
	void CloseMenu()
	{
		ref ExpansionScriptViewMenu menu;
		if (m_ActiveMenus.Find(m_CurrentMenu.GetID(), menu))
		{
			Print("[EXPANSION UI MANAGER]: Removing menu " + menu + " with id " + m_CurrentMenu.GetID() + " from UI manager.");
			m_ActiveMenus.Remove(m_CurrentMenu.GetID());
		}
			
		if (m_CurrentMenu)
		{
			if (m_CurrentMenu.IsVisible())
				m_CurrentMenu.Hide();
			
			Print("[EXPANSION UI MANAGER]: Deleting menu instance " + m_CurrentMenu + " with id " + m_CurrentMenu.GetID() + " from UI manager.");
			delete m_CurrentMenu;
		}
	}
	
	void CloseAll()
	{
		foreach (ref ExpansionScriptViewMenu menu: m_ActiveMenus)
		{
			if (menu.IsVisible())
				menu.Hide();
			
			delete menu;
		}
	}
	
	void RegisterMenu(int id, string className)
	{
		string viewName;
		if (!m_Menus.Find(id, viewName))
		{
			if (viewName != className)
			{
				m_Menus.Insert(id, className);
				Print("[EXPANSION UI MANAGER]: Registered menu " + className + " with id " + id + " to UI manager.");
			}
		}
	}
	
	void CreateSVMenu(int id)
	{
		string viewName;
		if (m_Menus.Find(id, viewName))
		{
			ref ExpansionScriptViewMenu viewMenu;
			
			if (!m_ActiveMenus.Find(id, viewMenu))
			{
				viewMenu = CreateMenuInstance(viewName);
			}
			else
			{
				delete viewMenu;
				viewMenu = CreateMenuInstance(viewName);
			}
			
			if (viewMenu)
			{
				viewMenu.SetID(id);
				SetMenu(viewMenu);
				m_ActiveMenus.Insert(id, viewMenu);
				viewMenu.Show();
				Print("[EXPANSION UI MANAGER]: Created menu " + viewMenu + " with id " + id + ".");
			}
		}
	}
	
	ref map<int, string> GetMenus()
	{
		return m_Menus;
	}
	
	ref map<int, ExpansionScriptViewMenu> GetActiveMenus()
	{
		return m_ActiveMenus;
	}
	
	ExpansionScriptViewMenu CreateMenuInstance(string className)
	{
		return ExpansionScriptViewMenu.Cast(className.ToType().Spawn());
	}
};
#endif