/*
 * ExpansionVehicleGearbox.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionVehicleGearbox : ExpansionVehicleModule
{
	int m_ControlIndex = 0;

	float m_TimeToUncoupleClutch;
	float m_TimeToCoupleClutch;
	float m_MaxClutchTorque;

	float m_Reverse;
	ref array<float> m_Ratios = new array<float>();

	int m_Gear;
	int m_TargetGear;

	float m_Time;

	float m_Clutch;
	int m_ClutchState; // 0 - nothing, 1 - waiting, 2 - pressing, 3 - reached, 4 - releasing

	void ExpansionVehicleGearbox(EntityAI pVehicle, string rootPath)
	{
		string path;

		path = rootPath + " controller";
		if (GetGame().ConfigIsExisting(path))
			m_ControlIndex = GetGame().ConfigGetInt(path);

		path = rootPath + " ratios";
		if (GetGame().ConfigIsExisting(path))
			GetGame().ConfigGetFloatArray(path, m_Ratios);

		path = rootPath + " reverse";
		if (GetGame().ConfigIsExisting(path))
			m_Reverse = GetGame().ConfigGetFloat(path);

		path = rootPath + " timeToUncoupleClutch";
		if (GetGame().ConfigIsExisting(path))
			m_TimeToUncoupleClutch = GetGame().ConfigGetFloat(path);

		path = rootPath + " timeToCoupleClutch";
		if (GetGame().ConfigIsExisting(path))
			m_TimeToCoupleClutch = GetGame().ConfigGetFloat(path);

		path = rootPath + " maxClutchTorque";
		if (GetGame().ConfigIsExisting(path))
			m_MaxClutchTorque = GetGame().ConfigGetFloat(path);
	}

	int Count()
	{
		return m_Ratios.Count() + 1;
	}

	override void Control(ExpansionPhysicsState pState, DayZPlayerImplement pDriver)
	{
		if (!pDriver)
			return;

		UAInterface input = pDriver.GetInputInterface();

		if (input.SyncedPress_ID(UACarShiftGearUp))
		{
			m_TargetGear++;
		}
		else if (input.SyncedPress_ID(UACarShiftGearDown))
		{
			m_TargetGear--;
		}

		if (m_TargetGear < 0)
			m_TargetGear = 0;

		if (m_TargetGear > Count())
			m_TargetGear = Count();

		auto hcv = ExpansionHumanCommandVehicle.Cast(pDriver.GetCommand_Script());
		if (hcv)
		{
			if (m_TargetGear != m_Gear && m_ClutchState == 0)
			{
				hcv.SignalGearChange();

				m_ClutchState = 2;
				m_Time = 0;
			}
		}
	}

	override void PreSimulate(ExpansionPhysicsState pState)
	{
		float clutchDt;

		switch (m_ClutchState)
		{
		case 0:
			m_Clutch = 0;
			break;
		case 1:
			m_Clutch = 0;
			m_Time += pState.m_DeltaTime;
			if (m_Time > 0.5) // clutch simulation probably in a deadlock, reset
			{
				m_ClutchState = 0;
				m_Time = 0;
			}
			break;
		case 2:
			m_Clutch += pState.m_DeltaTime / m_TimeToCoupleClutch;
			if (m_Clutch >= 1.0)
				m_ClutchState = 3;
			break;
		case 3:
			m_Clutch = 1;

			if (m_Gear != m_TargetGear)
				m_Gear = m_TargetGear;

			m_ClutchState = 4;
			break;
		case 4:
			m_Clutch -= pState.m_DeltaTime / m_TimeToUncoupleClutch;
			if (m_Clutch <= 0)
				m_ClutchState = 0;
			break;
		}

		float ratio = 0;

		if (m_Clutch > 0.1)
			ratio = 0;
		else if (m_Gear == 0)
			ratio = -m_Reverse;
		else if (m_Gear > 1)
			ratio = m_Ratios[m_Gear - 2];

		m_Controller.m_Clutch[m_ControlIndex] = m_Clutch;
		m_Controller.m_Gear[m_ControlIndex] = m_Gear;
		m_Controller.m_Ratio[m_ControlIndex] = ratio;
	}

#ifdef CF_DebugUI
	bool CF_OnDebugUpdate(CF_Debug instance, CF_DebugUI_Type type)
	{
		instance.Add("Clutch", m_Clutch);
		instance.Add("Gear", m_Gear);

		return true;
	}
#endif
};
