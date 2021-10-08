/**
 * ExpansionController.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionController
 * @brief		Handles the raw values for controlling the various modules of the vehicle. 
 */
class ExpansionController
{
	float m_Throttle[4];

	bool m_State[4];
	float m_RPM[4];
	float m_Torque[4];

	float m_Clutch[4];

	float m_Ratio[4];
	int m_Gear[4];

	float m_Brake[4];

	float m_Yaw;
	float m_Pitch;
	float m_Roll;

	float GetSteering()
	{
		return m_Yaw;
	}

	void SetSteering(float value)
	{
		m_Yaw = value;
	}

	float GetYaw()
	{
		return m_Yaw;
	}

	void SetYaw(float value)
	{
		m_Yaw = value;
	}

	float GetPitch()
	{
		return m_Pitch;
	}

	void SetPitch(float value)
	{
		m_Pitch = value;
	}

	float GetRoll()
	{
		return m_Roll;
	}

	void SetRoll(float value)
	{
		m_Roll = value;
	}

	float GetThrottle(int index = 0)
	{
		return m_Throttle[index];
	}

	void SetThrottle(float value, int index = 0)
	{
		m_Throttle[index] = value;
	}

	float GetClutch(int index = 0)
	{
		return m_Clutch[index];
	}

	void SetClutch(float value, int index = 0)
	{
		m_Clutch[index] = value;
	}

	float GetBrake(int index = 0)
	{
		return m_Brake[index];
	}

	void SetBrake(float value, int index = 0)
	{
		m_Brake[index] = value;
	}

	int GetGear(int index = 0)
	{
		return m_Gear[index];
	}

	void SetGear(int value, int index = 0)
	{
		m_Gear[index] = value;
	}

	float GetRatio(int index = 0)
	{
		return m_Ratio[index];
	}

	void SetRatio(float value, int index = 0)
	{
		m_Ratio[index] = value;
	}

#ifdef CF_DebugUI
	bool CF_OnDebugUpdate(CF_Debug instance, CF_DebugUI_Type type)
	{
		int i;

		int maxControls = 4;

		string res;

		res = string.Empty;
		for (i = 0; i < maxControls; i++)
		{
			res += "" + m_Throttle[i];
			if (i < maxControls - 1)
				res += ", ";
		}
		instance.Add("Throttle", res);

		res = string.Empty;
		for (i = 0; i < maxControls; i++)
		{
			res += "" + m_State[i];
			if (i < maxControls - 1)
				res += ", ";
		}
		instance.Add("State", res);

		res = string.Empty;
		for (i = 0; i < maxControls; i++)
		{
			res += "" + m_RPM[i];
			if (i < maxControls - 1)
				res += ", ";
		}
		instance.Add("RPM", res);

		res = string.Empty;
		for (i = 0; i < maxControls; i++)
		{
			res += "" + m_Torque[i];
			if (i < maxControls - 1)
				res += ", ";
		}
		instance.Add("Torque", res);

		res = string.Empty;
		for (i = 0; i < maxControls; i++)
		{
			res += "" + m_Clutch[i];
			if (i < maxControls - 1)
				res += ", ";
		}
		instance.Add("Clutch", res);

		res = string.Empty;
		for (i = 0; i < maxControls; i++)
		{
			res += "" + m_Ratio[i];
			if (i < maxControls - 1)
				res += ", ";
		}
		instance.Add("Ratio", res);

		res = string.Empty;
		for (i = 0; i < maxControls; i++)
		{
			res += "" + m_Gear[i];
			if (i < maxControls - 1)
				res += ", ";
		}
		instance.Add("Gear", res);

		res = string.Empty;
		for (i = 0; i < maxControls; i++)
		{
			res += "" + m_Brake[i];
			if (i < maxControls - 1)
				res += ", ";
		}
		instance.Add("Brake", res);

		instance.Add("Yaw", m_Yaw);
		instance.Add("Pitch", m_Pitch);
		instance.Add("Roll", m_Roll);

		return true;
	}
#endif
};
