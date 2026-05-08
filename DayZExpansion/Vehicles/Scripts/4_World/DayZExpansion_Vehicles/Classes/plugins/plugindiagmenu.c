#ifdef DIAG_DEVELOPER
modded class PluginDiagMenu
{
	protected string m_ExpansionTextDiagsVehiclesMenu         = "EXP VEHICLES";
    
	protected int m_ExpansionDiagsRootMenu_Vehicles;
		protected int m_ExpansionDiagsEnum_VehiclesHeliSimMode;
		protected int m_ExpansionDiagsEnum_VehiclesHeliAirFrictionMode;
		protected int m_ExpansionDiagsEnum_VehiclesHeliAirFrictionX;
		protected int m_ExpansionDiagsEnum_VehiclesHeliAirFrictionY;
		protected int m_ExpansionDiagsEnum_VehiclesHeliAirFrictionZ;
		protected int m_ExpansionDiagsEnum_VehiclesHeliRBS;
		protected int m_ExpansionDiagsEnum_VehiclesHeliCollectiveDecay;
	
	override protected void RegisterModdedDiagsIDs()
	{
		super.RegisterModdedDiagsIDs();
		
		m_ExpansionDiagsRootMenu_Vehicles = GetModdedDiagID();
			m_ExpansionDiagsEnum_VehiclesHeliSimMode = GetModdedDiagID();
			m_ExpansionDiagsEnum_VehiclesHeliAirFrictionMode = GetModdedDiagID();
			m_ExpansionDiagsEnum_VehiclesHeliAirFrictionX = GetModdedDiagID();
			m_ExpansionDiagsEnum_VehiclesHeliAirFrictionY = GetModdedDiagID();
			m_ExpansionDiagsEnum_VehiclesHeliAirFrictionZ = GetModdedDiagID();
			m_ExpansionDiagsEnum_VehiclesHeliRBS = GetModdedDiagID();
			m_ExpansionDiagsEnum_VehiclesHeliCollectiveDecay = GetModdedDiagID();
	}
	
	override protected void RegisterModdedDiags()
	{
		super.RegisterModdedDiags();

		DiagMenu.RegisterMenu(m_ExpansionDiagsRootMenu_Vehicles, m_ExpansionTextDiagsVehiclesMenu, GetModdedRootMenu());
		{
			DiagMenu.RegisterItem(m_ExpansionDiagsEnum_VehiclesHeliSimMode, "", "Heli Sim Mode", m_ExpansionDiagsRootMenu_Vehicles,Expansion_GetVariableNames(ExpansionHelicopterSimulationMode, ","));
			
			DiagMenu.RegisterItem(m_ExpansionDiagsEnum_VehiclesHeliAirFrictionMode, "", "Heli Air Friction Mode", m_ExpansionDiagsRootMenu_Vehicles, Expansion_GetVariableNames(ExpansionHelicopterSimulationAirFrictionMode, ","));

			DiagMenu.RegisterRange(m_ExpansionDiagsEnum_VehiclesHeliAirFrictionX, "", "Heli Air Friction X", m_ExpansionDiagsRootMenu_Vehicles,string.Format("0,1.27,%1,0.01", ExpansionVehicleHelicopter_OLD.s_AirFriction[0]));

			DiagMenu.RegisterRange(m_ExpansionDiagsEnum_VehiclesHeliAirFrictionY, "", "Heli Air Friction Y", m_ExpansionDiagsRootMenu_Vehicles,string.Format("0,1.27,%1,0.01", ExpansionVehicleHelicopter_OLD.s_AirFriction[1]));

			DiagMenu.RegisterRange(m_ExpansionDiagsEnum_VehiclesHeliAirFrictionZ, "", "Heli Air Friction Z", m_ExpansionDiagsRootMenu_Vehicles,string.Format("0,1.27,%1,0.01", ExpansionVehicleHelicopter_OLD.s_AirFriction[2]));
	
			DiagMenu.RegisterBool(m_ExpansionDiagsEnum_VehiclesHeliRBS, "", "Heli RBS", m_ExpansionDiagsRootMenu_Vehicles);
	
			DiagMenu.RegisterBool(m_ExpansionDiagsEnum_VehiclesHeliCollectiveDecay, "", "Heli Collective Decay", m_ExpansionDiagsRootMenu_Vehicles);
		}
	}

	//! TODO: Maybe move to Core?
	string Expansion_GetVariableNames(typename e, string delim = "|")
	{
		int cnt = e.GetVariableCount();
		string names;

		for (int i = 0; i < cnt; ++i)
		{
			if (names.Length() > 0)
				names += delim;
			names += e.GetVariableName(i);
		}

		return names;
	}
};

modded class PluginDiagMenuClient
{
	override protected void BindCallbacks()
	{
		super.BindCallbacks();

		DiagMenu.BindCallback(m_ExpansionDiagsEnum_VehiclesHeliSimMode, Expansion_SetHeliSimMode);
		DiagMenu.BindCallback(m_ExpansionDiagsEnum_VehiclesHeliAirFrictionMode, Expansion_SetHeliAirFrictionMode);
		DiagMenu.BindCallback(m_ExpansionDiagsEnum_VehiclesHeliAirFrictionX, Expansion_SetHeliAirFrictionX);
		DiagMenu.BindCallback(m_ExpansionDiagsEnum_VehiclesHeliAirFrictionY, Expansion_SetHeliAirFrictionY);
		DiagMenu.BindCallback(m_ExpansionDiagsEnum_VehiclesHeliAirFrictionZ, Expansion_SetHeliAirFrictionZ);
		DiagMenu.BindCallback(m_ExpansionDiagsEnum_VehiclesHeliRBS, Expansion_SetHeliRBS);
		DiagMenu.BindCallback(m_ExpansionDiagsEnum_VehiclesHeliCollectiveDecay, Expansion_SetHeliCollectiveDecay);
	}

	static void Expansion_SetHeliSimMode(int value)
	{
		ExpansionVehicleHelicopter_OLD.s_UseLegacyFlightModel = false;
		ExpansionVehicleHelicopter_OLD.s_SimulationMode = value;
	}

	static void Expansion_SetHeliAirFrictionMode(int value)
	{
		ExpansionVehicleHelicopter_OLD.s_UseLegacyFlightModel = false;
		ExpansionVehicleHelicopter_OLD.s_AirFrictionMode = value;
	}

	static void Expansion_SetHeliAirFrictionX(float value)
	{
		ExpansionVehicleHelicopter_OLD.s_AirFriction[0] = value;
	}

	static void Expansion_SetHeliAirFrictionY(float value)
	{
		ExpansionVehicleHelicopter_OLD.s_AirFriction[1] = value;
	}

	static void Expansion_SetHeliAirFrictionZ(float value)
	{
		ExpansionVehicleHelicopter_OLD.s_AirFriction[2] = value;
	}

	static void Expansion_SetHeliRBS(bool enabled, int id)
	{
		GetExpansionClientSettings().EnableRetreatingBladeStall = enabled;
		ExpansionVehicleHelicopter_OLD.s_RBS = enabled;
	}

	static void Expansion_SetHeliCollectiveDecay(bool enabled, int id)
	{
		GetExpansionClientSettings().EnableCollectiveDecay = enabled;
		ExpansionVehicleHelicopter_OLD.s_CollectiveDecay = enabled;
	}
};
#endif