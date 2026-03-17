/**
 * ExpansionMarketModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONMODMARKET
modded class ExpansionMarketModule
{
	override bool CheckCanUseTrader(PlayerBase player, ExpansionTraderObjectBase trader)
	{
		if (!super.CheckCanUseTrader(player, trader))
			return false;

		if (g_Game.IsServer() && GetExpansionSettings().GetQuest().EnableQuests)
		{
			int questID = trader.GetTraderMarket().RequiredCompletedQuestID;
			if (questID > -1)
			{
				//! Check if player has completed required quest
				if (!ExpansionQuestModule.GetModuleInstance().HasCompletedQuest(questID, player.GetIdentity().GetId()))
				{
					ExpansionQuestConfig questConfig = ExpansionQuestModule.GetModuleInstance().GetQuestConfigByID(questID);
					if (!questConfig)
					{
						Error(ToString() + "::OnExecuteServer - Could not get quest config for quest ID: " + questID);
						return true;
					}
		
					ExpansionNotification("STR_EXPANSION_QUEST", new StringLocaliser("STR_EXPANSION_QUEST_REQUIRED_TRADER", questConfig.Title), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, ExpansionNotificationType.TOAST).Create(player.GetIdentity());
					return false;
				}
			}
		}

		return true;
	}
};
#endif