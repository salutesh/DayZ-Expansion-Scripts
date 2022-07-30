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
			ExpansionHardlineItemRarity rarity = GetExpansionSettings().GetHardline().GetHardlineItemRarityByType(itemClassName);
			int required;
			if (rarity && !HasRankForRarity(player, rarity, required))
			{
				StringLocaliser rankTitle = new StringLocaliser("#STR_EXPANSION_HARDLINE_MARKET_RANKLOW");
				StringLocaliser rankText = new StringLocaliser("#STR_EXPANSION_HARDLINE_MARKET_RANKLOW_BUY_DESC", required.ToString());
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
			ExpansionHardlineItemRarity rarity = GetExpansionSettings().GetHardline().GetHardlineItemRarityByType(itemClassName);
			int required;
			if (rarity && !HasRankForRarity(player, rarity, required))
			{
				StringLocaliser rankTitle = new StringLocaliser("#STR_EXPANSION_HARDLINE_MARKET_RANKLOW");
				StringLocaliser rankText = new StringLocaliser("#STR_EXPANSION_HARDLINE_MARKET_RANKLOW_SELL_DESC", required.ToString());
				ExpansionNotification(rankTitle, rankText, EXPANSION_NOTIFICATION_ICON_INFO, COLOR_EXPANSION_NOTIFICATION_EXPANSION, 3, ExpansionNotificationType.MARKET).Create(player.GetIdentity());
				return;
			}
		}
		
		super.Exec_RequestSell(player, itemClassName, count, playerSentPrice, trader, playerSentSellDebug);
	}
	
	bool HasRankForRarity(PlayerBase player, ExpansionHardlineItemRarity rarity, out int required)
	{
		if (rarity == ExpansionHardlineItemRarity.NONE)
		{
			return true;
		}

		int humanity = player.GetHumanity();
		int rank = GetRankForRarity(rarity);

		if (player.IsBandit())
		{
			if (humanity > -rank)
			{
				required = -rank;
				return false;
			}
		}
		else
		{
			//! Hero / bambi

			if (humanity < rank)
			{
				required = rank;
				return false;
			}
		}

		return true;
	}

	int GetRankForRarity(ExpansionHardlineItemRarity rarity)
	{
		switch (rarity)
		{
			case ExpansionHardlineItemRarity.POOR:
				return GetExpansionSettings().GetHardline().PoorItemRequirement;
			
			case ExpansionHardlineItemRarity.COMMON:
				return GetExpansionSettings().GetHardline().CommonItemRequirement;
			
			case ExpansionHardlineItemRarity.UNCOMMON:
				return GetExpansionSettings().GetHardline().UncommonItemRequirement;
			
			case ExpansionHardlineItemRarity.RARE:
				return GetExpansionSettings().GetHardline().RareItemRequirement;
			
			case ExpansionHardlineItemRarity.EPIC:
				return GetExpansionSettings().GetHardline().EpicItemRequirement;
			
			case ExpansionHardlineItemRarity.LEGENDARY:
				return GetExpansionSettings().GetHardline().LegendaryItemRequirement;
			
			case ExpansionHardlineItemRarity.MYTHIC:
				return GetExpansionSettings().GetHardline().MythicItemRequirement;
			
			case ExpansionHardlineItemRarity.EXOTIC:
				return GetExpansionSettings().GetHardline().ExoticItemRequirement;
		}

		return 0;
	}
};
#endif
