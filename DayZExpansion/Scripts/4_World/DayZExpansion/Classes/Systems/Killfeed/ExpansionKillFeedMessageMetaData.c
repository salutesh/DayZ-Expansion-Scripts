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
		this.Message = ExpansionKillFeedModule.GetKillFeedMessage(type);
		this.Icon = icon;
		this.FeedParam1 = param1;
		this.FeedParam2 = param2;
		this.FeedParam3 = param3;
		this.FeedParam4 = param4;
	}
}
