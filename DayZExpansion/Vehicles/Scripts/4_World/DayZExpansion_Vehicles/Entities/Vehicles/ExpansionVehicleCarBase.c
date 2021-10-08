/*
 * ExpansionVehicleCarBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * Copyright (c) 2005 Erwin Coumans http://continuousphysics.com/Bullet/
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionVehicleCarBase
 * @brief		This class handle bicycle simulation
 **/
class ExpansionVehicleCarBase extends ExpansionVehicleBase
{
	void ExpansionVehicleCarBase()
	{
		string path;

		if (m_Engines.Count() == 0)
		{
			path = "CfgVehicles " + GetType() + " SimulationModule Engine";
			AddModule(Expansion_CreateEngine(this, path));
		}

		path = "CfgVehicles " + GetType() + " SimulationModule Gearbox";
		AddModule(new ExpansionVehicleGearbox(this, path));

		path = "CfgVehicles " + GetType() + " SimulationModule Throttle";
		AddModule(new ExpansionVehicleCarThrottle(this, path));

		path = "CfgVehicles " + GetType() + " SimulationModule Steering";
		AddModule(new ExpansionVehicleCarSteering(this, path));
	}

	override int GetAnimInstance()
	{
		return ExpansionVehicleAnimInstances.EXPANSION_MH6;
	}

	override int GetSeatAnimationType(int posIdx)
	{
		switch (posIdx)
		{
		case 0:
			return DayZPlayerConstants.VEHICLESEAT_DRIVER;
		}

		return 0;
	}

	override bool CrewCanGetThrough(int posIdx)
	{
		return true;
	}

	override bool CanReachDoorsFromSeat(string pDoorsSelection, int pCurrentSeat)
	{
		return true;
	}

	override bool IsVitalCarBattery()
	{
		return false;
	}

	override bool IsVitalSparkPlug()
	{
		return false;
	}

	override bool IsVitalRadiator()
	{
		return false;
	}

	override bool IsVitalGlowPlug()
	{
		return false;
	}

	override bool IsVitalEngineBelt()
	{
		return false;
	}

	override bool IsVitalTruckBattery()
	{
		return false;
	}

	override bool IsCar()
	{
		return true;
	}

	override float GetTransportCameraDistance()
	{
		return 4.0;
	}

	override vector GetTransportCameraOffset()
	{
		return "0 1.3 0";
	}

#ifdef CF_DebugUI
	override bool CF_OnDebugUpdate(CF_Debug instance, CF_DebugUI_Type type)
	{
		super.CF_OnDebugUpdate(instance, type);

		return true;
	}
#endif

	override void OnAnimationUpdate(float pDt)
	{
		super.OnAnimationUpdate(pDt);

		SetAnimationPhase("steeringwheel", GetController().GetSteering());
	}
};
