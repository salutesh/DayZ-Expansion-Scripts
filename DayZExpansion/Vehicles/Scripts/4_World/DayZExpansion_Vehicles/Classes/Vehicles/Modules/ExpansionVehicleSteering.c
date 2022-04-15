/*
 * ExpansionVehicleSteering.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionVehicleSteering : ExpansionVehicleModule
{
	void ExpansionVehicleSteering(EntityAI vehicle, string rootPath)
	{
		m_TEMP_DeferredInit = false;
		m_SettingsChanged = false;
		m_Control = true;
		m_PreSimulate = false;
		m_Simulate = false;
		m_Animate = false;
		m_Network = false;
	}
};
