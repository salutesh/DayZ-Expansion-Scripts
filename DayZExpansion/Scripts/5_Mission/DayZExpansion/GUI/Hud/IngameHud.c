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
	bool m_ExpansionHudNVState;
	bool m_ExpansionEarplugState;
	bool m_ExpansionNVSetting;

	Widget m_NVPanel;
	ImageWidget m_NVOverlayImage;
	ImageWidget m_NVBatteryIcon;
	TextWidget m_NVBatteryVal;
	int m_NVBatteryState;

	Widget m_EarPlugsPanel;
	
	void IngameHud()
	{
		ExpansionSettings.SI_General.Insert(Expansion_OnGeneralSettingsUpdated);
	}
	
	void ~IngameHud()
	{
		ExpansionSettings.SI_General.Remove(Expansion_OnGeneralSettingsUpdated);
	}

	override void Init(Widget hud_panel_widget)
	{
		super.Init(hud_panel_widget);

		m_NVPanel = GetGame().GetWorkspace().CreateWidgets("DayZExpansion/GUI/layouts/expansion_night_vision.layout");
		if (m_NVPanel)
		{
			m_NVPanel.Show(false);

			m_NVOverlayImage = ImageWidget.Cast(m_NVPanel.FindAnyWidget("NVOverlay"));
			m_NVBatteryIcon = ImageWidget.Cast(m_NVPanel.FindAnyWidget("NVBatteryIcon"));

			m_NVBatteryIcon.LoadImageFile(0, "DayZExpansion/Core/GUI/icons/hud/battery_empty_64x64.edds");
			m_NVBatteryIcon.LoadImageFile(1, "DayZExpansion/Core/GUI/icons/hud/battery_low_64x64.edds");
			m_NVBatteryIcon.LoadImageFile(2, "DayZExpansion/Core/GUI/icons/hud/battery_med_64x64.edds");
			m_NVBatteryIcon.LoadImageFile(3, "DayZExpansion/Core/GUI/icons/hud/battery_high_64x64.edds");

			m_NVBatteryVal = TextWidget.Cast(m_NVPanel.FindAnyWidget("NVBatteryVal"));
		}

		m_EarPlugsPanel = GetGame().GetWorkspace().CreateWidgets("DayZExpansion/GUI/layouts/expansion_earplugs.layout");
	}

	override void Update(float timeslice)
	{
		super.Update(timeslice);

		if (m_NVPanel.IsVisible())
		{
			UpdateNV();
		}
	}

	void Expansion_OnGeneralSettingsUpdated()
	{
		m_ExpansionNVSetting = GetExpansionSettings().GetGeneral().EnableHUDNightvisionOverlay;
	}

	override void RefreshHudVisibility()
	{
		super.RefreshHudVisibility();

		if (m_NVPanel)
		{
			m_NVPanel.Show(m_HudState && m_ExpansionHudNVState && m_ExpansionNVSetting);
		}
		
		if (m_EarPlugsPanel)
		{
			m_EarPlugsPanel.Show(m_HudState && m_ExpansionEarplugState);
		}
	}

	void ShowNV(bool show)
	{
		m_ExpansionHudNVState = show;
		
		RefreshHudVisibility();
	}

	bool GetNVState()
	{
		return m_ExpansionHudNVState;
	}

	int BATTERY_EMPTY_COLOR = ARGB(255, 231, 76, 60);
	int BATTERY_LOW_COLOR = ARGB(255, 230, 126, 34);
	int BATTERY_MED_COLOR = ARGB(255, 243, 156, 18);
	int BATTERY_FULL_COLOR = ARGB(255, 46, 204, 113);

	void RefreshNVBatteryState(int percent)
	{
		if (percent < 25)
		{
			//! 0 - 25% Energy
			m_NVBatteryIcon.SetImage(0);
			m_NVBatteryIcon.SetColor(BATTERY_EMPTY_COLOR);
		}
		else if (percent >= 25 && percent < 30)
		{
			//! 25 - 30% Energy
			m_NVBatteryIcon.SetImage(1);
			m_NVBatteryIcon.SetColor(BATTERY_LOW_COLOR);
		}
		else if (percent >= 30 && percent < 75)
		{
			//! 30 - 75% Energy
			m_NVBatteryIcon.SetImage(2);
			m_NVBatteryIcon.SetColor(BATTERY_MED_COLOR);
		}
		else if (percent >= 75)
		{
			//! 75 - 100% Energy
			m_NVBatteryIcon.SetImage(3);
			m_NVBatteryIcon.SetColor(BATTERY_FULL_COLOR);
		}

		m_NVBatteryVal.SetText(percent.ToString() + "%");
	}

	void SetNVBatteryState(int percent)
	{
		m_NVBatteryState = percent;
	}

	void UpdateNV()
	{
		RefreshNVBatteryState(m_NVBatteryState);
	}

	bool GetEarplugsState()
	{
		return m_ExpansionEarplugState;
	}

	void ToggleEarplugs()
	{
		m_ExpansionEarplugState = !m_ExpansionEarplugState;

		if (m_ExpansionEarplugState)
		{
			if (GetExpansionClientSettings())
			{
				GetGame().GetSoundScene().SetSoundVolume(GetExpansionClientSettings().EarplugLevel, 1);
			}
			else
			{
				GetGame().GetSoundScene().SetSoundVolume(0.5, 1);
			}
		}
		else
		{
			GetGame().GetSoundScene().SetSoundVolume(g_Game.m_volume_sound, 1);
		}

		RefreshHudVisibility();
	}

	void UpdateEarplugs()
	{
		if (GetExpansionClientSettings())
		{
			GetGame().GetSoundScene().SetSoundVolume(GetExpansionClientSettings().EarplugLevel, 1);

			RefreshHudVisibility();
		}
	}
};
