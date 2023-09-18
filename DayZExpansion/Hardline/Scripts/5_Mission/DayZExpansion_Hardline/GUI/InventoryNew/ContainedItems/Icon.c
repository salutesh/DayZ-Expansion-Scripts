/**
 * Icon.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class Icon
{
	override void InitEx(EntityAI obj, bool refresh = true)
	{
		super.InitEx(obj, refresh);

		Expansion_UpdateItemRarity();
	}
	
	override void SetItemPreview()
    {
        super.SetItemPreview();

        Expansion_UpdateItemRarity();
    }
		
	override void FullScreen()
	{
		super.FullScreen();

		Expansion_UpdateItemRarity();
	}
	
	override void DestroyWhiteBackground()
	{
		super.DestroyWhiteBackground();

		Expansion_UpdateItemRarity();
	}
	
	void Expansion_UpdateItemRarity()
	{
		if (m_Item && GetExpansionSettings().GetHardline().EnableItemRarity && GetExpansionSettings().GetHardline().UseItemRarityOnInventoryIcons)
		{
			ExpansionHardlineItemRarity rarity = m_Item.Expansion_GetRarity();
			int color;
			string rarityName = typename.EnumToString(ExpansionHardlineItemRarity, rarity);
			ExpansionStatic.GetVariableIntByName(ExpansionHardlineItemRarityColor, rarityName, color);
			m_ColorWidget.SetColor(color);
		}
	}
};