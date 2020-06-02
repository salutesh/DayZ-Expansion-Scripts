/**
 * ExpansionSignRoadBarrier.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
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
	protected ScriptedLightBase m_Light;

	// ------------------------------------------------------------
	// Constructor
	// ------------------------------------------------------------
    void ExpansionSignRoadBarrier()
    {
    }

	// ------------------------------------------------------------
	// Destructor
	// ------------------------------------------------------------
	void ~ExpansionSignRoadBarrier()
	{
	}

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
			SetObjectMaterial( 0, "dayzexpansion\objects\basebuilding\misc\signs\data\bariera_lampa_on.rvmat" );

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
			SetObjectMaterial( 0, "dayzexpansion\objects\basebuilding\misc\signs\data\bariera_lampa.rvmat" );

			if ( m_Light )
			{
				m_Light.Destroy();
			}
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
	override void OnPlacementComplete( Man player )
	{
		if ( IsMissionHost() )
		{
			//! Create sign
			PlayerBase player_base = PlayerBase.Cast( player );
			vector position = player_base.GetLocalProjectionPosition();
			vector orientation = player_base.GetLocalProjectionOrientation();
			
			SetPosition( position );
			SetOrientation( orientation );
			
			//! Make the kit invisible, so it can be destroyed from deploy UA when action ends
			HideAllSelections();
		}	
		
		SetIsDeploySound( true );
	}
} 