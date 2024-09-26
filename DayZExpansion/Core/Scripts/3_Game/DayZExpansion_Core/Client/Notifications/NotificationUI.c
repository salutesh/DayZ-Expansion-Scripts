/**
 * ExpansionNotificationUI.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONUI
modded class NotificationUI
{
	void NotificationUI()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.NOTIFICATIONS, this, "NotificationUI");
#endif

		NotificationSystem.GetInstance().m_OnNotificationAdded.Remove( AddNotification );
		NotificationSystem.GetInstance().m_OnNotificationRemoved.Remove( RemoveNotification );
	}
};
#endif
