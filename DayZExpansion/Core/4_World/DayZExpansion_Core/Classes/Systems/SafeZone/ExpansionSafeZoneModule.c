/**
 * ExpansionSafeZoneModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
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

	ExpansionSafeZoneSettings GetSettings()
	{
		if ( !m_Settings )
			m_Settings = GetExpansionSettings().GetSafeZone();

		return m_Settings;
	}

	override bool IsEnabled()
	{
		return (super.IsEnabled() && GetSettings().Enabled);
	}

	//! Note: Only called if safezone module is enabled (IsEnabled needs to return true)
	override void OnMissionLoaded()
	{
		super.OnMissionLoaded();

		if ( IsMissionHost() )
			thread ThreadSafeZone();
	}

	override bool IsClient()
	{
		return false;
	}

	static bool IsInside( vector position )
	{
		ExpansionSafeZoneModule module = ExpansionSafeZoneModule.Cast( GetModuleManager().GetModule( ExpansionSafeZoneModule ) );

		return module && module.IsEnabled() && module.IsInsideSafeZone( position );
	}

	private bool IsInsideSafeZone( vector position )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSafeZoneModule:: IsInsideSafeZone - Start");
		#endif
		
		array<ref ExpansionSafeZoneCircle> circleZones = GetSettings().CircleZones;
		array<ref ExpansionSafeZonePolygon> polygonZones = GetSettings().PolygonZones;

		if ( !circleZones || !polygonZones )
			return false;

		//! Check all circle zones first, because it's lighter on perf than polygon check
		foreach ( ExpansionSafeZoneCircle circleZone : circleZones )
		{
			vector center = Vector( circleZone.Center[0], position[1], circleZone.Center[2] );

			if ( vector.Distance( position, center ) < circleZone.Radius )
			{
				return true;
			}
		}

		//! If the position is not inside a circle zone we check polygon zones,
		//! heavier on performance depending on polygon complexity
		bool isInsidePolygonZone;
		foreach ( ExpansionSafeZonePolygon polyZone : polygonZones )
		{
			TVectorArray polygonPosition = polyZone.Positions;

			for ( int k = 0, l = polygonPosition.Count() - 1; k < polygonPosition.Count(); ++k)
			{
				if ( ( polygonPosition[k][2] > position[2] ) != ( polygonPosition[l][2] > position[2] ) && position[0] < ( polygonPosition[l][0] - polygonPosition[k][0] ) * ( position[2] - polygonPosition[k][2] ) / ( polygonPosition[l][2] - polygonPosition[k][2] ) + polygonPosition[k][0] )
				{
					isInsidePolygonZone = !isInsidePolygonZone;
				}

				l = k;
			}

			if ( isInsidePolygonZone )
			{
				return true;
			}
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionSafeZoneModule:: IsInsideSafeZone - End");
		#endif

		return false;
	}

	private void CheckPlayers()
	{
		array< PlayerBase > players = PlayerBase.GetAll();

		foreach ( PlayerBase player : players )
		{
			if ( !player )
				continue;

			bool isInside = IsInsideSafeZone( player.GetPosition() );

			if ( isInside && !player.IsInSafeZone() )
				player.OnEnterSafeZone();
			else if ( !isInside && player.IsInSafeZone() )
				player.OnLeavingSafeZone();
		}
	}

	private void CheckZombies()
	{
		set< ZombieBase > zombies = ZombieBase.GetAll();

		foreach ( ZombieBase zombie : zombies )
		{
			if ( !zombie )
				continue;

			bool isInside = IsInsideSafeZone( zombie.GetPosition() );

			if ( isInside && !zombie.IsInSafeZone() )
				zombie.OnEnterSafeZone();
			else if ( !isInside && zombie.IsInSafeZone() )
				zombie.OnLeftSafeZone();
		}
	}

	private void CheckVehicles()
	{
		bool isInside;

		set< CarScript > cars = CarScript.GetAll();

		foreach ( CarScript car : cars )
		{
			if ( !car )
				continue;

			isInside = IsInsideSafeZone( car.GetPosition() );

			if ( isInside && !car.IsInSafeZone() )
				car.OnEnterSafeZone();
			else if ( !isInside && car.IsInSafeZone() )
				car.OnLeftSafeZone();
		}

		#ifdef EXPANSIONMODVEHICLE
		set< ExpansionVehicleBase > vehicles = ExpansionVehicleBase.GetAll();

		foreach ( ExpansionVehicleBase vehicle : vehicles )
		{
			if ( !vehicle )
				continue;

			isInside = IsInsideSafeZone( vehicle.GetPosition() );

			if ( isInside && !vehicle.IsInSafeZone() )
				vehicle.OnEnterSafeZone();
			else if ( !isInside && vehicle.IsInSafeZone() )
				vehicle.OnLeftSafeZone();
		}
		#endif
	}

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

			//! Check if we have the two types of zone setup
			if ( !GetSettings().CircleZones || !GetSettings().PolygonZones )
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

			//! Make sure the time the thread sleeps is never zero and force a minimum value
			Sleep( Math.Max( GetSettings().FrameRateCheckSafeZoneInMs, 25 ) );
		}
	}
}
