/**
 * ExpansionMarketMenuTooltip.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketMenuTooltip: ExpansionScriptView
{
	ref ExpansionMarketMenuTooltipController m_TooltipController;
			
	protected ImageWidget tooltip_icon;
	protected Widget tooltip_header;
	protected Widget tooltip_content;
	protected HtmlWidget tooltip_title;
	protected HtmlWidget tooltip_text;
	protected WrapSpacerWidget spacer_content;
	protected float m_ContentOffsetX;
	protected float m_ContentOffsetY;
	
	void ExpansionMarketMenuTooltip()
	{		
		if (!m_TooltipController)
			m_TooltipController = ExpansionMarketMenuTooltipController.Cast(GetController());
		
		tooltip_icon.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("ColorItemInfoIcon"));
		tooltip_header.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorTooltipsHeaders"));
		tooltip_content.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorTooltipsBackground"));
		tooltip_title.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("ColorItemInfoAttachments"));
		tooltip_text.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("ColorItemInfoAttachments"));
	}
	
	void ~ExpansionMarketMenuTooltip()
	{
		m_TooltipController.SpacerEntries.Clear();
	}
	
	override string GetLayoutFile() 
	{
		return "DayZExpansion/Market/GUI/layouts/market/expansion_market_menu_tooltip.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionMarketMenuTooltipController;
	}
	
	void SetTitle(string title)
	{
		m_TooltipController.TooltipTitle = title;
		m_TooltipController.NotifyPropertyChanged("TooltipTitle");
	}
	
	void SetText(string text)
	{
		m_TooltipController.TooltipText = text;
		m_TooltipController.NotifyPropertyChanged("TooltipText");
	}
	
	void ClearEntries()
	{
		m_TooltipController.SpacerEntries.Clear();
	}
	
	void AddEntry(string label)
	{
		ExpansionMarketMenuTooltipEntry entry = new ExpansionMarketMenuTooltipEntry(label);			
		m_TooltipController.SpacerEntries.Insert(entry);
	}
	
	void SetContentOffset(float x, float y)
	{
		m_ContentOffsetX = x;
		m_ContentOffsetY = y;
	}
	
	override void OnHide()
	{
		tooltip_content.Show(false);
		spacer_content.Show(false);
		
		for (int i = 0; i < m_TooltipController.SpacerEntries.Count(); i++)
		{
			ExpansionMarketMenuTooltipEntry entry = m_TooltipController.SpacerEntries[i];
			entry.Hide();
		}
	}
	
	override void OnShow()
	{
		super.OnShow();

		if (m_TooltipController.TooltipText != "")
		{
			tooltip_content.Show(true);
			tooltip_text.Show(true);
		}
		
		if (m_TooltipController.SpacerEntries.Count() > 0)
		{
			tooltip_content.Show(true);
			spacer_content.Show(true);
			
			for (int i = 0; i < m_TooltipController.SpacerEntries.Count(); i++)
			{
				ExpansionMarketMenuTooltipEntry entry = m_TooltipController.SpacerEntries[i];
				entry.Show();
			}
		}
		
		int x, y;
		GetGame().GetMousePos(x, y);
		int w, h;
		GetScreenSize(w, h);
		GetLayoutRoot().SetPos(x + w * m_ContentOffsetX, y + h * m_ContentOffsetY, true);
		
		GetLayoutRoot().Update();
	}
}

class ExpansionMarketMenuTooltipController: ExpansionViewController
{
	string TooltipTitle;
	string TooltipText;
	ref ObservableCollection<ref ExpansionMarketMenuTooltipEntry> SpacerEntries = new ObservableCollection<ref ExpansionMarketMenuTooltipEntry>(this);
}