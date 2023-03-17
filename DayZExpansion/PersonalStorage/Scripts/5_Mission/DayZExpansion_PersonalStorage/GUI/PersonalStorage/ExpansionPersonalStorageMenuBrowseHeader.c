/**
 * ExpansionPersonalStorageMenuBrowseHeader.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionPersonalStorageMenuBrowseHeader: ExpansionScriptView
{
	protected ref ExpansionPersonalStorageMenuBrowseHeaderController m_PersonalStorageMenuListHeaderController;
	protected ref ExpansionPersonalStorageMenu m_PersonalStorageMenu;

	protected ButtonWidget item_name_button;
	protected TextWidget item_name_text;
	protected ImageWidget item_name_icon;

	protected bool m_NameSortState;

	void ExpansionPersonalStorageMenuBrowseHeader(ExpansionPersonalStorageMenu menu)
	{
		Class.CastTo(m_PersonalStorageMenuListHeaderController, GetController());
		m_PersonalStorageMenu = menu;
	}

	override string GetLayoutFile()
	{
		return "DayZExpansion/PersonalStorage/GUI/layouts/expansion_personal_storage_browse_header.layout";
	}

	override typename GetControllerType()
	{
		return ExpansionPersonalStorageMenuBrowseHeaderController;
	}

	void OnNameSortClick()
	{
		m_NameSortState = !m_NameSortState;

		if (m_NameSortState)
		{
			item_name_icon.ClearFlags(WidgetFlags.FLIPU);
			m_PersonalStorageMenu.Browse_Filter_ClassNameZA();
		}
		else
		{
			item_name_icon.SetFlags(WidgetFlags.FLIPU);
			m_PersonalStorageMenu.Browse_Filter_ClassNameAZ();
		}
	}

	override void OnShow()
	{
		super.OnShow();

		item_name_icon.SetFlags(WidgetFlags.FLIPU);
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (w != NULL)
		{
			if (w == item_name_button)
			{
				item_name_button.SetColor(ARGB(255, 255, 255, 255));
				item_name_text.SetColor(ARGB(255, 0, 0, 0));
				item_name_icon.SetColor(ARGB(255, 0, 0, 0));
				return true;
			}
		}

		return false;
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (w != NULL)
		{
			if (w == item_name_button)
			{
				item_name_button.SetColor(ARGB(255, 0, 0, 0));
				item_name_text.SetColor(ARGB(255, 255, 255, 255));
				item_name_icon.SetColor(ARGB(255, 255, 255, 255));
				return true;
			}
		}

		return false;
	}
};
class ExpansionPersonalStorageMenuBrowseHeaderController: ExpansionViewController {};