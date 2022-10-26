/**
 * ExpansionMarketMenuSkinsDropdownElement.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketMenuSkinsDropdownElement: ExpansionScriptView
{
	ref ExpansionMarketMenu m_MarketMenu;
	ref ExpansionMarketMenuSkinsDropdownElementController m_ElementController;

	int m_SkinIndex;
	string m_SkinName;
	string m_ClassName;
	
	protected ButtonWidget dropdown_element_button;
	
	EntityAI m_Object;
	protected ref ExpansionItemPreviewTooltip m_ItemTooltip;
	
	void ExpansionMarketMenuSkinsDropdownElement(ExpansionMarketMenu menu, int skinIndex, string skinName, string className = "")
	{
		m_MarketMenu = menu;
		m_SkinIndex = skinIndex;
		m_SkinName = skinName;
		m_ClassName = className;
		
		if (!m_ElementController)
			m_ElementController = ExpansionMarketMenuSkinsDropdownElementController.Cast(GetController());
		
		SetView();
	}
	
	void ~ExpansionMarketMenuSkinsDropdownElement()
	{
		if (m_Object)
			GetGame().ObjectDelete(m_Object);
	}
	
	override string GetLayoutFile() 
	{
		return "DayZExpansion/Market/GUI/layouts/market/expansion_market_menu_skins_dropdown_element.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionMarketMenuSkinsDropdownElementController;
	}
	
	void SetView()
	{
		m_ElementController.Text = m_SkinName;
		m_ElementController.NotifyPropertyChanged("Text");
		
		ExpansionMarketMenu.CreatePreviewObject(m_ClassName, m_Object);
	}
			
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		switch (w)
		{
			case dropdown_element_button:
			{
				dropdown_element_button.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("ColorFilterOptionsButton"));
				if (!m_ItemTooltip && m_Object)
				{
					m_ItemTooltip = new ExpansionItemPreviewTooltip(m_Object);
					m_ItemTooltip.SetContentOffset(-0.136718, -0.131944);
					m_ItemTooltip.Show();
				}
				break;
			}
		}
		
		return super.OnMouseEnter(w, x, y);
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		switch (w)
		{
			case dropdown_element_button:
			{
				dropdown_element_button.SetColor(ARGB(0, 0, 0, 0));
				if (m_ItemTooltip)
				{
					m_ItemTooltip = null;
				}
				break;
			}
		}
		return super.OnMouseLeave(w, enterW, x, y);
	}
	
	void OnElementButtonClick()
	{
		if (m_MarketMenu)
		{
			m_MarketMenu.SetSkinSelectorState();
			m_MarketMenu.GetSelectedMarketItemElement().UpdateSelectedVariantOrSkin(m_ClassName, m_SkinIndex);
		}
	}
}

class ExpansionMarketMenuSkinsDropdownElementController: ExpansionViewController
{
	string Text;
}