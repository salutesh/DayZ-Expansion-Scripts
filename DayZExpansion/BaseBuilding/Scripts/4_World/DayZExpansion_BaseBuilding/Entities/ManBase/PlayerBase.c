/**
 * ExpansionPlayerBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class PlayerBase
{
	protected int m_TerritoryIdInside;

	protected ExpansionTerritoryModule m_TerritoryModule;

	// ------------------------------------------------------------
	// PlayerBase Constructor
	// ------------------------------------------------------------
	void PlayerBase()
	{
		m_TerritoryIdInside = -1;

		CF_Modules<ExpansionTerritoryModule>.Get(m_TerritoryModule);
	}
	
	// ------------------------------------------------------------
	// Expansion SetActions
	// ------------------------------------------------------------
	override void SetActions( out TInputActionMap InputActionMap )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.PLAYER, this, "SetActions");
#endif

		super.SetActions( InputActionMap );
				
		AddAction( ExpansionActionSelectNextPlacement, InputActionMap );
		AddAction( ExpansionActionCrackSafe, InputActionMap );
		AddAction( ExpansionActionDestroyBarbedWire, InputActionMap );
		AddAction( ExpansionActionDestroyLock, InputActionMap );
	}
	
	// ------------------------------------------------------------
	// PlayerBase GetBuildNoBuildZone
	// ------------------------------------------------------------
	ExpansionBuildNoBuildZone GetBuildNoBuildZone()
	{
		foreach ( ExpansionBuildNoBuildZone zone : GetExpansionSettings().GetBaseBuilding().Zones )
		{
			vector pos = GetPosition();
			vector center = zone.Center;

			if ( center[1] == 0 )
				pos[1] == 0;
	
			if ( vector.Distance( pos, zone.Center ) <= zone.Radius )
				return zone;
		}

		return NULL;
	}
	
	// ------------------------------------------------------------
	// PlayerBase TerritoryModuleExists
	// ------------------------------------------------------------
	private bool TerritoryModuleExists()
	{
		if ( !m_TerritoryModule )
		{
			#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
			EXPrint("PlayerBase::TerritoryModuleExists - [ERROR] Territory module is NULL!");
			#endif
			return false;
		}

		return true;
	}
	
	// ------------------------------------------------------------
	// PlayerBase IsInsideOwnPerimeter
	// Check if player is in own territory's perimeter (but not in territory itself)
	// ------------------------------------------------------------
	bool IsInsideOwnPerimeter( float territorySize = -1, float perimeterSize = -1 )
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXPrint("PlayerBase::IsInsideOwnPerimeter - Start");
		#endif

		return !IsInsideOwnTerritory( territorySize ) && IsInsideOwnTerritoryOrPerimeter( territorySize, perimeterSize );
	}
		
	// ------------------------------------------------------------
	// PlayerBase IsInPerimeter
	// Check if player is in a territory's perimeter (but not in territory itself)
	// ------------------------------------------------------------
	bool IsInPerimeter( float territorySize = -1, float perimeterSize = -1 )
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXPrint("PlayerBase::IsInPerimeter - Start");
		#endif

		return !IsInTerritory( territorySize ) && IsInTerritoryOrPerimeter( territorySize, perimeterSize );
	}
		
	// ------------------------------------------------------------
	// PlayerBase IsInTerritory
	// Check if player is in a territory
	// ------------------------------------------------------------
	bool IsInTerritory(float territorySize = -1)
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXPrint("PlayerBase::IsInTerritory - Start");
		#endif

		if ( !TerritoryModuleExists() )
			return false;
			
		return m_TerritoryModule.IsInTerritory( GetPosition(), territorySize );
	}

	// ------------------------------------------------------------
	// PlayerBase IsInTerritoryOrPerimeter
	// Check if player is in a territory or in its perimeter
	// ------------------------------------------------------------
	bool IsInTerritoryOrPerimeter( float territorySize = -1, float perimeterSize = -1 )
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXPrint("PlayerBase::IsInTerritoryOrPerimeter - Start");
		#endif

		if ( !TerritoryModuleExists() )
			return false;
			
		return m_TerritoryModule.IsInTerritoryOrPerimeter( GetPosition(), territorySize, perimeterSize );
	}
	
	// ------------------------------------------------------------
	// PlayerBase IsInsideOwnTerritory
	// Check if player is in own territory
	// ------------------------------------------------------------
	bool IsInsideOwnTerritory( float territorySize = -1 )
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXPrint("PlayerBase::IsInsideOwnTerritory - Start");
		#endif

		if ( !TerritoryModuleExists() )
			return false;

		string playerUID;
		if ( IsMissionHost() )
			playerUID = m_PlayerUID;

		return m_TerritoryModule.IsInsideOwnTerritory( GetPosition(), territorySize, playerUID );
	}

	// ------------------------------------------------------------
	// PlayerBase IsInsideOwnTerritoryOrPerimeter
	// Check if player is in own territory or in its perimeter
	// ------------------------------------------------------------
	bool IsInsideOwnTerritoryOrPerimeter( float territorySize = -1, float perimeterSize = -1 )
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXPrint("PlayerBase::IsInsideOwnTerritoryOrPerimeter - Start");
		#endif

		if ( !TerritoryModuleExists() )
			return false;

		string playerUID;
		if ( IsMissionHost() )
			playerUID = m_PlayerUID;
			
		return m_TerritoryModule.IsInsideOwnTerritoryOrPerimeter( GetPosition(), territorySize, perimeterSize, playerUID );
	}
	
	// ------------------------------------------------------------
	// PlayerBase SetTerritoryIDInside, only client side
	// ------------------------------------------------------------
	void SetTerritoryIDInside(int territoryID)
	{
		m_TerritoryIdInside = territoryID;
	}
	
	// ------------------------------------------------------------
	// PlayerBase GetTerritoryIDInside, only client side
	// ------------------------------------------------------------
	int GetTerritoryIDInside()
	{
		return m_TerritoryIdInside;
	}
	
	// ------------------------------------------------------------
	// PlayerBase FindNearestFlag
	// Returns nearest TerritoryFlag object at given position
	// or NULL if no TerritoryFlag object is found.
	// ------------------------------------------------------------
	TerritoryFlag FindNearestFlag()
	{
		if ( !IsMissionClient() )
			return NULL;
		
		vector pos = GetPosition();
		array<Object> objects = new array<Object>;
		array<CargoBase> proxyCargos = new array<CargoBase> ;
		float radius = 10;
		GetGame().GetObjectsAtPosition3D( pos, radius, objects, proxyCargos );
		float distance;
		TerritoryFlag nearestFlag;
		
		if ( objects && objects.Count() > 0 )
		{
			for ( int i = 0; i < objects.Count(); ++i )
			{
				TerritoryFlag flag;
				if ( Class.CastTo( flag, objects.Get( i ) ) )
				{
					distance = vector.Distance( pos, flag.GetPosition() );
					if ( distance < radius )
					{
						radius = distance;
						nearestFlag = flag;
					}
				}
			}
		}

		return nearestFlag;
	}
		
	// ------------------------------------------------------------
	// Expansion IsTargetInActiveRefresherRange
	// ------------------------------------------------------------	
	override bool IsTargetInActiveRefresherRange(EntityAI target)
	{
		//! Duplicate of vanilla code except using TerritorySize instead of constant
		array<vector> temp = new array<vector>;
		temp = GetGame().GetMission().GetActiveRefresherLocations();
		int count = temp.Count();
		if (count > 0)
		{
			float territorySize = GetExpansionSettings().GetTerritory().TerritorySize;

			vector pos = target.GetPosition();
			for (int i = 0; i < count; i++)
			{
				if ( vector.Distance(pos,temp.Get(i)) < territorySize )
					return true;
			}
			
			return false;
		}
		else
		{
			return false;
		}
	}
};
