/**
 * ExpansionChatSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionChatSettingsBase
 * @brief		Chat settings base class
 **/
class ExpansionChatSettingsBase: ExpansionSettingBase
{
	bool EnableGlobalChat;
#ifdef EXPANSIONMODGROUPS
	bool EnablePartyChat;
#endif
	bool EnableTransportChat;
}


/**@class		ExpansionChatSettingsV1
 * @brief		Chat settings v2 class
 **/
class ExpansionChatSettingsV1: ExpansionChatSettingsBase
{
	int SystemChatColor;
	int AdminChatColor;
	int GlobalChatColor;
	int DirectChatColor;
	int TransportChatColor;
#ifdef EXPANSIONMODGROUPS
	int PartyChatColor;
#endif
	int TransmitterChatColor;
}

/**@class		ExpansionChatSettings
 * @brief		Chat settings class
 **/
class ExpansionChatSettings: ExpansionChatSettingsBase
{
	static const int VERSION = 2;
	
	ref ExpansionChatColors ChatColors;
	
	[NonSerialized()]
	private bool m_IsLoaded;
	
	void ExpansionChatSettings()
	{
		ChatColors = new ExpansionChatColors;
	}

	// ------------------------------------------------------------
	override bool OnRecieve( ParamsReadContext ctx )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "OnRecieve").Add(ctx);
#endif

		ctx.Read(EnableGlobalChat);
#ifdef EXPANSIONMODGROUPS
		ctx.Read(EnablePartyChat);
#endif
		ctx.Read(EnableTransportChat);

		ChatColors.OnReceive(ctx);

		ChatColors.Update();

		m_IsLoaded = true;

		ExpansionSettings.SI_Chat.Invoke();

		return true;
	}
	
	override void OnSend( ParamsWriteContext ctx )
	{
		ctx.Write(EnableGlobalChat);
#ifdef EXPANSIONMODGROUPS
		ctx.Write(EnablePartyChat);
#endif
		ctx.Write(EnableTransportChat);

		ChatColors.OnSend(ctx);
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
		
		ScriptRPC rpc = new ScriptRPC;
		OnSend( rpc );
		rpc.Send( null, ExpansionSettingsRPC.Chat, true, identity );
		
		return 0;
	}

	// ------------------------------------------------------------
	override bool Copy( ExpansionSettingBase setting )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "Copy").Add(setting);
#endif

		ExpansionChatSettings settings = ExpansionChatSettings.Cast( setting );
		if ( !settings )
			return false;

		CopyInternal( settings );
		
		return true;
	}
	
	// ------------------------------------------------------------
	private void CopyInternal(  ExpansionChatSettings s )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "CopyInternal").Add(s);
#endif

		ChatColors = s.ChatColors;

		ExpansionChatSettingsBase sb = s;
		CopyInternal( sb );
	}

	// ------------------------------------------------------------
	private void CopyInternal(  ExpansionChatSettingsBase s )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "CopyInternal").Add(s);
#endif

		EnableGlobalChat = s.EnableGlobalChat;
#ifdef EXPANSIONMODGROUPS
		EnablePartyChat = s.EnablePartyChat;
#endif
		EnableTransportChat = s.EnableTransportChat;
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

		bool chatSettingsExist = FileExist(EXPANSION_CHAT_SETTINGS);

		if (chatSettingsExist)
		{
			EXPrint("[ExpansionChatSettings] Load existing setting file:" + EXPANSION_CHAT_SETTINGS);
			
			ExpansionChatSettings settingsDefault = new ExpansionChatSettings;
			settingsDefault.Defaults();

			ExpansionChatSettingsBase settingsBase;

			JsonFileLoader<ExpansionChatSettingsBase>.JsonLoadFile(EXPANSION_CHAT_SETTINGS, settingsBase);

			if (settingsBase.m_Version < VERSION)
			{
				EXPrint("[ExpansionChatSettings] Load - Converting v" + settingsBase.m_Version + " \"" + EXPANSION_CHAT_SETTINGS + "\" to v" + VERSION);

				if (settingsBase.m_Version < 2)
				{
					ExpansionChatSettingsV1 settings_v1;

					JsonFileLoader<ExpansionChatSettingsV1>.JsonLoadFile(EXPANSION_CHAT_SETTINGS, settings_v1);

					ChatColors.Set("SystemChatColor", settings_v1.SystemChatColor);
					ChatColors.Set("AdminChatColor", settings_v1.AdminChatColor);
					ChatColors.Set("GlobalChatColor", settings_v1.GlobalChatColor);
					ChatColors.Set("DirectChatColor", settings_v1.DirectChatColor);
					ChatColors.Set("TransportChatColor", settings_v1.TransportChatColor);
#ifdef EXPANSIONMODGROUPS
					ChatColors.Set("PartyChatColor", settings_v1.PartyChatColor);
#endif
					ChatColors.Set("TransmitterChatColor", settings_v1.TransmitterChatColor);
				}
				else
				{
					JsonFileLoader<ExpansionChatSettings>.JsonLoadFile(EXPANSION_CHAT_SETTINGS, this);
				}

				//! Copy over old settings that haven't changed
				CopyInternal(settingsBase);

				m_Version = VERSION;
				save = true;
			}
			else
			{
				JsonFileLoader<ExpansionChatSettings>.JsonLoadFile(EXPANSION_CHAT_SETTINGS, this);
			}

			ChatColors.Update();
		}
		else
		{
			EXPrint("[ExpansionChatSettings] No existing setting file:" + EXPANSION_CHAT_SETTINGS + ". Creating defaults!");
			Defaults();
			save = true;
		}
		
		if (save)
			Save();
		
		return chatSettingsExist;
	}
	
	// ------------------------------------------------------------
	override bool OnSave()
	{
		Print("[ExpansionChatSettings] Saving settings");

		JsonFileLoader<ExpansionChatSettings>.JsonSaveFile( EXPANSION_CHAT_SETTINGS, this );

		return true;
	}
	
	// ------------------------------------------------------------
	override void Update( ExpansionSettingBase setting )
	{
		super.Update( setting );

		ExpansionSettings.SI_Chat.Invoke();
	}

	// ------------------------------------------------------------
	override void Defaults()
	{
		m_Version = VERSION;
				
		EnableGlobalChat = true;
#ifdef EXPANSIONMODGROUPS
		EnablePartyChat = true;
#endif
		EnableTransportChat = true;
		
		ChatColors.Update();
	}
	
	// ------------------------------------------------------------
	override string SettingName()
	{
		return "Chat Settings";
	}
};
