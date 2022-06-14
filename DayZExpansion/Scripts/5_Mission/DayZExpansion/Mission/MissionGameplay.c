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
	//! PlayerList check
	protected bool m_Expansion_PlayerListTogglePressed;
	
	//! Modules
	ref ExpansionAutorunModule m_AutoRunModule;
	
	// ------------------------------------------------------------
	// Constructor
	// ------------------------------------------------------------
	void MissionGameplay()
	{
		CF_Modules<ExpansionAutorunModule>.Get(m_AutoRunModule);
	}

	// ------------------------------------------------------------
	// OnUpdate
	// ------------------------------------------------------------
	override void OnUpdate( float timeslice )
	{
		super.OnUpdate( timeslice );

		if (!m_bLoaded)
		{
			return;
		}

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
		UIScriptedMenu topMenu = m_UIManager.GetMenu(); 	//! Expansion reference to menu
		PlayerBase playerPB = PlayerBase.Cast(man);	//! Expansion reference to player
		ExpansionScriptViewMenuBase viewMenu = GetDayZExpansion().GetExpansionUIManager().GetMenu();
		ExpansionPlayerListMenu playerListMenu = ExpansionPlayerListMenu.Cast(viewMenu);
		
		if (playerPB && playerPB.GetHumanInventory()) 
		{
			//! Expansion reference to item in hands
			EntityAI itemInHands = playerPB.GetHumanInventory().GetEntityInHands();

			if (playerPB.GetPlayerState() == EPlayerStates.ALIVE && !playerPB.IsUnconscious())
			{
				//TODO: Make ExpansionInputs class and handle stuff there to keep this clean
				if (!topMenu && !inputIsFocused)
				{
					//! Autorun
					if (input.LocalPress("UAExpansionAutoRunToggle", false))
					{
						if (!man.GetParent() && GetExpansionSettings() && GetExpansionSettings().GetGeneral().EnableAutoRun)
						{
							m_AutoRunModule.AutoRun();
						}
					}
																				
					//! Toggle Earplugs
					if (input.LocalPress( "UAExpansionEarplugsToggle", false )  && !viewMenu)
					{
						m_Hud.ToggleEarplugs();
					}
					
					//! Toggle Player list menu
					if (input.LocalPress("UAExpansionPlayerListToggle", false) && !m_Expansion_PlayerListTogglePressed)
					{
						m_Expansion_PlayerListTogglePressed = true;
						OnPlayerListTogglePressed();
					}
					else if (input.LocalRelease("UAExpansionPlayerListToggle", false) || input.LocalValue("UAExpansionPlayerListToggle", false) == 0)
					{
						m_Expansion_PlayerListTogglePressed = false;
					}
				}

				if (m_AutoRunModule)
				{
					//! Autowalk
					if (GetExpansionSettings() && GetExpansionSettings().GetGeneral().EnableAutoRun)
					{
						m_AutoRunModule.UpdateAutoWalk();
					}
					
					//! Stop autorun when different inputs are pressed
					if (!m_AutoRunModule.IsDisabled())
					{
						if (ExpansionStatic.INPUT_FORWARD() || ExpansionStatic.INPUT_BACK() || ExpansionStatic.INPUT_LEFT() || ExpansionStatic.INPUT_RIGHT() || ExpansionStatic.INPUT_STANCE())
						{
							m_AutoRunModule.AutoRun();
						}
					}
				}
			}
			
			//! Nightvision check
			if (GetExpansionSettings() && GetExpansionSettings().GetGeneral().EnableHUDNightvisionOverlay)
			{
				PlayerCheckNV(playerPB);
			}
		}
	}
	
	// ------------------------------------------------------------
	// Expansion PlayerCheckNV
	// ------------------------------------------------------------
	void PlayerCheckNV(PlayerBase player)
	{		
		if (!GetGame())
			return;
		
		if (player && player.GetCurrentCamera())
		{
			private DayZPlayerCameraBase camera = DayZPlayerCameraBase.Cast(GetGame().GetPlayer().GetCurrentCamera());
			if (camera)
			{
				if (camera && camera.IsCameraNV()) 
				{
					if (!m_Hud.GetNVState())
				 		m_Hud.ShowNV(true);
				}
				else
				{
					if (m_Hud.GetNVState())
				 		m_Hud.ShowNV(false);
				}
			}
		}
		
		EntityAI entity;
		NVGoggles googles;
		ItemBase headgear;
		ItemBase eyewear;
		ItemBase handitem;
		
		if (player && player.FindAttachmentBySlotName("Headgear") != null)
			headgear = ItemBase.Cast(player.FindAttachmentBySlotName("Headgear"));
		
		if (player && player.FindAttachmentBySlotName("Eyewear") != null)
			eyewear = ItemBase.Cast(player.FindAttachmentBySlotName("Eyewear"));
		
		if (player && player.GetHumanInventory().GetEntityInHands() != null)
			handitem = ItemBase.Cast(player.GetHumanInventory().GetEntityInHands());
		
		// Nvg - Headgear check
		if (headgear)
		{
			entity = headgear.FindAttachmentBySlotName("NVG");
			if (entity)
			{
				Class.CastTo(googles, entity);
				GetNVBatteryState(googles);
			}
		}
		// Nvg - Eyewear check
		if (eyewear)
		{
			entity = eyewear.FindAttachmentBySlotName("NVG");
			if (entity)
			{
				Class.CastTo(googles, entity);
				GetNVBatteryState(googles);
			}
		}
		// Nvg - In hands check
		if (handitem)
		{
			entity = handitem;
			if (entity)
			{
				Class.CastTo(googles, entity);
				GetNVBatteryState(googles);
			}
		}
	}
	
	// ------------------------------------------------------------
	// Expansion PlayerCheckNV
	// ------------------------------------------------------------
	void GetNVBatteryState(NVGoggles googles)
	{
		if (GetGame().IsDedicatedServer())
		{
			return;
		}

		int energy_percent = 0;
		if (googles && googles.GetCompEM().CanWork())
		{
			energy_percent = googles.GetBatteryEnergy();					
			m_Hud.SetNVBatteryState(energy_percent);
		}
	}
	
	// ------------------------------------------------------------
	// Override OnPlayerListTogglePressed
	// ------------------------------------------------------------	
	void OnPlayerListTogglePressed()
	{
		if (GetExpansionSettings().GetPlayerList().EnablePlayerList)
		{
			ExpansionUIManager uiManager = GetDayZGame().GetExpansionGame().GetExpansionUIManager();	//! Reference to expansion ui manager
			ScriptView menu	= uiManager.GetMenu();																					//! Reference to current opened script view menu
			ExpansionPlayerListMenu playerListMenu = ExpansionPlayerListMenu.Cast(menu);
			if (!playerListMenu)
			{
				uiManager.CreateSVMenu("ExpansionPlayerListMenu");
			} 
			else if (playerListMenu && playerListMenu.IsVisible())
			{
				uiManager.CloseMenu();
			}
		}
	}
	
	// ------------------------------------------------------------
	// Expansion OnKeyPress
	//! Includes key-press commands for some inputs in menus
	// ------------------------------------------------------------
	override void OnKeyPress(int key)
	{
		super.OnKeyPress(key);
		
		//TODO: use Input system instead
		switch (key)
		{
			case KeyCode.KC_PRIOR:
			{
				if (m_Hud.GetEarplugsState())
				{
					GetExpansionClientSettings().EarplugLevel = Math.Clamp(GetExpansionClientSettings().EarplugLevel + 0.01, 0.0, 1.0);
					GetExpansionClientSettings().Save();
					m_Hud.UpdateEarplugs();
				}
				break;
			}
			
			case KeyCode.KC_NEXT:
			{
				if (m_Hud.GetEarplugsState())
				{
					GetExpansionClientSettings().EarplugLevel = Math.Clamp(GetExpansionClientSettings().EarplugLevel - 0.01, 0.0, 1.0);
					GetExpansionClientSettings().Save();
					m_Hud.UpdateEarplugs();
				}
				break;
			}
		}
	}
}
