/**
 * ExpansionSignRoadBarrier.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionSignRoadBarrier
 * @brief		
 **/
class ExpansionSignRoadBarrier: Container_Base 
{

	typename ATTACHMENT_BATTERY = Battery9V;

	//! Expansion turn on sound
	const string SOUND_TURN_ON		= "Flashlight_TurnOn_SoundSet";

	//! Expansion turn off sound
	const string SOUND_TURN_OFF		= "Flashlight_TurnOff_SoundSet";

	//! Expansion burning sound instance
	protected EffectSound m_SoundBurningLoop;

	//! Expansion turn on sound instance
	protected EffectSound m_SoundTurnOn;

	//! Expansion turn off sound instance
	protected EffectSound m_SoundTurnOff;
	
	protected ScriptedLightBase m_Light;

	// ------------------------------------------------------------
	// SetActions
	// ------------------------------------------------------------
	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ActionTogglePlaceObject);
		AddAction(ActionPlaceObject);

		AddAction(ActionTurnOnWhileOnGround);
		AddAction(ActionTurnOffWhileOnGround);
	}

	// ------------------------------------------------------------
	// OnSwitchOn
	// ------------------------------------------------------------
	override void OnSwitchOn()
	{
		super.OnSwitchOn();

		if ( GetGame().IsClient() || !GetGame().IsMultiplayer() )
		{
			SoundTurnOn();
			SetObjectMaterial( 0, "DayZExpansion\\Objects\\Basebuilding\\Misc\\Signs\\Data\\bariera_lampa_on.rvmat" );

			if ( !m_Light )
			{
				m_Light = ScriptedLightBase.CreateLight( ExpansionPointLight, "0 0 0" );

				m_Light.FadeIn(0.3);			
				m_Light.SetCastShadow(true);				
				m_Light.SetFlareVisible(false);

				vector colorRGB = Vector( 1.0, 0.5, 0 );
				m_Light.SetDiffuseColor( colorRGB[0], colorRGB[1], colorRGB[2] );
				m_Light.SetAmbientColor( colorRGB[0], colorRGB[1], colorRGB[2] );
				m_Light.SetRadiusTo( 2.5 );
				m_Light.SetBrightnessTo( 1 );

				m_Light.AttachOnMemoryPoint( this, "zluty pozicni blik" );

				m_Light.SetEnabled( true );
			}
			else
			{
				m_Light.SetEnabled( true );
			}
		}
	}

	// ------------------------------------------------------------
	// OnSwitchOff
	// ------------------------------------------------------------
	override void OnSwitchOff()
	{
		super.OnSwitchOff();

		if ( GetGame().IsClient() || !GetGame().IsMultiplayer() )
		{
			SoundTurnOff();
			SetObjectMaterial( 0, "DayZExpansion\\Objects\\Basebuilding\\Misc\\Signs\\Data\\bariera_lampa.rvmat" );

			if ( m_Light )
			{
				m_Light.Destroy();
			}
		}
	}

	// ------------------------------------------------------------
	// Expansion SoundTurnOn
	// ------------------------------------------------------------
	protected void SoundTurnOn()
	{
		PlaySoundSet( m_SoundTurnOn, SOUND_TURN_ON, 0.1, 0.1 );
	}

	// ------------------------------------------------------------
	// Expansion SoundTurnOff
	// ------------------------------------------------------------
	protected void SoundTurnOff()
	{
		PlaySoundSet( m_SoundTurnOff, SOUND_TURN_OFF, 0.1, 0.1 );
	}

	// ------------------------------------------------------------
	// CanPutInCargo
	// ------------------------------------------------------------
	override bool CanPutInCargo (EntityAI parent)
	{
		return true;
	}

	// ------------------------------------------------------------
	// IsContainer
	// ------------------------------------------------------------
	override bool IsContainer()
	{
		return true;
	}

	// ------------------------------------------------------------
	// IsHeavyBehaviour
	// ------------------------------------------------------------
	override bool IsHeavyBehaviour() 
	{
		return true;
	}

	// ------------------------------------------------------------
	// OnPlacementComplete
	// ------------------------------------------------------------
	override void OnPlacementComplete( Man player, vector position = "0 0 0", vector orientation = "0 0 0" )
	{
		if ( IsMissionHost() )
		{			
			SetPosition( position );
			SetOrientation( orientation );
			
			//! Make the kit invisible, so it can be destroyed from deploy UA when action ends
			HideAllSelections();
		}	
		
		SetIsDeploySound( true );
	}

	
	// ------------------------------------------------------------
	// OnWork
	// ------------------------------------------------------------
	override void OnWork( float consumed_energy )
	{
		if ( !GetGame().IsServer()  ||  !GetGame().IsMultiplayer() ) // Client side
		{
			Battery9V battery = Battery9V.Cast( GetCompEM().GetEnergySource() );
			
			if (battery  &&  m_Light)
			{
				float efficiency = battery.GetEfficiency0To1();
				
				if ( efficiency < 1 )
				{
					OnSwitchOn();
				}
				else
				{
					OnSwitchOff();
				}
			}
		}
	}

	// ------------------------------------------------------------
	// EEItemDetached
	// ------------------------------------------------------------
	override void EEItemDetached( EntityAI item, string slot_name ) 
	{
		super.EEItemDetached( item, slot_name );
			Battery9V battery = Battery9V.Cast( GetCompEM().GetEnergySource() );
		
		if ( item.Type() == ATTACHMENT_BATTERY )
		{	
			OnSwitchOff();
		}	
	}
} 