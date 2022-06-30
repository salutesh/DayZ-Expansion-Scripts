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
	override Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("DayZExpansion/Hardline/GUI/layouts/expansion_inventory_inspect.layout");
		return layoutRoot;
	}

	override void SetItem(EntityAI item)
	{
		InspectMenuNew.UpdateItemInfoRarity(layoutRoot, item);
		
		super.SetItem(item);
	}
	
	static void UpdateItemInfoRarity(Widget root_widget, EntityAI item)
	{
		ImageWidget rarityElement = ImageWidget.Cast(root_widget.FindAnyWidget("ItemRarityWidgetBackground"));
		if (!rarityElement)
			return;

		ItemBase itemBase = ItemBase.Cast(item);
		if (itemBase && GetExpansionSettings().GetHardline().UseItemRarity)
		{
			ExpansionHardlineItemRarity rarity = itemBase.Expansion_GetRarity();
			int color;
			string text;
			
			if (rarity == ExpansionHardlineItemRarity.NONE)
			{
				rarityElement.Show(false);
				return;
			}
			
			switch (rarity)
			{
				case ExpansionHardlineItemRarity.MYTHIC:
				{
					color = EXPASNION_ITEM_COLOR_MYTHIC;
					text = "Mythic";
				}
				break;
				
				case ExpansionHardlineItemRarity.EXOTIC:
				{
					color = EXPASNION_ITEM_COLOR_EXOTIC;
					text = "Exotic";
				}
				break;
				
				case ExpansionHardlineItemRarity.LEGENDARY:
				{
					color = EXPASNION_ITEM_COLOR_LEGENDARY;
					text = "Legendary";
				}
				break;
				
				case ExpansionHardlineItemRarity.EPIC:
				{
					color = EXPASNION_ITEM_COLOR_EPIC;
					text = "Epic";
				}
				break;

				case ExpansionHardlineItemRarity.RARE:
				{
					color = EXPASNION_ITEM_COLOR_RARE;
					text = "Rare";
				}
				break;
				
				case ExpansionHardlineItemRarity.UNCOMMON:
				{
					color = EXPASNION_ITEM_COLOR_UNCOMMON;
					text = "Uncommon";
				}
				break;
				
				case ExpansionHardlineItemRarity.COMMON:
				{
					color = EXPASNION_ITEM_COLOR_COMMON;
					text = "Common";
				}
				break;
				
				case ExpansionHardlineItemRarity.POOR:
				{
					color = EXPASNION_ITEM_COLOR_POOR;
					text = "Poor";
				}
				break;
			}

			rarityElement.Show(true);
			WidgetTrySetText(root_widget, "ItemRarityWidget", text, color);

			return;
		}

		rarityElement.Show(false);
	}
};
