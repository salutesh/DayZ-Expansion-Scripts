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
			DeferredInit();
		}
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

	override void DeferredInit()
	{
		m_AreaDamageTimer.Run( 1, this, "CreateDamageTrigger", NULL, false ); 
	}

	void CreateDamageTrigger()
	{
		DestroyDamageTrigger();

		m_AreaDamage = new AreaDamageRegularDeferred( this );
		m_AreaDamage.SetExtents( "-2.96 0 -0.77", "2.97 1.36 0.74" );
		m_AreaDamage.SetLoopInterval( 0.5 );
		m_AreaDamage.SetDeferDuration( 0.5 );
		m_AreaDamage.SetHitZones( { "Head","Torso","LeftHand","LeftLeg","LeftFoot","RightHand","RightLeg","RightFoot" } );
		m_AreaDamage.SetAmmoName( "BarbedWireDamage" );
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
		return false;
	}

	// ------------------------------------------------------------
	// AfterStoreLoad
	// ------------------------------------------------------------
	override void AfterStoreLoad()
	{
		
	}
} 