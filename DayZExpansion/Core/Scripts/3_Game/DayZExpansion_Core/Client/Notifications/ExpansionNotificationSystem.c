/**
 * ExpansionNotificationSystem.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
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
	void CreateNotification( StringLocaliser title, StringLocaliser text, string icon, int color, float time = 7, PlayerIdentity identity = NULL, ExpansionNotificationType type = ExpansionNotificationType.TOAST)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.NOTIFICATIONS, this, "CreateNotification");
#endif

		ExpansionNotification( title, text, icon, color, time, type ).Create( identity );
	}

	void CreateNotification( string title, string text, string icon, int color, float time = 7, PlayerIdentity identity = NULL, ExpansionNotificationType type = ExpansionNotificationType.TOAST)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.NOTIFICATIONS, this, "CreateNotification");
#endif

		CreateNotification( new StringLocaliser( title ), new StringLocaliser( text ), icon, color, time, identity, type );
	}
};

static ref ExpansionNotificationSystem g_exNotificationBase;

// ------------------------------------------------------------
// ExpansionNotificationSystem GetNotificationSystem
// ------------------------------------------------------------
static ExpansionNotificationSystem GetNotificationSystem()
{
#ifdef EXPANSIONTRACE
	auto trace = CF_Trace_0(ExpansionTracing.NOTIFICATIONS, "GetNotificationSystem");
#endif
	
	EXPrint("DEPRECATED: ExpansionNotificationSystem is deprecated, use ExpansionNotification instead");
	
	if ( !g_exNotificationBase )
	{
		g_exNotificationBase = new ExpansionNotificationSystem();
	}
	
	return g_exNotificationBase;
}

// ------------------------------------------------------------
// ExpansionNotificationSystem DestroyNotificationSystem
// ------------------------------------------------------------
static void DestroyNotificationSystem()
{
#ifdef EXPANSIONTRACE
	auto trace = CF_Trace_0(ExpansionTracing.NOTIFICATIONS, "DestroyNotificationSystem");
#endif
	if (g_exNotificationBase)
		delete g_exNotificationBase;
}

class ExpansionNotificationTemplate<Class T>
{
	protected T m_Title;
	protected T m_Text;
	protected string m_Icon;
	protected int m_Color;
	protected float m_Time;
	protected ExpansionNotificationType m_Type;
	
	void ExpansionNotificationTemplate( T title, T text, string icon = "", int color = 0, float time = 7, ExpansionNotificationType type = ExpansionNotificationType.TOAST)
	{
		m_Title = title;
		m_Text = text;
		m_Icon = icon;
		m_Color = color;
		m_Time = time;
		m_Type = type;
	}

	void Create( PlayerIdentity identity = NULL )
	{
		Create( m_Icon, m_Color, identity, m_Type );
	}

	void Create( string icon, int color, PlayerIdentity identity = NULL, ExpansionNotificationType type = ExpansionNotificationType.TOAST )
	{
		if ( GetExpansionSettings() && GetExpansionSettings().GetNotification().EnableNotification )
		{
			NotificationSystem.Create_Expansion( m_Title, m_Text, icon, color, m_Time, identity, type );
		}
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

static ExpansionNotificationTemplate<StringLocaliser> ExpansionNotification( StringLocaliser title, StringLocaliser text, string icon = "", int color = 0, float time = 7, ExpansionNotificationType type = ExpansionNotificationType.TOAST )
{
	return new ExpansionNotificationTemplate<StringLocaliser>( title, text, icon, color, time, type );
}

static ExpansionNotificationTemplate<string> ExpansionNotification( string title, string text, string icon = "", int color = 0, float time = 7, ExpansionNotificationType type = ExpansionNotificationType.TOAST )
{
	return new ExpansionNotificationTemplate<string>( title, text, icon, color, time, type );
}

static ExpansionNotificationTemplate<StringLocaliser> ExpansionNotification( string title, StringLocaliser text, string icon = "", int color = 0, float time = 7, ExpansionNotificationType type = ExpansionNotificationType.TOAST )
{
	return ExpansionNotification( new StringLocaliser( title ), text, icon, color, time, type );
}

static ExpansionNotificationTemplate<StringLocaliser> ExpansionNotification( StringLocaliser title, string text, string icon = "", int color = 0, float time = 7, ExpansionNotificationType type = ExpansionNotificationType.TOAST )
{
	return ExpansionNotification( title, new StringLocaliser( text ), icon, color, time, type );
}
