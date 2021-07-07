/**
 * MainMenuExpansionNewsfeed.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionNewsfeed extends ScriptedWidgetEventHandler
{
	protected Widget m_Root;
	protected Widget m_Feedback;
	protected TextWidget m_FeedbackText;
	protected Widget m_Twitter;
	protected TextWidget m_TwitterText;
	protected Widget m_Youtube;
	protected TextWidget m_YoutubeText;
	protected Widget m_Discord;
	protected TextWidget m_DiscordText;
	protected Widget m_Wiki;
	protected TextWidget m_WikiText;
	
	protected RichTextWidget m_MainText1;
	protected RichTextWidget m_MainText2;
	protected RichTextWidget m_MainText3;
	protected RichTextWidget m_SecText1;
	protected RichTextWidget m_SecText2;
	protected RichTextWidget m_SecText3;

	protected static const string EXPANSION_DISCORD_URL = "https://discord.gg/WabhFUa";
	protected static const string EXPANSION_FEEDBACK_URL = "https://exp.thurston.pw/";
	protected static const string EXPANSION_TWITTER_HANDLE = "DayZExpansion";
	protected static const string EXPANSION_YOUTUBE_URL = "https://www.youtube.com/channel/UCZNgSvIEWfru963tQZOAVJg";
	protected static const string EXPANSION_WIKI_URL = "https://github.com/salutesh/DayZ-Expansion-Scripts/wiki";
	protected static string m_DiscordURL = EXPANSION_DISCORD_URL;
	protected static string m_FeedbackURL = EXPANSION_FEEDBACK_URL;
	protected static string m_TwitterHandle = EXPANSION_TWITTER_HANDLE;
	protected static string m_YoutubeURL = EXPANSION_YOUTUBE_URL;
	protected static string m_WikiURL = EXPANSION_WIKI_URL;
	
	// ------------------------------------------------------------
	// ExpansionNewsfeed Contructor
	// ------------------------------------------------------------	
	void ExpansionNewsfeed( Widget parent )
	{				
		m_Root				= GetGame().GetWorkspace().CreateWidgets("DayZExpansion/GUI/layouts/ui/expansion_newsfeed.layout", parent);
		
		m_Feedback			= Widget.Cast(m_Root.FindAnyWidget("feedback_tracker"));
		m_FeedbackText		= TextWidget.Cast(m_Feedback.FindWidget("feedback_tracker_text" ));
		m_Twitter			= Widget.Cast(m_Root.FindAnyWidget("twitter"));
		m_TwitterText		= TextWidget.Cast(m_Twitter.FindWidget("twitter_text" ));
		m_Youtube			= Widget.Cast(m_Root.FindAnyWidget("youtube"));
		m_YoutubeText		= TextWidget.Cast(m_Youtube.FindWidget("youtube_text" ));
		m_Discord			= Widget.Cast(m_Root.FindAnyWidget("discord"));
		m_DiscordText		= TextWidget.Cast(m_Discord.FindWidget("discord_text" ));
		m_Wiki				= Widget.Cast(m_Root.FindAnyWidget("wiki"));
		m_WikiText			= TextWidget.Cast(m_Wiki.FindAnyWidget("wiki_text"));
		
		m_MainText1			= RichTextWidget.Cast(m_Root.FindAnyWidget("InfoT1"));
		m_MainText2			= RichTextWidget.Cast(m_Root.FindAnyWidget("InfoT2"));
		m_MainText3			= RichTextWidget.Cast(m_Root.FindAnyWidget("InfoT3"));
		m_SecText1			= RichTextWidget.Cast(m_Root.FindAnyWidget("InfoC1"));
		m_SecText2			= RichTextWidget.Cast(m_Root.FindAnyWidget("InfoC2"));
		m_SecText3			= RichTextWidget.Cast(m_Root.FindAnyWidget("InfoC3"));
		
		SetNewsfeed();
		
		m_Root.SetHandler( this );
	}
	
	// ------------------------------------------------------------
	// ShowNewsfeed
	// ------------------------------------------------------------	
	void SetNewsfeed()
	{		
		string client_version = "";
		
		if(GetDayZGame())
			client_version = GetDayZGame().GetExpansionClientVersion();
		
		//! Section 1
		m_MainText1.SetText("DayZ Expansion Mod Version " + client_version);
		m_MainText1.Update();
	
		m_SecText1.SetText("Join the DayZ Expansion Mod Discord for a detailed list of the latest changes and additions.");
		m_SecText1.Update();
		
		//! Section 2
		m_MainText2.SetText("Help to improve the DayZ Expansion Mod!");
		m_MainText2.Update();
		
		m_SecText2.SetText("Please consider using the DayZ Expansion Mod Discord to report bugs and issues with the mod or to provide feedback and suggestions to our team.");
		m_SecText2.Update();
		
		//! Section 3
		m_MainText3.Show(false);
		//m_MainText3.SetText("");
		//m_MainText3.Update();
		
		m_SecText3.Show(false);
		//m_SecText3.SetText("");
		//m_SecText3.Update();

		m_Youtube.Show(false);

		//! Use social media settings.
		//! This will work if we are currently connected to the server or have been connected previously without closing the game.
		if ( GetExpansionSettings().GetSocialMedia() )
		{
			m_DiscordURL = GetExpansionSettings().GetSocialMedia().Discord;
			if ( m_DiscordURL != EXPANSION_DISCORD_URL )
			{
				m_DiscordText.SetText("Server Discord");
				m_DiscordText.Update();
				m_Discord.Show(m_DiscordURL != "");
			}

			m_FeedbackURL = GetExpansionSettings().GetSocialMedia().Forums;
			if ( m_FeedbackURL != EXPANSION_FEEDBACK_URL )
			{
				m_FeedbackText.SetText("Server Forums");
				m_FeedbackText.Update();
				m_Feedback.Show(m_FeedbackURL != "");
			}

			m_TwitterHandle = GetExpansionSettings().GetSocialMedia().Twitter;
			if ( m_TwitterHandle != EXPANSION_TWITTER_HANDLE )
			{
				m_TwitterText.SetText("@" + m_TwitterHandle);
				m_TwitterText.Update();
				m_Twitter.Show(m_TwitterHandle != "");
			}

			m_YoutubeURL = GetExpansionSettings().GetSocialMedia().YouTube;
			if ( m_YoutubeURL != EXPANSION_YOUTUBE_URL )
			{
				m_YoutubeText.SetText("YouTube");
				m_YoutubeText.Update();
				m_Youtube.Show(m_YoutubeURL != "");
			}

			m_WikiURL = GetExpansionSettings().GetSocialMedia().Homepage;
			if ( m_WikiURL != EXPANSION_WIKI_URL )
			{
				m_WikiText.SetText("Homepage");
				m_WikiText.Update();
				m_Wiki.Show(m_WikiURL != "");
			}
		}
	}
	
	// ------------------------------------------------------------
	// OpenDiscord
	// ------------------------------------------------------------	
	void OpenDiscord()
	{
		GetGame().OpenURL(m_DiscordURL);
	}

	// ------------------------------------------------------------
	// OpenFeedback
	// ------------------------------------------------------------	
	void OpenFeedback()
	{
		GetGame().OpenURL(m_FeedbackURL);
	}
	
	// ------------------------------------------------------------
	// OpenTwitter
	// ------------------------------------------------------------		
	void OpenTwitter()
	{
		GetGame().OpenURL("https://twitter.com/" + m_TwitterHandle);
	}
	
	// ------------------------------------------------------------
	// OpenYoutube
	// ------------------------------------------------------------	
	void OpenYoutube()
	{
		GetGame().OpenURL(m_YoutubeURL);
	}
	
	// ------------------------------------------------------------
	// OpenWiki
	// ------------------------------------------------------------	
	void OpenWiki()
	{
		GetGame().OpenURL(m_WikiURL);
	}
	
	// ------------------------------------------------------------
	// Override OnClick
	// ------------------------------------------------------------		
	override bool OnClick(Widget w, int x, int y, int button)
	{
		if(button == MouseState.LEFT)
		{	
			switch(w)
			{
				case m_Feedback:
					OpenFeedback();
				break;
				case m_Twitter:
					OpenTwitter();
				break;
				case m_Youtube:
					OpenYoutube();
				break;
				case m_Discord:
					OpenDiscord();
				break;
				case m_Wiki:
					OpenWiki();
				break;
			}
			
		}
		return false;
	}
	
	// ------------------------------------------------------------
	// Override OnMouseEnter
	// ------------------------------------------------------------	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if(IsFocusable(w))
		{
			ColorRed(w, x, y);
			return true;
		}
		return false;
	}
	
	// ------------------------------------------------------------
	// Override OnMouseLeave
	// ------------------------------------------------------------	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if(IsFocusable(w))
		{
			ColorWhite(w, enterW, x, y);
			return true;
		}
		return false;
	}
	
	// ------------------------------------------------------------
	// Override OnFocus
	// ------------------------------------------------------------
	override bool OnFocus(Widget w, int x, int y)
	{
		if(IsFocusable(w))
		{
			ColorRed(w, x, y);
			return true;
		}
		return false;
	}
	
	// ------------------------------------------------------------
	// Override OnFocusLost
	// ------------------------------------------------------------
	override bool OnFocusLost(Widget w, int x, int y)
	{
		if(IsFocusable(w))
		{
			ColorWhite(w, null, x, y);
			return true;
		}
		return false;
	}
	
	// ------------------------------------------------------------
	// IsFocusable
	// ------------------------------------------------------------
	bool IsFocusable( Widget w )
	{
		if(w)
		{
			return (w == m_Feedback || w == m_Twitter || w == m_Youtube || w == m_Discord || w == m_Wiki);
		}
		return false;
	}
	
	//Coloring functions (Until WidgetStyles are useful)
	// ------------------------------------------------------------
	// ColorRed
	// ------------------------------------------------------------
	void ColorRed(Widget w, int x, int y)
	{
		SetFocus(w);
		if(w.IsInherited(ButtonWidget))
		{
			ButtonWidget button = ButtonWidget.Cast(w);
			button.SetTextColor(ARGB( 255, 255, 0, 0 ));
			button.SetAlpha(0.9);
		}
		
		TextWidget text	= TextWidget.Cast(w.FindWidget(w.GetName() + "_text"));
		TextWidget text2 = TextWidget.Cast(w.FindWidget(w.GetName() + "_text_1"));
		ImageWidget image = ImageWidget.Cast(w.FindWidget( w.GetName() + "_image"));
		
		if(text)
		{
			text.SetColor(ARGB(255, 255, 0, 0));
		}
		
		if(text2)
		{
			text2.SetColor(ARGB(255, 255, 0, 0));
		}
		
		if(image)
		{
			image.SetColor(ARGB(255, 255, 0, 0));
		}
	}
	
	// ------------------------------------------------------------
	// ColorWhite
	// ------------------------------------------------------------
	void ColorWhite( Widget w, Widget enterW, int x, int y )
	{
		if( w.IsInherited( ButtonWidget ) )
		{
			ButtonWidget button = ButtonWidget.Cast(w);
			button.SetTextColor(ARGB( 255, 255, 255, 255));
			button.SetAlpha(0.75);
		}
		
		TextWidget text	= TextWidget.Cast(w.FindWidget(w.GetName() + "_text"));
		TextWidget text2 = TextWidget.Cast(w.FindWidget(w.GetName() + "_text_1"));
		ImageWidget image = ImageWidget.Cast( w.FindWidget(w.GetName() + "_image"));
		
		if(text)
		{
			text.SetColor(ARGB(255, 255, 255, 255));
		}
		
		if(text2)
		{
			text2.SetColor(ARGB(255, 255, 255, 255));
		}
		
		if(image)
		{
			image.SetColor(ARGB(255, 255, 255, 255));
		}
	}
	
	// ------------------------------------------------------------
	// ShowNewsfeed
	// ------------------------------------------------------------
	void ShowNewsfeed(bool state)
	{
		m_Root.Show(state);
	}
	
	// ------------------------------------------------------------
	// GetRoot
	// ------------------------------------------------------------
	Widget GetRoot()
	{
		return m_Root;
	}
}
