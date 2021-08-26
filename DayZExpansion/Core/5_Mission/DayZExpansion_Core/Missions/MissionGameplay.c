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
		
		#ifdef EXPANSIONMODVEHICLE
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
	// OnMissionStart
	// ------------------------------------------------------------	
	override void OnMissionStart()
	{
		super.OnMissionStart();
		
		//! Unlock inputs again if not working ?!
		LockInputs(false, false);
		GetUApi().UpdateControls();
		
		#ifdef EDITOR
		GetUApi().GetInputByName("UAExpansionMapToggle").ForceDisable(true);
		GetUApi().GetInputByName("UAExpansionMapDeleteMarker").ForceDisable(true);
		GetUApi().GetInputByName("UAExpansionQuickMarker").ForceDisable(true);
		GetUApi().GetInputByName("UAExpansionQuickMarker").ForceDisable(true);
		GetUApi().GetInputByName("UAExpansionGPSToggle").ForceDisable(true);
		GetUApi().GetInputByName("UAExpansionPlayerListToggle").ForceDisable(true);
		GetUApi().UpdateControls();
		#endif
	}
	
	// ------------------------------------------------------------
	// OnMissionFinish
	// ------------------------------------------------------------
	override void OnMissionFinish()
	{
		super.OnMissionFinish();
		
		//! Unlock inputs again if not working ?!
		LockInputs(false, false);
		GetUApi().UpdateControls();
	}
	
	// ------------------------------------------------------------
	// LockInputs
	// ------------------------------------------------------------	
	void LockInputs(bool state, bool lockMovement = true)
	{
		TIntArray inputIDs = new TIntArray;
		GetUApi().GetActiveInputs(inputIDs);

		TStringArray skip = new TStringArray;
		skip.Insert("UAUIBack");

		if (!lockMovement)
		{
			//! Allow player movement
			skip.Insert("UAMoveForward");
			skip.Insert("UAMoveBack");
			skip.Insert("UAMoveLeft");
			skip.Insert("UAMoveRight");
			skip.Insert("UATurbo");
			skip.Insert("UAWalkRunTemp");
			skip.Insert("UAWalkRunToggle");
		}

		TIntArray skipIDs = new TIntArray;
		foreach (string inputName : skip)
		{
			skipIDs.Insert(GetUApi().GetInputByName(inputName).ID());
		}

		foreach (int inputID : inputIDs)
		{
			if (skipIDs.Find(inputID) == -1)
			{
				GetUApi().GetInputByID(inputID).ForceDisable(state);
			}
		}
	}
};