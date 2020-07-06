/**
 * ExpansionMerlin.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMerlinWreck: ExpansionHelicopterWreck {}

/**@class		ExpansionMerlin
 * @brief		
 **/
class ExpansionMerlin: ExpansionHelicopterScript
{
	// ------------------------------------------------------------
	void ExpansionMerlin()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMerlin::Constructor - Start");
		#endif

		//! Vanilla
		m_dmgContactCoef							= 0.06;

		//! explosions
		m_ExplosionSize								= 50;

		m_MaxSpeed									= 200.0;

		m_AltitudeFullForce							= 1000.0;
		m_AltitudeNoForce							= 2000.0;

		m_EngineStartDuration						= 20.0;

		m_MinAutoRotateSpeed						= 2.0;
		m_MaxAutoRotateSpeed						= 10.0;

		m_BodyFrictionCoef							= 1.6;
		m_LiftForceCoef								= 1.0;

		m_CyclicForceCoef							= 1.5;

		m_CyclicSideSpeed							= 2.0;
		m_CyclicSideMax								= 0.5;
		m_CyclicSideCoef							= 0.6;

		m_CyclicForwardSpeed						= 10.0;
		m_CyclicForwardMax							= 1.0;
		m_CyclicForwardCoef							= 0.4;

		m_AntiTorqueSpeed							= 1.8;
		m_AntiTorqueMax								= 0.04;

		m_BankForceCoef								= 0.2;
		m_TailForceCoef								= 6.0;

		RegisterNetSyncVariableBool( "m_MonitorEnabled" );

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMerlin::Constructor - End");
		#endif
	}

	// ------------------------------------------------------------
	override void LoadConstantVariables()
	{
		super.LoadConstantVariables();
	}
	
	// ------------------------------------------------------------
	override CarLightBase CreateFrontLight()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionMerlin::CreateFrontLight");
		#endif
		
		return CarLightBase.Cast( ScriptedLightBase.CreateLight( ExpansionHelicopterFrontLight ) );
	}
	
	// ------------------------------------------------------------
	override int GetAnimInstance()
	{
		return ExpansionVehicleAnimInstances.EXPANSION_MH6;
	}

	// ------------------------------------------------------------
	override int GetSeatAnimationType( int posIdx )
	{
		switch( posIdx )
		{
		case 0:
			return DayZPlayerConstants.VEHICLESEAT_DRIVER;
		case 1:
			return DayZPlayerConstants.VEHICLESEAT_CODRIVER;
		case 2:
			return DayZPlayerConstants.VEHICLESEAT_CODRIVER;
		case 3:
			return DayZPlayerConstants.VEHICLESEAT_CODRIVER;
		}

		return 0;
	}
	
	// ------------------------------------------------------------
	override int Get3rdPersonCameraType()
	{
		return DayZPlayerCameras.DAYZCAMERA_3RD_VEHICLE;
	}
	
	// ------------------------------------------------------------
	override bool CrewCanGetThrough( int posIdx )
	{		
		return true;
	}
	
	// ------------------------------------------------------------
	override bool IsVitalHelicopterBattery()
	{
		return true;
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
		return false;
	}

	// ------------------------------------------------------------
	override bool IsVitalIgniterPlug()
	{
		return true;
	}

	// ------------------------------------------------------------	
	override bool IsVitalHydraulicHoses()
	{
		return true;
	}

	// ------------------------------------------------------------
	override bool IsVitalEngineBelt()
	{
		return false;
	}

	// ------------------------------------------------------------
	override bool CanObjectAttach( Object obj )
	{
/* 		if ( vector.Distance( GetPosition(), obj.GetPosition() ) > m_BoundingRadius * 1.5 )
			return false;

		return true; */
		return false;
	}

	// ------------------------------------------------------------
	override float GetActionDistanceFuel()
	{
		return 2.5;
	}

	// ------------------------------------------------------------
	override float GetCameraHeight()
	{
		return 1;
	}

	// ------------------------------------------------------------
	override float GetCameraDistance()
	{
		return 25;
	}

	// ------------------------------------------------------------
	override float GetWreckAltitude()
	{
		return 1.5;
	}

	// ------------------------------------------------------------
	override vector GetWreckOffset()
	{
		return "0 0 -2.5";
	}
	
	// ------------------------------------------------------------
	override string GetAnimSourceFromSelection( string selection )
	{
		switch( selection )
		{
			case "cargoramp":
				return "cargoramp";
			case "door_back_l":
				return "door_back_l";
			case "door_back_r":
				return "door_back_r";
		}

		return "";
	}

	// --------------------------------------------------------- ---
	override void UpdateLights(int new_gear = -1) 
	{
		super.UpdateLights( new_gear );

		if ( !GetGame().IsMultiplayer() || GetGame().IsClient() ) 
		{
			ItemBase battery;
			
			if ( IsVitalHelicopterBattery() )
				battery = ItemBase.Cast( FindAttachmentBySlotName("ExpansionHelicopterBattery") );
			
			if ( battery )
			{
				int b;

				vector color;
				vector ambient;

				if ( m_HeadlightsOn )
				{
					if ( m_Lights.Count() == 0 )
					{
						CreateLights( this, "interiorlight1", ExpansionPointLight, Vector(1, 0.01, 0.01), Vector(1, 0.01, 0.01), 5, 0.85, false, true );
						CreateLights( this, "interiorlight2", ExpansionPointLight, Vector(1, 0.01, 0.01), Vector(1, 0.01, 0.01), 5, 0.85, false, true );
						CreateLights( this, "switch_lightsac", ExpansionPointLight, Vector(0, 0.1, 1), Vector(1, 1, 1), 5, 0.5, false, true );

						// CreateLights( this, "interiorlight1", ExpansionPointLight, Vector(0.1, 0.1, 0.1), Vector(0.1, 0.1, 0.1), 2, 0.1, false, true );
						// CreateLights( this, "interiorlight2", ExpansionPointLight, Vector(0.1, 0.1, 0.1), Vector(0.1, 0.1, 0.1), 2, 0.1, false, true );

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