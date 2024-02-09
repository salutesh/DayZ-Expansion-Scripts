/**
 * ExpansionChatColors.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionChatColors: ExpansionColorSettingsBase
{
	string SystemChatColor;
	string AdminChatColor;
	string GlobalChatColor;
	string DirectChatColor;
	string TransportChatColor;
	string PartyChatColor;
	string TransmitterChatColor;

	//! These can be used in script via vanilla methods, e.g.
	//! player.MessageStatus("Hello") or GetGame().ChatMP(player, "Hello", "colorStatusChannel")
	//! player.MessageAction("Hello") or GetGame().ChatMP(player, "Hello", "colorAction")
	//! player.MessageFriendly("Hello") or GetGame().ChatMP(player, "Hello", "colorFriendly")
	//! player.MessageImportant("Hello") or GetGame().ChatMP(player, "Hello", "colorImportant")
	string StatusMessageColor;
	string ActionMessageColor;
	string FriendlyMessageColor;
	string ImportantMessageColor;
	string DefaultMessageColor;

	override void Update()
	{
		super.Update();

		//! Make sure none of the colors are zero (all transparent) and set defaults

		if (!Get("SystemChatColor"))
			Set("SystemChatColor", ARGB(255, 235, 69, 235));

		if (!Get("AdminChatColor"))
			Set("AdminChatColor", ARGB(255, 255, 57, 43));

		if (!Get("GlobalChatColor"))
			Set("GlobalChatColor", ARGB(255, 88, 195, 255));

		if (!Get("DirectChatColor"))
			Set("DirectChatColor", ARGB(255, 255, 255, 255));

		if (!Get("TransportChatColor"))
			Set("TransportChatColor", ARGB(255, 255, 206, 9));

		if (!Get("PartyChatColor"))
			Set("PartyChatColor", ARGB(255, 10, 250, 122));

		if (!Get("TransmitterChatColor"))
			Set("TransmitterChatColor", ARGB(255, 249, 255, 73));

		if (!Get("StatusMessageColor"))
			Set("StatusMessageColor", COLOR_BLUE);

		if (!Get("ActionMessageColor"))
			Set("ActionMessageColor", COLOR_YELLOW);

		if (!Get("FriendlyMessageColor"))
			Set("FriendlyMessageColor", COLOR_GREEN);

		if (!Get("ImportantMessageColor"))
			Set("ImportantMessageColor", COLOR_RED);

		if (!Get("DefaultMessageColor"))
			Set("DefaultMessageColor", COLOR_WHITE);
	}
}
