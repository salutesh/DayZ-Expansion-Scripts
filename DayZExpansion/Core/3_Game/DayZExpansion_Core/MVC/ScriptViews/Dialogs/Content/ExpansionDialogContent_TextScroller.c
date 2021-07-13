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

#ifdef DABS_FRAMEWORK
class ExpansionDialogContent_TextScroller: ExpansionDialogContentBase
{
	ref ExpansionDialogContent_TextScrollerController m_TextScrollerController;	
	private string m_Text = "";
	
	void ExpansionDialogContent_TextScroller(ExpansionDialogBase dialog)
	{
		m_Dialog = dialog;

		if (!m_TextScrollerController)
			m_TextScrollerController = ExpansionDialogContent_TextScrollerController.Cast(GetController());
	}
	
	override string GetLayoutFile() 
	{
		return "DayZExpansion/Core/GUI/layouts/mvc/dialogs/expansion_dialog_content_textscroller.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionDialogContent_TextScrollerController;
	}
	
	string GetDialogText()
	{
		return m_Text;
	}
	
	void SetDialogText(string text)
	{
		m_Text = text;
	}
	
	void SetContent()
	{		
		m_TextScrollerController.DialogText = GetDialogText();
		m_TextScrollerController.NotifyPropertyChanged("DialogText");
	}
	
	override void OnShow()
	{
		super.OnShow();
		
		SetContent();
	}
};

class ExpansionDialogContent_TextScrollerController: ExpansionViewController
{
	string DialogText;
}

class ExpansionMenuDialogContent_TextScroller: ExpansionMenuDialogContentBase
{
	ref ExpansionMenuDialogContent_TextScrollerController m_TextScrollerController;	
	private string m_Text = "";
	
	void ExpansionDialogContent_TextScroller(ExpansionMenuDialogBase dialog)
	{
		m_Dialog = dialog;

		if (!m_TextScrollerController)
			m_TextScrollerController = ExpansionMenuDialogContent_TextScrollerController.Cast(GetController());
	}
	
	override string GetLayoutFile() 
	{
		return "DayZExpansion/Core/GUI/layouts/mvc/dialogs/expansion_menu_dialog_content_textscroller.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionMenuDialogContent_TextScrollerController;
	}
	
	string GetDialogText()
	{
		return m_Text;
	}
	
	void SetDialogText(string text)
	{
		m_Text = text;
	}
	
	void SetContent()
	{		
		m_TextScrollerController.DialogText = GetDialogText();
		m_TextScrollerController.NotifyPropertyChanged("DialogText");
	}
	
	override void OnShow()
	{
		super.OnShow();
		
		SetContent();
	}
};

class ExpansionMenuDialogContent_TextScrollerController: ExpansionViewController
{
	string DialogText;
}
#endif