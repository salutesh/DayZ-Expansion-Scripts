/*
 * ExpansionVehicleGearboxAdvanced.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionVehicleGearboxAdvanced : ExpansionVehicleGearbox
{
	float m_Time;

	int m_TargetGear;

	int m_ClutchState; // 0 - nothing, 1 - waiting, 2 - pressing, 3 - reached, 4 - releasing

	void ExpansionVehicleGearboxAdvanced(EntityAI pVehicle, string rootPath)
	{
		string path;

		path = rootPath + " ratios";
		if (GetGame().ConfigIsExisting(path))
			GetGame().ConfigGetFloatArray(path, m_Ratios);

		path = rootPath + " gears";
		if (GetGame().ConfigIsExisting(path))
			GetGame().ConfigGetTextArray(path, m_Gears);

		string clutchRootPath = rootPath;
		bool clutch_119;
		if (clutchRootPath.Replace("Gearbox", "Clutch") == 1 && GetGame().ConfigIsExisting(clutchRootPath))
			clutch_119 = true;

		if (clutch_119)
			path = clutchRootPath + " uncoupleTime";
		else
			path = rootPath + " timeToUncoupleClutch";
		if (GetGame().ConfigIsExisting(path))
			m_TimeToUncoupleClutch = GetGame().ConfigGetFloat(path);

		if (clutch_119)
			path = clutchRootPath + " coupleTime";
		else
			path = rootPath + " timeToCoupleClutch";
		if (GetGame().ConfigIsExisting(path))
			m_TimeToCoupleClutch = GetGame().ConfigGetFloat(path);

		if (clutch_119)
			path = clutchRootPath + " maxTorqueTransfer";
		else
			path = rootPath + " maxClutchTorque";
		if (GetGame().ConfigIsExisting(path))
			m_MaxClutchTorque = GetGame().ConfigGetFloat(path);

		m_Control = true;
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

		if (m_TargetGear > Count() - 1)
			m_TargetGear = Count() - 1;

		auto hcv = ExpansionHumanCommandVehicle.Cast(pDriver.GetCommand_Script());
		if (hcv)
		{
			if (m_TargetGear != m_Gear && (m_ClutchState == 0 || m_ClutchState == 3))
			{
				hcv.SignalGearChange();

				m_ClutchState = 2;
				m_Time = 0;
			}
		}
	}

	override void PreSimulate(ExpansionPhysicsState pState)
	{
		bool requiresClutch;
		if (m_Engine.m_RPM < m_Engine.m_RPMClutch)
			requiresClutch = true;
		if (m_ClutchState < 2 && requiresClutch)
			m_ClutchState = 2;

		bool changeGear = false;

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
			{
				m_Clutch = 1;
				m_ClutchState = 4;
				changeGear = true;
				if (requiresClutch)
					m_ClutchState = 3;
			}

			if (!requiresClutch && m_Gear == m_TargetGear)
			{
				m_ClutchState = 4;
				m_Clutch = Math.Min(m_Clutch, 1.0);
			}
			break;
		case 3:
			m_Clutch = 1;

			changeGear = true;

			if (!requiresClutch)
				m_ClutchState = 4;

			break;
		case 4:
			m_Clutch -= pState.m_DeltaTime / m_TimeToUncoupleClutch;
			if (m_Clutch <= 0)
				m_ClutchState = 0;
			break;
		}
		
		m_Clutch = Math.Clamp(m_Clutch, 0.0, 1.0);

		if (changeGear && m_Gear != m_TargetGear)
		{
			m_Gear = m_TargetGear;
		}

		m_Ratio = m_Ratios[m_Gear];

		super.PreSimulate(pState);
	}

#ifdef CF_DebugUI
	override bool CF_OnDebugUpdate(CF_Debug instance, CF_DebugUI_Type type)
	{
		super.CF_OnDebugUpdate(instance, type);

		instance.Add("Clutch State", m_ClutchState);
		
		return true;
	}
#endif
};
