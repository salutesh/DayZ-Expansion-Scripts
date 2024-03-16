/**
 * ExpansionWoodFloor.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionFloorBase: ExpansionBaseBuilding
{
	private bool m_Expansion_HasFloor;
	private bool m_Expansion_IsRoof;

	void ExpansionFloorBase()
	{
		RegisterNetSyncVariableBool( "m_Expansion_HasFloor" );
		RegisterNetSyncVariableBool( "m_Expansion_IsRoof" );

		m_Expansion_CurrentBuild = "wood";
	}

	override bool IsLastStage()
	{
		return m_Expansion_HasFloor;
	}

	override bool IsLastStageBuilt()
	{
		return IsPartBuilt( m_Expansion_CurrentBuild + "_floorfinished" ) || IsPartBuilt( m_Expansion_CurrentBuild + "_hatchfinished" ) );
	}

	bool IsRoof()
	{
		return m_Expansion_IsRoof;
	}

	void SetRoof(bool state)
	{
		m_Expansion_IsRoof = state;

		SetSynchDirty();
	}

	override string GetConstructionKitType()
	{
		if (IsRoof())
			return "ExpansionRoofKit";
		return "ExpansionFloorKit";
	}

	override bool NameOverride(out string output)
	{
		if (IsRoof())
			output = "#STR_EXPANSION_BB_" + m_Expansion_CurrentBuild + "_ROOF_FINISHED";
		else if (IsLastStage())
			output = "#STR_EXPANSION_BB_" + m_Expansion_CurrentBuild + "_FLOOR_FINISHED";
		else
			output = "#STR_EXPANSION_BB_" + m_Expansion_CurrentBuild + "_FLOOR_BASE";
		return true;
	}

	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();

		UpdateVisuals();
	}
	
	override void AfterStoreLoad()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.CE, this, "AfterStoreLoad");
#endif
	
		super.AfterStoreLoad();

		if ( m_ExpansionSaveVersion < 18 )
			m_Expansion_HasFloor = IsLastStageBuilt();
		
		UpdateVisuals();	
	}

	#ifdef EXPANSION_MODSTORAGE
	override void CF_OnStoreSave(CF_ModStorageMap storage)
	{
		super.CF_OnStoreSave(storage);

		auto ctx = storage[DZ_Expansion_BaseBuilding];
		if (!ctx) return;

		ctx.Write(m_Expansion_HasFloor);
		ctx.Write(m_Expansion_IsRoof);
	}
	
	override bool CF_OnStoreLoad(CF_ModStorageMap storage)
	{
		if (!super.CF_OnStoreLoad(storage))
			return false;

		auto ctx = storage[DZ_Expansion_BaseBuilding];
		if (!ctx) return true;

		if (!ctx.Read(m_Expansion_HasFloor))
			return false;

		if (!ctx.Read(m_Expansion_IsRoof))
			return false;

		return true;
	}
	#endif

	override bool ExpansionGetCollisionBox( out vector minMax[2] )
	{
		minMax[0] = "-3 0 -3";
		minMax[1] = "3 0.25 3";
		return true;
	}
	
	override void OnPartBuiltServer( notnull Man player, string part_name, int action_id )
	{
		m_Expansion_HasFloor = false;

		ExpansionUpdateBaseBuildingStateFromPartBuilt( part_name );

		super.OnPartBuiltServer(player, part_name, action_id );
		UpdateVisuals();
	}

	override void ExpansionUpdateBaseBuildingStateFromPartBuilt( string part_name )
	{
		if ( part_name == m_Expansion_CurrentBuild + "_floorfinished" || part_name == m_Expansion_CurrentBuild + "_hatchfinished" )
		{
			m_Expansion_HasFloor = true;
		}
	}

	override void OnPartDismantledServer( notnull Man player, string part_name, int action_id )
	{
		m_Expansion_HasFloor = false;

		super.OnPartDismantledServer( player, part_name, action_id );
		UpdateVisuals();
	}

	override void OnPartDestroyedServer( Man player, string part_name, int action_id, bool destroyed_by_connected_part = false )
	{
		m_Expansion_HasFloor = false;

		super.OnPartDestroyedServer( player, part_name, action_id, destroyed_by_connected_part );
		UpdateVisuals();
	}

	override bool CanPutInCargo (EntityAI parent)
	{
		return false;
	}

	override bool IsPlayerInside( PlayerBase player, string selection )
	{
		if ( !IsLastStage() )
			return true;

		vector player_pos = player.GetPosition();
		vector floor_pos = GetPosition();

		if (IsRoof())
		{
			//! Don't allow dismantling roof from above
			if (player_pos[1] > floor_pos[1])
				return false;
		}
		else
		{
			//! Don't allow dismantling floor from below
			if (player_pos[1] < floor_pos[1])
				return false;
		}

		vector minMax[2];
		ExpansionGetCollisionBox(minMax);

		vector test_position = WorldToModel(player_pos);

		return Math.IsPointInRectangle(minMax[0], minMax[1], test_position);
	}

	override bool IsFacingPlayer( PlayerBase player, string selection )
	{
		return false;
	}

	override bool IsFacingCamera( string selection )
	{
		return false;
	}
} 