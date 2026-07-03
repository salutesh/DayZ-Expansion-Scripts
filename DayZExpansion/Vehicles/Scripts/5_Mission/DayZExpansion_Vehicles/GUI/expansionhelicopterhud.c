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

class ExpansionHelicopterHud : VehicleHudBase
{
	static int VORTEX_WARNING_COLOR = ARGB(255, 255, 191, 0);
	static int VORTEX_WARNING_SHADOW_COLOR = ARGB(255, 128, 64, 0);
	static int VORTEX_ALARM_COLOR = ARGB(255, 255, 140, 57);
	static int VORTEX_ALARM_SHADOW_COLOR = ARGB(255, 128, 0, 0);

#ifdef DIAG_DEVELOPER
	FileHandle m_DiagSpeedLogFile;
	float m_DiagSpeedLogTime;
#endif
	
	protected ExpansionHelicopterScript m_CurrentHelicopter;
	protected ref ExpansionBarrelRollDetector m_BarrelRollDetector;

	protected ImageWidget			m_HeliAttitudeSky;
	protected ImageWidget			m_HeliAttitudeGround;
	protected ImageWidget			m_HeliAttitudeHorizonLine;
	protected Widget				m_HeliAttitudePitchPanel;
	protected Widget				m_HeliAttitudePitch;
	protected ref map<int, Widget>	m_HeliAttitudePitchLines = new map<int, Widget>;
	protected ref map<int, Widget>	m_HeliAttitudePitchLabelL = new map<int, Widget>;
	protected ref map<int, Widget>	m_HeliAttitudePitchLabelR = new map<int, Widget>;
	protected Widget				m_HeliAttitudeCenter;
	protected Widget				m_HeliAttitudeHorizonLineL;
	protected Widget				m_HeliAttitudeHorizonLineR;

	protected ImageWidget			m_HeliSpeedPointer;
	protected TextWidget			m_HeliSpeedValue;
	protected TextWidget			m_HeliSpeedValueH;

	protected TextWidget			m_HeliALTValue;
	protected ProgressBarWidget		m_HeliALTProgressBar;

	protected ImageWidget			m_HeliFuelPointer;
	protected ImageWidget			m_HeliFuelLight;
		
	protected ImageWidget			m_HeliHydraulicFluidPointer;
	protected ImageWidget			m_HeliHydraulicFluidLight;
	protected ImageWidget			m_HeliHydraulicFluidLight1;

	protected ImageWidget			m_HeliBatteryLight;
	protected ImageWidget			m_HeliEngineLight;
	
	protected Widget				m_InfoPanel;
	protected ImageWidget			m_InfoIcon;
	protected TextWidget			m_InfoLabel;

	protected Widget				m_HeliShieldPanel;
	protected ImageWidget			m_HeliShieldIcon;
	
	protected ImageWidget			m_HeliALTPointerTE;
	protected ImageWidget			m_HeliALTPointerH;
	protected ImageWidget			m_HeliALTPointerTH;
	
	protected Widget 				m_HeliClimbPanel;
	protected ImageWidget    		m_HeliClimbPointer;
	protected TextWidget			m_HeliClimbValue;
	
	protected Widget				m_HeliVortexPanel;
	protected TextWidget			m_HeliVortexIcon;

	protected Widget				m_HeliAutoHoverPanel;
	protected Widget				m_HeliAutoCollectivePanel;

	protected TextWidget			m_HeliOutdoorTempValue;
	
	protected Widget				m_HeliPitchLine;
	protected TextWidget			m_HeliPitchValue;
	protected Widget				m_HeliPitchHorizonLineL;
	protected Widget				m_HeliPitchHorizonLineR;

	protected ProgressBarWidget		m_HeliThrustProgressBar;
	protected TextWidget			m_HeliThrustValue;
	protected Widget				m_HeliThrust100;
	protected Widget				m_HeliThrust50;
	protected Widget				m_HeliThrust0;
	protected Widget				m_HeliThrustN20;

	protected Widget				m_HeliLightsIcon;
	
	protected float					m_TimeSinceLastEngineLightChange;
	protected bool 					m_HeliDamageZoneHitEngineState;
	protected float					m_HeliDamageZonesHitTimer;
	protected bool					m_LightsOn;
	protected PlayerBase			m_DriverPlayer;

	protected Widget				m_AircraftMarkers;
	protected ImageWidget			m_WaterlineMarker;
	protected ImageWidget			m_FlightPathMarker;

	ref ExpansionRollingAverage		m_FlightPathVelocity_X_Avg = new ExpansionRollingAverage(6);
	ref ExpansionRollingAverage		m_FlightPathVelocity_Y_Avg = new ExpansionRollingAverage(6);
	ref ExpansionRollingAverage		m_FlightPathVelocity_Z_Avg = new ExpansionRollingAverage(6);

#ifdef DIAG_DEVELOPER
	protected bool					m_FlightPathVelocity_Dbg;
#endif

	void ~ExpansionHelicopterHud()
	{
		if (g_Game)
			GetExpansionClientSettings().SI_UpdateSetting.Remove(Expansion_OnClientSettingsUpdated);
	}

	override void Init(Widget vehicleHudPanels)
	{
		m_VehiclePanel = g_Game.GetWorkspace().CreateWidgets("DayZExpansion/Vehicles/GUI/layouts/hud/hud_helicopters.layout", vehicleHudPanels);

		m_HeliAttitudeSky = ImageWidget.Cast(m_VehiclePanel.FindAnyWidget("AttitudeSky"));
		m_HeliAttitudeGround = ImageWidget.Cast(m_VehiclePanel.FindAnyWidget("AttitudeGround"));
		m_HeliAttitudeHorizonLine = ImageWidget.Cast(m_VehiclePanel.FindAnyWidget("AttitudeHorizonLine"));
		m_HeliAttitudePitchPanel = m_VehiclePanel.FindAnyWidget("AttitudePitchPanel");
		m_HeliAttitudePitch = m_VehiclePanel.FindAnyWidget("AttitudePitch");
		m_HeliAttitudeCenter = m_VehiclePanel.FindAnyWidget("Attitude_Center");
		m_HeliAttitudeHorizonLineL = m_VehiclePanel.FindAnyWidget("Attitude_HorizonLine_L");
		m_HeliAttitudeHorizonLineR = m_VehiclePanel.FindAnyWidget("Attitude_HorizonLine_R");

		for (int n = 0; n <= 12; ++n)
		{
			int deg = (6 - n) * 5;

			if (deg)
			{
				int degAbs = Math.AbsInt(deg);
				string dir;

				if (deg < 0)
					dir = "Down";
				else
					dir = "Up";

				m_HeliAttitudePitchLines[deg] = m_VehiclePanel.FindAnyWidget(string.Format("Attitude_Pitch%1_%2", dir, degAbs));

				if (degAbs < 30 && degAbs % 10 == 0)
				{
					m_HeliAttitudePitchLabelL[deg] = m_VehiclePanel.FindAnyWidget(string.Format("Attitude_Pitch%1_%2_Label_L", dir, degAbs));
					m_HeliAttitudePitchLabelR[deg] = m_VehiclePanel.FindAnyWidget(string.Format("Attitude_Pitch%1_%2_Label_R", dir, degAbs));
				}
			}
		}

		m_HeliSpeedPointer = ImageWidget.Cast(m_VehiclePanel.FindAnyWidget("SpeedPointer"));
		m_HeliSpeedValue = TextWidget.Cast(m_VehiclePanel.FindAnyWidget("SpeedValue"));
		m_HeliSpeedValueH = TextWidget.Cast(m_VehiclePanel.FindAnyWidget("SpeedValueH"));

		m_HeliALTValue = TextWidget.Cast(m_VehiclePanel.FindAnyWidget("ALTValue"));
		m_HeliALTProgressBar = ProgressBarWidget.Cast(m_VehiclePanel.FindAnyWidget("ALTProgressBar"));

		m_HeliBatteryLight = ImageWidget.Cast( m_VehiclePanel.FindAnyWidget("BatteryLight"));
		
		m_HeliEngineLight = ImageWidget.Cast( m_VehiclePanel.FindAnyWidget("EngineLight"));

		m_HeliFuelPointer = ImageWidget.Cast(m_VehiclePanel.FindAnyWidget("FuelPointer"));
		m_HeliFuelLight = ImageWidget.Cast(m_VehiclePanel.FindAnyWidget("FuelLight"));
		
		m_HeliHydraulicFluidPointer = ImageWidget.Cast(m_VehiclePanel.FindAnyWidget("HydraulicFluidPointer"));
		m_HeliHydraulicFluidLight = ImageWidget.Cast(m_VehiclePanel.FindAnyWidget("HydraulicFluidLight"));
		m_HeliHydraulicFluidLight1 = ImageWidget.Cast(m_VehiclePanel.FindAnyWidget("HydraulicFluidLight1"));

		m_InfoPanel = m_VehiclePanel.FindAnyWidget("InfoPanel");
		m_InfoIcon = ImageWidget.Cast(m_VehiclePanel.FindAnyWidget("InfoIcon"));
		m_InfoLabel = TextWidget.Cast(m_VehiclePanel.FindAnyWidget("InfoLabel"));
		
		m_HeliShieldPanel = m_VehiclePanel.FindAnyWidget("ShieldIndicator");
		m_HeliShieldIcon = ImageWidget.Cast(m_VehiclePanel.FindAnyWidget("ShieldIcon"));
		
		m_HeliALTPointerTE = ImageWidget.Cast(m_VehiclePanel.FindAnyWidget("ALTPointerTE"));
		m_HeliALTPointerH = ImageWidget.Cast(m_VehiclePanel.FindAnyWidget("ALTPointerH"));
		m_HeliALTPointerTH = ImageWidget.Cast(m_VehiclePanel.FindAnyWidget("ALTPointerTH"));
		
		m_HeliVortexPanel = m_VehiclePanel.FindAnyWidget("VortexIndicator");
		m_HeliVortexIcon = TextWidget.Cast(m_VehiclePanel.FindAnyWidget("VortexIcon"));

		m_HeliAutoHoverPanel = m_VehiclePanel.FindAnyWidget("AutoHoverIndicator");
		m_HeliAutoCollectivePanel = m_VehiclePanel.FindAnyWidget("AutoCollectiveIndicator");

		m_HeliOutdoorTempValue = TextWidget.Cast(m_VehiclePanel.FindAnyWidget("OutdoorTempValue"));

		m_HeliClimbPanel = m_VehiclePanel.FindAnyWidget("ClimbPanel");
		m_HeliClimbPointer = ImageWidget.Cast(m_VehiclePanel.FindAnyWidget("ClimbPointer"));
		m_HeliClimbValue = TextWidget.Cast(m_VehiclePanel.FindAnyWidget("ClimbValue"));
		
		m_HeliPitchLine = m_VehiclePanel.FindAnyWidget("PitchLine");
		m_HeliPitchValue = TextWidget.Cast(m_VehiclePanel.FindAnyWidget("CurrentPitchValue"));
		m_HeliPitchHorizonLineL = m_VehiclePanel.FindAnyWidget("HorizonLine_L");
		m_HeliPitchHorizonLineR = m_VehiclePanel.FindAnyWidget("HorizonLine_R");

		m_HeliThrustProgressBar = ProgressBarWidget.Cast(m_VehiclePanel.FindAnyWidget("ThrustProgressBar"));
		m_HeliThrustValue = TextWidget.Cast(m_VehiclePanel.FindAnyWidget("CurrentThrustValue"));
		m_HeliThrust100 = m_VehiclePanel.FindAnyWidget("Thrust100");
		m_HeliThrust50 = m_VehiclePanel.FindAnyWidget("Thrust50");
		m_HeliThrust0 = m_VehiclePanel.FindAnyWidget("Thrust0");
		m_HeliThrustN20 = m_VehiclePanel.FindAnyWidget("ThrustN20");
		
		m_HeliLightsIcon = m_VehiclePanel.FindAnyWidget("LightsIcon");

		m_AircraftMarkers = g_Game.GetWorkspace().CreateWidgets("DayZExpansion/Vehicles/GUI/layouts/hud/hud_aircraft_markers.layout");
		m_WaterlineMarker = ImageWidget.Cast(m_AircraftMarkers.FindAnyWidget("WaterlineMarker"));
		m_FlightPathMarker = ImageWidget.Cast(m_AircraftMarkers.FindAnyWidget("FlightPathMarker"));

		GetExpansionClientSettings().SI_UpdateSetting.Insert(Expansion_OnClientSettingsUpdated);

		m_BarrelRollDetector = new ExpansionBarrelRollDetector(m_VehiclePanel);
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

		m_FlightPathVelocity_X_Avg.Reset();
		m_FlightPathVelocity_Y_Avg.Reset();
		m_FlightPathVelocity_Z_Avg.Reset();
	}

	override void HideVehicleInfo()
	{
		m_CurrentHelicopter	= null;
		m_DriverPlayer = null;
		m_LightsOn = false;
	}

	override void RefreshVehicleHud(float timeslice)
	{
		if (!m_CurrentHelicopter)
			return;

		ExpansionVehicleHelicopter simulation = m_CurrentHelicopter.m_Simulation;
		auto pState = m_CurrentHelicopter.m_State;

		bool legacySim = simulation.m_SimulationMode != ExpansionHelicopterSimulationMode.RotorDisk;
		bool legacyAirFriction = simulation.m_AirFrictionMode != ExpansionHelicopterSimulationAirFrictionMode.Balanced;
		bool manualTrim = !simulation.m_AutoTrim;
		string infoText;

		if (legacySim || legacyAirFriction)
			infoText = "LEGACY ";

		vector ori = m_CurrentHelicopter.GetOrientation();

		//! Horizontal velocity
		vector transform[4];
		m_CurrentHelicopter.GetTransform(transform);
		vector dir = transform[2];
		dir[1] = 0.0;  //! Null pitch
		dir.Normalize();
		transform[0] = -dir.Perpend();  //! Eliminate roll
		transform[2] = dir;
		vector velocity = pState.m_LinearVelocity.InvMultiply3(transform);

		vector velocityMS = pState.m_LinearVelocityMS;

		//! Forward velocity with vertical compensation
		//! Scale vertical component to 0 as forward approaches 0
		vector fwdVelocityVComp;
		fwdVelocityVComp[1] = velocityMS[1];
		fwdVelocityVComp[2] = velocityMS[2];
		vector fwdVelocityVCompNorm = fwdVelocityVComp.Normalized();
		float fwdSpeedVCompNormAbs = Math.AbsFloat(fwdVelocityVCompNorm[2]);
		float fwdSpeedVCompAbs = Math.AbsFloat(fwdVelocityVComp[2]);
		float vSpeedAbs = Math.AbsFloat(fwdVelocityVComp[1]) + 0.0001;
		float speedComp = ExpansionMath.LinearConversion(0, vSpeedAbs, fwdSpeedVCompAbs, fwdSpeedVCompNormAbs, 1.0);
		fwdVelocityVComp[1] = fwdVelocityVComp[1] * fwdSpeedVCompNormAbs;
		fwdVelocityVComp[2] = fwdVelocityVComp[2] * speedComp;

		float fwdSpeedVComp = fwdVelocityVComp.Length() * Math.Sign(fwdVelocityVComp[2]) * 3.6;
		float fwdSpeedMS = velocityMS[2] * 3.6;
		float groundSpeed = velocity[2] * 3.6;

		float speed;
		int speedDisplay;
		switch (speedDisplay)
		{
			case 1:  //! Vertically compensated forward speed
				speed = fwdSpeedVComp;
				break;

			case 2:
				speed = groundSpeed;
				break;

			case 3:
			default:
				speed = fwdSpeedMS;
				break;
		}

	#ifdef DIAG_DEVELOPER
		vector longitudinalVelocity;
		longitudinalVelocity[1] = velocityMS[1];
		longitudinalVelocity[2] = velocityMS[2];

		float longitudinalSpeed = longitudinalVelocity.Length() * 3.6;

		if (PluginDiagMenu.s_Expansion_LogHeliSpeed)
		{
			if (!m_DiagSpeedLogFile)
			{
				int year;
				int month;
				int day;
				int hour;
				int minute;
				int second;
				
				GetYearMonthDay(year, month, day);
				GetHourMinuteSecond(hour, minute, second);

				string speedLogFileNameFmt = "$profile:%1_speedlog_%2-%3-%4_%5-%6-%7.csv";
				string type = m_CurrentHelicopter.GetType();
				string speedLogFileName = string.Format(speedLogFileNameFmt, type, year, month, day, hour, minute, second);

				m_DiagSpeedLogFile = OpenFile(speedLogFileName, FileMode.WRITE);
				m_DiagSpeedLogTime = 0;

				FPrintln(m_DiagSpeedLogFile, "Time PitchAngle Speed LongitudinalSpeed GroundSpeed FwdSpeed");
			}

			if (m_DiagSpeedLogFile)
			{
				string speedLogLine = string.Format("%1 %2 %3 %4 %5 %6", m_DiagSpeedLogTime, ori[1], speed, longitudinalSpeed, groundSpeed, fwdSpeedMS);

				FPrintln(m_DiagSpeedLogFile, speedLogLine);

				m_DiagSpeedLogTime += timeslice;
			}
		}
		else if (m_DiagSpeedLogFile)
		{
			CloseFile(m_DiagSpeedLogFile);
			FileHandle file;
			m_DiagSpeedLogFile = file;
		}
	#endif

		if (manualTrim)
		{
			infoText += "MANUAL TRIM";

			float trim;

			trim += simulation.m_CyclicForwardTrim;
			trim += simulation.m_CyclicBackwardTrim;
			trim += simulation.m_CyclicLeftTrim;
			trim += simulation.m_CyclicRightTrim;
			trim += simulation.m_AntiTorqueLeftTrim;
			trim += simulation.m_AntiTorqueRightTrim;

			if (trim != 0)
				infoText += " SET";

		#ifndef DIAG_DEVELOPER
			m_InfoLabel.SetText(infoText);
		#endif
		}
	#ifndef DIAG_DEVELOPER
		else if (legacySim || legacyAirFriction)
		{
			m_InfoLabel.SetText(infoText);
		}

		m_InfoPanel.Show(legacySim || legacyAirFriction || manualTrim);
	#else
		m_InfoPanel.Show(true);

		float angularPitch = pState.m_AngularVelocityMS[0];
		float angularYaw = pState.m_AngularVelocityMS[1];
		float angularRoll = pState.m_AngularVelocityMS[2];
		string angularLabel = " ";

		if (Math.AbsFloat(angularYaw) > Math.AbsFloat(angularRoll) && Math.AbsFloat(angularYaw) > Math.AbsFloat(angularPitch))
			angularLabel += (Math.Round(angularYaw * 1000) / 1000).ToString(false) + " Y";
		else if (Math.AbsFloat(angularPitch) > Math.AbsFloat(angularRoll))
			angularLabel += (Math.Round(angularPitch * 1000) / 1000).ToString(false) + " P";
		else
			angularLabel += (Math.Round(angularRoll * 1000) / 1000).ToString(false) + " R";

		angularLabel.Replace("000 ", " ");
		angularLabel.Replace("-0.", "−0.");
		angularLabel.Replace("-1.", "−1.");
		angularLabel.Replace(" 0.", " +0.");
		angularLabel.Replace(" 1.", " +1.");

		infoText += angularLabel;

		m_InfoLabel.SetText(infoText);
	#endif

		if (legacySim != legacyAirFriction)
		{
			m_InfoIcon.SetColor(VORTEX_WARNING_COLOR);
			m_InfoLabel.SetColor(VORTEX_WARNING_COLOR);
		}
		else
		{
			m_InfoIcon.SetColor(COLOR_WHITE);
			m_InfoLabel.SetColor(COLOR_WHITE);
		}

		//! altimeter
		float altValue = m_CurrentHelicopter.GetPosition()[1];
		float altTE = (altValue / 100) * 360;	//! 10m steps
		float altH = (altValue / 1000) * 360;	//! 100m steps
		float altTH = (altValue / 10000) * 360;	//! 1000m steps
		
		m_HeliALTPointerTE.SetRotation(0, 0, altTE, true);
		m_HeliALTPointerH.SetRotation(0, 0, altH, true);
		m_HeliALTPointerTH.SetRotation(0, 0, altTH, true);

		float vrsSeverity = simulation.m_VRSSeverity;
		if (vrsSeverity > 0)
		{
			m_HeliVortexIcon.SetText("VORTEX");
			m_HeliVortexPanel.Show(true);

			if (vrsSeverity > 0.1)
			{
				m_HeliVortexIcon.SetColor(VORTEX_ALARM_COLOR);
				m_HeliVortexIcon.SetShadow(6, VORTEX_ALARM_SHADOW_COLOR);
			}
			else
			{
				m_HeliVortexIcon.SetColor(VORTEX_WARNING_COLOR);
				m_HeliVortexIcon.SetShadow(6, VORTEX_WARNING_SHADOW_COLOR);
			}

			//! Blinking thrust indicator effect when trying to raise collective while limited by VRS
			if (vrsSeverity > 0.1 && simulation.m_CollectiveTarget > simulation.m_Collective && m_HeliThrustProgressBar.GetColor() == COLOR_WHITE)
				m_HeliThrustProgressBar.SetColor(COLOR_RED);
			else
				m_HeliThrustProgressBar.SetColor(COLOR_WHITE);
		}
		else
		{
			m_HeliThrustProgressBar.SetColor(COLOR_WHITE);

			float rbsSeverity = simulation.m_RBSSeverity;

			if (rbsSeverity > 0.0)
			{
				m_HeliVortexIcon.SetText("STALL");
				m_HeliVortexPanel.Show(true);

				if (rbsSeverity > 0.1)
				{
					m_HeliVortexIcon.SetColor(VORTEX_ALARM_COLOR);
					m_HeliVortexIcon.SetShadow(6, VORTEX_ALARM_SHADOW_COLOR);
				}
				else
				{
					m_HeliVortexIcon.SetColor(VORTEX_WARNING_COLOR);
					m_HeliVortexIcon.SetShadow(6, VORTEX_WARNING_SHADOW_COLOR);
				}
			}
			else
			{
				m_HeliVortexPanel.Show(false);
			}
		}

		bool autoHover = m_CurrentHelicopter.IsAutoHover();
		bool autoCollective = simulation.m_AutoCollective;

		m_HeliAutoHoverPanel.Show(autoHover);
		m_HeliAutoCollectivePanel.Show(!autoHover && autoCollective);

		if (autoHover && (simulation.m_RotorSpeedTarget > 0 || simulation.m_RotorSpeed == 0))
		{
			m_HeliALTValue.SetText(Math.Round(altValue).ToString() + "/" + Math.Round(m_CurrentHelicopter.GetAutoHoverTargetHeight()).ToString());
		}
		else
		{
			m_HeliALTValue.SetText(Math.Round(altValue).ToString());
		}

		//! attitude
		float horizonOffsetV = m_CurrentHelicopter.GetDirection()[1];

		m_HeliAttitudeSky.SetRotation(0, 0, -ori[2]);
		m_HeliAttitudeGround.SetRotation(0, 0, -ori[2]);

		float indicatorW, indicatorH;
		float horizonW, horizonH;
		float attitudePitchW, attitudePitchH;

		m_HeliAttitudeGround.GetSize(indicatorW, indicatorH);
		m_HeliAttitudeHorizonLine.GetSize(horizonW, horizonH);
		m_HeliAttitudePitch.GetSize(attitudePitchW, attitudePitchH);

		float scaleV = attitudePitchH / indicatorH;
		float horizonScaleV = attitudePitchH / horizonH;

		m_HeliAttitudeGround.SetMaskProgress(0.505 - horizonOffsetV * scaleV);
		m_HeliAttitudeHorizonLine.SetRotation(0, 0, -ori[2]);
		m_HeliAttitudeHorizonLine.SetMaskProgress(0.515 - horizonOffsetV * horizonScaleV);

		float pitchOffsetV = ((indicatorH * 0.5 - attitudePitchH * 0.5) / indicatorH) * 0.5;
		float attitudePitchPanelW, attitudePitchPanelH;

		m_HeliAttitudePitchPanel.GetSize(attitudePitchPanelW, attitudePitchPanelH);

		float pitchScaleV = indicatorH / attitudePitchPanelH;
		float pitchPosY = pitchOffsetV + horizonOffsetV * pitchScaleV * scaleV;

		m_HeliAttitudePitch.SetPos(0, pitchPosY);

		float roll = Math.Round(ori[2]);
		float pitch = Math.Round(ori[1]);

		float pitchPanelPosX, pitchPanelPosY;
		float pitchLinePosX, pitchLinePosY;

		m_HeliAttitudePitchPanel.GetPos(pitchPanelPosX, pitchPanelPosY);

		for (int n = 0; n <= 12; ++n)
		{
			int deg = (6 - n) * 5;

			if (deg)
			{
				int degAbs = Math.AbsInt(deg);
				bool show = false;

				Widget pitchLine = m_HeliAttitudePitchLines[deg];
				
				pitchLine.GetPos(pitchLinePosX, pitchLinePosY);

				pitchLinePosY = pitchPanelPosY + pitchPosY * attitudePitchPanelH + (0.5 + pitchLinePosY) * attitudePitchH;

				if (pitchLinePosY > pitchPanelPosY && pitchLinePosY < pitchPanelPosY + attitudePitchPanelH)
					show = true;

				pitchLine.Show(show);

				if (degAbs < 30 && degAbs % 10 == 0)
				{
					m_HeliAttitudePitchLabelL[deg].Show(show);
					m_HeliAttitudePitchLabelR[deg].Show(show);
				}
					

				//PrintFormat("%1 %2", deg, m_HeliAttitudePitchLines[deg].IsVisible());
			}
		}

		if (roll)
		{
			m_HeliAttitudeCenter.SetColor(ARGB(255, 160, 160, 160));
		}
		else
		{
			m_HeliAttitudeCenter.SetColor(COLOR_WHITE);
		}

		if (pitch)
		{
			m_HeliPitchHorizonLineL.SetColor(ARGB(255, 160, 160, 160));
			m_HeliPitchHorizonLineR.SetColor(ARGB(255, 160, 160, 160));
		}
		else
		{
			m_HeliPitchHorizonLineL.SetColor(COLOR_WHITE);
			m_HeliPitchHorizonLineR.SetColor(COLOR_WHITE);
		}

		if (pitch || roll)
		{
			m_HeliAttitudeHorizonLineL.SetColor(ARGB(255, 160, 160, 160));
			m_HeliAttitudeHorizonLineR.SetColor(ARGB(255, 160, 160, 160));
		}
		else
		{
			m_HeliAttitudeHorizonLineL.SetColor(COLOR_WHITE);
			m_HeliAttitudeHorizonLineR.SetColor(COLOR_WHITE);
		}

		m_HeliPitchLine.SetPos(0, Math.Clamp(horizonOffsetV, -0.5, 0.5));
		m_HeliPitchValue.SetText(pitch.ToString() + "°");

		//! climb/fall rate
		float verticalVelocity = pState.m_LinearVelocity[1]; //! climb/fall speed in m/s
		m_HeliClimbValue.SetText((Math.Round(verticalVelocity * 10) / 10).ToString());
		m_HeliClimbPointer.SetRotation(0, 0, Math.Round(verticalVelocity * 0.04 * 180) - 90, true);  //! 0.04 = 1 / 25

		//! collective/thrust
		float collective;
		if ((simulation.m_AutoHover || simulation.m_AutoCollective) && simulation.m_RotorSpeedTarget > 0)
			collective = simulation.m_CollectiveInterpolated;
		else
			collective = simulation.m_CollectiveTarget;

		m_HeliThrustProgressBar.SetCurrent(collective);
		float collectivePct = Math.Round(collective * 100);
		string thrust = collectivePct.ToString();

		if (collectivePct == -20)
			m_HeliThrustN20.SetColor(COLOR_WHITE);
		else
			m_HeliThrustN20.SetColor(ARGB(255, 160, 160, 160));

		if (collectivePct == 0)
			m_HeliThrust0.SetColor(COLOR_WHITE);
		else
			m_HeliThrust0.SetColor(ARGB(255, 160, 160, 160));

		if (collectivePct == 50)
			m_HeliThrust50.SetColor(COLOR_WHITE);
		else
			m_HeliThrust50.SetColor(ARGB(255, 160, 160, 160));

		if (collectivePct == 100)
			m_HeliThrust100.SetColor(COLOR_WHITE);
		else
			m_HeliThrust100.SetColor(ARGB(255, 160, 160, 160));

	#ifdef DIAG_DEVELOPER
		float effectiveThrust = collective * simulation.m_RotorSpeed;
		float effectiveThrustDiff = Math.Round((collective - effectiveThrust) * 100);

		if (effectiveThrustDiff > 0)
		{
			thrust += "-" + effectiveThrustDiff.ToString();
			m_HeliThrustValue.SetColor(0xfffff000);
		}
		else
		{
			m_HeliThrustValue.SetColor(COLOR_WHITE);
		}
	#endif

		m_HeliThrustValue.SetText(thrust + "%");

		//! lights
		if (m_LightsOn != m_CurrentHelicopter.IsScriptedLightsOn())
		{
			m_LightsOn = m_CurrentHelicopter.IsScriptedLightsOn();
			m_HeliLightsIcon.Show(m_LightsOn);
		}
		
		//! outdoor temperature
		//float temperature = g_Game.GetMission().GetWorldData().GetBaseEnvTemperatureAtObject(m_CurrentHelicopter);
		//m_HeliOutdoorTempValue.SetText("Temp:" + Math.Floor(temperature).ToString() + "C");

		//! Speed
		float speedAbs = Math.AbsFloat(speed);
		//m_HeliSpeedPointer.SetRotation(0, 0, (speedAbs / 400) * 360 - 130, true);
		m_HeliSpeedValue.SetText(Math.Round(speed).ToString());

		//! Groundspeed side (horizontal)
		float sideSpeed = Math.Round(velocity[0] * 3.6);
		float sideSpeedAbs = Math.AbsFloat(sideSpeed);
		string speedValueH;

		if (sideSpeed != 0.0)
		{
			string arrow;

			if (sideSpeed > 0.0)
				arrow = "›";
			else if (sideSpeed < 0.0)
				arrow = "‹";

			string arrows;

			int intensity = Math.Clamp(sideSpeedAbs / 5, 1, 5);
			for (int i = 0; i < intensity; ++i)
			{
				arrows += arrow;
			}

			speedValueH = string.Format("%1 %2 %1", arrows, sideSpeedAbs);
		}
		else
			speedValueH = string.Format("‹ %1 ›", sideSpeedAbs);;

		m_HeliSpeedValueH.SetText(speedValueH);
		
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
		bool invulnerable = pState.m_IsInvulnerable;
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
		if (!pState.m_IsSync)
		{
			m_HeliShieldIcon.SetImage(1);
			if (pState.m_HaltPhysics)
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
		
		if (invulnerable || !pState.m_IsSync)
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

		//! Waterline marker
		vector centerOfMass = m_CurrentHelicopter.GetPhysics().GetCenterOfMass();
		vector origin = m_CurrentHelicopter.ModelToWorld(centerOfMass);
		vector nosePos = origin + m_CurrentHelicopter.GetDirection() * 1000;
		vector nosePosScrn = g_Game.GetScreenPosRelative(nosePos);

		if (nosePosScrn[0] <= 0 || nosePosScrn[0] >= 1 || nosePosScrn[1] <= 0 || nosePosScrn[1] >= 1 || nosePosScrn[2] < 0)
		{
			m_WaterlineMarker.Show(false);
		}
		else
		{
			m_WaterlineMarker.SetPos(nosePosScrn[0] - 0.5, nosePosScrn[1] - 0.5);
			m_WaterlineMarker.Show(true);
		}

		//! Flight path marker

		vector currentVelocity = m_CurrentHelicopter.GetPhysics().GetVelocity();
		vector flightPathVelocity;

		m_FlightPathVelocity_X_Avg.Add(currentVelocity[0] + pState.m_LinearAcceleration[0]);
		m_FlightPathVelocity_Y_Avg.Add(currentVelocity[1] + pState.m_LinearAcceleration[1]);
		m_FlightPathVelocity_Z_Avg.Add(currentVelocity[2] + pState.m_LinearAcceleration[2]);

		flightPathVelocity[0] = m_FlightPathVelocity_X_Avg.Get();
		flightPathVelocity[1] = m_FlightPathVelocity_Y_Avg.Get();
		flightPathVelocity[2] = m_FlightPathVelocity_Z_Avg.Get();

	#ifdef DIAG_DEVELOPER
		if (KeyState(KeyCode.KC_SPACE))
		{
			ClearKey(KeyCode.KC_SPACE);
			m_FlightPathVelocity_Dbg = !m_FlightPathVelocity_Dbg;
			string msg = "Flight path velocity dbg " + m_FlightPathVelocity_Dbg.ToString();
			g_Game.GetMission().OnEvent(ChatMessageEventTypeID, new ChatMessageEventParams(ChatChannelType.System, "", msg, "colorAction"));
		}

		if (m_FlightPathVelocity_Dbg)
		{
			flightPathVelocity = currentVelocity;
			m_FlightPathMarker.SetColor(ARGB(255, 255, 255, 0));
		}
		else
		{
			m_FlightPathMarker.SetColor(COLOR_WHITE);
		}
	#endif

		vector flightPath = origin + flightPathVelocity.Normalized() * 1000;

	/*
		vector side = pState.m_Transform[0];
		vector up   = pState.m_Transform[1];

		vector angularVel = pState.m_AngularVelocity;

		float localPitchRate = vector.Dot(angularVel, side);
		float localYawRate = vector.Dot(angularVel, up);
		float pitchOffset = localPitchRate * simulation.m_CyclicForwardMax;
		float yawOffset = localYawRate * simulation.m_AntiTorqueMax;

		flightPath = flightPath + (up * pitchOffset) + (side * yawOffset);
	*/

		vector flightPathScrn = g_Game.GetScreenPosRelative(flightPath);

		if (speedAbs < 4 || flightPathScrn[0] <= 0 || flightPathScrn[0] >= 1 || flightPathScrn[1] <= 0 || flightPathScrn[1] >= 1 || flightPathScrn[2] < 0)
		{
			m_FlightPathMarker.Show(false);

			m_FlightPathVelocity_X_Avg.Reset();
			m_FlightPathVelocity_Y_Avg.Reset();
			m_FlightPathVelocity_Z_Avg.Reset();
		}
		else
		{
			m_FlightPathMarker.SetPos(flightPathScrn[0] - 0.5, flightPathScrn[1] - 0.5);
			m_FlightPathMarker.Show(true);
		}

		//! Adjust waterline & flightpath marker rotation depending on camera
		if (m_DriverPlayer.IsCameraInsideVehicle())
		{
			m_WaterlineMarker.SetRotation(0, 0, 0);
			m_FlightPathMarker.SetRotation(0, 0, 0);
		}
		else
		{
			m_WaterlineMarker.SetRotation(0, 0, ori[2]);
			m_FlightPathMarker.SetRotation(0, 0, ori[2]);
		}

		m_BarrelRollDetector.Update(m_CurrentHelicopter, pState, timeslice);
	}

	void Expansion_OnClientSettingsUpdated()
	{
		if (m_VehiclePanel && m_VehiclePanel.IsVisible() && m_AircraftMarkers)
			m_AircraftMarkers.Show(!GetExpansionClientSettings().DisableFlightPathMarker);
	}
	
	override void ShowPanel()
	{
		super.ShowPanel();

		if (m_AircraftMarkers && !GetExpansionClientSettings().DisableFlightPathMarker)
			m_AircraftMarkers.Show(true);
	}
	
	override void HidePanel()
	{
		super.HidePanel();

		if (m_AircraftMarkers)
			m_AircraftMarkers.Show(false);
	}
};
