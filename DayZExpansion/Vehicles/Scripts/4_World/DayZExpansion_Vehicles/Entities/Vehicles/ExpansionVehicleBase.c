/**
 * ExpansionVehicleBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

enum ExpansionVehicleDynamicState
{
	STATIC = 0,
	TRANSITIONING,
	DYNAMIC
};

//! This is just a stub
class ExpansionVehicleBase extends ItemBase
{
	ref array<ExpansionVehicleGearbox> m_Gearboxes = new array<ExpansionVehicleGearbox>();

	ref ExpansionController m_Controller = new ExpansionController();

	ExpansionVehicleDifferentialType m_DifferentialType;

	protected float m_dmgContactCoef;

	static set<ExpansionVehicleBase> GetAll()
	{
		return new set<ExpansionVehicleBase>;
	}

	void HandleController(DayZPlayerImplement driver, float dt)
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

	void AddModule(ExpansionVehicleModule module)
	{
	}

	ExpansionController GetExpansionController()
	{
		return null;
	}

	ExpansionController GetController()
	{
		return null;
	}

	int CrewSize()
	{
		return 0;
	}

	int CrewMemberIndex(Human player)
	{
		return -1;
	}

	Human CrewMember(int posIdx)
	{
		return null;
	}

	void CrewGetIn(Human player, int posIdx)
	{
	}

	Human CrewGetOut(int posIdx)
	{
		return null;
	}

	void CrewEntry(int posIdx, out vector pos, out vector dir)
	{
	}

	void CrewTransform(int posIdx, out vector mat[4])
	{
	}

	int Get3rdPersonCameraType()
	{
		return DayZPlayerCameras.DAYZCAMERA_3RD_VEHICLE;
	}

	float GetSpeedometer()
	{
		return 0;
	}

	float GetFluidCapacity(CarFluid fluid)
	{
		return 0;
	}

	float GetFluidFraction(CarFluid fluid)
	{
		return 0;
	}

	void Fill(CarFluid fluid, float amount)
	{
	}

	float EngineGetRPMMax()
	{
		return 0;
	}

	float EngineGetRPMRedline()
	{
		return 0;
	}

	float EngineGetRPM()
	{
		return 0;
	}

	bool EngineIsOn()
	{
		return false;
	}

	void OnContact(string zoneName, vector localPos, IEntity other, Contact data)
	{
	}

	int GetPersistentIDA()
	{
		return 0;
	}

	int GetPersistentIDB()
	{
		return 0;
	}

	int GetPersistentIDC()
	{
		return 0;
	}

	int GetPersistentIDD()
	{
		return 0;
	}

	void SetPersistentIDA(int newIDA)
	{
	}

	void SetPersistentIDB(int newIDB)
	{
	}

	void SetPersistentIDC(int newIDC)
	{
	}

	void SetPersistentIDD(int newIDD)
	{
	}

	bool HasKey()
	{
		return false;
	}

	void PairKeyTo(ExpansionCarKey key)
	{
	}

	void ResetKeyPairing()
	{
	}

	bool CanBeLocked()
	{
		return false;
	}

	void LockCar(ExpansionCarKey key = NULL)
	{
	}

	void PlayHorn()
	{
	}

	void StopHorn()
	{
	}

	bool HasGear()
	{
		return false;
	}

	void SwitchGear()
	{
	}

	bool IsExploded()
	{
		return false;
	}

	int GetCarDoorsState(string slotType)
	{
		return CarDoorState.DOORS_MISSING;
	}

	bool IsBatteryWorking()
	{
		return false;
	}

	bool IsPlane()
	{
		return false;
	}

	bool IsBoat()
	{
		return false;
	}

	bool IsHelicopter()
	{
		return false;
	}

	bool IsCar()
	{
		return false;
	}

	float GetTransportCameraDistance()
	{
		return 6.0;
	}

	vector GetTransportCameraOffset()
	{
		return "0 3.2 0";
	}

	bool IsFreeLook()
	{
		return false;
	}
};
