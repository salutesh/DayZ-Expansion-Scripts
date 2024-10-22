/**
 * ExpansionActionOpenTraderMenu.c
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
modded class ExpansionActionOpenTraderMenu
{
	protected const int SCIENCE_SOCIETY_TRADER_QUEST = -1;
		
	override void OnStartServer(ActionData action_data)
	{
		super.OnStartServer(action_data);
		
	#ifdef EXPANSIONMODQUESTS
		if (GetExpansionSettings().GetQuest().EnableQuests)
		{
			ExpansionTraderObjectBase traderObject = ExpansionMarketModule.GetTraderFromObject(action_data.m_Target.GetObject());
			if (!traderObject)
				return;

			ExpansionMarketTrader trader = traderObject.GetTraderMarket();
			if (!trader)
				return;
			
			int questID = GetQuestIDByDisplayName(trader.DisplayName);
			Print(ToString() + "::OnExecuteServer - Need to complete quest with ID: " + questID);
			if (questID > -1)
			{
				//! Check if player has completed required quest
				if (!ExpansionQuestModule.GetModuleInstance().HasCompletedQuest(questID, action_data.m_Player.GetIdentity().GetId()))
				{
					ExpansionQuestConfig questConfig = ExpansionQuestModule.GetModuleInstance().GetQuestConfigByID(questID);
					if (!questConfig)
					{
						Error(ToString() + "::OnExecuteServer - Could not get quest config for quest ID: " + questID);
						return;
					}
		
					ExpansionNotification(new StringLocaliser("Trader locked!"), new StringLocaliser("You have no access to this trader yet. You need to compelete the quest " + questConfig.GetTitle() + " first to use this trader."), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_AMETHYST, 10, ExpansionNotificationType.TOAST).Create(action_data.m_Player.GetIdentity());
					return;
				}
			}
		}
	#endif
	}
	
	int GetQuestIDByDisplayName(string displayName)
	{
		switch (displayName)
		{
			case "Science Society":
				return SCIENCE_SOCIETY_TRADER_QUEST;
			default:
				return -1;
		}
		
		return -1;
	}
}
#endif