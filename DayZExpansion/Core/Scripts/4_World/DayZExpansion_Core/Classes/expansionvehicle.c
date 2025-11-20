/**
 * @brief Wrapper class for vehicle functionality that is shared among vehicles inheriting from otherwise unrelated classes
 */
class ExpansionVehicle
{
	static ref CF_DoublyLinkedNodes_WeakRef<ExpansionVehicle> s_All = new CF_DoublyLinkedNodes_WeakRef<ExpansionVehicle>();

	ref CF_DoublyLinkedNode_WeakRef<ExpansionVehicle> m_Node;

	protected bool m_IsBike;
	protected bool m_IsBoat;
	protected bool m_IsCar;
	protected bool m_IsHelicopter;
	protected bool m_IsPlane;
	protected bool m_IsDuck;

	protected bool m_IsCECreated;
	protected bool m_DestructionLogged;
	protected bool m_HasLifetime;
	protected int m_CargoCount;

	protected bool m_IsInSafeZone;
	protected bool m_SZCleanup;
	protected float m_SZParkingTime;

	protected string m_LastDriverUID;
	protected bool m_SynchLastDriverUID;
	protected bool m_LastDriverUIDSynched;

	protected float m_CrewKilledTimestamp;

	protected float m_FuelConsumption;
#ifdef DIAG_DEVELOPER
	protected float m_FuelConsumption_Prev;
#endif

	void ExpansionVehicle()
	{
		m_Node = s_All.Add(this);
	}

	void ~ExpansionVehicle()
	{
	#ifndef DIAG_DEVELOPER
		if (!g_Game)
			return;
	#endif

		if (s_All)
			s_All.Remove(m_Node);
	}

	static ExpansionVehicle Get(IEntity entity, bool errorOnFailure = false, bool includeAttached = false)
	{
		BoatScript bs;
		CarScript cs;
		//HelicopterScript hs;
	#ifdef EXPANSIONMODVEHICLE
		ExpansionVehicleBase evb;
	#endif
		DayZPlayerImplement player;

		if (Class.CastTo(cs, entity))
			return cs.GetExpansionVehicle();
		else if (Class.CastTo(bs, entity))
			return bs.GetExpansionVehicle();
		//else if (Class.CastTo(hs, entity))
			//return new ExpansionVehicleT<HelicopterScript>(hs);
	#ifdef EXPANSIONMODVEHICLE
		else if (Class.CastTo(evb, entity))
			return evb.GetExpansionVehicle();
	#endif
		else if (Class.CastTo(player, entity))
		{
			if (includeAttached)
			{
				//! Expansion_GetParent will both check GetParent (which will satisfy any players with
				//! VehicleCmd aka sitting in vehicle seats as well as custom player attachment systems of
				//! Expansion under DayZ 1.25 and 3rd party mods) as well as DayZ 1.26 vanilla PhysicsGetLinkedEntity
				return Get(player.Expansion_GetParent());
			}
			else
			{
				//! Don't use GetParent here since it is used for player attachment by Expansion (only under DayZ 1.25)
				//! and potentially other vehicle mods that do not use vanilla 1.26 attachment system
				auto vehCmd = player.GetCommand_Vehicle();
				if (vehCmd)
					return Get(vehCmd.GetTransport(), errorOnFailure);
			#ifdef EXPANSIONMODVEHICLE
				auto exVehCmd = player.GetCommand_ExpansionVehicle();
				if (exVehCmd)
					return Get(exVehCmd.GetObject(), errorOnFailure);
			#endif
			}
		}
		

		if (errorOnFailure)
			EXError.Error(null, string.Format("%1 is not of a known vehicle type", entity));

		return null;
	}

	static bool Get(out ExpansionVehicle ev, IEntity entity, bool errorOnFailure = false, bool includeAttached = false)
	{
		ev = Get(entity, errorOnFailure, includeAttached);

		if (!ev)
			return false;

		return true;
	}

	static ExpansionVehicleType GetVehicleType(string type)
	{
		string vehicleClass = g_Game.ConfigGetTextOut("CfgVehicles " + type + " vehicleClass");
		switch (vehicleClass)
		{
			case "Expansion_Helicopter":
			case "Expansion_Plane":
			case "Vehicles_Helicopter":
			case "Vehicles_Plane":
				return ExpansionVehicleType.AIR;
			case "Expansion_Boat":
			case "Expansion_Ship":
			case "Vehicles_Boat":
			case "Vehicles_Ship":
				return ExpansionVehicleType.WATER;
		#ifdef HypeTrain
			case "HypeTrain_Locomotive":
			case "HypeTrain_Wagon":
				return ExpansionVehicleType.RAILTRACK;
		#endif
		}

		if (g_Game.IsKindOf(type, "BoatScript"))
			return ExpansionVehicleType.WATER;

		if (g_Game.IsKindOf(type, "CarScript"))
			return ExpansionVehicleType.LAND;

		return ExpansionVehicleType.NONE;
	}

	void DeferredInit()
	{
		EntityAI entity = GetEntity();
		if (IsStoreLoaded() && !entity.IsSetForDeletion() && ExpansionEntityStoragePlaceholder.Expansion_HasPlaceholder(this))
		{
			EXPrint("Deleting " + entity + " " + entity.GetPosition() + " global ID " + GetGlobalID().IDToString());
			entity.Delete();
		}
	}

	void SetActions()
	{
	}

	void Init()
	{
	}

	//! -----------------------------------------------------------------------

	EntityAI GetEntity()
	{
		EXError.Error(this, "NOT IMPLEMENTED");
		return null;
	}

	Transport GetTransport()
	{
		EXError.Error(this, "NOT IMPLEMENTED");
		return null;
	}

	BoatScript GetBoat()
	{
		return BoatScript.Cast(GetEntity());
	}

	CarScript GetCar()
	{
		return CarScript.Cast(GetEntity());
	}

	HelicopterScript GetHelicopter()
	{
		return HelicopterScript.Cast(GetEntity());
	}

	ExpansionGlobalID GetGlobalID()
	{
		EXError.Error(this, "NOT IMPLEMENTED");
		return null;
	}

#ifdef FEATURE_NETWORK_RECONCILIATION
	NetworkMoveStrategy GetNetworkMoveStrategy()
	{
		EXError.Error(this, "NOT IMPLEMENTED");
		return NetworkMoveStrategy.NONE;
	}
#endif

	string GetPersistentIDString()
	{
		return ExpansionStatic.GetPersistentIDString(GetEntity());
	}

	vector GetOrientation()
	{
		return GetEntity().GetOrientation();
	}

	vector GetPosition()
	{
		return GetEntity().GetPosition();
	}

	string GetType()
	{
		return GetEntity().GetType();
	}

	string GetDisplayName()
	{
		return GetEntity().GetDisplayName();
	}

	int GetHealthLevel(string zone = "")
	{
		return GetEntity().GetHealthLevel(zone);
	}

	void SetSkin(int skinIndex)
	{
		EXError.Error(this, "NOT IMPLEMENTED");
	}

	bool HasSkin(int skinIndex)
	{
		EXError.Error(this, "NOT IMPLEMENTED");
		return false;
	}

	int GetCurrentSkinIndex()
	{
		EXError.Error(this, "NOT IMPLEMENTED");
		return -1;
	}

	string GetCurrentSkinName()
	{
		EXError.Error(this, "NOT IMPLEMENTED");
		return string.Empty;
	}

	void UpdateCargoCount(int delta)
	{
		m_CargoCount += delta;
	#ifdef SERVER
		EntityAI entity = GetEntity();
		if (entity.IsInitialized())
			entity.SetSynchDirty();
	#endif
	#ifdef DIAG_DEVELOPER
		EXTrace.Print(EXTrace.GENERAL_ITEMS, this, GetEntity().ToString() + " cargo count + " + delta + " = " + m_CargoCount);
	#endif
	}

	int GetCargoCount()
	{
		return m_CargoCount;
	}

	void FillFluid(int fluid, float amount)
	{
		EXError.Error(this, "NOT IMPLEMENTED");
	}

	void FillFluids(float relativeAmount = 1.0)
	{
		EXError.Error(this, "NOT IMPLEMENTED");
	}

	void CloneFluidsTo(ExpansionVehicle dst)
	{
		EXError.Error(this, "NOT IMPLEMENTED");
	}

	void LeakFluids()
	{
		EXError.Error(this, "NOT IMPLEMENTED");
	}

	float GetFluidCapacity(int fluid)
	{
		EXError.Error(this, "NOT IMPLEMENTED");
		return 0;
	}

	float GetFluidFraction(int fluid)
	{
		EXError.Error(this, "NOT IMPLEMENTED");
		return 0;
	}

	void WriteFluidsTo(ParamsWriteContext ctx)
	{
		EXError.Error(this, "NOT IMPLEMENTED");
	}

	bool ReadFluidsFrom(ParamsReadContext ctx, int entityStorageVersion)
	{
		EXError.Error(this, "NOT IMPLEMENTED");
		return false;
	}

	void Synchronize()
	{
		EXError.Error(this, "NOT IMPLEMENTED");
	}

	bool IsBike()
	{
		return m_IsBike;
	}

	bool IsBoat()
	{
		return m_IsBoat;
	}

	bool IsCar()
	{
		return m_IsCar;
	}

	bool IsHelicopter()
	{
		return m_IsHelicopter;
	}

	bool IsPlane()
	{
		return m_IsPlane;
	}

	bool IsDuck()
	{
		return m_IsDuck;
	}

	bool IsCECreated()
	{
		return m_IsCECreated;
	}

	bool IsDestroyed()
	{
		return GetEntity().IsDamageDestroyed();
	}

	bool IsExploded()
	{
		EXError.Error(this, "NOT IMPLEMENTED");
		return false;
	}

	bool IsScriptedLightsOn()
	{
		EXError.Error(this, "NOT IMPLEMENTED");
		return false;
	}

	bool IsVitalAircraftBattery()
	{
		EXError.Error(this, "NOT IMPLEMENTED");
		return false;
	}

	bool IsVitalCarBattery()
	{
		EXError.Error(this, "NOT IMPLEMENTED");
		return false;
	}

	bool IsVitalHelicopterBattery()
	{
		EXError.Error(this, "NOT IMPLEMENTED");
		return false;
	}

	bool IsVitalTruckBattery()
	{
		EXError.Error(this, "NOT IMPLEMENTED");
		return false;
	}

	int GetEngineStartAnimation()
	{
		EXError.Error(this, "NOT IMPLEMENTED");
		return 0;
	}

	int GetEngineStopAnimation()
	{
		EXError.Error(this, "NOT IMPLEMENTED");
		return 0;
	}

	bool OnBeforeEngineStart(int index = 0)
	{
		EXError.Error(this, "NOT IMPLEMENTED");
		return false;
	}

	void OnEngineStart(int index = 0)
	{
	}

	void OnEngineStop(int index = 0)
	{
	}

	bool EngineIsOn()
	{
		EXError.Error(this, "NOT IMPLEMENTED");
		return false;
	}

	bool EngineIsOn(int index)
	{
		EXError.Error(this, "NOT IMPLEMENTED");
		return false;
	}

	int EngineGetRunningAboveOrAtIdleRPMCount()
	{
		EXError.Error(this, "NOT IMPLEMENTED");
		return 0;
	}

	void EngineStart()
	{
		EXError.Error(this, "NOT IMPLEMENTED");
	}

	void EngineStop()
	{
		EXError.Error(this, "NOT IMPLEMENTED");
	}

	int EngineGetCount()
	{
		EXError.Error(this, "NOT IMPLEMENTED");
		return 0;
	}

	int EngineGetCurrent()
	{
		EXError.Error(this, "NOT IMPLEMENTED");
		return -1;
	}

	void EngineSetNext()
	{
		EXError.Error(this, "NOT IMPLEMENTED");
	}

	string EngineGetName()
	{
		EXError.Error(this, "NOT IMPLEMENTED");
		return string.Empty;
	}

	float GetThrottle()
	{
		EXError.Error(this, "NOT IMPLEMENTED");
		return 0;
	}

	float GetThrottle(int index)
	{
		EXError.Error(this, "NOT IMPLEMENTED");
		return 0;
	}

	void ConsumeFuel(float fuelConsumption)
	{
		EXError.Error(this, "NOT IMPLEMENTED");
	}

	void ConsumeFuelOverTime(float dt)
	{
		EXError.Error(this, "NOT IMPLEMENTED");
	}

	bool HasGear()
	{
		EXError.Error(this, "NOT IMPLEMENTED");
		return false;
	}

	void SwitchGear()
	{
		EXError.Error(this, "NOT IMPLEMENTED");
	}

	bool IsInSafeZone()
	{
		return m_IsInSafeZone;
	}

	bool CanBeLocked()
	{
		return true;
	}

	bool CanBeDamaged()
	{
		if (GetExpansionSettings().GetSafeZone().Enabled && IsInSafeZone())
			return !GetExpansionSettings().GetSafeZone().DisableVehicleDamageInSafeZone;

		return GetEntity().GetAllowDamage();
	}

	bool CanObjectAttach(Object obj)
	{
		EXError.Error(this, "NOT IMPLEMENTED");
		return false;
	}

	bool CanPlayerAttach()
	{
		EXError.Error(this, "NOT IMPLEMENTED");
		return false;
	}
	
	bool IsFunctional(bool checkOptionalParts = false, set<typename> missingComponents = null)
	{
		EXError.Error(this, "NOT IMPLEMENTED");
		return false;
	}

	CarDoorState GetDoorState(int seatIdx)
	{
		EXError.Error(this, "NOT IMPLEMENTED");
		return CarDoorState.DOORS_MISSING;
	}

	CarDoorState GetDoorState(string slotName)
	{
		EXError.Error(this, "NOT IMPLEMENTED");
		return CarDoorState.DOORS_MISSING;
	}

	bool HasDoor(int seatIdx)
	{
		return GetDoorState(seatIdx) != CarDoorState.DOORS_MISSING;
	}

	bool IsDoorClosed(int seatIdx)
	{
		return GetDoorState(seatIdx) == CarDoorState.DOORS_CLOSED;
	}

	void SetLockState(ExpansionVehicleLockState lockState)
	{
		EXError.Error(this, "NOT IMPLEMENTED");
	}

	ExpansionVehicleLockState GetLockState()
	{
		EXError.Error(this, "NOT IMPLEMENTED");
		return ExpansionVehicleLockState.NOLOCK;
	}

	bool IsLocked()
	{
		return false;
	}

	void ResetKeyPairing()
	{
		EXError.Error(this, "NOT IMPLEMENTED");
	}

	int CrewSize()
	{
		EXError.Error(this, "NOT IMPLEMENTED");
		return 0;
	}

	Human CrewMember(int posIdx)
	{
		EXError.Error(this, "NOT IMPLEMENTED");
		return null;
	}

	int CrewMemberIndex(Human player)
	{
		EXError.Error(this, "NOT IMPLEMENTED");
		return -1;
	}

	int CrewPositionIndex(int componentIdx)
	{
		EXError.Error(this, "NOT IMPLEMENTED");
		return -1;
	}

	bool IsPlayerInSeat(DayZPlayerImplement player, int seatIdx = DayZPlayerConstants.VEHICLESEAT_DRIVER)
	{
	#ifdef EXPANSIONMODVEHICLE
		if (GetEntity().IsInherited(ExpansionVehicleBase))
		{
			auto exVehCmd = player.GetCommand_ExpansionVehicle();
			if (exVehCmd && exVehCmd.GetVehicleSeat() == seatIdx)
				return true;

			return false;
		}
	#endif

		auto vehCmd = player.GetCommand_Vehicle();
		if (vehCmd && vehCmd.GetVehicleSeat() == seatIdx)
			return true;

		return false;
	}

	int GetSeatAnimationType(int posIdx)
	{
		EXError.Error(this, "NOT IMPLEMENTED");
		return 0;
	}

	int GetAnimInstance()
	{
		EXError.Error(this, "NOT IMPLEMENTED");
		return 0;
	}

	string GetAnimSourceFromSelection(string selection)
	{
		EXError.Error(this, "NOT IMPLEMENTED");
		return string.Empty;
	}

	string GetDoorInvSlotNameFromSeatPos(int posIdx)
	{
		EXError.Error(this, "NOT IMPLEMENTED");
		return string.Empty;
	}

	string GetDoorSelectionNameFromSeatPos(int posIdx)
	{
		EXError.Error(this, "NOT IMPLEMENTED");
		return string.Empty;
	}

	float GetSpeedometer()
	{
		vector velocity = GetVelocity(GetEntity());
		velocity[1] = 0;
		return velocity.Length() * 3.6;
	}

	float GetSpeedometerAbsolute()
	{
		return Math.AbsFloat(GetSpeedometer());
	}

	bool CrewCanGetThrough(int posIdx)
	{
		EXError.Error(this, "NOT IMPLEMENTED");
		return false;
	}

	bool CanGetInSeat(PlayerBase player, int seatIdx)
	{
		EXError.Error(this, "NOT IMPLEMENTED");
		return false;
	}

	void OnVariablesSynchronized()
	{
	}

	void SetLastDriverUID(PlayerBase player)
	{
		m_LastDriverUID = player.GetIdentityUID();

		EXPrint(ToString() + "::SetLastDriverUID - " + m_LastDriverUID);

		if (!IsMissionHost())
			return;

		m_SynchLastDriverUID = true;

		GetEntity().SetSynchDirty();
	}

	void ResetLastDriverUIDSynch()
	{
		EXPrint(ToString() + "::ResetLastDriverUIDSynch");

		m_SynchLastDriverUID = false;

		GetEntity().SetSynchDirty();
	}

	string GetLastDriverUID()
	{
		return m_LastDriverUID;
	}

	void SetCrewKilledTimestamp(float timestamp)
	{
		m_CrewKilledTimestamp = timestamp;
	}

	float GetCrewKilledTimestamp()
	{
		return m_CrewKilledTimestamp;
	}

	void SetAllDoorsAnimationPhase(float phase)
	{
		EXError.Error(this, "NOT IMPLEMENTED");
	}

	void CloseAllDoors()
	{
	}

	set<Human> GetCrew(bool playersOnly = false, bool includeAttached = false)
	{
		EXError.Error(this, "NOT IMPLEMENTED");
		return new set<Human>;
	}

	void ForceCrewGetOut()
	{
		EXError.Error(this, "NOT IMPLEMENTED");
	}

	void CreateSafeZoneCleanup()
	{
		if (m_SZCleanup)
			return;

		EntityAI vehicle = GetEntity();

		if (ExpansionStatic.IsAnyOf(vehicle, GetExpansionSettings().GetSafeZone().ForceSZCleanup_ExcludedItems, true))
			return;

		m_SZCleanup = true;

	#ifdef EXPANSION_SAFEZONE_DEBUG
		EXPrint("[CORE][Expansion_SZCleanup] " + ToString() + " " + vehicle.GetPosition() + " marked for cleanup - lifetime " + vehicle.GetLifetime());
	#endif
	}

	void SafeZoneCleanup(float elapsedSinceLastUpdate)
	{
		//! If vehicle is not in a safezone, bail
		if (!m_IsInSafeZone)
			return;

		//! If vehicle vehicle safezone cleanup is not enabled, bail
		if (!m_SZCleanup)
			return;

	#ifdef EXPANSIONMODVEHICLE
		//! Heli needs to be landed to count as parked
		ExpansionHelicopterScript heli;
		if (Class.CastTo(heli, GetEntity()) && heli.Expansion_EngineIsSpinning() && !heli.IsLanded())
			return;
	#endif

		EntityAI vehicle = GetEntity();

		//! If vehicle isn't moving, consider it parked and increase parking time
		if (GetVelocity(vehicle).LengthSq() < 0.01)
			m_SZParkingTime += elapsedSinceLastUpdate;

		//! If parking time is zero, bail
		if (!m_SZParkingTime)
			return;

		//! If parking time exceeds max allowed vehicle lifetime in SZ, delete it
		float lifetime = GetExpansionSettings().GetSafeZone().VehicleLifetimeInSafeZone;
		if (m_SZParkingTime > lifetime)
		{
			ForceCrewGetOut();

			//! If we have last driver UID, notify player that their vehicle is being deleted
			if (m_LastDriverUID)
			{
				PlayerBase player = PlayerBase.GetPlayerByUID(m_LastDriverUID);
				if (player)
				{
					string displayName = GetDisplayName();
					string posText = ExpansionStatic.VectorToString(GetPosition(), ExpansionVectorToString.Labels);
					string lifetimeText = ExpansionStatic.GetTimeString(lifetime, true);
					CF_Localiser localiser = new CF_Localiser("STR_EXPANSION_SAFEZONE_VEHICLE_CLEANUP_1", displayName, posText, lifetimeText);
					ExpansionNotification("STR_EXPANSION_SAFEZONE_TITLE", localiser).Error(player.GetIdentity());
				}
			}

			g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DeleteVehicleWhenCrewLeft, 3000, false, lifetime);
		}
	}

	void DeleteVehicleWhenCrewLeft(float lifetime)
	{
		if (!GetCrew(false, false).Count())
		{
			//! If we have last driver UID, notify player that their vehicle is being deleted
			if (m_LastDriverUID)
			{
				PlayerBase player = PlayerBase.GetPlayerByUID(m_LastDriverUID);
				if (player)
				{
					string displayName = GetDisplayName();
					string posText = ExpansionStatic.VectorToString(GetPosition(), ExpansionVectorToString.Labels);
					string lifetimeText = ExpansionStatic.GetTimeString(lifetime, true);
					CF_Localiser localiser = new CF_Localiser("STR_EXPANSION_SAFEZONE_VEHICLE_CLEANUP_2", displayName, posText, lifetimeText);
					ExpansionNotification("STR_EXPANSION_SAFEZONE_TITLE", localiser).Error(player.GetIdentity());
				}
			}

			GetEntity().Delete();
		}
	}

	void ResetSZParkingTime()
	{
		m_SZParkingTime = 0;
	}

	void OnEnterZone(ExpansionZoneType type)
	{
		if (type != ExpansionZoneType.SAFE) return;

		EXTrace.Print(EXTrace.VEHICLES, GetEntity(), "::OnEnterZone " + GetPosition());

		m_IsInSafeZone = true;

		if (GetExpansionSettings().GetSafeZone().EnableForceSZCleanupVehicles)
			CreateSafeZoneCleanup();
	}

	void OnExitZone(ExpansionZoneType type)
	{
		if (type != ExpansionZoneType.SAFE) return;

		EXTrace.Print(EXTrace.VEHICLES, GetEntity(), "::OnExitZone " + GetPosition());

		m_IsInSafeZone = false;
		m_SZCleanup = false;
		ResetSZParkingTime();
	}

	void OnCECreate()
	{
		m_IsCECreated = true;
	}

	void OnCEUpdate()
	{
	}

	bool IsStoreLoaded()
	{
		EXError.Error(this, "NOT IMPLEMENTED");
		return false;
	}

	bool IsStoreSaved()
	{
		EXError.Error(this, "NOT IMPLEMENTED");
		return false;
	}

	void OnDestroyed(Object killer)
	{
		if (g_Game.IsServer() && GetExpansionSettings().GetLog().VehicleDestroyed && !m_DestructionLogged)
		{
    		GetExpansionSettings().GetLog().PrintLog("[VehicleDestroyed] {1:type} (id={1:persistent_id} pos={1:position})", GetEntity());

			m_DestructionLogged = true;
		}
	}

	void OnDelete()
	{
		if (g_Game.IsServer() && GetExpansionSettings().GetLog().VehicleDeleted)
		{
			GetExpansionSettings().GetLog().PrintLog("[VehicleDeleted] {1:type} (id={1:persistent_id} pos={1:position})", GetEntity());
		}
	}

	void OnDoorOpened(string selection)
	{
	}

	void OnDoorClosed(string selection)
	{
	}

	void OnLocked(ExpansionVehicleLockState previousLockState)
	{
	}

	void OnUnlocked(ExpansionVehicleLockState previousLockState)
	{
	}

	protected void KeyMessage(string message)
	{
#ifdef EXPANSION_CARKEY_LOGGING
	#ifndef SERVER
		PlayerBase player;
		if (Class.CastTo(player, g_Game.GetPlayer()))
			player.Message(message);
	#endif

		Print(message);
#endif
	}
}

class ExpansionVehicleT<Class T>: ExpansionVehicle
{
	protected T m_Vehicle;

#ifdef EXPANSIONMODVEHICLE
	//! Very annoying that vehicle mod stuff has to be here, but you cannot mod classes with type parameters...
	protected EffectSound m_SoundLock;
#endif

	void ExpansionVehicleT(T vehicle)
	{
		m_Vehicle = vehicle;

		m_IsBike = vehicle.Expansion_IsBike();
		m_IsBoat = vehicle.Expansion_IsBoat();
		m_IsCar = vehicle.Expansion_IsCar();
		m_IsHelicopter = vehicle.Expansion_IsHelicopter();
		m_IsPlane = vehicle.Expansion_IsPlane();
		m_IsDuck = vehicle.Expansion_IsDuck();

		m_FuelConsumption = g_Game.ConfigGetFloat("CfgVehicles " + vehicle.GetType() + " fuelConsumption");
	}

	override void Init()
	{
		super.Init();

		m_Vehicle.RegisterNetSyncVariableInt("m_ExpansionVehicle.m_CargoCount");
		m_Vehicle.RegisterNetSyncVariableBool("m_ExpansionVehicle.m_SynchLastDriverUID");

	#ifdef EXPANSIONMODMARKET
		m_Vehicle.RegisterNetSyncVariableInt("m_ExpansionVehicle.m_ParkingFine");
	#endif

	#ifdef EXPANSIONMODVEHICLE
		m_NetsyncData = new ExpansionNetsyncData(m_Vehicle);

		m_Vehicle.m_Expansion_RPCManager.RegisterClient("RPC_PlayLockSound", (ExpansionVehicle) this);
	#endif
	}

	//! -----------------------------------------------------------------------

	override EntityAI GetEntity()
	{
		return m_Vehicle;
	}

	override Transport GetTransport()
	{
		return Transport.Cast(GetEntity());  //! @note will obviously return null if entity doesn't inherit from transport
	}

	override ExpansionGlobalID GetGlobalID()
	{
		return m_Vehicle.m_Expansion_GlobalID;
	}
	
#ifdef FEATURE_NETWORK_RECONCILIATION
	override NetworkMoveStrategy GetNetworkMoveStrategy()
	{
		return m_Vehicle.GetNetworkMoveStrategy();
	}
#endif

	override void SetSkin(int skinIndex)
	{
		m_Vehicle.ExpansionSetSkin(skinIndex);
	}

	override bool HasSkin(int skinIndex)
	{
		return m_Vehicle.ExpansionHasSkin(skinIndex);
	}

	override int GetCurrentSkinIndex()
	{
		return m_Vehicle.ExpansionGetCurrentSkinIndex();
	}

	override string GetCurrentSkinName()
	{
		return m_Vehicle.ExpansionGetCurrentSkinName();
	}

	override void FillFluid(int fluid, float amount)
	{
		m_Vehicle.Fill(fluid, amount);
	}

	override void FillFluids(float relativeAmount = 1.0)
	{
		typename fluidEnum = CarFluid;
		if (GetBoat())
			fluidEnum = BoatFluid;
		int fluid;
		for (int i = 0; i < EnumTools.GetEnumSize(fluidEnum); i++)
		{
			fluid = EnumTools.GetEnumValue(fluidEnum, i);
			m_Vehicle.Fill(fluid, m_Vehicle.GetFluidCapacity(fluid) * relativeAmount);
		}
	}

	override void CloneFluidsTo(ExpansionVehicle dst)
	{
		typename fluidEnum = CarFluid;
		if (GetBoat())
			fluidEnum = BoatFluid;
		int fluid;
		for (int i = 0; i < EnumTools.GetEnumSize(fluidEnum); i++)
		{
			fluid = EnumTools.GetEnumValue(fluidEnum, i);
			dst.FillFluid(fluid, m_Vehicle.GetFluidFraction(fluid) * dst.GetFluidCapacity(fluid));
		}
	}

	override void LeakFluids()
	{
		typename fluidEnum = CarFluid;
		if (GetBoat())
			fluidEnum = BoatFluid;
		int fluid;
		for (int i = 0; i < EnumTools.GetEnumSize(fluidEnum); i++)
		{
			fluid = EnumTools.GetEnumValue(fluidEnum, i);
			m_Vehicle.LeakAll(fluid);
		}
	}

	override float GetFluidCapacity(int fluid)
	{
		return m_Vehicle.GetFluidCapacity(fluid);
	}

	override float GetFluidFraction(int fluid)
	{
		return m_Vehicle.GetFluidFraction(fluid);
	}

	override void WriteFluidsTo(ParamsWriteContext ctx)
	{
		typename fluidEnum = CarFluid;
		if (GetBoat())
			fluidEnum = BoatFluid;
		int fluidCount = EnumTools.GetEnumSize(fluidEnum);
		ctx.Write(fluidCount);
		for (int i = 0; i < fluidCount; i++)
		{
			ctx.Write(m_Vehicle.GetFluidFraction(EnumTools.GetEnumValue(fluidEnum, i)));
		}
	}

	override bool ReadFluidsFrom(ParamsReadContext ctx, int entityStorageVersion)
	{
		int fluid;
		float fluidFraction;
		typename fluidEnum = CarFluid;
		if (GetBoat())
			fluidEnum = BoatFluid;
		int fluidEnumSize = EnumTools.GetEnumSize(fluidEnum);
		int fluidCount;

		if (entityStorageVersion >= 6)
			ctx.Read(fluidCount);
		else
			fluidCount = fluidEnumSize;

	#ifdef DIAG_DEVELOPER
		EXTrace.Print(EXTrace.GENERAL_ITEMS, null, "Fluid count: " + fluidCount);
	#endif

		for (int i = 0; i < fluidCount; i++)
		{
			if (i < fluidEnumSize)
				fluid = EnumTools.GetEnumValue(fluidEnum, i);

			if (!ctx.Read(fluidFraction))
			{
				EXError.Error(null, GetType() + ": Couldn't read fluid fraction for " + typename.EnumToString(fluidEnum, fluid));
			}

			if (i < fluidEnumSize)
				m_Vehicle.Fill(fluid, fluidFraction * m_Vehicle.GetFluidCapacity(fluid));
		}
		return true;
	}

	override void Synchronize()
	{
		m_Vehicle.Synchronize();
	}

	override bool IsStoreLoaded()
	{
		return m_Vehicle.Expansion_IsStoreLoaded();
	}

	override bool IsStoreSaved()
	{
		return m_Vehicle.Expansion_IsStoreSaved();
	}

	override bool IsFunctional(bool checkOptionalParts = false, set<typename> missingComponents = null)
	{
		return m_Vehicle.Expansion_IsVehicleFunctional(checkOptionalParts, missingComponents);
	}

	override bool EngineIsOn()
	{
	#ifdef EXPANSIONMODVEHICLE
		return m_Vehicle.Expansion_EngineIsOn();
	#else
		return m_Vehicle.EngineIsOn();
	#endif
	}

	override void EngineStart()
	{
	#ifdef EXPANSIONMODVEHICLE
		m_Vehicle.Expansion_EngineStart();
	#else
		m_Vehicle.EngineStart();
	#endif
	}

	override void EngineStop()
	{
	#ifdef EXPANSIONMODVEHICLE
		m_Vehicle.Expansion_EngineStop();
	#else
		m_Vehicle.EngineStop();
	#endif
	}

	override float GetThrottle()
	{
		return m_Vehicle.Expansion_GetThrottle();
	}

	override int CrewSize()
	{
		return m_Vehicle.CrewSize();
	}

	override Human CrewMember(int posIdx)
	{
		return m_Vehicle.CrewMember(posIdx);
	}

	override int CrewMemberIndex(Human player)
	{
		return m_Vehicle.CrewMemberIndex(player);
	}

	override int CrewPositionIndex(int componentIdx)
	{
		return m_Vehicle.CrewPositionIndex(componentIdx);
	}

	override int GetSeatAnimationType(int posIdx)
	{
		return m_Vehicle.GetSeatAnimationType(posIdx);
	}

	override int GetAnimInstance()
	{
		return m_Vehicle.GetAnimInstance();
	}

	override string GetAnimSourceFromSelection(string selection)
	{
		return m_Vehicle.Expansion_GetAnimSourceFromSelection(selection);
	}

	override string GetDoorInvSlotNameFromSeatPos(int posIdx)
	{
		return m_Vehicle.Expansion_GetDoorInvSlotNameFromSeatPos(posIdx);
	}

	override string GetDoorSelectionNameFromSeatPos(int posIdx)
	{
		return m_Vehicle.Expansion_GetDoorSelectionNameFromSeatPos(posIdx);
	}

	override void SetAllDoorsAnimationPhase(float phase)
	{
		CarDoor carDoor;

		for (int i = 0; i < m_Vehicle.GetInventory().AttachmentCount(); i++)
		{
			if (Class.CastTo(carDoor, m_Vehicle.GetInventory().GetAttachmentFromIndex(i)))
			{
				TStringArray selectionNames = {};

				carDoor.GetActionComponentNameList(0, selectionNames);

				if (!selectionNames.Count())
					continue;

				TStringArray animSources = {};
				string animSource;

				foreach (string selectionName: selectionNames)
				{
					animSource = m_Vehicle.Expansion_GetAnimSourceFromSelection(selectionName);
					if (animSource)
						animSources.Insert(animSource);
				}

				 //! Turns out correct selection is always last one when doing it this way
				int lastIdx = animSources.Count() - 1;
				if (lastIdx > -1)
				{
					animSource = animSources[lastIdx];
					if (m_Vehicle.GetAnimationPhase(animSource) != phase)
						m_Vehicle.SetAnimationPhase(animSource, phase);
				}
			}
		}
	}

	override bool CrewCanGetThrough(int posIdx)
	{
		return m_Vehicle.CrewCanGetThrough(posIdx);
	}

	override bool CanGetInSeat(PlayerBase player, int seatIdx)
	{
		return m_Vehicle.Expansion_CanGetInSeat(player, seatIdx);
	}

	override void CloseAllDoors()
	{
		m_Vehicle.Expansion_CloseAllDoors();
	}

	override void OnVariablesSynchronized()
	{
		if (m_SynchLastDriverUID != m_LastDriverUIDSynched)
		{
			m_LastDriverUIDSynched = m_SynchLastDriverUID;

			if (!m_SynchLastDriverUID)
				return;

			//! Reset m_LastDriverUID client-side if vehicle has driver and it is not the player
			Human driver = m_Vehicle.CrewMember(DayZPlayerConstants.VEHICLESEAT_DRIVER);
			Man player = g_Game.GetPlayer();
			if (driver && player && driver != player)
			{
				m_LastDriverUID = "";
			}
		}
	}

	override set<Human> GetCrew(bool playersOnly = false, bool includeAttached = false)
	{
		set<Human> players = new set<Human>;
		Human crew;

		//! Seated players
		for (int i = 0; i < m_Vehicle.CrewSize(); i++)
		{
			crew = m_Vehicle.CrewMember(i);
			if (!crew)
				continue;

			if (!playersOnly || crew.GetIdentity())
				players.Insert(crew);
		}

		if (!includeAttached)
			return players;

		//! Attached players
		//! TODO: this is fairly inefficient
		auto node = PlayerBase.s_Expansion_AllPlayers.m_Head;
		while (node)
		{
			crew = node.m_Value;

			node = node.m_Next;

			if (crew.PhysicsGetLinkedEntity() != m_Vehicle)
				continue;

			if (!playersOnly || crew.GetIdentity())
				players.Insert(crew);
		}

		return players;
	}

	override void ForceCrewGetOut()
	{
		//! TODO/FIXME ForceCrewGetOut doesn't work under DayZ 1.28 or newer
		return;

		auto crew = GetCrew(false, false);
		foreach (auto member: crew)
		{
			//! Open the door so player can get out
			int crewIdx = CrewMemberIndex(member);
			string selection = GetDoorSelectionNameFromSeatPos(crewIdx);
			if (selection)
			{
				string animSource = GetAnimSourceFromSelection(selection);
				if (animSource)
					m_Vehicle.SetAnimationPhase(animSource, 1.0);
			}

			//! Push them out
			HumanCommandVehicle vehCommand = member.GetCommand_Vehicle();
			if (vehCommand)
				vehCommand.GetOutVehicle();
		}
	}

	override bool CanObjectAttach(Object obj)
	{
		return m_Vehicle.Expansion_CanObjectAttach(obj);
	}

	override bool CanPlayerAttach()
	{
		return m_Vehicle.Expansion_CanPlayerAttach();
	}

	override CarDoorState GetDoorState(int seatIdx)
	{
		string slotName = GetDoorInvSlotNameFromSeatPos(seatIdx);
		return GetDoorState(slotName);
	}

	override CarDoorState GetDoorState(string slotName)
	{
		return m_Vehicle.Expansion_GetCarDoorsState(slotName);
	}

#ifdef EXPANSIONMODVEHICLE
	override void SetActions()
	{
		m_Vehicle.Expansion_AddAction(ExpansionActionPairKey);
		m_Vehicle.Expansion_AddAction(ExpansionActionAdminUnpairKey);

		m_Vehicle.Expansion_AddAction(ExpansionActionLockVehicle);
	}

	override bool AllDoorsClosed(bool includeHoodAndTrunk = true)
	{
		foreach (ExpansionDoor door: m_Doors)
		{
			if (!includeHoodAndTrunk && (door.m_IsHood || door.m_IsTrunk))
				continue;

			if (m_Vehicle.Expansion_GetCarDoorsState(door.m_InventorySlot) != CarDoorState.DOORS_CLOSED)
				return false;
		}

		return true;
	}

	override void SetHasPilot(bool state)
	{
		EntityAI entity = GetEntity();
		ExpansionHelicopterScript hs;
		ExpansionVehicleHelicopterBase evhb;
		if (Class.CastTo(hs, entity))
			hs.SetHasPilot(state);
		else if (Class.CastTo(evhb, entity))
			evhb.SetHasPilot(state);
	}

	override bool LeavingSeatDoesAttachment(int posIdx)
	{
		return m_Vehicle.LeavingSeatDoesAttachment(posIdx);
	}

	override int NumberTowConnections()
	{
		return m_Vehicle.Expansion_NumberTowConnections();
	}

	override bool GetOverlappingTowConnection(vector towPosition, float towRadius, out int index)
	{
		return m_Vehicle.Expansion_GetOverlappingTowConnection(towPosition, towRadius, index);
	}

	override vector GetTowDirection()
	{
		 return m_Vehicle.Expansion_GetTowDirection();
	}

	override float GetTowLength()
	{
		return m_Vehicle.Expansion_GetTowLength();
	}

	override vector GetTowPosition()
	{
		return m_Vehicle.Expansion_GetTowPosition();
	}

	override vector GetTowPositionWS()
	{
		return m_Vehicle.ModelToWorld(GetTowPosition());
	}

	override EntityAI GetTowedEntity()
	{
		return m_Vehicle.Expansion_GetTowedEntity();
	}

	override bool IsTowing()
	{
		return m_Vehicle.Expansion_IsTowing();
	}

	override void CreateTow(Object tow, int index)
	{
		m_Vehicle.Expansion_CreateTow(tow, index);
	}

	override void DestroyTow()
	{
		m_Vehicle.Expansion_DestroyTow();
	}

	override bool IsScriptedLightsOn()
	{
		return m_Vehicle.IsScriptedLightsOn();
	}

	override bool IsVitalAircraftBattery()
	{
		return m_Vehicle.IsVitalAircraftBattery();
	}

	override bool IsVitalCarBattery()
	{
		return m_Vehicle.IsVitalCarBattery();
	}

	override bool IsVitalHelicopterBattery()
	{
		return m_Vehicle.IsVitalHelicopterBattery();
	}

	override bool IsVitalTruckBattery()
	{
		return m_Vehicle.IsVitalTruckBattery();
	}

	override bool IsExploded()
	{
		return m_Vehicle.Expansion_IsExploded();
	}

	override int GetEngineStartAnimation()
	{
		return m_Vehicle.Expansion_EngineStartAnimation();
	}

	override int GetEngineStopAnimation()
	{
		return m_Vehicle.Expansion_EngineStopAnimation();
	}

	override int EngineGetCount()
	{
		return m_Vehicle.Expansion_EngineGetCount();
	}

	override int EngineGetCurrent()
	{
		return m_Vehicle.Expansion_EngineGetCurrent();
	}

	override void EngineSetNext()
	{
		m_Vehicle.Expansion_EngineSetNext();
	}

	override string EngineGetName()
	{
		return m_Vehicle.Expansion_EngineGetName();
	}

	override bool OnBeforeEngineStart(int index = 0)
	{
		if (g_Game.IsClient())
			return true;

		float engineHealth = GetEntity().GetHealth01("Engine", "");
		//! @note chance when engine is damaged (health level 0.5)
		float chance = GetExpansionSettings().GetVehicle().DamagedEngineStartupChancePercent / 100.0;
		//! @note calculated chance follows a power curve (linear if chance == 0.5)
		float chanceMin = 0.0025;
		bool clamp = chance == 0;

		if (chance < chanceMin)
			chance = chanceMin;

		chance = Math.Pow(engineHealth, -ExpansionMath.Log2(chance));

		if (clamp)
			chance = ExpansionMath.LinearConversion(chanceMin, 1.0, chance, 0.0, 1.0);

		if (chance >= Math.RandomFloatInclusive(0.0, 1.0))
			return true;

		return false;
	}

	override void OnEngineStart(int index = 0)
	{
		if (g_Game.IsServer() && GetExpansionSettings().GetLog().VehicleEngine)
			GetExpansionSettings().GetLog().PrintLog("[VehicleEngine] Player \"{1:name}\" (id={1:id}) started vehicle {2:name} (id={2:persistent_id} pos={2:position})", CrewMember(DayZPlayerConstants.VEHICLESEAT_DRIVER), GetEntity());
	}

	override void OnEngineStop(int index = 0)
	{
		if (g_Game.IsServer() && GetExpansionSettings().GetLog().VehicleEngine)
			GetExpansionSettings().GetLog().PrintLog("[VehicleEngine] Player \"{1:name}\" (id={1:id}) stopped vehicle {2:name} (id={2:persistent_id} pos={2:position})", CrewMember(DayZPlayerConstants.VEHICLESEAT_DRIVER), GetEntity());
	}

	override bool EngineIsOn(int index)
	{
		return m_Vehicle.Expansion_EngineIsOn(index);
	}

	override float GetThrottle(int index)
	{
		return m_Vehicle.Expansion_GetThrottle(index);
	}

	override int EngineGetRunningAboveOrAtIdleRPMCount()
	{
		int count;

		for (int i = 0; i < m_Vehicle.Expansion_EngineGetCount(); ++i)
		{
			if (m_Vehicle.Expansion_EngineGetRPM(i) >= m_Vehicle.Expansion_EngineGetRPMIdle(i) || m_Vehicle.Expansion_GetThrottle(i) > 0)
				count++;
		}

		return count;
	}

	override void ConsumeFuel(float fuelConsumption)
	{
		float fuelConsumptionNominal = fuelConsumption;

		float fuelConsumptionPct = GetExpansionSettings().GetVehicle().FuelConsumptionPercent;

		if (fuelConsumptionPct != 100)
			fuelConsumption *= fuelConsumptionPct * 0.01;

		//! @note fuel consumption for engine 0 is handled by the game internally, so we need to subtract nominal fuel consumption.
		//! If resulting consumption is negative, FuelConsumptionPercent was set to below 100% (= add some fuel back after consumption)
		if (m_Vehicle.Expansion_EngineGetRPM(0) >= m_Vehicle.Expansion_EngineGetRPMIdle(0) || m_Vehicle.Expansion_GetThrottle(0) > 0)
			fuelConsumption -= fuelConsumptionNominal / EngineGetRunningAboveOrAtIdleRPMCount();

	#ifdef DIAG_DEVELOPER
		if (EXTrace.VEHICLES && fuelConsumption != m_FuelConsumption_Prev)
		{
			EXTrace.Print(true, this, m_Vehicle.GetType() + " fuel consumption (liters/s) " + fuelConsumption + " (" + fuelConsumptionPct + "%%)");
			m_FuelConsumption_Prev = fuelConsumption;
		}
	#endif

		if (fuelConsumption != 0.0)
		{
			int fluid;
			if (m_Vehicle.IsInherited(Boat))
				fluid = BoatFluid.FUEL;
			else
				fluid = CarFluid.FUEL;

			if (fuelConsumption < 0.0)
			{
				//! Will only be negative if engine 0 is on with FuelConsumptionPercent < 100

				//! Need to make sure to only add fuel if not already depleted else we will never run out of fuel
				if (m_Vehicle.GetFluidFraction(fluid) > 0)
					m_Vehicle.Fill(fluid, -fuelConsumption);
			}
			else
			{
				m_Vehicle.Leak(fluid, fuelConsumption);
			}
		}
	}

	override void ConsumeFuelOverTime(float dt)
	{
		ConsumeFuel(m_FuelConsumption * dt / 3600.0);
	}

	override bool HasGear()
	{
		return m_Vehicle.Expansion_HasGear();
	}

	override void SwitchGear()
	{
		m_Vehicle.Expansion_SwitchGear();
	}

	override ExpansionPhysicsState GetPhysicsState()
	{
		return m_Vehicle.Expansion_GetPhysicsState();
	}

	override void OnDoorOpened(string selection)
	{
		super.OnDoorOpened(selection);

		m_Vehicle.Expansion_OnDoorOpened(selection);
	}

	override void OnDoorClosed(string selection)
	{
		super.OnDoorClosed(selection);

		m_Vehicle.Expansion_OnDoorClosed(selection);
	}

	override void OnLocked(ExpansionVehicleLockState previousLockState)
	{
		KeyMessage("OnLocked");

		if (g_Game.IsServer())
		{
			auto rpc = m_Vehicle.m_Expansion_RPCManager.CreateRPC("RPC_PlayLockSound");
			PlayerBase.Expansion_SendNear(rpc, GetEntity().GetPosition(), 20.0, GetEntity(), true);
		}
	}

	override void OnUnlocked(ExpansionVehicleLockState previousLockState)
	{
		KeyMessage("OnUnlocked");

		if (g_Game.IsServer())
		{
			switch (previousLockState)
			{
				case ExpansionVehicleLockState.LOCKED:
				case ExpansionVehicleLockState.FORCEDLOCKED:
					break;
				default:
					return;
			}

			auto rpc = m_Vehicle.m_Expansion_RPCManager.CreateRPC("RPC_PlayLockSound");
			PlayerBase.Expansion_SendNear(rpc, GetEntity().GetPosition(), 20.0, GetEntity(), true);
		}
	}

	void RPC_PlayLockSound(PlayerIdentity sender, ParamsReadContext ctx)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.VEHICLES, this);
#endif
		
		if (m_SoundLock)
			delete m_SoundLock;

		m_SoundLock = SEffectManager.PlaySound("Expansion_Car_Lock_SoundSet", m_Vehicle.GetPosition());
		m_SoundLock.SetSoundAutodestroy(true);
	}
#endif
}
