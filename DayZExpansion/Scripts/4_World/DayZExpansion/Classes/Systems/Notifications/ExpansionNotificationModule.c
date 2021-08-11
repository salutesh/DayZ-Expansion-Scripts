/**
 * ExpansionNotificationModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionNotificationModule
 * @brief		This class handle notification toasts system
 **/
class ExpansionNotificationModule: JMModuleBase
{
	ref array<ref ExpansionNotificationView> m_Notifications;
	ref array<ref NotificationRuntimeData> m_NotificationData;
	ref ExpansionNotificationHUD m_NotificationHUD;
	
	// ------------------------------------------------------------
	void ExpansionNotificationModule()
	{		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNotificationModule::ExpansionNotificationModule - Start");
		#endif
		
		m_Notifications = new array<ref ExpansionNotificationView>;
		m_NotificationData = new array<ref NotificationRuntimeData>;
	
		if (IsMissionClient())
		{		
			NotificationSystem.BindOnAdd(AddNotification);
			NotificationSystem.BindOnRemove(RemoveNotification);
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNotificationModule::ExpansionNotificationModule - End");
		#endif
	}
	
	// ------------------------------------------------------------
	void ~ExpansionNotificationModule()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNotificationModule::~ExpansionNotificationModule - Start");
		#endif
		
		if (IsMissionClient())
		{
			m_NotificationHUD = NULL;
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNotificationModule::~ExpansionNotificationModule - End");
		#endif
	}

	// ------------------------------------------------------------
	int GetNotificationsCount()
	{
		return m_Notifications.Count();
	}

	// ------------------------------------------------------------
	void AddNotification(NotificationRuntimeData data)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNotificationModule::AddNotification - Start");
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
				default:
					notificationElement = new ExpansionNotificationViewToast(data, this, m_NotificationHUD);
					break;
			}
			
			AddNotificationElement(notificationElement, data.GetType());
			m_Notifications.InsertAt(notificationElement, 0);
			
			m_NotificationData.InsertAt(data, 0);
			m_Notifications[0].ShowNotification();
		}
			
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNotificationModule::AddNotification - End");
		#endif
	}
	
	// ------------------------------------------------------------
	void AddNotificationElement(ExpansionNotificationView notificationElement, ExpansionNotificationType type)
	{
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
			default:
				m_NotificationHUD.AddNotificationToatsElemement(ExpansionNotificationViewToast.Cast(notificationElement));
				break;
		}
	}

	// ------------------------------------------------------------
	void RemoveNotification(NotificationRuntimeData data)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNotificationModule::RemoveNotification - Start");
		#endif
		
		if (!data)
			return;

		int index = m_NotificationData.Find(data);

		if (index >= 0)
			HideNotification( index );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNotificationModule::RemoveNotification - End");
		#endif
	}

	// ------------------------------------------------------------
	void HideNotification(int index)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNotificationModule::HideNotification - Start");
		#endif
		
		if (GetGame().IsMultiplayer() && GetGame().IsServer())
			return;

		if (index < 0)
			return;
			
		if (index >= m_Notifications.Count())
			return;

		m_Notifications[index].HideNotification();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNotificationModule::HideNotification - End");
		#endif
	}

	// ------------------------------------------------------------
	// DEPRECATED
	// ------------------------------------------------------------
	void RemovingNotification(ExpansionNotificationView notif)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNotificationModule::RemovingNotification - Start");
		#endif
		
		if (!notif)
			return;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNotificationModule::RemovingNotification - End");
		#endif
	}
	
	// ------------------------------------------------------------
	void RemoveNotification(ExpansionNotificationView notif)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNotificationModule::RemoveNotification - Start");
		#endif
		
		if (!notif)
			return;

		int index = m_Notifications.Find(notif);
		
		if (index > -1)
		{
			m_Notifications.RemoveOrdered(index);
			m_NotificationData.RemoveOrdered(index);
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNotificationModule::RemoveNotification - End");
		#endif
	}
	
	// ------------------------------------------------------------
	override void OnUpdate(float timeslice)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNotificationModule::OnUpdate - Start");
		#endif
		
		super.OnUpdate(timeslice);

		if (!IsMissionClient())
			return;

		for (int i = 0; i < m_Notifications.Count(); i++)
		{
			if (!m_Notifications[i])
				continue;

			m_Notifications[i].Update(timeslice);
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNotificationModule::OnUpdate - End");
		#endif
	}
	
	// ------------------------------------------------------------
	void SetExpansionNotificationHUD(ExpansionNotificationHUD notificationHUD)
	{
		m_NotificationHUD = notificationHUD;
	}
	
	// ------------------------------------------------------------
	override void OnMissionLoaded()
	{
		if (IsMissionClient())
		{
			if (m_NotificationHUD)
				delete m_NotificationHUD;
			
			m_NotificationHUD = new ExpansionNotificationHUD;
		}
	}
		
	// ------------------------------------------------------------
	override void OnClientReconnect(PlayerBase player, PlayerIdentity identity)
	{
		if (IsMissionClient())
		{
			if (m_NotificationHUD)
				delete m_NotificationHUD;
			
			m_NotificationHUD = new ExpansionNotificationHUD;
		}
	}
	
	// ------------------------------------------------------------
	override void OnInvokeConnect( PlayerBase player, PlayerIdentity identity )
	{
		if (IsMissionClient())
		{
			if (m_NotificationHUD)
				delete m_NotificationHUD;
			
			m_NotificationHUD = new ExpansionNotificationHUD;
		}
	}
}