/**
 * ExpansionScriptView.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMenuDialogBase: ExpansionScriptView
{
	Widget dialog_base_header;
	TextWidget dialog_base_title;
	Widget dialog_base_footer;
	ButtonWidget dialog_close_button;
	
	ref ExpansionScriptViewMenu m_ParentMenu;
	ref ExpansionMenuDialogBaseController m_DialogBaseController;
	
	void ExpansionMenuDialogBase(ExpansionScriptViewMenu parentMenu)
	{
		m_ParentMenu = parentMenu;
		
		if (!m_DialogBaseController)
			m_DialogBaseController = ExpansionMenuDialogBaseController.Cast(GetController());
		
		SetBaseDialogView();
	}
	
	override string GetLayoutFile() 
	{
		return "DayZExpansion/Core/GUI/layouts/mvc/dialogs/expansion_menu_dialog_base.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionMenuDialogBaseController;
	}
	
	string GetDialogTitle()
	{
		return "";
	}
	
	bool HasHeader()
	{
		return false;
	}
	
	bool HasFooter()
	{
		return false;
	}
	
	bool HasCloseButton()
	{
		return false;
	}
	
	void SetBaseDialogView()
	{
		if (!HasHeader())
			dialog_base_header.Show(false);
		
		if (!HasCloseButton() && HasHeader())
			dialog_close_button.Show(false);
		
		if (GetDialogTitle() == "")
		{
			dialog_base_title.Show(false);
		} 
		else
		{
			m_DialogBaseController.DialogTitle = GetDialogTitle();
			m_DialogBaseController.NotifyPropertyChanged("DialogTitle");
		}
		
		if (!HasFooter())
			dialog_base_footer.Show(false);
	}
	
	void AddContent(ExpansionMenuDialogContentBase content)
	{
		m_DialogBaseController.DialogContents.Insert(content);
	}
	
	void AddButton(ExpansionMenuDialogButtonBase button)
	{
		m_DialogBaseController.DialogButtons.Insert(button);
	}
	
	ExpansionScriptViewMenu GetParentMenu()
	{
		return m_ParentMenu;
	}
}

class ExpansionMenuDialogBaseController: ExpansionViewController
{
	ref ObservableCollection<ref ExpansionMenuDialogContentBase> DialogContents = new ObservableCollection<ref ExpansionMenuDialogContentBase>(this);
	ref ObservableCollection<ref ExpansionMenuDialogButtonBase> DialogButtons = new ObservableCollection<ref ExpansionMenuDialogButtonBase>(this);
	string DialogTitle;
}

class ExpansionMenuDialogButtonBase: ExpansionScriptView 
{
	ref ExpansionMenuDialogBase m_Dialog;
	
	void ExpansionMenuDialogButtonBase(ExpansionMenuDialogBase dialog)
	{
		m_Dialog = dialog;
	}
	
	ExpansionMenuDialogBase GetDialog()
	{
		return m_Dialog;
	}
};

class ExpansionMenuDialogContentBase: ExpansionScriptView 
{
	ref ExpansionMenuDialogBase m_Dialog;
	
	void ExpansionMenuDialogContentBase(ExpansionMenuDialogBase dialog)
	{
		m_Dialog = dialog;
	}
	
	ExpansionMenuDialogBase GetDialog()
	{
		return m_Dialog;
	}
};