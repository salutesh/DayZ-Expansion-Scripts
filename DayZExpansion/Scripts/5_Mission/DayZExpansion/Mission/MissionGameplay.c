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
	protected float m_Expansion_NVUpdateTick;
	//! Modules
	protected ref ExpansionAutorunModule m_AutoRunModule;

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
	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);

		if (!m_bLoaded)
			return;

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

		Man man = GetGame().GetPlayer(); //! Refernce to man
		Input input = GetGame().GetInput(); //! Reference to input
		UIScriptedMenu topMenu = m_UIManager.GetMenu(); //! Expansion reference to menu
		PlayerBase playerPB = PlayerBase.Cast(man);	//! Expansion reference to player
		ExpansionScriptViewMenuBase viewMenu = GetDayZExpansion().GetExpansionUIManager().GetMenu();
		ExpansionPlayerListMenu playerListMenu = ExpansionPlayerListMenu.Cast(viewMenu);

		if (playerPB)
		{
			if (playerPB.GetPlayerState() == EPlayerStates.ALIVE && !playerPB.IsUnconscious())
			{
				//TODO: Make ExpansionInputs class and handle stuff there to keep this clean
				if (!topMenu && !inputIsFocused)
				{
					//! Autorun
					if (input.LocalPress("UAExpansionAutoRunToggle", false))
					{
						if (!man.GetParent() && GetExpansionSettings().GetGeneral(false).IsLoaded() && GetExpansionSettings().GetGeneral().EnableAutoRun)
							m_AutoRunModule.AutoRun();
					}

					//! Toggle Earplugs
					if (input.LocalPress( "UAExpansionEarplugsToggle", false )  && !viewMenu)
					{
						if (GetExpansionSettings().GetGeneral(false).IsLoaded() && GetExpansionSettings().GetGeneral().EnableEarPlugs)
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
					if (GetExpansionSettings().GetGeneral(false).IsLoaded() && GetExpansionSettings().GetGeneral().EnableAutoRun)
						m_AutoRunModule.UpdateAutoWalk();

					//! Stop autorun when different inputs are pressed
					if (!m_AutoRunModule.IsDisabled())
					{
						if (ExpansionStatic.INPUT_FORWARD() || ExpansionStatic.INPUT_BACK() || ExpansionStatic.INPUT_LEFT() || ExpansionStatic.INPUT_RIGHT() || ExpansionStatic.INPUT_STANCE())
							m_AutoRunModule.AutoRun();
					}
				}
			}

			//! Nightvision check
			if (GetExpansionSettings().GetGeneral(false).IsLoaded() && GetExpansionSettings().GetGeneral().EnableHUDNightvisionOverlay)
			{
				m_Expansion_NVUpdateTick += timeslice;
				if (m_Expansion_NVUpdateTick > 0.1)
				{
					m_Expansion_NVUpdateTick = 0.0;
					PlayerCheckNV(playerPB);
				}
			}
		}
	}

	// ------------------------------------------------------------
	// Expansion PlayerCheckNV
	// ------------------------------------------------------------
	void PlayerCheckNV(PlayerBase player)
	{
		if (!m_Hud)
			return;

		DayZPlayerCameraBase camera;
		if (Class.CastTo(camera, player.GetCurrentCamera()))
		{
			if (camera.IsCameraNV())
			{
				if (!m_Hud.GetNVState())
					m_Hud.ShowNV(true);
			}
			else
			{
				if (m_Hud.GetNVState())
					m_Hud.ShowNV(false);

				return;
			}
		}

		ItemBase nvItem = player.Expansion_GetNVItem();
		if (nvItem)
			m_Hud.SetNVBatteryState(nvItem.Expansion_GetBatteryEnergy());
	}

	// ------------------------------------------------------------
	// Expansion GetNVBatteryState
	// ------------------------------------------------------------
	void GetNVBatteryState(NVGoggles googles)
	{
		Error("DEPRECATED");
	}

	// ------------------------------------------------------------
	// Override OnPlayerListTogglePressed
	// ------------------------------------------------------------
	void OnPlayerListTogglePressed()
	{
		if (GetExpansionSettings().GetPlayerList().EnablePlayerList)
		{
			ExpansionUIManager uiManager = GetDayZGame().GetExpansionGame().GetExpansionUIManager(); //! Reference to expansion ui manager
			ScriptView menu	= uiManager.GetMenu(); //! Reference to current opened script view menu
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
};
