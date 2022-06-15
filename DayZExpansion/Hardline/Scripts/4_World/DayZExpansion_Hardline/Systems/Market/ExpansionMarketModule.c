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
		
		ExpansionHardlineItemData itemData = GetExpansionSettings().GetHardline().GetHardlineItemDataByType(itemClassName);
		
		int required;
		if (itemData && !HasRankForItem(player, itemData.GetRarity(), required))
		{			
			StringLocaliser rankTitle = new StringLocaliser("Rank to low!");
			StringLocaliser rankText = new StringLocaliser("This item requires %1 humanity to buy!", required.ToString());
			ExpansionNotification(rankTitle, rankText, EXPANSION_NOTIFICATION_ICON_INFO, COLOR_EXPANSION_NOTIFICATION_EXPANSION, 3, ExpansionNotificationType.MARKET).Create(player.GetIdentity());
			return;
		}

		super.Exec_RequestPurchase(player, itemClassName, count, currentPrice, trader, includeAttachments, skinIndex, attachmentIDs);
	}
	
	override private void Exec_RequestSell(notnull PlayerBase player, string itemClassName, int count, int playerSentPrice, ExpansionTraderObjectBase trader, ExpansionMarketSellDebug playerSentSellDebug)
	{
		if (!player)
		{
			return;
		}
		
		ExpansionHardlineItemData itemData = GetExpansionSettings().GetHardline().GetHardlineItemDataByType(itemClassName);
		
		int required;
		if (itemData && !HasRankForItem(player, itemData.GetRarity(), required))
		{
			StringLocaliser rankTitle = new StringLocaliser("Rank to low!");
			StringLocaliser rankText = new StringLocaliser("This item requires %1 humanity to sell!", required.ToString());
			ExpansionNotification(rankTitle, rankText, EXPANSION_NOTIFICATION_ICON_INFO, COLOR_EXPANSION_NOTIFICATION_EXPANSION, 3, ExpansionNotificationType.MARKET).Create(player.GetIdentity());
			return;
		}
		
		super.Exec_RequestSell(player, itemClassName, count, playerSentPrice, trader, playerSentSellDebug);
	}
	
	bool HasRankForItem(PlayerBase player, int rarity, out int required)
	{
		int humanity = player.GetHumanity();
		int negative;

		if (player.IsHero())
		{
			if (rarity == ExpansionHardlineItemRarity.POOR && humanity < GetExpansionSettings().GetHardline().PoorItemRequirement)
			{
				required = GetExpansionSettings().GetHardline().PoorItemRequirement;
				return false;
			}
			
			if (rarity == ExpansionHardlineItemRarity.COMMON && humanity < GetExpansionSettings().GetHardline().CommonItemRequirement)
			{
				required = GetExpansionSettings().GetHardline().CommonItemRequirement;
				return false;
			}
			
			if (rarity == ExpansionHardlineItemRarity.UNCOMMON && humanity < GetExpansionSettings().GetHardline().UncommonItemRequirement)
			{
				required = GetExpansionSettings().GetHardline().UncommonItemRequirement;
				return false;
			}
			
			if (rarity == ExpansionHardlineItemRarity.RARE && humanity < GetExpansionSettings().GetHardline().RareItemRequirement)
			{
				required = GetExpansionSettings().GetHardline().RareItemRequirement;
				return false;
			}
			
			if (rarity == ExpansionHardlineItemRarity.EPIC && humanity < GetExpansionSettings().GetHardline().EpicItemRequirement)
			{
				required = GetExpansionSettings().GetHardline().EpicItemRequirement;
				return false;
			}
			
			if (rarity == ExpansionHardlineItemRarity.LEGENDARY && humanity < GetExpansionSettings().GetHardline().LegendaryItemRequirement)
			{
				required = GetExpansionSettings().GetHardline().LegendaryItemRequirement;
				return false;
			}
			
			if (rarity == ExpansionHardlineItemRarity.MYTHIC && humanity < GetExpansionSettings().GetHardline().MythicItemRequirement)
			{
				required = GetExpansionSettings().GetHardline().MythicItemRequirement;
				return false;
			}
			
			if (rarity == ExpansionHardlineItemRarity.EXOTIC && humanity < GetExpansionSettings().GetHardline().ExoticItemRequirement)
			{
				required = GetExpansionSettings().GetHardline().ExoticItemRequirement;
				return false;
			}
		}
		else if (player.IsBandit())
		{	
			negative =- GetExpansionSettings().GetHardline().PoorItemRequirement;
			if (rarity == ExpansionHardlineItemRarity.POOR && humanity > negative)
			{
				required =- negative;
				return false;
			}
			
			negative =- GetExpansionSettings().GetHardline().CommonItemRequirement;
			if (rarity == ExpansionHardlineItemRarity.COMMON && humanity > negative)
			{
				required =- negative;
				return false;
			}
			
			negative =- GetExpansionSettings().GetHardline().UncommonItemRequirement;
			if (rarity == ExpansionHardlineItemRarity.UNCOMMON && humanity > negative)
			{
				required =- negative;
				return false;
			}
			
			negative =- GetExpansionSettings().GetHardline().RareItemRequirement;
			if (rarity == ExpansionHardlineItemRarity.RARE && humanity > negative)
			{
				required =- negative;
				return false;
			}
			
			negative =- GetExpansionSettings().GetHardline().EpicItemRequirement;
			if (rarity == ExpansionHardlineItemRarity.EPIC && humanity > negative)
			{
				required =- negative;				
				return false;
			}

			negative =- GetExpansionSettings().GetHardline().LegendaryItemRequirement;
			if (rarity == ExpansionHardlineItemRarity.LEGENDARY && humanity > negative)
			{
				required =- negative;
				return false;
			}
			
			negative =- GetExpansionSettings().GetHardline().MythicItemRequirement;
			if (rarity == ExpansionHardlineItemRarity.MYTHIC && humanity > negative)
			{
				required =- negative;
				return false;
			}

			negative =- GetExpansionSettings().GetHardline().ExoticItemRequirement;
			if (rarity == ExpansionHardlineItemRarity.EXOTIC && humanity > negative)
			{
				required =- negative;
				return false;
			}
		}
		else if (!player.IsBandit() && !player.IsHero())
		{
			if (rarity == ExpansionHardlineItemRarity.POOR && humanity < GetExpansionSettings().GetHardline().PoorItemRequirement)
			{
				required = GetExpansionSettings().GetHardline().PoorItemRequirement;
				return false;
			}
			
			if (rarity == ExpansionHardlineItemRarity.COMMON && humanity < GetExpansionSettings().GetHardline().CommonItemRequirement)
			{
				required = GetExpansionSettings().GetHardline().CommonItemRequirement;
				return false;
			}
			
			if (rarity == ExpansionHardlineItemRarity.UNCOMMON && humanity < GetExpansionSettings().GetHardline().UncommonItemRequirement)
			{
				required = GetExpansionSettings().GetHardline().UncommonItemRequirement;
				return false;
			}
			
			if (rarity == ExpansionHardlineItemRarity.RARE && humanity < GetExpansionSettings().GetHardline().RareItemRequirement)
			{
				required = GetExpansionSettings().GetHardline().RareItemRequirement;
				return false;
			}
			
			if (rarity == ExpansionHardlineItemRarity.EPIC && humanity < GetExpansionSettings().GetHardline().EpicItemRequirement)
			{
				required = GetExpansionSettings().GetHardline().EpicItemRequirement;
				return false;
			}
			
			if (rarity == ExpansionHardlineItemRarity.LEGENDARY && humanity < GetExpansionSettings().GetHardline().LegendaryItemRequirement)
			{
				required = GetExpansionSettings().GetHardline().LegendaryItemRequirement;
				return false;
			}
			
			if (rarity == ExpansionHardlineItemRarity.MYTHIC && humanity < GetExpansionSettings().GetHardline().MythicItemRequirement)
			{
				required = GetExpansionSettings().GetHardline().MythicItemRequirement;
				return false;
			}
			
			if (rarity == ExpansionHardlineItemRarity.EXOTIC && humanity < GetExpansionSettings().GetHardline().ExoticItemRequirement)
			{
				required = GetExpansionSettings().GetHardline().ExoticItemRequirement;
				return false;
			}
		}

		return true;
	}
};
#endif