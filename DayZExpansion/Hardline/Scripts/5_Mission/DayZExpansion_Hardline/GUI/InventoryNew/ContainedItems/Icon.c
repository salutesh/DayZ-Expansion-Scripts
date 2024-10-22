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

modded class CargoContainer
{
	void CargoContainer( LayoutHolder parent, bool is_attachment = false )
	{
		GetExpansionClientSettings().SI_UpdateSetting.Insert(Expansion_OnSettingsUpdated);
		ExpansionSettings.SI_Hardline.Insert(Expansion_OnSettingsUpdated);
	}
	
	void ~CargoContainer()
	{
		GetExpansionClientSettings().SI_UpdateSetting.Remove(Expansion_OnSettingsUpdated);
		ExpansionSettings.SI_Hardline.Remove(Expansion_OnSettingsUpdated);
	}
	
	protected void Expansion_OnSettingsUpdated()
	{
		foreach	(Icon icon: m_Icons)
		{
			icon.Expansion_UpdateItemRarity();
		}
	}
}

modded class Icon
{
	protected ImageWidget m_Expansion_RarityColor;

	void Icon( LayoutHolder parent, bool hands_icon = false )
	{
		m_Expansion_RarityColor = ImageWidget.Cast(GetGame().GetWorkspace().CreateWidgets("DayZExpansion/Hardline/GUI/layouts/expansion_icon_rarity.layout", m_ItemPreview));
		m_Expansion_RarityColor.Show(false);
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
	
	override void Expansion_UpdateItemRarity()
	{
		Expansion_UpdateItemRarityEx(m_Item, m_Expansion_RarityColor, m_HandsIcon);
	}

	override protected void Expansion_ResetColor()
	{
		m_Expansion_RarityColor.SetColor(m_ExpansionBaseColor);
	}
	
	override void InitLock(EntityAI parent, EntityAI obj, int x_pos, int y_pos, bool flip)
	{
		super.InitLock(parent, obj, x_pos, y_pos, flip);
		m_Expansion_RarityColor.Show(false);
	}
};
