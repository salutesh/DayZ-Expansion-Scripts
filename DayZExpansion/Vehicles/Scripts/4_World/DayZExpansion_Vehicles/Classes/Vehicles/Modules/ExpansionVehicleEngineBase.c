/*
 * ExpansionVehicleEngineBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

enum ExpansionVehicleEngineType
{
	CAR,
	POINTS
};

static ExpansionVehicleEngineBase Expansion_CreateEngine(EntityAI vehicle, string path)
{
	int type = ExpansionVehicleEngineType.CAR;
	string typePath = path + " type";

	if (g_Game.ConfigIsExisting(typePath))
	{
		switch (g_Game.ConfigGetType(typePath))
		{
		case CT_STRING:
			string typeName;
			g_Game.ConfigGetText(typePath, typeName);
			typeName.ToUpper();
			type = typename.StringToEnum(ExpansionVehicleEngineType, typeName);
			break;
		default:
			type = g_Game.ConfigGetInt(typePath);
			break;
		}
	}

	switch (type)
	{
	case ExpansionVehicleEngineType.CAR:
		return new ExpansionVehicleCarEngine(vehicle, path);
	case ExpansionVehicleEngineType.POINTS:
		return new ExpansionVehicleEngine(vehicle, path);
	}

	return null;
}

class ExpansionVehicleEngineBase : ExpansionVehicleRotational
{
	int m_EngineIndex;
	int m_GearIndex;
	int m_ThrottleIndex;

	bool m_State;
	
	float m_RPM;

	float m_RPMIdle;
	float m_RPMMin;
	float m_RPMClutch;
	float m_RPMRedline;
	float m_RPMMax;

	float m_Throttle;
	float m_ThrottleRange;
	float m_ThrottleIdle;

	float m_MinTorque;
	float m_MaxTorque;

	ExpansionVehicleGearbox m_Gearbox;

	string m_DamageZone;
	float m_Health;

	float m_FuelConsumption;

	void ExpansionVehicleEngineBase(EntityAI vehicle, string rootPath)
	{
		string path;

		path = rootPath + " gear";
		if (g_Game.ConfigIsExisting(path))
			m_GearIndex = g_Game.ConfigGetInt(path);

		path = rootPath + " throttle";
		if (g_Game.ConfigIsExisting(path))
			m_ThrottleIndex = g_Game.ConfigGetInt(path);

		path = rootPath + " inertia";
		m_Inertia = 0.9;
		if (g_Game.ConfigIsExisting(path))
			m_Inertia = g_Game.ConfigGetFloat(path);

		path = rootPath + " rpmIdle";
		m_RPMIdle = 0;
		if (g_Game.ConfigIsExisting(path))
			m_RPMIdle = g_Game.ConfigGetFloat(path);

		path = rootPath + " rpmMin";
		m_RPMMin = 0;
		if (g_Game.ConfigIsExisting(path))
			m_RPMMin = g_Game.ConfigGetFloat(path);

		path = rootPath + " rpmClutch";
		m_RPMClutch = 0;
		if (g_Game.ConfigIsExisting(path))
			m_RPMClutch = g_Game.ConfigGetFloat(path);

		path = rootPath + " rpmRedline";
		m_RPMRedline = 0;
		if (g_Game.ConfigIsExisting(path))
			m_RPMRedline = g_Game.ConfigGetFloat(path);

		path = rootPath + " rpmMax";
		m_RPMMax = 0;
		if (g_Game.ConfigIsExisting(path))
			m_RPMMax = g_Game.ConfigGetFloat(path);

		path = rootPath + " damageZone";
		m_DamageZone = "Engine";
		if (g_Game.ConfigIsExisting(path))
			m_DamageZone = g_Game.ConfigGetTextOut(path);

		m_Position = ExpansionVehiclesStatic.GetCenterSelection(m_Vehicle, "geometryView", "dmgzone_" + m_DamageZone);

		m_FuelConsumption = g_Game.ConfigGetFloat("CfgVehicles " + m_Vehicle.GetType() + " fuelConsumption");
	}

	override void Init()
	{
		super.Init();

		if (m_Vehicle.IsInherited(CarScript))
			Init_CarScript();
		else
			Init_ExpansionVehicle();
		
		AddComponent(m_Gearbox);

		m_Gearbox.m_Engine = this;
		m_GearIndex = m_Gearbox.m_GearIndex;

		m_ThrottleIdle = m_RPMIdle / m_RPMMax;
		m_ThrottleRange = 1.0 - m_ThrottleIdle;
	}

	void Init_CarScript()
	{
		auto vehicle = CarScript.Cast(m_Vehicle);

		if (m_GearIndex >= 0)
			m_Gearbox = vehicle.m_Gearboxes[m_GearIndex];

		if (!m_Gearbox)
		{
			ExpansionVehicleGearbox gBox = new ExpansionVehicleGearbox(m_Vehicle, "");
			vehicle.AddModule(gBox);
			m_Gearbox = gBox;
		}
	}

	void Init_ExpansionVehicle()
	{
		auto vehicle = ExpansionVehicleBase.Cast(m_Vehicle);

		if (m_GearIndex >= 0)
			m_Gearbox = vehicle.m_Gearboxes[m_GearIndex];

		if (!m_Gearbox)
		{
			ExpansionVehicleGearbox gBox = new ExpansionVehicleGearbox(m_Vehicle, "");
			vehicle.AddModule(gBox);
			m_Gearbox = gBox;
		}
	}

	override void PreSimulate(ExpansionPhysicsState pState)
	{
		m_Torque = 0;
		m_MinTorque = CalculateMinTorque();
		
		m_Throttle = ((m_Controller.m_Throttle[m_ThrottleIndex] * m_ThrottleRange) + m_ThrottleIdle) * m_Controller.m_State[m_EngineIndex];
		m_Throttle = Math.Clamp(m_Throttle, 0, 1);

		// apply a starting torque
		if (m_Controller.m_State[m_EngineIndex] && m_State != m_Controller.m_State[m_EngineIndex])
		{
			m_Torque = 50.0;
			if (m_RPM >= m_RPMIdle)
			{
				m_State = true;
			}
		}
		else if (!m_Controller.m_State[m_EngineIndex])
		{
			m_State = false;
		}
		else if (m_State)
		{
			m_MaxTorque = CalculateTorque(pState);
			m_Torque = (m_MaxTorque - m_MinTorque) * m_Throttle;
		}
		
		m_Torque += m_MinTorque;

		if (m_Velocity + m_Torque < 0)
		{
			m_Velocity = 0.0;
			m_Controller.m_State[m_EngineIndex] = false;
		}

		m_Controller.m_Torque[m_EngineIndex] = m_Torque;
	}
	
	override void ProcessTorque(ExpansionPhysicsState pState)
	{
	}

	override void ProcessAcceleration(ExpansionPhysicsState pState)
	{
		super.ProcessAcceleration(pState);

		m_RPM = ToRPM(m_Velocity);
	}
	
#ifdef CF_DebugUI
	override bool CF_OnDebugUpdate(CF_Debug instance, CF_DebugUI_Type type)
	{
		super.CF_OnDebugUpdate(instance, type);

		instance.Add("RPM", m_RPM);
		instance.Add("Throttle", m_Throttle);
		instance.Add("Throttle Range", m_ThrottleRange);
		instance.Add("Throttle Idle", m_ThrottleIdle);
		instance.Add("Min Torque", m_MinTorque);
		instance.Add("Max Torque", m_MaxTorque);
		instance.Add("Engine Torque", (m_MaxTorque * m_Throttle));

		return true;
	}
#endif

	static float SmoothStep(float t)
	{
		return t * t * (3.0 - (2.0 * t));
	}

	float CalculateTorque(ExpansionPhysicsState pState)
	{
		return 0;
	}

	float CalculateMinTorque()
	{
		return -1.125 * m_RPM / 60.0;
	}

	void ProcessHealth(float pDt, float pFuel, float pCoolant, float pOil, float pBrake, out float pOutHealth, out float pOutFuel)
	{
		float dmg;

		m_Health = m_Vehicle.GetHealth01(m_DamageZone, "");
		pOutHealth += m_Health;

		if (pFuel <= 0.0 || m_Health <= 0.0)
		{
			m_Controller.m_State[m_EngineIndex] = false;
		}

		auto vehicle = ExpansionVehicle.Get(m_Vehicle);

		//! @note helicopters are special - they have a CarScript engine, but it will never be on, and its RPM as well as throttle will always be zero.
		//! Yet, it will be used to process engine health and fuel consumption.

		if (!vehicle.IsHelicopter() && !vehicle.EngineIsOn(m_EngineIndex))
			return;

		CarScript car = vehicle.GetCar();

		if (vehicle.CanBeDamaged())
		{
			if (m_RPM >= m_RPMRedline)
			{
				if (m_RPM > m_RPMMax && GetExpansionSettings().GetVehicle().RevvingOverMaxRPMRuinsEngineInstantly)
					dmg += m_Vehicle.GetMaxHealth(m_DamageZone, "") * 0.05;

			#ifdef DAYZ_1_27
				dmg += m_RPM * 0.001 * Math.RandomFloat(0.02, 1.0) * pDt;
			#else
				//! 1.28+
				dmg += m_RPM * 0.001 * m_Transport.RandomFloat(0.02, 1.0) * pDt;
			#endif

				//! 1.19+
				if (car)
					car.SetEngineZoneReceivedHit(true);
			}
			//! 1.19+
			else if (car)
			{
				car.SetEngineZoneReceivedHit(false);
			}

			if (pCoolant >= 0 && pCoolant < 0.5)
			{
			#ifdef DAYZ_1_27
				dmg += (1.0 - pCoolant) * Math.RandomFloat(0.02, 10.00) * pDt;
			#else
				//! 1.28+
				dmg += (1.0 - pCoolant) * m_Transport.RandomFloat(0.02, 10.00) * pDt;
			#endif
			}

			if (pOil < 1.0)
			{
				dmg += Math.Lerp(0.02, 10, 1.0 - pOil);
			}

			if (dmg != 0.0)
			{
				m_Vehicle.AddHealth(m_DamageZone, "Health", -dmg);
			}
		}

		if (m_RPM >= m_RPMIdle || vehicle.IsHelicopter() || vehicle.GetThrottle(m_EngineIndex) > 0)
		{
			pOutFuel += m_FuelConsumption * pDt / 3600.0;
		}
	}
};
