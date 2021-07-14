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
	// ------------------------------------------------------------
	// ExpansionNotificationSystem Create_Expansion
	// ------------------------------------------------------------
	// Create ur own system until CF have a better framework
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
	
	// ------------------------------------------------------------
	// ExpansionNotificationSystem Create_Expansion
	// ------------------------------------------------------------
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

	override void AddNotif( ref NotificationRuntimeData data )
	{
		super.AddNotif( data );
	}
};