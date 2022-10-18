/**
 * ExpansionNewsFeedText.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifndef CF_DebugUI
class ExpansionNewsFeedText: ExpansionScriptView
{
	protected ref ExpansionNewsFeedTextController m_NewsFeedTextController;

	protected string m_Title;
	protected string m_Text;

	void ExpansionNewsFeedText(string title, string text)
	{
		Class.CastTo(m_NewsFeedTextController, GetController());

		m_Title = title;
		m_Text = text;

		SetView();
	}

	override string GetLayoutFile()
	{
		return "DayZExpansion/GUI/layouts/ui/expansion_newsfeed_text.layout";
	}

	override typename GetControllerType()
	{
		return ExpansionNewsFeedTextController;
	}

	protected void SetView()
	{
		m_NewsFeedTextController.EntryTitle = m_Title;
		m_NewsFeedTextController.NotifyPropertyChanged("EntryTitle");

		m_NewsFeedTextController.EntryText = m_Text;
		m_NewsFeedTextController.NotifyPropertyChanged("EntryText");
	}
};

class ExpansionNewsFeedTextController: ExpansionViewController
{
	string EntryTitle;
	string EntryText;
};
#endif