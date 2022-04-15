/**
 * ExpansionATMMenuPlayerEntry.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionATMMenuPlayerEntry: ExpansionScriptView
{
	protected ref ExpansionATMMenu m_ATMMenu;
	protected ref ExpansionATMMenuPlayerEntryController m_ATMMenuPlayerEntryController;
	
	ref SyncPlayer m_Player;
	
	protected Widget player_element_button_background;
	//protected ButtonWidget player_element_button;
	protected TextWidget player_element_text;
	protected Widget player_element_button_highlight;
	
	private bool m_Selected = false;
	
	// ------------------------------------------------------------
	// ExpansionATMMenuPlayerEntry Constructor
	// ------------------------------------------------------------	
	void ExpansionATMMenuPlayerEntry(ExpansionATMMenu atm, SyncPlayer player)
	{
		m_ATMMenu = atm;
		m_Player = player;
		
		if (!m_ATMMenuPlayerEntryController)
			m_ATMMenuPlayerEntryController = ExpansionATMMenuPlayerEntryController.Cast(GetController());
				
		SetView();
	}

	// ------------------------------------------------------------
	// ExpansionATMMenuPlayerEntry GetLayoutFile
	// ------------------------------------------------------------
	override string GetLayoutFile() 
	{
		return "DayZExpansion/Market/GUI/layouts/atm/expansion_atm_menu_player_entry.layout";
	}
	
	// ------------------------------------------------------------
	// ExpansionATMMenuPlayerEntry GetControllerType
	// ------------------------------------------------------------
	override typename GetControllerType() 
	{
		return ExpansionATMMenuPlayerEntryController;
	}
	
	// ------------------------------------------------------------
	// ExpansionATMMenuPlayerEntry SetView
	// ------------------------------------------------------------	
	void SetView()
	{
		player_element_button_background.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorLabels"));
		//player_element_button.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("ColorSetQuantityButton"));
		player_element_text.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorText"));
		
		m_ATMMenuPlayerEntryController.PlayerName = m_Player.m_PlayerName;
		m_ATMMenuPlayerEntryController.NotifyPropertyChanged("PlayerName");
	}
	
	// ------------------------------------------------------------
	// ExpansionATMMenuPlayerEntry OnPlayerEntryButtonClick
	// ------------------------------------------------------------	
	void OnPlayerEntryButtonClick()
	{
		if (!m_Selected)
		{
			SetSelected(true);
			m_ATMMenu.SetPlayer(m_Player, this);
		}
		else
		{
			SetSelected(false);
			m_ATMMenu.SetPlayer(NULL, this);
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionATMMenuPlayerEntry SetSelected
	// ------------------------------------------------------------	
	void SetSelected(bool state)
	{
		m_Selected = state;
		player_element_button_highlight.Show(state);
	}
	
	// ------------------------------------------------------------
	// ExpansionATMMenuPlayerEntry IsSelected
	// ------------------------------------------------------------	
	bool IsSelected()
	{
		return m_Selected;
	}
}

class ExpansionATMMenuPlayerEntryController: ExpansionViewController
{
	string PlayerName;
};