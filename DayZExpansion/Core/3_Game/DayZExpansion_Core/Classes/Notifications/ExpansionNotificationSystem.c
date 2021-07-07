/**
 * ExpansionNotificationSystem.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionNotificationSystem
 * @brief		This class handle notification toasts system
 **/
class ExpansionNotificationSystem
{
	static ref ScriptInvoker SI_CreateNotification = new ScriptInvoker();
	
	// ------------------------------------------------------------
	// ExpansionNotificationSystem CreateNotification
	// ------------------------------------------------------------
	void CreateNotification( ref StringLocaliser title, ref StringLocaliser text, string icon, int color, float time = 3 )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNotificationSystem::CreateNotification - Start");
		#endif
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "NOTIFICATION: " + title.Format() + ": " + text.Format() );
		#endif

		#ifdef EXPANSIONMOD
		if ( GetExpansionSettings() && GetExpansionSettings().GetNotification().EnableNotification )
		{
		#endif
			NotificationSystem.Create_Expansion( title, text, icon, color, time );
		#ifdef EXPANSIONMOD
		}
		#endif

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNotificationSystem::CreateNotification - End");
		#endif
	}

	// ------------------------------------------------------------
	// ExpansionNotificationSystem CreateNotification
	// ------------------------------------------------------------
	void CreateNotification( ref StringLocaliser title, ref StringLocaliser text, string icon, int color, float time, PlayerIdentity identity )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNotificationSystem::CreateNotification - Start");
		#endif
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "NOTIFICATION: " + title.Format() + ": " + text.Format() );
		#endif

		NotificationSystem.Create_Expansion( title, text, icon, color, time, identity );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNotificationSystem::CreateNotification - End");
		#endif
	}
};

static ref ExpansionNotificationSystem g_exNotificationBase;

// ------------------------------------------------------------
// ExpansionNotificationSystem GetNotificationSystem
// ------------------------------------------------------------
static ref ExpansionNotificationSystem GetNotificationSystem()
{
	#ifdef EXPANSIONEXPRINT
	EXPrint("ExpansionNotificationSystem::GetNotificationSystem - Start");
	#endif
	
	if ( !g_exNotificationBase )
	{
		g_exNotificationBase = new ExpansionNotificationSystem();
	}
	
	#ifdef EXPANSIONEXPRINT
	EXPrint("ExpansionNotificationSystem::GetNotificationSystem - Return: " + g_exNotificationBase.ToString() );
	#endif
	
	return g_exNotificationBase;
}

// ------------------------------------------------------------
// ExpansionNotificationSystem DestroyNotificationSystem
// ------------------------------------------------------------
static void DestroyNotificationSystem()
{
	#ifdef EXPANSIONEXPRINT
	EXPrint("ExpansionNotificationSystem::DestroyNotificationSystem - Start");
	#endif
	
	delete g_exNotificationBase;
	
	#ifdef EXPANSIONEXPRINT
	EXPrint("ExpansionNotificationSystem::DestroyNotificationSystem - End");
	#endif
}