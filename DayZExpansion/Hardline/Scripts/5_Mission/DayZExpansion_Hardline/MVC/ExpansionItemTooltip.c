/**
 * ExpansionItemTooltip.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ExpansionItemTooltip
{	
	override void UpdateItemStats()
	{
		super.UpdateItemStats();
		
		if (GetExpansionSettings().GetHardline().EnableItemRarity)
			UpdateItemRarity();
	}
	
    void UpdateItemRarity()
	{
		ItemBase itemBase;
		if (Class.CastTo(itemBase, m_Item))
		{	
			ExpansionHardlineItemRarity rarity = itemBase.Expansion_GetRarity();
			if (rarity == ExpansionHardlineItemRarity.NONE)
				return;
	
			string rarityName = typename.EnumToString(ExpansionHardlineItemRarity, rarity);
			string text = "#" + "STR_EXPANSION_HARDLINE_" + rarityName;
			int color;
			typename type = ExpansionHardlineItemRarityColor;
			ExpansionStatic.GetVariableIntByName(type, rarityName, color);
	
	        ExpansionItemTooltipStatElement element = new ExpansionItemTooltipStatElement(text, color);
	        m_ItemTooltipController.ItemStatsElements.Insert(element);
		}
	}
};
