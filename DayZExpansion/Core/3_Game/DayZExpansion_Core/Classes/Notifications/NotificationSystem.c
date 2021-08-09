/**
 * NotificationSystem.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class NotificationSystem
{
	// ------------------------------------------------------------
	// NotificationSystem Create_Expansion
	// ------------------------------------------------------------
	// Create ur own system until CF have a better framework
	static void Create_Expansion( StringLocaliser title, StringLocaliser text, string icon, int color, float time = 7, PlayerIdentity sendTo = NULL, ExpansionNotificationType type = ExpansionNotificationType.TOAST, Object obj = NULL )
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
			#ifdef EXPANSIONMOD
			rpc.Write( type );
			rpc.Write( obj );
			rpc.Send( NULL, ExpansionRPC.CreateNotification, true, sendTo );
			#else
			rpc.Send( NULL, NotificationSystemRPC.Create, true, sendTo );
			#endif
		}
		else
		{
			if ( !m_Instance )
				return;

			#ifdef EXPANSIONMOD
			m_Instance.ExpansionCreateNotification( title, text, icon, color, time, sendTo, type, obj );
			#else
			m_Instance.CreateNotification( title, text, icon, color, time, sendTo );
			#endif
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("NotificationSystem::Create_Expansion - End");
		#endif
	}

	static void Create_Expansion( string title, string text, string icon, int color, float time = 7, PlayerIdentity sendTo = NULL, ExpansionNotificationType type = ExpansionNotificationType.TOAST, Object obj = NULL )
	{
		Create_Expansion( new StringLocaliser( title ), new StringLocaliser( text ), icon, color, time, sendTo, type, obj );
	}

	#ifdef CF_BUGFIX_REF
	//! THIS NEEDS TO BE HERE otherwise it WON'T WORK (compile error) when Expansion Main isn't loaded
	override void AddNotif( NotificationRuntimeData data )
	{
		super.AddNotif( data );
	}
	#else
	//! THIS NEEDS TO BE HERE otherwise it WON'T WORK (compile error) when Expansion Main isn't loaded
	override void AddNotif( ref NotificationRuntimeData data )
	{
		super.AddNotif( data );
	}
	#endif
};