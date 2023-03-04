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
	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);

		Man man = GetGame().GetPlayer(); //! Refernce to man
		UIScriptedMenu topMenu = m_UIManager.GetMenu(); //! Expansion reference to menu
		PlayerBase playerPB = PlayerBase.Cast(man);	//! Expansion reference to player
		ExpansionScriptViewMenu viewMenu = ExpansionScriptViewMenu.Cast(GetDayZExpansion().GetExpansionUIManager().GetMenu());
		bool showPartyHUDSetting = GetExpansionSettings().GetParty(false).IsLoaded() && (GetExpansionSettings().GetParty().ShowPartyMemberHUD && GetExpansionClientSettings().ShowPartyMemberHUD);
		
		if (playerPB && playerPB.GetHumanInventory())
		{
			if (playerPB.GetPlayerState() == EPlayerStates.ALIVE && showPartyHUDSetting)
			{
				bool isChatInputMenu;
				if (topMenu)
					isChatInputMenu = topMenu.IsInherited(ChatInputMenu);

				if ((topMenu || viewMenu) && !isChatInputMenu)
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
		}
	}
};
#endif
