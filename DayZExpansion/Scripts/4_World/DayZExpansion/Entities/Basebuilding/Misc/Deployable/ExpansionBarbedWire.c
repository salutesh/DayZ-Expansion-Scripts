/**
 * ExpansionBarbedWire.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionBarbedWire
 * @brief		
 **/
class ExpansionBarbedWire: ExpansionBaseBuilding 
{
	protected ref AreaDamageLoopedDeferred_NoVehicle m_AreaDamage;

	// ------------------------------------------------------------
	// Constructor
	// ------------------------------------------------------------
	void ExpansionBarbedWire()
	{
	}

	// ------------------------------------------------------------
	// Destructor
	// ------------------------------------------------------------
	void ~ExpansionBarbedWire()
	{
		DestroyDamageTrigger();
	}

	// ------------------------------------------------------------
	// Override EOnInit
	// ------------------------------------------------------------
	override void EOnInit( IEntity other, int extra)
	{
		super.EOnInit( other, extra );

		if ( GetGame().IsServer() )
		{
			GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( CreateDamageTrigger, 100, false );
		}
	}

	// ------------------------------------------------------------
	// CanPutIntoHands
	// ------------------------------------------------------------
	override bool CanPutIntoHands(EntityAI parent)
	{
		return false;
	}

	// ------------------------------------------------------------
	// CanPutInCargo
	// ------------------------------------------------------------
	override bool CanPutInCargo (EntityAI parent)
	{
		return false;
	}
	
	override bool CanObstruct()
	{
		return true;
	}

	// ------------------------------------------------------------
	// GetConstructionKitType
	// ------------------------------------------------------------
	override string GetConstructionKitType()
	{
		return "ExpansionBarbedWireKit";
	}

	void CreateDamageTrigger()
	{
		DestroyDamageTrigger();

		m_AreaDamage = new AreaDamageLoopedDeferred_NoVehicle( this );
		m_AreaDamage.SetDamageComponentType( AreaDamageComponentTypes.HITZONE );
		m_AreaDamage.SetExtents( "-2.96 0 -0.77", "2.97 1.36 0.74" );
		m_AreaDamage.SetLoopInterval( 1.0 );
		m_AreaDamage.SetDeferDuration( 0.2 );
		m_AreaDamage.SetHitZones( { "Torso","LeftHand","LeftLeg","LeftFoot","RightHand","RightLeg","RightFoot" } );
		m_AreaDamage.SetAmmoName( "BarbedWireHit" );
		m_AreaDamage.Spawn();
	}

	void DestroyDamageTrigger()
	{
		if ( m_AreaDamage )
		{
			m_AreaDamage.Destroy();
		}
	}

	// ------------------------------------------------------------
	// CanBeDamaged
	// ------------------------------------------------------------
	override bool CanBeDamaged()
	{
		return true;
	}

	override void SetPartsAfterStoreLoad()
	{
		//! No-op - needs to be here to avoid NULL pointers
	}

	// ------------------------------------------------------------
	// EEKilled
	// ------------------------------------------------------------
	override void EEKilled( Object killer )
	{
		bool canRaidBarbedWire = GetExpansionSettings().GetRaid().CanRaidBarbedWire;

		if ( !canRaidBarbedWire )
			return;

		super.EEKilled( killer );
	}

	override void ExpansionOnDestroyed( Object killer )
	{
		if ( !IsDamageDestroyed() )  //! When called by ExpansionActionDestroyBarbedWire health is at 1%, set health to 0 so EEKilled is called (destroy sound)
			SetHealth( 0 );

		//TODO: Maybe spawn wire as replacement?
		if ( !ToDelete() )
			Delete();
	}
	
	override protected string GetDestroySound()
	{		
		return "putDown_BarbedWire_SoundSet";
	}
}
