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
	static ref ScriptInvoker SI_BaseBuilding = new ScriptInvoker();
	static ref ScriptInvoker SI_Raid = new ScriptInvoker();
	static ref ScriptInvoker SI_Territory = new ScriptInvoker();
	
	protected autoptr ExpansionBaseBuildingSettings m_SettingsBaseBuilding;
	protected autoptr ExpansionRaidSettings m_SettingsRaid;
	protected autoptr ExpansionTerritorySettings m_SettingsTerritory;
	
	// ------------------------------------------------------------
	// Expansion OnServerInit
	// ------------------------------------------------------------
	override protected void OnServerInit()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "OnServerInit");
#endif

		LoadSetting( m_SettingsBaseBuilding );
		LoadSetting( m_SettingsRaid );
		LoadSetting( m_SettingsTerritory );

		m_NetworkedSettings.Insert( "expansionbasebuildingsettings" );
		m_NetworkedSettings.Insert( "expansionraidsettings" );
		m_NetworkedSettings.Insert( "expansionterritorysettings" );
		
		super.OnServerInit();
	}

	// ------------------------------------------------------------
	override void Unload()
	{
		super.Unload();

		m_SettingsBaseBuilding.Unload();
		m_SettingsRaid.Unload();
		m_SettingsTerritory.Unload();
	}
	
	// ------------------------------------------------------------
	// Expansion CheckSettingsLoaded
	// Called on Client
	// ------------------------------------------------------------
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

		if ( !IsSettingLoaded( m_SettingsBaseBuilding, m_SettingsLoaded ) )
			return;

		if ( !IsSettingLoaded( m_SettingsRaid, m_SettingsLoaded ) )
			return;

		if ( !IsSettingLoaded( m_SettingsTerritory, m_SettingsLoaded ) )
			return;

		super.CheckSettingsLoaded();
	}
	
	// ------------------------------------------------------------
	// Override Init
	// ------------------------------------------------------------
	override void Init()
	{
		m_SettingsBaseBuilding = new ExpansionBaseBuildingSettings;
		m_SettingsRaid = new ExpansionRaidSettings;
		m_SettingsTerritory = new ExpansionTerritorySettings;

		super.Init();
	}
	
	// ------------------------------------------------------------
	// Expansion Send
	// Can only be called on the server.
	// ------------------------------------------------------------
	override void Send( notnull PlayerIdentity identity )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "Send").Add(identity);
#endif

		if ( IsMissionClient() )
			return;

		super.Send( identity );

		m_SettingsBaseBuilding.Send( identity );
		m_SettingsRaid.Send( identity );
		m_SettingsTerritory.Send( identity );
	}
	
	// ------------------------------------------------------------
	// OnRPC
	// ------------------------------------------------------------
	override bool OnRPC( PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "OnRPC");
#endif

		if (super.OnRPC(sender, target, rpc_type, ctx))
			return true;

		if ( rpc_type <= ExpansionSettingsRPC.INVALID || rpc_type >= ExpansionSettingsRPC.COUNT )
			return false;
		
		switch ( rpc_type )
		{			
			case ExpansionSettingsRPC.BaseBuilding:
			{
				Expansion_Assert_False( m_SettingsBaseBuilding.OnRecieve( ctx ), "Failed reading BaseBuilding settings" );

				return true;
			}
			
			case ExpansionSettingsRPC.Raid:
			{
				Expansion_Assert_False( m_SettingsRaid.OnRecieve( ctx ), "Failed reading Raid settings" );

				return true;
			}
			
			case ExpansionSettingsRPC.Territory:
			{
				Expansion_Assert_False( m_SettingsTerritory.OnRecieve( ctx ), "Failed reading Territory settings" );

				return true;
			}
		}

		return false;
	}
	
	// ------------------------------------------------------------
	// Expansion Save
	// Called on server
	// ------------------------------------------------------------
	override void Save()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "Save");
#endif

		super.Save();

		if ( IsMissionHost() && GetGame().IsMultiplayer() )
		{
			m_SettingsBaseBuilding.Save();
			m_SettingsRaid.Save();
			m_SettingsTerritory.Save();
		}
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionBaseBuildingSettings GetBaseBuilding
	// ------------------------------------------------------------
	ExpansionBaseBuildingSettings GetBaseBuilding()
	{
		return m_SettingsBaseBuilding;
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionRaidSettings GetRaid
	// ------------------------------------------------------------
	ExpansionRaidSettings GetRaid()
	{
		return m_SettingsRaid;
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionTerritorySettings GetTerritory
	// ------------------------------------------------------------
	ExpansionTerritorySettings GetTerritory()
	{
		return m_SettingsTerritory;
	}
};
