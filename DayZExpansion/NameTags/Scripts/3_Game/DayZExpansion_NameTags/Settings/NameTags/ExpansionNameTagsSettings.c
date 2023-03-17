/**
 * ExpansionNameTagsSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionNameTagsSettingsBase
 * @brief		Name tags settings base class
 **/
class ExpansionNameTagsSettingsBase: ExpansionSettingBase
{
	bool EnablePlayerTags;
	int PlayerTagViewRange;
	string PlayerTagsIcon;
}

class ExpansionNameTagsSettingsV0: ExpansionNameTagsSettingsBase
{
	bool ShowPlayerTagsInSafeZones;
	bool ShowPlayerTagsInTerritories;
}

/**@class		ExpansionNameTagsSettings
 * @brief		Name tags settings class
 **/
class ExpansionNameTagsSettings: ExpansionNameTagsSettingsBase
{
	static const int VERSION = 4;

	int PlayerTagsColor;
	int PlayerNameColor;

	bool OnlyInSafeZones;
	bool OnlyInTerritories;
	
	bool ShowPlayerItemInHands;
	bool ShowNPCTags;
#ifdef EXPANSIONMODAI
	bool ShowPlayerFaction;
#endif
#ifdef EXPANSIONMODHARDLINE
	bool UseRarityColorForItemInHands;
#endif
	
	[NonSerialized()]
	private bool m_IsLoaded;

	// ------------------------------------------------------------
	// ExpansionNameTagsSettings OnRecieve
	// ------------------------------------------------------------
	override bool OnRecieve( ParamsReadContext ctx )
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "OnRecieve").Add(ctx);
	#endif

		if ( !ctx.Read( EnablePlayerTags ) )
		{
			Error("ExpansionNameTagsSettings::OnRecieve EnablePlayerTags");
			return false;
		}
		
		if ( !ctx.Read( PlayerTagViewRange ) )
		{
			Error("ExpansionNameTagsSettings::OnRecieve PlayerTagViewRange");
			return false;
		}
		
		if ( !ctx.Read( PlayerTagsIcon ) )
		{
			Error("ExpansionNameTagsSettings::OnRecieve PlayerTagsIcon");
			return false;
		}
		
		if ( !ctx.Read( PlayerTagsColor ) )
		{
			Error("ExpansionNameTagsSettings::OnRecieve PlayerTagsColor");
			return false;
		}
		
		if ( !ctx.Read( PlayerNameColor ) )
		{
			Error("ExpansionNameTagsSettings::OnRecieve PlayerNameColor");
			return false;
		}
		
		if ( !ctx.Read( OnlyInSafeZones ) )
		{
			Error("ExpansionNameTagsSettings::OnRecieve OnlyInSafeZones");
			return false;
		}
		
		if ( !ctx.Read( OnlyInTerritories ) )
		{
			Error("ExpansionNameTagsSettings::OnRecieve OnlyInTerritories");
			return false;
		}
		
		if ( !ctx.Read( ShowPlayerItemInHands ) )
		{
			Error("ExpansionNameTagsSettings::OnRecieve ShowPlayerFaction");
			return false;
		}
		
		if ( !ctx.Read( ShowNPCTags ) )
		{
			Error("ExpansionNameTagsSettings::OnRecieve ShowNPCTags");
			return false;
		}
		
	#ifdef EXPANSIONMODAI
		if ( !ctx.Read( ShowPlayerFaction ) )
		{
			Error("ExpansionNameTagsSettings::OnRecieve ShowPlayerFaction");
			return false;
		}
	#endif
		
	#ifdef EXPANSIONMODHARDLINE
		if ( !ctx.Read( UseRarityColorForItemInHands ) )
		{
			Error("ExpansionNameTagsSettings::OnRecieve UseRarityColorForItemInHands");
			return false;
		}
	#endif

		m_IsLoaded = true;
		
		EXLogPrint("Received Name-Tag settings");

		ExpansionSettings.SI_NameTags.Invoke();
	
		return true;
	}
	
	// ------------------------------------------------------------
	// ExpansionNameTagsSettings OnSend
	// ------------------------------------------------------------
	override void OnSend( ParamsWriteContext ctx )
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "OnSend").Add(ctx);
	#endif

		ctx.Write( EnablePlayerTags );
		ctx.Write( PlayerTagViewRange );
		ctx.Write( PlayerTagsIcon );
		ctx.Write( PlayerTagsColor );
		ctx.Write( PlayerNameColor );
		ctx.Write( OnlyInSafeZones );
		ctx.Write( OnlyInTerritories );
		ctx.Write( ShowPlayerItemInHands );
		ctx.Write( ShowNPCTags );
	#ifdef EXPANSIONMODAI
		ctx.Write( ShowPlayerFaction );
	#endif
		
	#ifdef EXPANSIONMODHARDLINE
		ctx.Write( UseRarityColorForItemInHands );
	#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionNameTagsSettings Send
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
		
		auto rpc = ExpansionScriptRPC.Create();
		OnSend( rpc );
		rpc.Send( null, ExpansionSettingsRPC.NameTags, true, identity );
		
		return 0;
	}

	// ------------------------------------------------------------
	// ExpansionNameTagsSettings Copy
	// ------------------------------------------------------------
	override bool Copy( ExpansionSettingBase setting )
	{
		ExpansionNameTagsSettings s;
		if ( !Class.CastTo( s, setting ) )
			return false;

		CopyInternal( s );
		return true;
	}

	// ------------------------------------------------------------
	private void CopyInternal( ExpansionNameTagsSettings s )
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "CopyInternal").Add(s);
	#endif

		PlayerTagsColor = s.PlayerTagsColor;
		PlayerNameColor = s.PlayerNameColor;

		OnlyInSafeZones = s.OnlyInSafeZones;
		OnlyInTerritories = s.OnlyInTerritories;
	
		ShowPlayerItemInHands = s.ShowPlayerItemInHands;
		ShowNPCTags = s.ShowNPCTags;
	#ifdef EXPANSIONMODAI
		ShowPlayerFaction = s.ShowPlayerFaction;
	#endif
	#ifdef EXPANSIONMODHARDLINE
		UseRarityColorForItemInHands = s.UseRarityColorForItemInHands;
	#endif

		ExpansionNameTagsSettingsBase sb = s;
		CopyInternal( sb );
	}
	
	// ------------------------------------------------------------
	// ExpansionBookSettings CopyInternal
	// ------------------------------------------------------------
	private void CopyInternal( ExpansionNameTagsSettingsBase s )
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "CopyInternal").Add(s);
	#endif

		EnablePlayerTags = s.EnablePlayerTags;

		PlayerTagViewRange = s.PlayerTagViewRange;
		PlayerTagsIcon = s.PlayerTagsIcon;
	}
	
	// ------------------------------------------------------------
	// ExpansionBookSettings IsLoaded
	// ------------------------------------------------------------
	override bool IsLoaded()
	{
		return m_IsLoaded;
	}
	
	// ------------------------------------------------------------
	// ExpansionBookSettings Unload
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
		bool nameTagsSettingsExist = FileExist(EXPANSION_NAMETAGS_SETTINGS);
		if (nameTagsSettingsExist)
		{
			CF_Log.Info(ToString() + "::OnLoad - Load existing setting file %1", EXPANSION_NAMETAGS_SETTINGS);
			
			ExpansionNameTagsSettings settingsDefault = new ExpansionNameTagsSettings;
			settingsDefault.Defaults();

			ExpansionNameTagsSettingsBase settingsBase;
			JsonFileLoader<ExpansionNameTagsSettingsBase>.JsonLoadFile(EXPANSION_NAMETAGS_SETTINGS, settingsBase);

			if (settingsBase.m_Version < VERSION)
			{
				CF_Log.Info(ToString() + "::OnLoad - Converting v %1 %2 to v %3", settingsBase.m_Version.ToString(), EXPANSION_NAMETAGS_SETTINGS, VERSION.ToString());

				if (settingsBase.m_Version < 1)
				{
					CopyInternal(settingsBase);

					PlayerTagsColor = settingsDefault.PlayerTagsColor;
					PlayerNameColor = settingsDefault.PlayerNameColor;

					ExpansionNameTagsSettingsV0 settingsV0;
					JsonFileLoader<ExpansionNameTagsSettingsV0>.JsonLoadFile(EXPANSION_NAMETAGS_SETTINGS, settingsV0);
					OnlyInSafeZones = settingsV0.ShowPlayerTagsInSafeZones;
					OnlyInTerritories = settingsV0.ShowPlayerTagsInTerritories;
				}
				else
				{
					JsonFileLoader<ExpansionNameTagsSettings>.JsonLoadFile(EXPANSION_NAMETAGS_SETTINGS, this);
				}

				m_Version = VERSION;
				save = true;
			}
			else
			{
				JsonFileLoader<ExpansionNameTagsSettings>.JsonLoadFile(EXPANSION_NAMETAGS_SETTINGS, this);
			}
		}
		else
		{
			CF_Log.Info(ToString() + "::OnLoad - No existing setting file %1. Creating defaults!", EXPANSION_NAMETAGS_SETTINGS);
			Defaults();
			save = true;
		}
		
		if (save)
			Save();
		
		return nameTagsSettingsExist;
	}

	// ------------------------------------------------------------
	// ExpansionNameTagsSettings OnSave
	// ------------------------------------------------------------
	override bool OnSave()
	{
		CF_Log.Info(ToString() + "::OnSave - Saving settings %1", EXPANSION_NAMETAGS_SETTINGS);
		JsonFileLoader<ExpansionNameTagsSettings>.JsonSaveFile( EXPANSION_NAMETAGS_SETTINGS, this );

		return true;
	}
	
	// ------------------------------------------------------------
	// ExpansionNameTagsSettings Update
	// ------------------------------------------------------------
	override void Update( ExpansionSettingBase setting )
	{
		super.Update( setting );

		ExpansionSettings.SI_NameTags.Invoke();
	}

	// ------------------------------------------------------------
	// ExpansionNameTagsSettings Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		m_Version = VERSION;	
		
		EnablePlayerTags = true;
		PlayerTagViewRange = 5;
		PlayerTagsIcon = EXPANSION_NOTIFICATION_ICON_PERSONA;
		PlayerTagsColor = -1;
		PlayerNameColor = -1;
		OnlyInSafeZones = false;
		OnlyInTerritories = false;
		ShowPlayerItemInHands = false;
		ShowNPCTags = false;
	#ifdef EXPANSIONMODAI
		ShowPlayerFaction = false;
	#endif
	#ifdef EXPANSIONMODHARDLINE
		UseRarityColorForItemInHands = false;
	#endif
	}
		
	// ------------------------------------------------------------
	// ExpansionBookSettings SettingName
	// ------------------------------------------------------------
	override string SettingName()
	{
		return "Name Tags Settings";
	}
};