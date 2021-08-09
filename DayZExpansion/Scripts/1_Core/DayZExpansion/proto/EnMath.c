/**
 * EnMath.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class Math
{
	/**
	 * @brief Returns the direction the number is facing, 0 if zero
	 * 
	 * @return float
	 */
	static float Sign( float number )
	{
		if ( number > 0 )
			return 1;
		if ( number < 0 )
			return -1;
		return 1;
	}

	/**
	 * @brief Returns the direction the number is facing
	 * 
	 * @return float
	 */
	static float SignNoNeg( float number )
	{
		if ( number < 0 )
			return -1;
		return 1;
	}

	/**
	 * @brief Square the number but keeping the sign
	 * 
	 * @return float
	 */
	static float SquareSign( float number )
	{
		return number * number * Sign( number );
	}

	/**
	 * @brief (n * (n + 1)) * Sign(n)
	 * 
	 * @return float
	 */
	static float SquareSignPercent( float number )
	{
		return number * ( 1 + number ) * Sign( number );
	}

	static float Interpolate( float value, float cMin, float cMax, float vMin, float vMax )
	{
		if ( value <= cMin )
			return vMin;
		if ( value >= cMax )
			return vMax;
			
		return ( value - cMin ) / ( cMax - cMin ) * ( vMax - vMin ) + vMin;
	}

	static bool AngleDir( float a, float b )
	{
		float fR1 = a - b;
		float fR2 = b - a;

		if ( fR1 < 0 )
			fR1 += 360;
		if ( fR2 < 0 )
			fR2 += 360;

		return fR2 < fR1;
	}

	static float AngleDiff( float a, float b )
	{
		float angle = 180 - Math.AbsFloat( Math.AbsFloat( a - b ) - 180 );
		if ( a + angle != b )
			return -angle;
		return angle;
	}
};