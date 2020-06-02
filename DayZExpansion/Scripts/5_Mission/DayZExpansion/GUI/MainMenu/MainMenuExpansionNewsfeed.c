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

class MainMenuExpansionNewsfeed extends ScriptedWidgetEventHandler
{
	protected Widget		m_Root;
	
	protected Widget		m_Feedback;
	protected Widget		m_ExpansionForum;
	protected Widget		m_Twitter;
	protected Widget		m_Youtube;
	protected Widget		m_Discord;
	protected Widget		m_Github;
	protected Widget		m_Patreon;
	
	protected TextWidget	m_MainText1;
	protected TextWidget	m_MainText2;
	protected TextWidget	m_MainText3;
	protected TextWidget	m_SecText1;
	protected TextWidget	m_SecText2;
	protected TextWidget	m_SecText3;
	
	// ------------------------------------------------------------
	// MainMenuExpansionNewsfeed Contructor
	// ------------------------------------------------------------	
	void MainMenuExpansionNewsfeed( Widget root )
	{
		m_Root				= root;
		
		m_Feedback			= m_Root.FindAnyWidget( "feedback_tracker" );
		m_ExpansionForum	= m_Root.FindAnyWidget( "expansion_forums" );
		m_Twitter			= m_Root.FindAnyWidget( "twitter" );
		m_Youtube			= m_Root.FindAnyWidget( "youtube" );
		m_Discord			= m_Root.FindAnyWidget( "discord" );
		m_Github			= m_Root.FindAnyWidget( "github" );
		m_Patreon			= m_Root.FindAnyWidget( "patreon" );
		
		m_ExpansionForum.Show(false);
		m_Github.Show(false);
		
		m_MainText1			= TextWidget.Cast( m_Root.FindAnyWidget( "InfoT1" ) );
		m_MainText2			= TextWidget.Cast( m_Root.FindAnyWidget( "InfoT2" ) );
		m_MainText3			= TextWidget.Cast( m_Root.FindAnyWidget( "InfoT3" ) );
		m_SecText1			= TextWidget.Cast( m_Root.FindAnyWidget( "InfoC1" ) );
		m_SecText2			= TextWidget.Cast( m_Root.FindAnyWidget( "InfoC2" ) );
		m_SecText3			= TextWidget.Cast( m_Root.FindAnyWidget( "InfoC3" ) );
		
		m_MainText2.Show(false);
		m_SecText2.Show(false);
		
		ShowNewsfeed();
		
		m_Root.SetHandler( this );
	}
	
	// ------------------------------------------------------------
	// ShowNewsfeed
	// ------------------------------------------------------------	
	void ShowNewsfeed()
	{
		m_MainText1.SetText( "#STR_EXPANSION_NEWSFEED_TITLE_1" );
		m_MainText1.Update();
		//m_MainText2.SetText( "#STR_EXPANSION_NEWSFEED_TITLE_2" );
		//m_MainText2.Update();
		m_MainText3.SetText( "#STR_EXPANSION_NEWSFEED_TITLE_3" );
		m_MainText3.Update();
		m_SecText1.SetText( "#STR_EXPANSION_NEWSFEED_TEXT_1" );
		m_SecText1.Update();
		//m_SecText2.SetText( "#STR_EXPANSION_NEWSFEED_TEXT_2" );
		//m_SecText2.Update();
		m_SecText3.SetText( "#STR_EXPANSION_NEWSFEED_TEXT_3" );
		m_SecText3.Update();
	}
	
	// ------------------------------------------------------------
	// HideNewsfeed
	// ------------------------------------------------------------	
	void HideNewsfeed()
	{
		m_Root.Show( false );
	}
	
	// ------------------------------------------------------------
	// OpenDiscord
	// ------------------------------------------------------------	
	void OpenDiscord()
	{
		GetGame().OpenURL( "https://discord.gg/jvqw5yS" );
	}

	// ------------------------------------------------------------
	// OpenFeedback
	// ------------------------------------------------------------	
	void OpenFeedback()
	{
		GetGame().OpenURL( "https://exp.thurston.pw/" );
	}
	
	// ------------------------------------------------------------
	// OpenTwitter
	// ------------------------------------------------------------		
	void OpenTwitter()
	{
		GetGame().OpenURL( "https://twitter.com/dayzexpansion" );
	}
	
	// ------------------------------------------------------------
	// OpenYoutube
	// ------------------------------------------------------------	
	void OpenYoutube()
	{
		GetGame().OpenURL( "https://www.youtube.com/channel/UCZNgSvIEWfru963tQZOAVJg" );
	}
	
	// ------------------------------------------------------------
	// OpenPatreon
	// ------------------------------------------------------------	
	void OpenPatreon()
	{
		GetGame().OpenURL( "https://www.patreon.com/dayzexpansion" );
	}
	
	// ------------------------------------------------------------
	// Override OnClick
	// ------------------------------------------------------------		
	override bool OnClick( Widget w, int x, int y, int button )
	{
		super.OnClick(w, x, y, button);
		
		if( button == MouseState.LEFT )
		{
			if ( w == m_Feedback )
			{
				OpenFeedback();
				return true;
			}
			else if ( w == m_Twitter )
			{
				OpenTwitter();
				return true;
			}
			else if ( w == m_Youtube )
			{
				OpenYoutube();
				return true;
			}
			else if ( w == m_Discord )
			{
				OpenDiscord();
				return true;
			}
			else if ( w == m_Patreon )
			{
				OpenPatreon();
				return true;
			}
		}
		return false;
	}
	
	// ------------------------------------------------------------
	// Override OnMouseEnter
	// ------------------------------------------------------------	
	override bool OnMouseEnter( Widget w, int x, int y )
	{
		if( IsFocusable( w ) )
		{
			ColorRed( w, x, y );
			return true;
		}
		return false;
	}
	
	// ------------------------------------------------------------
	// Override OnMouseLeave
	// ------------------------------------------------------------	
	override bool OnMouseLeave( Widget w, Widget enterW, int x, int y )
	{
		if( IsFocusable( w ) )
		{
			ColorWhite( w, enterW, x, y );
			return true;
		}
		return false;
	}
	
	// ------------------------------------------------------------
	// Override OnFocus
	// ------------------------------------------------------------
	override bool OnFocus( Widget w, int x, int y )
	{
		if( IsFocusable( w ) )
		{
			ColorRed( w, x, y );
			return true;
		}
		return false;
	}
	
	// ------------------------------------------------------------
	// Override OnFocusLost
	// ------------------------------------------------------------
	override bool OnFocusLost( Widget w, int x, int y )
	{
		if( IsFocusable( w ) )
		{
			ColorWhite( w, null, x, y );
			return true;
		}
		return false;
	}
	
	// ------------------------------------------------------------
	// IsFocusable
	// ------------------------------------------------------------
	bool IsFocusable( Widget w )
	{
		if( w )
		{
			return ( w == m_Feedback || w == m_ExpansionForum || w == m_Twitter || w == m_Youtube || w == m_Discord || w == m_Github );
		}
		return false;
	}
	
	//Coloring functions (Until WidgetStyles are useful)
	// ------------------------------------------------------------
	// ColorRed
	// ------------------------------------------------------------
	void ColorRed( Widget w, int x, int y )
	{
		SetFocus( w );
		if( w.IsInherited( ButtonWidget ) )
		{
			ButtonWidget button = ButtonWidget.Cast( w );
			button.SetTextColor( ARGB( 255, 255, 0, 0 ) );
			button.SetAlpha( 0.9 );
		}
		
		TextWidget text		= TextWidget.Cast(w.FindWidget( w.GetName() + "_text" ) );
		TextWidget text2	= TextWidget.Cast(w.FindWidget( w.GetName() + "_text_1" ) );
		ImageWidget image	= ImageWidget.Cast( w.FindWidget( w.GetName() + "_image" ) );
		
		if( text )
		{
			text.SetColor( ARGB( 255, 255, 0, 0 ) );
		}
		
		if( text2 )
		{
			text2.SetColor( ARGB( 255, 255, 0, 0 ) );
		}
		
		if( image )
		{
			image.SetColor( ARGB( 255, 255, 0, 0 ) );
		}
	}
	
	// ------------------------------------------------------------
	// ColorWhite
	// ------------------------------------------------------------
	void ColorWhite( Widget w, Widget enterW, int x, int y )
	{
		if( w.IsInherited( ButtonWidget ) )
		{
			ButtonWidget button = ButtonWidget.Cast( w );
			button.SetTextColor( ARGB( 255, 255, 255, 255 ) );
			button.SetAlpha( 0.75 );
		}
		
		TextWidget text		= TextWidget.Cast(w.FindWidget( w.GetName() + "_text" ) );
		TextWidget text2	= TextWidget.Cast(w.FindWidget( w.GetName() + "_text_1" ) );
		ImageWidget image	= ImageWidget.Cast( w.FindWidget( w.GetName() + "_image" ) );
		
		if( text )
		{
			text.SetColor( ARGB( 255, 255, 255, 255 ) );
		}
		
		if( text2 )
		{
			text2.SetColor( ARGB( 255, 255, 255, 255 ) );
		}
		
		if( image )
		{
			image.SetColor( ARGB( 255, 255, 255, 255 ) );
		}
	}
}
