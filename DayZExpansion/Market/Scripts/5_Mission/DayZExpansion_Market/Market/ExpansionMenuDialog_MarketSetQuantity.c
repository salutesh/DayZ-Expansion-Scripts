/**
 * ExpansionMenuDialog_MarketSetQuantity.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMenuDialog_MarketSetQuantity: ExpansionDialogBase
{
	protected ref ExpansionDialogContent_Text m_Text;
	protected ref ExpansionDialogContent_Editbox m_Editbox;
	protected ref ExpansionMenuDialogButton_Text_MarketSetQuantity_Accept m_AcceptButton;
	protected ref ExpansionMenuDialogButton_Text_MarketSetQuantity_Cancel m_CancelButton;
	protected ref ExpansionMarketMenu m_MarketMenu;

	void ExpansionMenuDialog_MarketSetQuantity(ScriptView parentView)
	{
		m_ParentView = parentView;
		
		if (!m_MarketMenu)
			m_MarketMenu = ExpansionMarketMenu.Cast(m_ParentView);
		
		ExpansionDialogContentSpacer spacer;
		spacer = new ExpansionDialogContentSpacer(this);
		AddContent(spacer);
		
		if (!m_Text)
		{
			m_Text = new ExpansionDialogContent_Text(this);
			AddContent(m_Text);
			m_Text.SetText("#STR_EXPANSION_MARKET_QUANTITY_DIALOG_TEXT");
			m_Text.Show();
		}
		
		spacer = new ExpansionDialogContentSpacer(this);
		AddContent(spacer);
		
		if (!m_Editbox)
		{
			m_Editbox = new ExpansionDialogContent_Editbox(this);
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

	override string GetDialogTitle()
	{
		return "#STR_EXPANSION_MARKET_QUANTITY_DIALOG_TITLE";
	}

	ExpansionDialogContent_Editbox GetEditbox()
	{
		return m_Editbox;
	}

	ExpansionMarketMenu GetMarketMenu()
	{
		return m_MarketMenu;
	}
};

class ExpansionMenuDialogButton_Text_MarketSetQuantity_Accept: ExpansionDialogButton_Text
{
	protected ref ExpansionMenuDialog_MarketSetQuantity m_SetQuantityDialog;

	void ExpansionMenuDialogButton_Text_MarketSetQuantity_Accept(ExpansionDialogBase dialog)
	{
		m_Dialog = dialog;		
		
		if (!m_SetQuantityDialog)
			m_SetQuantityDialog = ExpansionMenuDialog_MarketSetQuantity.Cast(m_Dialog);
		
		SetButtonText("#STR_EXPANSION_MARKET_QUANTITY_ACCEPT");
	}

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

class ExpansionMenuDialogButton_Text_MarketSetQuantity_Cancel: ExpansionDialogButton_Text
{
	protected ref ExpansionMenuDialog_MarketSetQuantity m_SetQuantityDialog;

	void ExpansionMenuDialogButton_Text_MarketSetQuantity_Cancel(ExpansionDialogBase dialog)
	{
		m_Dialog = dialog;
		
		if (!m_SetQuantityDialog)
			m_SetQuantityDialog = ExpansionMenuDialog_MarketSetQuantity.Cast(m_Dialog);
		
		SetButtonText("#STR_EXPANSION_MARKET_QUANTITY_CANCLE");
	}

	override void OnButtonClick()
	{
		m_SetQuantityDialog.Hide();
		m_SetQuantityDialog.GetMarketMenu().SetMenuState(ExpansionMarketMenuState.NONE);
	}
};