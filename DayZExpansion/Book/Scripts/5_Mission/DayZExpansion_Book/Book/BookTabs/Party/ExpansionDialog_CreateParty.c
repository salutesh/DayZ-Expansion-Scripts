/**
 * ExpansionDialog_CreateParty.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONMODGROUPS
class ExpansionDialog_CreateParty: ExpansionDialogBookBase
{
	ref ExpansionDialogContent_Text m_Text;
	ref ExpansionDialogBookContent_Editbox m_Editbox;
	ref ExpansionDialogButton_Text_CreateParty_Accept m_AcceptButton;
	ref ExpansionDialogButton_Text_CreateParty_Cancel m_CancelButton;
	ref ExpansionBookMenuTabParty m_PartyTab;

	void ExpansionDialog_CreateParty(ExpansionScriptView parentView)
	{
		m_ParentView = parentView;
		
		if (!Class.CastTo(m_PartyTab, m_ParentView))
			return;
		
		if (!m_Text)
		{
			m_Text = new ExpansionDialogContent_Text(this);
			AddContent(m_Text);
			m_Text.SetText("#STR_EXPANSION_BOOK_GROUP_ENTER_PARTYNAME");
			m_Text.SetTextColor(ARGB(255,0,0,0));
			m_Text.Show();
		}
		
		if (!m_Editbox)
		{
			m_Editbox = new ExpansionDialogBookContent_Editbox(this);
			AddContent(m_Editbox);
			m_Editbox.Show();
		}
		
		if (!m_AcceptButton)
		{
			m_AcceptButton = new ExpansionDialogButton_Text_CreateParty_Accept(this, parentView);
			AddButton(m_AcceptButton);
			m_AcceptButton.Show();
		}
		
		if (!m_CancelButton)
		{
			m_CancelButton = ExpansionDialogButton_Text_CreateParty_Cancel(this, parentView);
			AddButton(m_CancelButton);
			m_CancelButton.Show();
		}
	}
		
	override string GetDialogTitle()
	{
		return "#STR_EXPANSION_BOOK_GROUP_PAGE_TITLE";
	}
	
	ExpansionDialogBookContent_Editbox GetEditbox()
	{
		return m_Editbox;
	}
	
	override void OnShow()
	{
		super.OnShow();	
		SwitchMovementLockState(true);
	}
	
	override void OnHide()
	{
		super.OnHide();	
		SwitchMovementLockState(false);
	}
};

class ExpansionDialogButton_Text_CreateParty_Accept: ExpansionDialogBookButton_Text
{
	ref ExpansionDialog_CreateParty m_CreatePartyDialog;
	ref ExpansionPartyModule m_PartyModule;
	ref ExpansionBookMenuTabParty m_PartyTab;
	
	void ExpansionDialogButton_Text_CreateParty_Accept(ExpansionDialogBase dialog, ExpansionScriptView parentView)
	{
		m_Dialog = dialog;
		
		if (!Class.CastTo(m_PartyTab, parentView))
			return;
		
		if (!m_CreatePartyDialog)
			m_CreatePartyDialog = ExpansionDialog_CreateParty.Cast(m_Dialog);
		
		if (!m_PartyModule)
			m_PartyModule = ExpansionPartyModule.Cast(CF_ModuleCoreManager.Get(ExpansionPartyModule));
		
		SetButtonText("#STR_EXPANSION_ACCEPT");
		SetTextColor(ARGB(255,0,0,0));
	}
	
	override void OnButtonClick()
	{
		if (m_CreatePartyDialog.GetEditbox().GetEditboxText() != "")
		{
			string party_name = m_CreatePartyDialog.GetEditbox().GetEditboxText();
			TStringArray allCharacters = {"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z","a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z","0","1","2","3","4","5","6","7","8","9","_"," ","'","\""};
			if (party_name.Length() < 3 || party_name.Length() > 32)
			{
				ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "STR_EXPANSION_PARTY_SYSTEM_NAME").Error();
				return;
			}
			
			for (int i = 0; i < party_name.Length(); i++)
			{
				if (allCharacters.Find(party_name.Get(i)) == -1)
				{
					ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "STR_EXPANSION_PARTY_SYSTEM_ALPHANUMERIC").Error();
					return;
				}
			}
	
		   	m_PartyModule.CreateParty(party_name);
			m_CreatePartyDialog.Hide();
			m_PartyTab.Hide();
		}
	}
};

class ExpansionDialogButton_Text_CreateParty_Cancel: ExpansionDialogBookButton_Text
{
	ref ExpansionDialog_CreateParty m_CreatePartyDialog;
	ref ExpansionBookMenuTabParty m_PartyTab;
	
	void ExpansionDialogButton_Text_CreateParty_Cancel(ExpansionDialogBase dialog, ExpansionScriptView parentView)
	{
		m_Dialog = dialog;
		
		if (!Class.CastTo(m_PartyTab, parentView))
			return;
			
		if (!m_CreatePartyDialog)
			m_CreatePartyDialog = ExpansionDialog_CreateParty.Cast(m_Dialog);
		
		SetButtonText("#STR_EXPANSION_CANCEL");
		SetTextColor(ARGB(255,0,0,0));
	}
	
	override void OnButtonClick()
	{
		m_CreatePartyDialog.Hide();
		m_PartyTab.Hide();
	}
};
#endif
