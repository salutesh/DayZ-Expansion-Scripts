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
	protected int m_ExpansionBaseColor = -1;
	protected bool m_ExpansionUseRarityColor;
	
	void Icon( LayoutHolder parent, bool hands_icon = false )
	{
		GetExpansionClientSettings().SI_UpdateSetting.Insert(OnClientSettingChanged);
		
		if (GetExpansionClientSettings().RarityColorToggle)
			Expansion_UpdateItemRarity();
	}
	
	override void InitEx(EntityAI obj, bool refresh = true)
	{
		super.InitEx(obj, refresh);
		
		if (GetExpansionClientSettings().RarityColorToggle)
			Expansion_UpdateItemRarity();
	}
	
	override void SetItemPreview()
    {
        super.SetItemPreview();
		
		if (GetExpansionClientSettings().RarityColorToggle)
        	Expansion_UpdateItemRarity();
    }
		
	override void FullScreen()
	{
		super.FullScreen();
		
		if (GetExpansionClientSettings().RarityColorToggle)
			Expansion_UpdateItemRarity();
	}
	
	override void DestroyWhiteBackground()
	{
		super.DestroyWhiteBackground();
		
		if (GetExpansionClientSettings().RarityColorToggle)
			Expansion_UpdateItemRarity();
	}
	
	void Expansion_UpdateItemRarity()
	{
		if (m_Item && GetExpansionSettings().GetHardline().EnableItemRarity && GetExpansionSettings().GetHardline().UseItemRarityOnInventoryIcons)
		{
			if (m_ExpansionBaseColor == -1)
				m_ExpansionBaseColor = m_ColorWidget.GetColor();
			
			ExpansionHardlineItemRarity rarity = m_Item.Expansion_GetRarity();
			int color;
			string rarityName = typename.EnumToString(ExpansionHardlineItemRarity, rarity);
			ExpansionStatic.GetVariableIntByName(ExpansionHardlineItemRarityColor, rarityName, color);
			m_ColorWidget.SetColor(color);
			m_ExpansionUseRarityColor = true;
		}
	}
	
	void OnClientSettingChanged()
	{
		if (GetExpansionClientSettings().RarityColorToggle && !m_ExpansionUseRarityColor)
		{
			Expansion_UpdateItemRarity();
		}
		else
		{
			if (m_ExpansionBaseColor > -1)
			{
				m_ColorWidget.SetColor(m_ExpansionBaseColor);
				m_ExpansionUseRarityColor = false;
			}
		}
	}
};