/**
 * ExpansionGlobalChatModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

enum ExpansionGlobalChatRPC
{
	INVALID = 20500,
	AddChatMessage,
	COUNT
}

/**@class		ExpansionGlobalChatModule
 * @brief		This class handle global chat
 **/
class ExpansionGlobalChatModule: JMModuleBase
{
	// ------------------------------------------------------------
	void ExpansionGlobalChatModule()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionGlobalChatModule::ExpansionGlobalChatModule - Start");
		#endif
		
		GetPermissionsManager().RegisterPermission( "Admin.Chat" );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionGlobalChatModule::ExpansionGlobalChatModule - End");
		#endif
	}
	
	// ------------------------------------------------------------
	void AddChatMessage( ref ParamsReadContext ctx, PlayerIdentity sender, Object target )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionGlobalChatModule::AddChatMessage - Start");
		#endif
		
		ChatMessageEventParams data;

		if ( !ctx.Read( data ) )
		{
			#ifdef EXPANSIONEXPRINT
			EXPrint("ExpansionGlobalChatModule::AddChatMessage - Start");
			#endif
			return;
		}
		
		if ( IsMissionHost() )
		{
			bool canSendMessage = true;
			string channelName = "";
			
			switch ( data.param1 )
			{
				case ExpansionChatChannels.CCAdmin:
					canSendMessage = GetPermissionsManager().HasPermission( "Admin.Chat", sender );
					channelName = "Admin";
					break;
				case ExpansionChatChannels.CCGlobal:
					canSendMessage = GetExpansionSettings().GetGeneral().EnableGlobalChat;
					channelName = "Global";
					break;
			}

			if ( canSendMessage )
			{
				data.param2 = sender.GetName();
				string steamid = sender.GetPlainId();
				string biuid = sender.GetId();
				string idtable = sender.GetPlayerId().ToString();

				string format = ( "[" + GetDateTime() + "] [" + channelName + "] " + data.param2 + ": " + data.param3 + " (SteamID = " + steamid + ", BIUid = " + biuid + ", PlayerID = " + idtable + ")");

				if ( GetGame().IsMultiplayer() )
				{
					ScriptRPC rpc = new ScriptRPC();
					rpc.Write( data );
					rpc.Send( null, ExpansionGlobalChatRPC.AddChatMessage, true );
				}
			}
		} else
		{
			GetGame().GetMission().OnEvent( ChatMessageEventTypeID, data );
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionGlobalChatModule::AddChatMessage - End");
		#endif
	}

	private string GetDateTime()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionGlobalChatModule::GetDateTime - Start");
		#endif
		int year, month, day;
		int hour, minute, second;
		GetYearMonthDay( year, month, day );

		GetHourMinuteSecond( hour, minute, second );
		string date = day.ToStringLen( 2 ) + "." + month.ToStringLen( 2 ) + "." + year.ToStringLen( 2 );
		string time = hour.ToStringLen( 2 ) + ":" + minute.ToStringLen( 2 ) + ":" + second.ToStringLen( 2 );

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionGlobalChatModule::GetDateTime - End");
		#endif
		return (date + " / " + time);
	}
	
	// ------------------------------------------------------------
	// Override GetRPCMin
	// ------------------------------------------------------------
	override int GetRPCMin()
	{
		return ExpansionGlobalChatRPC.INVALID;
	}
	
	// ------------------------------------------------------------
	// Override GetRPCMax
	// ------------------------------------------------------------
	override int GetRPCMax()
	{
		return ExpansionGlobalChatRPC.COUNT;
	}
	
	// ------------------------------------------------------------
	// Override OnRPC
	// ------------------------------------------------------------
	override void OnRPC( PlayerIdentity sender, Object target, int rpc_type, ref ParamsReadContext ctx )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionGlobalChatModule::OnRPC - Start");
		#endif
		switch ( rpc_type )
		{
			case ExpansionGlobalChatRPC.AddChatMessage:
				AddChatMessage( ctx, sender, target );
			break;
		}
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionGlobalChatModule::OnRPC - End");
		#endif
	}
}