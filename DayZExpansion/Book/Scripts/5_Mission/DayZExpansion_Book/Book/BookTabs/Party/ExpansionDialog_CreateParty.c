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
class ExpansionDialog_CreateParty: ExpansionScriptView
{
	protected EditBoxWidget dialog_editbox_name;
	protected EditBoxWidget dialog_editbox_tag;
	
	protected ButtonWidget button_create_button;
	protected TextWidget button_create_text;
	
	protected ButtonWidget button_cancel_button;
	protected TextWidget button_cancel_text; 
	
	ref ExpansionBookMenuTabParty m_PartyTab;
	ref ExpansionPartyModule m_PartyModule;
	ref ExpansionDialog_CreatePartyController m_DialogController;

	void ExpansionDialog_CreateParty(ScriptView parentView)
	{		
		if (!Class.CastTo(m_PartyTab, parentView))
			return;
		
		m_PartyModule = ExpansionPartyModule.Cast(CF_ModuleCoreManager.Get(ExpansionPartyModule));
		m_DialogController = ExpansionDialog_CreatePartyController.Cast(GetController());
		
		m_DialogController.DialogTitle = "#STR_EXPANSION_BOOK_GROUP_PAGE_TITLE";
		m_DialogController.NotifyPropertyChanged("DialogTitle");
	}
	
	override string GetLayoutFile() 
	{
		return "DayZExpansion/Book/GUI/layouts/dialogs/expansion_dialog_create_group.layout";
	}

	override typename GetControllerType()
	{
		return ExpansionDialog_CreatePartyController;
	}
	
	override void OnShow()
	{
		super.OnShow();	
		SwitchMovementLockState(true);
	}
	
	override void OnHide()
	{
		SwitchMovementLockState(false);
		super.OnHide();	
	}
	
	protected void SwitchMovementLockState(bool lockMovement)
	{
		if (m_PartyTab)
		{
			ExpansionBookMenuTabBase tabBase = m_PartyTab;
			if (!tabBase)
				return;
			
			tabBase.GetBookMenu().SwitchMovementLockState(lockMovement);
		}
	}
	
	protected void OnCreateButtonClick()
	{
		if (dialog_editbox_name.GetText() == "")
			return;

		if (dialog_editbox_name.GetText() == "")
			return;

		string party_name = dialog_editbox_name.GetText();
		if (party_name.Length() < 3 || party_name.Length() > 32)
		{
			ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "STR_EXPANSION_PARTY_SYSTEM_NAME").Error();
			return;
		}

		if (!HasValidCharacters(party_name))
			return;
		
		string party_tag = dialog_editbox_tag.GetText();
		if (party_tag.Length() != ExpansionPartyData.GROUP_TAG_LENGTH )
		{
			if ( GetExpansionSettings().GetParty().ForcePartyToHaveTags && party_tag.Length() == 0 )
			{
				ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", string.Format("Your party tag need to be %1 characters!", ExpansionPartyData.GROUP_TAG_LENGTH)).Error();
				return;
			}
		}

		if (!HasValidCharacters(party_tag))
			return;

		m_PartyModule.CreateParty(party_name, party_tag);
		Hide();
		m_PartyTab.Hide();
	}
	
	private bool HasValidCharacters(string word)
	{
		TStringArray allCharacters = {"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z","a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","y","z","0","1","2","3","4","5","6","7","8","9","_"," ","'","\""};
		for (int i = 0; i < word.Length(); i++)
		{
			if (allCharacters.Find(word.Get(i)) == -1)
			{
				ExpansionNotification("STR_EXPANSION_PARTY_NOTIF_TITLE", "STR_EXPANSION_PARTY_SYSTEM_ALPHANUMERIC").Error();
				return false;
			}
		}

		return true;
	}
	
	protected void OnCancelButtonClick()
	{
		Hide();
		m_PartyTab.Hide();
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (w != null)
		{
			if (w == button_create_button)
			{
				button_create_text.SetColor(ARGB(255,255,255,255));
			}
			else if (w == button_create_button)
			{
				button_cancel_text.SetColor(ARGB(255,255,255,255));
			}
		}
		
		return false;
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (w != null)
		{
			if (w == button_create_button)
			{
				button_create_text.SetColor(ARGB(255,0,0,0));
			}
			else if (w == button_create_button)
			{
				button_cancel_text.SetColor(ARGB(255,0,0,0));
			}
		}

		return false;
	}
};
class ExpansionDialog_CreatePartyController: ExpansionViewController 
{
	string DialogTitle;
};
#endif
