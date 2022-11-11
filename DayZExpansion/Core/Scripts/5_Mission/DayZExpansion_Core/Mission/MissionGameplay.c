/**
 * MissionGameplay.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		MissionGameplay
 * @brief		
 **/
modded class MissionGameplay
{
	ref ExpansionUIMenuManager m_EXUIMenuManager;
	
	// ------------------------------------------------------------
	// Constructor
	// ------------------------------------------------------------
	void MissionGameplay()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GLOBAL, this, "MissionGameplay");
#endif
		
		CreateExpansionUIMenuManager();
	}

	// ------------------------------------------------------------
	// Destructor
	// ------------------------------------------------------------
	void ~MissionGameplay()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GLOBAL, this, "~MissionGameplay");
#endif
		
		DestroyExpansionUIMenuManager();
		DestroyNotificationSystem();
	}
	
	// ------------------------------------------------------------
	// Override PlayerControlDisable
	// ------------------------------------------------------------
	override void PlayerControlDisable(int mode)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GLOBAL, this, "PlayerControlDisable");
#endif
	
		switch (mode)
		{
			case INPUT_EXCLUDE_ALL:
			{
				GetUApi().ActivateExclude("menuexpansion");
				GetUApi().UpdateControls();
				break;
			}
		}
		
		super.PlayerControlDisable(mode);
	}

	// ------------------------------------------------------------
	// OnUpdate
	// ------------------------------------------------------------
	override void OnUpdate(float timeslice)
	{	
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GLOBAL, this, "OnUpdate");
#endif
		
		super.OnUpdate(timeslice);
		
		if (!m_bLoaded)
		{
			return;
		}
		
		//! Why is this here?
		#ifdef EXPANSION_WEAPON_DEBUG_OBJECTS
		GetDayZExpansion().OnUpdate( timeslice );
		#endif

		//! Checking for keyboard focus
		bool inputIsFocused = false;
		
		//! Reference to focused windget
		Widget focusedWidget = GetFocus();

		if (focusedWidget)
		{
			if (focusedWidget.ClassName().Contains("EditBoxWidget"))
			{
				inputIsFocused = true;
			} 
			else if (focusedWidget.ClassName().Contains("MultilineEditBoxWidget"))
			{
				inputIsFocused = true;
			}
		}

		Man man = GetGame().GetPlayer(); 	//! Refernce to man
		Input input = GetGame().GetInput(); 	//! Reference to input
		ExpansionScriptViewMenu viewMenu = ExpansionScriptViewMenu.Cast(GetDayZExpansion().GetExpansionUIManager().GetMenu());
		
		//TODO: Make ExpansionInputs class and handle stuff there to keep this clean
		if (viewMenu && viewMenu.IsVisible())
		{
			//! Close current opened expansion script view menu when ESC is pressed
			if (input.LocalPress("UAUIBack", false))
			{
				if (viewMenu && viewMenu.IsVisible() && viewMenu.CanClose())
					GetDayZExpansion().GetExpansionUIManager().CloseMenu();
			}
		}
	}

	// ------------------------------------------------------------
	// Pause
	// ------------------------------------------------------------	
	override void Pause()
	{
		if (GetDayZExpansion().GetExpansionUIManager().GetMenu())
			return;

		super.Pause();
	}
	
	// ------------------------------------------------------------
	// Override CloseAllMenus
	// ------------------------------------------------------------
	override void CloseAllMenus()
	{
		super.CloseAllMenus();
		
		if (GetDayZGame().GetExpansionGame().GetExpansionUIManager().GetMenu())
			GetDayZGame().GetExpansionGame().GetExpansionUIManager().CloseAll();
	}
	
	// ------------------------------------------------------------
	// CreateExpansionUIMenuManager
	// ------------------------------------------------------------	
	void CreateExpansionUIMenuManager()
	{
		ExpansionUIManager exUIManager = GetDayZGame().GetExpansionGame().GetExpansionUIManager();
		if (exUIManager && !m_EXUIMenuManager)
			m_EXUIMenuManager = new ExpansionUIMenuManager(exUIManager);
	}
	
	// ------------------------------------------------------------
	// DestroyExpansionUIMenuManager
	// ------------------------------------------------------------		
	void DestroyExpansionUIMenuManager()
	{
		if (m_EXUIMenuManager)
			m_EXUIMenuManager = null;
	}
	
	// ------------------------------------------------------------
	// GetExpansionUIMenuManager
	// ------------------------------------------------------------	
	ExpansionUIMenuManager GetExpansionUIMenuManager()
	{
		return m_EXUIMenuManager;
	}

	// ------------------------------------------------------------
	// IsMenuOpened
	// ------------------------------------------------------------
	bool IsMenuOpened()
	{
		if (GetDayZGame().GetExpansionGame().GetExpansionUIManager().GetMenu())
			return true;
		
		return false;
	}
	
	// ------------------------------------------------------------
	// OnMissionFinish
	// ------------------------------------------------------------
	override void OnMissionFinish()
	{
		super.OnMissionFinish();
		
		//! Close and destroy any open ScriptView menus. This will return control to the player and unlock all inputs
		ExpansionUIManager manager = GetDayZGame().GetExpansionGame().GetExpansionUIManager();
		ExpansionScriptViewMenuBase menu = manager.GetMenu();
		if (menu)
		{
			manager.CloseMenu();
		}
	}
};
