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

class ExpansionFloorBase extends ExpansionBaseBuilding
{
	private bool m_HasFloor;
	private bool m_IsRoof;

	void ExpansionFloorBase()
	{
		RegisterNetSyncVariableBool( "m_HasFloor" );
		RegisterNetSyncVariableBool( "m_IsRoof" );

		m_CurrentBuild = "wood";
	}

	override bool IsLastStage()
	{
		return m_HasFloor;
	}

	override bool IsLastStageBuilt()
	{
		return IsPartBuilt( m_CurrentBuild + "_floorfinished" ) || IsPartBuilt( m_CurrentBuild + "_hatchfinished" ) );
	}

	bool IsRoof()
	{
		return m_IsRoof;
	}

	void SetRoof(bool state)
	{
		m_IsRoof = state;

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
			output = "#STR_EXPANSION_BB_" + m_CurrentBuild + "_ROOF_FINISHED";
		else if (IsLastStage())
			output = "#STR_EXPANSION_BB_" + m_CurrentBuild + "_FLOOR_FINISHED";
		else
			output = "#STR_EXPANSION_BB_" + m_CurrentBuild + "_FLOOR_BASE";
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
			m_HasFloor = IsLastStageBuilt();
		
		UpdateVisuals();	
	}

	override void OnStoreSave( ParamsWriteContext ctx )
	{
		#ifdef EXPANSION_MODSTORAGE
		if ( GetGame().SaveVersion() >= EXPANSION_VERSION_GAME_MODSTORAGE_TARGET )
		{
			super.OnStoreSave( ctx );
			return;
		}
		#endif

		super.OnStoreSave( ctx );
		
		ctx.Write( m_HasFloor );
		ctx.Write( m_IsRoof );
	}

	override bool OnStoreLoad( ParamsReadContext ctx, int version )
	{
		if ( Expansion_Assert_False( super.OnStoreLoad( ctx, version ), "[" + this + "] Failed reading OnStoreLoad super" ) )
			return false;

		#ifdef EXPANSION_MODSTORAGE
		if ( version > EXPANSION_VERSION_GAME_MODSTORAGE_TARGET || m_ExpansionSaveVersion > EXPANSION_VERSION_SAVE_MODSTORAGE_TARGET )
			return true;
		#endif

		if ( m_ExpansionSaveVersion < 18 )
			return true;

		if ( Expansion_Assert_False( ctx.Read( m_HasFloor ), "[" + this + "] Failed reading m_HasFloor" ) )
			return false;

		if ( m_ExpansionSaveVersion < 28 )
			return true;

		if ( Expansion_Assert_False( ctx.Read( m_IsRoof ), "[" + this + "] Failed reading m_IsRoof" ) )
			return false;
		
		return true;
	}

	#ifdef EXPANSION_MODSTORAGE
	override void CF_OnStoreSave(CF_ModStorageMap storage)
	{
		super.CF_OnStoreSave(storage);

		auto ctx = storage[DZ_Expansion_BaseBuilding];
		if (!ctx) return;

		ctx.Write(m_HasFloor);
		ctx.Write(m_IsRoof);
	}
	
	override bool CF_OnStoreLoad(CF_ModStorageMap storage)
	{
		if (!super.CF_OnStoreLoad(storage))
			return false;

		auto ctx = storage[DZ_Expansion_BaseBuilding];
		if (!ctx) return true;

		if (!ctx.Read(m_HasFloor))
			return false;

		if (!ctx.Read(m_IsRoof))
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
		m_HasFloor = false;

		ExpansionUpdateBaseBuildingStateFromPartBuilt( part_name );

		super.OnPartBuiltServer(player, part_name, action_id );
		UpdateVisuals();
	}

	override void ExpansionUpdateBaseBuildingStateFromPartBuilt( string part_name )
	{
		if ( part_name == m_CurrentBuild + "_floorfinished" || part_name == m_CurrentBuild + "_hatchfinished" )
		{
			m_HasFloor = true;
		}
	}

	override void OnPartDismantledServer( notnull Man player, string part_name, int action_id )
	{
		m_HasFloor = false;

		super.OnPartDismantledServer( player, part_name, action_id );
		UpdateVisuals();
	}

	override void OnPartDestroyedServer( Man player, string part_name, int action_id, bool destroyed_by_connected_part = false )
	{
		m_HasFloor = false;

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