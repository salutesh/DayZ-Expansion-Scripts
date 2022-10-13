/**
 * ExpansionNewsFeedLink.c
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
class ExpansionNewsFeedLink: ExpansionScriptView
{
	protected ref ExpansionNewsFeedLinkController m_NewsFeedLinkController;

	protected string m_Label;
	protected string m_Icon;
	protected string m_URL;

	protected ImageWidget entry_icon;
	protected TextWidget label;
	protected ButtonWidget button;

	void ExpansionNewsFeedLink(string label, string icon, string url)
	{
		Class.CastTo(m_NewsFeedLinkController, GetController());

		m_Label = label;
		m_Icon = icon;
		m_URL = url;

		SetView();
	}

	override string GetLayoutFile()
	{
		return "DayZExpansion/GUI/layouts/ui/expansion_newsfeed_link.layout";
	}

	override typename GetControllerType()
	{
		return ExpansionNewsFeedLinkController;
	}
	
	protected void SetView()
	{
		m_NewsFeedLinkController.EntryIcon = m_Icon;
		m_NewsFeedLinkController.NotifyPropertyChanged("EntryIcon");

		m_NewsFeedLinkController.Label = m_Label;
		m_NewsFeedLinkController.NotifyPropertyChanged("Label");
	}

	void OnButtonClick()
	{
		GetGame().OpenURL(m_URL);
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (w == button)
		{
			entry_icon.SetColor(ARGB(255,1,1,1));
			label.SetColor(ARGB(255,1,1,1));
		}

		return super.OnMouseEnter(w, x, y);
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (w == button)
		{
			entry_icon.SetColor(ARGB(255,220,220,220));
			label.SetColor(ARGB(255,220,220,220));
		}

		return super.OnMouseLeave(w, enterW, x, y);
	}
};

class ExpansionNewsFeedLinkController: ExpansionViewController
{
	string EntryIcon;
	string Label;
};
#endif