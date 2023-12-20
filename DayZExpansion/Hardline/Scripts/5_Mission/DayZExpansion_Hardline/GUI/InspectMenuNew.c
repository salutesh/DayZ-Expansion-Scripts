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
		if (Expansion_UseCustomLayout())
		{
			layoutRoot = GetGame().GetWorkspace().CreateWidgets(Expansion_GetInspectionMenuLayout());
		}
		else
		{
			layoutRoot = super.Init();
		}
		
		return layoutRoot;
	}

	override void SetItem(EntityAI item)
	{
		if (Expansion_UseCustomLayout())
		{
			InspectMenuNew.Expansion_UpdateItemInfoRarity(layoutRoot, item);
		}
		
		super.SetItem(item);
	}

	string Expansion_GetInspectionMenuLayout()
	{
		return "DayZExpansion/Hardline/GUI/layouts/expansion_inventory_inspect.layout";
	}
	
	static bool Expansion_UseCustomLayout()
	{
		return GetExpansionSettings().GetHardline().EnableItemRarity;
	}

	static void Expansion_UpdateItemInfoRarity(Widget root_widget, EntityAI item)
	{
		ImageWidget rarityElement = ImageWidget.Cast(root_widget.FindAnyWidget("ItemRarityWidgetBackground"));
		if (!rarityElement)
			return;

		ItemBase itemBase = ItemBase.Cast(item);
		if (itemBase && Expansion_UseCustomLayout())
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
			ExpansionStatic.GetVariableIntByName(ExpansionHardlineItemRarityColor, rarityName, color);
			rarityElement.Show(true);
			WidgetTrySetText(root_widget, "ItemRarityWidget", text, color);
			return;
		}

		rarityElement.Show(false);
	}
};
