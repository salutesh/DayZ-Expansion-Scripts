/**
 * NotificationSystem.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class NotificationSystem
{
	autoptr array< ref NotificationRuntimeData > m_ExNotifications;

	void NotificationSystem()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.NOTIFICATIONS, this, "NotificationSystem");
#endif
		
		m_ExNotifications = new array< ref NotificationRuntimeData >;
	}

	static void Create_Expansion( StringLocaliser title, StringLocaliser text, string icon, int color, float time = 7, PlayerIdentity sendTo = NULL, ExpansionNotificationType type = ExpansionNotificationType.TOAST, Object obj = NULL )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.NOTIFICATIONS, "NotificationSystem", "Create_Expansion");
#endif

		if ( IsMissionHost() )
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write( title );
			rpc.Write( text );
			rpc.Write( icon );
			rpc.Write( color );
			rpc.Write( time );
			rpc.Write( type );
			rpc.Write( obj );
			rpc.Send( NULL, ExpansionRPC.CreateNotification, true, sendTo );
		}
		else
		{
			if ( !m_Instance )
				return;

			m_Instance.ExpansionCreateNotification( title, text, icon, color, time, sendTo, type, obj );
		}
	}

	static void Create_Expansion( string title, string text, string icon, int color, float time = 7, PlayerIdentity sendTo = NULL, ExpansionNotificationType type = ExpansionNotificationType.TOAST, Object obj = NULL )
	{
		Create_Expansion( new StringLocaliser( title ), new StringLocaliser( text ), icon, color, time, sendTo, type, obj );
	}

	static void ExpansionCreateNotification( ref StringLocaliser title, ref StringLocaliser text, string icon, int color, float time = 3, PlayerIdentity sendTo = NULL, ExpansionNotificationType type = ExpansionNotificationType.TOAST, Object obj = NULL )
	{	
		if ( IsMissionHost() )
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write( title );
			rpc.Write( text );
			rpc.Write( icon );
			rpc.Write( color );
			rpc.Write( time );
			rpc.Write( type );
			rpc.Write( obj );
			rpc.Send( NULL, ExpansionRPC.CreateNotification, true, sendTo );
		} 
		else
		{
			Exec_ExpansionCreateNotification( title, text, icon, color, time, type, obj );
		}
	}

	private static void Exec_ExpansionCreateNotification(  StringLocaliser title, StringLocaliser text, string icon, int color, float time, ExpansionNotificationType type = ExpansionNotificationType.TOAST, Object obj = NULL )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.NOTIFICATIONS, "NotificationSystem", "Exec_ExpansionCreateNotification");
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

		NotificationRuntimeData data = new NotificationRuntimeData( time, new NotificationData( icon, title.Format() ), text.Format() );
		data.SetColor( color );
		data.m_LeaveJoinNotif = leaveJoinNotif;
		data.SetType( type );
		data.SetObject( obj );
		
		m_Instance.AddNotif( data );
	}
	
	#ifdef CF_BUGFIX_REF
	override static void RPC_CreateNotification( PlayerIdentity sender, Object target, ParamsReadContext ctx )
	{
		//! This is only here to make all CF notifications (non-Expansion mods) use Expansion visual style for consistency

#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.NOTIFICATIONS, "NotificationSystem", "RPC_CreateNotification");
#endif
		
		StringLocaliser title = new StringLocaliser( "" );
		if ( !ctx.Read( title ) )
			return;

		StringLocaliser text = new StringLocaliser( "" );
		if ( !ctx.Read( text ) )
			return;

		string icon;
		if ( !ctx.Read( icon ) )
			return;

		int color;
		if ( !ctx.Read( color ) )
			return;

		float time;
		if ( !ctx.Read( time ) )
			return;

		Exec_ExpansionCreateNotification( title, text, icon, color, time );
	}
	
	// ------------------------------------------------------------
	// NotificationSystem AddNotif
	// ------------------------------------------------------------
	override void AddNotif(  NotificationRuntimeData data )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.NOTIFICATIONS, this, "AddNotif");
#endif
		
		data.SetTime( data.GetTime() - NOTIFICATION_FADE_TIME );

		m_ExNotifications.Insert( data );
		m_OnNotificationAdded.Invoke( data );
	}
	#else
	
	override static void RPC_CreateNotification( PlayerIdentity sender, Object target, ref ParamsReadContext ctx )
	{
		//! This is only here to make all CF notifications (non-Expansion mods) use Expansion visual style for consistency

#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.NOTIFICATIONS, "NotificationSystem", "RPC_CreateNotification");
#endif
		
		StringLocaliser title = new StringLocaliser( "" );
		if ( !ctx.Read( title ) )
			return;

		StringLocaliser text = new StringLocaliser( "" );
		if ( !ctx.Read( text ) )
			return;

		string icon;
		if ( !ctx.Read( icon ) )
			return;

		int color;
		if ( !ctx.Read( color ) )
			return;

		float time;
		if ( !ctx.Read( time ) )
			return;

		Exec_ExpansionCreateNotification( title, text, icon, color, time );

	}

	override void AddNotif( ref NotificationRuntimeData data )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.NOTIFICATIONS, this, "AddNotif");
#endif

		data.SetTime( data.GetTime() - NOTIFICATION_FADE_TIME );

		m_ExNotifications.Insert( data );
		m_OnNotificationAdded.Invoke( data );
	}
	#endif
	
	static void RPC_ExpansionCreateNotification( PlayerIdentity sender, Object target, ParamsReadContext ctx )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.NOTIFICATIONS, "NotificationSystem", "RPC_ExpansionCreateNotification");
#endif

		StringLocaliser title = new StringLocaliser( "" );
		if ( !ctx.Read( title ) )
			return;

		StringLocaliser text = new StringLocaliser( "" );
		if ( !ctx.Read( text ) )
			return;

		string icon;
		if ( !ctx.Read( icon ) )
			return;

		int color;
		if ( !ctx.Read( color ) )
			return;

		float time;
		if ( !ctx.Read( time ) )
			return;
		
		int type;
		if ( !ctx.Read( type ) )
			return;
		
		Object obj;
		if ( !ctx.Read( obj ) )
			return;

		Exec_ExpansionCreateNotification( title, text, icon, color, time, type, obj );
	}

	override static void Update(float timeslice)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.NOTIFICATIONS, "NotificationSystem", "Update");
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
	}

	override NotificationData GetNotificationData( NotificationType type )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.NOTIFICATIONS, this, "GetNotificationData");
#endif
		
		if ( m_DataArray.Contains(type) )
		{
			return m_DataArray.Get( type );
		}
		
		return null;
	}
};