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

		Expansion_EnableRPCManager();
		Expansion_RegisterBothRPC("RPC_AddChatMessage");
	}

	// ------------------------------------------------------------
	void RPC_AddChatMessage(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
		auto trace = EXTrace.Start(ExpansionTracing.CHAT, this);
		
		ChatMessageEventParams data;

		CarScript vehicle;
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
			bool canSendMessage;
			string channelName = "";
			
			switch ( data.param1 )
			{
				case ExpansionChatChannels.CCAdmin:
					if (GetPermissionsManager().HasPermission( "Admin.Chat", sender ))
						canSendMessage = true;
					channelName = "Admin";
					break;
				case ExpansionChatChannels.CCGlobal:
					if (GetExpansionSettings().GetChat().EnableGlobalChat)
						canSendMessage = true;
					channelName = "Global";
					break;
#ifdef EXPANSIONMODGROUPS
				case ExpansionChatChannels.CCTeam:
					if (!m_PartyModule) break;
					if (player) partyID = m_PartyModule.GetPartyID(player);

					if (partyID >= 0 && GetExpansionSettings().GetChat().EnablePartyChat)
						canSendMessage = true;
					channelName = "Team";
					break;
#endif
				case ExpansionChatChannels.CCTransport:
					if (player && Class.CastTo(vehicle, player.GetParent()) && GetExpansionSettings().GetChat().EnableTransportChat)
						canSendMessage = true;
					channelName = "Transport";
					break;
				default:
					canSendMessage = true;
					break;
			}

			if ( canSendMessage )
			{
				data.param2 = sender.GetName();
				string biuid = sender.GetId();

				if ( GetGame().IsMultiplayer() )
				{
					auto rpc = Expansion_CreateRPC("RPC_AddChatMessage");
					rpc.Write(data);

					if (vehicle)
					{
						//! Only send RPC to vehicle crew
						set<Human> crew = vehicle.Expansion_GetVehicleCrew();
						foreach (Human crewMember: crew)
						{
							rpc.Expansion_Send(vehicle, true, crewMember.GetIdentity());
						}
					}
				#ifdef EXPANSIONMODGROUPS
					else if (partyID >= 0)
					{
						rpc.Write(partyID);

						//! Only send RPC to party players
						ExpansionPartyData party = player.Expansion_GetParty();
						if (party)
						{
							array<ref ExpansionPartyPlayerData> players = party.GetPlayers();
							foreach (ExpansionPartyPlayerData partyMember: players)
							{
								PlayerBase partyPlayer = partyMember.Player;
								if (partyPlayer && partyPlayer.GetIdentity())
									rpc.Expansion_Send(true, partyPlayer.GetIdentity());
							}
						}
					}
				#endif
					else
					{
						//! Send RPC to everyone (global)
						rpc.Expansion_Send(true);
					}
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
					Object localParent = Object.Cast( g_Game.GetPlayer().GetParent() );
					//! `target` will only be non-NULL if player is in same network bubble as sender
					if (target && target.IsTransport() && localParent == target)
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

				break;
#endif
			}

			GetGame().GetMission().OnEvent(ChatMessageEventTypeID, data);
		}
	}
};
