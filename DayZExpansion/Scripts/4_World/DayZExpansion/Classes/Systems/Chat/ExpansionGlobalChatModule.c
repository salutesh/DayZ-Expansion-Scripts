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

/**@class		ExpansionGlobalChatModule
 * @brief		This class handle global chat
 **/
class ExpansionGlobalChatModule: JMModuleBase
{
	protected ref ExpansionPartyModule m_PartyModule;

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
	void AddChatMessage( ParamsReadContext ctx, PlayerIdentity sender, Object target )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionGlobalChatModule::AddChatMessage - Start");
		#endif
		
		ChatMessageEventParams data;

		Object parent;
		int partyID = -1;

		if (!m_PartyModule) // i tried this in OnInit, and it still gave me errors
			m_PartyModule = ExpansionPartyModule.Cast( GetModuleManager().GetModule(ExpansionPartyModule) );

		if ( !ctx.Read( data ) )
		{
			#ifdef EXPANSIONEXPRINT
			EXPrint("ExpansionGlobalChatModule::AddChatMessage - Start");
			#endif
			return;
		}
		
		if ( IsMissionHost() )
		{
			PlayerBase player = GetPlayerObjectByIdentity(sender);
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
				case ExpansionChatChannels.CCTeam:
					if (!m_PartyModule) break;
					if (player) partyID = m_PartyModule.GetPartyID(player);

					canSendMessage = player && m_PartyModule.HasParty(player) && GetExpansionSettings().GetGeneral().EnablePartyChat;
					channelName = "Team";
					break;
				case ExpansionChatChannels.CCTransport:
					if (player) parent = Object.Cast( player.GetParent() );
					canSendMessage = parent && parent.IsTransport() && GetExpansionSettings().GetGeneral().EnableTransportChat;
					channelName = "Transport";
					break;
			}

			if ( canSendMessage )
			{
				data.param2 = sender.GetName();
				string steamid = sender.GetPlainId();
				string biuid = sender.GetId();
				string idtable = sender.GetPlayerId().ToString();

				if ( GetGame().IsMultiplayer() )
				{
					ScriptRPC rpc = new ScriptRPC();
					rpc.Write(data);

					if (parent)
						rpc.Write(parent);
					if (partyID >= 0)
						rpc.Write(partyID);

					rpc.Send(null, ExpansionGlobalChatRPC.AddChatMessage, true);
				}

				// Uses similar format as vanilla direct chat log
				if ( GetExpansionSettings().GetLog().Chat )
					GetExpansionSettings().GetLog().PrintLog( "[Chat - " + channelName + "](\"" + data.param2 + "\"(id=" + biuid + ")): " + data.param3 );
			}
		} else
		{
			switch (data.param1)
			{
				case ExpansionChatChannels.CCTransport:
					if (!ctx.Read(parent))
						return;

					Object localParent = Object.Cast( g_Game.GetPlayer().GetParent() );
					if (parent.IsTransport() && localParent == parent)
						break;
					else
						return;
					
				break;

				case ExpansionChatChannels.CCTeam:
					if (!ctx.Read(partyID))
							return;

					if (partyID == m_PartyModule.GetPartyID())
						break;
					else
						return;

				break;
			}

			GetGame().GetMission().OnEvent(ChatMessageEventTypeID, data);
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
	
	#ifdef CF_BUGFIX_REF
	override void OnRPC( PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx )
	#else
	override void OnRPC( PlayerIdentity sender, Object target, int rpc_type, ref ParamsReadContext ctx )
	#endif
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