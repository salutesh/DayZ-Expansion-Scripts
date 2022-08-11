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
	override private void Exec_RequestPurchase(notnull PlayerBase player, string itemClassName, int count, int currentPrice, ExpansionTraderObjectBase trader, bool includeAttachments = true, int skinIndex = -1, TIntArray attachmentIDs = NULL)
	{
		if (!player)
		{
			return;
		}
		
		if (GetExpansionSettings().GetHardline().UseHumanity && GetExpansionSettings().GetHardline().UseItemRarity)
		{
			ExpansionHardlineItemRarity rarity = GetExpansionSettings().GetHardline().GetItemRarityByType(itemClassName);
			int required;
			if (rarity && !HasHumanityForRarity(player, rarity, required))
			{
				StringLocaliser rankTitle = new StringLocaliser("STR_EXPANSION_HARDLINE_MARKET_RANKLOW");
				StringLocaliser rankText = new StringLocaliser("STR_EXPANSION_HARDLINE_MARKET_RANKLOW_BUY_DESC", required.ToString());
				ExpansionNotification(rankTitle, rankText, EXPANSION_NOTIFICATION_ICON_INFO, COLOR_EXPANSION_NOTIFICATION_EXPANSION, 3, ExpansionNotificationType.MARKET).Create(player.GetIdentity());
				return;
			}
		}

		super.Exec_RequestPurchase(player, itemClassName, count, currentPrice, trader, includeAttachments, skinIndex, attachmentIDs);
	}
	
	override private void Exec_RequestSell(notnull PlayerBase player, string itemClassName, int count, int playerSentPrice, ExpansionTraderObjectBase trader, ExpansionMarketSellDebug playerSentSellDebug)
	{
		if (!player)
		{
			return;
		}
		
		if (GetExpansionSettings().GetHardline().UseHumanity && GetExpansionSettings().GetHardline().UseItemRarity)
		{
			ExpansionHardlineItemRarity rarity = GetExpansionSettings().GetHardline().GetItemRarityByType(itemClassName);
			int required;
			if (rarity && !HasHumanityForRarity(player, rarity, required))
			{
				StringLocaliser rankTitle = new StringLocaliser("STR_EXPANSION_HARDLINE_MARKET_RANKLOW");
				StringLocaliser rankText = new StringLocaliser("STR_EXPANSION_HARDLINE_MARKET_RANKLOW_SELL_DESC", required.ToString());
				ExpansionNotification(rankTitle, rankText, EXPANSION_NOTIFICATION_ICON_INFO, COLOR_EXPANSION_NOTIFICATION_EXPANSION, 3, ExpansionNotificationType.MARKET).Create(player.GetIdentity());
				return;
			}
		}
		
		super.Exec_RequestSell(player, itemClassName, count, playerSentPrice, trader, playerSentSellDebug);
	}
	
	bool HasHumanityForRarity(PlayerBase player, ExpansionHardlineItemRarity rarity, out int required)
	{
		if (rarity == ExpansionHardlineItemRarity.NONE)
		{
			return true;
		}

		required = GetExpansionSettings().GetHardline().GetHumanityForRarity(rarity);

		if (player.IsBandit())
			required = -required;

		return Math.AbsFloat(player.GetHumanity()) >= Math.AbsFloat(required);
	}
};
#endif
