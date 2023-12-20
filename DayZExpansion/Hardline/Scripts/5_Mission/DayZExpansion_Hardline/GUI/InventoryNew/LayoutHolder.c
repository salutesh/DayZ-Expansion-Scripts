/**
 * LayoutHolder.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class LayoutHolder
{
	protected int m_ExpansionBaseColor = -1;
	protected bool m_ExpansionUseRarityColor;

	protected void Expansion_UpdateItemRarity()
	{
	}

	protected void Expansion_UpdateItemRarityEx(ItemBase item, Widget colorWidget)
	{
		auto settings = GetExpansionSettings().GetHardline(false);
		if (item && settings.IsLoaded() && settings.EnableItemRarity && settings.UseItemRarityOnInventoryIcons && GetExpansionClientSettings().RarityColorToggle)
		{
			if (m_ExpansionBaseColor == -1)
				m_ExpansionBaseColor = colorWidget.GetColor();
			
			ExpansionHardlineItemRarity rarity = item.Expansion_GetRarity();
			int color;
			string rarityName = typename.EnumToString(ExpansionHardlineItemRarity, rarity);
			ExpansionStatic.GetVariableIntByName(ExpansionHardlineItemRarityColor, rarityName, color);
			colorWidget.SetColor(color);
			m_ExpansionUseRarityColor = true;
		}
	}

	protected void Expansion_ResetColor()
	{
	}
}
