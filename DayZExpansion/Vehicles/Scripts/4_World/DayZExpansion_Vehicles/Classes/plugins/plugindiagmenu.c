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
		protected int m_ExpansionDiagsEnum_VehiclesHeliAutoCollectiveMode;
		protected int m_ExpansionDiagsEnum_VehiclesHeliAutoTrim;
		protected int m_ExpansionDiagsEnum_VehiclesHeliTranslatingTendency;
	
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
			m_ExpansionDiagsEnum_VehiclesHeliTranslatingTendency = GetModdedDiagID();
			m_ExpansionDiagsEnum_VehiclesHeliAutoCollectiveMode = GetModdedDiagID();
			m_ExpansionDiagsEnum_VehiclesHeliAutoTrim = GetModdedDiagID();
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

			//! Settings that are also avaialble via client settings (needs to be kept in sync)

			DiagMenu.RegisterBool(m_ExpansionDiagsEnum_VehiclesHeliRBS, "", "Heli RBS", m_ExpansionDiagsRootMenu_Vehicles);

			DiagMenu.RegisterBool(m_ExpansionDiagsEnum_VehiclesHeliTranslatingTendency, "", "Heli Translating Tendency", m_ExpansionDiagsRootMenu_Vehicles);
	
			DiagMenu.RegisterItem(m_ExpansionDiagsEnum_VehiclesHeliAutoCollectiveMode, "", "Heli Auto Collective", m_ExpansionDiagsRootMenu_Vehicles,Expansion_GetVariableNames(ExpansionHelicopterAutoCollectiveMode, ","));
	
			DiagMenu.RegisterBool(m_ExpansionDiagsEnum_VehiclesHeliAutoTrim, "", "Heli Disable Auto-Trim", m_ExpansionDiagsRootMenu_Vehicles);

		#ifndef SERVER
			auto settings = GetExpansionClientSettings();
			settings.SI_UpdateSetting.Insert(Expansion_OnClientSettingsChanged);

			Expansion_OnClientSettingsChanged();
		#endif
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

	void Expansion_OnClientSettingsChanged()
	{
		auto settings = GetExpansionClientSettings();

		DiagMenu.SetValue(m_ExpansionDiagsEnum_VehiclesHeliRBS, settings.EnableRetreatingBladeStall);
		DiagMenu.SetValue(m_ExpansionDiagsEnum_VehiclesHeliTranslatingTendency, settings.EnableTranslatingTendency);
		DiagMenu.SetValue(m_ExpansionDiagsEnum_VehiclesHeliAutoCollectiveMode, settings.AutoCollectiveMode);
		DiagMenu.SetValue(m_ExpansionDiagsEnum_VehiclesHeliAutoTrim, settings.DisableAutoTrim);
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
		DiagMenu.BindCallback(m_ExpansionDiagsEnum_VehiclesHeliTranslatingTendency, Expansion_SetHeliTranslatingTendency);
		DiagMenu.BindCallback(m_ExpansionDiagsEnum_VehiclesHeliAutoCollectiveMode, Expansion_SetHeliAutoCollectiveMode);
		DiagMenu.BindCallback(m_ExpansionDiagsEnum_VehiclesHeliAutoTrim, Expansion_SetHeliAutoTrim);
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
		auto settings = GetExpansionClientSettings();
		enabled = !settings.EnableRetreatingBladeStall;
		settings.EnableRetreatingBladeStall = enabled;
		ExpansionVehicleHelicopter_OLD.s_RBS = enabled;
	}

	static void Expansion_SetHeliTranslatingTendency(bool enabled, int id)
	{
		auto settings = GetExpansionClientSettings();
		enabled = !settings.EnableTranslatingTendency;
		settings.EnableTranslatingTendency = enabled;
		ExpansionVehicleHelicopter_OLD.s_TranslatingTendency = enabled;
	}

	static void Expansion_SetHeliAutoCollectiveMode(int mode)
	{
		auto settings = GetExpansionClientSettings();
		settings.AutoCollectiveMode = mode;

		switch (mode)
		{
			case ExpansionHelicopterAutoCollectiveMode.AlwaysOn:
				ExpansionVehicleHelicopter_OLD.s_AutoCollective = true;
				break;

			case ExpansionHelicopterAutoCollectiveMode.AlwaysOff:
				ExpansionVehicleHelicopter_OLD.s_AutoCollective = false;
				break;
		}
	}

	static void Expansion_SetHeliAutoTrim(bool enabled, int id)
	{
		auto settings = GetExpansionClientSettings();
		enabled = !settings.DisableAutoTrim;
		settings.DisableAutoTrim = enabled;
		ExpansionVehicleHelicopter_OLD.s_AutoTrim = !enabled;
	}
};
#endif