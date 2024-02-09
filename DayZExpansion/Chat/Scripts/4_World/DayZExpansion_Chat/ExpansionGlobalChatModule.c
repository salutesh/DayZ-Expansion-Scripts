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
	void ExpansionGlobalChatModule()
	{
		auto trace = EXTrace.Start(ExpansionTracing.CHAT, this);

		GetPermissionsManager().RegisterPermission("Admin.Chat");
	}

	override void OnInit()
	{
		super.OnInit();

		Expansion_EnableRPCManager();
		Expansion_RegisterBothRPC("RPC_AddChatMessage");
	}

	//! @note This RPC is only ever sent/received in MP
	void RPC_AddChatMessage(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
		auto trace = EXTrace.Start(ExpansionTracing.CHAT, this);

		ChatMessageEventParams data;

		if (!ctx.Read(data))
			return;

	#ifdef SERVER
		if (CanSendInChannel(data.param1, sender))
			AddChatMessage_Server(sender, target, ctx, data);
	#else
		AddChatMessage_Client(sender, target, ctx, data);
	#endif
	}

	void AddChatMessage_Server(PlayerIdentity sender, Object target, ParamsReadContext ctx, ChatMessageEventParams data)
	{
		PlayerBase player = PlayerBase.Cast(sender.GetPlayer());
		data.param2 = sender.GetName();

		auto rpc = Expansion_CreateRPC("RPC_AddChatMessage");
		rpc.Write(data);

		switch (data.param1)
		{
			case ExpansionChatChannels.CCTransport:
				CarScript vehicle;
				if (Class.CastTo(vehicle, player.GetParent()))
				{
					//! Only send RPC to vehicle crew
					set<Human> crew = vehicle.Expansion_GetVehicleCrew();
					foreach (Human crewMember: crew)
					{
						rpc.Expansion_Send(vehicle, true, crewMember.GetIdentity());
					}
				}

				break;

#ifdef EXPANSIONMODGROUPS
			case ExpansionChatChannels.CCTeam:
				int partyID = player.Expansion_GetPartyID();
				if (partyID >= 0)
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
							{
								rpc.Expansion_Send(true, partyPlayer.GetIdentity());
							}
						}
					}
				}
				
				break;
#endif

			default:
				//! Send RPC to everyone (global)
				rpc.Expansion_Send(true);
				break;
		}

		GetGame().GetMission().OnEvent(ChatMessageEventTypeID, data);

		// Uses similar format as vanilla direct chat log
		if (GetExpansionSettings().GetLog().Chat)
		{
			string channelName = GetChannelName(data.param1);
			string uid = sender.GetId();

			GetExpansionSettings().GetLog().PrintLog("[Chat - " + channelName + "](\"" + data.param2 + "\"(id=" + uid + ")): " + data.param3);
			GetGame().AdminLog("[Chat - " + channelName + "](\"" + data.param2 + "\"(id=" + uid + ")): " + data.param3);
		}
	}

	void AddChatMessage_Client(PlayerIdentity sender, Object target, ParamsReadContext ctx, ChatMessageEventParams data)
	{
		switch (data.param1)
		{
			case ExpansionChatChannels.CCTransport:
				Object localParent = Object.Cast(g_Game.GetPlayer().GetParent());
				//! `target` will only be non-NULL if player is in same network bubble as sender
				if (target && target.IsTransport() && localParent == target)
					break;
				else
					return;

			break;

#ifdef EXPANSIONMODGROUPS
			case ExpansionChatChannels.CCTeam:
				int partyID;
				if (!ctx.Read(partyID))
					return;

				if (partyID == ExpansionPartyModule.s_Instance.GetPartyID())
					break;
				else
					return;

			break;
#endif
		}

		GetGame().GetMission().OnEvent(ChatMessageEventTypeID, data);
	}

	bool CanSendInChannel(ExpansionChatChannels channel, PlayerIdentity sender)
	{
		PlayerBase player = PlayerBase.Cast(sender.GetPlayer());

		switch (channel)
		{
			case ExpansionChatChannels.CCAdmin:
			{
				if (GetPermissionsManager().HasPermission("Admin.Chat", sender))
				{
					return true;
				}

				break;
			}
			case ExpansionChatChannels.CCGlobal:
			{
				if (GetExpansionSettings().GetChat().EnableGlobalChat)
				{
					return true;
				}

				break;
			}
#ifdef EXPANSIONMODGROUPS
			case ExpansionChatChannels.CCTeam:
			{
				if (!player)
				{
					break;
				}

				int partyID = player.Expansion_GetPartyID();
				if (partyID >= 0 && GetExpansionSettings().GetChat().EnablePartyChat)
				{
					return true;
				}

				break;
			}
#endif
			case ExpansionChatChannels.CCTransport:
			{
				if (!player)
				{
					break;
				}

				CarScript vehicle;
				if (Class.CastTo(vehicle, player.GetParent()) && GetExpansionSettings().GetChat().EnableTransportChat)
				{
					return true;
				}

				break;
			}
			default:
			{
				return true;
			}
		}

		return false;
	}

	static string GetChannelName(ExpansionChatChannels channel)
	{
		switch (channel)
		{
			case ExpansionChatChannels.CCLocal:
			case ExpansionChatChannels.CCDirect: return "Direct";
			case ExpansionChatChannels.CCSystem: return "System";
			case ExpansionChatChannels.CCAdmin: return "Admin";
			case ExpansionChatChannels.CCMegaphone: return "Megaphone";
			case ExpansionChatChannels.CCTransmitter: return "Radio";
			case ExpansionChatChannels.CCPublicAddressSystem: return "PAS";
			case ExpansionChatChannels.CCBattlEye: return "BattlEye";
			case ExpansionChatChannels.CCTransport: return "Transport";
			case ExpansionChatChannels.CCGlobal: return "Global";
			case ExpansionChatChannels.CCTeam: return "Team";
		}

		return "Unknown";
	}
};
