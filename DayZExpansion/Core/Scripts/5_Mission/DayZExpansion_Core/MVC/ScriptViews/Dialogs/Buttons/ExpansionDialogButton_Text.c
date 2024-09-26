/**
 * ExpansionDialogButton_Text.c
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
class ExpansionDialogButton_Text: ExpansionDialogButtonBase
{
	ref ExpansionDialogButton_TextController m_TextButtonController;

	string m_Text;

	TextWidget dialog_text;
	ButtonWidget dialog_button;

	void ExpansionDialogButton_Text(ExpansionDialogBase dialog)
	{
		m_Dialog = dialog;

		if (!m_TextButtonController)
			m_TextButtonController = ExpansionDialogButton_TextController.Cast(GetController());
	}

	override string GetLayoutFile()
	{
		return "DayZExpansion/Core/GUI/layouts/mvc/dialogs/expansion_dialog_button_text.layout";
	}

	override typename GetControllerType()
	{
		return ExpansionDialogButton_TextController;
	}

	string GetButtonText()
	{
		return m_Text;
	}

	void SetButtonText(string text)
	{
		m_Text = text;
	}

	void SetContent()
	{
		m_TextButtonController.ButtonText = GetButtonText();
		m_TextButtonController.NotifyPropertyChanged("ButtonText");
	}

	void OnButtonClick();

	override void OnShow()
	{
		super.OnShow();

		SetContent();
	}

	void SetTextColor(int color)
	{
		dialog_text.SetColor(color);
	}
};

class ExpansionDialogButton_TextController: ExpansionViewController
{
	string ButtonText;
};
#endif
