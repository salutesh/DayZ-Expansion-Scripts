/**
 * ExpansionBarbedWire.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
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
	protected ref AreaDamageBase m_AreaDamage;

	protected ref Timer m_AreaDamageTimer;

	protected bool m_TriggerActive;

	// ------------------------------------------------------------
	// Constructor
	// ------------------------------------------------------------
	void ExpansionBarbedWire()
	{
		m_TriggerActive = false;

		m_AreaDamageTimer = new Timer();

		SetEventMask( EntityEvent.INIT );
	}

	// ------------------------------------------------------------
	// Destructor
	// ------------------------------------------------------------
	void ~ExpansionBarbedWire()
	{
	}

	// ------------------------------------------------------------
	// Override EOnInit
	// ------------------------------------------------------------
	override void EOnInit( IEntity other, int extra)
	{
		super.EOnInit( other, extra );

		if ( GetGame().IsServer() )
		{
			m_AreaDamageTimer.Run( 1, this, "CreateDamageTrigger", NULL, false );
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

		m_AreaDamage = new AreaDamageRegularDeferred( this );
		m_AreaDamage.SetExtents( "-2.96 0 -0.77", "2.97 1.36 0.74" );
		m_AreaDamage.SetLoopInterval( 1.0 );
		m_AreaDamage.SetDeferDuration( 0.2 );
		m_AreaDamage.SetHitZones( { "Torso","LeftHand","LeftLeg","LeftFoot","RightHand","RightLeg","RightFoot" } );
		m_AreaDamage.SetAmmoName( "BarbedWireHit" );
		m_AreaDamage.Spawn();

		m_TriggerActive = true;
	}

	void DestroyDamageTrigger()
	{
		if ( m_AreaDamage )
		{
			m_AreaDamage.Destroy();
		}

		m_TriggerActive = false;
	}

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
		if ( IsDamageDestroyed() )
			return;

		ItemBase item = ItemBase.Cast( GetGame().CreateObject( "BarbedWire", GetPosition() ) );
		item.SetOrientation( GetOrientation() );
		item.SetHealth( 0 );
		SetHealth( 0 );
	}
	
	override protected string GetDestroySound()
	{		
		return "putDown_BarbedWire_SoundSet";
	}
}
