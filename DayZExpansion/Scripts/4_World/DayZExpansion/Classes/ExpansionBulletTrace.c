/**
 * ExpansionBulletTrace.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionBulletTrace
{
	Object m_Object;
	vector m_PreviousPosition;
	float m_PreviousTime;

	ref array< Shape > m_Shapes;

	void ExpansionBulletTrace( Object obj )
	{
		m_Object = obj;
		
		if ( m_Object )
			m_PreviousPosition = m_Object.GetPosition();

		m_Shapes = new array< Shape >();
	}

	void ~ExpansionBulletTrace()
	{
		for ( int i = 0; i < m_Shapes.Count(); ++i )
			m_Shapes[i].Destroy();

		delete m_Shapes;
	}

	/**
	 * 
	 * /return Return false to delete the class and debug shapes
	 */
	bool OnUpdate( float timeslice )
	{
		if ( m_Object == NULL )
			return true;
		
		float time = GetGame().GetTickTime();
		float delta = ( time - m_PreviousTime );
		m_PreviousTime = time;
		
		//Print( delta );
		//Print( timeslice );

		vector pts[2]
		pts[0] = m_PreviousPosition;
		pts[1] = m_Object.GetPosition();

		vector velocity = vector.Direction( pts[0], pts[1] );
		float speed = ( velocity.Length() / delta );
		
		float scale = 500;

		int r = 1.0 - Math.Clamp( ( speed - ( scale * 0.0 ) ) / ( scale * 2.0 ), 0.0, 1.0 );
		int g = 1.0 - Math.Clamp( ( speed - ( scale * 1.0 ) ) / ( scale * 2.0 ), 0.0, 1.0 );
		int b = 1.0 - Math.Clamp( ( speed - ( scale * 2.0 ) ) / ( scale * 2.0 ), 0.0, 1.0 );
		
		r *= r * 255;
		g *= g * 255;
		b *= b * 255;
		
		//Print( speed );
		//Print( r );
		//Print( g );
		//Print( b );

		m_PreviousPosition = m_Object.GetPosition();
		
		m_Shapes.Insert( Shape.CreateLines( ARGBtoInt( 200, r, g, b ), ShapeFlags.TRANSP, pts, 2 ) );

		return true;
	}
};