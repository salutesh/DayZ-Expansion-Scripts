/**
 * ExpansionMarketMenuItemManagerPresetElement.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2024 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONMODMARKET
modded class ExpansionMarketMenuItemManagerPresetElement
{	
	override void OnElementButtonClick()
	{
		if (GetExpansionSettings().GetHardline().UseReputation && GetExpansionSettings().GetHardline().UseItemRarityForMarketPurchase)
		{
			int i;
			PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
			array<string> removedAtts = {};
			array<string> atts = m_Preset.GetAttachments();
			
			CF_Localiser title = new CF_Localiser("STR_EXPANSION_HARDLINE_MARKET_REPLOW");
			CF_Localiser text;
			
			for (i = atts.Count() - 1; i >= 0; --i)
			{
				string att = atts[i];
				ExpansionHardlineItemRarity rarity = GetExpansionSettings().GetHardline().GetItemRarityByType(att);
				if (rarity && !m_MarketMenuItemManager.GetMarketMenu().GetMarketModule().HasRepForRarity(player, rarity))
				{
					removedAtts.Insert(att);
					atts.Remove(i);
				}
			}
			
			if (removedAtts.Count() > 0)
			{
				string removedText;
				foreach (string removedAtt: removedAtts)
				{
					string displayName = ExpansionStatic.GetItemDisplayNameWithType(removedAtt);
					if (!removedText)
					{
						removedText = displayName;
					}
					else
					{
						removedText = removedText + ", " + displayName;
					}
				}

				//! TODO: translation
				text = new CF_Localiser("The following items have been removed from the preset because you don't have the required reputation to use them: " + removedText);
				ExpansionNotification(title, text, EXPANSION_NOTIFICATION_ICON_INFO, COLOR_EXPANSION_NOTIFICATION_INFO, 7, ExpansionNotificationType.MARKET).Create();
			}
		}
		
		super.OnElementButtonClick();
	}
};
#endif
