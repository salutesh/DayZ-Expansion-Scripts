/**
 * ExpansionBookTabRecipes.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionBookTabRecipes extends ExpansionBookTabBase
{
	protected int COLOR_EXPANSION_RECIPES_BOOKMARK_ICON = ARGB( 255, 26, 188, 156 );
	
	// ------------------------------------------------------------
	// ExpansionBookTabRecipes Contructor
	// ------------------------------------------------------------
	void ExpansionBookTabRecipes()
	{
		SetButtonIconPath( "set:expansion_iconset image:icon_info" );
		SetButtonText( "" );
		SetButtonColor( COLOR_EXPANSION_RECIPES_BOOKMARK_ICON );
		SetLayoutPath( "DayZExpansion/GUI/layouts/book/expansion_book_page_recipes.layout" );
	}
	
	// ------------------------------------------------------------
	// Override CanBeShow
	// ------------------------------------------------------------
	override bool CanBeShow()
	{
		return true;
	}
	
	// ------------------------------------------------------------
	// Override OnCreatedLayout
	// ------------------------------------------------------------
	//Layout root var is m_RootLayout
	override void OnCreatedLayout()
	{
		
	}
}