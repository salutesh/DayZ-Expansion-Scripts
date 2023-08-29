/**
 * ExpansionGarageMenuInfo.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionGarageMenuInfo: ExpansionScriptView
{
	protected ref ExpansionGarageMenuInfoController m_GarageInfoController;
	protected MultilineTextWidget info_text;

	void ExpansionGarageMenuInfo(string text, int color, Widget parent)
	{
		Class.CastTo(m_GarageInfoController, GetController());

		m_GarageInfoController.InfoText = text;
		m_GarageInfoController.NotifyPropertyChanged("InfoText");

		info_text.SetColor(color);

		parent.AddChild(GetLayoutRoot());
		parent.Update();
	}

	override string GetLayoutFile()
	{
		return "DayZExpansion/Garage/GUI/layouts/expansion_garage_menu_info_entry.layout";
	}

	override typename GetControllerType()
	{
		return ExpansionGarageMenuInfoController;
	}
};

class ExpansionGarageMenuInfoController: ExpansionViewController
{
	string InfoText;
};