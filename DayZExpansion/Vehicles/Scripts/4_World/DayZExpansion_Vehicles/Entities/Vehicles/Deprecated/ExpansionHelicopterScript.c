/**
 * ExpansionHelicopterScript.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
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
		super.LongDeferredInit();

		m_IsInitialized = true;

		if (IsMissionHost() && !IsLanded())
		{
			//TODO: teleport to land instead

			//! Makes it land safely after server restart if pilot died/disconnected
			dBodyActive(this, ActiveState.ACTIVE);
		}
	}

	bool IsAutoHover()
	{
		return m_Simulation.IsAutoHover();
	}

	void SwitchAutoHover()
	{
		m_Simulation.SwitchAutoHover();
	}

	bool IsFreeLook()
	{
		return m_Simulation.IsFreeLook();
	}

	override void SwitchGear()
	{
		Error("Not implemented!");
	}

	override void EOnContact(IEntity other, Contact extra)
	{
		if (m_Expansion_IsBeingTowed)
			return;

		if (IsMissionHost() && m_Simulation.m_EnableHelicopterExplosions && CanBeDamaged())
		{
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

			if (extra.Impulse > impulseRequired)
			{
#ifdef EXPANSIONVEHICLELOG
				Print(dot);
				Print(impulseRequired);
				Print(other);
				Print(extra.Impulse);
				Print(GetVelocity(this));
				Print(dBodyGetAngularVelocity(this));
#endif
				//Print( "Boom!" );
				//! Maybe instead just tick damage down instead?
				//! Should have a way to repair the helicopter then though
				Explode(DT_EXPLOSION, "RGD5Grenade_Ammo");
			}
		}
	}

	// ------------------------------------------------------------
	override void ExpansionOnExplodeServer(int damageType, string ammoType)
	{
		vector position = GetPosition();
		vector orientation = GetOrientation();

		//SetInvisible( true );

		EntityAI attachment;
		for (int j = 0; j < GetInventory().AttachmentCount(); j++)
		{
			attachment = GetInventory().GetAttachmentFromIndex(j);

			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(GetGame().ObjectDelete, 0, false, attachment);
		}

		PlayerBase player;
		DayZPlayerCommandDeathCallback callback;

		for (int i = 0; i < CrewSize(); i++)
		{
			if (Class.CastTo(player, CrewMember(i)))
			{
				player.StartCommand_Fall(0);

				CrewDeath(i);
				CrewGetOut(i);

				player.UnlinkFromLocalSpace();

				player.SetAllowDamage(true);
				player.SetHealth(0.0);

				dBodySetInteractionLayer(player, PhxInteractionLayers.RAGDOLL);

				RemoveChild(player);
			}
		}

		IEntity child = GetChildren();
		while (child)
		{
			if (Class.CastTo(player, child))
			{
				child = child.GetSibling();

				player.UnlinkFromLocalSpace();
				RemoveChild(player);

				player.SetAllowDamage(true);
				player.SetHealth(0.0);

				dBodySetInteractionLayer(player, PhxInteractionLayers.RAGDOLL);

				player.StartCommand_Fall(0);
			}
			else
			{
				child = child.GetSibling();
			}
		}

		ExpansionWreck wreck;
		if (Class.CastTo(wreck, GetGame().CreateObjectEx(GetWreck(), position + "0 2.5 0", ECE_CREATEPHYSICS | ECE_UPDATEPATHGRAPH)))
		{
			wreck.SetPosition(position + "0 2.5 0");
			wreck.SetOrientation(orientation);

			wreck.CreateDynamicPhysics(PhxInteractionLayers.DYNAMICITEM);
			wreck.SetDynamicPhysicsLifeTime(60);
			wreck.EnableDynamicCCD(true);

			wreck.SetOffset(GetWreckOffset());
			wreck.SetAltitude(GetWreckAltitude());

			wreck.SetHealth(0.0);
			dBodySetMass(wreck, m_State.m_Mass);

			vector inertiaM[3];
			dBodyGetInvInertiaTensorWorld(this, inertiaM);
			dBodySetInertiaTensorM(wreck, inertiaM);
			dBodySetInertiaTensorV(wreck, dBodyGetLocalInertia(this));

			SetVelocity(wreck, m_State.m_LinearVelocity);
			dBodySetAngularVelocity(wreck, m_State.m_AngularVelocity);

			dBodyApplyForce(wreck, m_State.m_LinearAcceleration * m_State.m_Mass);

			// GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( MiscGameplayFunctions.TransferInventory, 1, false, this, wreck, playerForTransfer );

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

				if (obj.GetHealth("", "") > 0)
				{
					obj.SetHealth("", "", 0);

					if (dBodyIsSet(obj))
						dBodyDestroy(obj);

					GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(GetGame().ObjectDelete, obj);
				}
			}

			super.ExpansionOnExplodeServer(damageType, ammoType);

			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(GetGame().ObjectDelete, this);
		}
		else
		{
			super.ExpansionOnExplodeServer(damageType, ammoType);
		}
	}

	override void ExpansionOnExplodeClient(int damageType, string ammoType)
	{
		super.ExpansionOnExplodeClient(damageType, ammoType);

		if (!IsMissionOffline() && GetGame().GetPlayer().GetParent() == this)
		{
			GetGame().GetPlayer().UnlinkFromLocalSpace();
			GetGame().GetPlayer().StartCommand_Fall(0);
		}
	}

	override float OnSound(CarSoundCtrl ctrl, float oldValue)
	{
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
		super.SetActions();

		AddAction(ExpansionActionSwitchAutoHover);
		AddAction(ExpansionActionSwitchAutoHoverInput);
	}

	override int GetAnimInstance()
	{
		return ExpansionVehicleAnimInstances.EX_HATCHBACK;
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

	override bool CanReachSeatFromDoors(string pSeatSelection, vector pFromPos, float pDistance = 1.0)
	{
		return true;
	}

	override int Get3rdPersonCameraType()
	{
		return DayZPlayerCameras.DAYZCAMERA_3RD_VEHICLE;
	}

	override bool CrewCanGetThrough(int posIdx)
	{
		return true;
	}

	override bool CanReachDoorsFromSeat(string pDoorsSelection, int pCurrentSeat)
	{
		return true;
	}

	override bool IsVitalHelicopterBattery()
	{
		return true;
	}

	override bool IsVitalCarBattery()
	{
		if (m_CarBatteryVanillaState)
		{
			return IsVitalHelicopterBattery() || IsVitalAircraftBattery();
		}

		return false;
	}

	bool IsLanded()
	{
#ifdef EXPANSIONEXPRINT
		EXLogPrint("ExpansionHelicopterScript::IsLanded - Start");
#endif

		vector pos = GetPosition();

		if (m_LastKnownPosition && vector.Distance(pos, m_LastKnownPosition) < 0.01)
			return m_IsLanded;

		m_LastKnownPosition = pos;

		float offset = 0.5;

		//! Add offset if pitch or roll are out of whack
		vector ori = GetOrientation();

		if (ori[1] >= 45 || ori[1] <= -45 || ori[2] >= 45 || ori[2] <= -45)
			offset += 10;

		//! Ray input
		vector start = pos;
		vector end = pos - Vector(0, GetModelAnchorPointY() + offset, 0);

		//! Ray output
		vector hit;
		vector hitpos;

		//! Ray hitindex output
		int hitindex;

		//! Ray
		m_IsLanded = DayZPhysics.RaycastRV(start, end, hitpos, hit, hitindex, NULL, NULL, this);

#ifdef EXPANSIONEXPRINT
		EXLogPrint(GetType() + "::IsLanded - End and return " + m_IsLanded);
#endif

		return m_IsLanded;
	}

	override bool CanSimulate()
	{
		//! Need to simulate for at least one frame, otherwise funky DayZ physics make heli explode when getting in
		if (m_IsInitialized && IsLanded())
		{
			//! Only simulate if rotor speed is above zero
			//! Prevents premature stop of rotor animation and smoke particle on client
			//! Prevents heli bouncing around when not in use

			return m_Simulation.m_RotorSpeed > 0 || m_Simulation.m_RotorSpeedTarget > 0;
		}
		else
		{
			return dBodyIsActive(this) && dBodyIsDynamic(this);
		}
	}

	override bool IsVitalSparkPlug()
	{
		return true;
	}

	override bool IsVitalRadiator()
	{
		return false;
	}

	override bool IsVitalGlowPlug()
	{
		return false;
	}

	bool IsVitalIgniterPlug()
	{
		return false;
	}

	bool IsVitalHydraulicHoses()
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

	override CarLightBase CreateFrontLight()
	{
		return CarLightBase.Cast(ScriptedLightBase.CreateLight(ExpansionHelicopterFrontLight));
	}

	override CarLightBase CreateRearLight()
	{
		return CarRearLightBase.Cast(ScriptedLightBase.CreateLight(ExpansionRearHelicopterLights));
	}

	override float GetCameraHeight()
	{
		return 5;
	}

	override float GetCameraDistance()
	{
		return 15;
	}

	override bool IsHelicopter()
	{
		return true;
	}

	override bool IsCar()
	{
		return false;
	}

	float GetAutoHoverTargetHeight()
	{
		return m_Simulation.m_AutoHoverAltitude;
	}

	override bool Expansion_IsBeingTowed()
	{
		return m_Expansion_IsBeingTowed;
	}

	override bool Expansion_IsTowing()
	{
		return m_Expansion_IsTowing;
	}

	override int Expansion_NumberTowConnections()
	{
		return 1;
	}

	override void Expansion_GetTowConnection(int index, out vector position, out vector size)
	{
		vector minMax[2];
		GetCollisionBox(minMax);

		position = Vector(0.0, minMax[1][1] + Expansion_GetTowLength(), 0.0);
		size = "2.0 2.0 2.0";
	}

	override vector Expansion_GetTowPosition()
	{
		vector minMax[2];
		GetCollisionBox(minMax);

		return Vector(0.0, minMax[0][1] - Expansion_GetTowLength(), 0.0);
	}

	override vector Expansion_GetTowDirection()
	{
		vector transform[4];
		GetTransform(transform);
		return -transform[1];
	}

	override float Expansion_GetTowLength()
	{
		return 5.0;
	}

	//! Is it already towing something ? And is it locked ?
	override bool Expansion_CanConnectTow(notnull Object other)
	{
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
