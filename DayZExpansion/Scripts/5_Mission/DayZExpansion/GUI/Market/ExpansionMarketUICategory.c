/**
 * ExpansionMarketUICategory.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketUICategory: ScriptedWidgetEventHandler
{	
	//! Widgets
	protected Widget							m_LayoutRoot;
	protected Widget							m_CategoryPanel;
	protected ButtonWidget						m_CategroyButton;
	protected GridSpacerWidget					m_CategroyGrid;
	protected TextWidget						m_CategroyTitle;

	// ------------------------------------------------------------
	// ExpansionMarketUICategory Constructor
	// ------------------------------------------------------------
	void ExpansionMarketUICategory()
	{

	}
	
	// ------------------------------------------------------------
	// ExpansionMarketUICategory Destructor
	// ------------------------------------------------------------
	void ~ExpansionMarketUICategory()
	{

	}
	
	// ------------------------------------------------------------
	// Expansion OnWidgetScriptInit
	// ------------------------------------------------------------
	void OnWidgetScriptInit( Widget w )
	{
		m_LayoutRoot = w;
		m_LayoutRoot.SetHandler( this );

		m_CategoryPanel = Widget.Cast( m_LayoutRoot.FindAnyWidget("category_header_panel") );
		m_CategroyButton = ButtonWidget.Cast( m_LayoutRoot.FindAnyWidget( "category_collapse" ) );
		m_CategroyGrid = GridSpacerWidget.Cast( m_LayoutRoot.FindAnyWidget( "category_items" ) );
		m_CategroyTitle = TextWidget.Cast( m_LayoutRoot.FindAnyWidget( "category_title" ) );

		m_CategroyGrid.Show(false);
	}

	void ToogleItemGrid()
	{
		if ( m_CategroyGrid.IsVisible() )
			m_CategroyGrid.Show(false);
		else
			m_CategroyGrid.Show(true);
	}
	
	// ------------------------------------------------------------
	// Override bool OnClick
	// ------------------------------------------------------------
	override bool OnClick( Widget w, int x, int y, int button )
	{
		if (w == m_CategroyButton)
		{
			ToogleItemGrid();
		}

		return super.OnClick(w, x, y, button);
	}

	// ------------------------------------------------------------
	// Override OnMouseEnter
	// ------------------------------------------------------------
	override bool OnMouseEnter(Widget w, int x, int y)
	{	
		if( w== m_CategroyButton )
		{
			m_CategoryPanel.SetColor( ARGB(255,45,45,45) );
			m_CategroyTitle.SetColor( ARGB(255,189,195,199) );

			return true;
		}

	 	return super.OnMouseEnter(w, x, y);
	}
	
	// ------------------------------------------------------------
	// Override OnMouseLeave
	// ------------------------------------------------------------
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{	
		if( w== m_CategroyButton )
		{
			m_CategoryPanel.SetColor( ARGB(255,23,23,23) );
			m_CategroyTitle.SetColor( ARGB(255,255,255,255) );

			return true;
		}

	 	return super.OnMouseLeave(w, enterW, x, y);
	}
}