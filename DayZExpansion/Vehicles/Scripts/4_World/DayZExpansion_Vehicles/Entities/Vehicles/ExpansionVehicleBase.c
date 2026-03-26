/**
 * ExpansionVehicleBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionVehicleBase: ExpansionVehicleBaseBase
{
	static ref set<ExpansionVehicleBase> m_allVehicles = new set<ExpansionVehicleBase>;

	ref array<ExpansionVehicleGearbox> m_Gearboxes = new array<ExpansionVehicleGearbox>();

	ref ExpansionController m_Controller = new ExpansionController();
	ref ExpansionPhysicsState m_State = new ExpansionPhysicsStateT<ExpansionVehicleBase>(this);

	ExpansionVehicleDifferentialType m_DifferentialType;

	ref ExpansionGlobalID m_Expansion_GlobalID = new ExpansionGlobalID();

	ref ExpansionRPCManager m_Expansion_RPCManager;

	ref ExpansionVehicle m_ExpansionVehicle = new ExpansionVehicleT<ExpansionVehicleBase>(this);

	static set<ExpansionVehicleBase> GetAll()
	{
		return m_allVehicles;
	}

	ExpansionVehicle GetExpansionVehicle()
	{
		return m_ExpansionVehicle;
	}

	void DBGAddShape(Shape shape)
	{
	}

	vector DBGFixDebugPosition(inout vector position)
	{
		return position;
	}

	void HandleController(DayZPlayerImplement driver, float dt)
	{
	}

	void Expansion_OnHandleController(DayZPlayerImplement driver, float dt)
	{
	}

	ExpansionVehicleAxle GetAxle(int axle)
	{
		return null;
	}

	float GetAxleCount()
	{
		return 0;
	}

	float GetNumWheels()
	{
		return 0;
	}

	void AddModule(ExpansionVehicleModule module)
	{
	}

	int GetCrewIndex(string selection)
	{
		return -1;
	}

	void Expansion_AddAction(typename actionName)
	{
	}

	float GetFluidCapacity(CarFluid fluid)
	{
		return 0;
	}

	float GetFluidFraction(CarFluid fluid)
	{
		return 0;
	}

	void Leak(CarFluid fluid, float amount)
	{
	}

	void LeakAll(CarFluid fluid)
	{
	}

	void Fill(CarFluid fluid, float amount)
	{
	}

	float GetEnviroHeatComfortOverride()
	{
		return 0;
	}

	string Expansion_EngineGetName()
	{
		return "";
	}

	int Expansion_EngineStartAnimation()
	{
		return 0;
	}

	int Expansion_EngineStopAnimation()
	{
		return 0;
	}

	int Expansion_EngineGetCurrent()
	{
		return -1;
	}

	int Expansion_EngineGetCount()
	{
		return 0;
	}

	void Expansion_EngineSetNext()
	{
	}

	void Expansion_EngineStart()
	{
	}

	void Expansion_EngineStop()
	{
	}

	float Expansion_EngineGetRPM(int index)
	{
		return 0;
	}

	float Expansion_EngineGetRPMIdle(int index)
	{
		return 0;
	}

	bool Expansion_EngineIsOn(int index)
	{
		return false;
	}

	bool Expansion_EngineIsOn()
	{
		return false;
	}

	bool IsLocked()
	{
		EXError.Error(this, "DEPRECATED, use GetExpansionVehicle().IsLocked");
		return m_ExpansionVehicle.IsLocked();
	}

	void Expansion_OnDoorOpened(string selection)
	{
	}

	void Expansion_OnDoorClosed(string selection)
	{
	}

	void Expansion_OnGotOut(PlayerBase player, int seatIdx)
	{
	}

	bool Expansion_HasGear()
	{
		return false;
	}

	void Expansion_SwitchGear()
	{
	}

	bool Expansion_IsStoreLoaded()
	{
		return false;
	}

	bool Expansion_IsStoreSaved()
	{
		return false;
	}

	bool IsExploded()
	{
		return false;
	}

	bool Expansion_IsExploded()
	{
		return false;
	}

	string GetAnimSourceFromSelection(string selection)
	{
		return "";
	}
	
	string Expansion_GetAnimSourceFromSelection(string selection)
	{
		return GetAnimSourceFromSelection(selection);
	}
	
	string Expansion_GetDoorInvSlotNameFromSeatPos(int posIdx)
	{
		return GetDoorInvSlotNameFromSeatPos(posIdx);
	}
	
	string Expansion_GetDoorSelectionNameFromSeatPos(int posIdx)
	{
		return GetDoorSelectionNameFromSeatPos(posIdx);
	}

	int Expansion_GetCarDoorsState(string slotType)
	{
		return GetCarDoorsState(slotType);
	}

	void Expansion_CloseAllDoors()
	{
		m_ExpansionVehicle.SetAllDoorsAnimationPhase(0.0);
	}

	string GetDoorSelectionNameFromSeatPos(int posIdx)
	{
		return "";
	}

	string GetDoorInvSlotNameFromSeatPos(int posIdx)
	{
		return "";
	}

	int GetCarDoorsState(string slotType)
	{
		return CarDoorState.DOORS_MISSING;
	}

	bool IsVitalAircraftBattery()
	{
		return false;
	}

	bool IsVitalHelicopterBattery()
	{
		return false;
	}

	bool IsVitalIgniterPlug()
	{
		return false;
	}

	bool IsVitalCarBattery()
	{
		return false;
	}

	bool IsVitalTruckBattery()
	{
		return false;
	}

	float Expansion_GetThrottle()
	{
		return m_Controller.GetThrottle();
	}

	float Expansion_GetThrottle(int index)
	{
		return m_Controller.GetThrottle(index);
	}

	bool Expansion_IsVehicleFunctional(bool checkOptionalParts = false, set<typename> missingComponents = null)
	{
		return true;
	}

	bool IsScriptedLightsOn()
	{
		return false;
	}

	bool Expansion_CanGetInSeat(PlayerBase player, int seatIdx)
	{
		return true;
	}

	bool Expansion_CanPlayerAttach()
	{
		return ConfigGetFloat("animPhysDetachSpeed") > 0;
	}

	bool Expansion_CanObjectAttach(Object obj)
	{
		return ConfigGetFloat("animPhysDetachSpeed") > 0;
	}

	bool Expansion_IsBike()
	{
		return false;
	}

	bool Expansion_IsPlane()
	{
		return false;
	}

	bool Expansion_IsBoat()
	{
		return false;
	}

	bool Expansion_IsHelicopter()
	{
		return false;
	}

	bool Expansion_IsCar()
	{
		return false;
	}

	bool Expansion_IsDuck()
	{
		return false;
	}

	bool LeavingSeatDoesAttachment(int posIdx)
	{
		return true;
	}

	bool CanBeDamaged()
	{
		return true;
	}

	bool IsFreeLook()
	{
		return false;
	}
	
	ExpansionPhysicsState Expansion_GetPhysicsState()
	{
		return m_State;
	}

	void ExpansionSetSkin(int skinIndex)
	{
	}

	bool ExpansionHasSkin(int skinIndex)
	{
		return false;
	}

	string ExpansionGetCurrentSkinName()
	{
		return "";
	}
	
	int ExpansionGetCurrentSkinIndex()
	{
		return 0;
	}
};
