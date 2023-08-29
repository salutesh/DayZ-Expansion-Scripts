/**
 * ExpansionNotificationSchedulerSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionNotificationSchedule
{
	int Hour;
	int Minute;
	int Second;
	string Title;
	string Text;
	string Icon;
	string Color;

	[NonSerialized()]
	int m_LastShownTimestamp;

	[NonSerialized()]
	ref ExpansionNotificationScheduleColor m_Color;

	void ExpansionNotificationSchedule()
	{
		InitColor();
	}

	void InitColor(string color = "")
	{
		m_Color = new ExpansionNotificationScheduleColor();
		if (color)
			m_Color.Set("Color", color);
		else
			m_Color.Update();
	}
}

class ExpansionNotificationScheduleColor: ExpansionColorSettingsBase
{
	string Color;
	override void Update()
	{
		super.Update();

		//! Make sure none of the colors are zero (all transparent) and set defaults
		if (!Get("Color"))
			Set("Color", COLOR_EXPANSION_NOTIFICATION_INFO);
	}
}

class ExpansionNotificationSchedulerSettings: ExpansionSettingBase
{
	bool Enabled;
	bool UTC;
	bool UseMissionTime;
	ref array<ref ExpansionNotificationSchedule> Notifications = new array<ref ExpansionNotificationSchedule>;
	
	static const int VERSION = 2;
	
	[NonSerialized()]
	private bool m_IsLoaded;
	
	override int Send( PlayerIdentity identity )
	{
		//! Not sent to client
	}

	override bool Copy( ExpansionSettingBase setting )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "Copy").Add(setting);
#endif

		ExpansionNotificationSchedulerSettings s;
		if ( !Class.CastTo( s, setting ) )
			return false;

		CopyInternal( s );
		return true;
	}
	
	private void CopyInternal( ExpansionNotificationSchedulerSettings s )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "CopyInternal").Add(s);
#endif
		
		Enabled = s.Enabled;
		UTC = s.UTC;
		UseMissionTime = s.UseMissionTime;
		Notifications = s.Notifications;
	}
	
	override bool IsLoaded()
	{
		return m_IsLoaded;
	}
	
	override void Unload()
	{
		m_IsLoaded = false;
	}
	
	override bool OnLoad()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "OnLoad");
#endif

		m_IsLoaded = true;

		bool save;

		bool settingsExist = FileExist(EXPANSION_NOTIFICATIONSCHEDULER_SETTINGS);

		if (settingsExist)
		{
			//CF_Log.Info("[ExpansionNotificationSchedulerSettings] Load existing setting file:" + EXPANSION_NOTIFICATIONSCHEDULER_SETTINGS);
			JsonFileLoader<ExpansionNotificationSchedulerSettings>.JsonLoadFile(EXPANSION_NOTIFICATIONSCHEDULER_SETTINGS, this);

			foreach (ExpansionNotificationSchedule schedule: Notifications)
			{
				schedule.InitColor(schedule.Color);
			}
			
			if (m_Version < 2)
			{
				UseMissionTime = false;
			}
		}
		else
		{
			//CF_Log.Info("[ExpansionNotificationSchedulerSettings] No existing setting file:" + EXPANSION_NOTIFICATIONSCHEDULER_SETTINGS + ". Creating defaults!");

			Defaults();
			save = true;
		}
		
		if (save)
		{
			Save();
		}

		return settingsExist;
	}

	override bool OnSave()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "OnSave");
#endif

		JsonFileLoader<ExpansionNotificationSchedulerSettings>.JsonSaveFile( EXPANSION_NOTIFICATIONSCHEDULER_SETTINGS, this );

		return true;
	}
	
	override void Update( ExpansionSettingBase setting )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.SETTINGS, this, "Update").Add(setting);
#endif

		super.Update( setting );

		ExpansionSettings.SI_NotificationScheduler.Invoke();
	}

	override void Defaults()
	{
		m_Version = VERSION;
		
		Enabled = false;
		UTC = false;
		UseMissionTime = false;
		
		ExpansionNotificationSchedule schedule = new ExpansionNotificationSchedule();
		schedule.Hour = 22;
		schedule.Minute = 00;
		schedule.Second = 0;
		schedule.Title = "Notification Schedule Test 1";
		schedule.Text = "Lorem ipsum dolor sit amet";
		schedule.Icon = "Info";
		Notifications.Insert(schedule);
		
		schedule = new ExpansionNotificationSchedule();
		schedule.Hour = 22;
		schedule.Minute = 01;
		schedule.Second = 0;
		schedule.Title = "Notification Schedule Test 2";
		schedule.Text = "Lorem ipsum dolor sit amet";
		schedule.Icon = "Info";
		Notifications.Insert(schedule);
	}

	override string SettingName()
	{
		return "Notification Scheduler Settings";
	}
};