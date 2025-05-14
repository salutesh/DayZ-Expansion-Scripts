/**
 * ExpansionP2PMarketMenuItemTooltipEntryItemInfo.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2025 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionP2PMarketMenuItemTooltipEntryItemInfo: ExpansionScriptView
{
	protected ref ExpansionP2PMarketMenuItemTooltipEntryItemInfoController m_TooltipEntryController;
	protected ExpansionP2PMarketMenuItemTooltip m_TooltipElement;
	
	protected ImageWidget tooltip_icon;
	protected TextWidget tooltip_entry_title;
	protected TextWidget tooltip_entry_text;
	
	void ExpansionP2PMarketMenuItemTooltipEntryItemInfo(ExpansionP2PMarketMenuItemTooltip tooltip)
	{
		m_TooltipElement = tooltip;
		
		if (!m_TooltipEntryController)
			m_TooltipEntryController = ExpansionP2PMarketMenuItemTooltipEntryItemInfoController.Cast(GetController());
	}
	
	override string GetLayoutFile() 
	{
		return "DayZExpansion/P2PMarket/GUI/layouts/expansion_p2pmarket_item_element_tooltip_entry.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionP2PMarketMenuItemTooltipEntryItemInfoController;
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
};

class ExpansionP2PMarketMenuItemTooltipEntryItemInfoController: ExpansionViewController
{
	string EntryIcon;
	string EntryText;
};