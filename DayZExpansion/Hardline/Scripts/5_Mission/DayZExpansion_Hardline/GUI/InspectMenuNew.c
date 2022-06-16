/**
 * InspectMenuNew.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class InspectMenuNew
{
	private ExpansionHardlineModule m_HardlineModule;
	override Widget Init()
	{
		if (GetExpansionSettings().GetHardline().UseItemRarity)
		{
			layoutRoot = GetGame().GetWorkspace().CreateWidgets("DayZExpansion/Hardline/GUI/layouts/expansion_inventory_inspect.layout");
			return layoutRoot;
		}
		
		return super.Init();
	}

	override void SetItem(EntityAI item)
	{
		if (item && GetExpansionSettings().GetHardline().UseItemRarity)
			InspectMenuNew.UpdateItemInfoRarity(layoutRoot, item);
		
		super.SetItem(item);
	}
	
	static void UpdateItemInfoRarity(Widget root_widget, EntityAI item)
	{
		ItemBase itemBase = ItemBase.Cast(item);
		if (itemBase && itemBase.GetRarity() != -1)
		{
			int rarity;
			int color;
			string text;
			string type = item.GetType();
			type.ToLower();
			ExpansionHardlineItemData itemData = GetExpansionSettings().GetHardline().GetHardlineItemDataByType(type);
			if (!itemData)
				rarity = ExpansionHardlineItemRarity.COMMON;
			else
				rarity = itemData.GetRarity();
			
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
				case ExpansionHardlineItemRarity.POOR:
				{
					color = EXPASNION_ITEM_COLOR_POOR;
					text = "Poor";
					break;
				}
			}
		}

		WidgetTrySetText(root_widget, "ItemRarityWidget", text, color);
	}
};