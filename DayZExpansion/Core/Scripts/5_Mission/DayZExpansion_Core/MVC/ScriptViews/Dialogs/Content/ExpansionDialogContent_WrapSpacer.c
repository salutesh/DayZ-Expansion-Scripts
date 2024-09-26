/**
 * ExpansionDialogContent_TextScroller.c
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
class ExpansionDialogContent_WrapSpacer: ExpansionDialogContentBase
{
	ref ExpansionDialogContent_WrapSpacerController m_WrapSpacerController;

	void ExpansionDialogContent_WrapSpacer(ExpansionDialogBase dialog)
	{
		m_Dialog = dialog;

		if (!m_WrapSpacerController)
			m_WrapSpacerController = ExpansionDialogContent_WrapSpacerController.Cast(GetController());
	}

	override string GetLayoutFile()
	{
		return "DayZExpansion/Core/GUI/layouts/mvc/dialogs/expansion_dialog_wrapspacer.layout";
	}

	override typename GetControllerType()
	{
		return ExpansionDialogContent_WrapSpacerController;
	}

	void AddSpacerContent(ExpansionScriptView content)
	{
		m_WrapSpacerController.SpacerContent.Insert(content);
	}
};

class ExpansionDialogContent_WrapSpacerController: ExpansionViewController
{
	ref ObservableCollection<ref ExpansionScriptView> SpacerContent = new ObservableCollection<ref ExpansionScriptView>(this);
}

class ExpansionDialogContent_WrapSpacer_Entry: ExpansionScriptView
{
	ref ExpansionDialogContent_WrapSpacer_EntryController m_EntryController;
	ExpansionDialogContent_WrapSpacer m_WrapSpacerElement;

	protected RichTextWidget entry;
	string m_Text;

	void ExpansionDialogContent_WrapSpacer_Entry(ExpansionDialogContent_WrapSpacer wrapSpacerElement, string text)
	{
		m_WrapSpacerElement = wrapSpacerElement;
		m_Text = text;

		if (!m_EntryController)
			m_EntryController = ExpansionDialogContent_WrapSpacer_EntryController.Cast(GetController());

		SetView();
	}

	override string GetLayoutFile()
	{
		return "DayZExpansion/Core/GUI/layouts/mvc/dialogs/expansion_dialog_wrapspacer_entry.layout";
	}

	override typename GetControllerType()
	{
		return ExpansionDialogContent_WrapSpacer_EntryController;
	}

	void SetView()
	{
		m_EntryController.EntryText = m_Text;
		m_EntryController.NotifyPropertyChanged("EntryText");
	}

	ExpansionDialogContent_WrapSpacer GetWrapSpacerElement()
	{
		return m_WrapSpacerElement;
	}

	void SetTextColor(int color)
	{
		entry.SetColor(color);
	}
}

class ExpansionDialogContent_WrapSpacer_EntryController: ExpansionViewController
{
	string EntryText;
};
#endif
