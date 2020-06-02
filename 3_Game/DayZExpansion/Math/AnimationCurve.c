/**
 * AnimationCurve.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class AnimationCurve
{
	private autoptr array< vector > m_Data;

	private float m_MinX = 100000;
	private float m_MaxX = -100000;

	private float m_MinY = 100000;
	private float m_MaxY = -100000;

	void AnimationCurve()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("AnimationCurve::AnimationCurve - Start");
		#endif
		
		m_Data = new array< vector >;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("AnimationCurve::AnimationCurve - End");
		#endif
	}

	void Finish()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("AnimationCurve::Finish - Start");
		#endif
		
		for ( int i = 0; i < m_Data.Count(); i++ )
		{
			if ( m_MinX > m_Data[i][0] )
				m_MinX = m_Data[i][0];

			if ( m_MaxX < m_Data[i][0] )
				m_MaxX = m_Data[i][0];

			if ( m_MinY > m_Data[i][1] )
				m_MinY = m_Data[i][1];

			if ( m_MaxY < m_Data[i][1] )
				m_MaxY = m_Data[i][1];
		}

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( m_MinX );
		#endif

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( m_MaxX );
		#endif

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( m_MinY );
		#endif
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( m_MaxY );
		#endif
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("AnimationCurve::Finish - End");
		#endif
	}

	void Add( float x, float y )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("AnimationCurve::Add - Start");
		#endif
		
		if ( m_Data.Count() > 0 && x <= m_Data[ m_Data.Count() - 1 ][0] )
		{
			Error( "[EXPANSION] AnimationCurve: X must be Greater than Previous" );
			return;
		}

		m_Data.Insert( Vector( x, y, 0 ) );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint( "AnimationCurve::Add - End");
		#endif
	}

	float EvaluateLinear( float x )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("AnimationCurve::EvaluateLinear - Start");
		#endif
		
		if ( x < m_MinX )
		{
			#ifdef EXPANSIONEXPRINT
		EXPrint("AnimationCurve::EvaluateLinear - Return: " + (m_Data[0][1]).ToString() );
		#endif
			return m_Data[0][1];
		}
		
		if ( x > m_MaxX )
		{
			#ifdef EXPANSIONEXPRINT
		EXPrint("AnimationCurve::EvaluateLinear - Return: " + (m_Data[m_Data.Count() - 1][1]).ToString() );
		#endif
			return m_Data[m_Data.Count() - 1][1];
		}

		for ( int i = 0; i < m_Data.Count() - 1; i++ )
		{
			if ( x < m_Data[i][0] )
				continue;

			float x1 = m_Data[i][0];
			float x2 = m_Data[i + 1][0];
			float y1 = m_Data[i][1];
			float y2 = m_Data[i + 1][1];
			float interp = ( x - x1 ) / ( x2 - x1 );

			#ifdef EXPANSIONEXPRINT
		EXPrint("AnimationCurve::EvaluateLinear - Return: " + ((y2 - y1) * interp + y1).ToString() );
		#endif

			return (y2 - y1) * interp + y1;
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("AnimationCurve::EvaluateLinear - Return: 0");
		#endif

		return 0;
	}
}