/**
 * MissionGameplay.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
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

		CreateDayZExpansion();
		
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

		DestroyDayZExpansion();

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
				break;
			}
			
			case INPUT_EXCLUDE_CHAT_EXPANSION:
			{
				GetUApi().ActivateExclude("chatexpansion");
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
		
		// GetDayZExpansion().OnUpdate( timeslice );

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

		#ifdef EXPANSIONMODVEHICLE
		Man man 				= GetGame().GetPlayer(); 	//! Refernce to man
		Input input 			= GetGame().GetInput(); 	//! Reference to input
		UIScriptedMenu topMenu 	= m_UIManager.GetMenu(); 	//! Expansion reference to menu
		PlayerBase playerPB 	= PlayerBase.Cast( man );	//! Expansion reference to player
		
		if ( playerPB )
		{
			HumanCommandVehicle hcv = playerPB.GetCommand_Vehicle();
			if ( hcv && hcv.GetVehicleSeat() == DayZPlayerConstants.VEHICLESEAT_DRIVER )
			{
				CarScript carScript = CarScript.Cast( hcv.GetTransport() );
				if ( carScript )
				{
					carScript.UpdateExpansionController();
				}
			}

			ExpansionHumanCommandVehicle ehcv = playerPB.GetCommand_ExpansionVehicle();
			if ( ehcv && ehcv.GetVehicleSeat() == DayZPlayerConstants.VEHICLESEAT_DRIVER )
			{
				ExpansionVehicleBase expansionVehicleScript = ExpansionVehicleBase.Cast( ehcv.GetTransport() );
				if ( expansionVehicleScript )
				{
					expansionVehicleScript.UpdateExpansionController();
				}
			}
		}
		#endif

		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionGameplay::OnUpdate - End");
		#endif
	}
	
	void CreateExpansionUIMenuManager()
	{
		ref ExpansionUIManager exUIManager = GetDayZGame().GetExpansionGame().GetExpansionUIManager();
		if (exUIManager && !m_EXUIMenuManager)
			m_EXUIMenuManager = new ExpansionUIMenuManager(exUIManager);
	}
	
	void DestroyExpansionUIMenuManager()
	{
		if (m_EXUIMenuManager)
			delete m_EXUIMenuManager;
	}
	
	ExpansionUIMenuManager GetExpansionUIMenuManager()
	{
		return m_EXUIMenuManager;
	}
	
	bool IsMenuOpened()
	{
		if (GetDayZGame().GetExpansionGame().GetExpansionUIManager().GetMenu())
			return true;
		
		return false;
	}
};