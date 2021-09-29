/**
 * ExpansionDialogContent_TextScroller.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMenuDialogContent_WrapSpacer: ExpansionMenuDialogContentBase 
{
	ref ExpansionMenuDialogContent_WrapSpacerController m_WrapSpacerController;
	
	void ExpansionMenuDialogContent_WrapSpacer(ExpansionMenuDialogBase dialog)
	{
		m_Dialog = dialog;

		if (!m_WrapSpacerController)
			m_WrapSpacerController = ExpansionMenuDialogContent_WrapSpacerController.Cast(GetController());
	}
	
	override string GetLayoutFile() 
	{
		return "DayZExpansion/Core/GUI/layouts/mvc/dialogs/expansion_menu_dialog_wrapspacer.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionMenuDialogContent_WrapSpacerController;
	}
	
	void AddSpacerContent(ExpansionScriptView content)
	{
		m_WrapSpacerController.SpacerContent.Insert(content);
	}
};

class ExpansionMenuDialogContent_WrapSpacerController: ExpansionViewController
{
	ref ObservableCollection<ref ExpansionScriptView> SpacerContent = new ObservableCollection<ref ExpansionScriptView>(this);
}

class ExpansionMenuDialogContent_WrapSpacer_Entry: ExpansionScriptView
{
	ref ExpansionMenuDialogContent_WrapSpacer_EntryController m_EntryController;
	ExpansionMenuDialogContent_WrapSpacer m_WrapSpacerElement;
	
	protected RichTextWidget entry;
	string m_Text;
	
	// ------------------------------------------------------------
	// ExpansionMenuDialogContent_WrapSpacer_Entry Constructor
	// ------------------------------------------------------------
	void ExpansionMenuDialogContent_WrapSpacer_Entry(ExpansionMenuDialogContent_WrapSpacer wrapSpacerElement, string text)
	{
		m_WrapSpacerElement = wrapSpacerElement;
		m_Text = text;
		
		if (!m_EntryController)
			m_EntryController = ExpansionMenuDialogContent_WrapSpacer_EntryController.Cast(GetController());
				
		SetView();
	}
	
	// ------------------------------------------------------------
	// ExpansionMenuDialogContent_WrapSpacer_Entry GetLayoutFile
	// ------------------------------------------------------------
	override string GetLayoutFile() 
	{
		return "DayZExpansion/Core/GUI/layouts/mvc/dialogs/expansion_menu_dialog_wrapspacer_entry.layout";
	}
	
	// ------------------------------------------------------------
	// ExpansionMenuDialogContent_WrapSpacer_Entry GetControllerType
	// ------------------------------------------------------------
	override typename GetControllerType() 
	{
		return ExpansionMenuDialogContent_WrapSpacer_EntryController;
	}
	
	// ------------------------------------------------------------
	// ExpansionMenuDialogContent_WrapSpacer_Entry SetView
	// ------------------------------------------------------------
	void SetView()
	{
		m_EntryController.EntryText = m_Text;
		m_EntryController.NotifyPropertyChanged("EntryText");
	}
	
	// ------------------------------------------------------------
	// ExpansionMenuDialogContent_WrapSpacer_Entry GetWrapSpacerElement
	// ------------------------------------------------------------
	ExpansionMenuDialogContent_WrapSpacer GetWrapSpacerElement()
	{
		return m_WrapSpacerElement;
	}
	
	// ------------------------------------------------------------
	// ExpansionMenuDialogContent_WrapSpacer_Entry SetTextColor
	// ------------------------------------------------------------	
	void SetTextColor(int color)
	{
		entry.SetColor(color);
	}
}

class ExpansionMenuDialogContent_WrapSpacer_EntryController: ExpansionViewController
{
	string EntryText;
}