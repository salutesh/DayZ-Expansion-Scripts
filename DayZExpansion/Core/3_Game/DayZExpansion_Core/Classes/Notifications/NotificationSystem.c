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
	static void Create_Expansion( ref StringLocaliser title, ref StringLocaliser text, string icon, int color, float time = 7, PlayerIdentity sendTo = NULL )
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

	static void Create_Expansion( string title, string text, string icon, int color, float time = 7, PlayerIdentity sendTo = NULL )
	{
		Create_Expansion( new StringLocaliser( title ), new StringLocaliser( text ), icon, color, time, sendTo );
	}

	override void AddNotif( ref NotificationRuntimeData data )
	{
		super.AddNotif( data );
	}
};