/**
 * CreditsMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class CreditsMenu
{
	protected static float						EXPANSION_CREDIT_SCROLL_SPEED	= 100.0;	//pixels per second (relative to 1080p res)
	
	override Widget Init()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionCreditsMenu::Init - Start");
		#endif
		float x_f;
		int x, y;
		
		layoutRoot	= GetGame().GetWorkspace().CreateWidgets( "DayZExpansion/GUI/layouts/credits/expansion_credits_menu.layout", null );
		m_Logo		= ImageWidget.Cast( layoutRoot.FindAnyWidget( "Logo" ) );
		m_Scroller	= ScrollWidget.Cast( layoutRoot.FindAnyWidget( "CreditsPanel" ) );
		m_Content	= WrapSpacerWidget.Cast( layoutRoot.FindAnyWidget( "CreditsContent" ) );
		
		GetScreenSize( x, y );
		
		m_MenuFadeInIncrement	= 1 / MENU_FADEIN_TIME;
		m_LogoFadeInIncrement	= 1 / LOGO_FADEIN_TIME;
		m_ScrollIncrement		= EXPANSION_CREDIT_SCROLL_SPEED * ( 1080 / y );
		
		m_Scroller.VScrollToPos01( 0 );
		m_Scroller.GetScreenSize( x_f, m_ScrollSize );
		
		GetGame().GameScript.Call( this, "LoadDataAsync", null );
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionCreditsMenu::Init - End");
		#endif
		return layoutRoot;
	}
}