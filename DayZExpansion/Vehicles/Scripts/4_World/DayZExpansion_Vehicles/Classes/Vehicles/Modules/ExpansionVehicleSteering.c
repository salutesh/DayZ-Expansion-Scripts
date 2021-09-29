/*
 * ExpansionVehicleSteering.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionVehicleSteering : ExpansionVehicleModule
{
	void ExpansionVehicleSteering(EntityAI vehicle, string rootPath)
	{
	}
};

class ExpansionVehicleCarSteering : ExpansionVehicleSteering
{
	ref array<float> m_IncreasingSpeed = new array<float>();
	ref array<float> m_DecreasingSpeed = new array<float>();
	ref array<float> m_CenteringSpeed = new array<float>();

	float m_Current;
	float m_Target;

	void ExpansionVehicleCarSteering(EntityAI vehicle, string rootPath)
	{
		LoadSpeed(rootPath, "increaseSpeed", m_IncreasingSpeed);
		LoadSpeed(rootPath, "decreaseSpeed", m_DecreasingSpeed);
		LoadSpeed(rootPath, "centeringSpeed", m_CenteringSpeed);
	}

	override void Control(ExpansionPhysicsState pState, DayZPlayerImplement pDriver)
	{
		UAInterface input = pDriver.GetInputInterface();

		float left = input.SyncedValue_ID(UACarLeft);
		float right = input.SyncedValue_ID(UACarRight);

		m_Target = right - left;
		
		float currentAbs = Math.AbsFloat(m_Current);
		float targetAbs = Math.AbsFloat(m_Target);
		float velocity = pState.m_LinearVelocity.Length();
		float speed;

		if (m_Target == 0)
			speed = GetSpeed(velocity, m_CenteringSpeed);
		else if (targetAbs > currentAbs)
			speed = GetSpeed(velocity, m_IncreasingSpeed);
		else if (targetAbs < currentAbs)
			speed = GetSpeed(velocity, m_DecreasingSpeed);

		float vel = (m_Target - m_Current) * speed * pState.m_DeltaTime;
		m_Current += Math.Clamp(vel, -speed * pState.m_DeltaTime, speed * pState.m_DeltaTime);

		m_Controller.m_Yaw = m_Current;
	}

	bool LoadSpeed(string rootPath, string pName, out array<float> pValues)
	{
		GetGame().ConfigGetFloatArray(rootPath + " " + pName, pValues);

		if (pValues.Count() == 0)
			return false;

		if (pValues.Count() <= 2)
			return false;

		for (int i = 0; i < pValues.Count() / 2; ++i)
			pValues[i * 2] = pValues[i * 2] / 3.6;

		return true;
	}

	float GetSpeed(float pSpeed, array<float> pValues)
	{
		int count = pValues.Count() * 0.5;
		float speed;

		for (int i = 1; i < count; i++)
		{
			if (pValues[i * 2] > pSpeed)
			{
				int c1 = (i - 1) * 2;
				int c2 = i * 2;

				float cs1 = pValues[c1];
				float cs2 = pValues[c2];

				float s1 = pValues[c1 + 1];
				float s2 = pValues[c2 + 1];

				float speedLerp = 1.0 - (cs2 - pSpeed) / (cs2 - cs1);

				speed = Math.Lerp(s1, s2, speedLerp) * Math.DEG2RAD;

				return speed;
			}
		}

		speed = pValues[pValues.Count() - 1] * Math.DEG2RAD;

		return speed;
	}
};

class ExpansionVehicleBikeSteering : ExpansionVehicleSteering
{
	float m_Current;
	float m_Target;

	override void Control(ExpansionPhysicsState pState, DayZPlayerImplement pDriver)
	{
		UAInterface input = pDriver.GetInputInterface();

		float left = input.SyncedValue_ID(UACarLeft);
		float right = input.SyncedValue_ID(UACarRight);

		float velocity = Math.AbsFloat(pState.m_LinearVelocityMS[2]);

		m_Target = (right - left) / (Math.Max(velocity, 1.0));
		m_Current += Math.Clamp(m_Target - m_Current, -20.0 * pState.m_DeltaTime, 20.0 * pState.m_DeltaTime);

		m_Controller.m_Yaw = m_Current;
	}
};
