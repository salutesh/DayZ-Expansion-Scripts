/**
 * ExpansionMenuDialog_MarketConfirmSell.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMenuDialog_MarketConfirmSell: ExpansionMenuDialogBase
{
	ref ExpansionMenuDialogContent_Text m_Text;
	ref ExpansionMenuDialogButton_Text_MarketConfirmSell_Accept m_AcceptButton;
	ref ExpansionMenuDialogButton_Text_MarketConfirmSell_Cancel m_CancelButton;
	
	ref ExpansionMenuDialogContent_WrapSpacer m_WrapSpacer;
	ref ExpansionMenuDialogContent_Text m_AdditionalText;
	
	ref ExpansionMarketMenu m_MarketMenu;
	ref ExpansionMarketMenuDialogData m_DialogData;
	
	bool m_IncludeAttachments;
	bool m_KeyInput = false;
	
	// ------------------------------------------------------------
	// ExpansionMenuDialog_MarketConfirmSell Constructor
	// ------------------------------------------------------------	
	void ExpansionMenuDialog_MarketConfirmSell(ExpansionScriptViewMenu parentMenu, ExpansionMarketMenuDialogData dialogData)
	{
		m_ParentMenu = parentMenu;
		m_DialogData = dialogData;
		
		if (!m_MarketMenu)
			m_MarketMenu = ExpansionMarketMenu.Cast(m_ParentMenu);
		
		ExpansionMenuDialogContentSpacer spacer;
		spacer = new ExpansionMenuDialogContentSpacer(this);
		AddContent(spacer);	
		
		if (!m_Text)
		{
			m_Text = new ExpansionMenuDialogContent_Text(this);
			AddContent(m_Text);
			
			string displayName = ExpansionStatic.GetItemDisplayNameWithType(m_DialogData.ClassName);
			StringLocaliser text;
			if (!m_DialogData.IncludeAttachments)
			{
				text = new StringLocaliser("STR_EXPANSION_MARKET_SELL_DIALOG_TEXT", m_DialogData.Amount.ToString(), displayName, m_DialogData.Price.ToString());
				m_Text.SetText(text.Format());
			}
			else
			{
				text = new StringLocaliser("STR_EXPANSION_MARKET_SELL_DIALOG_TEXT_ATTACHMENTS", m_DialogData.Amount.ToString(), displayName, m_DialogData.Price.ToString());
				m_Text.SetText(text.Format());
			}
						
			m_Text.Show();
		}
		
		spacer = new ExpansionMenuDialogContentSpacer(this);
		AddContent(spacer);	
				
		if (!m_AcceptButton)
		{
			m_AcceptButton = new ExpansionMenuDialogButton_Text_MarketConfirmSell_Accept(this);
			AddButton(m_AcceptButton);
			m_AcceptButton.Show();
		}
		
		if (!m_CancelButton)
		{
			m_CancelButton = ExpansionMenuDialogButton_Text_MarketConfirmSell_Cancel(this);
			AddButton(m_CancelButton);
			m_CancelButton.Show();
		}
		
		if (!m_AdditionalText)
		{
			m_AdditionalText = new ExpansionMenuDialogContent_Text(this);
			m_AdditionalText.SetTextColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.ColorItemInfoAttachments));
			AddContent(m_AdditionalText);
		}
		
		spacer = new ExpansionMenuDialogContentSpacer(this);
		AddContent(spacer);
		
		if (!m_WrapSpacer)
		{			
			m_WrapSpacer = ExpansionMenuDialogContent_WrapSpacer(this);
			AddContent(m_WrapSpacer);
			
			PopulateAttachmentsList();
		}
		
		dialog_info_button.SetColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.ColorItemInfoIcon));
	}
	
	// ------------------------------------------------------------
	// ExpansionMenuDialog_MarketConfirmSell GetDialogTitle
	// ------------------------------------------------------------	
	override string GetDialogTitle()
	{
		return "#STR_EXPANSION_MARKET_SELL_DIALOG_TITLE";
	}
	
	// ------------------------------------------------------------
	// ExpansionMenuDialog_MarketConfirmSell GetMarketMenu
	// ------------------------------------------------------------		
	ExpansionMarketMenu GetMarketMenu()
	{
		return m_MarketMenu;
	}
	
	// ------------------------------------------------------------
	// ExpansionMenuDialog_MarketConfirmSell PopulateAttachmentsList
	// ------------------------------------------------------------	
	void PopulateAttachmentsList()
	{
		ExpansionMarketPlayerItem playerItem;
		array<ref ExpansionMarketPlayerItem> items = m_MarketMenu.GetPlayerItems();
		
		if (items.Count() == 0)
			return;
		
		m_WrapSpacer.Show();
		
		ExpansionMenuDialogContentSpacer spacer;
				
		for (int j = 0; j < items.Count(); j++)
		{
			string itemName = items[j].ClassName;
			itemName.ToLower();
			
			if (itemName == m_MarketMenu.GetSelectedMarketItem().ClassName)
			{
				playerItem = items[j];
			}
		}
		
		if (playerItem)
		{			
			ExpansionMenuDialogContent_WrapSpacer_Entry entry;
			string name;
			if (playerItem.ContainerItems.Count() > 1)
			{		
				for (int i = 0; i < playerItem.ContainerItems.Count(); i++)
				{
					name = playerItem.ContainerItems.GetKey(i);
					int quantity = playerItem.ContainerItems.Get(name);
					entry = new ExpansionMenuDialogContent_WrapSpacer_Entry(m_WrapSpacer, ExpansionStatic.GetItemDisplayNameWithType(name));	
					entry.SetTextColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.ColorItemInfoIcon));	
					m_WrapSpacer.AddSpacerContent(entry);
				}
				
				if (playerItem.IsWeapon())
				{
					m_AdditionalText.SetText("#STR_EXPANSION_MARKET_ITEM_TOOLTIP_WARNING_SELL_WEAPONATT");
				}
				else
				{
					m_AdditionalText.SetText("#STR_EXPANSION_MARKET_ITEM_TOOLTIP_WARNING_SELL_ATT");
				}
				m_AdditionalText.Show();
				spacer = new ExpansionMenuDialogContentSpacer(this);
				AddContent(spacer);
			}
			
			if (playerItem.IsMagazine())
			{
				MagazineStorage magStorage = MagazineStorage.Cast(playerItem.Item);
				if (magStorage.GetAmmoCount() > 0)
				{
					spacer = new ExpansionMenuDialogContentSpacer(this);
					AddContent(spacer);
					StringLocaliser text  = new StringLocaliser("STR_EXPANSION_MARKET_SELL_DIALOG_WARNING_MAG", magStorage.GetAmmoCount().ToString());
					m_AdditionalText.SetText(text.Format());
					m_AdditionalText.Show();
					spacer = new ExpansionMenuDialogContentSpacer(this);
					AddContent(spacer);
				}
			}
			
			if (playerItem.IsAttached())
			{
				if (playerItem.GetItem().GetHierarchyParent())
				{
					name = ExpansionStatic.GetItemDisplayNameWithType(playerItem.GetItem().GetHierarchyParent().Type().ToString());
					entry = new ExpansionMenuDialogContent_WrapSpacer_Entry(m_WrapSpacer, ExpansionStatic.GetItemDisplayNameWithType(name));	
					entry.SetTextColor(ExpansionColor.HexToARGB(GetExpansionSettings().GetMarket().MarketMenuColors.ColorItemInfoIcon));	
					m_WrapSpacer.AddSpacerContent(entry);
					m_AdditionalText.SetText("#STR_EXPANSION_MARKET_SELL_DIALOG_WARNING_ATTACHED");
					m_AdditionalText.Show();
					spacer = new ExpansionMenuDialogContentSpacer(this);
					AddContent(spacer);
				}
			}
		}
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
			GetMarketMenu().OnConfirmSellButtonClick();
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

class ExpansionMenuDialogButton_Text_MarketConfirmSell_Accept: ExpansionMenuDialogButton_Text
{
	ref ExpansionMenuDialog_MarketConfirmSell m_ConfirmDialog;
	
	// ------------------------------------------------------------
	// ExpansionMenuDialogButton_Text_MarketConfirmSell_Accept Constructor
	// ------------------------------------------------------------	
	void ExpansionMenuDialogButton_Text_MarketConfirmSell_Accept(ExpansionMenuDialogBase dialog)
	{
		m_Dialog = dialog;		
		
		if (!m_ConfirmDialog)
			m_ConfirmDialog = ExpansionMenuDialog_MarketConfirmSell.Cast(m_Dialog);
		
		SetButtonText("#STR_EXPANSION_MARKET_DIALOG_ACCEPT");
	}
	
	// ------------------------------------------------------------
	// ExpansionMenuDialogButton_Text_MarketConfirmSell_Accept OnButtonClick
	// ------------------------------------------------------------	
	override void OnButtonClick()
	{	
		m_ConfirmDialog.GetMarketMenu().OnConfirmSellButtonClick();
		m_ConfirmDialog.GetMarketMenu().m_KeyInput = false;
	}
};

class ExpansionMenuDialogButton_Text_MarketConfirmSell_Cancel: ExpansionMenuDialogButton_Text
{
	ref ExpansionMenuDialog_MarketConfirmSell m_ConfirmDialog;
	
	// ------------------------------------------------------------
	// ExpansionMenuDialogButton_Text_MarketConfirmSell_Cancel Constructor
	// ------------------------------------------------------------	
	void ExpansionMenuDialogButton_Text_MarketConfirmSell_Cancel(ExpansionMenuDialogBase dialog)
	{
		m_Dialog = dialog;
		
		if (!m_ConfirmDialog)
			m_ConfirmDialog = ExpansionMenuDialog_MarketConfirmSell.Cast(m_Dialog);
		
		SetButtonText("#STR_EXPANSION_MARKET_DIALOG_CANCLE");
	}
	
	// ------------------------------------------------------------
	// ExpansionMenuDialogButton_Text_MarketConfirmSell_Cancel OnButtonClick
	// ------------------------------------------------------------	
	override void OnButtonClick()
	{
		m_ConfirmDialog.Hide();
		m_ConfirmDialog.GetMarketMenu().SetMenuState(ExpansionMarketMenuState.NONE);
		m_ConfirmDialog.GetMarketMenu().m_KeyInput = false;
	}
};