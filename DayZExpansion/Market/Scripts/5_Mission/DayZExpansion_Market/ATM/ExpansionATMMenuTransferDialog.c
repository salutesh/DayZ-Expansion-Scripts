/**
 * ExpansionATMMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionATMMenuTransferDialog: ExpansionDialogBase
{
	protected ref ExpansionDialogContent_Text m_Text;
	protected ref ExpansionATMMenuTransferDialogButton_Accept m_AcceptButton;
	protected ref ExpansionATMMenuTransferDialogButton_Cancel m_CancelButton;
	
	protected ref ExpansionATMMenu m_ATMMenu;
	protected ref SyncPlayer m_Player;
	protected int m_Amount;
	
	void ExpansionATMMenuTransferDialog(ScriptView parentView, SyncPlayer player, int amount)
	{
		m_ParentView = parentView;
		m_Player = player;
		m_Amount = amount;
		
		if (!m_ATMMenu)
			m_ATMMenu = ExpansionATMMenu.Cast(m_ParentView);
		
		if (!m_Text)
		{
			m_Text = new ExpansionDialogContent_Text(this);
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

class ExpansionATMMenuTransferDialogButton_Accept: ExpansionDialogButton_Text
{
	protected ref ExpansionATMMenuTransferDialog m_TransferDialog;
	
	void ExpansionATMMenuTransferDialogButton_Accept(ExpansionDialogBase dialog)
	{
		m_Dialog = dialog;		
		
		if (!m_TransferDialog)
			m_TransferDialog = ExpansionATMMenuTransferDialog.Cast(m_Dialog);
		
		SetButtonText("ACCEPT");
		SetTextColor(-10178716);
	}
	
	override void OnButtonClick()
	{
		m_TransferDialog.GetATMMenu().ConfirmTransfer();
	}
};

class ExpansionATMMenuTransferDialogButton_Cancel: ExpansionDialogButton_Text
{
	protected ref ExpansionATMMenuTransferDialog m_TransferDialog;
	
	void ExpansionATMMenuTransferDialogButton_Cancel(ExpansionDialogBase dialog)
	{
		m_Dialog = dialog;
		
		if (!m_TransferDialog)
			m_TransferDialog = ExpansionATMMenuTransferDialog.Cast(m_Dialog);
		
		SetButtonText("CANCEL");
		SetTextColor(-3657166);
	}
	
	override void OnButtonClick()
	{
		m_TransferDialog.Hide();
		m_TransferDialog.Destroy();
	}
};

class ExpansionATMMenuPartyTransferDialog: ExpansionDialogBase
{
	protected ref ExpansionDialogContent_Text m_Text;
	protected ref ExpansionATMMenuPartyTransferDialogButton_Accept m_AcceptButton;
	protected ref ExpansionATMMenuPartyTransferDialogButton_Cancel m_CancelButton;
	
	protected ref ExpansionATMMenu m_ATMMenu;
	protected int m_Amount;
	#ifdef EXPANSIONMODGROUPS
	protected ref ExpansionPartyData m_Party;
	
	void ExpansionATMMenuPartyTransferDialog(ScriptView parentView, int amount, ExpansionPartyData party)
	{
		m_ParentView = parentView;
		m_Amount = amount;
		m_Party = party;
		
		if (!m_ATMMenu)
			m_ATMMenu = ExpansionATMMenu.Cast(m_ParentView);
		
		if (!m_Text)
		{
			m_Text = new ExpansionDialogContent_Text(this);
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

class ExpansionATMMenuPartyTransferDialogButton_Accept: ExpansionDialogButton_Text
{
	protected ref ExpansionATMMenuPartyTransferDialog m_PartyTransferDialog;
	
	void ExpansionATMMenuPartyTransferDialogButton_Accept(ExpansionDialogBase dialog)
	{
		m_Dialog = dialog;		
		
		if (!m_PartyTransferDialog)
			m_PartyTransferDialog = ExpansionATMMenuPartyTransferDialog.Cast(m_Dialog);
		
		SetButtonText("ACCEPT");
		SetTextColor(-10178716);
	}
	
	override void OnButtonClick()
	{
		m_PartyTransferDialog.GetATMMenu().ConfirmPartyTransfer();
	}
};

class ExpansionATMMenuPartyTransferDialogButton_Cancel: ExpansionDialogButton_Text
{
	protected ref ExpansionATMMenuPartyTransferDialog m_PartyTransferDialog;
	
	void ExpansionATMMenuPartyTransferDialogButton_Cancel(ExpansionDialogBase dialog)
	{
		m_Dialog = dialog;
		
		if (!m_PartyTransferDialog)
			m_PartyTransferDialog = ExpansionATMMenuPartyTransferDialog.Cast(m_Dialog);
		
		SetButtonText("CANCEL");
		SetTextColor(-3657166);
	}
	
	override void OnButtonClick()
	{
		m_PartyTransferDialog.Hide();
		m_PartyTransferDialog.Destroy();
	}
};