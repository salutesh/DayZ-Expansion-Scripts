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
	void CreateNotification( ref StringLocaliser title, ref StringLocaliser text, string icon, int color, float time = 7, PlayerIdentity identity = NULL )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNotificationSystem::CreateNotification - Start");
		#endif
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "NOTIFICATION: " + title.Format() + ": " + text.Format() );
		#endif

		ExpansionNotification( title, text, icon, color, time ).Create( identity );

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionNotificationSystem::CreateNotification - End");
		#endif
	}

	void CreateNotification( string title, string text, string icon, int color, float time = 7, PlayerIdentity identity = NULL )
	{
		CreateNotification( new StringLocaliser( title ), new StringLocaliser( text ), icon, color, time, identity );
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
	
	EXPrint("DEPRECATED: ExpansionNotificationSystem is deprecated, use ExpansionNotification instead");
	
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

class ExpansionNotificationTemplate<Class T>
{
	protected T m_Title;
	protected T m_Text;
	protected string m_Icon;
	protected int m_Color;
	protected float m_Time;

	void ExpansionNotificationTemplate( T title, T text, string icon = "", int color = 0, float time = 7 )
	{
		m_Title = title;
		m_Text = text;
		m_Icon = icon;
		m_Color = color;
		m_Time = time;
	}

	void Create( PlayerIdentity identity = NULL )
	{
		Create( m_Icon, m_Color, identity );
	}

	void Create( string icon, int color, PlayerIdentity identity = NULL )
	{
		#ifdef EXPANSIONMOD
		if ( GetExpansionSettings() && GetExpansionSettings().GetNotification().EnableNotification )
		{
		#endif
			NotificationSystem.Create_Expansion( m_Title, m_Text, icon, color, m_Time, identity );
		#ifdef EXPANSIONMOD
		}
		#endif
	}

	void Error( PlayerIdentity identity = NULL )
	{
		if ( !m_Icon )
			m_Icon = EXPANSION_NOTIFICATION_ICON_ERROR;

		Create( m_Icon, COLOR_EXPANSION_NOTIFICATION_ERROR, identity );
	}

	void Info( PlayerIdentity identity = NULL )
	{
		if ( !m_Icon )
			m_Icon = EXPANSION_NOTIFICATION_ICON_INFO;

		Create( m_Icon, COLOR_EXPANSION_NOTIFICATION_INFO, identity );
	}

	void Success( PlayerIdentity identity = NULL )
	{
		if ( !m_Icon )
			m_Icon = EXPANSION_NOTIFICATION_ICON_INFO;

		Create( m_Icon, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, identity );
	}
}

static ExpansionNotificationTemplate<StringLocaliser> ExpansionNotification( StringLocaliser title, StringLocaliser text, string icon = "", int color = 0, float time = 7 )
{
	return new ExpansionNotificationTemplate<StringLocaliser>( title, text, icon, color, time );
}

static ExpansionNotificationTemplate<string> ExpansionNotification( string title, string text, string icon = "", int color = 0, float time = 7 )
{
	return new ExpansionNotificationTemplate<string>( title, text, icon, color, time );
}

static ExpansionNotificationTemplate<StringLocaliser> ExpansionNotification( string title, StringLocaliser text, string icon = "", int color = 0, float time = 7 )
{
	return ExpansionNotification( new StringLocaliser( title ), text, icon, color, time );
}

static ExpansionNotificationTemplate<StringLocaliser> ExpansionNotification( StringLocaliser title, string text, string icon = "", int color = 0, float time = 7 )
{
	return ExpansionNotification( title, new StringLocaliser( text ), icon, color, time );
}
