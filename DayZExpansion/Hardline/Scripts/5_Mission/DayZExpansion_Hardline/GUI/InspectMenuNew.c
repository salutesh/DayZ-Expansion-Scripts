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
		if (itemBase && GetExpansionSettings().GetHardline().EnableItemRarity)
		{
			ExpansionHardlineItemRarity rarity = itemBase.Expansion_GetRarity();
			
			if (rarity == ExpansionHardlineItemRarity.NONE)
			{
				rarityElement.Show(false);
				return;
			}

			string rarityName = typename.EnumToString(ExpansionHardlineItemRarity, rarity);
			string text = "#" + "STR_EXPANSION_HARDLINE_" + rarityName;
			int color;
			typename type = ExpansionHardlineItemRarityColor;
			ExpansionStatic.GetVariableIntByName(type, rarityName, color);

			rarityElement.Show(true);
			WidgetTrySetText(root_widget, "ItemRarityWidget", text, color);

			return;
		}

		rarityElement.Show(false);
	}
};
