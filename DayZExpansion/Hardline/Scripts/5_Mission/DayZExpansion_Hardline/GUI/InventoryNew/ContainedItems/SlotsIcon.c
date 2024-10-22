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

modded class SlotsContainer
{
	void SlotsContainer(LayoutHolder parent, EntityAI slot_parent) 
	{
		GetExpansionClientSettings().SI_UpdateSetting.Insert(Expansion_OnSettingsUpdated);
		ExpansionSettings.SI_Hardline.Insert(Expansion_OnSettingsUpdated);
	}
	
	void ~SlotsContainer()
	{
		GetExpansionClientSettings().SI_UpdateSetting.Remove(Expansion_OnSettingsUpdated);
		ExpansionSettings.SI_Hardline.Remove(Expansion_OnSettingsUpdated);
	}
	
	protected void Expansion_OnSettingsUpdated()
	{
		foreach	(SlotsIcon icon: m_Icons)
		{
			icon.Expansion_UpdateItemRarity();
		}
	}
}

modded class SlotsIcon
{
	protected ImageWidget m_Expansion_RarityColor;
	void SlotsIcon(LayoutHolder parent, Widget root, int index, EntityAI slot_parent) 
	{
		m_Expansion_RarityColor = ImageWidget.Cast(GetGame().GetWorkspace().CreateWidgets("DayZExpansion/Hardline/GUI/layouts/expansion_icon_rarity.layout", m_ItemPreview));
		m_Expansion_RarityColor.Show(false);
	}

	override void Init(EntityAI obj, bool reservation = false)
	{
		super.Init(obj, reservation);

		Expansion_UpdateItemRarity();
	}
	
	override void SetItemPreview() 
	{
		super.SetItemPreview();
		
		Expansion_UpdateItemRarity();
	}
	
	override void ClearRemainSelected()
	{
		super.ClearRemainSelected();
		
		if (!m_Item)
			m_Expansion_RarityColor.Show(false);
	}
	
	override void Expansion_UpdateItemRarity()
	{
		Expansion_UpdateItemRarityEx(m_Item, m_Expansion_RarityColor, m_Reserved);
	}
	
	override protected void Expansion_ResetColor()
	{
		m_Expansion_RarityColor.SetColor(m_ExpansionBaseColor);
	}
};
