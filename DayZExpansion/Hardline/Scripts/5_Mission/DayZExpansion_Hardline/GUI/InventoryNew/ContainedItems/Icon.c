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
	void Icon( LayoutHolder parent, bool hands_icon = false )
	{
		Expansion_UpdateItemRarity();
	}
	
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
	
	override protected void Expansion_UpdateItemRarity()
	{
		Expansion_UpdateItemRarityEx(m_Item, m_ColorWidget);
	}

	override protected void Expansion_ResetColor()
	{
		m_ColorWidget.SetColor(m_ExpansionBaseColor);
	}
};
