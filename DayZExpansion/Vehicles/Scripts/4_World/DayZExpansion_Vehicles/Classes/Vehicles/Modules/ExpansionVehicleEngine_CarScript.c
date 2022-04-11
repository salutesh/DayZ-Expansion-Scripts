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
		m_FuelConsumption = 0.0;

		Class.CastTo(m_Car, m_Vehicle);
	}

	override void ProcessHealth(float pDt, float pFuel, float pCoolant, float pOil, float pBrake, out float pOutHealth, out float pOutFuel)
	{
		m_RPM = m_Car.EngineGetRPM();
		m_RPMRedline = m_Car.EngineGetRPMRedline();
		m_RPMMax = m_Car.EngineGetRPMMax();

		super.ProcessHealth(pDt, pFuel, pCoolant, pOil, pBrake, pOutHealth, pOutFuel);
	}
};
