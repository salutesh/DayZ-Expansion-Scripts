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
	static ref ScriptInvoker SI_NameTags = new ScriptInvoker();
	protected autoptr ExpansionNameTagsSettings m_SettingsNameTags;
		
	// ------------------------------------------------------------
	// Expansion OnServerInit
	// ------------------------------------------------------------
	override protected void OnServerInit()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("[NAME TAGS] ExpansionSettings::OnServerInit - Start");
		#endif
		
		LoadSetting( m_SettingsNameTags );
		
		m_NetworkedSettings.Insert( "expansionnametagssettings" );
		
		super.OnServerInit();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("[NAME TAGS] ExpansionSettings::OnServerInit - End");
		#endif
	}

	// ------------------------------------------------------------
	override void Unload()
	{
		super.Unload();

		m_SettingsNameTags.Unload();
	}
	
	// ------------------------------------------------------------
	// Expansion CheckSettingsLoaded
	// Called on Client
	// ------------------------------------------------------------
	override protected void CheckSettingsLoaded()
	{		
		if ( !IsMissionClient() )
		{
			m_SettingsLoaded = true;

			return;
		}

		if ( m_SettingsLoaded )
			return;
		
		if ( !IsSettingLoaded( m_SettingsNameTags, m_SettingsLoaded ) )
			return;

		super.CheckSettingsLoaded();
	}
	
	// ------------------------------------------------------------
	// Override Init
	// ------------------------------------------------------------
	override void Init()
	{
		m_SettingsNameTags = new ExpansionNameTagsSettings;

		super.Init();
	}
	
	// ------------------------------------------------------------
	// Expansion Send
	// Can only be called on the server.
	// ------------------------------------------------------------
	override void Send( notnull PlayerIdentity identity )
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "[NAME TAGS] ExpansionSettings::SendSettings - Start identity : " + identity );
		#endif

		if ( IsMissionClient() )
			return;

		super.Send( identity );

		m_SettingsNameTags.Send( identity );

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("[NAME TAGS] ExpansionSettings::SendSettings - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// OnRPC
	// ------------------------------------------------------------
	override bool OnRPC( PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("[NAME TAGS] ExpansionSettings::OnRPC - Start");
		#endif

		if (super.OnRPC(sender, target, rpc_type, ctx))
			return true;

		if ( rpc_type <= ExpansionSettingsRPC.INVALID || rpc_type >= ExpansionSettingsRPC.COUNT )
			return false;
		
		switch ( rpc_type )
		{
			case ExpansionSettingsRPC.NameTags:
			{
				Expansion_Assert_False( m_SettingsNameTags.OnRecieve( ctx ), "Failed reading Name Tags settings" );
				#ifdef EXPANSIONEXPRINT
				EXPrint("ExpansionSettings::OnRPC RPC_NameTags");
				#endif

				return true;
			}
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("[NAME TAGS] ExpansionSettings::OnRPC - End");
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
		EXPrint("[NAME TAGS] ExpansionSettings::Save - Start");
		#endif

		super.Save();

		if ( IsMissionHost() && GetGame().IsMultiplayer() )
		{
			m_SettingsNameTags.Save();
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("[NAME TAGS] ExpansionSettings::Save - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionNameTagsSettings GetNameTags
	// ------------------------------------------------------------
	ExpansionNameTagsSettings GetNameTags()
	{
		return m_SettingsNameTags;
	}
}