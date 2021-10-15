/**
 * MissionGameplay.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
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
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::MissionGameplay - Start");
		#endif

		GetExpansionSettings().GameInit();
		
		if ( IsMissionClient() )
			GetExpansionClientSettings().Load();

		if ( !IsMissionOffline() && g_exGlobalSettings )
			g_exGlobalSettings.Unload();
		
		CreateExpansionUIMenuManager();

		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::MissionGameplay - End");
		#endif
	}

	// ------------------------------------------------------------
	// Destructor
	// ------------------------------------------------------------
	void ~MissionGameplay()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::~MissionGameplay - Start");
		#endif

		if (g_exGlobalSettings)
			g_exGlobalSettings.Unload();
		
		DestroyExpansionUIMenuManager();

		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::~MissionGameplay - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Override PlayerControlDisable
	// ------------------------------------------------------------
	override void PlayerControlDisable(int mode)
	{
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
	override void OnUpdate( float timeslice )
	{	
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::OnUpdate - Start");
		#endif
				
		super.OnUpdate(timeslice);
		
		if ( !m_bLoaded )
		{
			#ifdef EXPANSIONEXPRINT
			EXPrint("MissionGameplay::OnUpdate - End");
			#endif

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

		if ( focusedWidget )
		{
			if ( focusedWidget.ClassName().Contains( "EditBoxWidget" ) )
			{
				inputIsFocused = true;
			} 
			else if ( focusedWidget.ClassName().Contains( "MultilineEditBoxWidget" ) )
			{
				inputIsFocused = true;
			}
		}

		Man man 							= GetGame().GetPlayer(); 	//! Refernce to man
		Input input 						= GetGame().GetInput(); 	//! Reference to input
		UIScriptedMenu topMenu 				= m_UIManager.GetMenu(); 	//! Expansion reference to menu
		PlayerBase playerPB 				= PlayerBase.Cast( man );	//! Expansion reference to player		
		ExpansionScriptViewMenu viewMenu 	= ExpansionScriptViewMenu.Cast(GetDayZExpansion().GetExpansionUIManager().GetMenu());
		
		if (viewMenu && viewMenu.IsVisible())
		{
			//! Close current opened expansion script view menu when ESC is pressed
			if (input.LocalPress("UAUIBack", false))
			{
				if (viewMenu && viewMenu.IsVisible())
					GetDayZExpansion().GetExpansionUIManager().CloseMenu();
			}
		}
					
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::OnUpdate - End");
		#endif
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
			delete m_EXUIMenuManager;
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