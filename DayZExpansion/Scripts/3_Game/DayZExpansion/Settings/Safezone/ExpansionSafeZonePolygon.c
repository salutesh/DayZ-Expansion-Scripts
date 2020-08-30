/**
 * ExpansionSafeZonePolygon.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionSafeZonePolygon
 * @brief
 **/
class ExpansionSafeZonePolygon: ExpansionSafeZoneElement
{
	autoptr TVectorArray Positions;

	vector CenterPolygon; 	//! TODO:  need to not be public for 1.09
	float RadiusPolygon; 	//! TODO: need to not be public for 1.09

	// ------------------------------------------------------------
	// Expansion ExpansionSafeZonePolygon
	// ------------------------------------------------------------
	void ExpansionSafeZonePolygon( ref TVectorArray positions )
	{
		Type = ExpansionSafeZoneTypes.POLYGON;
		Positions = positions;

		if ( !IsPolygon() || IsPolygonComplex() )
		{
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint("[ExpansionSafeZonePolygon] Polygon is not valid, this safezone has not been initialized");
			#endif
			
			GetExpansionSettings().GetSafeZone().RemoveSafeZonePolygon( this );
			return;

			UpdateCenter();
			UpdateRadius();
		}
	}

	private void UpdateCenter()
	{
		float accumulatedArea = 0.0;
		float centerX = 0.0;
		float centerZ = 0.0;

		for ( int i = 0, j = Positions.Count() - 1; i < Positions.Count(); ++i )
		{
			float temp = Positions[i][0] * Positions[j][2] - Positions[j][0] * Positions[i][2];
			accumulatedArea += temp;
			centerX += (Positions[i][0] + Positions[j][0]) * temp;
			centerZ += (Positions[i][2] + Positions[j][2]) * temp;

			j = i;
		}

		if ( accumulatedArea == 0 )
		{
			CenterPolygon = vector.Zero;
			return;
		}
			
		accumulatedArea *= 3;
		CenterPolygon = Vector(centerX / accumulatedArea, 0, centerZ / accumulatedArea);
	}

	private void UpdateRadius()
	{
		if ( !CenterPolygon )
		{
			RadiusPolygon = 0;
			return;
		}

		float max_distance = 0.000001;

		for( int i = 0; i < Positions.Count(); ++i )
		{
			float distance = vector.Distance( CenterPolygon, Positions[i] );
			if ( distance > max_distance )
			{
				max_distance = distance;
			}
		}

		RadiusPolygon = max_distance;
	}

	private bool IsPolygonComplex()
	{
		//Missing code here to detect complex polygon
		return false;
	}

	private bool IsPolygon()
	{
		if ( Positions.Count() > 2 )
		{
			return true;
		}

		return false;
	}
}