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

#ifdef EXPANSION_VEHICLE_NEW_HUD
modded class IngameHud
{
	ref array<ref ExpansionVehicleModuleUI> m_Expansion_Modules = new array<ref ExpansionVehicleModuleUI>();

	Widget m_Expansion_VehiclePanel;

	Widget m_LeftHudPanel;

	ImageWidget m_Expansion_VehicleSpeedPointer;
	TextWidget m_Expansion_VehicleSpeedValue;

	ImageWidget m_Expansion_VehicleTemperaturePointer;
	ImageWidget m_Expansion_VehicleFuelPointer;

	ImageWidget m_Expansion_VehicleBatteryLight;

	ExpansionVehicleBase m_Expansion_CurrentVehicle;

	override void Init(Widget hud_panel_widget)
	{
		super.Init(hud_panel_widget);

		m_LeftHudPanel = m_HudPanelWidget.FindAnyWidget("LeftHUDPanel");
		if (m_LeftHudPanel)
		{
			m_Expansion_VehiclePanel = GetGame().GetWorkspace().CreateWidgets("DayZExpansion/Vehicles/GUI/layouts/hud/vehicle.layout", m_LeftHudPanel);

			m_Expansion_VehicleSpeedPointer = ImageWidget.Cast(m_Expansion_VehiclePanel.FindAnyWidget("Expansion_SpeedPointer"));
			m_Expansion_VehicleSpeedValue = TextWidget.Cast(m_Expansion_VehiclePanel.FindAnyWidget("Expansion_SpeedValue"));
			m_Expansion_VehicleTemperaturePointer = ImageWidget.Cast(m_Expansion_VehiclePanel.FindAnyWidget("Expansion_TemperaturePointer"));
			m_Expansion_VehicleFuelPointer = ImageWidget.Cast(m_Expansion_VehiclePanel.FindAnyWidget("Expansion_FuelPointer"));
			m_Expansion_VehicleBatteryLight = ImageWidget.Cast(m_Expansion_VehiclePanel.FindAnyWidget("Expansion_BatteryLight"));
		}
	}

	override void ShowVehicleInfo()
	{
		m_Expansion_CurrentVehicle = null;

		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (!player)
			return;

		auto hcv = player.GetCommand_ExpansionVehicle();
		if (hcv && hcv.GetVehicleSeat() == DayZPlayerConstants.VEHICLESEAT_DRIVER)
		{
			m_Expansion_CurrentVehicle = hcv.GetTransport();
		}

		if (!m_Expansion_CurrentVehicle)
		{
			super.ShowVehicleInfo();
			return;
		}

		if (m_Expansion_CurrentVehicle.IsPlane() || m_Expansion_CurrentVehicle.IsHelicopter())
		{
			m_Expansion_Modules.Insert(new ExpansionVehicleAltimeterModuleUI(m_Expansion_CurrentVehicle, null, m_Expansion_VehiclePanel));
		}

		for (int i = 0; i < m_Expansion_CurrentVehicle.m_Engines.Count(); i++)
		{
			m_Expansion_Modules.Insert(new ExpansionVehicleEngineModuleUI(m_Expansion_CurrentVehicle, m_Expansion_CurrentVehicle.m_Engines[i], m_Expansion_VehiclePanel));
		}

		m_Expansion_VehiclePanel.Show(true);
		m_Expansion_VehicleBatteryLight.Show(false);
	}

	override void HideVehicleInfo()
	{
		super.HideVehicleInfo();

		m_Expansion_Modules.Clear();

		m_Expansion_VehiclePanel.Show(false);
	}

	override void RefreshVehicleHud(float timeslice)
	{
		super.RefreshVehicleHud(timeslice);

		if (m_Expansion_Modules.Count() > 0 && !GetGame().GetUIManager().GetMenu())
		{
			m_Expansion_VehiclePanel.Show(true);

			float speed_value = Math.AbsFloat(m_Expansion_CurrentVehicle.GetSpeedometer() / 200);
			m_Expansion_VehicleSpeedPointer.SetRotation(0, 0, speed_value * 260 - 130, true);
			m_Expansion_VehicleSpeedValue.SetText(Math.AbsInt(m_Expansion_CurrentVehicle.GetSpeedometer()).ToString());

			m_Expansion_VehicleFuelPointer.SetRotation(0, 0, m_Expansion_CurrentVehicle.GetFluidFraction(CarFluid.FUEL) * 260 - 130, true);
			m_Expansion_VehicleTemperaturePointer.SetRotation(0, 0, -1 * m_Expansion_CurrentVehicle.GetFluidFraction(CarFluid.COOLANT) * 260 + 130, true);

			for (int i = 0; i < m_Expansion_Modules.Count(); i++)
			{
				m_Expansion_Modules[i].OnUpdate(timeslice);
			}
		}
		else
		{
			m_Expansion_VehiclePanel.Show(false);
		}
	}
};
#endif
