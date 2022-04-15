/**
 * IngameHud.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class IngameHud
{
	Widget m_GPSPanel;
	Widget m_MapStatsPanel;
	Widget m_MapPosPanel;
	TextWidget m_PlayerPosVal;
	Widget m_PlayerPosPanel;
	TextWidget m_PlayerALTVal;
	Widget m_PlayerALTPanel;
	TextWidget m_PlayerDirVal;
	Widget m_PlayerDirPanel;

	Widget m_GPSMapPanel;
	Widget m_MapMarkerPanel;
	MapWidget m_MapWidget;
	ref ExpansionMapMarkerPlayerArrow m_PlayerArrowMarker;

	float m_GPSMapScale = 0.1;

	Widget m_CompassPanel;
	ImageWidget m_CompassImage;
	bool m_AddedCompassSettings;

	bool m_ExpansionGPSState;
	bool m_ExpansionGPSMapState;
	bool m_ExpansionGPSMapStatsState;
	int m_ExpansionGPSPosSetting;
	bool m_ExpansionGPSSetting;

	bool m_ExpansionCompassSetting;
	bool m_ExpansionCompassState;
	bool m_ExpansionCompassDesiredState;

	bool m_HasGPSItem;
	float m_CloseTime = 0;
	bool m_GPSWasOpened = false;

	bool m_HasGPSForCompassItem;
	bool m_HasCompassItem;
	bool m_CompassWasOpened;
	PlayerBase m_Player;
	bool m_ExpansionCompassToggle;

	autoptr ExpansionLocatorUI m_Expansion_LocatorUI;

	void IngameHud()
	{
		m_AddedCompassSettings = false;
		m_ExpansionCompassState = false;
		m_ExpansionCompassDesiredState = true;

		m_HasGPSItem = false;
		m_HasGPSForCompassItem = false;
		m_HasCompassItem = false;
		m_ExpansionCompassToggle = true;

		GetExpansionClientSettings().SI_UpdateSetting.Insert(RefreshHudVisibility);
	}

	void ~IngameHud()
	{
		GetExpansionClientSettings().SI_UpdateSetting.Remove(RefreshHudVisibility);
	}

	override void Init(Widget hud_panel_widget)
	{
		super.Init(hud_panel_widget);

		m_GPSPanel = GetGame().GetWorkspace().CreateWidgets("DayZExpansion/Navigation/GUI/layouts/expansion_gps.layout");
		if (m_GPSPanel)
		{
			m_GPSPanel.Show(false);

			m_MapStatsPanel = Widget.Cast(m_GPSPanel.FindAnyWidget("MapStatsPanel"));
			m_MapPosPanel = Widget.Cast(m_GPSPanel.FindAnyWidget("cursor_position"));
			m_PlayerPosVal = TextWidget.Cast(m_GPSPanel.FindAnyWidget("player_position_value"));
			m_PlayerPosPanel = Widget.Cast(m_GPSPanel.FindAnyWidget("player_position"));
			m_PlayerALTVal = TextWidget.Cast(m_GPSPanel.FindAnyWidget("player_alt_value"));
			m_PlayerALTPanel = Widget.Cast(m_GPSPanel.FindAnyWidget("player_alt"));
			m_PlayerDirVal = TextWidget.Cast(m_GPSPanel.FindAnyWidget("player_direction_value"));
			m_PlayerDirPanel = Widget.Cast(m_GPSPanel.FindAnyWidget("player_direction"));

			m_GPSMapPanel = Widget.Cast(m_GPSPanel.FindAnyWidget("GPSMapPanel"));
			m_MapMarkerPanel = Widget.Cast(m_GPSPanel.FindAnyWidget("MapMarkerPanel"));
			m_MapWidget = MapWidget.Cast(m_GPSPanel.FindAnyWidget("Map"));

			//! Player arrow needs to be always created, as we don't have access to server settings on client side
			//! when the HUD is created for the first time to check if it's needed or not
			m_PlayerArrowMarker = new ExpansionMapMarkerPlayerArrow(NULL, m_MapWidget);
			m_PlayerArrowMarker.SetName("");
		}

		m_CompassPanel = GetGame().GetWorkspace().CreateWidgets("DayZExpansion/Navigation/GUI/layouts/expansion_compass.layout");
		if (m_CompassPanel)
		{
			m_CompassPanel.Show(false);
			m_CompassImage = ImageWidget.Cast(m_CompassPanel.FindAnyWidget("CompassImage"));
		}

		m_Expansion_LocatorUI = new ExpansionLocatorUI();
	}

	override void Update(float timeslice)
	{
		super.Update(timeslice);
		
		UIScriptedMenu topMenu = GetGame().GetUIManager().GetMenu();
		ExpansionScriptViewMenu viewMenu = ExpansionScriptViewMenu.Cast(GetDayZExpansion().GetExpansionUIManager().GetMenu());

		if (topMenu || viewMenu)
		{
			if (GetGPSMapState())
				m_PlayerArrowMarker.Hide();
			return;
		}
		else
		{
			if (GetGPSMapState() && !m_PlayerArrowMarker.GetLayoutRoot().IsVisible())
				m_PlayerArrowMarker.Hide();
		}
		
		if (GetExpansionSettings().GetMap().ShowPlayerPosition == 1 || GetExpansionSettings().GetMap().ShowPlayerPosition == 2)
		{
			if (m_PlayerArrowMarker)
			{
				m_PlayerArrowMarker.Update(timeslice);
			}
		}

		if (!GetDayZGame().GetExpansionGame().GetExpansionUIManager().GetMenu())
		{
			if (WasGPSOpened())
			{
				SetWasGPSOpened(false);
			}

			if (WasCompassOpened())
			{
				SetWasCompassOpened(false);
			}
		}

		if (m_GPSPanel && m_GPSPanel.IsVisible())
		{
			RefreshGPS();
		}

		if (m_CompassPanel && m_CompassPanel.IsVisible())
		{
			RefreshCompass();
		}

		m_Player = PlayerBase.Cast(GetGame().GetPlayer());
		if (m_Player && m_Player.IsAlive())
		{
			m_CloseTime += timeslice;

			if (GetDayZGame().GetExpansionGame().GetExpansionUIManager().GetMenu())
			{
				if (GetGPSState())
				{
					SetWasGPSOpened(true);
				}

				if (GetCompassState())
				{
					SetWasCompassOpened(true);
				}

				return;
			}

			//! GPS HUD
			UpdateGPS();

			//! COMPASS HUD
			if (GetExpansionSettings() && GetExpansionSettings().GetMap())
			{
				if (GetExpansionSettings().GetMap().EnableHUDCompass)
				{
					if (GetExpansionSettings().GetMap().NeedCompassItemForHUDCompass || GetExpansionSettings().GetMap().NeedGPSItemForHUDCompass)
					{
						UpdateCompass();
					}
					else
					{
						CompassShow();
					}
				}
			}
		}
	}

	void UpdateGPS()
	{
		//! If GPS is visable and player has no GPS item then close the GPS overlay when setting is enabled
		if (GetExpansionSettings().GetMap().NeedGPSItemForKeyBinding && GetGPSState())
		{
			if (!m_Player)
				return;

			if (!m_Player.HasItemGPS() && m_HasGPSItem)
			{
				m_HasGPSItem = false;
				ToggleHUDGPS();
			}
		}

		/*if (GetUApi().GetInputByName("UAExpansionGPSToggle").LocalHoldBegin() && GetExpansionSettings().GetMap().EnableHUDGPS && m_CloseTime > 0.75)
		{
			//! If it's already open, just close it
			if (GetGPSState())
			{
				ToggleHUDGPS();
			}
			else if (!GetGPSState())
			{
				if (GetExpansionSettings().GetMap().NeedGPSItemForKeyBinding)
				{
					if (!m_Player)
						return;

					if (m_Player.HasItemGPS())
					{
						m_HasGPSItem = true;
						ToggleHUDGPS();
					}
				}
				else
				{
					ToggleHUDGPS();
				}
			}
		}*/
	}

	void UpdateCompass()
	{
		if (!m_Player)
		{
			return;
		}

		if (GetCompassState())
		{
			if (GetExpansionSettings().GetMap().NeedGPSItemForHUDCompass && !m_Player.HasItemGPS() && m_HasGPSForCompassItem)
			{
				m_HasGPSForCompassItem = false;
				CompassHide();
			}

			if (GetExpansionSettings().GetMap().NeedCompassItemForHUDCompass && !m_Player.HasItemCompass() && m_HasCompassItem)
			{
				m_HasCompassItem = false;
				CompassHide();
			}
		}
		else
		{
			if (GetExpansionSettings().GetMap().NeedGPSItemForHUDCompass && m_Player.HasItemGPS())
			{
				m_HasGPSForCompassItem = true;
				CompassShow();
			}

			if (GetExpansionSettings().GetMap().NeedCompassItemForHUDCompass && m_Player.HasItemCompass())
			{
				m_HasCompassItem = true;
				CompassShow();
			}
		}
	}

	override void RefreshHudVisibility()
	{
		super.RefreshHudVisibility();

		m_ExpansionGPSSetting = GetExpansionSettings().GetMap().EnableHUDGPS;
		m_ExpansionGPSPosSetting = GetExpansionSettings().GetMap().ShowPlayerPosition;
		m_ExpansionCompassSetting = GetExpansionSettings().GetMap().EnableHUDCompass;

		if (m_GPSPanel)
		{
			m_GPSPanel.Show(m_HudState && m_ExpansionGPSState && m_ExpansionGPSSetting);
		}

		if (m_GPSMapPanel)
		{
			m_GPSMapPanel.Show(m_HudState && m_ExpansionGPSState && m_ExpansionGPSMapState && m_ExpansionGPSSetting);
		}

		if (m_PlayerArrowMarker)
		{
			m_PlayerArrowMarker.ShowRoot(m_HudState && m_ExpansionGPSState && m_ExpansionGPSMapState && m_ExpansionGPSSetting && (m_ExpansionGPSPosSetting == 1 || m_ExpansionGPSPosSetting == 2));
		}

		if (m_MapStatsPanel)
		{
			m_MapStatsPanel.Show(m_HudState && m_ExpansionGPSState && m_ExpansionGPSMapStatsState && m_ExpansionGPSSetting);
		}

		if (m_CompassPanel)
		{
			m_CompassPanel.Show(m_HudState && m_ExpansionCompassSetting && m_ExpansionCompassState && m_ExpansionCompassToggle);
		}
	}

	void RefreshGPS()
	{
		if (m_MapStatsPanel.IsVisible())
		{
			RefreshGPSMapStats();
		}

		if (m_GPSMapPanel.IsVisible())
		{
			RefreshGPSMap();
		}
	}

	void RefreshGPSMapStats()
	{
		vector m_cameraPosition, m_cameraDirection;
		float yaw, normalizedAngle;
		int dir, alt;
		PlayerBase player;

		if (Class.CastTo(player, GetGame().GetPlayer()))
		{
			m_cameraPosition = GetGame().GetCurrentCameraPosition();
			m_cameraDirection = GetGame().GetCurrentCameraDirection().VectorToAngles();
			yaw = m_cameraDirection[0];
			normalizedAngle = Math.NormalizeAngle(yaw);
			dir = Math.Round(normalizedAngle);
			alt = Math.Round(m_cameraPosition[1]);

			m_PlayerPosVal.SetText(Math.Round(m_cameraPosition[0]).ToString() + " : " + Math.Round(m_cameraPosition[2]).ToString());
			m_PlayerDirVal.SetText(dir.ToString());
			m_PlayerALTVal.SetText(alt.ToString());
		}
	}

	void RefreshGPSMap()
	{
		float scale;
		vector map_pos, camera_pos;
		float camera_x, camera_y;
		int deviation_x, deviation_y;
		int multiplier;
		float shift_x, shift_y;
		string worldName;

		if (!(GetGame().GetPlayer() && GetGame().GetCurrentCameraPosition()))
			return;

		string world_name = "empty";
		GetGame().GetWorldName(world_name);
		world_name.ToLower();

		if (world_name.IndexOf("gloom") == world_name.Length() - 5)
			world_name = world_name.Substring(0, world_name.Length() - 5);

		camera_pos = GetGame().GetCurrentCameraPosition();
		camera_x = camera_pos[0];
		camera_y = camera_pos[2];
		scale = GetCurrentGPSMapScale();

		multiplier = Math.Round(scale * 10);

		//Print("IngameHud::RefreshGPSMap - Map scale: " + scale);
		
		switch (world_name)
		{
			// 25600 / 25600
			case "banov":
			{
				if (scale >= 0.1)
				{
					shift_x = 642.5;
					shift_y = 485.5;
				}
				break;
			}
			case "chernarusplus":
			{
				if (scale >= 0.1)
				{
					shift_x = 642.5;
					shift_y = 485.5;
				}
				break;
			}
			case "deerisle":
			{
				// 16374 / 16400
				if (scale >= 0.1)
				{
					shift_x = 682.5;
					shift_y = 525.5;
				}
				break;
			}
			case "chiemsee":
			{
				// 10240 / 10240
				if (scale >= 0.1)
				{
					shift_x = 430;
					shift_y = 327.5;
				}
				break;
			}
			case "enoch":
			case "namalsk":
			case "esseker":
			case "takistan":
			case "takistanplus":
			{
				// 12800 / 12800
				if (scale >= 0.1)
				{
					shift_x = 545.0;
					shift_y = 412.5;
				}
				break;
			}
		}

		camera_x = camera_x + (shift_x * multiplier);
		camera_y = camera_y - (shift_y * multiplier);
		map_pos = Vector(camera_x, 0, camera_y);

		m_MapWidget.SetScale(scale);
		m_MapWidget.SetMapPos(map_pos);
	}

	void RefreshCompass()
	{
		if (!m_AddedCompassSettings)
		{
			int compass_color = GetExpansionSettings().GetMap().CompassColor;
			m_CompassImage.SetColor(compass_color);

			m_AddedCompassSettings = true;
		}

		vector player_dir = GetGame().GetCurrentCameraDirection();
		float player_angle = player_dir.VectorToAngles().GetRelAngles()[0];
		float image_pos;

		if ((player_angle <= 180) && (player_angle >= 0))
		{
			image_pos = (player_angle / -180) + 1;
		}
		else
		{
			image_pos = (player_angle / -180) - 1;
		}

		//! Fix slight inaccuracy in compass HUD and sudden "jump" when looking north past 360 degrees
		image_pos *= 0.995;

		m_CompassImage.SetPos(image_pos, 0, true);
	}

	void ShowCompass(bool show)
	{
		if (!m_ExpansionCompassDesiredState)
			return;

		m_ExpansionCompassState = show;

		RefreshHudVisibility();
	}

	void SetShowCompass( bool show )
	{
		ShowCompass( show );

		m_ExpansionCompassDesiredState = show;
	}

	bool GetCompassState()
	{
		return m_ExpansionCompassState;
	}

	void ShowGPS(bool show)
	{
		m_ExpansionGPSState = show;

		RefreshHudVisibility();
	}

	bool GetGPSState()
	{
		return m_ExpansionGPSState;
	}

	bool GetGPSMapState()
	{
		return m_ExpansionGPSMapState;
	}

	void ShowGPSMap(bool show)
	{
		m_ExpansionGPSMapState = show;
		RefreshHudVisibility();
	}

	bool GetGPSMapStatsState()
	{
		return m_ExpansionGPSMapStatsState;
	}

	void ShowGPSMapStats(bool show)
	{
		m_ExpansionGPSMapStatsState = show;
		RefreshHudVisibility();
	}

	void ToggleHUDGPS()
	{
		if (!GetGPSState())
		{
			GPSShow();
		}
		else
		{
			GPSHide();
		}
	}

	void GPSShow()
	{
		m_CloseTime = 0;

		ShowGPS(true);

		if (GetGPSMapStatsState())
			ShowGPSMapStats(false);

		if (!GetGPSMapState())
			ShowGPSMap(true);
	}

	void GPSHide()
	{
		m_CloseTime = 0;
		
		ShowGPS(false);
	}

	bool WasGPSOpened()
	{
		return m_GPSWasOpened;
	}

	void SetWasGPSOpened(bool state)
	{
		m_GPSWasOpened = state;
		ToggleHUDGPS();
	}

	void ToggleHUDCompass()
	{
		CompassShow(!GetCompassState());
	}

	void CompassShow(bool state = true)
	{
		ShowCompass(state);
	}

	void CompassHide()
	{
		CompassShow(false);
	}

	bool WasCompassOpened()
	{
		return m_CompassWasOpened;
	}

	void SetWasCompassOpened(bool state)
	{
		m_CompassWasOpened = state;
		ToggleHUDCompass();
	}
		
	void SetCompassToggleState()
	{
		m_ExpansionCompassToggle = !m_ExpansionCompassToggle;
		RefreshHudVisibility();
	}

	float GetCurrentGPSMapScale()
	{
		return m_GPSMapScale;
	}
	
	void SetGPSMapScale(float scale)
	{
		m_GPSMapScale = scale;
		RefreshGPSMap();
	}

	vector GetCurrentMapPos()
	{
		vector cameraPos;
		vector mapPos;

		cameraPos = GetGame().GetCurrentCameraPosition();
		mapPos = m_MapWidget.MapToScreen(cameraPos);

		return mapPos;
	}

	int GetCurrentMapDir()
	{
		vector cameraOri;
		float yaw;
		float normalizedAngle;
		int dir;

		cameraOri = GetGame().GetCurrentCameraDirection().VectorToAngles();
		yaw = cameraOri[0];
		normalizedAngle = Math.NormalizeAngle(yaw);
		dir = Math.Round(normalizedAngle);

		return dir;
	}
};
