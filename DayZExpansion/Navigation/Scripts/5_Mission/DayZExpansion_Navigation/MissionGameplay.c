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
	ExpansionMarkerModule m_MarkerModule;

	bool m_Expansion_MarkerToggleState = true;
	bool m_Expansion_ServerMarkerToggleState = true;

	ref MapMenu m_Expansion_MapMenu;
	ref ExpansionMapMenu m_Expansion_ExpansionMapMenu;

	void MissionGameplay()
	{
		Class.CastTo(m_MarkerModule, GetModuleManager().GetModule(ExpansionMarkerModule));
	}

	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);

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

		Man man = GetGame().GetPlayer();				//! Refernce to man
		Input input = GetGame().GetInput();				//! Reference to input
		UIScriptedMenu topMenu = m_UIManager.GetMenu(); //! Expansion reference to menu
		PlayerBase playerPB = PlayerBase.Cast(man);		//! Expansion reference to player
		ExpansionScriptViewMenu viewMenu = ExpansionScriptViewMenu.Cast(GetDayZExpansion().GetExpansionUIManager().GetMenu());
		
		//TODO: Make ExpansionInputs class and handle stuff there to keep this clean
		if (playerPB && playerPB.GetHumanInventory())
		{
			if (playerPB.GetPlayerState() == EPlayerStates.ALIVE && !playerPB.IsUnconscious())
			{
				if (!topMenu && !inputIsFocused)
				{
					//! Map Menu
					if (input.LocalPress("UAExpansionMapToggle", false) && !viewMenu)
					{
						ToggleMapMenu(playerPB);
					}

					//! GPS
					if (input.LocalPress("UAExpansionGPSToggle", false))
					{
						if (GetExpansionSettings() && GetExpansionSettings().GetMap().EnableHUDGPS)
						{
							if (GetExpansionSettings().GetMap().NeedGPSItemForKeyBinding)
							{
								if (PlayerBase.Cast(GetGame().GetPlayer()).HasItemGPS())
								{
									ToggleHUDGPSMode();
								}
							}
							else
							{
								ToggleHUDGPSMode();
							}
						}
					}
					
					if (input.LocalHold("UAExpansionGPSToggle"))
					{
						if (GetExpansionSettings().GetMap().EnableHUDGPS && m_Hud.m_CloseTime > 0.75)
						{
							//! If it's already open, just close it
							if (m_Hud.GetGPSState())
							{
								m_Hud.ToggleHUDGPS();
							}
							else if (!m_Hud.GetGPSState())
							{
								if (GetExpansionSettings().GetMap().NeedGPSItemForKeyBinding)
								{			
									if (PlayerBase.Cast(GetGame().GetPlayer()).HasItemGPS())
									{
										m_Hud.m_HasGPSItem = true;
										m_Hud.ToggleHUDGPS();
									}
								}
								else
								{
									m_Hud.ToggleHUDGPS();
								}
							}
						}
					}
					
					if (input.LocalPress("UAExpansionGPSMapScaleDown", false))
					{
						if (GetExpansionSettings() && GetExpansionSettings().GetMap().EnableHUDGPS && m_Hud.GetGPSMapState())
						{
							DecreaseGPSMapScale();
						}
					}

					if (input.LocalPress("UAExpansionGPSMapScaleUp", false))
					{
						if (GetExpansionSettings() && GetExpansionSettings().GetMap().EnableHUDGPS && m_Hud.GetGPSMapState())
						{
							IncreaseGPSMapScale();
						}
					}

					//! Expansion Compass Hud
					if (input.LocalPress("UAExpansionCompassToggle", false) && m_Hud)
					{
						if (GetExpansionSettings().GetMap().EnableHUDCompass)
						{
							m_Hud.SetCompassToggleState();
						}
					}

					if (m_MarkerModule)
					{
						if (input.LocalPress("UAExpansion3DMarkerToggle", false) && !viewMenu)
						{

							m_Expansion_MarkerToggleState = !m_Expansion_MarkerToggleState;
							m_Expansion_ServerMarkerToggleState = m_Expansion_MarkerToggleState;

							if (m_Expansion_MarkerToggleState)
							{
								ExpansionNotification("STR_EXPANSION_MARKERTOGGLE_TITLE", "STR_EXPANSION_MARKERTOGGLEALL_OFF", EXPANSION_NOTIFICATION_ICON_MARKER, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 5).Info(playerPB.GetIdentity());
								m_MarkerModule.SetVisibility(ExpansionMapMarkerType.SERVER, EXPANSION_MARKER_VIS_WORLD);
								m_MarkerModule.SetVisibility(ExpansionMapMarkerType.PARTY, EXPANSION_MARKER_VIS_WORLD);
								m_MarkerModule.SetVisibility(ExpansionMapMarkerType.PLAYER, EXPANSION_MARKER_VIS_WORLD);
								m_MarkerModule.SetVisibility(ExpansionMapMarkerType.PERSONAL, EXPANSION_MARKER_VIS_WORLD);
							}
							else
							{
								ExpansionNotification("STR_EXPANSION_MARKERTOGGLE_TITLE", "STR_EXPANSION_MARKERTOGGLEALL_ON", EXPANSION_NOTIFICATION_ICON_MARKER, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 5).Info(playerPB.GetIdentity());
								m_MarkerModule.RemoveVisibility(ExpansionMapMarkerType.SERVER, EXPANSION_MARKER_VIS_WORLD);
								m_MarkerModule.RemoveVisibility(ExpansionMapMarkerType.PARTY, EXPANSION_MARKER_VIS_WORLD);
								m_MarkerModule.RemoveVisibility(ExpansionMapMarkerType.PLAYER, EXPANSION_MARKER_VIS_WORLD);
								m_MarkerModule.RemoveVisibility(ExpansionMapMarkerType.PERSONAL, EXPANSION_MARKER_VIS_WORLD);
							}
						}

						if (input.LocalPress("UAExpansionServerMarkersToggle", false) && !viewMenu)
						{
							m_Expansion_ServerMarkerToggleState = !m_Expansion_ServerMarkerToggleState;

							if (m_Expansion_ServerMarkerToggleState)
							{
								ExpansionNotification("STR_EXPANSION_MARKERTOGGLE_TITLE", "STR_EXPANSION_MARKERTOGGLESERVER_OFF", EXPANSION_NOTIFICATION_ICON_MARKER, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 5).Info(playerPB.GetIdentity());
								m_MarkerModule.SetVisibility(ExpansionMapMarkerType.SERVER, EXPANSION_MARKER_VIS_WORLD);
							}
							else
							{
								ExpansionNotification("STR_EXPANSION_MARKERTOGGLE_TITLE", "STR_EXPANSION_MARKERTOGGLESERVER_ON", EXPANSION_NOTIFICATION_ICON_MARKER, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 5).Info(playerPB.GetIdentity());
								m_MarkerModule.RemoveVisibility(ExpansionMapMarkerType.SERVER, EXPANSION_MARKER_VIS_WORLD);
							}
						}
					}
				}
			}
		}
	}

	override void OnMissionFinish()
	{
		super.OnMissionFinish();

		if (m_Expansion_MapMenu)
			m_Expansion_MapMenu.CloseMapMenu(); //! Safely destroys map menu

		if (m_Expansion_ExpansionMapMenu)
			m_Expansion_ExpansionMapMenu.CloseMapMenu(true); //! Safely destroys expansion map menu
	}

	void ToggleMapMenu(PlayerBase playerPB)
	{
		if (!GetExpansionSettings().GetMap().EnableMap)
		{
			bool show_map;
			if (m_Expansion_MapMenu && m_Expansion_MapMenu.IsVisible())
			{
				m_Expansion_MapMenu.CloseMapMenu();
			}
			else if (!GetGame().GetUIManager().GetMenu() && GetExpansionSettings().GetMap() && GetExpansionSettings().GetMap().CanOpenMapWithKeyBinding)
			{
				if (GetExpansionSettings().GetMap().NeedMapItemForKeyBinding)
				{
					if (playerPB.HasItemMap() || playerPB.HasItemGPS())
						show_map = true;
				}
				else
				{
					show_map = true;
				}

				if (show_map)
				{
					if (m_Expansion_MapMenu)
					{
						GetGame().GetUIManager().ShowScriptedMenu(m_Expansion_MapMenu, NULL);
					}
					else
					{
						m_Expansion_MapMenu = MapMenu.Cast(GetGame().GetUIManager().EnterScriptedMenu(MENU_MAP, NULL));
					}
				}
			}
		}
		else
		{
			if (m_Expansion_ExpansionMapMenu && m_Expansion_ExpansionMapMenu.IsVisible())
			{
				m_Expansion_ExpansionMapMenu.CloseMapMenu();
			}
			else if (!GetGame().GetUIManager().GetMenu() && GetExpansionSettings().GetMap() && GetExpansionSettings().GetMap().CanOpenMapWithKeyBinding)
			{
				if (GetExpansionSettings().GetMap().NeedMapItemForKeyBinding)
				{
					if (playerPB.HasItemMap() || playerPB.HasItemGPS())
						show_map = true;
				}
				else
				{
					show_map = true;
				}

				if (show_map)
				{
					if (m_Expansion_ExpansionMapMenu)
					{
						GetGame().GetUIManager().ShowScriptedMenu(m_Expansion_ExpansionMapMenu, NULL);
					}
					else
					{
						m_Expansion_ExpansionMapMenu = ExpansionMapMenu.Cast(GetGame().GetUIManager().EnterScriptedMenu(MENU_EXPANSION_MAP, NULL));
					}
				}
			}
		}
	}

	void DecreaseGPSMapScale()
	{
		if (!m_Hud)
		{
			return;
		}

		float current_scale;
		float new_scale;

		current_scale = m_Hud.GetCurrentGPSMapScale();
		new_scale = (current_scale - 0.1);

		if (new_scale <= 0.1)
			return;

		m_Hud.SetGPSMapScale(new_scale);
	}

	void IncreaseGPSMapScale()
	{
		if (!m_Hud)
		{
			return;
		}
		float current_scale;
		float new_scale;

		current_scale = m_Hud.GetCurrentGPSMapScale();
		new_scale = (current_scale + 0.1);

		if (new_scale >= 0.8)
			return;

		m_Hud.SetGPSMapScale(new_scale);
	}

	/**
	 * @brief Changes GPS Mode
	 */
	void ToggleHUDGPSMode()
	{
		if (!m_Hud)
			return;

		if (!m_Hud.GetGPSMapState() && m_Hud.GetGPSMapStatsState())
		{
			m_Hud.ShowGPSMap(true);
			m_Hud.ShowGPSMapStats(false);
		}
		else if (m_Hud.GetGPSMapState() && !m_Hud.GetGPSMapStatsState())
		{
			m_Hud.ShowGPSMap(false);
			m_Hud.ShowGPSMapStats(true);
		}
	}
	
	/**
	 * @brief Changes GPS visibility
	 */
	void ToggleHUDGPS()
	{
		m_Hud.ToggleHUDGPS();
	}
};
