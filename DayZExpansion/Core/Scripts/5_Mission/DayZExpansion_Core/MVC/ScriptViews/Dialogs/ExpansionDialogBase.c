/**
 * ExpansionScriptView.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONUI
class ExpansionDialogContentSpacer: ExpansionDialogContentBase
{
	void ExpansionDialogContentSpacer(ExpansionDialogBase dialog)
	{
		m_Dialog = dialog;
	}

	override string GetLayoutFile()
	{
		return "DayZExpansion/Core/GUI/layouts/mvc/dialogs/expansion_dialog_content_spacer.layout";
	}
};

class ExpansionDialogBase: ExpansionScriptView
{
	Widget dialog_base_header;
	TextWidget dialog_base_title;
	Widget dialog_base_footer;
	ButtonWidget dialog_close_button;
	ImageWidget dialog_base_backround;
	WrapSpacerWidget dialog_body_content;

	ref ScriptView m_ParentView;
	ref ExpansionDialogBaseController m_DialogBaseController;

	void ExpansionDialogBase(ScriptView parentView)
	{
		m_ParentView = parentView;

		if (!m_DialogBaseController)
			m_DialogBaseController = ExpansionDialogBaseController.Cast(GetController());

		SetBaseDialogView();
	}
	
	override string GetLayoutFile()
	{
		return "DayZExpansion/Core/GUI/layouts/mvc/dialogs/expansion_dialog_base.layout";
	}

	override typename GetControllerType()
	{
		return ExpansionDialogBaseController;
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

	void AddContent(ExpansionDialogContentBase content)
	{
		m_DialogBaseController.DialogContents.Insert(content);
		m_DialogBaseController.DialogContents.Insert(new ExpansionDialogContentSpacer(this));
	}

	void AddButton(ExpansionDialogButtonBase button)
	{
		m_DialogBaseController.DialogButtons.Insert(button);
	}

	ScriptView GetParentView()
	{
		return m_ParentView;
	}
};

class ExpansionDialogBaseController: ExpansionViewController
{
	ref ObservableCollection<ref ExpansionDialogContentBase> DialogContents = new ObservableCollection<ref ExpansionDialogContentBase>(this);
	ref ObservableCollection<ref ExpansionDialogButtonBase> DialogButtons = new ObservableCollection<ref ExpansionDialogButtonBase>(this);
	string DialogTitle;
};

class ExpansionDialogButtonBase: ExpansionScriptView
{
	ref ExpansionDialogBase m_Dialog;

	void ExpansionDialogButtonBase(ExpansionDialogBase dialog)
	{
		m_Dialog = dialog;
	}

	ExpansionDialogBase GetDialog()
	{
		return m_Dialog;
	}
};

class ExpansionDialogContentBase: ExpansionScriptView
{
	ref ExpansionDialogBase m_Dialog;

	void ExpansionDialogContentBase(ExpansionDialogBase dialog)
	{
		m_Dialog = dialog;
	}

	ExpansionDialogBase GetDialog()
	{
		return m_Dialog;
	}
};
#endif
