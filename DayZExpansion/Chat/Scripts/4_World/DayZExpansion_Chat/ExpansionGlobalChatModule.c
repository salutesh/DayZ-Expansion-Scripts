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
#ifdef EXTRACE
		auto trace = EXTrace.Start(ExpansionTracing.CHAT, this);
#endif

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
#ifdef EXTRACE
		auto trace = EXTrace.Start(ExpansionTracing.CHAT, this);
#endif

		ExpansionChatMessageEventParams data;

		if (!ctx.Read(data))
			return;

	#ifdef SERVER
		if (CanSendInChannel(data.param1, sender))
			AddChatMessage_Server(sender, target, ctx, data);
	#else
		AddChatMessage_Client(sender, target, ctx, data);
	#endif
	}

	void FilterBlacklistedWords(inout string text)
	{
		foreach (string word: GetExpansionSettings().GetChat().BlacklistedWords)
		{
			ExpansionString.Replace(text, word, "***", true);
		}
	}

	void AddChatMessage_Server(PlayerIdentity sender, Object target, ParamsReadContext ctx, ExpansionChatMessageEventParams data)
	{
		PlayerBase player = PlayerBase.Cast(sender.GetPlayer());

		data.param2 = sender.GetName();
		string originalText = data.param3;
		FilterBlacklistedWords(data.param3);
		data.param5 = sender.GetId();

	#ifdef EXPANSIONMODGROUPS
		ExpansionPartyData party = player.Expansion_GetParty();

		if (party && GetExpansionSettings().GetParty().DisplayPartyTag)
			data.param6 = party.GetPartyTagFormatted();
	#endif

		auto rpc = Expansion_CreateRPC("RPC_AddChatMessage");
		rpc.Write(data);

		switch (data.param1)
		{
			case ExpansionChatChannels.CCTransport:
				auto vehicle = ExpansionVehicle.Get(player, false, true);
				if (vehicle)
				{
					//! Only send RPC to vehicle crew
					set<Human> crew = vehicle.GetCrew(true, true);
					foreach (Human crewMember: crew)
					{
						rpc.Expansion_Send(vehicle.GetEntity(), true, crewMember.GetIdentity());
					}
				}

				break;

#ifdef EXPANSIONMODGROUPS
			case ExpansionChatChannels.CCTeam:
				if (party)
				{
					rpc.Write(party.GetPartyID());

					//! Only send RPC to party players
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
				
				break;
#endif

			case ExpansionChatChannels.CCDirect:
				float range = GetVoiceRange(player);
				player.Expansion_SendNear(rpc, player.GetPosition(), range, null, true);
				break;

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

			GetExpansionSettings().GetLog().PrintLog("[Chat - " + channelName + "](\"" + data.param2 + "\"(id=" + data.param5 + ")): " + originalText);
			GetGame().AdminLog("[Chat - " + channelName + "](\"" + data.param2 + "\"(id=" + data.param5 + ")): " + originalText);
		}
	}

	float GetVoiceRange(Object player)
	{
		int voiceLevel = GetGame().GetVoiceLevel(player);
		float range;

		switch (voiceLevel)
		{
			case VoiceLevel.Whisper:
				range = 9.0;
				break;
			case VoiceLevel.Shout:
				range = 60.0;
				break;
			default:
				//! Talk
				range = 30.0;
				break;
		}

		EXTrace.Print(EXTrace.CHAT, this, "GetVoiceRange " + player + " voice level " + voiceLevel + " range " + range);

		return range;
	}

	void AddChatMessage_Client(PlayerIdentity sender, Object target, ParamsReadContext ctx, ExpansionChatMessageEventParams data)
	{
		switch (data.param1)
		{
			case ExpansionChatChannels.CCTransport:
				//! `target` will only be non-NULL if player is in same network bubble as sender
				ExpansionVehicle vehicle;
				if (ExpansionVehicle.Get(vehicle, g_Game.GetPlayer(), false, true) && vehicle.GetEntity() == target)
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

				if (ExpansionVehicle.Get(player, false, true) && GetExpansionSettings().GetChat().EnableTransportChat)
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
