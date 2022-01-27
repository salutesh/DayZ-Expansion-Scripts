/**
 * ExpansionMarketMenuTooltipEntry.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketMenuTooltipEntry: ExpansionScriptView
{
	ref ExpansionMarketMenuTooltipEntryController m_TooltipEntryController;	
	protected RichTextWidget tooltip_entry_text;
	string m_Label;

	void ExpansionMarketMenuTooltipEntry(string label)
	{
		m_Label = label;
		
		if (!m_TooltipEntryController)
			m_TooltipEntryController = ExpansionMarketMenuTooltipEntryController.Cast(GetController());
		
		tooltip_entry_text.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorText"));
		
		SetView();
	}

	override string GetLayoutFile() 
	{
		return "DayZExpansion/Market/GUI/layouts/market/expansion_market_menu_tooltip_entry.layout";
	}

	override typename GetControllerType() 
	{
		return ExpansionMarketMenuTooltipEntryController;
	}

	void SetView()
	{
		string displayName = ExpansionStatic.GetItemDisplayNameWithType(m_Label);
		if (displayName != string.Empty)
		{
			m_TooltipEntryController.EntryText = displayName;
		}
		else
		{
			m_TooltipEntryController.EntryText = m_Label;
		}
		
		m_TooltipEntryController.NotifyPropertyChanged("EntryText");
		tooltip_entry_text.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("ColorItemInfoAttachments"));
	}
}
class ExpansionMarketMenuTooltipEntryController: ExpansionViewController
{
	string EntryText;
}