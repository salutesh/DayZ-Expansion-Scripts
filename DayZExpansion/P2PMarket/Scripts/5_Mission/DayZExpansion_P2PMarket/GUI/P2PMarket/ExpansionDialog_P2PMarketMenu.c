/**
 * ExpansionDialog_P2PMarketMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionDialog_P2PMarketMenu: ExpansionDialogBase
{
	protected ref ExpansionDialogContent_Text m_Text;
	protected ref ExpansionDialogButton_P2PMarketMenu_Accept m_AcceptButton;
	protected ref ExpansionDialogButton_P2PMarketMenu_Cancel m_CancelButton;
	protected ExpansionP2PMarketMenu m_P2PMarketMenu;

	void ExpansionDialog_P2PMarketMenu(ScriptView parentView, string text)
	{
		m_ParentView = parentView;

		if (!m_P2PMarketMenu)
			m_P2PMarketMenu = ExpansionP2PMarketMenu.Cast(m_ParentView);
		
		ExpansionDialogContentSpacer spacer;
		spacer = new ExpansionDialogContentSpacer(this);
		AddContent(spacer);

		if (!m_Text)
		{
			m_Text = new ExpansionDialogContent_Text(this);
			AddContent(m_Text);
			m_Text.SetText(text);
			m_Text.SetTextColor(ARGB(255, 220, 220, 220));
			m_Text.Show();
		}
		
		spacer = new ExpansionDialogContentSpacer(this);
		AddContent(spacer);

		if (!m_AcceptButton)
		{
			m_AcceptButton = new ExpansionDialogButton_P2PMarketMenu_Accept(this);
			AddButton(m_AcceptButton);
			m_AcceptButton.Show();
		}

		if (!m_CancelButton)
		{
			m_CancelButton = new ExpansionDialogButton_P2PMarketMenu_Cancel(this);
			AddButton(m_CancelButton);
			m_CancelButton.Show();
		}
	}

	override string GetDialogTitle()
	{
		return "#STR_EXPANSION_MARKET_P2P_TRADERNAME";
	}
};

class ExpansionDialogButton_P2PMarketMenu_Accept: ExpansionDialogButton_Text
{
	protected ExpansionDialog_P2PMarketMenu m_MenuDialog;
	protected ExpansionP2PMarketMenu m_P2PMarketMenu;

	void ExpansionDialogButton_P2PMarketMenu_Accept(ExpansionDialogBase dialog)
	{
		if (!m_MenuDialog)
			m_MenuDialog = ExpansionDialog_P2PMarketMenu.Cast(GetDialog());

		if (!m_P2PMarketMenu)
			m_P2PMarketMenu = ExpansionP2PMarketMenu.Cast(m_MenuDialog.GetParentView());

		SetButtonText("#STR_EXPANSION_ACCEPT");
		SetTextColor(ARGB(255, 220, 220, 220));
	}

	override void OnButtonClick()
	{
		if (m_P2PMarketMenu.GetViewState() == ExpansionP2PMarketMenuViewState.DetailViewListingItem)
		{
			m_P2PMarketMenu.OnPurchaseListingClick();
		}
		else if (m_P2PMarketMenu.GetViewState() == ExpansionP2PMarketMenuViewState.DetailViewPlayerItem)
		{
			m_P2PMarketMenu.OnConfirmListingClick();
		}
		
		m_P2PMarketMenu.CancelDialogView();
		m_MenuDialog.Hide();
	}
};

class ExpansionDialogButton_P2PMarketMenu_Cancel: ExpansionDialogButton_Text
{
	protected ExpansionDialog_P2PMarketMenu m_MenuDialog;
	protected ExpansionP2PMarketMenu m_P2PMarketMenu;

	void ExpansionDialogButton_P2PMarketMenu_Cancel(ExpansionDialogBase dialog)
	{
		if (!m_MenuDialog)
			m_MenuDialog = ExpansionDialog_P2PMarketMenu.Cast(GetDialog());

		if (!m_P2PMarketMenu)
			m_P2PMarketMenu = ExpansionP2PMarketMenu.Cast(m_MenuDialog.GetParentView());

		SetButtonText("#STR_EXPANSION_CANCEL");
		SetTextColor(ARGB(255, 220, 220, 220));
	}

	override void OnButtonClick()
	{	
		m_P2PMarketMenu.CancelDialogView();
		m_MenuDialog.Hide();
	}
};