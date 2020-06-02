/**
 * ExpansionPartySettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionPartySettings
 * @brief		Party settings class
 **/
class ExpansionPartySettings: ExpansionSettingBase
{
	bool EnableParties;
    int MaxInParty; // If <= 0, unlimited party size
	bool ShowPartyMembers3DMarkers;
	bool UseWholeMapForInviteList; // Use it if you want whole map available in invite list, instead only nearby players

	[NonSerialized()]
	private bool m_IsLoaded;

    // ------------------------------------------------------------
	// Expansion ExpansionPartySettings
	// ------------------------------------------------------------
	void ExpansionPartySettings()
	{
	}
	
	override void HandleRPC( ref ParamsReadContext ctx )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionPartySettings::HandleRPC - Start");
		#endif
		
		ExpansionPartySettings setting;
		if ( !ctx.Read( setting ) )
		{
			Error("ExpansionPartySettings::HandleRPC setting");
			return;
		}

		CopyInternal( setting );

		m_IsLoaded = true;

		ExpansionSettings.SI_Party.Invoke();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionPartySettings::HandleRPC - End");
		#endif
    }

	override int Send( PlayerIdentity identity )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionPartySettings::Send - Start");
		#endif
		
		if ( !IsMissionHost() )
		{
			return 0;
		}
		
		ref ExpansionPartySettings thisSetting = this;
		
		ScriptRPC rpc = new ScriptRPC;
		rpc.Write( thisSetting );
		rpc.Send( null, ExpansionSettingsRPC.Party, true, identity );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionPartySettings::Send - End and return");
		#endif
		return 0;
	}

	// ------------------------------------------------------------
	// Expansion Send
	// ------------------------------------------------------------
    override bool Copy( ExpansionSettingBase setting )
    {
		ExpansionPartySettings s;
		if ( !Class.CastTo( s, setting ) )
			return false;

		CopyInternal( s );
		return true;
	}

	private void CopyInternal( ref ExpansionPartySettings s )
	{
		EnableParties = s.EnableParties;
		MaxInParty = s.MaxInParty;
		ShowPartyMembers3DMarkers = s.ShowPartyMembers3DMarkers;
		UseWholeMapForInviteList = s.UseWholeMapForInviteList;
    }
	
	// ------------------------------------------------------------
	override bool IsLoaded()
	{
		return m_IsLoaded;
	}

	// ------------------------------------------------------------
	override void Unload()
	{
		m_IsLoaded = false;
	}

	override bool OnLoad()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionPartySettings::Load - Start");
		#endif

		m_IsLoaded = true;

		if ( FileExist( EXPANSION_PARTY_SETTINGS ) )
		{
			JsonFileLoader<ExpansionPartySettings>.JsonLoadFile( EXPANSION_PARTY_SETTINGS, this );

			#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionPartySettings::Load - End");
		#endif
			return true;
		}

		Defaults();
		Save();

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionPartySettings::Load - End");
		#endif
		return false;
	}

	// ------------------------------------------------------------
	// Expansion Save
	// ------------------------------------------------------------
	override bool OnSave()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("[ExpansionPartySettings] Saving settings");
		#endif

		JsonFileLoader<ExpansionPartySettings>.JsonSaveFile( EXPANSION_PARTY_SETTINGS, this );

		return true;
	}

	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
    override void Defaults()
	{
        #ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("[ExpansionPartySettings] Loading default settings");
		#endif
		
		EnableParties = true;
		MaxInParty = 10;
        ShowPartyMembers3DMarkers = true;
		UseWholeMapForInviteList = false;
	}
}