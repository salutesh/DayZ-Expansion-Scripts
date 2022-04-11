/**
 * ExpansionSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ExpansionSettings
{
	static ref ScriptInvoker SI_Vehicle = new ScriptInvoker();
	
	protected autoptr ExpansionVehicleSettings m_SettingsVehicle;
	
	override protected void OnServerInit()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "OnServerInit");
#endif

		LoadSetting( m_SettingsVehicle);

		m_NetworkedSettings.Insert( "expansionvehiclesettings" );
		
		super.OnServerInit();
	}

	override void Unload()
	{
		super.Unload();

		m_SettingsVehicle.Unload();
	}
	
	override protected void CheckSettingsLoaded()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "CheckSettingsLoaded");
#endif

		if ( !IsMissionClient() )
		{
			m_SettingsLoaded = true;

			return;
		}

		if ( m_SettingsLoaded )
			return;
		
		if ( !IsSettingLoaded( m_SettingsVehicle, m_SettingsLoaded ) )
			return;

		super.CheckSettingsLoaded();
	}
	
	override void Init()
	{
		m_SettingsVehicle = new ExpansionVehicleSettings;

		super.Init();
	}
	
	override void Send( notnull PlayerIdentity identity )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "Send").Add(identity);
#endif

		if ( IsMissionClient() )
			return;

		super.Send( identity );

		m_SettingsVehicle.Send( identity );
	}
	
	override bool OnRPC( PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_4(ExpansionTracing.SETTINGS, this, "OnRPC").Add(sender).Add(target).Add(rpc_type).Add(ctx);
#endif

		if (super.OnRPC(sender, target, rpc_type, ctx))
			return true;

		if ( rpc_type <= ExpansionSettingsRPC.INVALID || rpc_type >= ExpansionSettingsRPC.COUNT )
			return false;
		
		switch ( rpc_type )
		{
			case ExpansionSettingsRPC.Vehicle:
			{
				Expansion_Assert_False( m_SettingsVehicle.OnRecieve( ctx ), "Failed reading Vehicle settings" );

				return true;
			}
		}

		return false;
	}

	override void Save()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "Save");
#endif

		super.Save();

		if ( IsMissionHost() && GetGame().IsMultiplayer() )
		{
			m_SettingsVehicle.Save();
		}
	}
	
	ExpansionVehicleSettings GetVehicle()
	{
		return m_SettingsVehicle;
	}
};
