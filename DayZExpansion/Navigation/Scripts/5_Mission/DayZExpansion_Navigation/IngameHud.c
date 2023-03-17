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
	protected Widget m_GPSPanel;
	protected Widget m_MapStatsPanel;
	protected Widget m_MapPosPanel;
	protected TextWidget m_PlayerPosVal;
	protected Widget m_PlayerPosPanel;
	protected TextWidget m_PlayerALTVal;
	protected Widget m_PlayerALTPanel;
	protected TextWidget m_PlayerDirVal;
	protected Widget m_PlayerDirPanel;

	protected Widget m_GPSMapPanel;
	protected Widget m_MapMarkerPanel;
	protected MapWidget m_MapWidget;
	protected ref ExpansionMapMarkerPlayerArrow m_PlayerArrowMarker;

	protected float m_GPSMapScale = 0.1;

	protected Widget m_CompassPanel;
	protected ImageWidget m_CompassImage;
	protected Widget m_CompassBadge1;
	protected Widget m_CompassBadge2;
	protected Widget m_CompassBadge3;
	protected bool m_AddedCompassSettings;

	protected bool m_ExpansionGPSState;
	protected bool m_ExpansionGPSMapState;
	protected bool m_ExpansionGPSMapStatsState;
	protected int m_ExpansionGPSPosSetting;
	protected bool m_ExpansionGPSSetting;

	protected bool m_ExpansionCompassSetting;
	protected bool m_ExpansionCompassState;
	protected bool m_ExpansionCompassDesiredState;

	protected bool m_HasGPSItem;
	protected float m_CloseTime = 0;
	protected bool m_GPSWasOpened = false;

	protected bool m_HasGPSForCompassItem;
	protected bool m_HasCompassItem;
	protected bool m_CompassWasOpened;
	protected PlayerBase m_Player;
	protected bool m_ExpansionCompassToggle;

	protected autoptr ExpansionLocatorUI m_Expansion_LocatorUI;

	void IngameHud()
	{
		m_AddedCompassSettings = false;
		m_ExpansionCompassState = false;
		m_ExpansionCompassDesiredState = true;

		m_HasGPSItem = false;
		m_HasGPSForCompassItem = false;
		m_HasCompassItem = false;
		m_ExpansionCompassToggle = true;

		ExpansionSettings.SI_Map.Insert(Expansion_OnNavigationSettingsUpdated);
	}

	void ~IngameHud()
	{
		ExpansionSettings.SI_Map.Remove(Expansion_OnNavigationSettingsUpdated);
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
			m_PlayerArrowMarker = new ExpansionMapMarkerPlayerArrow(m_MapMarkerPanel, m_MapWidget);
			m_PlayerArrowMarker.SetName("");
		}

		m_CompassPanel = GetGame().GetWorkspace().CreateWidgets("DayZExpansion/Navigation/GUI/layouts/expansion_compass.layout");
		if (m_CompassPanel)
		{
			m_CompassPanel.Show(false);
			m_CompassImage = ImageWidget.Cast(m_CompassPanel.FindAnyWidget("CompassImage"));
			m_CompassBadge1 = Widget.Cast(m_CompassPanel.FindAnyWidget("CompassBadge1"));
			m_CompassBadge2 = Widget.Cast(m_CompassPanel.FindAnyWidget("CompassBadge2"));
			m_CompassBadge3 = Widget.Cast(m_CompassPanel.FindAnyWidget("CompassBadge3"));
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
		}
		else
		{
			if (GetGPSMapState() && !m_PlayerArrowMarker.GetLayoutRoot().IsVisible())
				m_PlayerArrowMarker.Hide();
		}

		if (!GetExpansionSettings().GetMap(false).IsLoaded())
			return;

		if (GetExpansionSettings().GetMap().ShowPlayerPosition == 1 || GetExpansionSettings().GetMap().ShowPlayerPosition == 2)
		{
			if (m_PlayerArrowMarker)
				m_PlayerArrowMarker.Update(timeslice);
		}

		if (!GetDayZGame().GetExpansionGame().GetExpansionUIManager().GetMenu())
		{
			if (WasGPSOpened())
				SetWasGPSOpened(false);

			if (WasCompassOpened())
				SetWasCompassOpened(false);
		}

		if (m_GPSPanel && m_GPSPanel.IsVisible())
			RefreshGPS();

		if (m_CompassPanel && m_CompassPanel.IsVisible())
			RefreshCompass();

		m_Player = PlayerBase.Cast(GetGame().GetPlayer());
		if (m_Player && m_Player.IsAlive())
		{
			m_CloseTime += timeslice;

			if (GetDayZGame().GetExpansionGame().GetExpansionUIManager().GetMenu())
			{
				if (GetGPSState())
					SetWasGPSOpened(true);

				if (GetCompassState())
					SetWasCompassOpened(true);

				return;
			}

			//! GPS HUD
			UpdateGPS();

			//! COMPASS HUD
			if (GetExpansionSettings().GetMap())
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
	}

	void UpdateCompass()
	{
		if (!m_Player)
			return;

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

	void Expansion_OnNavigationSettingsUpdated()
	{
		m_ExpansionGPSSetting = GetExpansionSettings().GetMap().EnableHUDGPS;
		m_ExpansionGPSPosSetting = GetExpansionSettings().GetMap().ShowPlayerPosition;
		m_ExpansionCompassSetting = GetExpansionSettings().GetMap().EnableHUDCompass;
	}

	override void RefreshHudVisibility()
	{
		super.RefreshHudVisibility();

		if (m_GPSPanel)
			m_GPSPanel.Show(m_HudState && m_ExpansionGPSState && m_ExpansionGPSSetting);

		if (m_GPSMapPanel)
			m_GPSMapPanel.Show(m_HudState && m_ExpansionGPSState && m_ExpansionGPSMapState && m_ExpansionGPSSetting);

		if (m_PlayerArrowMarker)
		{
			bool show = m_HudState && m_ExpansionGPSState && m_ExpansionGPSMapState && m_ExpansionGPSSetting && (m_ExpansionGPSPosSetting == 1 || m_ExpansionGPSPosSetting == 2);
			m_PlayerArrowMarker.Show(show);
		}

		if (m_MapStatsPanel)
			m_MapStatsPanel.Show(m_HudState && m_ExpansionGPSState && m_ExpansionGPSMapStatsState && m_ExpansionGPSSetting);

		if (m_CompassPanel)
			m_CompassPanel.Show(m_HudState && m_ExpansionCompassSetting && m_ExpansionCompassState && m_ExpansionCompassToggle);
	}

	void RefreshGPS()
	{
		if (m_MapStatsPanel.IsVisible())
			RefreshGPSMapStats();

		if (m_GPSMapPanel.IsVisible())
		{
			UpdateGPSZoom();
			RefreshGPSMap();
		}
	}

	void UpdateGPSZoom()
	{
		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (player)
		{
			HumanCommandVehicle hcv = player.GetCommand_Vehicle();
			if (hcv)
			{
				CarScript car = CarScript.Cast(hcv.GetTransport());
				if (car)
				{
					float carSpeed = car.GetSpeedometerAbsolute() / 200;
					
					// Converting negative speed (backward)
					if ( carSpeed < 0.0 )
						carSpeed += carSpeed * 2;

					m_GPSMapScale = carSpeed;

					if ( m_GPSMapScale >= 0.8 ) 	m_GPSMapScale = 0.8;
					else if (m_GPSMapScale <= 0.1)	m_GPSMapScale = 0.1;
				}
			}
		}
	}

	void RefreshGPSMapStats()
	{
		if (!(GetGame().GetPlayer() && GetGame().GetCurrentCameraPosition()))
			return;

		vector cameraPosition = GetGame().GetCurrentCameraPosition();
		vector cameraDirection = GetGame().GetCurrentCameraDirection().VectorToAngles();
		float yaw = cameraDirection[0];
		float normalizedAngle = Math.NormalizeAngle(yaw);
		int dir = Math.Round(normalizedAngle);
		int alt = Math.Round(cameraPosition[1]);

		m_PlayerPosVal.SetText(Math.Round(cameraPosition[0]).ToString() + " : " + Math.Round(cameraPosition[2]).ToString());
		m_PlayerDirVal.SetText(dir.ToString());
		m_PlayerALTVal.SetText(alt.ToString());
	}

	void RefreshGPSMap()
	{
		if (!(GetGame().GetPlayer() && GetGame().GetCurrentCameraPosition()))
			return;

		vector tempPosition = GetGame().GetCurrentCameraPosition();
		vector mapPosition = Vector(tempPosition[0], tempPosition[1], tempPosition[2]);
		float scale = GetCurrentGPSMapScale();

		m_MapWidget.SetScale(scale);
		m_MapWidget.SetMapPos(mapPosition);
	}

	void RefreshCompass()
	{
		if (!m_AddedCompassSettings)
		{
			int compass_color = GetExpansionSettings().GetMap().CompassColor;
			int compass_badges_color = GetExpansionSettings().GetMap().CompassBadgesColor;

			m_CompassImage.SetColor(compass_color);
			m_CompassBadge1.SetColor(compass_badges_color);
			m_CompassBadge2.SetColor(compass_badges_color);
			m_CompassBadge3.SetColor(compass_badges_color);
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

	void SetShowCompass(bool show)
	{
		ShowCompass(show);
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

	ExpansionLocatorUI GetExpanisonLocatorUI()
	{
		return m_Expansion_LocatorUI;
	}

	float HUDCloseTime()
	{
		return m_CloseTime;
	}

	void SetHasGPSItem(bool state)
	{
		m_HasGPSItem = state;
	}

	override bool Expansion_GetHudState()
	{
		return GetHudState();
	};
};
