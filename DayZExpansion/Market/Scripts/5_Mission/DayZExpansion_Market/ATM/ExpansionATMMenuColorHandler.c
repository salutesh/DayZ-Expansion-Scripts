/**
 * ExpansionATMMenuColorHandler.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionATMMenuColorHandler
{
	protected Widget m_LayooutRoot;
	
	protected Widget atm_vignette;

	protected Widget atm_header;
	protected TextWidget atm_text;
	
	protected TextWidget MaxLabel;
	protected TextWidget MaxValue;
	
	protected TextWidget MoneyDepositLabel;
	protected TextWidget MoneyDepositValue;
	
	protected TextWidget PlayerMoneyLabel;
	protected TextWidget PlayerMoneyValue;
	
	protected TextWidget AmountLabel;
	protected EditBoxWidget AmountValue;
	protected ImageWidget AmountBackground;
	
	protected ButtonWidget ButtonWithdraw;
	protected TextWidget ButtonWithdrawLabel;
	
	protected ButtonWidget ButtonWithdrawAll;
	protected TextWidget ButtonWithdrawAllLabel;
	
	protected ButtonWidget ButtonDeposit;
	protected TextWidget ButtonDepositLabel;
	
	protected ButtonWidget ButtonDepositAll;
	protected TextWidget ButtonDepositAllLabel;
	
	protected ButtonWidget ButtonTransfer;
	protected TextWidget ButtonTransferLabel;
	
	protected ButtonWidget ButtonTransferAll;
	protected TextWidget ButtonTransferAllLabel;
	
	protected Widget atm_separator;
	
	protected Widget atm_transfer_header;
	protected TextWidget atm_transfer_text;
	
	protected ButtonWidget atm_filter_clear;
	protected ImageWidget atm_filter_search_icon;
	protected ImageWidget atm_filter_clear_icon;
	protected EditBoxWidget atm_filter_box;
	protected ImageWidget atm_filter_background;
	
	protected Widget atm_transfer_separator;
	
	protected Widget atm_party_header;
	protected TextWidget atm_party_text;
	
	protected TextWidget PartyNameLabel;
	protected TextWidget PartyNameValue;
	
	protected TextWidget PartyIDLabel;
	protected TextWidget PartyIDValue;
	
	protected TextWidget PartyOwnerLabel;
	protected TextWidget PartyOwnerValue;
	
	protected TextWidget PartyMaxLabel;
	protected TextWidget PartyMaxValue;

	protected TextWidget PartyMoneyDepositLabel;
	protected TextWidget PartyMoneyDepositValue;
	
	protected ButtonWidget PartyButtonDeposit;
	protected TextWidget ButtonPartyDepositLabel;
	
	protected ButtonWidget PartyButtonDepositAll;
	protected TextWidget ButtonPartyDepositAllLabel;
	
	protected ButtonWidget PartyButtonWithdraw;
	protected TextWidget PartyButtonWithdrawLabel;
	
	protected ButtonWidget PartyButtonWithdrawAll;
	protected TextWidget PartyButtonWithdrawAllLabel;
	
	void ExpansionATMMenuColorHandler(Widget layoutRoot)
	{
		m_LayooutRoot = layoutRoot;
		
		SetColors();
	}
	
	void SetColors()
	{
		//! Menu vignette
		atm_vignette = ImageWidget.Cast(m_LayooutRoot.FindAnyWidget("atm_vignette"));
		atm_vignette.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorVignette"));
		
		//! Personal ATM Header
		atm_header = Widget.Cast(m_LayooutRoot.FindAnyWidget("atm_header"));
		atm_header.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorHeaders"));
		
		//! Personal ATM Header text
		atm_text = TextWidget.Cast(m_LayooutRoot.FindAnyWidget("atm_text"));
		atm_text.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorText"));
		
		//! Max description
		MaxLabel = TextWidget.Cast(m_LayooutRoot.FindAnyWidget("MaxLabel"));
		MaxLabel.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorText"));
		
		//! Max value
		MaxValue = TextWidget.Cast(m_LayooutRoot.FindAnyWidget("MaxValue"));
		MaxValue.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorText"));
		
		//! Money deposited description
		MoneyDepositLabel = TextWidget.Cast(m_LayooutRoot.FindAnyWidget("MoneyDepositLabel"));
		MoneyDepositLabel.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorText"));
		
		//! Money deposited value
		MoneyDepositValue = TextWidget.Cast(m_LayooutRoot.FindAnyWidget("MoneyDepositValue"));
		MoneyDepositValue.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorText"));
		
		//! Player money description
		PlayerMoneyLabel = TextWidget.Cast(m_LayooutRoot.FindAnyWidget("PlayerMoneyLabel"));
		PlayerMoneyLabel.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorText"));
		
		//! Amount description
		AmountLabel = TextWidget.Cast(m_LayooutRoot.FindAnyWidget("AmountLabel"));
		AmountLabel.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorText"));
		
		// Amount value editbox
		AmountValue = EditBoxWidget.Cast(m_LayooutRoot.FindAnyWidget("AmountValue"));
		AmountValue.SetTextColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorText"));
		
		//! Amount background
		AmountBackground = ImageWidget.Cast(m_LayooutRoot.FindAnyWidget("AmountBackground"));
		AmountBackground.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorLabels"));
		
		//! Withdraw button
		ButtonWithdraw = ButtonWidget.Cast(m_LayooutRoot.FindAnyWidget("ButtonWithdraw"));
		ButtonWithdraw.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("ColorSellButton"));
		
		//! Withdraw button text
		ButtonWithdrawLabel = TextWidget.Cast(m_LayooutRoot.FindAnyWidget("ButtonWithdrawLabel"));
		ButtonWithdrawLabel.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorText"));
		
		//! Withdraw all button
		ButtonWithdrawAll = ButtonWidget.Cast(m_LayooutRoot.FindAnyWidget("ButtonWithdrawAll"));
		ButtonWithdrawAll.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("ColorSellButton"));
		
		//! Withdraw all button text
		ButtonWithdrawAllLabel = TextWidget.Cast(m_LayooutRoot.FindAnyWidget("ButtonWithdrawAllLabel"));
		ButtonWithdrawAllLabel.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorText"));
		
		//! Deposit button
		ButtonDeposit = ButtonWidget.Cast(m_LayooutRoot.FindAnyWidget("ButtonDeposit"));
		ButtonDeposit.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("ColorBuyButton"));
		
		//! Deposit button text
		ButtonDepositLabel = TextWidget.Cast(m_LayooutRoot.FindAnyWidget("ButtonDepositLabel"));
		ButtonDepositLabel.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorText"));
		
		//! Deposit all button
		ButtonDepositAll = ButtonWidget.Cast(m_LayooutRoot.FindAnyWidget("ButtonDepositAll"));
		ButtonDepositAll.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("ColorBuyButton"));
		
		//! Deposit all button text
		ButtonDepositAllLabel = TextWidget.Cast(m_LayooutRoot.FindAnyWidget("ButtonDepositAllLabel"));
		ButtonDepositAllLabel.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorText"));
		
		//! Transfer button
		ButtonTransfer = ButtonWidget.Cast(m_LayooutRoot.FindAnyWidget("ButtonTransfer"));
		ButtonTransfer.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("ColorBuyButton"));
		
		//! Transfer button text
		ButtonTransferLabel = TextWidget.Cast(m_LayooutRoot.FindAnyWidget("ButtonTransferLabel"));
		ButtonTransferLabel.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorText"));
		
		//! Transfer all button
		ButtonTransferAll = ButtonWidget.Cast(m_LayooutRoot.FindAnyWidget("ButtonTransferAll"));
		ButtonTransferAll.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("ColorBuyButton"));
		
		//! Transfer all button text
		ButtonTransferAllLabel = TextWidget.Cast(m_LayooutRoot.FindAnyWidget("ButtonTransferAllLabel"));
		ButtonTransferAllLabel.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorText"));
		
		//! Personal ATM seperator line
		atm_separator = Widget.Cast(m_LayooutRoot.FindAnyWidget("atm_separator"));
		atm_separator.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorText"));
		
		//! Transfer Header
		atm_transfer_header = Widget.Cast(m_LayooutRoot.FindAnyWidget("atm_transfer_header"));
		atm_transfer_header.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorHeaders"));
		
		//! Transfer Header text
		atm_transfer_text = TextWidget.Cast(m_LayooutRoot.FindAnyWidget("atm_transfer_text"));
		atm_transfer_text.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorText"));
		
		//! Search filter button
		atm_filter_clear = ButtonWidget.Cast(m_LayooutRoot.FindAnyWidget("atm_filter_clear"));
		atm_filter_clear.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("ColorSearchFilterButton"));
		
		//! Search editbox
		atm_filter_box = EditBoxWidget.Cast(m_LayooutRoot.FindAnyWidget("atm_filter_box"));
		atm_filter_box.SetTextColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorText"));
		
		//! Search editbox background
		atm_filter_background = ImageWidget.Cast(m_LayooutRoot.FindAnyWidget("atm_filter_background"));
		atm_filter_background.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorLabels"));
		
		//! Party Header
		atm_party_header = Widget.Cast(m_LayooutRoot.FindAnyWidget("atm_party_header"));
		atm_party_header.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorHeaders"));
		
		//! Party Header text
		atm_party_text = TextWidget.Cast(m_LayooutRoot.FindAnyWidget("atm_party_text"));
		atm_party_text.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorText"));
		
		//! Party name description
		PartyNameLabel = TextWidget.Cast(m_LayooutRoot.FindAnyWidget("PartyNameLabel"));
		PartyNameLabel.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorText"));
		
		//! Party name value
		PartyNameValue = TextWidget.Cast(m_LayooutRoot.FindAnyWidget("PartyNameValue"));
		PartyNameValue.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorText"));
		
		//! Party id description
		PartyIDLabel = TextWidget.Cast(m_LayooutRoot.FindAnyWidget("PartyIDLabel"));
		PartyIDLabel.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorText"));
		
		//! Party id value
		PartyIDValue = TextWidget.Cast(m_LayooutRoot.FindAnyWidget("PartyIDValue"));
		PartyIDValue.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorText"));
		
		//! Party owner description
		PartyOwnerLabel = TextWidget.Cast(m_LayooutRoot.FindAnyWidget("PartyOwnerLabel"));
		PartyOwnerLabel.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorText"));
		
		//! Party owner value
		PartyOwnerValue = TextWidget.Cast(m_LayooutRoot.FindAnyWidget("PartyOwnerValue"));
		PartyOwnerValue.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorText"));
		
		//! Party max money description
		PartyMaxLabel = TextWidget.Cast(m_LayooutRoot.FindAnyWidget("PartyMaxLabel"));
		PartyMaxLabel.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorText"));
		
		//! Party max money value
		PartyMaxValue = TextWidget.Cast(m_LayooutRoot.FindAnyWidget("PartyMaxValue"));
		PartyMaxValue.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorText"));
		
		//! Party money description
		PartyMoneyDepositLabel = TextWidget.Cast(m_LayooutRoot.FindAnyWidget("PartyMoneyDepositLabel"));
		PartyMoneyDepositLabel.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorText"));
		
		//! Party money value
		PartyMoneyDepositValue = TextWidget.Cast(m_LayooutRoot.FindAnyWidget("PartyMoneyDepositValue"));
		PartyMoneyDepositValue.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorText"));
		
		//! Party deposit button 
		PartyButtonDeposit = ButtonWidget.Cast(m_LayooutRoot.FindAnyWidget("PartyButtonDeposit"));
		PartyButtonDeposit.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("ColorBuyButton"));
		
		//! Party deposit button text
		ButtonPartyDepositLabel = TextWidget.Cast(m_LayooutRoot.FindAnyWidget("ButtonPartyDepositLabel"));
		ButtonPartyDepositLabel.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorText"));
		
		//! Party deposit all button 
		PartyButtonDepositAll = ButtonWidget.Cast(m_LayooutRoot.FindAnyWidget("PartyButtonDepositAll"));
		PartyButtonDepositAll.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("ColorBuyButton"));
		
		//! Party deposit all button text
		ButtonPartyDepositAllLabel = TextWidget.Cast(m_LayooutRoot.FindAnyWidget("ButtonPartyDepositAllLabel"));
		ButtonPartyDepositAllLabel.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorText"));
		
		//! Party withdraw button 
		PartyButtonWithdraw = ButtonWidget.Cast(m_LayooutRoot.FindAnyWidget("PartyButtonWithdraw"));
		PartyButtonWithdraw.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("ColorSellButton"));

		//! Party withdraw button text
		PartyButtonWithdrawLabel = TextWidget.Cast(m_LayooutRoot.FindAnyWidget("PartyButtonWithdrawLabel"));
		PartyButtonWithdrawLabel.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorText"));
		
		//! Party withdraw button 
		PartyButtonWithdrawAll = ButtonWidget.Cast(m_LayooutRoot.FindAnyWidget("PartyButtonWithdrawAll"));
		PartyButtonWithdrawAll.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("ColorSellButton"));

		//! Party withdraw button text
		PartyButtonWithdrawAllLabel = TextWidget.Cast(m_LayooutRoot.FindAnyWidget("PartyButtonWithdrawAllLabel"));
		PartyButtonWithdrawAllLabel.SetColor(GetExpansionSettings().GetMarket().MarketMenuColors.Get("BaseColorText"));
	}
};