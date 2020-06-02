/**
 * ExpansionLogger.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionLogger
{
	private string m_ChatLogFile;
	private string m_MarketLogFile;
	private string m_KillFeedLogFile;
		
	void ~ExpansionLogger()
	{
		ClearLogs();
	}
	
	void ClearLogs()
	{
		ClearMarketLogs();
		ClearChatLogs();
		ClearKillFeedLogs();
	}
	
	void ClearMarketLogs()
	{
		if ( FileExist( EXPANSION_MARKET_LOG_FOLDER ) )
		{
			ref array<string> market_log_files = new array<string>;
			market_log_files = FindFilesInLocation(EXPANSION_MARKET_LOG_FOLDER);
			if ( market_log_files.Count() > 0 )
			{
				for ( int i = 0; i < market_log_files.Count(); i++ )
				{
					int marketFileLog = OpenFile( market_log_files[i], FileMode.READ );
					bool deleteFile = false;
					if ( marketFileLog != 0 )
					{
						ref array<string> market_log_lines = new array<string>;
						string market_line_content;
						while ( FGets( marketFileLog, market_line_content ) > 0 )
						{
							market_log_lines.Insert( market_line_content );
						}
						
						if ( market_log_lines.Count() == 0 || !market_log_lines || market_log_lines == NULL )
							deleteFile = true;
					}
					
					CloseFile( marketFileLog );
					
					if ( deleteFile )
						DeleteFile( market_log_files[i] );
				}
			}
		}
	}
	
	void ClearChatLogs()
	{
		if ( FileExist( EXPANSION_CHAT_LOG_FOLDER ) )
		{
			ref array<string> chat_log_files = new array<string>;
			chat_log_files = FindFilesInLocation(EXPANSION_CHAT_LOG_FOLDER);
			if ( chat_log_files.Count() > 0 )
			{
				for ( int i = 0; i < chat_log_files.Count(); i++ )
				{
					int chatFileLog = OpenFile( chat_log_files[i], FileMode.READ );
					bool deleteFile = false;
					if ( chatFileLog != 0 )
					{
						ref array<string> chat_log_lines = new array<string>;
						string chat_line_content;
						while ( FGets( chatFileLog, chat_line_content ) > 0 )
						{
							chat_log_lines.Insert( chat_line_content );
						}
						
						if ( chat_log_lines.Count() == 0 || !chat_log_lines || chat_log_lines == NULL )
							deleteFile = true;
					}
					
					CloseFile( chatFileLog );
					
					if ( deleteFile )
						DeleteFile( chat_log_files[i] );
				}
			}
		}
	}
	
	void ClearKillFeedLogs()
	{
		if ( FileExist( EXPANSION_KILLFEED_LOG_FOLDER ) )
		{
			ref array<string> killfeed_log_files = new array<string>;
			killfeed_log_files = FindFilesInLocation(EXPANSION_KILLFEED_LOG_FOLDER);
			if ( killfeed_log_files.Count() > 0 )
			{
				for ( int i = 0; i < killfeed_log_files.Count(); i++ )
				{
					int killfeedFileLog = OpenFile( killfeed_log_files[i], FileMode.READ );
					bool deleteFile = false;
					if ( killfeedFileLog != 0 )
					{
						ref array<string> killfeed_log_lines = new array<string>;
						string killfeed_line_content;
						while ( FGets( killfeedFileLog, killfeed_line_content ) > 0 )
						{
							killfeed_log_lines.Insert( killfeed_line_content );
						}
						
						if ( killfeed_log_lines.Count() == 0 || !killfeed_log_lines || killfeed_log_lines == NULL )
							deleteFile = true;
					}
					
					CloseFile( killfeedFileLog );
					
					if ( deleteFile )
						DeleteFile( killfeed_log_files[i] );
				}
			}
		}
	}
	
	// ------------------------------------------------------------
	// Expansion Init
	// ------------------------------------------------------------	
	void Init()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionLogger::Init - Start");
		#endif
		
		if ( !FileExist( EXPANSION_LOG_FOLDER ) )
		{
			MakeDirectory( EXPANSION_LOG_FOLDER );
		}
		
		ClearMarketLogs();
		ClearChatLogs();
		ClearKillFeedLogs();
		
		CreateNewChatLog();
		CreateNewMarketLog();
		CreateNewKillFeedLog();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionLogger::Init - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion CreateNewChatLog
	// ------------------------------------------------------------	
	void CreateNewChatLog()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionLogger::CreateNewChatLog - Start");
		#endif
		
		if ( !FileExist( EXPANSION_CHAT_LOG_FOLDER ) )
		{
			MakeDirectory( EXPANSION_CHAT_LOG_FOLDER );
		}

		m_ChatLogFile = (EXPANSION_CHAT_LOG_FOLDER + "Chat_" + GetDateTime() + ".log");
		int fileLog = OpenFile( m_ChatLogFile, FileMode.WRITE );
		
		if ( fileLog != 0 )
		{
			CloseFile( fileLog );
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionLogger::CreateNewChatLog - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion CreateNewMarketLog
	// ------------------------------------------------------------	
	void CreateNewMarketLog()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionLogger::CreateNewChatLog - Start");
		#endif
		
		if ( !FileExist( EXPANSION_MARKET_LOG_FOLDER ) )
		{
			MakeDirectory( EXPANSION_MARKET_LOG_FOLDER );
		}

		m_MarketLogFile = (EXPANSION_MARKET_LOG_FOLDER + "Market_" + GetDateTime() + ".log");
		int fileLog = OpenFile( m_MarketLogFile, FileMode.WRITE );
		
		if ( fileLog != 0 )
		{
			CloseFile( fileLog );
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionLogger::CreateNewChatLog - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion CreateNewKillFeedLog
	// ------------------------------------------------------------	
	void CreateNewKillFeedLog()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionLogger::CreateNewKillFeedLog - Start");
		#endif
		
		if ( !FileExist( EXPANSION_KILLFEED_LOG_FOLDER ) )
		{
			MakeDirectory( EXPANSION_KILLFEED_LOG_FOLDER );
		}

		m_KillFeedLogFile = (EXPANSION_KILLFEED_LOG_FOLDER + "KillFeed_" + GetDateTime() + ".log");
		int fileLog = OpenFile( m_KillFeedLogFile, FileMode.WRITE );
		
		if ( fileLog != 0 )
		{
			CloseFile( fileLog );
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionLogger::CreateNewKillFeedLog - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion GetDateTime
	// ------------------------------------------------------------
	private string GetDateTime()
	{
		int year, month, day;
		int hour, minute, second;
		GetYearMonthDay( year, month, day );

		GetHourMinuteSecond( hour, minute, second );
		string date = day.ToStringLen( 2 ) + "-" + month.ToStringLen( 2 ) + "-" + year.ToStringLen( 2 );
		string time = hour.ToStringLen( 2 ) + "-" + minute.ToStringLen( 2 ) + "-" + second.ToStringLen( 2 );

		return (date + "_" + time);
	}
	
	// ------------------------------------------------------------
	// Expansion ChatLog
	// ------------------------------------------------------------
	void ChatLog( string text )
	{
		int chatFileLog = OpenFile( m_ChatLogFile, FileMode.APPEND );
		if ( chatFileLog != 0 )
		{
			FPrintln( chatFileLog, text );
			CloseFile( chatFileLog );
		}
	}
	
	// ------------------------------------------------------------
	// Expansion MarketLog
	// ------------------------------------------------------------
	void MarketLog( string text )
	{
		int marketFileLog = OpenFile( m_MarketLogFile, FileMode.APPEND );
		if ( marketFileLog != 0 )
		{
			FPrintln( marketFileLog, "[Market]::[" + GetDateTime() + "] " + text );
			CloseFile( marketFileLog );
		}
	}
	
	// ------------------------------------------------------------
	// Expansion KillFeedLog
	// ------------------------------------------------------------
	void KillFeedLog( string text )
	{
		int killFeedFileLog = OpenFile( m_KillFeedLogFile, FileMode.APPEND );
		if ( killFeedFileLog != 0 )
		{
			FPrintln( killFeedFileLog, "[KillFeed]::[" + GetDateTime() + "] " + text );
			CloseFile( killFeedFileLog );
		}
	}
}

static ref ExpansionLogger g_exLogger;
static ref ExpansionLogger GetExpansionLogger()
{
	if ( !g_exLogger )
	{
		g_exLogger = new ExpansionLogger;
	}

	return g_exLogger;
}

static void CreateExpansionLogger()
{
	if ( g_exLogger )
	{
		DestroyExpansionLogger();
	}

	g_exLogger = new ExpansionLogger;
	g_exLogger.Init();
}

static void DestroyExpansionLogger()
{
	delete g_exLogger;
}