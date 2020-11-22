/**
 * ExpansionSoundProxyBase.c
 *
 * Arkensor
 * www.arkensor.net/
 * © 2019 Arkensor
 *
*/

class ExpansionSoundProxyBase extends CarScript
{
	void ExpansionSoundProxyBase()
	{
		SetAllowDamage( false );

		dBodyActive( this, ActiveState.INACTIVE );
		dBodyDynamic( this, false );
		dBodySetMass( this, 0 );
		SetVelocity( this, "0 0 0" );
		dBodySetAngularVelocity( this, "0 0 0" );
	}

	void SetExpansionVehicle( ExpansionVehicleBase vehicle ) {}
	void SetBoat( ExpansionBoatScript boat ) {}

	override void OnContact( string zoneName, vector localPos, IEntity other, Contact data ) {}
	override void EOnPostSimulate( IEntity other, float timeSlice ) {}
	override bool IsInventoryVisible() { return false; }
}

class ExpansionVehicleScriptSoundProxyBase extends ExpansionSoundProxyBase
{
	private ExpansionVehicleBase m_Vehicle;

	override void SetExpansionVehicle( ExpansionVehicleBase vehicle )
	{
		m_Vehicle = vehicle;
	}

	override float OnSound( CarSoundCtrl ctrl, float oldValue )
	{		
		return m_Vehicle.OnSound( ctrl, oldValue );
	}
}

class ExpansionBoatScriptSoundProxyBase extends ExpansionSoundProxyBase
{
	private ExpansionBoatScript m_Boat;

	override void SetBoat( ExpansionBoatScript boat )
	{
		m_Boat = boat;
	}

	override float OnSound( CarSoundCtrl ctrl, float oldValue )
	{		
		//! return m_Boat.OnSoundBoat( ctrl, oldValue );
	}
}