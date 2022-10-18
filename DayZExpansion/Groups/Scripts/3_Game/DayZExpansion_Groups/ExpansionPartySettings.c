/**
 * ExpansionPartySettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
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
	static const int VERSION = 4;

	bool EnableParties;												// enable party module, allow players to create parties
	int MaxMembersInParty; 										// If <= 0, unlimited party size
	bool UseWholeMapForInviteList; 								// Use it if you want whole map available in invite list, instead only nearby players
	bool ShowPartyMember3DMarkers;					// If enabled, allow to see 3D marker above teammates location
	bool ShowDistanceUnderPartyMembersMarkers;		// Show the distance of the party member marker
	bool ShowNameOnPartyMembersMarkers;				// Show the name of the party member marker
	bool EnableQuickMarker;										// Enable/Diable quick marker option
	bool ShowDistanceUnderQuickMarkers;					//  Show the distance of the quick marker
	bool ShowNameOnQuickMarkers;							// Show the distance of the quick marker
	bool CanCreatePartyMarkers;									// Allow player to create party markers
	
	//! Added with version 2
	bool ShowPartyMemberHUD;									// Show the party hud interface that displays eacht party members name and health
	
	//! Added with version 3
	bool ShowHUDMemberBlood;
	bool ShowHUDMemberStates;
	bool ShowHUDMemberStance;
	
	//! Added with version 4
	bool ShowPartyMemberMapMarkers;
	
	[NonSerialized()]
	private bool m_IsLoaded;
	
	override bool OnRecieve( ParamsReadContext ctx )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "OnRecieve").Add(ctx);
#endif

		
		ExpansionPartySettings setting;
		if ( !ctx.Read( setting ) )
		{
			Error("ExpansionPartySettings::OnRecieve setting");
			return false;
		}

		CopyInternal( setting );

		m_IsLoaded = true;

		ExpansionSettings.SI_Party.Invoke();
	
		return true;
	}
	
	override void OnSend( ParamsWriteContext ctx )
	{
		ExpansionPartySettings thisSetting = this;

		ctx.Write( thisSetting );
	}

	override int Send( PlayerIdentity identity )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "Send").Add(identity);
#endif
		
		if ( !IsMissionHost() )
		{
			return 0;
		}
		
		ScriptRPC rpc = new ScriptRPC;
		OnSend( rpc );
		rpc.Send( null, ExpansionSettingsRPC.Party, true, identity );
		
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
	
	// ------------------------------------------------------------
	private void CopyInternal(  ExpansionPartySettings s )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "CopyInternal").Add(s);
#endif
		EnableParties = s.EnableParties;
		MaxMembersInParty = s.MaxMembersInParty;
		UseWholeMapForInviteList = s.UseWholeMapForInviteList;
		ShowPartyMember3DMarkers = s.ShowPartyMember3DMarkers;
		ShowDistanceUnderPartyMembersMarkers = s.ShowDistanceUnderPartyMembersMarkers;
		ShowNameOnPartyMembersMarkers = s.ShowNameOnPartyMembersMarkers;
		EnableQuickMarker = s.EnableQuickMarker;
		ShowDistanceUnderQuickMarkers = s.ShowDistanceUnderQuickMarkers;
		ShowNameOnQuickMarkers = s.ShowNameOnQuickMarkers;
		CanCreatePartyMarkers = s.CanCreatePartyMarkers;
		ShowPartyMemberHUD = s.ShowPartyMemberHUD;
		
		ShowHUDMemberBlood = s.ShowHUDMemberBlood;
		ShowHUDMemberStates = s.ShowHUDMemberStates;
		ShowHUDMemberStance = s.ShowHUDMemberStance;
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

	// ------------------------------------------------------------
	override bool OnLoad()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "OnLoad");
#endif

		m_IsLoaded = true;

		bool save;

		bool partySettingsExist = FileExist(EXPANSION_PARTY_SETTINGS);

		if (partySettingsExist)
		{
			EXPrint("[ExpansionPartySettings] Load existing setting file:" + EXPANSION_PARTY_SETTINGS);
			
			ExpansionPartySettings settingsDefault = new ExpansionPartySettings;
			settingsDefault.Defaults();

			JsonFileLoader<ExpansionPartySettings>.JsonLoadFile(EXPANSION_PARTY_SETTINGS, this);

			if (m_Version < VERSION)
			{
				EXPrint("[ExpansionPartySettings] Load - Converting v" + m_Version + " \"" + EXPANSION_PARTY_SETTINGS + "\" to v" + VERSION);
				
				if (m_Version < 2)
				{
					ShowPartyMemberHUD = settingsDefault.ShowPartyMemberHUD;
				}
				
				if (m_Version < 3)
				{
					ShowHUDMemberBlood = settingsDefault.ShowHUDMemberBlood;
					ShowHUDMemberStates = settingsDefault.ShowHUDMemberStates;
					ShowHUDMemberStance = settingsDefault.ShowHUDMemberStance;
				}
				
				if (m_Version < 4)
				{
					ShowPartyMemberMapMarkers = settingsDefault.ShowPartyMemberMapMarkers;
				}

				m_Version = VERSION;
				save = true;
			}
		}
		else
		{
			EXPrint("[ExpansionPartySettings] No existing setting file:" + EXPANSION_PARTY_SETTINGS + ". Creating defaults!");
			Defaults();
			save = true;
		}
		
		if (save)
			Save();
		
		return partySettingsExist;
	}

	// ------------------------------------------------------------
	// Expansion Save
	// ------------------------------------------------------------
	override bool OnSave()
	{
		Print("[ExpansionPartySettings] Saving settings");

		JsonFileLoader<ExpansionPartySettings>.JsonSaveFile( EXPANSION_PARTY_SETTINGS, this );

		return true;
	}

	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		m_Version = VERSION;
		
		//! Added with version 1
		EnableParties = true;
		MaxMembersInParty = 10;
		UseWholeMapForInviteList = false;
		ShowPartyMember3DMarkers = true;
		ShowPartyMemberMapMarkers = true;
		ShowDistanceUnderPartyMembersMarkers = true;
		ShowNameOnPartyMembersMarkers = true;
		EnableQuickMarker = true;
		ShowDistanceUnderQuickMarkers = true;
		ShowNameOnQuickMarkers = true;
		CanCreatePartyMarkers = true;
		
		//! Added with version 2
		ShowPartyMemberHUD = true;
		
		//! Added with version 3
		ShowHUDMemberBlood = true;
		ShowHUDMemberStates = true;
		ShowHUDMemberStance = true;
	}
	
	// ------------------------------------------------------------
	override string SettingName()
	{
		return "Party Settings";
	}
};