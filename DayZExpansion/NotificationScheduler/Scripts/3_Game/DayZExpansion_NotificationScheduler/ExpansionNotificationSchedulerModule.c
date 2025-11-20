/**
 * ExpansionNotificationSchedulerModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

[CF_RegisterModule(ExpansionNotificationSchedulerModule)]
class ExpansionNotificationSchedulerModule: CF_ModuleGame
{
	private const float UPDATE_TICK_TIME = 0.5;
	private float m_UpdateTimer = 0;
	private bool m_Enabled;
	
	override void OnInit()
	{
		super.OnInit();

		EnableUpdate();
		EnableSettingsChanged();
	}
	
	override bool IsClient()
	{
		return false;
	}
	
	override void OnSettingsChanged(Class sender, CF_EventArgs args)
	{
		super.OnSettingsChanged(sender, args);

		m_Enabled = GetExpansionSettings().GetNotificationScheduler().Enabled;
	}

	override void OnUpdate(Class sender, CF_EventArgs args)
	{
		super.OnUpdate(sender, args);

		if (!m_Enabled)
			return;

		auto update = CF_EventUpdateArgs.Cast(args);

		m_UpdateTimer += update.DeltaTime;
		if (m_UpdateTimer >= UPDATE_TICK_TIME)
		{
			bool useUTC = GetExpansionSettings().GetNotificationScheduler().UTC;
			bool useMissionTime = GetExpansionSettings().GetNotificationScheduler().UseMissionTime;
			
			CF_Date now;
			if (!useMissionTime)
			{
				now = CF_Date.Now(useUTC);
			}
			else
			{
				int missionTime = g_Game.GetTime();
				int seconds = (missionTime / 1000) % 60;
				int minutes = ((missionTime / (1000*60)) % 60);
				int hours  = ((missionTime / (1000*60*60)) % 24);
				
				now = CF_Date.CreateTime(hours, minutes, seconds);
			}
			
			int timestamp = now.DateToEpoch();
				
			for (int i = 0; i < GetExpansionSettings().GetNotificationScheduler().Notifications.Count(); i++)
			{
				ExpansionNotificationSchedule schedule = GetExpansionSettings().GetNotificationScheduler().Notifications[i];
				if (!schedule || schedule.m_LastShownTimestamp == timestamp)
					continue;
				
				if (schedule.Hour == now.GetHours() && schedule.Minute == now.GetMinutes() && schedule.Second == now.GetSeconds())
				{
					StringLocaliser title = new StringLocaliser(schedule.Title);
					StringLocaliser text = new StringLocaliser(schedule.Text);
					
					ExpansionIcon icon;
					string iconPath;
					if (!Class.CastTo(icon, ExpansionIcons.Get(schedule.Icon)))
						iconPath = schedule.Icon;
					else
						iconPath = icon.IconPath;
					
					ExpansionNotification(title, text, iconPath, schedule.m_Color.Get("Color"), 7, ExpansionNotificationType.BAGUETTE).Create();

					schedule.m_LastShownTimestamp = timestamp;
				}
			}
			
			m_UpdateTimer = 0.0;
		}
	}
}
