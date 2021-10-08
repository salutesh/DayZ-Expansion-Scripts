/**
 * ExpansionATMMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionATMMenuTransferDialog: ExpansionMenuDialogBase
{
	ref ExpansionMenuDialogContent_Text m_Text;
	ref ExpansionMenuDialogContent_Editbox m_Editbox;
	ref ExpansionATMMenuTransferDialogButton_Accept m_AcceptButton;
	ref ExpansionATMMenuTransferDialogButton_Cancel m_CancelButton;
	
	ref ExpansionATMMenu m_ATMMenu;
	ref SyncPlayer m_Player;
	int m_Amount;
	
	void ExpansionATMMenuTransferDialog(ExpansionScriptViewMenu parentMenu, SyncPlayer player, int amount)
	{
		m_ParentMenu = parentMenu;
		m_Player = player;
		m_Amount = amount;
		
		if (!m_ATMMenu)
			m_ATMMenu = ExpansionATMMenu.Cast(m_ParentMenu);
		
		if (!m_Text)
		{
			m_Text = new ExpansionMenuDialogContent_Text(this);
			AddContent(m_Text);
			m_Text.SetText(string.Format("Are you sure you want to transfer %1 to the player %2", m_Amount, m_Player.m_PlayerName));
			m_Text.Show();
		}
				
		if (!m_AcceptButton)
		{
			m_AcceptButton = new ExpansionATMMenuTransferDialogButton_Accept(this);
			AddButton(m_AcceptButton);
			m_AcceptButton.Show();
		}
		
		if (!m_CancelButton)
		{
			m_CancelButton = ExpansionATMMenuTransferDialogButton_Cancel(this);
			AddButton(m_CancelButton);
			m_CancelButton.Show();
		}
	}
	
	override string GetDialogTitle()
	{
		return "Transfer Money";
	}
		
	ExpansionATMMenu GetATMMenu()
	{
		return m_ATMMenu;
	}
};

class ExpansionATMMenuTransferDialogButton_Accept: ExpansionMenuDialogButton_Text
{
	ref ExpansionATMMenuTransferDialog m_TransferDialog;
	
	void ExpansionATMMenuTransferDialogButton_Accept(ExpansionMenuDialogBase dialog)
	{
		m_Dialog = dialog;		
		
		if (!m_TransferDialog)
			m_TransferDialog = ExpansionATMMenuTransferDialog.Cast(m_Dialog);
		
		SetButtonText("ACCEPT");
	}
	
	override void OnButtonClick()
	{
		m_TransferDialog.GetATMMenu().ConfirmTransfer();
	}
};

class ExpansionATMMenuTransferDialogButton_Cancel: ExpansionMenuDialogButton_Text
{
	ref ExpansionATMMenuTransferDialog m_TransferDialog;
	
	void ExpansionATMMenuTransferDialogButton_Cancel(ExpansionMenuDialogBase dialog)
	{
		m_Dialog = dialog;
		
		if (!m_TransferDialog)
			m_TransferDialog = ExpansionATMMenuTransferDialog.Cast(m_Dialog);
		
		SetButtonText("CANCEL");
	}
	
	override void OnButtonClick()
	{
		m_TransferDialog.Hide();
	}
};

class ExpansionATMMenuPartyTransferDialog: ExpansionMenuDialogBase
{
	ref ExpansionMenuDialogContent_Text m_Text;
	ref ExpansionMenuDialogContent_Editbox m_Editbox;
	ref ExpansionATMMenuPartyTransferDialogButton_Accept m_AcceptButton;
	ref ExpansionATMMenuPartyTransferDialogButton_Cancel m_CancelButton;
	
	ref ExpansionATMMenu m_ATMMenu;
	int m_Amount;
	#ifdef EXPANSIONMOD
	ref ExpansionPartyData m_Party;
	
	void ExpansionATMMenuPartyTransferDialog(ExpansionScriptViewMenu parentMenu, int amount, ExpansionPartyData party)
	{
		m_ParentMenu = parentMenu;
		m_Amount = amount;
		m_Party = party;
		
		if (!m_ATMMenu)
			m_ATMMenu = ExpansionATMMenu.Cast(m_ParentMenu);
		
		if (!m_Text)
		{
			m_Text = new ExpansionMenuDialogContent_Text(this);
			AddContent(m_Text);
			m_Text.SetText(string.Format("Are you sure you want to transfer %1 to the %2 party deposit?", m_Amount, m_Party.GetPartyName()));
			m_Text.Show();
		}
				
		if (!m_AcceptButton)
		{
			m_AcceptButton = new ExpansionATMMenuPartyTransferDialogButton_Accept(this);
			AddButton(m_AcceptButton);
			m_AcceptButton.Show();
		}
		
		if (!m_CancelButton)
		{
			m_CancelButton = ExpansionATMMenuPartyTransferDialogButton_Cancel(this);
			AddButton(m_CancelButton);
			m_CancelButton.Show();
		}
	}
	#endif
	
	override string GetDialogTitle()
	{
		return "Deposit Money";
	}
		
	ExpansionATMMenu GetATMMenu()
	{
		return m_ATMMenu;
	}
};

class ExpansionATMMenuPartyTransferDialogButton_Accept: ExpansionMenuDialogButton_Text
{
	ref ExpansionATMMenuPartyTransferDialog m_PartyTransferDialog;
	
	void ExpansionATMMenuPartyTransferDialogButton_Accept(ExpansionMenuDialogBase dialog)
	{
		m_Dialog = dialog;		
		
		if (!m_PartyTransferDialog)
			m_PartyTransferDialog = ExpansionATMMenuPartyTransferDialog.Cast(m_Dialog);
		
		SetButtonText("ACCEPT");
	}
	
	override void OnButtonClick()
	{
		m_PartyTransferDialog.GetATMMenu().ConfirmPartyTransfer();
	}
};

class ExpansionATMMenuPartyTransferDialogButton_Cancel: ExpansionMenuDialogButton_Text
{
	ref ExpansionATMMenuPartyTransferDialog m_PartyTransferDialog;
	
	void ExpansionATMMenuPartyTransferDialogButton_Cancel(ExpansionMenuDialogBase dialog)
	{
		m_Dialog = dialog;
		
		if (!m_PartyTransferDialog)
			m_PartyTransferDialog = ExpansionATMMenuPartyTransferDialog.Cast(m_Dialog);
		
		SetButtonText("CANCEL");
	}
	
	override void OnButtonClick()
	{
		m_PartyTransferDialog.Hide();
	}
};