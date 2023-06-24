/**
 * MissionGameplay.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		MissionGameplay
 * @brief		
 **/
#ifdef EXPANSIONMODGROUPSHUD
modded class MissionGameplay
{
	override void Expansion_OnUpdate(float timeslice, PlayerBase player, bool isAliveConscious, Input input, bool inputIsFocused, UIScriptedMenu menu, ExpansionScriptViewMenuBase viewMenu)
	{
		super.Expansion_OnUpdate(timeslice, player, isAliveConscious, input, inputIsFocused, menu, viewMenu);
		
		if (isAliveConscious)
		{
			if (input.LocalPress("UAExpansionPartyHUDToggle", false) && !viewMenu)
			{
				ExpansionNotification("STR_EXPANSION_PARTYHUD_TITLE", "STR_EXPANSION_PARTYHUD_TOGGLE", EXPANSION_NOTIFICATION_ICON_MARKER, COLOR_EXPANSION_NOTIFICATION_SUCCESS, 5).Info(player.GetIdentity());
				GetExpansionClientSettings().ShowPartyMemberHUD = !GetExpansionClientSettings().ShowPartyMemberHUD;
			}

			if (GetExpansionSettings().GetParty(false).IsLoaded() && (GetExpansionSettings().GetParty().ShowPartyMemberHUD && GetExpansionClientSettings().ShowPartyMemberHUD))
			{
				bool isChatInputMenu;
				if (menu)
					isChatInputMenu = menu.IsInherited(ChatInputMenu);

				if ((menu || viewMenu) && !isChatInputMenu)
				{
					if (m_Hud.GetPartyHUDState())
						m_Hud.PartyHUDHide();
				}
				else
				{
					if (!m_Hud.GetPartyHUDState())
						m_Hud.PartyHUDShow();
				}
			}
			else if (m_Hud.GetPartyHUDState())
			{
				m_Hud.PartyHUDHide();
			}
		}
	}
};
#endif
