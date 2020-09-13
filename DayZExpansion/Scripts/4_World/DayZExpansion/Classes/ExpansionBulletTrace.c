/**
 * ExpansionBulletTrace.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionBulletTrace
{
	Object m_Object;
	vector m_PreviousPosition;

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

		vector pts[2]
		pts[0] = m_PreviousPosition;
		pts[1] = m_Object.GetPosition();
		m_PreviousPosition = m_Object.GetPosition();
		
		m_Shapes.Insert( Shape.CreateLines( 0xEEFF3333, ShapeFlags.TRANSP, pts, 2 ) );

		return true;
	}
};