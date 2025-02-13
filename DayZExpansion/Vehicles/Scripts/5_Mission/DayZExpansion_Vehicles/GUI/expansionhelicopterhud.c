/**
 * ExpansionHelicopterHud.c
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
class ExpansionHelicopterHud : VehicleHudBase
{
	protected ExpansionHelicopterScript m_CurrentHelicopter;

	protected ImageWidget			m_HeliSpeedPointer;
	protected TextWidget			m_HeliSpeedValue;

	protected TextWidget			m_HeliALTValue;
	protected ProgressBarWidget		m_HeliALTProgressBar;

	protected ImageWidget			m_HeliFuelPointer;
	protected ImageWidget			m_HeliFuelLight;
		
	protected ImageWidget			m_HeliHydraulicFluidPointer;
	protected ImageWidget			m_HeliHydraulicFluidLight;
	protected ImageWidget			m_HeliHydraulicFluidLight1;

	protected ImageWidget			m_HeliBatteryLight;
	protected ImageWidget			m_HeliEngineLight;
	
	protected Widget 				m_HeliHoverAltitudePanel;
	protected TextWidget			m_HeliHoverAltitudeValue;
	
	protected Widget				m_HeliShieldPanel;
	protected ImageWidget			m_HeliShieldIcon;
	
	protected ImageWidget			m_HeliALTPointerTE;
	protected ImageWidget			m_HeliALTPointerH;
	protected ImageWidget			m_HeliALTPointerTH;
	
	protected Widget 				m_HeliClimbPanel;
	protected ImageWidget    		m_HeliClimbPointer;
	protected TextWidget			m_HeliClimbValue;
	
	protected Widget				m_HeliAutoHoverPanel;
	protected TextWidget			m_HeliOutdoorTempValue;
	
	protected ProgressBarWidget		m_HeliAligmentProgressBar;
	protected TextWidget			m_HeliAlignmentValue;

	protected ProgressBarWidget		m_HeliThrustProgressBar;
	protected TextWidget			m_HeliThrustValue;

	protected Widget				m_HeliLightsIcon;
	
	protected float					m_TimeSinceLastEngineLightChange;
	protected bool 					m_HeliDamageZoneHitEngineState;
	protected float					m_HeliDamageZonesHitTimer;
	protected bool					m_LightsOn;
	protected PlayerBase			m_DriverPlayer;

	override void Init(Widget vehicleHudPanels)
	{
		m_VehiclePanel = GetGame().GetWorkspace().CreateWidgets("DayZExpansion/Vehicles/GUI/layouts/hud/hud_helicopters.layout", vehicleHudPanels);

		m_HeliSpeedPointer = ImageWidget.Cast(m_VehiclePanel.FindAnyWidget("SpeedPointer"));
		m_HeliSpeedValue = TextWidget.Cast(m_VehiclePanel.FindAnyWidget("SpeedValue"));

		m_HeliALTValue = TextWidget.Cast(m_VehiclePanel.FindAnyWidget("ALTValue"));
		m_HeliALTProgressBar = ProgressBarWidget.Cast(m_VehiclePanel.FindAnyWidget("ALTProgressBar"));

		m_HeliBatteryLight = ImageWidget.Cast( m_VehiclePanel.FindAnyWidget("BatteryLight"));
		
		m_HeliEngineLight = ImageWidget.Cast( m_VehiclePanel.FindAnyWidget("EngineLight"));

		m_HeliFuelPointer = ImageWidget.Cast(m_VehiclePanel.FindAnyWidget("FuelPointer"));
		m_HeliFuelLight = ImageWidget.Cast(m_VehiclePanel.FindAnyWidget("FuelLight"));
		
		m_HeliHydraulicFluidPointer = ImageWidget.Cast(m_VehiclePanel.FindAnyWidget("HydraulicFluidPointer"));
		m_HeliHydraulicFluidLight = ImageWidget.Cast(m_VehiclePanel.FindAnyWidget("HydraulicFluidLight"));
		m_HeliHydraulicFluidLight1 = ImageWidget.Cast(m_VehiclePanel.FindAnyWidget("HydraulicFluidLight1"));
		
		m_HeliHoverAltitudePanel = m_VehiclePanel.FindAnyWidget("HoverALTIndicator");
		m_HeliHoverAltitudeValue = TextWidget.Cast(m_VehiclePanel.FindAnyWidget("HoverALTValue"));
		
		m_HeliShieldPanel = m_VehiclePanel.FindAnyWidget("ShieldIndicator");
		m_HeliShieldIcon = ImageWidget.Cast(m_VehiclePanel.FindAnyWidget("ShieldIcon"));
		
		m_HeliALTPointerTE = ImageWidget.Cast(m_VehiclePanel.FindAnyWidget("ALTPointerTE"));
		m_HeliALTPointerH = ImageWidget.Cast(m_VehiclePanel.FindAnyWidget("ALTPointerH"));
		m_HeliALTPointerTH = ImageWidget.Cast(m_VehiclePanel.FindAnyWidget("ALTPointerTH"));
		
		m_HeliAutoHoverPanel = m_VehiclePanel.FindAnyWidget("AutoHoverIndicator");
		m_HeliOutdoorTempValue = TextWidget.Cast(m_VehiclePanel.FindAnyWidget("OutdoorTempValue"));

		m_HeliClimbPanel = m_VehiclePanel.FindAnyWidget("ClimbPanel");
		m_HeliClimbPointer = ImageWidget.Cast(m_VehiclePanel.FindAnyWidget("ClimbPointer"));
		m_HeliClimbValue = TextWidget.Cast(m_VehiclePanel.FindAnyWidget("ClimbValue"));
		
		m_HeliAligmentProgressBar = ProgressBarWidget.Cast(m_VehiclePanel.FindAnyWidget("AligmentProgressBar"));
		m_HeliAlignmentValue = TextWidget.Cast(m_VehiclePanel.FindAnyWidget("CurrentAlignmentValue"));

		m_HeliThrustProgressBar = ProgressBarWidget.Cast(m_VehiclePanel.FindAnyWidget("ThrustProgressBar"));
		m_HeliThrustValue = TextWidget.Cast(m_VehiclePanel.FindAnyWidget("CurrentThrustValue"));
		
		m_HeliLightsIcon = m_VehiclePanel.FindAnyWidget("LightsIcon");
	}

	override void ShowVehicleInfo(PlayerBase player)
	{
		m_DriverPlayer = player;
		HumanCommandVehicle hcv = m_DriverPlayer.GetCommand_Vehicle();
		if (!hcv)
			return;

		m_CurrentHelicopter = ExpansionHelicopterScript.Cast(hcv.GetTransport());
		if (!m_CurrentHelicopter)
		{
			return;
		}
		
		//! inital lights check
		m_LightsOn = m_CurrentHelicopter.IsScriptedLightsOn();
		m_HeliLightsIcon.Show(m_LightsOn);

		m_VehiclePanel.Show(true);
	}

	override void HideVehicleInfo()
	{
		m_CurrentHelicopter	= null;
		m_DriverPlayer = null;
		m_LightsOn = false;
	}

	override void RefreshVehicleHud(float timeslice)
	{	
		//! altimeter
		float altValue = m_CurrentHelicopter.GetPosition()[1];
		float altTE = (altValue / 100) * 360;	//! 10m steps
		float altH = (altValue / 1000) * 360;	//! 100m steps
		float altTH = (altValue / 10000) * 360;	//! 1000m steps
		
		m_HeliALTPointerTE.SetRotation(0, 0, altTE, true);
		m_HeliALTPointerH.SetRotation(0, 0, altH, true);
		m_HeliALTPointerTH.SetRotation(0, 0, altTH, true);

		if (m_CurrentHelicopter.IsAutoHover())
		{
			m_HeliAutoHoverPanel.Show(true);
			m_HeliALTValue.SetText(Math.Round(altValue).ToString() + "/" + Math.Round(m_CurrentHelicopter.GetAutoHoverTargetHeight()).ToString());
		}
		else
		{
			m_HeliAutoHoverPanel.Show(false);
			m_HeliALTValue.SetText(Math.Round(altValue).ToString());
		}

		//! aligment
		float horizonOffsetV = 0.5 - m_CurrentHelicopter.GetDirection()[1];
		m_HeliAligmentProgressBar.SetCurrent(horizonOffsetV);
		m_HeliAlignmentValue.SetText(Math.Round(m_CurrentHelicopter.GetOrientation()[1]).ToString() + "°");

		//! climb/fall rate
		float verticalVelocity = GetVelocity(m_CurrentHelicopter)[1]; //! climb/fall speed in m/s
		m_HeliClimbValue.SetText(Math.Round(verticalVelocity).ToString());
		m_HeliClimbPointer.SetRotation(0, 0, Math.Round(verticalVelocity * 0.04 * 180) - 90, true);  //! 0.04 = 1 / 25

		//! collective/thrust
		m_HeliThrustProgressBar.SetCurrent(m_CurrentHelicopter.m_Simulation.m_MainRotorSpeed);
		m_HeliThrustValue.SetText(Math.Round(m_CurrentHelicopter.m_Simulation.m_MainRotorSpeed * 100).ToString() + "%");

		//! lights
		if (m_LightsOn != m_CurrentHelicopter.IsScriptedLightsOn())
		{
			m_LightsOn = m_CurrentHelicopter.IsScriptedLightsOn();
			m_HeliLightsIcon.Show(m_LightsOn);
		}
		
		//! outdoor temperature
		//float temperature = GetGame().GetMission().GetWorldData().GetBaseEnvTemperatureAtObject(m_CurrentHelicopter);
		//m_HeliOutdoorTempValue.SetText("Temp:" + Math.Floor(temperature).ToString() + "C");
		
		//! speed
		float speedValue = Math.AbsFloat(m_CurrentHelicopter.GetSpeedometer() / 400);
		m_HeliSpeedPointer.SetRotation(0, 0, speedValue * 360 - 130, true);
		m_HeliSpeedValue.SetText(Math.AbsInt(m_CurrentHelicopter.GetSpeedometer()).ToString());
		
		//! fuel
		m_HeliFuelPointer.SetRotation(0, 0, m_CurrentHelicopter.GetFluidFraction(CarFluid.FUEL) * 260 - 130, true);
		
		//! hydraulic fluid
		m_HeliHydraulicFluidPointer.SetRotation(0, 0, m_CurrentHelicopter.GetFluidFraction(CarFluid.OIL) * 260 - 130, true);

		//! hydraulic hoses
		EntityAI hydraulicHoses = m_CurrentHelicopter.FindAttachmentBySlotName("ExpansionHydraulicHoses");

		if (hydraulicHoses)
		{
			int hydraulicHosesHealthLevel = hydraulicHoses.GetHealthLevel();
		
			if (hydraulicHosesHealthLevel <= GameConstants.STATE_WORN)
			{
				m_HeliHydraulicFluidLight.SetColor(Colors.WHITE);
				m_HeliHydraulicFluidLight1.SetColor(Colors.WHITE);
			}
			else
			{
				int hydraulicHosesHealthColor = ItemManager.GetItemHealthColor(hydraulicHosesHealthLevel);

				m_HeliHydraulicFluidLight.SetColor(hydraulicHosesHealthColor);
				m_HeliHydraulicFluidLight1.SetColor(hydraulicHosesHealthColor);
			}

			m_HeliHydraulicFluidLight.SetAlpha(1);
			m_HeliHydraulicFluidLight1.SetAlpha(1);
		}
		
		int engineHealthLevel = m_CurrentHelicopter.GetHealthLevel("Engine");
		int fuelTankHealthLevel = m_CurrentHelicopter.GetHealthLevel("FuelTank");
		bool invulnerable = m_CurrentHelicopter.m_State.m_IsInvulnerable;
		bool newHealth = false;
		
		//! engine
		if (invulnerable)
		{
			m_HeliEngineLight.Show(true);
			m_HeliEngineLight.SetColor(ARGB(255, 0, 255, 255));
		}
		else 
		{
			if (m_CurrentHelicopter.HasEngineZoneReceivedHit())
			{
				if (m_TimeSinceLastEngineLightChange > 0.35)
				{
					m_HeliEngineLight.Show(!m_HeliEngineLight.IsVisible());
					if (engineHealthLevel <= GameConstants.STATE_WORN)
					{
						m_HeliEngineLight.SetColor(Colors.WHITE);
					}
					else
					{					
						m_HeliEngineLight.SetColor(ItemManager.GetItemHealthColor(engineHealthLevel));
					}
					
					m_HeliEngineLight.SetAlpha(1);
					m_TimeSinceLastEngineLightChange = 0;
				}
				
				m_TimeSinceLastEngineLightChange += timeslice;
				newHealth = true;
			}
			else if (engineHealthLevel > GameConstants.STATE_WORN)
			{
				m_HeliEngineLight.SetColor(ItemManager.GetItemHealthColor(engineHealthLevel));
				m_HeliEngineLight.SetAlpha(1);
				m_HeliEngineLight.Show(true);
			}
			else
			{
				m_HeliEngineLight.Show(false);
			}
		}
		
		//! fuel tank
		if (fuelTankHealthLevel <= GameConstants.STATE_WORN)
		{
			m_HeliFuelLight.SetColor(Colors.WHITE);
			m_HeliFuelLight.SetAlpha(1);
		}
		else if (fuelTankHealthLevel > GameConstants.STATE_WORN)
		{
			m_HeliFuelLight.SetColor(ItemManager.GetItemHealthColor(fuelTankHealthLevel));
			m_HeliFuelLight.SetAlpha(1);
		}
		
		//! shield indicator
		if (!m_CurrentHelicopter.m_State.m_IsSync)
		{
			m_HeliShieldIcon.SetImage(1);
			if (m_CurrentHelicopter.m_State.m_HaltPhysics)
			{
				m_HeliShieldIcon.SetColor(ARGB(255, 255, 0, 0));
			}
			else
			{
				m_HeliShieldIcon.SetColor(ARGB(255, 255, 255, 255));
			}
		}
		else
		{
			m_HeliShieldIcon.SetImage(0);
			m_HeliShieldIcon.SetColor(ARGB(255, 0, 255, 255));
		}
		
		if (invulnerable || !m_CurrentHelicopter.m_State.m_IsSync)
		{
			m_HeliShieldPanel.Show(true);
		}
		else
		{
			m_HeliShieldPanel.Show(false);
		}
		
		//! battery
		ItemBase battery = m_CurrentHelicopter.GetBattery();
		if (battery)
		{
			float batteryPercentage = m_CurrentHelicopter.Expansion_GetBatteryEnergy();
			if (batteryPercentage <= 10)
			{
				m_HeliBatteryLight.SetColor(BATTERY_EMPTY_COLOR);
				m_HeliBatteryLight.Show(true);
			}
			else if (batteryPercentage <= 33)
			{
				m_HeliBatteryLight.SetColor(BATTERY_LOW_COLOR);
				m_HeliBatteryLight.Show(true);
			}
			else if (batteryPercentage <= 66)
			{
				m_HeliBatteryLight.SetColor(BATTERY_MED_COLOR);
				m_HeliBatteryLight.Show(true);
			}
			else
			{
				m_HeliBatteryLight.Show(false);
			}
		}
		else
		{
			m_HeliBatteryLight.Show(false);
		}
		
		//! refresh backlit
		GetDayZGame().GetBacklit().RefreshVehicleLayout(-1, newHealth);
		
		//! general hit timer reset			
		if (m_HeliDamageZonesHitTimer > 1)
		{
			m_HeliDamageZoneHitEngineState = false;
			m_HeliDamageZonesHitTimer = 0;
		}
		
		m_HeliDamageZonesHitTimer += timeslice;
	}
};
#endif
