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
	#ifndef DAYZ_1_29
		//! 1.30+
		MotorbikeScript ms;
	#endif
		DayZPlayerImplement player;

		if (Class.CastTo(cs, entity))
			return cs.GetExpansionVehicle();
		else if (Class.CastTo(bs, entity))
			return bs.GetExpansionVehicle();
		//else if (Class.CastTo(hs, entity))
			//return new ExpansionVehicleT<HelicopterScript>(hs);
	#ifndef DAYZ_1_29
		//! 1.30+
		else if (Class.CastTo(ms, entity))
			return ms.GetExpansionVehicle();
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

	#ifndef DAYZ_1_29
		//! 1.30+
		if (g_Game.IsKindOf(type, "MotorbikeScript"))
			return ExpansionVehicleType.LAND;
	#endif

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

#ifndef DAYZ_1_29
	//! 1.30
	MotorbikeScript GetMotorbike()
	{
		return MotorbikeScript.Cast(GetEntity());
	}
#endif

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

//! Crashes with random compile error popup and segfaults
/*
	bool NeedElectricitySourceDevice()
	{
		EXError.Error(this, "NOT IMPLEMENTED");
		return false;
	}
	
	EntityAI GetElectricitySourceDevice()
	{
		EXError.Error(this, "NOT IMPLEMENTED");
		return null;
	}
	
	string GetElectricitySourceDeviceType()
	{
		EXError.Error(this, "NOT IMPLEMENTED");
		return "";
	}
*/

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

	bool EngineIsSpinning()
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
		if (g_Game.IsServer())
		{
			auto settings = GetExpansionSettings().GetLog();
			if (settings.VehicleDeleted)
			{
				//! Only log vehicle deleted message if not deleted as part of entity storage (e.g. cover/garage)
				EntityAI entity = GetEntity();
				if (ExpansionEntityStorageModule.GetSavedEntityToBeDeleted() != entity)
					settings.PrintLog("[VehicleDeleted] {1:type} (id={1:persistent_id} pos={1:position} lifetime={1:lifetime})", entity);
			}
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
