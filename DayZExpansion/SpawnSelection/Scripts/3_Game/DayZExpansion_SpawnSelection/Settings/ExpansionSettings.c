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
	static ref ScriptInvoker SI_Spawn = new ScriptInvoker();
	protected autoptr ExpansionSpawnSettings m_SettingsSpawn;

	// ------------------------------------------------------------
	// Expansion OnServerInit
	// ------------------------------------------------------------
	override protected void OnServerInit()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "OnServerInit");
#endif

		LoadSetting( m_SettingsSpawn );

		m_NetworkedSettings.Insert( "expansionspawnsettings" );

		super.OnServerInit();
	}

	// ------------------------------------------------------------
	override void Unload()
	{
		super.Unload();

		m_SettingsSpawn.Unload();
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

		if ( !IsSettingLoaded( m_SettingsSpawn, m_SettingsLoaded ) )
			return;

		super.CheckSettingsLoaded();
	}

	// ------------------------------------------------------------
	// Override Init
	// ------------------------------------------------------------
	override void Init()
	{
		m_SettingsSpawn = new ExpansionSpawnSettings;

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

		m_SettingsSpawn.Send( identity );
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
			case ExpansionSettingsRPC.Spawn:
			{
				Expansion_Assert_False( m_SettingsSpawn.OnRecieve( ctx ), "Failed reading Spawn settings" );

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
			m_SettingsSpawn.Save();
		}
	}

	// ------------------------------------------------------------
	// Expansion ExpansionSpawnSettings GetSpawn
	// ------------------------------------------------------------
	ExpansionSpawnSettings GetSpawn()
	{
		return m_SettingsSpawn;
	}
};
