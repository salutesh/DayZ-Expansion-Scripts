/**
 * ExpansionBoatHud.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2025 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifndef EXPANSION_VEHICLES_HUD_OLD
class ExpansionBoatHud : VehicleHudBase
{
	protected ExpansionBoatScript 	m_CurrentBoat;
	
	protected ImageWidget			m_BoatSpeedPointer;
	protected TextWidget			m_BoatSpeedValue;

	protected ImageWidget			m_BoatFuelLight;
	
	protected Widget				m_BoatGearPanel;
	protected TextWidget			m_BoatGearCurrent;
	protected TextWidget			m_BoatGearPrev;
	protected TextWidget			m_BoatGearNext;
	
	protected ImageWidget			m_BoatBatteryLight;
	protected ImageWidget			m_BoatEngineLight;

	protected Widget				m_BoatShieldPanel;
	protected ImageWidget			m_BoatShieldIcon;
	
	protected float					m_TimeSinceLastEngineLightChange;
	protected bool 					m_BoatDamageZoneHitEngineState;
	protected float					m_BoatDamageZonesHitTimer;
	protected int 					m_EngagedGear;
	
	override void Init(Widget vehicleHudPanels)
	{
		super.Init(vehicleHudPanels);
		
		m_VehiclePanel = GetGame().GetWorkspace().CreateWidgets("DayZExpansion/Vehicles/GUI/layouts/hud/hud_boats.layout", vehicleHudPanels);
		
		m_BoatSpeedPointer = ImageWidget.Cast(m_VehiclePanel.FindAnyWidget("SpeedPointer"));
		m_BoatSpeedValue = TextWidget.Cast(m_VehiclePanel.FindAnyWidget("SpeedValue"));

		m_BoatFuelLight = ImageWidget.Cast(m_VehiclePanel.FindAnyWidget("FuelLight"));
		
		m_BoatGearPanel	= Widget.Cast(m_VehiclePanel.FindAnyWidget("GearsIndicator"));
		m_BoatGearCurrent = TextWidget.Cast(m_VehiclePanel.FindAnyWidget("Current"));
		m_BoatGearPrev = TextWidget.Cast(m_VehiclePanel.FindAnyWidget("Prev"));
		m_BoatGearNext = TextWidget.Cast(m_VehiclePanel.FindAnyWidget("Next"));
		
		m_BoatBatteryLight = ImageWidget.Cast( m_VehiclePanel.FindAnyWidget("BatteryLight"));
		m_BoatEngineLight = ImageWidget.Cast( m_VehiclePanel.FindAnyWidget("EngineLight"));
		
		m_BoatShieldPanel = m_VehiclePanel.FindAnyWidget("ShieldIndicator");
		m_BoatShieldIcon = ImageWidget.Cast(m_VehiclePanel.FindAnyWidget("ShieldIcon"));
		
		m_VehicleCurrentGearValue = TextWidget.Cast(m_VehiclePanel.FindAnyWidget("Current"));
		m_VehicleNextGearValue = TextWidget.Cast(m_VehiclePanel.FindAnyWidget("Next"));
		m_VehiclePrevGearValue = TextWidget.Cast(m_VehiclePanel.FindAnyWidget("Prev"));
	}
	
	override void ShowVehicleInfo(PlayerBase player)
	{
		HumanCommandVehicle hcv = player.GetCommand_Vehicle();
		if (!hcv)
			return;

		m_CurrentBoat = ExpansionBoatScript.Cast(hcv.GetTransport());
		if (!m_CurrentBoat)
			return;
		
		m_VehiclePanel.Show(true);
		
		//! ToDo: Some boats have 5 gears?!
		m_VehicleGearTable.Set(-1, "");
		m_VehicleGearTable.Set(0, "R");
		m_VehicleGearTable.Set(1, "N");
		m_VehicleGearTable.Set(2, "F");
		m_VehicleGearTable.Set(3, "");
		m_VehicleGearTable.Set(4, "");
		m_VehicleGearTable.Set(5, "");
		
		m_EngagedGear = -10; // force initial update
	}
	
	override void HideVehicleInfo()
	{
		m_CurrentBoat = null;
		m_VehicleGearTable.Clear();
	}
	
	override void RefreshVehicleHud(float timeslice)
	{
		float speedValue = Math.AbsFloat(m_CurrentBoat.GetSpeedometer() / 400);
		
		m_BoatSpeedPointer.SetRotation(0, 0, speedValue * 260 - 130, true);
		m_BoatSpeedValue.SetText(Math.AbsInt(m_CurrentBoat.GetSpeedometer()).ToString());
		
		int engineHealthLevel = m_CurrentBoat.GetHealthLevel("Engine");
		float fuelFraction = m_CurrentBoat.GetFluidFraction(CarFluid.FUEL);
		bool invulnerable = m_CurrentBoat.m_State.m_IsInvulnerable;
		bool newHealth = false;
		
		//! engine
		if (invulnerable)
		{
			m_BoatEngineLight.Show(true);
			m_BoatEngineLight.SetColor(ARGB(255, 0, 255, 255));
		}
		else 
		{
			if (m_CurrentBoat.HasEngineZoneReceivedHit())
			{
				if (m_TimeSinceLastEngineLightChange > 0.35)
				{
					m_BoatEngineLight.Show(!m_BoatEngineLight.IsVisible());
					if (engineHealthLevel <= GameConstants.STATE_WORN)
					{
						m_BoatEngineLight.SetColor(Colors.WHITE);
					}
					else
					{					
						m_BoatEngineLight.SetColor(ItemManager.GetItemHealthColor(engineHealthLevel));
					}
					
					m_BoatEngineLight.SetAlpha(1);
					m_TimeSinceLastEngineLightChange = 0;
				}
				
				m_TimeSinceLastEngineLightChange += timeslice;
				newHealth = true;
			}
			else if (engineHealthLevel > GameConstants.STATE_WORN)
			{
				m_BoatEngineLight.SetColor(ItemManager.GetItemHealthColor(engineHealthLevel));
				m_BoatEngineLight.SetAlpha(1);
				m_BoatEngineLight.Show(true);
			}
			else
			{
				m_BoatEngineLight.Show(false);
			}
		}
		
		//! fuel tank
		if (fuelFraction > 0.45)
		{	
			m_BoatFuelLight.SetAlpha(0);
		}
		else
		{
			if (fuelFraction > 0.15)
				m_BoatFuelLight.SetColor(Colors.COLOR_DAMAGED);	
			else if (fuelFraction > 0)
				m_BoatFuelLight.SetColor(Colors.COLOR_BADLY_DAMAGED);	
			else 
				m_BoatFuelLight.SetColor(Colors.COLOR_RUINED);	
		
			m_BoatFuelLight.SetAlpha(1);
		}
		
		//! shield indicator
		if (!m_CurrentBoat.m_State.m_IsSync)
		{
			m_BoatShieldIcon.SetImage(1);
			if (m_CurrentBoat.m_State.m_HaltPhysics)
			{
				m_BoatShieldIcon.SetColor(ARGB(255, 255, 0, 0));
			}
			else
			{
				m_BoatShieldIcon.SetColor(ARGB(255, 255, 255, 255));
			}
		}
		else
		{
			m_BoatShieldIcon.SetImage(0);
			m_BoatShieldIcon.SetColor(ARGB(255, 0, 255, 255));
		}
		
		if (invulnerable || !m_CurrentBoat.m_State.m_IsSync)
		{
			m_BoatShieldPanel.Show(true);
		}
		else
		{
			m_BoatShieldPanel.Show(false);
		}
		
		//! battery
		ItemBase battery = m_CurrentBoat.GetBattery();
		if (battery)
		{
			float batteryPercentage = m_CurrentBoat.Expansion_GetBatteryEnergy();
			if (batteryPercentage <= 10)
			{
				m_BoatBatteryLight.SetColor(BATTERY_EMPTY_COLOR);
				m_BoatBatteryLight.Show(true);
			}
			else if (batteryPercentage <= 33)
			{
				m_BoatBatteryLight.SetColor(BATTERY_LOW_COLOR);
				m_BoatBatteryLight.Show(true);
			}
			else if (batteryPercentage <= 66)
			{
				m_BoatBatteryLight.SetColor(BATTERY_MED_COLOR);
				m_BoatBatteryLight.Show(true);
			}
			else
			{
				m_BoatBatteryLight.Show(false);
			}
		}
		else
		{
			m_BoatBatteryLight.Show(false);
		}

		UpdateGear();
	}
	
	protected void UpdateGear()
	{
		int gear = m_CurrentBoat.GetGear();
		if (m_EngagedGear == gear)
			return;
		
		if (gear > 2)
		{
			m_EngagedGear = 2;
		}
		else
		{
			m_EngagedGear = gear;
		}
		
		m_VehiclePrevGearValue.SetText(m_VehicleGearTable.Get(m_EngagedGear - 1));
		m_VehicleCurrentGearValue.SetText(m_VehicleGearTable.Get(m_EngagedGear));
		m_VehicleNextGearValue.SetText(m_VehicleGearTable.Get(m_EngagedGear + 1));
	}
}
#endif
