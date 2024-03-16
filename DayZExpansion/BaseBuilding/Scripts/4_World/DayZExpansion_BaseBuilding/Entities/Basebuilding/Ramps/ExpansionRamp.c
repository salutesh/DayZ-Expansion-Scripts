/**
 * ExpansionRamp.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionRampBase: ExpansionBaseBuilding
{
	private bool m_Expansion_HasRamp;

	void ExpansionRampBase()
	{
		RegisterNetSyncVariableBool( "m_Expansion_HasRamp" );

		m_Expansion_CurrentBuild = "wood";
	}

	override bool IsLastStage()
	{
		return m_Expansion_HasRamp;
	}

	override bool IsLastStageBuilt()
	{
		return IsPartBuilt( m_Expansion_CurrentBuild + "_finished" );
	}

	override string GetConstructionKitType()
	{
		return "ExpansionRampKit";
	}

	override bool NameOverride(out string output)
	{
		if (IsLastStage())
			output = "#STR_EXPANSION_BB_" + m_Expansion_CurrentBuild + "_RAMP_FINISHED";
		else
			output = "#STR_EXPANSION_BB_" + m_Expansion_CurrentBuild + "_RAMP_BASE";
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
			m_Expansion_HasRamp = IsLastStageBuilt();
		
		UpdateVisuals();	
	}

	#ifdef EXPANSION_MODSTORAGE
	override void CF_OnStoreSave(CF_ModStorageMap storage)
	{
		super.CF_OnStoreSave(storage);

		auto ctx = storage[DZ_Expansion_BaseBuilding];
		if (!ctx) return;

		ctx.Write(m_Expansion_HasRamp);
	}
	
	override bool CF_OnStoreLoad(CF_ModStorageMap storage)
	{
		if (!super.CF_OnStoreLoad(storage))
			return false;

		auto ctx = storage[DZ_Expansion_BaseBuilding];
		if (!ctx) return true;

		if (!ctx.Read(m_Expansion_HasRamp))
			return false;

		return true;
	}
	#endif

	override bool ExpansionGetCollisionBox( out vector minMax[2] )
	{
		minMax[0] = "-3 0 -3";
		minMax[1] = "3 1.75 3";
		return true;
	}

	override void OnPartBuiltServer( notnull Man player, string part_name, int action_id )
	{
		m_Expansion_HasRamp = false;

		ExpansionUpdateBaseBuildingStateFromPartBuilt( part_name );

		super.OnPartBuiltServer(player, part_name, action_id );
		UpdateVisuals();
	}

	override void ExpansionUpdateBaseBuildingStateFromPartBuilt( string part_name )
	{
		if ( part_name == m_Expansion_CurrentBuild + "_finished" )
		{
			m_Expansion_HasRamp = true;
		}
	}

	override void OnPartDismantledServer( notnull Man player, string part_name, int action_id )
	{
		m_Expansion_HasRamp = false;

		super.OnPartDismantledServer( player, part_name, action_id );
		UpdateVisuals();
	}

	override void OnPartDestroyedServer( Man player, string part_name, int action_id, bool destroyed_by_connected_part = false )
	{
		m_Expansion_HasRamp = false;

		super.OnPartDestroyedServer( player, part_name, action_id, destroyed_by_connected_part );
		UpdateVisuals();
	}

	override bool CanPutInCargo( EntityAI parent )
	{
		return false;
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