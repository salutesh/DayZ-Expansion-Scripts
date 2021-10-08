/**
 * ExpansionMenuDialog_MarketConfirmPurchase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMenuDialog_MarketConfirmPurchase: ExpansionMenuDialogBase
{
	ref ExpansionMenuDialogContent_Text m_Text;
	ref ExpansionMenuDialogButton_Text_MarketConfirmPurchase_Accept m_AcceptButton;
	ref ExpansionMenuDialogButton_Text_MarketConfirmPurchase_Cancel m_CancelButton;
	
	ref ExpansionMarketMenu m_MarketMenu;
	ref ExpansionMarketMenuDialogData m_DialogData;
	
	bool m_KeyInput = false;
	
	// ------------------------------------------------------------
	// ExpansionMenuDialogButton_Text_MarketSpawnOnGround_Cancel Constructor
	// ------------------------------------------------------------	
	void ExpansionMenuDialog_MarketConfirmPurchase(ExpansionScriptViewMenu parentMenu, ExpansionMarketMenuDialogData dialogData)
	{
		m_ParentMenu = parentMenu;
		m_DialogData = dialogData;
		
		if (!m_MarketMenu)
			m_MarketMenu = ExpansionMarketMenu.Cast(m_ParentMenu);
		
		if (!m_Text)
		{
			m_Text = new ExpansionMenuDialogContent_Text(this);
			AddContent(m_Text);
			
			string displayName = ExpansionStatic.GetItemDisplayNameWithType(m_DialogData.ClassName);
			StringLocaliser text;
			if (!m_DialogData.IncludeAttachments)
			{
				text = new StringLocaliser("STR_EXPANSION_MARKET_PURCHASE_DIALOG_TEXT", m_DialogData.Amount.ToString(), displayName, m_DialogData.Price.ToString());
				m_Text.SetText(text.Format());
			}
			else
			{
				text = new StringLocaliser("STR_EXPANSION_MARKET_PURCHASE_DIALOG_TEXT_ATTACHMENTS", m_DialogData.Amount.ToString(), displayName, m_DialogData.Price.ToString());
				m_Text.SetText(text.Format());
			}
			
			m_Text.Show();
		}
				
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
	
	// ------------------------------------------------------------
	// ExpansionMenuDialogButton_Text_MarketSpawnOnGround_Cancel GetDialogTitle
	// ------------------------------------------------------------
	override string GetDialogTitle()
	{
		return "#STR_EXPANSION_MARKET_PURCHASE_DIALOG_TITLE";
	}
	
	// ------------------------------------------------------------
	// ExpansionMenuDialogButton_Text_MarketSpawnOnGround_Cancel GetMarketMenu
	// ------------------------------------------------------------		
	ExpansionMarketMenu GetMarketMenu()
	{
		return m_MarketMenu;
	}
	
	// ------------------------------------------------------------
	// ExpansionMenuDialog_MarketConfirmSell GetUpdateTickRate
	// ------------------------------------------------------------	
	override float GetUpdateTickRate()
	{
		return 0.25;
	}
	
	// ------------------------------------------------------------
	// ExpansionMenuDialog_MarketConfirmSell Update
	// ------------------------------------------------------------	
	override void Update()
	{		
		if ((ExpansionStatic.Key_Y() || ExpansionStatic.Key_Z()) && !m_KeyInput)
		{
			GetMarketMenu().OnConfirmBuyButtonClick();
			m_KeyInput = true;
			return;
		}
		
		if (ExpansionStatic.Key_C() && !m_KeyInput)
		{
			Hide();
			GetMarketMenu().SetMenuState(ExpansionMarketMenuState.NONE);
			m_KeyInput = true;
			return;
		}
	}
};

class ExpansionMenuDialogButton_Text_MarketConfirmPurchase_Accept: ExpansionMenuDialogButton_Text
{
	ref ExpansionMenuDialog_MarketConfirmPurchase m_ConfirmDialog;
	
	// ------------------------------------------------------------
	// ExpansionMenuDialogButton_Text_MarketConfirmPurchase_Accept Constructor
	// ------------------------------------------------------------	
	void ExpansionMenuDialogButton_Text_MarketConfirmPurchase_Accept(ExpansionMenuDialogBase dialog)
	{
		m_Dialog = dialog;		
		
		if (!m_ConfirmDialog)
			m_ConfirmDialog = ExpansionMenuDialog_MarketConfirmPurchase.Cast(m_Dialog);
		
		SetButtonText("#STR_EXPANSION_MARKET_DIALOG_ACCEPT");
	}
	
	// ------------------------------------------------------------
	// ExpansionMenuDialogButton_Text_MarketConfirmPurchase_Accept OnButtonClick
	// ------------------------------------------------------------	
	override void OnButtonClick()
	{	
		m_ConfirmDialog.GetMarketMenu().OnConfirmBuyButtonClick();
		//m_ConfirmDialog.m_KeyInput = true;
		m_ConfirmDialog.GetMarketMenu().m_KeyInput = false;
	}
};

class ExpansionMenuDialogButton_Text_MarketConfirmPurchase_Cancel: ExpansionMenuDialogButton_Text
{
	ref ExpansionMenuDialog_MarketConfirmPurchase m_ConfirmDialog;
	
	// ------------------------------------------------------------
	// ExpansionMenuDialogButton_Text_MarketConfirmPurchase_Cancel Constructor
	// ------------------------------------------------------------		
	void ExpansionMenuDialogButton_Text_MarketConfirmPurchase_Cancel(ExpansionMenuDialogBase dialog)
	{
		m_Dialog = dialog;
		
		if (!m_ConfirmDialog)
			m_ConfirmDialog = ExpansionMenuDialog_MarketConfirmPurchase.Cast(m_Dialog);
		
		SetButtonText("#STR_EXPANSION_MARKET_DIALOG_CANCLE");
	}
	
	// ------------------------------------------------------------
	// ExpansionMenuDialogButton_Text_MarketConfirmPurchase_Cancel OnButtonClick
	// ------------------------------------------------------------		
	override void OnButtonClick()
	{
		m_ConfirmDialog.Hide();
		m_ConfirmDialog.GetMarketMenu().SetMenuState(ExpansionMarketMenuState.NONE);
		//m_ConfirmDialog.m_KeyInput = true;
		m_ConfirmDialog.GetMarketMenu().m_KeyInput = false;
	}
};
