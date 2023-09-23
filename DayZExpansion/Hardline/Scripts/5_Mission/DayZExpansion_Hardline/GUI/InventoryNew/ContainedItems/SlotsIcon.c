/**
 * SlotsIcon.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class SlotsIcon
{
	protected int m_ExpansionBaseColor = -1;
	protected bool m_ExpansionUseRarityColor;

	void SlotsIcon(LayoutHolder parent, Widget root, int index, EntityAI slot_parent) 
	{
		GetExpansionClientSettings().SI_UpdateSetting.Insert(OnClientSettingChanged);
		
		if (GetExpansionClientSettings().RarityColorToggle)
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(this.Expansion_UpdateItemRarity, 100, false);
	}
	
	override void Init(EntityAI obj, bool reservation = false)
	{
		super.Init(obj, reservation);

		if (GetExpansionClientSettings().RarityColorToggle)
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(this.Expansion_UpdateItemRarity, 100, false);
	}
	
	override void SetItemPreview() 
	{
		super.SetItemPreview();
		
		if (GetExpansionClientSettings().RarityColorToggle)
			Expansion_UpdateItemRarity();
	}
	
	protected void Expansion_UpdateItemRarity()
	{
		if (m_Item && GetExpansionSettings().GetHardline().EnableItemRarity && GetExpansionSettings().GetHardline().UseItemRarityOnInventoryIcons)
		{
			if (m_ExpansionBaseColor == -1)
				m_ExpansionBaseColor = m_ColWidget.GetColor();
			
			ExpansionHardlineItemRarity rarity = m_Item.Expansion_GetRarity();
			int color;
			string rarityName = typename.EnumToString(ExpansionHardlineItemRarity, rarity);
			ExpansionStatic.GetVariableIntByName(ExpansionHardlineItemRarityColor, rarityName, color);
			m_ColWidget.Show(true);
			m_ColWidget.SetColor(color);
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
				m_ColWidget.SetColor(m_ExpansionBaseColor);
				m_ExpansionUseRarityColor = false;
			}
		}
	}
};