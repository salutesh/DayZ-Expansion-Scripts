/**
 * ExpansionNotificationModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionNotificationModule
 * @brief		This class handle notification toasts system
 **/
#ifdef EXPANSIONUI
[CF_RegisterModule(ExpansionNotificationModule)]
class ExpansionNotificationModule: CF_ModuleWorld
{
	ref array<ref ExpansionNotificationView> m_Notifications;
	ref array<ref NotificationRuntimeData> m_NotificationData;
	ref ExpansionNotificationHUD m_NotificationHUD;
	bool m_Expansion_Bind;
	
	// ------------------------------------------------------------
	void ExpansionNotificationModule()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.NOTIFICATIONS, this, "ExpansionNotificationModule");
#endif

		m_Notifications = new array<ref ExpansionNotificationView>;
		m_NotificationData = new array<ref NotificationRuntimeData>;
	}
	
	// ------------------------------------------------------------
	void ~ExpansionNotificationModule()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.NOTIFICATIONS, this, "~ExpansionNotificationModule");
#endif

		if (IsMissionClient())
		{
			m_NotificationHUD = NULL;
		}
	}

	override void OnInit()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.NOTIFICATIONS, this, "OnInit");
#endif

		super.OnInit();

		EnableMissionLoaded();
		EnableMissionStart();
		EnableUpdate();
	}

	override bool IsServer()
	{
		return false;
	}

	// ------------------------------------------------------------
	int GetNotificationsCount()
	{
		return m_Notifications.Count();
	}

	// ------------------------------------------------------------
	void AddNotification(NotificationRuntimeData data)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.NOTIFICATIONS, this, "AddNotification");
#endif

		if (GetExpansionClientSettings() && GetExpansionClientSettings().ShowNotifications)
		{
			if (!data)
				return;
			
			if (!m_NotificationHUD)
				return;
				
			ExpansionNotificationView notificationElement;
			switch (data.GetType())
			{
				case ExpansionNotificationType.TOAST:
					notificationElement = new ExpansionNotificationViewToast(data, this, m_NotificationHUD);
					break;
				case ExpansionNotificationType.BAGUETTE:
					notificationElement = new ExpansionNotificationViewBaguette(data, this, m_NotificationHUD);
					break;
				case ExpansionNotificationType.ACTIVITY:
					notificationElement = new ExpansionNotificationViewActivity(data, this, m_NotificationHUD);
					break;
				case ExpansionNotificationType.KILLFEED:
					notificationElement = new ExpansionNotificationViewKillfeed(data, this, m_NotificationHUD);
					break;
				case ExpansionNotificationType.MARKET:
					notificationElement = new ExpansionNotificationViewMarket(data, this, m_NotificationHUD);
					break;
				case ExpansionNotificationType.GARAGE:
					notificationElement = new ExpansionNotificationViewGarage(data, this, m_NotificationHUD);
					break;
				default:
					notificationElement = new ExpansionNotificationViewToast(data, this, m_NotificationHUD);
					break;
			}
			
			AddNotificationElement(notificationElement, data.GetType());
			m_Notifications.InsertAt(notificationElement, 0);
			
			m_NotificationData.InsertAt(data, 0);
			m_Notifications[0].ShowNotification();
		}
	}
	
	// ------------------------------------------------------------
	void AddNotificationElement(ExpansionNotificationView notificationElement, ExpansionNotificationType type)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.NOTIFICATIONS, this, "AddNotificationElement");
#endif

		switch (type)
		{
			case ExpansionNotificationType.TOAST:
				m_NotificationHUD.AddNotificationToatsElemement(ExpansionNotificationViewToast.Cast(notificationElement));
				break;
			case ExpansionNotificationType.BAGUETTE:
				m_NotificationHUD.AddNotificationBaguetteElemement(ExpansionNotificationViewBaguette.Cast(notificationElement));
				break;
			case ExpansionNotificationType.ACTIVITY:
				m_NotificationHUD.AddNotificationActivityElemement(ExpansionNotificationViewActivity.Cast(notificationElement));
				break;
			case ExpansionNotificationType.KILLFEED:
				m_NotificationHUD.AddNotificationKillfeedElemement(ExpansionNotificationViewKillfeed.Cast(notificationElement));
				break;
			case ExpansionNotificationType.MARKET:
				m_NotificationHUD.AddNotificationMarketElemement(ExpansionNotificationViewMarket.Cast(notificationElement));
				break;
			case ExpansionNotificationType.GARAGE:
				m_NotificationHUD.AddNotificationGarageElemement(ExpansionNotificationViewGarage.Cast(notificationElement));
				break;
			default:
				m_NotificationHUD.AddNotificationToatsElemement(ExpansionNotificationViewToast.Cast(notificationElement));
				break;
		}
	}

	// ------------------------------------------------------------
	void RemoveNotification(NotificationRuntimeData data)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.NOTIFICATIONS, this, "RemoveNotification");
#endif

		if (!data)
			return;

		int index = m_NotificationData.Find(data);

		if (index >= 0)
			HideNotification( index );
	}

	// ------------------------------------------------------------
	void HideNotification(int index)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.NOTIFICATIONS, this, "HideNotification");
#endif

		if (g_Game.IsMultiplayer() && g_Game.IsServer())
			return;

		if (index < 0)
			return;
			
		if (index >= m_Notifications.Count())
			return;

		m_Notifications[index].HideNotification();
	}

	// ------------------------------------------------------------
	// DEPRECATED
	// ------------------------------------------------------------
	void RemovingNotification(ExpansionNotificationView notif)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.NOTIFICATIONS, this, "RemovingNotification");
#endif

		if (!notif)
			return;
	}
	
	// ------------------------------------------------------------
	void RemoveNotificationView(ExpansionNotificationView notif)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.NOTIFICATIONS, this, "RemoveNotificationView");
#endif

		if (!notif)
			return;

		int index = m_Notifications.Find(notif);
		
		if (index > -1)
		{
			m_Notifications.RemoveOrdered(index);
			m_NotificationData.RemoveOrdered(index);
		}
	}
	
	// ------------------------------------------------------------
	override void OnUpdate(Class sender, CF_EventArgs args)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.NOTIFICATIONS, this, "OnUpdate");
#endif

		super.OnUpdate(sender, args);

		auto update = CF_EventUpdateArgs.Cast(args);

		for (int i = 0; i < m_Notifications.Count(); i++)
		{
			if (!m_Notifications[i])
				continue;

			m_Notifications[i].AnimateNotification(update.DeltaTime);
		}
	}
	
	// ------------------------------------------------------------
	void SetExpansionNotificationHUD(ExpansionNotificationHUD notificationHUD)
	{
		m_NotificationHUD = notificationHUD;
	}

	override void OnMissionStart(Class sender, CF_EventArgs args)
	{
		super.OnMissionStart(sender, args);
	
		if (!m_Expansion_Bind)
		{
			m_Expansion_Bind = true;
			NotificationSystem.GetInstance().m_OnNotificationAdded.Insert(AddNotification);
			NotificationSystem.GetInstance().m_OnNotificationRemoved.Insert(RemoveNotification);
		}
	}
	
	// ------------------------------------------------------------
	override void OnMissionLoaded(Class sender, CF_EventArgs args)
	{
		super.OnMissionLoaded(sender, args);

		//! TODO: Why not use existing one?
		if (m_NotificationHUD)
			m_NotificationHUD.Destroy();
		
		m_NotificationHUD = new ExpansionNotificationHUD;
	}
};
#endif
