/**
 * ExpansionBookTabServerInfoButtonData.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionBookTabServerInfoButton extends ScriptedWidgetEventHandler
{
	Widget m_Root;
	ButtonWidget m_Button;
	ImageWidget m_Icon;
	
	string m_IconPath;		//! Icon that gets displayed on the button
	string m_URL;			//! URL of the website that gets opened when button is clicked
	string m_Tooltip;		//! Tooltip text that gets diplayed when pointing on the button
	int m_Color;			//! Icon color when pointing on the button.
	
	ref ExpansionUITooltip m_TooltipUI;
	
	// ------------------------------------------------------------
	// ExpansionBookTabServerInfoButton Constructor
	// ------------------------------------------------------------
	void ExpansionBookTabServerInfoButton(Widget parent, string icon, string url, string tooltip, int color)
	{
		m_Root	= Widget.Cast( GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/book/expansion_book_bookmark_vert.layout", parent ) );
		m_Button = ButtonWidget.Cast( m_Root.FindAnyWidget( "bookmark_button" ) );
		m_Icon = ImageWidget.Cast( m_Root.FindAnyWidget( "bookmark_icon" ) );
		
		m_IconPath = icon;
		m_URL = url;
		m_Tooltip = tooltip;
		m_Color = color;
		
		SetButton();
		
		m_Root.SetHandler( this );
	}
	
	// ------------------------------------------------------------
	// ExpansionBookTabServerInfoSection Destructor
	// ------------------------------------------------------------
	void ~ExpansionBookTabServerInfoButton()
	{
		if ( m_TooltipUI.IsVisable() )
			m_TooltipUI.HideTooltip();
			
		delete m_TooltipUI;
		delete m_Root;
	}
	
	// ------------------------------------------------------------
	// ExpansionBookTabServerInfoButtonData SetButton
	// ------------------------------------------------------------
	void SetButton()
	{
		m_Icon.LoadImageFile( 0, m_IconPath );
		
		m_TooltipUI = new ExpansionUITooltip(m_Tooltip, ""); 
	}	
	
	// ------------------------------------------------------------
	// ExpansionBookTabServerInfoButtonData OnButtonClick
	// ------------------------------------------------------------
	void OnButtonClick()
	{
		GetGame().OpenURL( m_URL );
	}
	
	// ------------------------------------------------------------
	// Override OnClick
	// ------------------------------------------------------------
	override bool OnClick( Widget w, int x, int y, int button )
	{
		if (m_Button && w == m_Button)
		{
			OnButtonClick();
			return true;
		}
		
		return super.OnClick(w, x, y, button);
	}
	
	// ------------------------------------------------------------
	// Override OnMouseEnter
	// ------------------------------------------------------------	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if ( m_Button && w == m_Button )
		{
			m_Icon.SetColor( m_Color );
			m_TooltipUI.ShowTooltip();
			return true;
		}

		return false;
	}

	// ------------------------------------------------------------
	// Override OnMouseLeave
	// ------------------------------------------------------------	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if ( m_Button && w == m_Button )
		{
			m_Icon.SetColor( ARGB( 255, 0, 0, 0 ) );
			m_TooltipUI.HideTooltip();
			return true;
		}

		return false;
	}
	
	// ------------------------------------------------------------
	// GetTooltip
	// ------------------------------------------------------------	
	ref ExpansionUITooltip GetTooltip()
	{
		return m_TooltipUI;
	}
}