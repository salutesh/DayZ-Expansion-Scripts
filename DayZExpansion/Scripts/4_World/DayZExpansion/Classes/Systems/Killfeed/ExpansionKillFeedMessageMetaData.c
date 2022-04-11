/**
 * ExpansionKillFeedMessageMetaData.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionKillFeedMessageMetaData
{
	string Message;
	string Icon;
	string FeedParam1;
	string FeedParam2;
	string FeedParam3;
	string FeedParam4;
	
	void ExpansionKillFeedMessageMetaData(ExpansionKillFeedMessageType type, string icon, string param1 = "", string param2 = "", string param3 = "", string param4 = "")
	{
		this.Message = SetMessage(type);
		this.Icon = icon;
		this.FeedParam1 = param1;
		this.FeedParam2 = param2;
		this.FeedParam3 = param3;
		this.FeedParam4 = param4;
	}
	
	private string SetMessage(ExpansionKillFeedMessageType type)
	{
		string message;
		switch ( type )
		{
			case ExpansionKillFeedMessageType.FALL:
				message = "STR_EXPANSION_KILLFEED_PLAYER_DIED_FALLING";
				break;
			case ExpansionKillFeedMessageType.CAR_HIT_DRIVER:
				message = "STR_EXPANSION_KILLFEED_PLAYER_KILLED_CRASH_CAR";
				break;
			case ExpansionKillFeedMessageType.CAR_HIT_NODRIVER:
				message = "STR_EXPANSION_KILLFEED_PLAYER_CRASHED_CAR";
				break;
			case ExpansionKillFeedMessageType.CAR_CRASH:
				message = "STR_EXPANSION_KILLFEED_PLAYER_CRASHED_CAR";
				break;
			case ExpansionKillFeedMessageType.CAR_CRASH_CREW:
				message = "%1 cant drive a %2 and exploded. Killing %3";
				break;
			case ExpansionKillFeedMessageType.HELI_HIT_DRIVER:
				message = "STR_EXPANSION_KILLFEED_PLAYER_KILLED_CRASH_HELI";
				break;
			case ExpansionKillFeedMessageType.HELI_HIT_NODRIVER:
				message = "STR_EXPANSION_KILLFEED_PLAYER_CRASHED_HELI";
				break;
			case ExpansionKillFeedMessageType.HELI_CRASH:
				message = "STR_EXPANSION_KILLFEED_PLAYER_CRASHED_HELI";
				break;
			case ExpansionKillFeedMessageType.HELI_CRASH_CREW:
				message = "%1 cant fly a %2 and exploded. Killing %3";
				break;
			/*case ExpansionKillFeedMessageType.BOAT_CRASH:
				message = "%1 crashed with his %2 boat.";
				break;
			case ExpansionKillFeedMessageType.BOAT_CRASH_CREW:
				message = "%1 cant drive a %2 and exploded. Killing %3";
				break;*/
			case ExpansionKillFeedMessageType.BARBEDWIRE:
				message = "STR_EXPANSION_KILLFEED_PLAYER_BARBED_WIRE";
				break;
			case ExpansionKillFeedMessageType.FIRE:
				message = "STR_EXPANSION_KILLFEED_PLAYER_DIED_FIRE";
				break;
			/*case ExpansionKillFeedMessageType.SPECIAL_EXPLOSION:
				message = "STR_EXPANSION_KILLFEED_PLAYER_DIED_EXPLOSION_WEAPON";
				break;*/
			case ExpansionKillFeedMessageType.WEAPON_EXPLOSION:
				message = "STR_EXPANSION_KILLFEED_PLAYER_DIED_EXPLOSION";
				break;
			case ExpansionKillFeedMessageType.DEHYDRATION:
				message = "STR_EXPANSION_KILLFEED_PLAYER_DIED_DEHYDRATION";
				break;
			case ExpansionKillFeedMessageType.STARVATION:
				message = "STR_EXPANSION_KILLFEED_PLAYER_DIED_STARVATION";
				break;
			case ExpansionKillFeedMessageType.BLEEDING:
				message = "STR_EXPANSION_KILLFEED_PLAYER_DIED_BLEEDING";
				break;
			case ExpansionKillFeedMessageType.SUICIDE:
				message = "STR_EXPANSION_KILLFEED_PLAYER_DIED_SUICIDE";
				break;
			case ExpansionKillFeedMessageType.WEAPON:
				message = "STR_EXPANSION_KILLFEED_PLAYER_KILLED_PLAYER_WEAPON";
				break;
			case ExpansionKillFeedMessageType.WEAPON_NODISTANCE:
				message = "STR_EXPANSION_KILLFEED_PLAYER_KILLED_PLAYER_WEAPON_NODISTANCE";
				break;
			case ExpansionKillFeedMessageType.MELEWEAPON:
				message = "STR_EXPANSION_KILLFEED_PLAYER_KILLED_PLAYER_MELE";
				break;
			case ExpansionKillFeedMessageType.BAREHANDS:
				message = "STR_EXPANSION_KILLFEED_PLAYER_KILLED_PLAYE_BAREHANDS";
				break;
			case ExpansionKillFeedMessageType.INFECTED:
				message = "STR_EXPANSION_KILLFEED_PLAYER_KILLED_ZOMBIE";
				break;
			case ExpansionKillFeedMessageType.ANIMAL:
				message = "STR_EXPANSION_KILLFEED_PLAYER_KILLED_ANIMAL";
				break;
			case ExpansionKillFeedMessageType.KILLED_UNKNOWN:
				message = "STR_EXPANSION_KILLFEED_PLAYER_KILLED_UNKNOWN";
				break;
			case ExpansionKillFeedMessageType.DIED_UNKNOWN:
				message = "STR_EXPANSION_KILLFEED_PLAYER_DIED_UNKNOWN";
				break;
		}
		
		return message;
	}
}