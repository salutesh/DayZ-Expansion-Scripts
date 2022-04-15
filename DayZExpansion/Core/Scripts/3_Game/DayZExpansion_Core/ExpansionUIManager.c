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
	ref ExpansionScriptViewMenuBase m_CurrentMenu;
	ref map<int, string> m_Menus;
	ref map<int, ExpansionScriptViewMenuBase> m_ActiveMenus;
	
	void ExpansionUIManager()
	{	
		m_CurrentMenu = NULL;
		m_Menus = new map<int, string>;
		m_ActiveMenus = new map<int, ExpansionScriptViewMenuBase>;
	}
	
	void SetMenu(ExpansionScriptViewMenuBase view)
	{
		if (m_CurrentMenu)
		{
			delete m_CurrentMenu;
			m_CurrentMenu = NULL;
		}
		
		m_CurrentMenu = view;
	}
	
	ExpansionScriptViewMenuBase GetMenu()
	{
		return m_CurrentMenu;
	}
	
	void CloseMenu()
	{
		ExpansionScriptViewMenuBase menu;
		if (GetMenu())
		{
			if (GetMenu().IsVisible())
				GetMenu().Hide();
			
			Print("[EXPANSION UI MANAGER]: Deleting menu instance " + GetMenu() + " with id " + GetMenu().GetID() + " from UI manager.");
			
			if (GetActiveMenus().Find(GetMenu().GetID(), menu))
			{
				Print("[EXPANSION UI MANAGER]: Removing menu " + menu + " with id " + GetMenu().GetID() + " from UI manager.");
				DestroySVMenu(GetMenu().GetID());
				m_CurrentMenu = NULL;
			}
		}
	}
	
	void CloseAll()
	{
		foreach (ExpansionScriptViewMenuBase menu: m_ActiveMenus)
		{
			if (menu.IsVisible())
				menu.Hide();
			
			delete menu;
		}
	}
	
	void RegisterMenu(int id, string className)
	{
		string viewName = "";
		if (m_Menus.Find(id, viewName))	
		{
			if (viewName != className)
			{
				Error("[EXPANSION UI MANAGER]: Trying to register new menu " + className + " with id " + id + " to Expansion UI Manager, but there is already a other menu registered with this id [" + viewName + "]. Register failed!");
			}
			else
			{
				Error("[EXPANSION UI MANAGER]: Trying to register new menu " + className + " with id " + id + " to Expansion UI Manager, but the same menu is already registered. Skiped!");
			}
		}
		else
		{
			m_Menus.Insert(id, className);
			Print("[EXPANSION UI MANAGER]: Registered menu " + className + " with id " + id + " in Expansion UI manager.");
		}
	}
	
	ExpansionScriptViewMenuBase CreateSVMenu(int id)
	{
		string viewName;
		ExpansionScriptViewMenuBase viewMenu;
		if (m_Menus.Find(id, viewName))
		{			
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
		
		return viewMenu;
	}
	
	void DestroySVMenu(int id, bool doDelete = true)
	{
		ExpansionScriptViewMenuBase viewMenu;
		if (m_ActiveMenus.Find(id, viewMenu))
		{
			m_ActiveMenus.Remove(id);
			if (doDelete)
				delete viewMenu;
		}
	}
	
	map<int, string> GetMenus()
	{
		return m_Menus;
	}
	
	map<int, ExpansionScriptViewMenuBase> GetActiveMenus()
	{
		return m_ActiveMenus;
	}
	
	ExpansionScriptViewMenuBase CreateMenuInstance(string className)
	{
		return ExpansionScriptViewMenuBase.Cast(className.ToType().Spawn());
	}
};