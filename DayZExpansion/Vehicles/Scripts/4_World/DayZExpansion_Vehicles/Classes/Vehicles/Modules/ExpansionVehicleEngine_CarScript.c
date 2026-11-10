/*
 * ExpansionVehicleEngine_CarScript.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionVehicleEngine_CarScript : ExpansionVehicleEngineBase
{
	CarScript m_Car;

	void ExpansionVehicleEngine_CarScript(EntityAI vehicle, string rootPath)
	{
		m_TEMP_DeferredInit = false;
		m_SettingsChanged = false;
		m_Control = false;
		m_PreSimulate = false;
		m_Simulate = false;
		m_Animate = false;
		m_Network = false;

		m_DamageZone = "Engine";

		Class.CastTo(m_Car, m_Vehicle);

		m_RPMRedline = m_Car.EngineGetRPMRedline();
		m_RPMMax = m_Car.EngineGetRPMMax();
	}

	override void ProcessAcceleration(ExpansionPhysicsState pState)
	{
		if (m_Car.Expansion_IsBoat())
		{
			float speed = pState.m_LinearVelocity.Length();
			int gear = m_Car.GetGear();
			float ratio = 1.0;

			ExpansionVehicleGearbox gearbox = m_Car.m_Gearboxes[0];

			if (gear >= CarGear.FIRST)
				ratio = gearbox.m_Ratios[gear];
			else if (gear == CarGear.REVERSE)
				ratio = Math.AbsFloat(gearbox.m_Ratios[0]);
			else if (gear == CarGear.NEUTRAL && m_Car.Expansion_EngineIsOn())
				speed = m_Car.GetThrottle() * 45;

			float rpmMin;

			if (m_Car.Expansion_EngineIsOn())
				rpmMin = m_RPMIdle;

			float rpmTarget = rpmMin + speed * (m_RPMRedline - rpmMin) * 0.025 * ratio;

			rpmTarget = Math.Clamp(rpmTarget, rpmMin, m_RPMMax);
			m_RPM = Math.Lerp(m_RPM, rpmTarget, pState.m_DeltaTime * 4);
		}
		else
		{
			super.ProcessAcceleration(pState);
		}
	}

	override void ProcessHealth(float pDt, float pFuel, float pCoolant, float pOil, float pBrake, out float pOutHealth, out float pOutFuel)
	{
		if (!m_Car.Expansion_IsHelicopter())
			m_RPM = m_Car.EngineGetRPM();

		super.ProcessHealth(pDt, pFuel, pCoolant, pOil, pBrake, pOutHealth, pOutFuel);
	}
};
