/**
 * ExpansionBookTabFake.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionBookTabFake extends ExpansionBookTabBase
{
	private ref TIntArray m_color = { ARGB( 255, 252, 23, 23 ), ARGB( 255, 36, 254, 0 ), ARGB( 255, 36, 103, 255 ), ARGB( 255, 255, 87, 34 ), ARGB( 255, 199, 38, 81 ) };
	
	void ExpansionBookTabFake( int number )
	{
		SetButtonIconPath( "set:expansion_iconset image:icon_status" );
		SetButtonText( "FAKE " + number.ToString() );
		SetButtonColor( m_color.GetRandomElement() );
	}
}