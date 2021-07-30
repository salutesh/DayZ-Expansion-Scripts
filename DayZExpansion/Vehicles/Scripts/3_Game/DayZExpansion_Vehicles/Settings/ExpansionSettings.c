/**
 * ExpansionSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ExpansionSettings
{
	static ref ScriptInvoker SI_Vehicle = new ScriptInvoker();
	
	protected autoptr ExpansionVehicleSettings m_SettingsVehicle;
	
	// ------------------------------------------------------------
	// Expansion OnServerInit
	// ------------------------------------------------------------
	override protected void OnServerInit()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("[VEHICLES] ExpansionSettings::OnServerInit - Start");
		#endif

		LoadSetting( m_SettingsVehicle);

		m_NetworkedSettings.Insert( "expansionvehiclesettings" );
		
		super.OnServerInit();

		#ifdef EXPANSIONEXPRINT
		EXPrint("[VEHICLES] ExpansionSettings::OnServerInit - End");
		#endif
	}

	// ------------------------------------------------------------
	override void Unload()
	{
		super.Unload();

		m_SettingsVehicle.Unload();
	}
	
	// ------------------------------------------------------------
	// Expansion CheckSettingsLoaded
	// Called on Client
	// ------------------------------------------------------------
	override protected void CheckSettingsLoaded()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("[VEHICLES] ExpansionSettings::CheckSettingsLoaded - Start");
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
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("[VEHICLES] ExpansionSettings::CheckSettingsLoaded - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Override Init
	// ------------------------------------------------------------
	override void Init()
	{
		m_SettingsVehicle = new ExpansionVehicleSettings;

		super.Init();
	}
	
	// ------------------------------------------------------------
	// Expansion Send
	// Can only be called on the server.
	// ------------------------------------------------------------
	override void Send( notnull PlayerIdentity identity )
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "[VEHICLES] ExpansionSettings::SendSettings - Start identity : " + identity );
		#endif

		if ( IsMissionClient() )
			return;

		super.Send( identity );

		m_SettingsVehicle.Send( identity );

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("[VEHICLES] ExpansionSettings::SendSettings - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// OnRPC
	// ------------------------------------------------------------
	override bool OnRPC( PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("[VEHICLES] ExpansionSettings::OnRPC - Start");
		#endif

		if ( !super.OnRPC( sender, target, rpc_type, ctx ) )
			return false;
		
		switch ( rpc_type )
		{
			case ExpansionSettingsRPC.Vehicle:
			{
				Expansion_Assert_False( m_SettingsVehicle.OnRecieve( ctx ), "Failed reading Vehicle settings" );
				#ifdef EXPANSIONEXPRINT
				EXPrint("ExpansionSettings::OnRPC RPC_Vehicle");
				#endif

				break;
			}
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("[VEHICLES] ExpansionSettings::OnRPC - End");
		#endif

		return true;
	}
	
	// ------------------------------------------------------------
	// Expansion Save
	// Called on server
	// ------------------------------------------------------------
	override void Save()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("[VEHICLES] ExpansionSettings::Save - Start");
		#endif

		super.Save();

		if ( IsMissionHost() && GetGame().IsMultiplayer() )
		{
			m_SettingsVehicle.Save();
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("[VEHICLES] ExpansionSettings::Save - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionVehicleSettings GetVehicle
	// ------------------------------------------------------------
	ExpansionVehicleSettings GetVehicle()
	{
		return m_SettingsVehicle;
	}
}
