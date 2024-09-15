/**
 * ExpansionVehiclePlaneBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
 */

/**
 * @class		ExpansionPlane
 * @brief		This class handle plane movement and physics
 */
class ExpansionVehiclePlaneBase: ExpansionVehicleBase
{
	private bool m_Hit;
	private bool m_HitDetermined;
	private Object m_HitObject;
	private vector m_HitPosition;
	private vector m_HitNormal;
	private float m_HitFraction;

	private float m_RotorAnimationPosition;

	void ExpansionVehiclePlaneBase()
	{
		SetEventMask(EntityEvent.CONTACT | EntityEvent.SIMULATE | EntityEvent.INIT);

		AddModule(new ExpansionVehiclePlaneThrottle(this, "CfgVehicles " + GetType() + " Throttles Throttle"));
		AddModule(new ExpansionVehicleYoke(this, "CfgVehicles " + GetType() + " Steering"));
	}

	protected override void OnPreSimulation(float pDt)
	{
		m_HitDetermined = false;

		PerformGroundRaycast();

		super.OnPreSimulation(pDt);
	}

	override void OnPostSimulation(float pDt)
	{
		super.OnPostSimulation(pDt);
	}

	protected override void OnParticleUpdate(float pDt)
	{
	}

	protected override void OnAnimationUpdate(float pDt)
	{
		SetAnimationPhase("compasspointer", GetOrientation()[0] * Math.DEG2RAD);
		SetAnimationPhase("attitudeDiveRTD", GetDirection()[1]);
		SetAnimationPhase("attitudeBankRTD", GetOrientation()[2] / 360);

		super.OnAnimationUpdate(pDt);
	}

	override void EOnContact(IEntity other, Contact extra)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.VEHICLES, this, "EOnContact").Add(other).Add(extra);
#endif

		if (IsMissionHost())
		{
			if (extra.Impulse > m_State.m_Mass * 25)
			{
				Explode(DT_EXPLOSION, "RGD5Grenade_Ammo");
			}
		}
	}

	override void ExpansionOnExplodeServer(int damageType, string ammoType)
	{
		super.ExpansionOnExplodeServer(damageType, ammoType);

		EntityAI item = null;
		item = FindAttachmentBySlotName("GlowPlug");
		if (item)
			item.SetHealth("", "", 0);

		item = FindAttachmentBySlotName("ExpansionAircraftBattery");
		if (item)
			item.SetHealth("", "", 0);

		item = FindAttachmentBySlotName("Reflector_1_1");
		if (item)
			item.SetHealth("", "", 0);

		item = FindAttachmentBySlotName("Reflector_2_1");
		if (item)
			item.SetHealth("", "", 0);

		m_Exploded = true;
		m_ExplodedSynchRemote = true;
	}

#ifdef CF_DebugUI
	override bool CF_OnDebugUpdate(CF_Debug instance, CF_DebugUI_Type type)
	{
		super.CF_OnDebugUpdate(instance, type);

		return true;
	}
#endif

	private void PerformGroundRaycast()
	{
		if (m_HitDetermined)
			return;

		float modelSize = m_BoundingRadius * 1.5;
		modelSize += 10.0;

		vector start = GetPosition();
		vector end = GetPosition() - Vector(0, modelSize, 0);

		PhxInteractionLayers collisionLayerMask = PhxInteractionLayers.BUILDING | PhxInteractionLayers.DOOR | PhxInteractionLayers.VEHICLE | PhxInteractionLayers.ROADWAY | PhxInteractionLayers.TERRAIN | PhxInteractionLayers.ITEM_SMALL | PhxInteractionLayers.ITEM_LARGE | PhxInteractionLayers.FENCE;

		m_Hit = false;
		if (dBodyIsDynamic(this))
			m_Hit = DayZPhysics.SphereCastBullet(start, end, 5.0, collisionLayerMask, this, m_HitObject, m_HitPosition, m_HitNormal, m_HitFraction);
		m_HitDetermined = true;
	}

	override int Get3rdPersonCameraType()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "Get3rdPersonCameraType");
#endif

		return DayZPlayerCameras.DAYZCAMERA_3RD_VEHICLE;
	}

	override int GetAnimInstance()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetAnimInstance");
#endif

		return VehicleAnimInstances.HATCHBACK;
	}

	override int GetSeatAnimationType(int posIdx)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "GetSeatAnimationType").Add(posIdx);
#endif

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
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "CrewCanGetThrough").Add(posIdx);
#endif

		return true;
	}

	override float OnSound(CarSoundCtrl ctrl, float oldValue)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.VEHICLES, this, "OnSound").Add(ctrl).Add(oldValue);
#endif

		if (m_Exploded)
			return 0;

		switch (ctrl)
		{
		case CarSoundCtrl.SPEED:
		{
			return Math.AbsFloat(GetSpeedometer());
		}
		case CarSoundCtrl.RPM:
		{
			return EngineGetRPM(0);
		}
		case CarSoundCtrl.ENGINE:
		{
			return EngineIsOn();
		}
		}

		return super.OnSound(ctrl, oldValue);
	}

	override bool CanReachDoorsFromSeat(string pDoorsSelection, int pCurrentSeat)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.VEHICLES, this, "CanReachDoorsFromSeat").Add(pDoorsSelection).Add(pCurrentSeat);
#endif

		return true;
	}

	override bool CanReachSeatFromDoors(string pSeatSelection, vector pFromPos, float pDistance = 1.0)
	{
		return true;
	}

	override bool Expansion_IsCar()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsCar");
#endif

		return false;
	}

	override bool Expansion_IsPlane()
	{
		return true;
	}

	override CarLightBase CreateFrontLight()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "CreateFrontLight");
#endif

		return CarLightBase.Cast(ScriptedLightBase.CreateLight(ExpansionHelicopterFrontLight));
	}

	override CarLightBase CreateRearLight()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "CreateRearLight");
#endif

		return CarRearLightBase.Cast(ScriptedLightBase.CreateLight(ExpansionRearHelicopterLights));
	}
};
