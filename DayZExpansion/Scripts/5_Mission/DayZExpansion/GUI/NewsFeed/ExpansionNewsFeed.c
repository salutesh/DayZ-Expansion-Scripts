/**
 * ExpansionNewsFeed.c
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
class ExpansionNewsFeed: ExpansionScriptView
{
	protected ref ExpansionNewsFeedController m_NewsFeedController;

	void ExpansionNewsFeed()
	{
		Class.CastTo(m_NewsFeedController, GetController());
		SetView();
	}

	void SetView()
	{
		int i;
		
		//! Add text entries from social media settings
		m_NewsFeedController.TextEntries.Clear();
		for (i = 0; i < GetExpansionSettings().GetSocialMedia().NewsFeedTexts.Count(); i++)
		{
			ExpansionNewsFeedTextSetting textSetting = GetExpansionSettings().GetSocialMedia().NewsFeedTexts[i];
			if (!textSetting)
				continue;

			ExpansionNewsFeedText textEntry = new ExpansionNewsFeedText(textSetting.GetTitle(), textSetting.GetText());
			if (textEntry)
				m_NewsFeedController.TextEntries.Insert(textEntry);
		}
		
		//! Add link entries from social media settings
		m_NewsFeedController.LinkEntries.Clear();
		for (i = 0; i < GetExpansionSettings().GetSocialMedia().NewsFeedLinks.Count(); i++)
		{
			ExpansionNewsFeedLinkSetting linkSetting = GetExpansionSettings().GetSocialMedia().NewsFeedLinks[i];
			if (!linkSetting)
				continue;

			ExpansionNewsFeedLink linkEntry = new ExpansionNewsFeedLink(linkSetting.GetLabel(), linkSetting.GetIcon(), linkSetting.GetURL());
			if (linkEntry)
				m_NewsFeedController.LinkEntries.Insert(linkEntry);
		}
	}

	override string GetLayoutFile()
	{
		return "DayZExpansion/GUI/layouts/ui/expansion_newsfeed.layout";
	}

	override typename GetControllerType()
	{
		return ExpansionNewsFeedController;
	}
};

class ExpansionNewsFeedController: ExpansionViewController
{
	ref ObservableCollection<ref ExpansionNewsFeedText> TextEntries = new ObservableCollection<ref ExpansionNewsFeedText>(this);
	ref ObservableCollection<ref ExpansionNewsFeedLink> LinkEntries = new ObservableCollection<ref ExpansionNewsFeedLink>(this);
};
#endif