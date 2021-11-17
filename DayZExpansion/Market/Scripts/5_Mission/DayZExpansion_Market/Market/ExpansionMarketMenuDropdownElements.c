/**
 * ExpansionMarketMenuDropdownElements.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketMenuDropdownElement_FilterPrimeMag: ExpansionMarketMenuDropdownElement
{
	void ExpansionMarketMenuDropdownElement_FilterPrimeMag(ExpansionMarketMenu menu, StringLocaliser text)
	{
		m_MarketMenu = menu;
		m_Text = text;
	}
	
	override void SetView()
	{
		super.SetView();
	}
	
	override void ApplyFilter()
	{
		m_MarketMenu.SetFilterPrimeMag(IsChecked());
		super.ApplyFilter();
	}
}

class ExpansionMarketMenuDropdownElement_FilterSecondMag: ExpansionMarketMenuDropdownElement
{
	void ExpansionMarketMenuDropdownElement_FilterSecondMag(ExpansionMarketMenu menu, StringLocaliser text)
	{
		m_MarketMenu = menu;
		m_Text = text;
	}
	
	override void SetView()
	{
		super.SetView();
	}
	
	override void ApplyFilter()
	{
		m_MarketMenu.SetFilterSecondMag(IsChecked());
		super.ApplyFilter();
	}
}

class ExpansionMarketMenuDropdownElement_FilterHandMag: ExpansionMarketMenuDropdownElement
{
	void ExpansionMarketMenuDropdownElement_FilterHandMag(ExpansionMarketMenu menu, StringLocaliser text)
	{
		m_MarketMenu = menu;
		m_Text = text;
	}
	
	override void SetView()
	{
		super.SetView();
	}
	
	override void ApplyFilter()
	{
		m_MarketMenu.SetFilterHandMag(IsChecked());
		super.ApplyFilter();
	}
}

class ExpansionMarketMenuDropdownElement_FilterPrimeAttach: ExpansionMarketMenuDropdownElement
{
	void ExpansionMarketMenuDropdownElement_FilterPrimeAttach(ExpansionMarketMenu menu, StringLocaliser text)
	{
		m_MarketMenu = menu;
		m_Text = text;
	}
	
	override void SetView()
	{
		super.SetView();
	}
	
	override void ApplyFilter()
	{
		m_MarketMenu.SetFilterPrimeAttach(IsChecked());
		super.ApplyFilter();
	}
}

class ExpansionMarketMenuDropdownElement_FilterSecondAttach: ExpansionMarketMenuDropdownElement
{
	void ExpansionMarketMenuDropdownElement_FilterSecondAttach(ExpansionMarketMenu menu, StringLocaliser text)
	{
		m_MarketMenu = menu;
		m_Text = text;
	}
	
	override void SetView()
	{
		super.SetView();
	}
	
	override void ApplyFilter()
	{
		m_MarketMenu.SetFilterSecondAttach(IsChecked());
		super.ApplyFilter();
		
	}
}

class ExpansionMarketMenuDropdownElement_FilterHandAttach: ExpansionMarketMenuDropdownElement
{
	void ExpansionMarketMenuDropdownElement_FilterHandAttach(ExpansionMarketMenu menu, StringLocaliser text)
	{
		m_MarketMenu = menu;
		m_Text = text;
	}
	
	override void SetView()
	{
		super.SetView();
	}
	
	override void ApplyFilter()
	{
		m_MarketMenu.SetFilterHandAttach(IsChecked());
		super.ApplyFilter();
	}
}

class ExpansionMarketMenuDropdownElement_FilterPrimeBullet: ExpansionMarketMenuDropdownElement
{
	void ExpansionMarketMenuDropdownElement_FilterPrimeBullet(ExpansionMarketMenu menu, StringLocaliser text)
	{
		m_MarketMenu = menu;
		m_Text = text;
	}
	
	override void SetView()
	{
		super.SetView();
	}
	
	override void ApplyFilter()
	{
		m_MarketMenu.SetFilterPrimeBullet(IsChecked());
		super.ApplyFilter();
	}
}

class ExpansionMarketMenuDropdownElement_FilterSecondBullet: ExpansionMarketMenuDropdownElement
{
	void ExpansionMarketMenuDropdownElement_FilterSecondBullet(ExpansionMarketMenu menu, StringLocaliser text)
	{
		m_MarketMenu = menu;
		m_Text = text;
	}
	
	override void SetView()
	{
		super.SetView();
	}
	
	override void ApplyFilter()
	{
		m_MarketMenu.SetFilterSecondBullet(IsChecked());
		super.ApplyFilter();
	}
}

class ExpansionMarketMenuDropdownElement_FilterHandBullet: ExpansionMarketMenuDropdownElement
{
	void ExpansionMarketMenuDropdownElement_FilterHandBullet(ExpansionMarketMenu menu, StringLocaliser text)
	{
		m_MarketMenu = menu;
		m_Text = text;
	}
	
	override void SetView()
	{
		super.SetView();
	}
	
	override void ApplyFilter()
	{
		m_MarketMenu.SetFilterHandBullet(IsChecked());
		super.ApplyFilter();
	}
}

class ExpansionMarketMenuDropdownElement: ExpansionScriptView
{
	ref ExpansionMarketMenu m_MarketMenu;
	ref ExpansionMarketMenuDropdownElementController m_ElementController;
	
	StringLocaliser m_Text;
	
	protected TextWidget dropdown_element_text;
	protected CheckBoxWidget dropdown_element_checkbox;
	protected ImageWidget dropdown_element_background;

	bool m_Update;
	
	void ExpansionMarketMenuDropdownElement(ExpansionMarketMenu menu, StringLocaliser text)
	{
		m_MarketMenu = menu;
		m_Text = text;
		
		if (!m_ElementController)
			m_ElementController = ExpansionMarketMenuDropdownElementController.Cast(GetController());
			
		dropdown_element_text.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.BaseColorText));
		dropdown_element_checkbox.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.BaseColorCheckboxes));
		dropdown_element_background.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.BaseColorLabels));
		
		SetView();
	}
	
	void ~ExpansionMarketMenuDropdownElement()
	{
		Hide();
	}
	
	override string GetLayoutFile() 
	{
		return "DayZExpansion/Market/GUI/layouts/market/expansion_market_menu_dropdown_element.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionMarketMenuDropdownElementController;
	}
	
	void SetView()
	{
		m_ElementController.Text = m_Text.GetText();
		m_ElementController.NotifyPropertyChanged("Text");
	}
	
	bool IsChecked()
	{
		return m_ElementController.CheckBox;
	}
	
	void SetChecked(bool state, bool update = true)
	{
		m_Update = update;
		m_ElementController.CheckBox = state;
		m_ElementController.NotifyPropertyChanged("CheckBox");
	}
	
	void ApplyFilter()
	{
		EXPrint(ToString() + "::ApplyFilter - update " + m_Update);
		if (m_Update)
		{
			m_MarketMenu.SetDropdownState();
			m_MarketMenu.UpdateOptionFilterStrings();
			m_MarketMenu.UpdateMarketCategories();
		}
		else
		{
			m_Update = true;
		}
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		switch (w)
		{
		case dropdown_element_checkbox:
			dropdown_element_checkbox.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.ColorSetQuantityButton));
			break;
		}
		
		return super.OnMouseEnter(w, x, y);
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		switch (w)
		{
		case dropdown_element_checkbox:
			dropdown_element_checkbox.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.BaseColorText));
			break;
		}
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
}

class ExpansionMarketMenuDropdownElementController: ExpansionViewController
{
	string Text;
	bool CheckBox;
	bool WasChecked;
	
	// ------------------------------------------------------------
	// ExpansionMarketMenuDropdownElementController PropertyChanged
	// ------------------------------------------------------------
	override void PropertyChanged(string property_name)
	{
		if (property_name == "CheckBox" && CheckBox != WasChecked)
		{
			EXPrint("ExpansionMarketMenuDropdownElementController::PropertyChanged - CheckBox " + CheckBox + " " + Text);
			WasChecked = CheckBox;
						
			ExpansionMarketMenuDropdownElement dropdownElement = ExpansionMarketMenuDropdownElement.Cast(GetParent());
			if (dropdownElement)
			{
				dropdownElement.ApplyFilter();
			}
		}
	}
}