/**
 * ExpansionMenuDialog_MarketConfirmSell.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMenuDialog_MarketConfirmSell: ExpansionDialogBase
{
	protected ref ExpansionDialogContent_Text m_Text;
	protected ref ExpansionMenuDialogButton_Text_MarketConfirmSell_Accept m_AcceptButton;
	protected ref ExpansionMenuDialogButton_Text_MarketConfirmSell_Cancel m_CancelButton;
	protected ref ExpansionDialogContent_WrapSpacer m_WrapSpacer;
	protected ref ExpansionDialogContent_Text m_AdditionalText;
	protected ref ExpansionDialogContentSpacer m_AdditionalText_Spacer;
	protected ref ExpansionMarketMenu m_MarketMenu;
	protected ref ExpansionMarketMenuDialogData m_DialogData;
	protected bool m_IncludeAttachments;
	protected bool m_KeyInput = false;

	void ExpansionMenuDialog_MarketConfirmSell(ScriptView parentView, ExpansionMarketMenuDialogData dialogData)
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
				text = new StringLocaliser("STR_EXPANSION_MARKET_SELL_DIALOG_TEXT", amount, displayName, price);
				m_Text.SetText(text.Format());
			}
			else
			{
				text = new StringLocaliser("STR_EXPANSION_MARKET_SELL_DIALOG_TEXT_ATTACHMENTS", amount, displayName, price);
				m_Text.SetText(text.Format());
			}
						
			m_Text.Show();
		}
		
		spacer = new ExpansionDialogContentSpacer(this);
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
			m_AdditionalText = new ExpansionDialogContent_Text(this);
			m_AdditionalText.SetTextColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("ColorItemInfoAttachments"));
			AddContent(m_AdditionalText);
			m_AdditionalText.Hide();
		
			m_AdditionalText_Spacer = new ExpansionDialogContentSpacer(this);
			AddContent(m_AdditionalText_Spacer);
		}
		
		if (!m_WrapSpacer)
		{			
			m_WrapSpacer = ExpansionDialogContent_WrapSpacer(this);
			AddContent(m_WrapSpacer);
			
			PopulateAttachmentsList();
		}
	}

	override string GetDialogTitle()
	{
		return "#STR_EXPANSION_MARKET_SELL_DIALOG_TITLE";
	}
	
	ExpansionMarketMenu GetMarketMenu()
	{
		return m_MarketMenu;
	}

	void PopulateAttachmentsList()
	{
		ExpansionMarketModule marketModule = ExpansionMarketModule.Cast(CF_ModuleCoreManager.Get(ExpansionMarketModule));
		ExpansionMarketPlayerItem playerItem;
		array<ref ExpansionMarketPlayerItem> items = m_MarketMenu.GetPlayerItems();
		
		if (items.Count() == 0)
			return;
		
		ExpansionDialogContentSpacer spacer;
				
		for (int j = 0; j < items.Count(); j++)
		{
			string itemName = items[j].ClassName;
			itemName.ToLower();
			
			itemName = marketModule.GetMarketItemClassName(marketModule.GetTrader().GetTraderMarket(), itemName);

			if (itemName == m_MarketMenu.GetSelectedMarketItem().ClassName)
			{
				playerItem = items[j];
			}
		}
		
		if (playerItem)
		{			
			ExpansionDialogContent_WrapSpacer_Entry entry;
			string name;
			if (playerItem.ContainerItems.Count() > 1)
			{
				foreach (string containerItemName, int quantity: playerItem.ContainerItems)
				{
					entry = new ExpansionDialogContent_WrapSpacer_Entry(m_WrapSpacer, ExpansionStatic.GetItemDisplayNameWithType(containerItemName));
					entry.SetTextColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("ColorItemInfoIcon"));	
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
				spacer = new ExpansionDialogContentSpacer(this);
				AddContent(spacer);
			}
			else if (playerItem.IsMagazine())
			{
				MagazineStorage magStorage = MagazineStorage.Cast(playerItem.Item);
				if (magStorage.GetAmmoCount() > 0)
				{
					StringLocaliser text  = new StringLocaliser("STR_EXPANSION_MARKET_SELL_DIALOG_WARNING_MAG", magStorage.GetAmmoCount().ToString());
					m_AdditionalText.SetText(text.Format());
					m_AdditionalText.Show();
				}
			}
			else if (playerItem.IsAttached())
			{
				if (playerItem.GetItem().GetHierarchyParent())
				{
					name = playerItem.GetItem().GetHierarchyParent().GetDisplayName();
					entry = new ExpansionDialogContent_WrapSpacer_Entry(m_WrapSpacer, name);	
					entry.SetTextColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("ColorItemInfoIcon"));	
					m_WrapSpacer.AddSpacerContent(entry);
					m_AdditionalText.SetText("#STR_EXPANSION_MARKET_SELL_DIALOG_WARNING_ATTACHED");
					m_AdditionalText.Show();
					spacer = new ExpansionDialogContentSpacer(this);
					AddContent(spacer);
				}
			}
		}

		if (!m_AdditionalText.IsVisible())
			m_AdditionalText_Spacer.Hide();

		if (!m_WrapSpacer.m_WrapSpacerController.SpacerContent.Count())
			m_WrapSpacer.Hide();
	}

	override float GetUpdateTickRate()
	{
		return 0.25;
	}

	override void Expansion_Update()
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

class ExpansionMenuDialogButton_Text_MarketConfirmSell_Accept: ExpansionDialogButton_Text
{
	protected ref ExpansionMenuDialog_MarketConfirmSell m_ConfirmDialog;

	void ExpansionMenuDialogButton_Text_MarketConfirmSell_Accept(ExpansionDialogBase dialog)
	{
		m_Dialog = dialog;		
		
		if (!m_ConfirmDialog)
			m_ConfirmDialog = ExpansionMenuDialog_MarketConfirmSell.Cast(m_Dialog);
		
		SetButtonText("#STR_EXPANSION_MARKET_DIALOG_ACCEPT");
	}

	override void OnButtonClick()
	{	
		m_ConfirmDialog.GetMarketMenu().OnConfirmSellButtonClick();
		m_ConfirmDialog.GetMarketMenu().m_KeyInput = false;
	}
};

class ExpansionMenuDialogButton_Text_MarketConfirmSell_Cancel: ExpansionDialogButton_Text
{
	protected ref ExpansionMenuDialog_MarketConfirmSell m_ConfirmDialog;

	void ExpansionMenuDialogButton_Text_MarketConfirmSell_Cancel(ExpansionDialogBase dialog)
	{
		m_Dialog = dialog;
		
		if (!m_ConfirmDialog)
			m_ConfirmDialog = ExpansionMenuDialog_MarketConfirmSell.Cast(m_Dialog);
		
		SetButtonText("#STR_EXPANSION_MARKET_DIALOG_CANCLE");
	}

	override void OnButtonClick()
	{
		m_ConfirmDialog.Hide();
		m_ConfirmDialog.GetMarketMenu().SetMenuState(ExpansionMarketMenuState.NONE);
		m_ConfirmDialog.GetMarketMenu().m_KeyInput = false;
	}
};