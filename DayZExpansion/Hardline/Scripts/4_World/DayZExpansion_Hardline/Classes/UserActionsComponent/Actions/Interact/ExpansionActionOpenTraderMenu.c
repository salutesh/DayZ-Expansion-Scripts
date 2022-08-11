/**
 * ExpansionActionOpenTraderMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONMODMARKET
modded class ExpansionActionOpenTraderMenu
{
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if (!super.ActionCondition(player, target, item))
			return false;

		m_TraderObject = ExpansionMarketModule.GetTraderFromObject(target.GetObject(), false);
		
		//! TODO: Conditions should be moved to market module and checked in Exec_RequestPurchase / Exec_RequestSell as well

		if (m_TraderObject.GetTraderMarket().DisplayName == "Hero Trader" && !player.IsHero())
			return false;
		
		if (m_TraderObject.GetTraderMarket().DisplayName == "Bandit Trader" && !player.IsBandit())
			return false;
		
		if (GetGame().IsServer() && GetExpansionSettings().GetHardline().UseHumanity)
		{
			int minHumanity = m_TraderObject.GetTraderMarket().MinRequiredHumanity;
			int maxHumanity = m_TraderObject.GetTraderMarket().MaxRequiredHumanity;
			//! 1 < -2147483647 == true >:(
			bool humanityLow;
			if ((player.GetHumanity() >= 0 && minHumanity >= 0) || (player.GetHumanity() < 0 && minHumanity < 0))
				humanityLow = player.GetHumanity() < minHumanity;
			else if (player.GetHumanity() < 0 && minHumanity >= 0)
				humanityLow = true;
			if (humanityLow || player.GetHumanity() > maxHumanity)
			{
				if (player.GetIdentity())
				{
					ExpansionHardlineRank minRank;
					ExpansionHardlineRank maxRank;
					int minRankReq;
					int maxRankReq;
					GetExpansionSettings().GetHardline().GetRankRange(minHumanity, maxHumanity, minRank, maxRank, minRankReq, maxRankReq);
					string minRankKey = "STR_EXPANSION_HARDLINE_RANK_" + typename.EnumToString(ExpansionHardlineRank, minRank);
					string maxRankKey = "STR_EXPANSION_HARDLINE_RANK_" + typename.EnumToString(ExpansionHardlineRank, maxRank);
					ExpansionNotification("STR_EXPANSION_HARDLINE_HUMANITY_OUTOFRANGE", new StringLocaliser("STR_EXPANSION_HARDLINE_HUMANITY_OUTOFRANGE_TRADER", minHumanity.ToString(), minRankKey, maxHumanity.ToString(), maxRankKey), EXPANSION_NOTIFICATION_ICON_ERROR, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, ExpansionNotificationType.MARKET).Create(player.GetIdentity());
				}

				return false;
			}
		}

		return true;
	}
}
#endif
