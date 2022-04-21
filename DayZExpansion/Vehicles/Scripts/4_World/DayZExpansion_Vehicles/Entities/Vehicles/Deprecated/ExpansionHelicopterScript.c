/**
 * ExpansionHelicopterScript.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionHelicopterScript
 * @brief		This class handle helicopter movement and physics
 **/
class ExpansionHelicopterScript extends CarScript
{
	ExpansionVehicleHelicopter_OLD m_Simulation;

	// the rotor speeds for the drag of the helicopter when going down while the engine is turned off
	float m_MinAutoRotateSpeed; // (m/s)
	float m_MaxAutoRotateSpeed; // (m/s)

	// Coefficient for cyclic control
	float m_BodyFrictionCoef;
	float m_LiftForceCoef;

	// Coefficient for cyclic control
	float m_CyclicForceCoef;

	// the rotation at which the forward cyclic moves (pitch)
	float m_CyclicForwardSpeed; // (m/s) change per tick
	float m_CyclicForwardMax;	// (m) per tick
	float m_CyclicForwardCoef;	// Coefficient for forward cyclic

	// the rotation at which the side cyclic moves (roll)
	float m_CyclicSideSpeed; // (m/s)
	float m_CyclicSideMax;	 // (m)
	float m_CyclicSideCoef;	 // Coefficient for side cyclic

	// the rotation at which the anti torque moves (yaw)
	float m_AntiTorqueSpeed; // (m/s)
	float m_AntiTorqueMax;	 // (m)

	float m_BankForceCoef = 0.7;
	float m_TailForceCoef = 8.0;

	// the time it takes for the engine to startup
	float m_EngineStartDuration; // (s)

	vector m_LinearFrictionCoef;
	float m_AngularFrictionCoef;

	// Lift factor (taken from Arma 2 config)
	int m_MaxFlightEnvelope = 15;
	float m_FlightEnvelope[15] = {0.0, 0.2, 0.9, 2.1, 2.5, 3.3, 3.5, 3.6, 3.7, 3.8, 3.8, 3.0, 0.9, 0.7, 0.5};

	bool m_IsInitialized;
	vector m_LastKnownPosition;
	bool m_IsLanded;
	vector m_Expansion_IsLandedHitPos;

	float m_Expansion_LastContactImpulse;

	void ExpansionHelicopterScript()
	{
		SetEventMask(EntityEvent.CONTACT | EntityEvent.SIMULATE);

		string path;

		path = "CfgVehicles " + GetType() + " SimulationModule maxSpeed";
		if (GetGame().ConfigIsExisting(path))
			m_MaxSpeed = GetGame().ConfigGetFloat(path);

		path = "CfgVehicles " + GetType() + " SimulationModule altitudeFullForce";
		if (GetGame().ConfigIsExisting(path))
			m_AltitudeFullForce = GetGame().ConfigGetFloat(path);

		path = "CfgVehicles " + GetType() + " SimulationModule altitudeNoForce";
		if (GetGame().ConfigIsExisting(path))
			m_AltitudeNoForce = GetGame().ConfigGetFloat(path);

		ExpansionVehicleHelicopter_OLD simulation = new ExpansionVehicleHelicopter_OLD(this);
		m_Simulation = simulation;
		AddModule(m_Simulation);

		RegisterNetSyncVariableFloat("m_Simulation.m_RotorSpeed");
	}

	override void AfterStoreLoad()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.CE, this, "LongDeferredInit");
#endif

		super.AfterStoreLoad();

		//! Fix for helis saved to storage before dmgZone for engine/fueltank/reflector were added
		if (GetExpansionSaveVersion() < 25)
		{
			if (GetHealth01("Engine", "") <= 0)
				SetHealth01("Engine", "", 1.0);

			if (GetHealth01("FuelTank", "") <= 0)
				SetHealth01("FuelTank", "", 1.0);

			if (GetHealth01("Reflector_1_1", "") <= 0)
				SetHealth01("Reflector_1_1", "", 1.0);
		}
	}

	void ~ExpansionHelicopterScript()
	{
	}

	override void LongDeferredInit()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "LongDeferredInit");
#endif

		super.LongDeferredInit();

		m_IsInitialized = true;

		if (IsMissionHost() && !IsLanded())
		{
			//TODO: teleport to land instead - but please not in autohover mode kthxbye

			//! Makes it land safely after server restart if pilot died/disconnected
			m_Simulation.m_RotorSpeed = 1;
			dBodyActive(this, ActiveState.ACTIVE);
		}
	}

	bool IsAutoHover()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsAutoHover");
#endif

		return m_Simulation.IsAutoHover();
	}

	void SwitchAutoHover()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "SwitchAutoHover");
#endif

		m_Simulation.SwitchAutoHover();
	}

	bool IsFreeLook()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsFreeLook");
#endif

		return m_Simulation.IsFreeLook();
	}

	override void SwitchGear()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "SwitchGear");
#endif

		Error("Not implemented!");
	}

	override void EOnContact(IEntity other, Contact extra)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.VEHICLES, this, "EOnContact").Add(other).Add(extra);
#endif

		//! Expansion helis do not receive vanilla OnContact (for some reason?), but some 3rd party ones do.
		//! Only call OnContact if impulse is different from last impulse handled by OnContact.
		//! Call order Expansion helis: EOnContact (called by base game), then OnContact (called by Expansion EOnContact)
		//! Call order 3rd party helis: OnContact (called by base game), then EOnContact (same)
		if (extra.Impulse && extra.Impulse != m_Expansion_LastContactImpulse)
		{
			OnContact("", WorldToModel(extra.Position), other, extra);
			m_Expansion_LastContactImpulse = 0;
		}
	}

	override void OnContact(string zoneName, vector localPos, IEntity other, Contact data)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_4(ExpansionTracing.VEHICLES, this, "EOnContact").Add(zoneName).Add(localPos).Add(other).Add(data);
#endif

		if (!m_IsInitialized)
			return;

		if (m_Expansion_IsBeingTowed)
			return;

		bool resetImpulse = GetGame().IsServer() && !IsDamageDestroyed();

		if (resetImpulse)
		{
			m_Expansion_LastContactImpulse = data.Impulse;

			vector transform[4];
			GetTransform(transform);

			//! Unfortunately GetTransform[1] sometimes returns "0.7 -0.7 0" sometimes on flat terrain "0 1 0"
			//! In game rendering does not show this behaviour and the helicopter appears to only translate, not rotate
			//! This is possibly a DayZ SA/Enfusion bug but it will need more testing. May also be the cause for some
			//! helicoper simulation weirdness when on the ground?

			float dot = vector.Dot(transform[1], vector.Up);
			float dotMO = dot - 1.0;

			const float maxVelocityMagnitude = 11.0; // ~40km/h
			float impulseRequired = m_State.m_Mass * maxVelocityMagnitude * ((dotMO * dotMO * dotMO) + 1.0) * 40.0;

			if (other) //! check done just incase
				impulseRequired += Math.Max(dBodyGetMass(other), 0.0) * maxVelocityMagnitude * 2.0;

			if (data.Impulse > impulseRequired || (m_Simulation.m_RotorSpeed > 0 && data.RelativeVelocityBefore.Length() >= maxVelocityMagnitude && !IsLanded()))
			{
#ifdef EXPANSIONVEHICLELOG
				Print(dot);
				Print(impulseRequired);
				Print(other);
				Print(data.Impulse);
				Print(GetVelocity(this));
				Print(dBodyGetAngularVelocity(this));
#endif
				resetImpulse = false;
			}
		}

		if (resetImpulse)
			data.Impulse = 0;  //! Make sure we take no damage while conditions are not met

		super.OnContact(zoneName, localPos, other, data);
	}

	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);

		bool isGlobal = !dmgZone || dmgZone == "GlobalHealth";

		float dmg = damageResult.GetDamage(dmgZone, "");

		//! Apply additional dmg if ammo type is grenade or rocket
		//! One direct hit with a rocket blows up a MH6, two direct hits blow up a Merlin
		//! Grenades take several more
		float additionalDmg;
		bool explode;
		switch (ammo)
		{
			case "RGD5Grenade_Ammo":  //! Base dmg = 50
			case "ExpansionRocket_Ammo":  //! Base dmg = 300
				additionalDmg = dmg * 17 - dmg;
				break;
			case "EnviroDmg":
				//! Never explode on collision
				break;
			default:
				//! Explode if base dmg exceeded heli max health, or 1 in 50 chance if it exceeded current heli health
				explode = isGlobal && (dmg > GetMaxHealth(dmgZone, "") || (Math.RandomInt(0, 50) < 1 && IsDamageDestroyed()));
				break;
		}

		if (additionalDmg)
		{
			//! Explode if additional dmg exceeds current health
			explode = isGlobal && additionalDmg > GetHealth(dmgZone, "");

			DecreaseHealth(dmgZone, "", additionalDmg);
		}

		if (isGlobal)
		{
			//! Always damage engine proportionally when taking global damage
			float engineMaxHealth = GetMaxHealth("Engine", "");
			float engineHealth = GetHealth("Engine", "");
			float engineHealthNew = engineMaxHealth * GetHealth01(dmgZone, "");
			if (engineHealthNew < engineHealth)
			{
				SetHealth("Engine", "", engineHealthNew);
			}
		}

		//! If explosions are disabled, the heli will just start burning once its health is depleted
		if (explode && m_Simulation.m_EnableHelicopterExplosions)
			Explode(DT_EXPLOSION, ammo);
	}

	// ------------------------------------------------------------
	override void ExpansionOnExplodeServer(int damageType, string ammoType)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.VEHICLES, this, "ExpansionOnExplodeServer").Add(damageType).Add(ammoType);
#endif

		super.ExpansionOnExplodeServer(damageType, ammoType);


		//! Seated players
		Human crew;
		for (int i = 0; i < CrewSize(); i++)
		{
			crew = CrewMember(i);

			if (!crew)
				continue;

			if (!crew.IsAlive())
				CrewDeath(i);

			Expansion_CrewGetOut(i);
		}

		//! Attached players
		IEntity child = GetChildren();
		PlayerBase player;
		while (child)
		{
			if (Class.CastTo(player, child))
			{
				//! Need to get sibling before unlinking
				child = child.GetSibling();

				Expansion_PlayerUnlinkFall(player);
			}
			else
			{
				child = child.GetSibling();
			}
		}

		Expansion_CreateWreck();
	}

	void Expansion_CreateWreck()
	{
		vector position = GetPosition();
		vector orientation = GetOrientation();

		ExpansionWreck wreck;
		vector modelBottomPos = ModelToWorld(Vector(0, -GetModelZeroPointDistanceFromGround(), 0));
		position[1] = modelBottomPos[1] + 1;
		if (Class.CastTo(wreck, GetGame().CreateObjectEx(GetWreck(), position, ECE_CREATEPHYSICS | ECE_UPDATEPATHGRAPH)))
		{
			wreck.SetPosition(position);
			wreck.SetOrientation(orientation);

			wreck.CreateDynamicPhysics(PhxInteractionLayers.DYNAMICITEM);
			wreck.SetDynamicPhysicsLifeTime(60);
			wreck.EnableDynamicCCD(true);

			wreck.SetOffset(GetWreckOffset());
			wreck.SetAltitude(GetWreckAltitude());

			wreck.SetHealth(0.0);

			dBodySetMass(wreck, dBodyGetMass(this));

			//! If we are not simulating, these can be 0 and would prematurely disable wreck physics
			if (m_State.m_Mass > 0 && m_State.m_LinearAcceleration.Length() > 0)
			{
				vector inertiaM[3];
				dBodyGetInvInertiaTensorWorld(this, inertiaM);
				dBodySetInertiaTensorM(wreck, inertiaM);
				dBodySetInertiaTensorV(wreck, dBodyGetLocalInertia(this));

				SetVelocity(wreck, m_State.m_LinearVelocity);
				dBodySetAngularVelocity(wreck, m_State.m_AngularVelocity);

				dBodyApplyForce(wreck, m_State.m_LinearAcceleration * m_State.m_Mass);
			}

			array<Object> objects = new array<Object>;
			array<CargoBase> proxy = new array<CargoBase>;
			GetGame().GetObjectsAtPosition(GetPosition(), 10, objects, proxy);

			for (int n = 0; n < objects.Count(); ++n)
			{
				Object obj = objects[n];

				TreeHard treeHard;
				TreeSoft treeSoft;
				BushHard bushHard;
				BushSoft bushSoft;
				if (Class.CastTo(treeHard, obj))
				{
					GetGame().RPCSingleParam(wreck, PlantType.TREE_HARD, new Param1<vector>(obj.GetPosition()), true);
				}
				else if (Class.CastTo(treeSoft, obj))
				{
					GetGame().RPCSingleParam(wreck, PlantType.TREE_SOFT, new Param1<vector>(obj.GetPosition()), true);
				}
				else if (Class.CastTo(bushHard, obj))
				{
					GetGame().RPCSingleParam(wreck, PlantType.BUSH_HARD, new Param1<vector>(obj.GetPosition()), true);
				}
				else if (Class.CastTo(bushSoft, obj))
				{
					GetGame().RPCSingleParam(wreck, PlantType.BUSH_SOFT, new Param1<vector>(obj.GetPosition()), true);
				}
				else
				{
					continue;
				}

				//! Deforestation is a go
				if (obj.GetHealth("", "") > 0)
				{
					obj.SetHealth("", "", 0);

					if (dBodyIsSet(obj))
						dBodyDestroy(obj);

					GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(GetGame().ObjectDelete, obj);
				}
			}

			//! Delay removal of heli so killfeed can show "killed by" (needs the object to still exist)
			dBodyActive(this, ActiveState.INACTIVE);
			dBodyDynamic(this, false);
			SetPosition("0 0 0");
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(GetGame().ObjectDelete, 5000, false, this);
		}
	}

	override void ExpansionOnExplodeClient(int damageType, string ammoType)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.VEHICLES, this, "ExpansionOnExplodeClient").Add(damageType).Add(ammoType);
#endif

		super.ExpansionOnExplodeClient(damageType, ammoType);

		PlayerBase player;
		if (!IsMissionOffline() && Class.CastTo(player, GetGame().GetPlayer()) && player.GetParent() == this)
		{
			bool isCrew;

			for (int i = 0; i < CrewSize(); i++)
			{
				if (CrewMember(i) == player)
				{
					isCrew = true;

					if (!player.IsAlive())
						CrewDeath(i);

					Expansion_CrewGetOut(i);
					break;
				}
			}

			if (!isCrew)
				Expansion_PlayerUnlinkFall(player);
		}
	}

	protected void Expansion_CrewGetOut(int i)
	{
		PlayerBase player = PlayerBase.Cast(CrewMember(i));

		CrewGetOut(i);

		if (player)
			Expansion_PlayerUnlinkFall(player, true);
	}

	protected void Expansion_PlayerUnlinkFall(PlayerBase player, bool replaceWithDummy = false)
	{
		player.UnlinkFromLocalSpace();
		player.DisableSimulation(false);

		if (replaceWithDummy)
		{
			Expansion_ReplaceDeadPlayerWithDummy(player);
		}
		else
		{
			if (!player.IsAlive())
				dBodySetInteractionLayer(player, PhxInteractionLayers.RAGDOLL);

			//! Needs to be called at least one simulation frame (25ms) later
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(player.StartCommand_Fall, 25, false, 0);
		}
	}

	protected void Expansion_ReplaceDeadPlayerWithDummy(PlayerBase player)
	{
		if (!player.IsAlive())
		{
			vector position = player.GetPosition();

			vector ground = ExpansionStatic.GetSurfaceWaterPosition(position);
			vector modelBottomPos = ModelToWorld(Vector(0, -GetModelZeroPointDistanceFromGround(), 0));
			vector start = Vector(position[0], modelBottomPos[1] - 0.1, position[2]);
			vector end = Vector(position[0], ground[1], position[2]);

			PhxInteractionLayers layerMask;
			layerMask |= PhxInteractionLayers.BUILDING;
			layerMask |= PhxInteractionLayers.VEHICLE;
			layerMask |= PhxInteractionLayers.ITEM_LARGE;
			layerMask |= PhxInteractionLayers.ROADWAY;
			layerMask |= PhxInteractionLayers.TERRAIN;
			layerMask |= PhxInteractionLayers.WATERLAYER;

			vector hitPosition;

			if (DayZPhysics.RayCastBullet( start, end, layerMask, player, NULL, hitPosition, NULL, NULL))
				ground = hitPosition;

			vector dummySpawnPosition = position;
			//if (position[1] - ground[1] <= 2.53)
				//dummySpawnPosition = ground;  //! If we died close to ground, spawn dummy on ground

			EntityAI dummy = EntityAI.Cast(GetGame().CreateObject(player.GetType(), dummySpawnPosition));

			ExpansionTransferInventory(player, dummy, true);

			vector velocity = GetVelocity(this);
			vector force = velocity * dBodyGetMass(dummy);
			dBodyApplyImpulse(dummy, force);

			float fallHeight = dummySpawnPosition[1] - ground[1];

			if (fallHeight > 2.53)
			{
				//! Can't set health to 0, can leave our dummy in a visually awkward standing pose after falling.
				//! Set health to 1 so the fall will kill it with a high probability (2.53m is roughly the height at which a player will start to take fall damage).
				dummy.SetHealth(1);
			}

			//! Just in case fall doesn't kill our dummy (maybe because we died on ground), kill it later.
			//! This may look a little awkward, but at least makes sure it's dead.
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(dummy.ProcessDirectDamage, Math.Sqrt((fallHeight * 2) / 9.81) * 1000, false, DT_CUSTOM, this, "", "FallDamage", "0 0 0", dummy.GetMaxHealth());

			//! Needs to be called at least two simulation frames (50ms) later
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(player.Delete, 50, false);
		}
	}

	protected override void Expansion_OnBeforeApplyPhysics(ExpansionPhysicsState pState)
	{
		if (IsLanded())
		{
			//! Bouncing/jolting/flipping fix
			//! 0% momentum at zero vertical distance to ground and/or rotor off,
			//! 100% momentum at >= 0.5m vertical distance to ground and full rotor speed,
			//! linearly interpolated
			float f = m_Simulation.m_RotorSpeed;
			if (f > 0)
			{
				vector modelBottomPos = ModelToWorld(Vector(0, -GetModelZeroPointDistanceFromGround(), 0));
				f *= ExpansionMath.LinearConversion(0, 0.5, modelBottomPos[1] - m_Expansion_IsLandedHitPos[1], 0, 1, true);
			}
			pState.m_LinearVelocity = pState.m_LinearVelocity * m_Simulation.m_RotorSpeed;
			pState.m_AngularVelocity = pState.m_AngularVelocity * f;
		}
	}

	override float OnSound(CarSoundCtrl ctrl, float oldValue)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.VEHICLES, this, "OnSound").Add(ctrl).Add(oldValue);
#endif

		if (m_State.m_Exploded)
			return 0;

		switch (ctrl)
		{
		case CarSoundCtrl.SPEED:
		{
			return m_Simulation.m_RotorSpeed; // this should just be the velocity length, maybe in KM/h
		}
		case CarSoundCtrl.RPM:
		{
			return m_Simulation.m_RotorSpeed;
		}
		case CarSoundCtrl.ENGINE:
		{
			if (m_Simulation.m_RotorSpeedTarget > 0)
				return 1;

			return 0;
		}
		}

		return oldValue;
	}

	override void SetActions()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "SetActions");
#endif

		super.SetActions();

		AddAction(ExpansionActionSwitchAutoHover);
		AddAction(ExpansionActionSwitchAutoHoverInput);
	}

	override int GetAnimInstance()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetAnimInstance");
#endif

		return ExpansionVehicleAnimInstances.EX_HATCHBACK;
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

	override bool CanReachSeatFromDoors(string pSeatSelection, vector pFromPos, float pDistance = 1.0)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_3(ExpansionTracing.VEHICLES, this, "CanReachSeatFromDoors").Add(pSeatSelection).Add(pFromPos).Add(pDistance);
#endif

		return true;
	}

	override int Get3rdPersonCameraType()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "Get3rdPersonCameraType");
#endif

		return DayZPlayerCameras.DAYZCAMERA_3RD_VEHICLE;
	}

	override bool CrewCanGetThrough(int posIdx)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "CrewCanGetThrough").Add(posIdx);
#endif

		return true;
	}

	override bool CanReachDoorsFromSeat(string pDoorsSelection, int pCurrentSeat)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.VEHICLES, this, "CanReachDoorsFromSeat").Add(pDoorsSelection).Add(pCurrentSeat);
#endif

		return true;
	}

	override bool IsVitalHelicopterBattery()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsVitalHelicopterBattery");
#endif

		return true;
	}

	override bool IsVitalCarBattery()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsVitalHelicopterBattery");
#endif

		if (m_CarBatteryVanillaState)
		{
			return IsVitalHelicopterBattery() || IsVitalAircraftBattery();
		}

		return false;
	}

	bool IsLanded()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsLanded");
#endif

		vector pos = GetPosition();

		//! 1cm tolerance to last known position
		if (m_LastKnownPosition && vector.DistanceSq(pos, m_LastKnownPosition) < 0.0001)
			return m_IsLanded;

		m_LastKnownPosition = pos;

		vector start = ModelToWorld(Vector(0, -GetModelZeroPointDistanceFromGround() + 0.5, 0));
		vector end = ModelToWorld(Vector(0, -GetModelZeroPointDistanceFromGround() - 0.5, 0));

		float surfaceY = GetGame().SurfaceY(pos[0], pos[2]);
		if (start[1] - surfaceY < 1)
		{
			if (!m_IsLanded)
			{
#ifdef EXPANSIONEXPRINT
				EXPrint(ToString() + "::IsLanded surface");
#endif

				m_Expansion_IsLandedHitPos = Vector(pos[0], surfaceY, pos[2]);

				m_IsLanded = true;
			}

			return true;
		}

		vector hitNormal;
		int hitindex;
		set<Object> hitObjects = new set<Object>;

		bool hit = DayZPhysics.RaycastRV(start, end, m_Expansion_IsLandedHitPos, hitNormal, hitindex, hitObjects, NULL, this);

		if (hit && hitObjects.Count() > 0)
		{
			//! Ignore objects that are children (pilot, passengers...)
			int hitChildCount;
			foreach (Object hitObject: hitObjects)
			{
				if (hitObject.GetParent() == this)
					hitChildCount++;
			}
			hit = hitChildCount < hitObjects.Count();
		}

#ifdef EXPANSIONEXPRINT
		if (hit != m_IsLanded)
			EXPrint(ToString() + "::IsLanded " + hit);
#endif

		m_IsLanded = hit;

		return m_IsLanded;
	}

	override bool Expansion_CanSimulate()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "Expansion_CanSimulate");
#endif

		if ((GetGame().IsServer() && GetGame().IsMultiplayer()) && !m_IsInitialized)
			return false;

		return true;
	}

	override bool Expansion_ShouldDisableSimulation()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "Expansion_ShouldDisableSimulation");
#endif

		//! NEVER return true for helis as it it interferes with vanilla collision handling and may cause the heli to get pushed into the ground
		//! if the heli gets deactivated in CarScript::EOnSimulate if no driver while vanilla collision code is still running.
		//! (CarScript doesn't have collision in inactive state, so will move through terrain as if it weren't there if it's pushed by an outside force).
		//! Vanilla WILL deactivate helis that are not in use the same as it does cars, so there is no need for us to do that explicitly.
		return false;
	}

	override bool IsVitalSparkPlug()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsVitalSparkPlug");
#endif

		return false;
	}

	override bool IsVitalRadiator()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsVitalRadiator");
#endif

		return false;
	}

	override bool IsVitalGlowPlug()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsVitalGlowPlug");
#endif

		return false;
	}

	override bool IsVitalIgniterPlug()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsVitalIgniterPlug");
#endif

		return false;
	}

	override bool IsVitalHydraulicHoses()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsVitalHydraulicHoses");
#endif

		return false;
	}

	override bool IsVitalEngineBelt()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsVitalEngineBelt");
#endif

		return false;
	}

	override bool IsVitalTruckBattery()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsVitalTruckBattery");
#endif

		return false;
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

	override float GetCameraHeight()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetCameraHeight");
#endif

		return 5;
	}

	override float GetCameraDistance()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetCameraDistance");
#endif

		return 15;
	}

	override bool IsHelicopter()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsHelicopter");
#endif

		return true;
	}

	override bool IsCar()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "IsCar");
#endif

		return false;
	}

	float GetAutoHoverTargetHeight()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetAutoHoverTargetHeight");
#endif

		return m_Simulation.m_AutoHoverAltitude;
	}

	override int Expansion_NumberTowConnections()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "GetAutoHoverTargetHeight");
#endif

		return 1;
	}

	override void Expansion_GetTowConnection(int index, out vector position, out vector size)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "Expansion_GetTowConnection").Add(index);
#endif

		vector minMax[2];
		GetCollisionBox(minMax);

		position = Vector(0.0, minMax[1][1] + Expansion_GetTowLength(), 0.0);
		size = "2.0 2.0 2.0";
	}

	override vector Expansion_GetTowPosition()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "Expansion_GetTowPosition");
#endif

		vector minMax[2];
		GetCollisionBox(minMax);

		return Vector(0.0, minMax[0][1] - Expansion_GetTowLength(), 0.0);
	}

	override vector Expansion_GetTowDirection()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "Expansion_GetTowDirection");
#endif

		vector transform[4];
		GetTransform(transform);
		return -transform[1];
	}

	override float Expansion_GetTowLength()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "Expansion_GetTowLength");
#endif

		return 5.0;
	}

	//! Is it already towing something ? And is it locked ?
	override bool Expansion_CanConnectTow(notnull Object other)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.VEHICLES, this, "Expansion_CanConnectTow").Add(other);
#endif

		ItemBase item;
		ExpansionVehicleBase evs;
		CarScript cs;
		if (Class.CastTo(evs, other))
		{
			return evs.Expansion_NumberTowConnections() > 0 && !evs.Expansion_IsTowing() && !evs.IsLocked();
		}
		else if (Class.CastTo(cs, other))
		{
			return cs.Expansion_NumberTowConnections() > 0 && !cs.Expansion_IsTowing() && !cs.IsLocked();
		}
		else if (Class.CastTo(item, other))
		{
			return item.Expansion_NumberTowConnections() > 0 && !item.Expansion_IsTowing();
		}

		//! don't...
		return false;
	}
};
