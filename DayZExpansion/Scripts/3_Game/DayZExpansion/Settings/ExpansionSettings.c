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
	static ref ScriptInvoker SI_General = new ScriptInvoker();
	static ref ScriptInvoker SI_PlayerList = new ScriptInvoker();
	static ref ScriptInvoker SI_SocialMedia = new ScriptInvoker();

	protected autoptr ExpansionGeneralSettings m_SettingsGeneral;
	protected autoptr ExpansionPlayerListSettings m_SettingsPlayerList;
	protected autoptr ExpansionSocialMediaSettings m_SettingsSocialMedia;

	// ------------------------------------------------------------
	// Expansion OnServerInit
	// ------------------------------------------------------------
	override protected void OnServerInit()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "OnServerInit");
#endif

		LoadSetting( m_SettingsGeneral );
		LoadSetting( m_SettingsPlayerList);
		LoadSetting( m_SettingsSocialMedia);

		m_NetworkedSettings.Insert( "expansiongeneralsettings" );
		m_NetworkedSettings.Insert( "expansionplayerlistsettings" );
		m_NetworkedSettings.Insert( "expansionsocialmediasettings" );

		super.OnServerInit();
	}

	// ------------------------------------------------------------
	override void Unload()
	{
		super.Unload();

		m_SettingsGeneral.Unload();
		m_SettingsPlayerList.Unload();
		m_SettingsSocialMedia.Unload();
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

		if ( !IsSettingLoaded( m_SettingsGeneral, m_SettingsLoaded ) )
			return;

		if ( !IsSettingLoaded( m_SettingsPlayerList, m_SettingsLoaded ) )
			return;

		if ( !IsSettingLoaded( m_SettingsSocialMedia, m_SettingsLoaded ) )
			return;

		super.CheckSettingsLoaded();
	}

	// ------------------------------------------------------------
	// Override Init
	// ------------------------------------------------------------
	override void Init()
	{
		m_SettingsGeneral = new ExpansionGeneralSettings;
		m_SettingsPlayerList = new ExpansionPlayerListSettings;
		m_SettingsSocialMedia = new ExpansionSocialMediaSettings;

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

		m_SettingsGeneral.Send( identity );
		m_SettingsPlayerList.Send( identity );
		m_SettingsSocialMedia.Send( identity );
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
			case ExpansionSettingsRPC.General:
			{
				Expansion_Assert_False( m_SettingsGeneral.OnRecieve( ctx ), "Failed reading General settings" );

				return true;
			}

			case ExpansionSettingsRPC.PlayerList:
			{
				Expansion_Assert_False( m_SettingsPlayerList.OnRecieve( ctx ), "Failed reading PlayerList settings" );

				return true;
			}

			case ExpansionSettingsRPC.SocialMedia:
			{
				Expansion_Assert_False( m_SettingsSocialMedia.OnRecieve( ctx ), "Failed reading PlayerList settings" );

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
			m_SettingsGeneral.Save();
			m_SettingsPlayerList.Save();
			m_SettingsSocialMedia.Save();
		}
	}

	// ------------------------------------------------------------
	// Expansion ExpansionGeneralSettings GetGeneral
	// ------------------------------------------------------------
	ExpansionGeneralSettings GetGeneral()
	{
		return m_SettingsGeneral;
	}

	// ------------------------------------------------------------
	// Expansion ExpansionPlayerListSettings GetPlayerList
	// ------------------------------------------------------------
	ExpansionPlayerListSettings GetPlayerList()
	{
		return m_SettingsPlayerList;
	}

	// ------------------------------------------------------------
	// Expansion ExpansionPlayerListSettings GetSocialMedia
	// ------------------------------------------------------------
	ExpansionSocialMediaSettings GetSocialMedia()
	{
		return m_SettingsSocialMedia;
	}
};
