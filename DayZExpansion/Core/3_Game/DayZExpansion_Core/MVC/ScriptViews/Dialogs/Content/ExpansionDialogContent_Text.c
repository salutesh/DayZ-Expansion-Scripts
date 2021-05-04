/**
 * ExpansionDialogContent_TextScroller.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionDialogContent_Text: ExpansionDialogContentBase
{
	ref ExpansionDialogContent_TextController m_TextController;	
	private string m_Text = "";
	
	void ExpansionDialogContent_Text(ExpansionDialogBase dialog)
	{
		m_Dialog = dialog;

		if (!m_TextController)
			m_TextController = ExpansionDialogContent_TextController.Cast(GetController());
	}
	
	override string GetLayoutFile() 
	{
		return "DayZExpansion/Core/GUI/layouts/mvc/dialogs/expansion_dialog_text.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionDialogContent_TextController;
	}
	
	string GetText()
	{
		return m_Text;
	}
	
	void SetText(string text)
	{
		m_Text = text;
	}
	
	void SetContent()
	{	
		m_TextController.Text = GetText();
		m_TextController.NotifyPropertyChanged("Text");
	}
	
	override void OnShow()
	{
		super.OnShow();
		
		SetContent();
	}
};

class ExpansionDialogContent_TextController: Controller
{
	string Text;
}

class ExpansionMenuDialogContent_Text: ExpansionMenuDialogContentBase
{
	ref ExpansionMenuDialogContent_TextController m_TextController;	
	private string m_Text = "";
	
	void ExpansionMenuDialogContent_Text(ExpansionMenuDialogBase dialog)
	{
		m_Dialog = dialog;

		if (!m_TextController)
			m_TextController = ExpansionMenuDialogContent_TextController.Cast(GetController());
	}
	
	override string GetLayoutFile() 
	{
		return "DayZExpansion/Core/GUI/layouts/mvc/dialogs/expansion_menu_dialog_text.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionMenuDialogContent_TextController;
	}
	
	string GetText()
	{
		return m_Text;
	}
	
	void SetText(string text)
	{
		m_Text = text;
	}
	
	void SetContent()
	{	
		m_TextController.Text = GetText();
		m_TextController.NotifyPropertyChanged("Text");
	}
	
	override void OnShow()
	{
		super.OnShow();
		
		SetContent();
	}
};

class ExpansionMenuDialogContent_TextController: Controller
{
	string Text;
}