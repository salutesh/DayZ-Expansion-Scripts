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
	void ExpansionItemTooltip(EntityAI item)
	{
		
	}
	
	void ~ExpansionItemTooltip()
	{
		
	}
	
    void UpdateItemRarity()
	{
		ItemBase itemBase;
		if (!Class.CastTo(itemBase, m_Item))
			return;
		
		ExpansionHardlineItemRarity rarity = itemBase.Expansion_GetRarity();
		string text;
		int color;
        switch (rarity)
        {
            case ExpansionHardlineItemRarity.MYTHIC:
            {
                color = EXPASNION_ITEM_COLOR_MYTHIC;
                text = "Mythic";
                break;
            }
            case ExpansionHardlineItemRarity.EXOTIC:
            {
                color = EXPASNION_ITEM_COLOR_EXOTIC;
                text = "Exotic";
                break;
            }
            case ExpansionHardlineItemRarity.LEGENDARY:
            {
                color = EXPASNION_ITEM_COLOR_LEGENDARY;
                text = "Legendary";
                break;
            }
            case ExpansionHardlineItemRarity.EPIC:
            {
                color = EXPASNION_ITEM_COLOR_EPIC;
                text = "Epic";
                break;
            }
            case ExpansionHardlineItemRarity.RARE:
            {
                color = EXPASNION_ITEM_COLOR_RARE;
                text = "Rare";
                break;
            }
            case ExpansionHardlineItemRarity.UNCOMMON:
            {
                color = EXPASNION_ITEM_COLOR_UNCOMMON;
                text = "Uncommon";
                break;
            }
            case ExpansionHardlineItemRarity.COMMON:
            {
                color = EXPASNION_ITEM_COLOR_COMMON;
                text = "Common";
                break;
            }
			case ExpansionHardlineItemRarity.NONE:
            {
                return;
            }
        }

		Print(ToString() + "UpdateItemRarity - Type:" + itemBase.GetType());
		Print(ToString() + "UpdateItemRarity - Rarity:" + rarity);
        Print(ToString() + "UpdateItemRarity - Text:" + text);
        Print(ToString() + "UpdateItemRarity - Color:" + color);

        ExpansionItemTooltipStatElement element = new ExpansionItemTooltipStatElement(text, color);
        m_ItemTooltipController.ItemStatsElements.Insert(element);

        Print(ToString() + "UpdateItemRarity - End");
	}
		
    override void Show()
	{
		Print(ToString() + "Show - Start");
		
		if (GetExpansionSettings().GetHardline().UseItemRarity)				
			UpdateItemRarity();

		super.Show();
		
		Print(ToString() + "Show - End");
	}
};
