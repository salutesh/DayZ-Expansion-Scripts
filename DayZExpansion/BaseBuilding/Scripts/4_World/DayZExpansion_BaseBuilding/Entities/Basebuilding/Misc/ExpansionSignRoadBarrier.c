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

	//! Expansion turn on sound instance
	protected EffectSound m_Expansion_SoundTurnOn;

	//! Expansion turn off sound instance
	protected EffectSound m_Expansion_SoundTurnOff;
	
	protected ExpansionPointLight m_Expansion_Light;

	protected ref Timer m_Expansion_BlinkTimer;

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

		if ( !GetGame().IsDedicatedServer() )
			Expansion_SoundTurnOn();
	}

	override void OnWorkStart()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.GENERAL_ITEMS, this);
#endif

		super.OnWorkStart();

		if ( !GetGame().IsDedicatedServer() )
		{
			if ( !m_Expansion_Light )
			{
				m_Expansion_Light = ExpansionPointLight.Cast(ScriptedLightBase.CreateLight( ExpansionPointLight, "0 0 0" ));

				m_Expansion_Light.FadeIn(0.3);			
				m_Expansion_Light.SetCastShadow(true);				
				m_Expansion_Light.SetFlareVisible(false);

				vector colorRGB = Vector( 1.0, 0.5, 0 );
				m_Expansion_Light.SetDiffuseColor( colorRGB[0], colorRGB[1], colorRGB[2] );
				m_Expansion_Light.SetAmbientColor( colorRGB[0], colorRGB[1], colorRGB[2] );
				m_Expansion_Light.SetRadiusTo( 2.5 );

				m_Expansion_Light.AttachOnMemoryPoint( this, "zluty pozicni blik" );
			}

			Expansion_EnableLight(true);

			if (!m_Expansion_BlinkTimer)
			{
				m_Expansion_BlinkTimer = new Timer();
			}

			m_Expansion_BlinkTimer.Run(1.0, this, "Expansion_BlinkLight", null, true);
		}
	}

	void Expansion_BlinkLight()
	{
		if (m_Expansion_Light.m_Brightness)
			Expansion_EnableLight(false);
		else
			Expansion_EnableLight(true);
	}

	void Expansion_EnableLight(bool enable)
	{
		if (enable)
		{
			SetObjectMaterial( 0, "DayZExpansion\\Objects\\Basebuilding\\Misc\\Signs\\Data\\bariera_lampa_on.rvmat" );
			if (m_Expansion_Light)
				m_Expansion_Light.SetBrightnessTo(1.0);
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).CallLater(Expansion_EnableLight, 250, false, false);
		}
		else
		{
			SetObjectMaterial( 0, "DayZExpansion\\Objects\\Basebuilding\\Misc\\Signs\\Data\\bariera_lampa.rvmat" );
			if (m_Expansion_Light)
				m_Expansion_Light.SetBrightnessTo(0.0);
		}
	}

	// ------------------------------------------------------------
	// OnSwitchOff
	// ------------------------------------------------------------
	override void OnSwitchOff()
	{
		super.OnSwitchOff();

		if ( !GetGame().IsDedicatedServer() )
			Expansion_SoundTurnOff();
	}

	override void OnWorkStop()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.GENERAL_ITEMS, this);
#endif

		super.OnWorkStop();

		if ( !GetGame().IsDedicatedServer() )
		{
			if ( m_Expansion_Light )
			{
				Expansion_EnableLight(false);
				m_Expansion_Light.Destroy();
			}

			if (m_Expansion_BlinkTimer)
			{
				m_Expansion_BlinkTimer.Stop();
			}
		}
	}

	protected void Expansion_SoundTurnOn()
	{
		PlaySoundSet( m_Expansion_SoundTurnOn, SOUND_TURN_ON, 0, 0 );
	}

	protected void Expansion_SoundTurnOff()
	{
		PlaySoundSet( m_Expansion_SoundTurnOff, SOUND_TURN_OFF, 0, 0 );
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