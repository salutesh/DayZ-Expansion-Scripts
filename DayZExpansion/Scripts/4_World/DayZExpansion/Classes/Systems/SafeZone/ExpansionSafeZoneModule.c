/**
 * ExpansionSafeZoneModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionSafeZoneModule
 * @brief		
 **/
class ExpansionSafeZoneModule: JMModuleBase
{
	private autoptr ExpansionSafeZoneSettings m_Settings;

	override void OnMissionLoaded()
	{
		super.OnMissionLoaded();

		m_Settings = GetExpansionSettings().GetSafeZone();

		GetGame().GameScript.Call( this, "ThreadSafeZone", NULL );
	}

	override bool IsClient()
	{
		return false;
	}

	bool IsInside( vector position )
	{
		if ( m_Settings.Enabled )
		{
			bool isInsideCircleZone;
			bool isInsidePolygonZone;
			int j;
			int p;

			IsInsideSafeZone( position, isInsideCircleZone, isInsidePolygonZone, j, p );

			return isInsideCircleZone || isInsidePolygonZone;
		}

		return false;
	}

	// ------------------------------------------------------------
	// Expansion IsInsideSafeZone
	// ------------------------------------------------------------
	private void IsInsideSafeZone( vector pos_obj, out bool isInsideCircleZone = false, out bool isInsidePolygonZone = false, out int j = 0, out int p = 0)
	{	
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSafeZoneModule:: IsInsideSafeZone - Start");
		#endif
		
		array<ref ExpansionSafeZoneCircle> circleZones = m_Settings.CircleZones;
		array<ref ExpansionSafeZonePolygon> polygonZones = m_Settings.PolygonZones;

		if ( !circleZones || !polygonZones )
			return;

		//Check all circle zone first, because it's not heavy on perf than polygon check
		for ( j = 0; j < circleZones.Count(); j++)
		{
			ExpansionSafeZoneCircle circleZone = circleZones[j];

			if ( !circleZone )
				continue;
									
			float xPower = circleZone.Center[0] - pos_obj[0];
			float zPower = circleZone.Center[2] - pos_obj[2];

			float distance_squared = ( xPower * xPower ) + ( zPower * zPower ); 
				
			if ( distance_squared < (circleZone.Radius * circleZone.Radius) ) 
			{
				isInsideCircleZone = true;
				return;
			}
		}

		// If he are not inside circle zone we check polygon zone, more heavy on performance
		if ( !isInsideCircleZone )
		{
			for ( p = 0; p < polygonZones.Count(); p++ )
			{
				ExpansionSafeZonePolygon polyZone = polygonZones[p];

				if ( !polyZone )
					continue;

				ref TVectorArray polygonPosition = polyZone.Positions;
					
				for ( int k = 0, l = polygonPosition.Count() - 1; k < polygonPosition.Count(); ++k)
				{
					if ( ( polygonPosition[k][2] > pos_obj[2] ) != ( polygonPosition[l][2] > pos_obj[2] ) && pos_obj[0] < ( polygonPosition[l][0] - polygonPosition[k][0] ) * ( pos_obj[2] - polygonPosition[k][2] ) / ( polygonPosition[l][2] - polygonPosition[k][2] ) + polygonPosition[k][0] )
					{
						isInsidePolygonZone = !isInsidePolygonZone;
					}

					l = k;
				}

				if ( isInsidePolygonZone )
				{
					return;
				}
			}
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSafeZoneModule:: IsInsideSafeZone - End");
		#endif
	}

	private void CheckPlayers()
	{
		array< PlayerBase > players = PlayerBase.GetAll();
		
		for ( int i = 0; i < players.Count(); i++ )
		{
			PlayerBase player = players[i];
			if ( !player )
				continue;

			bool isInside = IsInside( player.GetPosition() );
				
			if ( isInside )
			{
				if ( !player.IsInSafeZone() )
				{
					player.OnEnterSafeZone();
				}
			} else 
			{
				if ( player.IsInSafeZone() )
				{
					player.OnLeftSafeZone();
				}
			}
		}
	}

	private void CheckZombies()
	{
		for ( int i = 0; i < ZombieBase.GetAll().Count(); i++ )
		{
			ZombieBase zombie = ZombieBase.GetAll().Get( i );
			if ( !zombie )
				continue;

			bool isInside = IsInside( zombie.GetPosition() );
				
			if ( isInside )
			{
				if ( !zombie.IsInSafeZone() )
				{
					zombie.OnEnterSafeZone();
				}
			} else 
			{
				if ( zombie.IsInSafeZone() )
				{
					zombie.OnLeftSafeZone();
				}
			}
		}
	}

	private void CheckVehicles()
	{
		for ( int i = 0; i < CarScript.GetAll().Count(); i++ )
		{
			CarScript vehicle = CarScript.GetAll().Get( i );
			if ( !vehicle )
				continue;

			bool isInside = IsInside( vehicle.GetPosition() );
				
			if ( isInside )
			{
				if ( !vehicle.IsInSafeZone() )
				{
					vehicle.OnEnterSafeZone();
				}
			} else 
			{
				if ( vehicle.IsInSafeZone() )
				{
					vehicle.OnLeftSafeZone();
				}
			}
		}
	}

	// ------------------------------------------------------------
	// Expansion ThreadSafeZone
	// ------------------------------------------------------------
	private void ThreadSafeZone()
	{
		while ( true )
		{
			#ifdef EXPANSIONEXPRINT
			EXPrint("ExpansionSafeZoneModule:: ThreadSafeZone - Start");
			#endif

			if ( !IsEnabled() )
			{
				#ifdef EXPANSIONEXPRINT
				EXPrint("ExpansionSafeZoneModule:: ThreadSafeZone - End (IsDisabled)");
				#endif
				return;
			}

			array<ref ExpansionSafeZoneCircle> circleZones = m_Settings.CircleZones;
			array<ref ExpansionSafeZonePolygon> polygonZones = m_Settings.PolygonZones;
			
			//Check if we have the two types of zone setup
			if ( !circleZones || !polygonZones )
			{
				Error( "[ExpansionSafeZoneModule] Verify the safezone settings and restart the server." );
				#ifdef EXPANSIONEXPRINT
				EXPrint("ExpansionSafeZoneModule:: ThreadSafeZone - End (Zones Don't Exist)");
				#endif
				return;
			}

			CheckPlayers();
			CheckZombies();
			CheckVehicles();

			#ifdef EXPANSIONEXPRINT
			EXPrint("ExpansionSafeZoneModule:: ThreadSafeZone - End");
			#endif

			Sleep( m_Settings.FrameRateCheckSafeZoneInMs );
		}
	}
}

static bool ExpansionSafeZone_IsInside( vector position )
{
	ExpansionSafeZoneModule module = ExpansionSafeZoneModule.Cast( GetModuleManager().GetModule( ExpansionSafeZoneModule ) );
	
	if ( module )
	{
		return module.IsInside( position );
	}

	return false;
}