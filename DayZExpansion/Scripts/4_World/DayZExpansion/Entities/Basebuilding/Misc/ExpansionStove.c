/**
 * ExpansionStove.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionStove
 * @brief		
 **/
class ExpansionStove extends Container_Base 
{
	//! Expansion attachment of cooking pot
	typename ATTACHMENT_COOKING_POT = Pot;

	//! Expansion reference to cooking process
	ref Cooking m_CookingProcess;

	//! Expansion cooking equipment
	ItemBase m_CookingEquipment;

	//! Expansion is open switch
	protected bool m_IsOpen;

	//! Temperature threshold for starting coooking process (degree Celsius)
	protected const float PARAM_COOKING_TEMP_THRESHOLD			= 100;

	//! How much will temperature increase when attached on burning fireplace (degree Celsius)
	protected const float PARAM_COOKING_EQUIP_TEMP_INCREASE		= 10;	

	//! Maximum temperature of attached cooking equipment (degree Celsius)	
	protected const float PARAM_COOKING_EQUIP_MAX_TEMP			= 250;	

	//! Cooking time increase coeficient, can be used when balancing how fast a food can be cooked	
	protected const float PARAM_COOKING_TIME_INC_COEF			= 0.5;		

	//! Expansion burning sound
	const string SOUND_BURNING 		= "portablegasstove_burn_SoundSet";

	//! Expansion turn on sound
	const string SOUND_TURN_ON		= "portablegasstove_turn_on_SoundSet";

	//! Expansion turn off sound
	const string SOUND_TURN_OFF		= "portablegasstove_turn_off_SoundSet";

	//! Expansion burning sound instance
	protected EffectSound m_SoundBurningLoop;

	//! Expansion turn on sound instance
	protected EffectSound m_SoundTurnOn;

	//! Expansion turn off sound instance
	protected EffectSound m_SoundTurnOff;

	// ------------------------------------------------------------
	// Constructor
	// ------------------------------------------------------------
	void ExpansionStove()
	{
		RegisterNetSyncVariableBool("m_IsOpen");
	}

	// ------------------------------------------------------------
	// Destructor
	// ------------------------------------------------------------
	void ~ExpansionStove()
	{
	}

	// ------------------------------------------------------------
	// SetActions
	// ------------------------------------------------------------
	override void SetActions()
	{
		super.SetActions();
		
		AddAction(ActionLightItemOnFire);
		AddAction(ActionTurnOnWhileOnGround);
		AddAction(ActionTurnOffWhileOnGround);
		AddAction(ExpansionActionOpen);
		AddAction(ExpansionActionClose);

		AddAction(ActionTogglePlaceObject);
		AddAction(ActionPlaceObject);
	}

	// ------------------------------------------------------------
	// Expansion SetCookingEquipment
	// ------------------------------------------------------------
	void SetCookingEquipment( ItemBase equipment )
	{
		m_CookingEquipment = equipment;
	}
	
	// ------------------------------------------------------------
	// Expansion ClearCookingEquipment
	// ------------------------------------------------------------
	void ClearCookingEquipment()
	{
		SetCookingEquipment( NULL );
	}

	// ------------------------------------------------------------
	// Expansion GetCookingEquipment
	// ------------------------------------------------------------
	ItemBase GetCookingEquipment()
	{
		return m_CookingEquipment;
	}

	// ------------------------------------------------------------
	// Expansion CookWithEquipment
	// ------------------------------------------------------------
	void CookWithEquipment()
	{
		if ( m_CookingProcess == NULL )
		{
			m_CookingProcess = new Cooking();
		}
		
		m_CookingProcess.CookWithEquipment ( GetCookingEquipment(), PARAM_COOKING_TIME_INC_COEF );
	}

	// ------------------------------------------------------------
	// Expansion DestroyFireplace
	// ------------------------------------------------------------
	void DestroyFireplace()
	{
		GetGame().ObjectDelete( this );
	}

	// ------------------------------------------------------------
	// Expansion RemoveCookingAudioVisuals
	// ------------------------------------------------------------
	protected void RemoveCookingAudioVisuals()
	{
		if ( GetCookingEquipment() )
		{
			Bottle_Base cooking_pot = Bottle_Base.Cast( GetCookingEquipment() );
			cooking_pot.RemoveAudioVisualsOnClient();
		}
	}

	// ------------------------------------------------------------
	// Expansion SoundBurningStart
	// ------------------------------------------------------------
	protected void SoundBurningStart()
	{
		PlaySoundSetLoop( m_SoundBurningLoop, SOUND_BURNING, 0.1, 0.3 );
	}

	// ------------------------------------------------------------
	// Expansion SoundBurningStop
	// ------------------------------------------------------------
	protected void SoundBurningStop()
	{
		StopSoundSet( m_SoundBurningLoop );
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
	// Expansion IsOpenable
	// ------------------------------------------------------------
	override bool IsOpenable()
	{
		return true;
	}

	// ------------------------------------------------------------
	// Expansion CanOpen
	// ------------------------------------------------------------
	override bool CanOpen( PlayerBase player, string selection )
	{
		if (IsLocked())
			return false;
			
		if (selection == "doors1" && GetAnimationPhase("doors1") == 0 && IsOpenable())
			return true;
		
		return false;
	}

	// ------------------------------------------------------------
	// Expansion CanClose
	// ------------------------------------------------------------
	override bool CanClose( string selection )
	{
		if (selection == "doors1" && GetAnimationPhase("doors1") == 1 && IsOpenable())
			return true;
		
		return false;
	}

	// ------------------------------------------------------------
	// Expansion Open
	// ------------------------------------------------------------
	override void Open( string selection ) 
	{	
		m_IsOpen = true;

		//! Door open animation
		if (selection == "doors1")
			SetAnimationPhase( "doors1", 1 );

		SetSynchDirty();
		
		super.Open( selection );
	}
	
	// ------------------------------------------------------------
	// Expansion Close
	// ------------------------------------------------------------
	override void Close( string selection ) 
	{	
		m_IsOpen = false;

		//! Door open animation
		if (selection == "doors1")
			SetAnimationPhase( "doors1", 0 );
		
		SetSynchDirty();
		
		super.Close( selection );
	}

	// ------------------------------------------------------------
	// Expansion IsOpened
	// ------------------------------------------------------------
	override bool IsOpened()
	{
		return m_IsOpen;
	}

	// ------------------------------------------------------------
	// EEItemAttached
	// ------------------------------------------------------------
	override void EEItemAttached( EntityAI item, string slot_name ) 
	{
		super.EEItemAttached( item, slot_name );
		
		if ( item.Type() == ATTACHMENT_COOKING_POT )
		{
			ItemBase item_base = ItemBase.Cast( item );
			SetCookingEquipment( item_base );
		}
	}

	// ------------------------------------------------------------
	// EEItemDetached
	// ------------------------------------------------------------
	override void EEItemDetached( EntityAI item, string slot_name ) 
	{
		super.EEItemDetached( item, slot_name );
		
		if ( item.Type() == ATTACHMENT_COOKING_POT )
		{
			ClearCookingEquipment();
			
			RemoveCookingAudioVisuals();			
		}	
	}

	// ------------------------------------------------------------
	// OnSwitchOn
	// ------------------------------------------------------------
	override void OnSwitchOn()
	{
		super.OnSwitchOn();
		
		SoundTurnOn();
	}

	// ------------------------------------------------------------
	// OnSwitchOff
	// ------------------------------------------------------------
	override void OnSwitchOff()
	{
		super.OnSwitchOff();
		
		SoundTurnOff();
	}

	// ------------------------------------------------------------
	// OnWorkStart
	// ------------------------------------------------------------
	override void OnWorkStart()
	{
		SoundBurningStart();
	}

	// ------------------------------------------------------------
	// OnWorkStop
	// ------------------------------------------------------------
	override void OnWorkStop()
	{
		RemoveCookingAudioVisuals();
		
		SoundBurningStop();
	}

	// ------------------------------------------------------------
	// OnWork
	// ------------------------------------------------------------
	override void OnWork( float consumed_energy )
	{
		//! Manage cooking equipment
		if ( GetCookingEquipment() )
		{
			if ( GetGame() && GetGame().IsServer() )
			{
				float cook_equip_temp = GetCookingEquipment().GetTemperature();
				
				//! Start cooking
				if ( cook_equip_temp >= PARAM_COOKING_TEMP_THRESHOLD )
				{
					CookWithEquipment();
				}				
				
				//! Set temperature to cooking equipment
				cook_equip_temp = cook_equip_temp + PARAM_COOKING_EQUIP_TEMP_INCREASE;
				cook_equip_temp = Math.Clamp ( cook_equip_temp, 0, PARAM_COOKING_EQUIP_MAX_TEMP );
				GetCookingEquipment().SetTemperature( cook_equip_temp );
			}
		}
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
	// IsIgnited
	// ------------------------------------------------------------
	override bool IsIgnited()
	{
		return GetCompEM().IsWorking();
	}

	// ------------------------------------------------------------
	// CanIgniteItem
	// ------------------------------------------------------------
	override bool CanIgniteItem(EntityAI ignite_target = NULL)
	{
		return GetCompEM().IsWorking();
	}
	
	// ------------------------------------------------------------
	// CanReceiveItemIntoCargo
	// ------------------------------------------------------------
	override bool CanReceiveItemIntoCargo(EntityAI cargo)
	{
		return IsOpened();
	}
	
	// ------------------------------------------------------------
	// CanReleaseCargo
	// ------------------------------------------------------------
	override bool CanReleaseCargo(EntityAI attachment)
	{
		return IsOpened();
	}

	// ------------------------------------------------------------
	// OnPlacementComplete
	// ------------------------------------------------------------
	override void OnPlacementComplete( Man player )
	{
		if ( IsMissionHost() )
		{
			//! Create stove
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