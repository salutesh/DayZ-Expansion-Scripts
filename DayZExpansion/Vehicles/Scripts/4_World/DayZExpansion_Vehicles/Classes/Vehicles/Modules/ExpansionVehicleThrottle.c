/**
 * ExpansionVehicleThrottle.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionVehicleThrottle : ExpansionVehicleModule
{
	int m_ControlIndex = 0;

	float m_Throttle;

	void ExpansionVehicleThrottle(EntityAI vehicle, string rootPath)
	{
		string path;

		path = rootPath + " controller";
		if (GetGame().ConfigIsExisting(path))
			m_ControlIndex = GetGame().ConfigGetInt(path);
	}

#ifdef CF_DebugUI
	bool CF_OnDebugUpdate(CF_Debug instance, CF_DebugUI_Type type)
	{
		instance.Add("Throttle", m_Throttle);

		return true;
	}
#endif
};

class ExpansionVehicleCarThrottle : ExpansionVehicleThrottle
{
	float m_ReactionTime;
	float m_DefaultThrust;
	float m_GentleThrust;
	float m_TurboCoef;
	float m_GentleCoef;

	float m_Brake;

	void ExpansionVehicleCarThrottle(EntityAI vehicle, string rootPath)
	{
		string path;

		path = rootPath + " reactionTime";
		if (GetGame().ConfigIsExisting(path))
			m_ReactionTime = GetGame().ConfigGetFloat(path);

		path = rootPath + " defaultThrust";
		if (GetGame().ConfigIsExisting(path))
			m_DefaultThrust = GetGame().ConfigGetFloat(path);

		path = rootPath + " gentleThrust";
		if (GetGame().ConfigIsExisting(path))
			m_GentleThrust = GetGame().ConfigGetFloat(path);

		path = rootPath + " turboCoef";
		if (GetGame().ConfigIsExisting(path))
			m_TurboCoef = GetGame().ConfigGetFloat(path);

		path = rootPath + " gentleCoef";
		if (GetGame().ConfigIsExisting(path))
			m_GentleCoef = GetGame().ConfigGetFloat(path);
	}

	override void Control(ExpansionPhysicsState pState, DayZPlayerImplement pDriver)
	{
		if (!pDriver)
		{
			m_Throttle = 0;
			m_Brake = 1;
			if (m_Controller.m_Ratio[m_ControlIndex] == 0)
				m_Brake = 0;

			m_Controller.m_Throttle[m_ControlIndex] = m_Throttle;
			m_Controller.m_Brake[m_ControlIndex] = m_Brake;

			return;
		}

		UAInterface input = pDriver.GetInputInterface();

		float in = input.SyncedValue_ID(UACarForward);
		float gentle = input.SyncedValue_ID(UAVehicleSlow);
		float turbo = input.SyncedValue_ID(UAVehicleTurbo);

		m_Throttle = m_DefaultThrust * in;
		m_Throttle = Math.Lerp(m_Throttle, m_GentleThrust, gentle * in);
		m_Throttle = Math.Lerp(m_Throttle, 1.0, turbo * in);

		m_Brake = input.SyncedValue_ID(UACarBack);

		m_Controller.m_Throttle[m_ControlIndex] = m_Throttle;
		m_Controller.m_Brake[m_ControlIndex] = m_Brake;
	}
};

class ExpansionVehiclePlaneThrottle : ExpansionVehicleThrottle
{
	bool m_VariableInput = false;
	bool m_UpPressed;
	bool m_DownPressed;

	override void Control(ExpansionPhysicsState pState, DayZPlayerImplement pDriver)
	{
		if (pDriver)
		{
			UAInterface input = pDriver.GetInputInterface();

#ifdef COMPONENT_SYSTEM
			float engine = input.SyncedValue("UAExpansionPlaneEngine");
#else
			float engine = input.SyncedValue_ID(UAExpansionPlaneEngine);
#endif

			if (m_VariableInput || engine != 0.0)
			{
				m_VariableInput = true;
				m_Throttle = engine;
			}

#ifdef COMPONENT_SYSTEM
			m_UpPressed |= input.SyncedPress("UAExpansionPlaneEngineUp");
			if (input.SyncedRelease("UAExpansionPlaneEngineUp"))
				m_UpPressed = false;

			m_DownPressed |= input.SyncedPress("UAExpansionPlaneEngineDown");
			if (input.SyncedRelease("UAExpansionPlaneEngineDown"))
				m_DownPressed = false;
#else
			m_UpPressed |= input.SyncedPress_ID(UAExpansionPlaneEngineUp);
			if (input.SyncedRelease_ID(UAExpansionPlaneEngineUp))
				m_UpPressed = false;

			m_DownPressed |= input.SyncedPress_ID(UAExpansionPlaneEngineDown);
			if (input.SyncedRelease_ID(UAExpansionPlaneEngineDown))
				m_DownPressed = false;
#endif

			if (m_UpPressed || m_DownPressed)
			{
#ifdef COMPONENT_SYSTEM
				float engine_up = input.SyncedValue("UAExpansionPlaneEngineUp");
				float engine_down = input.SyncedValue("UAExpansionPlaneEngineDown");
#else
				float engine_up = input.SyncedValue_ID(UAExpansionPlaneEngineUp);
				float engine_down = input.SyncedValue_ID(UAExpansionPlaneEngineDown);
#endif

				m_VariableInput = false;
				m_Throttle += Math.Clamp((engine_up - engine_down) - m_Throttle, -0.5 * pState.m_DeltaTime, 0.5 * pState.m_DeltaTime);
			}
		}

		m_Throttle = Math.Clamp(m_Throttle, 0, 1);

		m_Controller.m_Throttle[m_ControlIndex] = m_Throttle;
	}
};
