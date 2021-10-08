/**
 * ExpansionSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ExpansionSettings
{
	static ref ScriptInvoker SI_Market = new ScriptInvoker();
	
	protected autoptr ExpansionMarketSettings m_SettingsMarket;
	
	// ------------------------------------------------------------
	// Expansion OnServerInit
	// ------------------------------------------------------------
	override protected void OnServerInit()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("[MARKET] ExpansionSettings::OnServerInit - Start");
		#endif

		LoadSetting( m_SettingsMarket);

		m_NetworkedSettings.Insert( "expansionmarketsettings" );
		
		super.OnServerInit();

		#ifdef EXPANSIONEXPRINT
		EXPrint("[MARKET] ExpansionSettings::OnServerInit - End");
		#endif
	}

	// ------------------------------------------------------------
	override void Unload()
	{
		super.Unload();

		m_SettingsMarket.Unload();
	}
	
	// ------------------------------------------------------------
	// Expansion CheckSettingsLoaded
	// Called on Client
	// ------------------------------------------------------------
	override protected void CheckSettingsLoaded()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("[MARKET] ExpansionSettings::CheckSettingsLoaded - Start");
		#endif

		if ( !IsMissionClient() )
		{
			m_SettingsLoaded = true;

			return;
		}

		if ( m_SettingsLoaded )
			return;
		
		if ( !IsSettingLoaded( m_SettingsMarket, m_SettingsLoaded ) )
			return;

		super.CheckSettingsLoaded();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("[MARKET] ExpansionSettings::CheckSettingsLoaded - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Override Init
	// ------------------------------------------------------------
	override void Init()
	{
		m_SettingsMarket = new ExpansionMarketSettings;

		super.Init();
	}
	
	// ------------------------------------------------------------
	// Expansion Send
	// Can only be called on the server.
	// ------------------------------------------------------------
	override void Send( notnull PlayerIdentity identity )
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "[MARKET] ExpansionSettings::SendSettings - Start identity : " + identity );
		#endif

		if ( IsMissionClient() )
			return;

		super.Send( identity );

		m_SettingsMarket.Send( identity );

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("[MARKET] ExpansionSettings::SendSettings - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// OnRPC
	// ------------------------------------------------------------
	override bool OnRPC( PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("[MARKET] ExpansionSettings::OnRPC - Start");
		#endif

		if (super.OnRPC(sender, target, rpc_type, ctx))
			return true;

		if ( rpc_type <= ExpansionSettingsRPC.INVALID || rpc_type >= ExpansionSettingsRPC.COUNT )
			return false;
		
		switch ( rpc_type )
		{
			case ExpansionSettingsRPC.Market:
			{
				Expansion_Assert_False( m_SettingsMarket.OnRecieve( ctx ), "Failed reading Market settings" );
				#ifdef EXPANSIONEXPRINT
				EXPrint("ExpansionSettings::OnRPC RPC_Market");
				#endif

				return true;
			}
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("[MARKET] ExpansionSettings::OnRPC - End");
		#endif

		return false;
	}
	
	// ------------------------------------------------------------
	// Expansion Save
	// Called on server
	// ------------------------------------------------------------
	override void Save()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("[MARKET] ExpansionSettings::Save - Start");
		#endif

		super.Save();

		if ( IsMissionHost() && GetGame().IsMultiplayer() )
		{
			m_SettingsMarket.Save();
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("[MARKET] ExpansionSettings::Save - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionMarketSettings GetMarket
	// ------------------------------------------------------------
	ExpansionMarketSettings GetMarket()
	{
		return m_SettingsMarket;
	}
}
