/**
 * ExpansionTerritorySettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionTerritorySettingsBase
 * @brief		Territory settings base class
 **/
class ExpansionTerritorySettingsBase: ExpansionSettingBase
{
	bool EnableTerritories;					//! If enabled, use the expansion territory system
	bool UseWholeMapForInviteList; 		//! Use it if you want whole map available in invite list, instead only nearby players.
	float TerritorySize;						//! The radius of a territory in meters.
	float TerritoryPerimeterSize			//! The radius who prevent territories to overlap
	int MaxMembersInTerritory; 			//! If <= 0, unlimited territory size.
	int MaxTerritoryPerPlayer;				//! If <= 0, unlimited territory number.
}

/**@class		ExpansionTerritorySettingsV2
 * @brief		Territory settings v2 class
 **/
class ExpansionTerritorySettingsV2: ExpansionTerritorySettingsBase
{
	float TerritoryAuthenticationRadius;	//! Players need to be in this radius to be able to accept a territory invite
}

/**@class		ExpansionTerritorySettings
 * @brief		Territory settings class
 **/
class ExpansionTerritorySettings: ExpansionTerritorySettingsBase
{
	static const int VERSION = 5;
	
	float TerritoryInviteAcceptRadius;	//! Players need to be in this radius to be able to accept a territory invite
	bool AuthenticateCodeLockIfTerritoryMember;   //! Territory members don't have to enter code on codelocks in territory

	//! Added with version 4
	int InviteCooldown;
	
	//! Added with version 5
	bool OnlyInviteGroupMember;

	[NonSerialized()]
	private bool m_IsLoaded;
	
	// ------------------------------------------------------------
	override bool OnRecieve( ParamsReadContext ctx )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "OnRecieve").Add(ctx);
#endif

		ExpansionTerritorySettings setting;
		if ( !ctx.Read( setting ) )
		{
			Error("ExpansionTerritorySettings::OnRecieve setting");
			return false;
		}

		CopyInternal( setting );
		
		m_IsLoaded = true;
		
		EXLogPrint("Received territory settings");

		ExpansionSettings.SI_Territory.Invoke();

		return true;
	}
	
	// ------------------------------------------------------------
	override void OnSend( ParamsWriteContext ctx )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "OnSend").Add(ctx);
#endif
		
		ExpansionTerritorySettings thisSetting = this;

		ctx.Write( thisSetting );
	}

	// ------------------------------------------------------------
	override int Send( PlayerIdentity identity )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "Send").Add(identity);
#endif
		if ( !IsMissionHost() )
		{
			return 0;
		}
		
		auto rpc = CreateRPC();
		OnSend( rpc );
		rpc.Expansion_Send(true, identity);
		
		return 0;
	}

	// ------------------------------------------------------------
	override bool Copy( ExpansionSettingBase setting )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "Copy").Add(setting);
#endif

		ExpansionTerritorySettings s;
		if ( !Class.CastTo( s, setting ) )
			return false;

		CopyInternal( s );
		
		return true;
	}

	// ------------------------------------------------------------
	private void CopyInternal(  ExpansionTerritorySettings s )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "CopyInternal").Add(s);
#endif

		TerritoryInviteAcceptRadius = s.TerritoryInviteAcceptRadius;
		AuthenticateCodeLockIfTerritoryMember = s.AuthenticateCodeLockIfTerritoryMember;
		InviteCooldown = s.InviteCooldown;
		OnlyInviteGroupMember = s.OnlyInviteGroupMember;
		
		ExpansionTerritorySettingsBase sb = s;
		CopyInternal( sb );
	}
	
	// ------------------------------------------------------------
	private void CopyInternal(  ExpansionTerritorySettingsBase s )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "CopyInternal").Add(s);
#endif

		EnableTerritories = s.EnableTerritories;
		UseWholeMapForInviteList = s.UseWholeMapForInviteList;
		TerritorySize = s.TerritorySize;
		TerritoryPerimeterSize = s.TerritoryPerimeterSize;
		MaxMembersInTerritory = s.MaxMembersInTerritory;
		MaxTerritoryPerPlayer = s.MaxTerritoryPerPlayer;
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

		bool territorySettingsExist = FileExist(EXPANSION_TERRITORY_SETTINGS);

		if (territorySettingsExist)
		{
			EXPrint("[ExpansionTerritorySettings] Load existing setting file:" + EXPANSION_TERRITORY_SETTINGS);

			JsonFileLoader<ExpansionTerritorySettings>.JsonLoadFile(EXPANSION_TERRITORY_SETTINGS, this);

			if (m_Version < VERSION)
			{
				EXPrint("[ExpansionTerritorySettings] Load - Converting v" + m_Version + " \"" + EXPANSION_TERRITORY_SETTINGS + "\" to v" + VERSION);
			
				ExpansionTerritorySettings settingsDefault = new ExpansionTerritorySettings;
				settingsDefault.Defaults();

				if (m_Version < 2)
				{
					TerritoryInviteAcceptRadius = settingsDefault.TerritoryInviteAcceptRadius;
				}
				else if (m_Version == 2)
				{
					ExpansionTerritorySettingsV2 settingsV2;
					JsonFileLoader<ExpansionTerritorySettingsV2>.JsonLoadFile(EXPANSION_TERRITORY_SETTINGS, settingsV2);

					TerritoryInviteAcceptRadius = settingsV2.TerritoryAuthenticationRadius;
				}

				if (m_Version < 4)
					InviteCooldown = settingsDefault.InviteCooldown;

				if (m_Version < 5)
					OnlyInviteGroupMember = settingsDefault.OnlyInviteGroupMember;

				m_Version = VERSION;
				save = true;
			}
		}
		else
		{
			EXPrint("[ExpansionTerritorySettings] No existing setting file:" + EXPANSION_TERRITORY_SETTINGS + ". Creating defaults!");
			Defaults();
			save = true;
		}
		
		if (save)
			Save();
				
		return territorySettingsExist;
	}

	// ------------------------------------------------------------
	override bool OnSave()
	{
		Print("[ExpansionTerritorySettings] Saving settings");
		
		JsonFileLoader<ExpansionTerritorySettings>.JsonSaveFile( EXPANSION_TERRITORY_SETTINGS, this );
		return true;
	}

	// ------------------------------------------------------------
	override void Defaults()
	{
		m_Version = VERSION;
		
		EnableTerritories = true;
		UseWholeMapForInviteList = false;
		TerritorySize = 150.0;
		TerritoryPerimeterSize = 150.0;
		MaxMembersInTerritory = 10;
		MaxTerritoryPerPlayer = 1;
		TerritoryInviteAcceptRadius = 150.0;
		AuthenticateCodeLockIfTerritoryMember = false;
		InviteCooldown = 0;
		OnlyInviteGroupMember = false;
	}
	
	// ------------------------------------------------------------
	override string SettingName()
	{
		return "Territory Settings";
	}
};