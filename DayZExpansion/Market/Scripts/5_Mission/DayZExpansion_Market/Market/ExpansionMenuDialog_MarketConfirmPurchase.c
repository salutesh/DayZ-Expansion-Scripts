/**
 * ExpansionMenuDialog_MarketConfirmPurchase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMenuDialog_MarketConfirmPurchase: ExpansionDialogBase
{
	protected ref ExpansionDialogContent_Text m_Text;
	protected ref ExpansionMenuDialogButton_Text_MarketConfirmPurchase_Accept m_AcceptButton;
	protected ref ExpansionMenuDialogButton_Text_MarketConfirmPurchase_Cancel m_CancelButton;
	protected ref ExpansionMarketMenu m_MarketMenu;
	protected ref ExpansionMarketMenuDialogData m_DialogData;
	
	bool m_KeyInput = false;

	void ExpansionMenuDialog_MarketConfirmPurchase(ScriptView parentView, ExpansionMarketMenuDialogData dialogData)
	{
		m_ParentView = parentView;
		m_DialogData = dialogData;
		
		if (!m_MarketMenu)
			m_MarketMenu = ExpansionMarketMenu.Cast(m_ParentView);
		
		ExpansionDialogContentSpacer spacer;
		spacer = new ExpansionDialogContentSpacer(this);
		AddContent(spacer);
		
		if (!m_Text)
		{
			m_Text = new ExpansionDialogContent_Text(this);
			AddContent(m_Text);
			
			string amount = m_DialogData.Amount.ToString();
			string displayName = ExpansionStatic.GetItemDisplayNameWithType(m_DialogData.ClassName);
			string price = m_MarketMenu.GetDisplayPrice(m_DialogData.Price, false, false, true);
			StringLocaliser text;
			if (!m_DialogData.IncludeAttachments)
			{
				text = new StringLocaliser("STR_EXPANSION_MARKET_PURCHASE_DIALOG_TEXT", amount, displayName, price);
				m_Text.SetText(text.Format());
			}
			else
			{
				text = new StringLocaliser("STR_EXPANSION_MARKET_PURCHASE_DIALOG_TEXT_ATTACHMENTS", amount, displayName, price);
				m_Text.SetText(text.Format());
			}
			
			m_Text.Show();
		}
		
		spacer = new ExpansionDialogContentSpacer(this);
		AddContent(spacer);
				
		if (!m_AcceptButton)
		{
			m_AcceptButton = new ExpansionMenuDialogButton_Text_MarketConfirmPurchase_Accept(this);
			AddButton(m_AcceptButton);
			m_AcceptButton.Show();
		}
		
		if (!m_CancelButton)
		{
			m_CancelButton = ExpansionMenuDialogButton_Text_MarketConfirmPurchase_Cancel(this);
			AddButton(m_CancelButton);
			m_CancelButton.Show();
		}
	}

	override string GetDialogTitle()
	{
		return "#STR_EXPANSION_MARKET_PURCHASE_DIALOG_TITLE";
	}
	
	ExpansionMarketMenu GetMarketMenu()
	{
		return m_MarketMenu;
	}

	override float GetUpdateTickRate()
	{
		return 0.25;
	}

	override void Expansion_Update()
	{
		if (!m_MarketMenu)
			return;

		if ((ExpansionStatic.Key_Y() || ExpansionStatic.Key_Z()) && !m_KeyInput)
		{
			m_MarketMenu.OnConfirmBuyButtonClick();
			m_KeyInput = true;
			return;
		}
		
		if (ExpansionStatic.Key_C() && !m_KeyInput)
		{
			Hide();
			m_MarketMenu.SetMenuState(ExpansionMarketMenuState.NONE);
			m_KeyInput = true;
			return;
		}
	}
};

class ExpansionMenuDialogButton_Text_MarketConfirmPurchase_Accept: ExpansionDialogButton_Text
{
	ref ExpansionMenuDialog_MarketConfirmPurchase m_ConfirmDialog;

	void ExpansionMenuDialogButton_Text_MarketConfirmPurchase_Accept(ExpansionDialogBase dialog)
	{
		m_Dialog = dialog;		
		
		if (!m_ConfirmDialog)
			m_ConfirmDialog = ExpansionMenuDialog_MarketConfirmPurchase.Cast(m_Dialog);
		
		SetButtonText("#STR_EXPANSION_MARKET_DIALOG_ACCEPT");
	}
	
	override void OnButtonClick()
	{	
		m_ConfirmDialog.GetMarketMenu().OnConfirmBuyButtonClick();
		m_ConfirmDialog.GetMarketMenu().m_KeyInput = false;
	}
};

class ExpansionMenuDialogButton_Text_MarketConfirmPurchase_Cancel: ExpansionDialogButton_Text
{
	ref ExpansionMenuDialog_MarketConfirmPurchase m_ConfirmDialog;

	void ExpansionMenuDialogButton_Text_MarketConfirmPurchase_Cancel(ExpansionDialogBase dialog)
	{
		m_Dialog = dialog;
		
		if (!m_ConfirmDialog)
			m_ConfirmDialog = ExpansionMenuDialog_MarketConfirmPurchase.Cast(m_Dialog);
		
		SetButtonText("#STR_EXPANSION_MARKET_DIALOG_CANCLE");
	}

	override void OnButtonClick()
	{
		m_ConfirmDialog.Hide();
		m_ConfirmDialog.GetMarketMenu().SetMenuState(ExpansionMarketMenuState.NONE);
		m_ConfirmDialog.GetMarketMenu().m_KeyInput = false;
	}
};
