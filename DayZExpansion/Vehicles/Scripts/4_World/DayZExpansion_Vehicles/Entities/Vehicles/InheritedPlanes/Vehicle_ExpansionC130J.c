/**
 * Vehicle_ExpansionC130J.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		Vehicle_ExpansionC130J
 * @brief		
 **/
class Vehicle_ExpansionC130J extends ExpansionVehiclePlaneBase
{
	// ------------------------------------------------------------
	void Vehicle_ExpansionC130J()
	{
		//! Vanilla
		m_dmgContactCoef							= 0.075;

		m_MaxSpeed									= 1000.0;

		m_BodyFrictionCoef							= 15.0;

		m_ElevatorForceCoef 						= 1.0;
		m_AileronForceCoef 							= 0.8;

		m_RudderForceCoef 							= 1.0;
		m_RudderInfluence 							= 0.98;
	}

	// ------------------------------------------------------------
	override int GetAnimInstance()
	{
		return ExpansionVehicleAnimInstances.EXPANSION_MH6;
	}

	// ------------------------------------------------------------
	override int GetSeatAnimationType( int posIdx )
	{
		if ( posIdx == 0 )
			return DayZPlayerConstants.VEHICLESEAT_DRIVER;

		return DayZPlayerConstants.VEHICLESEAT_CODRIVER;
	}

	// ------------------------------------------------------------
	override bool CrewCanGetThrough( int posIdx )
	{
		return true;
	}

	// ------------------------------------------------------------
	override bool CanReachDoorsFromSeat( string pDoorsSelection, int pCurrentSeat )
	{
		return true;		
	}

	// ------------------------------------------------------------
	override bool CanReachSeatFromSeat( int currentSeat, int nextSeat )
	{
		return true;
	}
	
	// ------------------------------------------------------------
	override bool CanReachSeatFromDoors( string pSeatSelection, vector pFromPos, float pDistance = 1.0 )
	{
		return true;		
	}

	// ------------------------------------------------------------
	override int Get3rdPersonCameraType()
	{
		return DayZPlayerCameras.DAYZCAMERA_3RD_VEHICLE;
	}
	
	// ------------------------------------------------------------
	override bool IsVitalAircraftBattery()
	{
		return true;
	}
	
	// ------------------------------------------------------------
	override bool IsVitalCarBattery()
	{
		return false;
	}

	// ------------------------------------------------------------
	override bool IsVitalSparkPlug()
	{
		return false;
	}
	
	// ------------------------------------------------------------
	override bool IsVitalRadiator()
	{
		return false;
	}
	
	// ------------------------------------------------------------
	override bool IsVitalGlowPlug()
	{
		return true;
	}

	// ------------------------------------------------------------
	override bool IsVitalEngineBelt()
	{
		return false;
	}

	// ------------------------------------------------------------
	override bool IsVitalTruckBattery()
	{
		return false;
	}

	// ------------------------------------------------------------
	override float GetCameraDistance()
	{
		return 25;
	}

	// ------------------------------------------------------------
	override bool CanObjectAttach( Object obj )
	{
 		if ( vector.Distance( GetPosition(), obj.GetPosition() ) > m_BoundingRadius * 1.5 )
			return false;
		
		return true;
	}

	// ------------------------------------------------------------
	override bool LeavingSeatDoesAttachment( int posIdx )
	{
		// @CAMINOonPC#6971 Never implemented this after being told to for the past 3 months
		return true;
	}

	// --------------------------------------------------------- ---
	override void UpdateLights(int new_gear = -1) 
	{
		super.UpdateLights( new_gear );

		if ( !GetGame().IsMultiplayer() || GetGame().IsClient() ) 
		{
			ItemBase battery;
			
			if ( IsVitalCarBattery() ) battery = ItemBase.Cast( FindAttachmentBySlotName("CarBattery") );
			if ( IsVitalTruckBattery() ) battery = ItemBase.Cast( FindAttachmentBySlotName("TruckBattery") );
			if ( IsVitalHelicopterBattery() ) battery = ItemBase.Cast( FindAttachmentBySlotName("ExpansionHelicopterBattery") );
			if ( IsVitalAircraftBattery() ) battery = ItemBase.Cast( FindAttachmentBySlotName("ExpansionAircraftBattery") );
			
			if ( battery )
			{
				int b;

				vector color;
				vector ambient;

				if ( m_HeadlightsOn )
				{
					if ( m_Lights.Count() == 0 )
					{
						CreateLights( this, "l2 svetlo", ExpansionHelicopterFrontLight, Vector(1.0, 0.8, 0.6), Vector(1.0, 0.8, 0.6), 220, 5, false, true );
						CreateLights( this, "p2 svetlo", ExpansionHelicopterFrontLight, Vector(1.0, 0.8, 0.6), Vector(1.0, 0.8, 0.6), 220, 5, false, true );
						CreateLights( this, "zamerny", ExpansionPointLight, Vector(1, 0.01, 0.01), Vector(1, 0.01, 0.01), 5, 0.85, false, true );

						//CreateLights( this, "interiorlight1", ExpansionPointLight, Vector(0.1, 0.1, 0.1), Vector(0.1, 0.1, 0.1), 2, 0.1, false, true );
						//CreateLights( this, "interiorlight2", ExpansionPointLight, Vector(0.1, 0.1, 0.1), Vector(0.1, 0.1, 0.1), 2, 0.1, false, true );

						// CreateParticle( this, "bily pozicni", ParticleList.EXPANSION_LIGHT_WHITE );
						// CreateParticle( this, "zeleny pozicni", ParticleList.EXPANSION_LIGHT_YELLOW );
						// CreateParticle( this, "cerveny pozicni", ParticleList.EXPANSION_LIGHT_RED );
						// CreateParticle( this, "cerveny pozicni blik", ParticleList.EXPANSION_LIGHT_RED );
					}
				}
				else
				{
					for ( b = 0; b < m_Particles.Count(); b++ )
					{
						m_Particles[b].Stop( );

						GetGame().ObjectDelete( m_Particles[b] );
					}

					for ( b =- 0; b < m_Lights.Count(); b++ )
					{
						m_Lights[b].ExpansionSetEnabled( false );

						GetGame().ObjectDelete( m_Lights[b] );
					}

					m_Lights.Clear();
				}
			}
		}
	}
}