/**
 * StringLocaliser.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifndef CF_EXPRESSION

modded class StringLocaliser
{
	private bool m_translatesparams;

	void StringLocaliser( string text, string param1 = "", string param2 = "", string param3 = "", string param4 = "", string param5 = "", string param6 = "", string param7 = "", string param8 = "", string param9 = "" )
	{
		if ( param1.IndexOf( "STR_" ) == 0 )
			m_translatesparams = true;
		else if ( param2.IndexOf( "STR_" ) == 0 )
			m_translatesparams = true;
		else if ( param3.IndexOf( "STR_" ) == 0 )
			m_translatesparams = true;
		else if ( param4.IndexOf( "STR_" ) == 0 )
			m_translatesparams = true;
		else if ( param5.IndexOf( "STR_" ) == 0 )
			m_translatesparams = true;
		else if ( param6.IndexOf( "STR_" ) == 0 )
			m_translatesparams = true;
		else if ( param7.IndexOf( "STR_" ) == 0 )
			m_translatesparams = true;
		else if ( param8.IndexOf( "STR_" ) == 0 )
			m_translatesparams = true;
		else if ( param9.IndexOf( "STR_" ) == 0 )
			m_translatesparams = true;
	}

	void SetTranslatesParams( bool translatesparams )
	{
		m_translatesparams = translatesparams;
	}

	override string Format()
	{
		string text = m_text;

		string p1 = m_param1;
		string p2 = m_param2;
		string p3 = m_param3;
		string p4 = m_param4;
		string p5 = m_param5;
		string p6 = m_param6;
		string p7 = m_param7;
		string p8 = m_param8;
		string p9 = m_param9;

		if ( m_translates )
		{
			text = Widget.TranslateString( "#" + m_text );
			if ( text == "" || text.Get( 0 ) == " " )
			{
				text = m_text;
			}
		}

		if ( m_translatesparams )
		{
			p1 = Widget.TranslateString( "#" + m_param1 );
			if ( p1 == "" || p1.Get( 0 ) == " " )
			{
				p1 = m_param1;
			}

			p2 = Widget.TranslateString( "#" + m_param2 );
			if ( p2 == "" || p1.Get( 0 ) == " " )
			{
				p2 = m_param2;
			}

			p3 = Widget.TranslateString( "#" + m_param3 );
			if ( p3 == "" || p3.Get( 0 ) == " " )
			{
				p3 = m_param3;
			}

			p4 = Widget.TranslateString( "#" + m_param4 );
			if ( p4 == "" || p4.Get( 0 ) == " " )
			{
				p4 = m_param4;
			}

			p5 = Widget.TranslateString( "#" + m_param5 );
			if ( p5 == "" || p5.Get( 0 ) == " " )
			{
				p5 = m_param5;
			}

			p6 = Widget.TranslateString( "#" + m_param6 );
			if ( p6 == "" || p6.Get( 0 ) == " " )
			{
				p6 = m_param6;
			}

			p7 = Widget.TranslateString( "#" + m_param7 );
			if ( p7 == "" || p7.Get( 0 ) == " " )
			{
				p7 = m_param7;
			}

			p8 = Widget.TranslateString( "#" + m_param8 );
			if ( p8 == "" || p8.Get( 0 ) == " " )
			{
				p8 = m_param8;
			}

			p9 = Widget.TranslateString( "#" + m_param9 );
			if ( p9 == "" || p9.Get( 0 ) == " " )
			{
				p9 = m_param9;
			}
		}

		return string.Format( text, p1, p2, p3, p4, p5, p6, p7, p8, p9 );
	}
}

#endif