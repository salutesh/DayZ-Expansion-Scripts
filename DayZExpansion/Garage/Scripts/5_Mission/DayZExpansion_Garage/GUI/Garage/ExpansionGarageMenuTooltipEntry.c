/**
 * ExpansionGarageMenuTooltipEntry.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionGarageMenuTooltipEntry: ExpansionScriptView
{
	protected ref ExpansionGarageMenuTooltipEntryController m_TooltipEntryController;
	protected RichTextWidget tooltip_entry_text;
	protected string m_Text;
	protected int m_Color;

	void ExpansionGarageMenuTooltipEntry(string text, int color)
	{
		m_Text = text;
		m_Color = color;

		if (!m_TooltipEntryController)
			m_TooltipEntryController = ExpansionGarageMenuTooltipEntryController.Cast(GetController());

		SetView();
	}

	override string GetLayoutFile()
	{
		return "DayZExpansion/Garage/GUI/layouts/expansion_garage_menu_tooltip_entry.layout";
	}

	override typename GetControllerType()
	{
		return ExpansionGarageMenuTooltipEntryController;
	}

	void SetView()
	{
		m_TooltipEntryController.EntryText = m_Text;
		m_TooltipEntryController.NotifyPropertyChanged("EntryText");
		tooltip_entry_text.SetColor(m_Color);
	}
};

class ExpansionGarageMenuTooltipEntryController: ExpansionViewController
{
	string EntryText;
};