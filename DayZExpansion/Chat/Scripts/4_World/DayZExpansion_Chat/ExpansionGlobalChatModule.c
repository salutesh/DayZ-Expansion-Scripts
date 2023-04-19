/**
 * ExpansionGlobalChatModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionGlobalChatModule
 * @brief		This class handle global chat
 **/

[CF_RegisterModule(ExpansionGlobalChatModule)]
class ExpansionGlobalChatModule: CF_ModuleWorld
{
#ifdef EXPANSIONMODGROUPS
	protected ref ExpansionPartyModule m_PartyModule;
#endif

	// ------------------------------------------------------------
	void ExpansionGlobalChatModule()
	{
		auto trace = EXTrace.Start(ExpansionTracing.CHAT, this);

		GetPermissionsManager().RegisterPermission( "Admin.Chat" );
	}
	
	override void OnInit()
	{
		super.OnInit();

		EnableRPC();
	}

	// ------------------------------------------------------------
	void AddChatMessage( ParamsReadContext ctx, PlayerIdentity sender, Object target )
	{
		auto trace = EXTrace.Start(ExpansionTracing.CHAT, this);
		
		if (!ExpansionScriptRPC.CheckMagicNumber(ctx))
			return;
		
		ChatMessageEventParams data;

		Object parent;
		int partyID = -1;

#ifdef EXPANSIONMODGROUPS
		if (!m_PartyModule) // i tried this in OnInit, and it still gave me errors
			m_PartyModule = ExpansionPartyModule.Cast( CF_ModuleCoreManager.Get(ExpansionPartyModule) );
#endif

		if ( !ctx.Read( data ) )
		{
			return;
		}
		
		if ( IsMissionHost() )
		{
			PlayerBase player = PlayerBase.GetPlayerByUID(sender.GetId());
			bool canSendMessage = true;
			string channelName = "";
			
			switch ( data.param1 )
			{
				case ExpansionChatChannels.CCAdmin:
					canSendMessage = GetPermissionsManager().HasPermission( "Admin.Chat", sender );
					channelName = "Admin";
					break;
				case ExpansionChatChannels.CCGlobal:
					canSendMessage = GetExpansionSettings().GetChat().EnableGlobalChat;
					channelName = "Global";
					break;
#ifdef EXPANSIONMODGROUPS
				case ExpansionChatChannels.CCTeam:
					if (!m_PartyModule) break;
					if (player) partyID = m_PartyModule.GetPartyID(player);

					canSendMessage = partyID >= 0 && GetExpansionSettings().GetChat().EnablePartyChat;
					channelName = "Team";
					break;
#endif
				case ExpansionChatChannels.CCTransport:
					if (player) parent = Object.Cast( player.GetParent() );
					canSendMessage = parent && parent.IsTransport() && GetExpansionSettings().GetChat().EnableTransportChat;
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
					auto rpc = ExpansionScriptRPC.Create();
					rpc.Write(data);

					if (parent)
						rpc.Write(parent);
					if (partyID >= 0)
						rpc.Write(partyID);

					rpc.Send(null, ExpansionGlobalChatRPC.AddChatMessage, true);
				}

				// Uses similar format as vanilla direct chat log
				if ( GetExpansionSettings().GetLog().Chat )
				{
					GetExpansionSettings().GetLog().PrintLog( "[Chat - " + channelName + "](\"" + data.param2 + "\"(id=" + biuid + ")): " + data.param3 );
					GetGame().AdminLog( "[Chat - " + channelName + "](\"" + data.param2 + "\"(id=" + biuid + ")): " + data.param3 );
				}
			}
		} else
		{
			switch (data.param1)
			{
				case ExpansionChatChannels.CCTransport:
					if (!ctx.Read(parent))
						return;

					Object localParent = Object.Cast( g_Game.GetPlayer().GetParent() );
					//! `parent` will only be non-NULL if player is in same network bubble as sender
					if (parent && parent.IsTransport() && localParent == parent)
						break;
					else
						return;
					
				break;

#ifdef EXPANSIONMODGROUPS
				case ExpansionChatChannels.CCTeam:
					if (!ctx.Read(partyID))
							return;

					if (partyID == m_PartyModule.GetPartyID())
						break;
					else
						return;

#endif
				break;
			}

			GetGame().GetMission().OnEvent(ChatMessageEventTypeID, data);
		}
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
	
	override void OnRPC(Class sender, CF_EventArgs args)
	{
		auto trace = EXTrace.Start(ExpansionTracing.CHAT, this);

		super.OnRPC(sender, args);

		auto rpc = CF_EventRPCArgs.Cast(args);

		switch ( rpc.ID )
		{
			case ExpansionGlobalChatRPC.AddChatMessage:
				AddChatMessage( rpc.Context, rpc.Sender, rpc.Target );
			break;
		}
	}
};
