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
	static ref ScriptInvoker SI_Book = new ScriptInvoker();
	protected autoptr ExpansionBookSettings m_SettingsBook;
		
	// ------------------------------------------------------------
	// Expansion OnServerInit
	// ------------------------------------------------------------
	override protected void OnServerInit()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("[BOOK] ExpansionSettings::OnServerInit - Start");
		#endif
		
		LoadSetting( m_SettingsBook );
		
		m_NetworkedSettings.Insert( "expansionbooksettings" );
		
		super.OnServerInit();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("[BOOK] ExpansionSettings::OnServerInit - End");
		#endif
	}

	// ------------------------------------------------------------
	override void Unload()
	{
		super.Unload();

		m_SettingsBook.Unload();
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
		
		if ( !IsSettingLoaded( m_SettingsBook, m_SettingsLoaded ) )
			return;

		super.CheckSettingsLoaded();
	}
	
	// ------------------------------------------------------------
	// Override Init
	// ------------------------------------------------------------
	override void Init()
	{
		m_SettingsBook = new ExpansionBookSettings;

		super.Init();
	}
	
	// ------------------------------------------------------------
	// Expansion Send
	// Can only be called on the server.
	// ------------------------------------------------------------
	override void Send( notnull PlayerIdentity identity )
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "[BOOK] ExpansionSettings::SendSettings - Start identity : " + identity );
		#endif

		if ( IsMissionClient() )
			return;

		super.Send( identity );

		m_SettingsBook.Send( identity );

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("[BOOK] ExpansionSettings::SendSettings - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// OnRPC
	// ------------------------------------------------------------
	override bool OnRPC( PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("[BOOK] ExpansionSettings::OnRPC - Start");
		#endif

		if (super.OnRPC(sender, target, rpc_type, ctx))
			return true;
		
		if ( rpc_type <= ExpansionSettingsRPC.INVALID || rpc_type >= ExpansionSettingsRPC.COUNT )
			return false;
		
		switch ( rpc_type )
		{
			case ExpansionSettingsRPC.Book:
			{
				Expansion_Assert_False( m_SettingsBook.OnRecieve( ctx ), "Failed reading Book settings" );
				#ifdef EXPANSIONEXPRINT
				EXPrint("ExpansionSettings::OnRPC RPC_Book");
				#endif

				return true;
			}
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("[BOOK] ExpansionSettings::OnRPC - End");
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
		EXPrint("[BOOK] ExpansionSettings::Save - Start");
		#endif

		super.Save();

		if ( IsMissionHost() && GetGame().IsMultiplayer() )
		{
			m_SettingsBook.Save();
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("[BOOK] ExpansionSettings::Save - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionBookSettings GetBook
	// ------------------------------------------------------------
	ExpansionBookSettings GetBook()
	{
		return m_SettingsBook;
	}
}