/**
 * ExpansionGarageMenuDialog.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionGarageMenuDialog: ExpansionDialogBase
{
	ref ExpansionDialogContent_Text m_Text;
	ref ExpansionGarageMenuDialog_Confirm m_ConfirmButton;
	ExpansionGarageMenu m_GarageMenu;

	void ExpansionGarageMenuDialog(ScriptView parentView)
	{
		m_ParentView = parentView;

		if (!m_GarageMenu)
			m_GarageMenu = ExpansionGarageMenu.Cast(m_ParentView);

		if (!m_ConfirmButton)
		{
			m_ConfirmButton = new ExpansionGarageMenuDialog_Confirm(this);
			AddButton(m_ConfirmButton);
			m_ConfirmButton.Show();
		}
	}

	void SetText(string text)
	{
		if (!m_Text)
		{
			m_Text = new ExpansionDialogContent_Text(this);
			AddContent(m_Text);
			m_Text.SetText(text);
			m_Text.SetTextColor(ARGB(255, 49, 129, 140));
			m_Text.Show();
		}
	}

	override string GetDialogTitle()
	{
		return "#STR_EXPANSION_GARAGE_MENU_TITLE";
	}
};

class ExpansionGarageMenuDialog_Confirm: ExpansionDialogButton_Text
{
	ExpansionGarageMenuDialog m_GarageDialog;
	ExpansionGarageMenu m_GarageMenu;

	void ExpansionGarageMenuDialog_Confirm(ExpansionDialogBase dialog)
	{
		if (!m_GarageDialog)
			m_GarageDialog = ExpansionGarageMenuDialog.Cast(GetDialog());

		if (!m_GarageMenu)
			m_GarageMenu = ExpansionGarageMenu.Cast(m_GarageDialog.GetParentView());

		SetButtonText("OK");
		SetTextColor(ARGB(255, 220, 220, 220));
	}

	override void OnButtonClick()
	{
		if (m_GarageMenu)
		{
			m_GarageMenu.OnConfirmDialog();
		}

		m_GarageDialog.Hide();
	}
};