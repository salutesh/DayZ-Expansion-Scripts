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
	autoptr array< ref NotificationRuntimeData > m_ExNotifications = {};

	static ref ExpansionRPCManager s_Expansion_RPCManager = Expansion_RegisterRPCManager();

	void NotificationSystem()
	{
#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Start(EXTrace.NOTIFICATIONS, this);
#endif

#ifdef SERVER
		string tmp;
		DumpStackString(tmp);
		TStringArray lines = {};
		tmp.Split("\n", lines);

		TStringArray stack = {};

		for (int i = 1; i < lines.Count(); i++)
		{
			string line = lines[i];
			string lineLower = line;
			lineLower.ToLower();

			if (lineLower.IndexOf("dayzexpansion") == -1)
				stack.Insert(line);
		}

		EXError.Error(this, "NotificationSystem should not be initialized on server!", stack);
#else
		s_Expansion_RPCManager.SetOwner(this);
#endif
	}

	static ExpansionRPCManager Expansion_RegisterRPCManager()
	{
		auto manager = new ExpansionRPCManager(null, NotificationSystem);
		manager.RegisterClient("RPC_ExpansionCreateNotification");
		return manager;
	}

	static void Create_Expansion( CF_Localiser title, CF_Localiser text, string icon, int color, float time = 7, PlayerIdentity sendTo = NULL, ExpansionNotificationType type = ExpansionNotificationType.TOAST, Object obj = NULL )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.NOTIFICATIONS, "NotificationSystem", "Create_Expansion");
#endif

		if ( IsMissionHost() )
		{
			auto rpc = s_Expansion_RPCManager.CreateRPC("RPC_ExpansionCreateNotification");
			CF_Localiser.Write(rpc, title);
			CF_Localiser.Write(rpc, text);
			rpc.Write( icon );
			rpc.Write( color );
			rpc.Write( time );
			rpc.Write( type );
			rpc.Write( obj );
			rpc.Expansion_Send(true, sendTo);
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
		Create_Expansion( new CF_Localiser( title ), new CF_Localiser( text ), icon, color, time, sendTo, type, obj );
	}

	static void ExpansionCreateNotification( CF_Localiser title, CF_Localiser text, string icon, int color, float time = 3, PlayerIdentity sendTo = NULL, ExpansionNotificationType type = ExpansionNotificationType.TOAST, Object obj = NULL )
	{	
		if ( IsMissionHost() )
		{
			auto rpc = s_Expansion_RPCManager.CreateRPC("RPC_ExpansionCreateNotification");
			CF_Localiser.Write(rpc, title);
			CF_Localiser.Write(rpc, text);
			rpc.Write( icon );
			rpc.Write( color );
			rpc.Write( time );
			rpc.Write( type );
			rpc.Write( obj );
			rpc.Expansion_Send(true, sendTo);
		} 
		else
		{
			Exec_ExpansionCreateNotification( title, text, icon, color, time, type, obj );
		}
	}

	private static void Exec_ExpansionCreateNotification(  CF_Localiser title, CF_Localiser text, string icon, int color, float time, ExpansionNotificationType type = ExpansionNotificationType.TOAST, Object obj = NULL )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.NOTIFICATIONS, "NotificationSystem", "Exec_ExpansionCreateNotification");
#endif
		
		if ( !m_Instance )
			return;

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
		
		bool leaveJoinNotif;
		//! https://feedback.bistudio.com/T173348
		if (joinNotif || leaveNotif)
			leaveJoinNotif = true;

		NotificationRuntimeData data = new NotificationRuntimeData( time, new NotificationData( icon, title.Format() ), text.Format() );
		data.SetColor( color );
		data.m_LeaveJoinNotif = leaveJoinNotif;
		data.SetType( type );
		data.SetObject( obj );
		
		m_Instance.AddNotif( data );
	}
	
	override static void RPC_CreateNotification( PlayerIdentity sender, Object target, ParamsReadContext ctx )
	{
		//! This is only here to make all CF notifications (non-Expansion mods) use Expansion visual style for consistency
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.NOTIFICATIONS, "NotificationSystem", "RPC_CreateNotification");
#endif
		
		CF_Localiser title = new CF_Localiser( "" );
		//! @note can't use CF_Localiser.Read here since we don't control the sending side
		if ( !ctx.Read( title ) )
			return;

		CF_Localiser text = new CF_Localiser( "" );
		//! @note can't use CF_Localiser.Read here since we don't control the sending side
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
		
		//! @note: The reason we *subtract* NOTIFICATION_FADE_TIME here is because CF *adds* it in NotificationRuntimeData constructor,
		//! and we don't want that. Jumped through years old commit logs to try and understand why this is here. Please document your code :-(
		data.SetTime( data.GetTime() - NOTIFICATION_FADE_TIME );

		m_ExNotifications.Insert( data );
		m_OnNotificationAdded.Invoke( data );
	}
	
	static void RPC_ExpansionCreateNotification( PlayerIdentity sender, Object target, ParamsReadContext ctx )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.NOTIFICATIONS, "NotificationSystem", "RPC_ExpansionCreateNotification");
#endif

		CF_Localiser title = new CF_Localiser( "" );
		if ( !CF_Localiser.Read( ctx, title ) )
			return;

		CF_Localiser text = new CF_Localiser( "" );
		if ( !CF_Localiser.Read( ctx, text ) )
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
				if ( data.GetTime() < g_Game.GetTickTime() && data.m_HasShown )
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