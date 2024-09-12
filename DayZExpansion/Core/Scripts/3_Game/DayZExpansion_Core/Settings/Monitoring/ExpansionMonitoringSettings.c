/**
 * ExpansionMonitoringSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionMonitoringSettings
 * @brief		Vehicle settings class
 **/
class ExpansionMonitoringSettings: ExpansionSettingBase
{
	static const int VERSION = 1;
	
	bool Enabled;
	
	[NonSerialized()]
	private bool m_IsLoaded;

	// ------------------------------------------------------------
	void ExpansionMonitoringSettings()
	{
	}
	
	// ------------------------------------------------------------
	override bool OnRecieve( ParamsReadContext ctx )
	{
		if (!ctx.Read(Enabled))
		{
			Error(ClassName() + "::OnRecieve Enabled");
			return false;
		}
		
		m_IsLoaded = true;

		return true;
	}
	
	override void OnSend( ParamsWriteContext ctx )
	{
		ctx.Write(Enabled);
	}
	
	// ------------------------------------------------------------
	override int Send( PlayerIdentity identity )
	{
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
		ExpansionMonitoringSettings s;
		if ( !Class.CastTo( s, setting ) )
			return false;

		CopyInternal( s );
		return true;
	}

	// ------------------------------------------------------------
	private void CopyInternal( ExpansionMonitoringSettings s )
	{
		Enabled = s.Enabled;
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
		m_IsLoaded = true;
		
		bool save;
		
		bool settingsExist = FileExist(EXPANSION_MONITORING_SETTINGS);
		
		if (settingsExist)
		{
			EXPrint("[ExpansionMonitoringSettings] Load existing setting file:" + EXPANSION_MONITORING_SETTINGS);

			JsonFileLoader<ExpansionMonitoringSettings>.JsonLoadFile( EXPANSION_MONITORING_SETTINGS, this );
		}
		else
		{
			EXPrint("[ExpansionMonitoringSettings] No existing setting file:" + EXPANSION_MONITORING_SETTINGS + ". Creating defaults!");
			Defaults();
			save = true;
		}
		
		if (save)
			Save();

		return settingsExist;
	}

	// ------------------------------------------------------------
	override bool OnSave()
	{
		Print("[ExpansionMonitoringSettings] Saving settings");

		JsonFileLoader<ExpansionMonitoringSettings>.JsonSaveFile( EXPANSION_MONITORING_SETTINGS, this );

		return true;
	}
	
	// ------------------------------------------------------------
	override void Update( ExpansionSettingBase setting )
	{
		super.Update( setting );
	}

	// ------------------------------------------------------------
	override void Defaults()
	{
		m_Version = VERSION;

#ifdef EXPANSIONMOD
		Enabled = true;
#endif

#ifdef EXPANSIONMODBOOK
		Enabled = true;
#endif

#ifdef EXPANSIONMODGROUPS
		Enabled = true;
#endif
	}
	
	override string SettingName()
	{
		return "Monitoring Settings";
	}
};