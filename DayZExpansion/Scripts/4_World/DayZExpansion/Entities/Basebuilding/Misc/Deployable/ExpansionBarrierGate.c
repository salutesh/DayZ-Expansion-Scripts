/**
 * ExpansionBarrierGate.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionBarrierGate
 * @brief		
 **/
class ExpansionBarrierGate: ExpansionBaseBuilding
{
	protected bool m_IsOpened;

	// ------------------------------------------------------------
	// Constructor
	// ------------------------------------------------------------
	void ExpansionBarrierGate()
	{
	}

	// ------------------------------------------------------------
	// Destructor
	// ------------------------------------------------------------
	void ~ExpansionBarrierGate()
	{
	}

	// ------------------------------------------------------------
	// CanPutInCargo
	// ------------------------------------------------------------
	override bool CanPutInCargo (EntityAI parent)
	{
		return false;
	}

	// ------------------------------------------------------------
	// IsInventoryVisible
	// ------------------------------------------------------------
	override bool IsInventoryVisible()
	{
		return true;
	}

	// ------------------------------------------------------------
	// GetConstructionKitType
	// ------------------------------------------------------------
	override string GetConstructionKitType()
	{
		return "ExpansionBarrierGateKit";
	}

	// ------------------------------------------------------------
	// SetActions
	// ------------------------------------------------------------
	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ActionFoldBaseBuildingObject);
		//! AddAction(ExpansionActionOpen);
		//! AddAction(ExpansionActionClose);
	}

	// ------------------------------------------------------------
	// Expansion CanOpen
	// ------------------------------------------------------------
	override bool ExpansionCanOpen( PlayerBase player, string selection )
	{
		if (selection == "gate" && GetAnimationPhase("gate") == 1)
			return true;
		
		return false;
	}

	// ------------------------------------------------------------
	// Expansion CanClose
	// ------------------------------------------------------------
	override bool CanClose( string selection )
	{
		if (selection == "gate" && GetAnimationPhase("gate") == 0)
			return true;
		
		return false;
	}

	// ------------------------------------------------------------
	// Expansion Open
	// ------------------------------------------------------------
	override void Open( string selection ) 
	{	
		//! Door open animation
		if (selection == "gate")
			SetAnimationPhase( "gate", 0 );

		m_IsOpened = true;

		SetSynchDirty();
		
		super.Open( selection );
	}
	
	// ------------------------------------------------------------
	// Expansion Close
	// ------------------------------------------------------------
	override void Close( string selection ) 
	{	
		//! Door open animation
		if (selection == "gate")
			SetAnimationPhase( "gate", 1 );

		m_IsOpened = false;
		
		SetSynchDirty();
		
		super.Close( selection );
	}

	override void OnStoreSave(ParamsWriteContext ctx)
	{
		#ifdef CF_MODULE_MODSTORAGE
		if ( GetGame().SaveVersion() >= 116 )
		{
			super.OnStoreSave( ctx );
			return;
		}
		#endif

		super.OnStoreSave( ctx );

		ctx.Write( m_IsOpened );
	}

	override bool OnStoreLoad( ParamsReadContext ctx, int version )
	{
		#ifdef CF_MODULE_MODSTORAGE
		if ( version >= 116 )
			return super.OnStoreLoad( ctx, version );
		#endif

		if ( !super.OnStoreLoad( ctx, version ) )
			return false;

		if ( Expansion_Assert_False( ctx.Read( m_IsOpened ), "[" + this + "] Failed reading m_IsOpened" ) )
			return false;

		return true;
	}

	#ifdef CF_MODULE_MODSTORAGE
	override void CF_OnStoreSave( CF_ModStorage storage, string modName )
	{
		super.CF_OnStoreSave( storage, modName );

		if ( modName != "DZ_Expansion" )
			return;
		
		storage.Write( m_IsOpened );
	}
	
	override bool CF_OnStoreLoad( CF_ModStorage storage, string modName )
	{
		if ( !super.CF_OnStoreLoad( storage, modName ) )
			return false;

		if ( modName != "DZ_Expansion" )
			return true;

		if ( Expansion_Assert_False( storage.Read( m_IsOpened ), "[" + this + "] Failed reading m_IsTerritory" ) )
			return false;

		return true;
	}
	#endif

	// ------------------------------------------------------------
	// CanBeDamaged
	// ------------------------------------------------------------
	override bool CanBeDamaged()
	{
		return true;
	}

	// ------------------------------------------------------------
	// AfterStoreLoad
	// ------------------------------------------------------------
	override void AfterStoreLoad()
	{
		//super.AfterStoreLoad();

		if ( m_IsOpened )
		{
			SetAnimationPhase( "gate", 0 );
		} else 
		{
			SetAnimationPhase( "gate", 1 );
		}
	}
}