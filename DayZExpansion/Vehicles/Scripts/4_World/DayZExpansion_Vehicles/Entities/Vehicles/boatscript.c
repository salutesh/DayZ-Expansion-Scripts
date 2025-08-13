#ifndef DAYZ_1_25
modded class BoatScript
{
	protected float m_Expansion_FuelCheckTime;

#ifdef EXPANSION_MODSTORAGE
	override void CF_OnStoreSave(CF_ModStorageMap storage)
	{
		super.CF_OnStoreSave(storage);

		auto ctx = storage[DZ_Expansion_Vehicles];
		if (!ctx) return;

		m_ExpansionVehicle.OnStoreSave(ctx);
	}

	override bool CF_OnStoreLoad(CF_ModStorageMap storage)
	{
		if (!super.CF_OnStoreLoad(storage))
			return false;

		auto ctx = storage[DZ_Expansion_Vehicles];
		if (!ctx) return true;

		if (ctx.GetVersion() < 53)
			return true;

		if (!m_ExpansionVehicle.OnStoreLoad(ctx))
			return false;

		return true;
	}
#endif

	override void EEOnAfterLoad()
	{
		super.EEOnAfterLoad();

		m_ExpansionVehicle.OnHierarchyAfterLoad();
	}

	override void EEOnCECreate()
	{
		super.EEOnCECreate();

		m_ExpansionVehicle.OnCECreate();
	}

	override void OnCEUpdate()
	{
		super.OnCEUpdate();

		m_ExpansionVehicle.AutoCover();
	}

	override void EEItemAttached(EntityAI item, string slot_name)
	{
		super.EEItemAttached(item, slot_name);

		m_ExpansionVehicle.OnItemAttached(item, slot_name);
	}

	override void EEItemDetached(EntityAI item, string slot_name)
	{
		super.EEItemDetached(item, slot_name);

		m_ExpansionVehicle.OnItemDetached(item, slot_name);
	}

	override void EOnPostSimulate(IEntity other, float timeSlice)
	{
		m_ExpansionVehicle.OnPostSimulate(timeSlice);

		if (GetGame().IsServer())
		{
			m_Expansion_FuelCheckTime += timeSlice;

			if (m_Expansion_FuelCheckTime > GameConstants.CARS_FLUIDS_TICK)
			{
				//! @note this is for additional fuel consumption (FuelConsumptionPercent != 100)
				if (EngineIsOn())
					m_ExpansionVehicle.ConsumeFuelOverTime(m_Expansion_FuelCheckTime);
	
				m_Expansion_FuelCheckTime = 0;
			}
		}
	}

	override bool CanReceiveAttachment(EntityAI attachment, int slotId)
	{
		if (!super.CanReceiveAttachment(attachment, slotId))
			return false;

		if (!m_ExpansionVehicle.CanReceiveAttachment(attachment, slotId))
			return false;

		return true;
	}

	override bool CanReleaseAttachment(EntityAI attachment)
	{
		if (!super.CanReleaseAttachment(attachment))
			return false;

		if (!m_ExpansionVehicle.CanReleaseAttachment(attachment))
			return false;

		return true;
	}

	override bool IsInventoryVisible()
	{
		if (!super.IsInventoryVisible())
			return false;

		if (!m_ExpansionVehicle.IsInventoryVisible())
			return false;

		return true;
	}

	bool IsScriptedLightsOn()
	{
		return false;
	}

	bool IsVitalAircraftBattery()
	{
		return false;
	}

	bool IsVitalCarBattery()
	{
		return false;
	}

	bool IsVitalHelicopterBattery()
	{
		return false;
	}

	bool IsVitalTruckBattery()
	{
		return false;
	}

	bool Expansion_IsExploded()
	{
		return false;
	}
	
	override bool NameOverride(out string output)
	{
		//! @note normally m_ExpansionVehicle cannot be NULL since it's set on BoatScript instantiation,
		//! but EntityAI initializes damagezones before we can set it and uses GetDisplayName if there are no damagezones configured
		if (m_ExpansionVehicle)
			return m_ExpansionVehicle.NameOverride(output);

		return false;
	}

	bool LeavingSeatDoesAttachment(int posIdx)
	{
		return true;
	}

	void Expansion_EngineSetNext()
	{
	}

	int Expansion_EngineGetCount()
	{
		return 1;
	}

	int Expansion_EngineGetCurrent()
	{
		return 0;
	}

	string Expansion_EngineGetName()
	{
		return "Boat";
	}

	float Expansion_EngineGetRPM(int index)
	{
		return EngineGetRPM();
	}

	float Expansion_EngineGetRPMIdle(int index)
	{
		return EngineGetRPMIdle();
	}

	bool Expansion_EngineIsOn(int index)
	{
		return EngineIsOn();
	}

	bool Expansion_HasGear()
	{
		return false;
	}

	void Expansion_SwitchGear()
	{
	}

	int Expansion_NumberTowConnections()
	{
		return 0;
	}

	bool Expansion_GetOverlappingTowConnection(vector towPosition, float towRadius, out int index)
	{
		return false;
	}

	vector Expansion_GetTowDirection()
	{
		return vector.Zero;
	}

	float Expansion_GetTowLength()
	{
		return 0.0;
	}

	vector Expansion_GetTowPosition()
	{
		return vector.Zero;
	}

	EntityAI Expansion_GetTowedEntity()
	{
		return null;
	}

	bool Expansion_IsTowing()
	{
		return false;
	}

	void Expansion_CreateTow(Object tow, int index)
	{
	}

	void Expansion_DestroyTow()
	{
	}

	ExpansionPhysicsState Expansion_GetPhysicsState()
	{
		return null;
	}

	override bool OnBeforeEngineStart()
	{
		if (!super.OnBeforeEngineStart())
			return false;

		if (!m_ExpansionVehicle.OnBeforeEngineStart(0))
			return false;

		return true;
	}

	override void OnEngineStart()
	{
		super.OnEngineStart();

		m_ExpansionVehicle.OnEngineStart(0);
	}

	override void OnEngineStop()
	{
		super.OnEngineStop();

		m_ExpansionVehicle.OnEngineStop(0);
	}

	void Expansion_EngineStart()
	{
		EngineStart();
	}

	int Expansion_EngineStartAnimation()
	{
		return DayZPlayerConstants.CMD_ACTIONMOD_STARTENGINE;
	}

	int Expansion_EngineStopAnimation()
	{
		return DayZPlayerConstants.CMD_ACTIONMOD_STOPENGINE;
	}

	float Expansion_GetThrottle(int index)
	{
		return GetThrottle();
	}

	void Expansion_OnDoorOpened(string selection)
	{
	}

	void Expansion_OnDoorClosed(string selection)
	{
	}
};
#endif