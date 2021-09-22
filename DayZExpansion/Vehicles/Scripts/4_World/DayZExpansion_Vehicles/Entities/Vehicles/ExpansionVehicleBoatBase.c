/**
 * ExpansionVehicleBoatBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionBoat
 * @brief		This class handle boat movement and physics
 **/
class ExpansionVehicleBoatBase extends ExpansionVehicleCarBase
{
	void ExpansionVehicleBoatBase()
	{
		SetEventMask(EntityEvent.CONTACT | EntityEvent.SIMULATE | EntityEvent.INIT);
	}

	override void DeferredInit()
	{
		super.DeferredInit();

		HideSelection("hiderotorblur");
		ShowSelection("hiderotor");

		SetVelocity(this, "0 0 0");
		dBodySetAngularVelocity(this, "0 0 0");

		vector position = GetPosition();
		vector orientation = GetOrientation();
		if (ExpansionStatic.SurfaceIsWater(position))
		{
			float depth = g_Game.GetWaterDepth(position);
			position[1] = position[1] + depth;
			SetPosition(position);
			SetOrientation(Vector(orientation[0], 0, 0));
		}
	}

	protected override void OnAnimationUpdate(float pDt)
	{
		super.OnAnimationUpdate(pDt);

		SetAnimationPhase("compasspointer", GetOrientation()[0] * Math.DEG2RAD);
		SetAnimationPhase("drivingWheel", m_Controller.GetSteering());
	}

	override int Get3rdPersonCameraType()
	{
		return DayZPlayerCameras.DAYZCAMERA_3RD_VEHICLE;
	}

	override int GetAnimInstance()
	{
		return ExpansionVehicleAnimInstances.EX_HATCHBACK;
	}

	protected override bool CanSimulate()
	{
		return true;
	}

	override int GetSeatAnimationType(int posIdx)
	{
		switch (posIdx)
		{
		case 0:
			return DayZPlayerConstants.VEHICLESEAT_DRIVER;
		case 1:
			return DayZPlayerConstants.VEHICLESEAT_CODRIVER;
		default:
			return 0;
		}

		return 0;
	}

	override bool CrewCanGetThrough(int posIdx)
	{
		return true;
	}

	override CarLightBase CreateRearLight()
	{
		return CarRearLightBase.Cast(ScriptedLightBase.CreateLight(ExpansionRearBoatLights));
	}

	override bool CanReachDoorsFromSeat(string pDoorsSelection, int pCurrentSeat)
	{
		return true;
	}

	override bool CanReachSeatFromDoors(string pSeatSelection, vector pFromPos, float pDistance = 1.0)
	{
		return true;
	}

	override bool IsCar()
	{
		return false;
	}

	override bool IsBoat()
	{
		return true;
	}

	override vector GetTransportCameraOffset()
	{
		return "0 1.5 0";
	}

	override float GetTransportCameraDistance()
	{
		return 12;
	}

	override bool CanConnectTow(notnull Object other)
	{
		return false;
	}
};
