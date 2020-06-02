/**
 * ExpansionNotificationModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
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
	protected ref array< ref ExpansionNotificationUI > m_Notifications;
	protected ref array< ref NotificationRuntimeData > m_NotificationData;
	protected ref ExpansionNotificationUIGrid m_NotificationUIGrid;
	
	// ------------------------------------------------------------
	void ExpansionNotificationModule()
	{		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNotificationModule::ExpansionNotificationModule - Start");
		#endif
		
		m_Notifications = new array< ref ExpansionNotificationUI >;
		m_NotificationData = new array< ref NotificationRuntimeData >;

		m_NotificationUIGrid = NULL;
		
		if ( IsMissionClient() )
		{
			NotificationSystem.BindOnAdd( AddNotification );
			NotificationSystem.BindOnRemove( RemoveNotification );
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
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNotificationModule::~ExpansionNotificationModule - End");
		#endif
	}

	/*override void RegisterKeyMouseBindings() 
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNotificationModule::RegisterKeyMouseBindings - Start");
		#endif
		
		super.RegisterKeyMouseBindings();
		
		RegisterBinding( new JMModuleBinding( "Input_ShowNotification", "UAExpansionNotificationTestShow", true ) );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNotificationModule::RegisterKeyMouseBindings - End");
		#endif
	}*/

	/*void Input_ShowNotification( UAInput input )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNotificationModule::Input_ShowNotification - Start");
		#endif
		
		if ( !(input.LocalPress()) )
			return;
		
		NotificationSystem.Create( new StringLocaliser( "STR_EXPANSION_MISSION_NOTIF_TITLE", "wow" ), new StringLocaliser( "STR_EXPANSION_MISSION_NOTIF_TITLE", "test 43" ), "set:expansion_iconset image:icon_info", ARGB( 255, 221, 38, 38 ) );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNotificationModule::Input_ShowNotification - End");
		#endif
	}*/

	// ------------------------------------------------------------
	int GetNotificationsCount()
	{
		return m_Notifications.Count();
	}

	// ------------------------------------------------------------
	/*void CreateNotification_Old( string title, string text, int color, float time = 3, PlayerIdentity sendTo )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNotificationModule::CreateNotification_Old - Start");
		#endif
		
		StringLocaliser oTitle = new StringLocaliser( title );
		StringLocaliser oText = new StringLocaliser( text );
		oTitle.SetTranslates( false );
		oText.SetTranslates( false );

		NotificationSystem.Create( oTitle, oText, "set:expansion_iconset image:icon_info", color, time, sendTo );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNotificationModule::CreateNotification_Old - End");
		#endif
	}*/

	// ------------------------------------------------------------
	void AddNotification( ref NotificationRuntimeData data )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNotificationModule::AddNotification - Start");
		#endif
	
		if ( GetExpansionClientSettings() && GetExpansionClientSettings().ShowNotifications )
		{
			if ( data == NULL )
				return;
			
			if ( m_NotificationUIGrid == NULL )
				return;
	
			m_Notifications.InsertAt( new ExpansionNotificationUI( data, this, m_NotificationUIGrid.GetGrid() ), 0 );
			m_NotificationData.InsertAt( data, 0 );
	
			m_Notifications[0].ShowNotification();
		
		}
			
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNotificationModule::AddNotification - End");
		#endif
	}

	// ------------------------------------------------------------
	void RemoveNotification( ref NotificationRuntimeData data )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNotificationModule::RemoveNotification - Start");
		#endif
		
		if ( data == NULL )
			return;

		int index = m_NotificationData.Find( data );

		if ( index >= 0 )
			HideNotification( index );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNotificationModule::RemoveNotification - End");
		#endif
	}

	// ------------------------------------------------------------
	void HideNotification( int index )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNotificationModule::HideNotification - Start");
		#endif
		
		if ( GetGame().IsMultiplayer() && GetGame().IsServer() )
			return;

		if ( index < 0 )
			return;
			
		if ( index >= m_Notifications.Count() )
			return;

		m_Notifications[index].HideNotification();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNotificationModule::HideNotification - End");
		#endif
	}

	// ------------------------------------------------------------
	// DEPRECATED
	// ------------------------------------------------------------
	void RemovingNotification( ExpansionNotificationUI notif )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNotificationModule::RemovingNotification - Start");
		#endif
		
		if ( !notif )
			return;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNotificationModule::RemovingNotification - End");
		#endif
	}
	
	// ------------------------------------------------------------
	void RemoveNotification( ExpansionNotificationUI notif )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNotificationModule::RemoveNotification - Start");
		#endif
		
		if ( !notif )
			return;

		int index = m_Notifications.Find( notif );
		
		if ( index > -1 )
		{
			m_Notifications.RemoveOrdered( index );
			m_NotificationData.RemoveOrdered( index );
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNotificationModule::RemoveNotification - End");
		#endif
	}
	
	// ------------------------------------------------------------
	override void OnUpdate( float timeslice )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNotificationModule::OnUpdate - Start");
		#endif
		
		super.OnUpdate( timeslice );

		if ( IsMissionClient() )
		{
			for ( int i = 0; i < m_Notifications.Count(); i++ )
			{
				if ( m_Notifications[i] )
				{
					m_Notifications[i].Update( timeslice );
				}
			}
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNotificationModule::OnUpdate - End");
		#endif
	}
	
	// ------------------------------------------------------------
	override void OnMissionLoaded()
	{
		if ( IsMissionClient() && m_NotificationUIGrid == NULL )
		{
			m_NotificationUIGrid = new ExpansionNotificationUIGrid();
		}
	}
}