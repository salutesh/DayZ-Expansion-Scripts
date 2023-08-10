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

	ref MapMenu m_Expansion_MapMenu;
	ref ExpansionMapMenu m_Expansion_ExpansionMapMenu;

	private int m_MarkerVisibilityMode;

	void MissionGameplay()
	{
		CF_Modules<ExpansionMarkerModule>.Get(m_MarkerModule);

		PlayerBase.Expansion_RegisterInventoryItemType(ItemMap);
		PlayerBase.Expansion_RegisterInventoryItemType(ExpansionGPS);
		PlayerBase.Expansion_RegisterInventoryItemType(Pen_ColorBase);
		PlayerBase.Expansion_RegisterInventoryItemType(ItemCompass);
	}

	override void Expansion_OnUpdate(float timeslice, PlayerBase player, bool isAliveConscious, Input input, bool inputIsFocused, UIScriptedMenu menu, ExpansionScriptViewMenuBase viewMenu)
	{
		super.Expansion_OnUpdate(timeslice, player, isAliveConscious, input, inputIsFocused, menu, viewMenu);
		
		if (isAliveConscious && !menu && !inputIsFocused)
		{
			//! Map Menu
			if (input.LocalPress("UAExpansionMapToggle", false) && !viewMenu)
			{
				ToggleMapMenu(player);
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
				if (GetExpansionSettings().GetMap().EnableHUDGPS && m_Hud.HUDCloseTime() > 0.75)
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
								m_Hud.SetHasGPSItem(true);
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
					switch (m_MarkerVisibilityMode)
					{
						// Stage 0: "Enabled all 3D Markers" 
						// enable ALL 3D Markers again, doesnt matter which one
						case 0:
						
							GetGame().GetMission().OnEvent(ChatMessageEventTypeID, new ChatMessageEventParams(CCDirect, "", "#STR_EXPANSION_MARKERTOGGLEALL_OFF", ""));
							m_MarkerModule.SetVisibility(ExpansionMapMarkerType.SERVER, EXPANSION_MARKER_VIS_WORLD);
							m_MarkerModule.SetVisibility(ExpansionMapMarkerType.PARTY, EXPANSION_MARKER_VIS_WORLD);
							m_MarkerModule.SetVisibility(ExpansionMapMarkerType.PLAYER, EXPANSION_MARKER_VIS_WORLD);
							m_MarkerModule.SetVisibility(ExpansionMapMarkerType.PERSONAL, EXPANSION_MARKER_VIS_WORLD);
						break;
						// Stage 1: "Disabled all Server 3D Markers" 
						// hide ONLY Server 3D Markers
						case 1:
							GetGame().GetMission().OnEvent(ChatMessageEventTypeID, new ChatMessageEventParams(CCDirect, "", "#STR_EXPANSION_MARKERTOGGLESERVER_ON", ""));
							m_MarkerModule.RemoveVisibility(ExpansionMapMarkerType.SERVER, EXPANSION_MARKER_VIS_WORLD);
						break;
						// Stage 2: "Disabled all Personal Party 3D Markers" 
						// hide ONLY server and all personal markers from the Party (except Member 3D Markers and privat personal 3D Markers)
						// TO ONLY SEE MEMBER 3D AND YOUR OWN PERSONAL 3D MARKERS!
						case 2:
							GetGame().GetMission().OnEvent(ChatMessageEventTypeID, new ChatMessageEventParams(CCDirect, "", "#STR_EXPANSION_MARKERTOGGLEPARTY_ON", ""));
							m_MarkerModule.RemoveVisibility(ExpansionMapMarkerType.PARTY, EXPANSION_MARKER_VIS_WORLD);
						break;
						// Stage 3: "Disabled all Personal 3D Markers" 
						// hide ONLY server and all personal and party markers (except Member 3D Markers)
						// TO ONLY SEE MEMBER 3D MARKERS!
						case 3:
							GetGame().GetMission().OnEvent(ChatMessageEventTypeID, new ChatMessageEventParams(CCDirect, "", "#STR_EXPANSION_MARKERTOGGLEPERSONAL_ON", ""));
							m_MarkerModule.RemoveVisibility(ExpansionMapMarkerType.PERSONAL, EXPANSION_MARKER_VIS_WORLD);
						break;
						// Stage 4: "Disabled all 3D Markers" 
						// hide ALL 3D Markers, doesnt matter which one
						case 4:
							GetGame().GetMission().OnEvent(ChatMessageEventTypeID, new ChatMessageEventParams(CCDirect, "", "#STR_EXPANSION_MARKERTOGGLEALL_ON", ""));
							m_MarkerModule.RemoveVisibility(ExpansionMapMarkerType.SERVER, EXPANSION_MARKER_VIS_WORLD);
							m_MarkerModule.RemoveVisibility(ExpansionMapMarkerType.PARTY, EXPANSION_MARKER_VIS_WORLD);
							m_MarkerModule.RemoveVisibility(ExpansionMapMarkerType.PLAYER, EXPANSION_MARKER_VIS_WORLD);
							m_MarkerModule.RemoveVisibility(ExpansionMapMarkerType.PERSONAL, EXPANSION_MARKER_VIS_WORLD);
						break;
					}

					if (m_Expansion_ExpansionMapMenu)
						m_Expansion_ExpansionMapMenu.GetMarkerList().UpdateToggleStates();

					m_MarkerVisibilityMode++;
					if ( m_MarkerVisibilityMode > 4 )
						m_MarkerVisibilityMode = 0;
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

	void ToggleMapMenu(PlayerBase player)
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
					if (player.HasItemMap() || player.HasItemGPS())
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
					if (player.HasItemMap() || player.HasItemGPS())
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
