#ifdef JM_COT_VEHICLEMANAGER
modded class JMVehiclesMenu
{
	override private void GetVehicleTypeInfo( int type, out int color, out string marker )
	{
		if ( ( type & JMVT_CAR ) != 0 )
		{
			color = ARGB( 255, 243, 156, 18 );
			marker = "DayZExpansion\\Core\\GUI\\icons\\marker\\marker_car.paa";
		} else if ( ( type & JMVT_BOAT ) != 0 )
		{
			color = ARGB( 255, 41, 128, 185 );
			marker = "DayZExpansion\\Core\\GUI\\icons\\marker\\marker_boat.paa";
		} else if ( ( type & JMVT_HELICOPTER ) != 0 )
		{
			color = ARGB( 255, 142, 68, 173 );
			marker = "DayZExpansion\\Core\\GUI\\icons\\marker\\marker_helicopter.paa";
		} else if ( ( type & JMVT_PLANE ) != 0 )
		{
			color = ARGB( 255, 34, 166, 179 );
			marker = "DayZExpansion\\Core\\GUI\\icons\\marker\\marker_helicopter.paa"; //! TODO: Change to a plane marker
		}
	}
}
#endif
