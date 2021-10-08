/**
 * ExpansionMarketMenuItemTooltipEntry.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketMenuItemTooltipEntryBase: ExpansionScriptView
{
	ref ExpansionMarketMenuItemTooltipEntryBaseController m_TooltipEntryBaseController;
	ref ExpansionMarketMenuItemTooltip m_TooltipElement;
	
	void ExpansionMarketMenuItemTooltip(ExpansionMarketMenuItemTooltip tooltip)
	{
		m_TooltipElement = tooltip;
		
		if (!m_TooltipEntryBaseController)
			m_TooltipEntryBaseController = ExpansionMarketMenuItemTooltipEntryBaseController.Cast(GetController());
	}
	
	override typename GetControllerType() 
	{
		return ExpansionMarketMenuItemTooltipEntryBaseController;
	}
}

class ExpansionMarketMenuItemTooltipEntryBaseController: ExpansionViewController
{

}

class ExpansionMarketMenuItemTooltipEntryItemInfo: ExpansionMarketMenuItemTooltipEntryBase
{
	ref ExpansionMarketMenuItemTooltipEntryItemInfoController m_TooltipEntryController;

	protected ImageWidget tooltip_icon;
	protected TextWidget tooltip_entry_title;
	protected TextWidget tooltip_entry_text;
	
	void ExpansionMarketMenuItemTooltipEntryItemInfo(ExpansionMarketMenuItemTooltip tooltip)
	{
		m_TooltipElement = tooltip;
		
		if (!m_TooltipEntryController)
			m_TooltipEntryController = ExpansionMarketMenuItemTooltipEntryItemInfoController.Cast(GetController());
	}
	
	override string GetLayoutFile() 
	{
		return "DayZExpansion/Market/GUI/layouts/market/expansion_market_menu_item_element_tooltip_entry.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionMarketMenuItemTooltipEntryItemInfoController;
	}
	
	void SetText(string text)
	{
		m_TooltipEntryController.EntryText = text;
		m_TooltipEntryController.NotifyPropertyChanged("EntryText");
	}
	
	void SetIcon(string icon)
	{
		m_TooltipEntryController.EntryIcon = ExpansionIcons.GetPath(icon);
		m_TooltipEntryController.NotifyPropertyChanged("EntryIcon");
	}
	
	void SetColor(int color)
	{
		tooltip_icon.SetColor(color);
		tooltip_entry_text.SetColor(color);
	}
}

class ExpansionMarketMenuItemTooltipEntryItemInfoController: ExpansionViewController
{
	string EntryIcon;
	string EntryText;
}