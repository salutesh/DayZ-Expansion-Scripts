/**
 * ExpansionMenuDialog_MarketSetQuantity.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMenuDialog_MarketSetQuantity: ExpansionMenuDialogBase
{
	ref ExpansionMenuDialogContent_Text m_Text;
	ref ExpansionMenuDialogContent_Editbox m_Editbox;
	ref ExpansionMenuDialogButton_Text_MarketSetQuantity_Accept m_AcceptButton;
	ref ExpansionMenuDialogButton_Text_MarketSetQuantity_Cancel m_CancelButton;
	
	ref ExpansionMarketMenu m_MarketMenu;
		
	// ------------------------------------------------------------
	// ExpansionMenuDialog_MarketSetQuantity Constructor
	// ------------------------------------------------------------	
	void ExpansionMenuDialog_MarketSetQuantity(ExpansionScriptViewMenu parentMenu)
	{
		m_ParentMenu = parentMenu;
		
		if (!m_MarketMenu)
			m_MarketMenu = ExpansionMarketMenu.Cast(m_ParentMenu);
		
		if (!m_Text)
		{
			m_Text = new ExpansionMenuDialogContent_Text(this);
			AddContent(m_Text);
			m_Text.SetText("#STR_EXPANSION_MARKET_QUANTITY_DIALOG_TEXT");
			m_Text.Show();
		}
		
		if (!m_Editbox)
		{
			m_Editbox = new ExpansionMenuDialogContent_Editbox(this);
			AddContent(m_Editbox);
			m_Editbox.Show();
			
			m_Editbox.SetEditboxText("");
		}
		
		if (!m_AcceptButton)
		{
			m_AcceptButton = new ExpansionMenuDialogButton_Text_MarketSetQuantity_Accept(this);
			AddButton(m_AcceptButton);
			m_AcceptButton.Show();
		}
		
		if (!m_CancelButton)
		{
			m_CancelButton = ExpansionMenuDialogButton_Text_MarketSetQuantity_Cancel(this);
			AddButton(m_CancelButton);
			m_CancelButton.Show();
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionMenuDialog_MarketSetQuantity GetDialogTitle
	// ------------------------------------------------------------	
	override string GetDialogTitle()
	{
		return "#STR_EXPANSION_MARKET_QUANTITY_DIALOG_TITLE";
	}
	
	// ------------------------------------------------------------
	// ExpansionMenuDialog_MarketSetQuantity GetEditbox
	// ------------------------------------------------------------	
	ExpansionMenuDialogContent_Editbox GetEditbox()
	{
		return m_Editbox;
	}
	
	// ------------------------------------------------------------
	// ExpansionMenuDialog_MarketSetQuantity GetMarketMenu
	// ------------------------------------------------------------	
	ExpansionMarketMenu GetMarketMenu()
	{
		return m_MarketMenu;
	}
};

class ExpansionMenuDialogButton_Text_MarketSetQuantity_Accept: ExpansionMenuDialogButton_Text
{
	ref ExpansionMenuDialog_MarketSetQuantity m_SetQuantityDialog;
	
	// ------------------------------------------------------------
	// ExpansionMenuDialogButton_Text_MarketSetQuantity_Accept Constructor
	// ------------------------------------------------------------
	void ExpansionMenuDialogButton_Text_MarketSetQuantity_Accept(ExpansionMenuDialogBase dialog)
	{
		m_Dialog = dialog;		
		
		if (!m_SetQuantityDialog)
			m_SetQuantityDialog = ExpansionMenuDialog_MarketSetQuantity.Cast(m_Dialog);
		
		SetButtonText("#STR_EXPANSION_MARKET_QUANTITY_ACCEPT");
	}
	
	// ------------------------------------------------------------
	// ExpansionMenuDialogButton_Text_MarketSetQuantity_Accept OnButtonClick
	// ------------------------------------------------------------	
	override void OnButtonClick()
	{
		if (m_SetQuantityDialog.GetEditbox().GetEditboxText() != "")
		{
			string quantity = m_SetQuantityDialog.GetEditbox().GetEditboxText();
			TStringArray allNumbers = {"0","1","2","3","4","5","6","7","8","9"};
			for (int i = 0; i < quantity.Length(); i++)
			{
				if (allNumbers.Find(quantity.Get(i)) == -1)
				{
					ExpansionNotification("STR_EXPANSION_MARKET_TITLE", "STR_EXPANSION_MARKET_QUANTITY_ERROR_NUMBERS").Error();
					return;
				}
			}
			
			m_SetQuantityDialog.GetMarketMenu().SetQuantity(quantity);
		}
	}
};

class ExpansionMenuDialogButton_Text_MarketSetQuantity_Cancel: ExpansionMenuDialogButton_Text
{
	ref ExpansionMenuDialog_MarketSetQuantity m_SetQuantityDialog;
	
	// ------------------------------------------------------------
	// ExpansionMenuDialogButton_Text_MarketSetQuantity_Cancel Constructor
	// ------------------------------------------------------------	
	void ExpansionMenuDialogButton_Text_MarketSetQuantity_Cancel(ExpansionMenuDialogBase dialog)
	{
		m_Dialog = dialog;
		
		if (!m_SetQuantityDialog)
			m_SetQuantityDialog = ExpansionMenuDialog_MarketSetQuantity.Cast(m_Dialog);
		
		SetButtonText("#STR_EXPANSION_MARKET_QUANTITY_CANCLE");
	}
	
	// ------------------------------------------------------------
	// ExpansionMenuDialogButton_Text_MarketSetQuantity_Cancel OnButtonClick
	// ------------------------------------------------------------
	override void OnButtonClick()
	{
		m_SetQuantityDialog.Hide();
		m_SetQuantityDialog.GetMarketMenu().SetMenuState(ExpansionMarketMenuState.NONE);
	}
};