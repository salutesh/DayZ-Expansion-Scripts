/**
 * NotificationSystem.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class NotificationSystem
{
	//Create ur own system until CF have a better framework
	static void Create_Expansion( ref StringLocaliser title, ref StringLocaliser text, string icon, int color, float time = 3 )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("NotificationSystem::Create_Expansion - Start");
		#endif

		if ( IsMissionHost() )
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write( title );
			rpc.Write( text );
			rpc.Write( icon );
			rpc.Write( color );
			rpc.Write( time );
			rpc.Send( NULL, NotificationSystemRPC.Create, true, NULL );
		}
		else
		{
			if ( !m_Instance )
				return;

			m_Instance.CreateNotification( title, text, icon, color, time, NULL );
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("NotificationSystem::Create_Expansion - End");
		#endif
	}

	static void Create_Expansion( ref StringLocaliser title, ref StringLocaliser text, string icon, int color, float time, PlayerIdentity sendTo )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("NotificationSystem::Create_Expansion - Start");
		#endif

		if ( IsMissionHost() )
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write( title );
			rpc.Write( text );
			rpc.Write( icon );
			rpc.Write( color );
			rpc.Write( time );
			rpc.Send( NULL, NotificationSystemRPC.Create, true, sendTo );
		}
		else
		{
			if ( !m_Instance )
				return;

			m_Instance.CreateNotification( title, text, icon, color, time, sendTo );
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("NotificationSystem::Create_Expansion - End");
		#endif
	}

	autoptr array< ref NotificationRuntimeData > m_ExNotifications;

	void NotificationSystem()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("NotificationSystem::NotificationSystem - Start");
		#endif
		
		m_ExNotifications = new array< ref NotificationRuntimeData >;

		#ifdef EXPANSIONEXPRINT
		EXPrint("NotificationSystem::NotificationSystem " + m_Instance);
		#endif
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("NotificationSystem::NotificationSystem - End");
		#endif
	}
	
	override private void Exec_CreateNotification( ref StringLocaliser title, ref StringLocaliser text, string icon, int color, float time )
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("NotificationSystem::CreateNotification - Start");
		#endif
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("NotificationSystem::CreateNotification title.GetText() : " + title.GetText());
		#endif
		
		bool joinNotif = false;
		bool leaveNotif = false;
				
		//! Don't show player notification if setting is off, same with leave notification
		if (title.GetText() == "STR_EXPANSION_PLAYER_JOINED_TITLE")
			joinNotif = true;
		
		if (title.GetText() == "STR_EXPANSION_PLAYER_LEFT_TITLE")
			leaveNotif = true;
		
		if ( joinNotif && !GetExpansionClientSettings().NotificationJoin )
			return;
		
		if ( leaveNotif && !GetExpansionClientSettings().NotificationLeave )
			return;
		
		bool leaveJoinNotif = joinNotif || leaveNotif;
		
		ref NotificationRuntimeData data = new NotificationRuntimeData( time, new NotificationData( icon, title.Format() ), text.Format() );
		data.SetColor( color );
		data.m_LeaveJoinNotif = leaveJoinNotif;

        AddNotif( data );
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("NotificationSystem::Exec_CreateNotification - End");
		#endif
	}

	override void AddNotif( ref NotificationRuntimeData data )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("NotificationSystem::AddNotif - Start");
		#endif
		
		data.SetTime( data.GetTime() - NOTIFICATION_FADE_TIME );

		m_ExNotifications.Insert( data );
		m_OnNotificationAdded.Invoke( data );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("NotificationSystem::AddNotif - End");
		#endif
	}

	override static void Update(float timeslice)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("NotificationSystem::Update - Start");
		#endif
		
		if ( m_Instance && IsMissionClient() )
		{
			array< NotificationRuntimeData > to_remove = new array< NotificationRuntimeData >;
			foreach ( NotificationRuntimeData data : m_Instance.m_ExNotifications )
			{
				if ( data.GetTime() < GetGame().GetTickTime() && data.m_HasShown )
				{
					to_remove.Insert( data );
				}
			}
			
			foreach ( NotificationRuntimeData removed : to_remove )
			{
				m_Instance.m_ExNotifications.RemoveItem( removed );
				m_Instance.m_OnNotificationRemoved.Invoke( removed );
			}
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("NotificationSystem::Update - End");
		#endif
	}
	
	override NotificationData GetNotificationData( NotificationType type )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("NotificationSystem::GetNotificationData - Start");
		#endif
		
		if ( m_DataArray.Contains(type) )
		{
			#ifdef EXPANSIONEXPRINT
		EXPrint("NotificationSystem::GetNotificationData - Return: " + m_DataArray.Get( type ).ToString() );
		#endif
			return m_DataArray.Get( type );
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("NotificationSystem::GetNotificationData - Return: null");
		#endif
		return null;
	}
}