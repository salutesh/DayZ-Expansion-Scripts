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
class ExpansionStove extends FireplaceBase 
{
	//! Expansion burning, turn on and turn off sound
	const string SOUND_BURNING 		= "portablegasstove_burn_SoundSet";
	const string SOUND_TURN_ON		= "portablegasstove_turn_on_SoundSet";
	const string SOUND_TURN_OFF		= "portablegasstove_turn_off_SoundSet";

	protected EffectSound m_SoundBurningLoop;
	protected EffectSound m_SoundTurnOn;
	protected EffectSound m_SoundTurnOff;
	
	//cooking
	protected const float PARAM_COOKING_TIME_INC_COEF			= 0.5;		//cooking time increase coeficient, can be used when balancing how fast a food can be cooked

	//! Expansion is open switch
	protected bool m_IsOpen;

	// ------------------------------------------------------------
	// Expansion GetCookingEquipment
	// ------------------------------------------------------------
	override ItemBase GetCookingEquipment()
	{
		return m_CookingEquipment;
	}

	// ------------------------------------------------------------
	// Expansion SetCookingEquipment
	// ------------------------------------------------------------
	override void SetCookingEquipment( ItemBase equipment )
	{
		m_CookingEquipment = equipment;
	}
	
	// ------------------------------------------------------------
	// Expansion ClearCookingEquipment
	// ------------------------------------------------------------
	override void ClearCookingEquipment()
	{
		SetCookingEquipment( NULL );
	}

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
	
	//================================================================
	// SOUNDS
	//================================================================
	protected void SoundBurningStart()
	{
		PlaySoundSetLoop( m_SoundBurningLoop, SOUND_BURNING, 0.1, 0.3 );
	}
	
	protected void SoundBurningStop()
	{
		StopSoundSet( m_SoundBurningLoop );
	}	

	protected void SoundTurnOn()
	{
		PlaySoundSet( m_SoundTurnOn, SOUND_TURN_ON, 0.1, 0.1 );
	}
	
	protected void SoundTurnOff()
	{
		PlaySoundSet( m_SoundTurnOff, SOUND_TURN_OFF, 0.1, 0.1 );
	}

	// ------------------------------------------------------------
	// Expansion CookWithEquipment
	// ------------------------------------------------------------
	override void CookWithEquipment()
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
	override void DestroyFireplace()
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
	// Expansion ExpansionIsOpenable
	// ------------------------------------------------------------
	override bool ExpansionIsOpenable()
	{
		return true;
	}

	// ------------------------------------------------------------
	// Expansion CanOpen
	// ------------------------------------------------------------
	override bool ExpansionCanOpen( PlayerBase player, string selection )
	{			
		if (selection == "doors1" && GetAnimationPhase("doors1") == 0 && ExpansionIsOpenable())
			return true;
		
		return false;
	}

	// ------------------------------------------------------------
	// Expansion CanClose
	// ------------------------------------------------------------
	override bool CanClose( string selection )
	{
		if (selection == "doors1" && GetAnimationPhase("doors1") == 1 && ExpansionIsOpenable())
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

	//returns if item attached to fireplace is fuel
	protected override bool IsFuel( ItemBase item )
	{
		if ( item.IsKindOf( "SmallGasCanister" ) || item.IsKindOf( "MediumGasCanister" ) || item.IsKindOf( "LargeGasCanister" ) )
		{
			return true;
		}
		
		return false;
	}

	//================================================================
	// ATTACHMENTS
	//================================================================	
	override bool CanReceiveAttachment( EntityAI attachment, int slotId )
	{
		if ( !super.CanReceiveAttachment(attachment, slotId) )
			return false;	
		
		ItemBase item = ItemBase.Cast( attachment );
		
		//fuel items
		if ( IsFuel ( item ) )
			return true;
		
		//direct cooking slots
		if ( ( item.Type() == ATTACHMENT_COOKING_POT ) || ( item.Type() == ATTACHMENT_FRYING_PAN ) || ( item.IsKindOf( "Edible_Base" ) ) )
			return true;
		
		return false;
	}
	
	override bool CanLoadAttachment( EntityAI attachment )
	{
		if ( !super.CanLoadAttachment(attachment) )
			return false;	
		
		ItemBase item = ItemBase.Cast( attachment );
		
		//fuel items
		if ( IsFuel ( item ) )
			return true;
		
		//direct cooking slots
		if ( ( item.Type() == ATTACHMENT_COOKING_POT ) || ( item.Type() == ATTACHMENT_FRYING_PAN ) || ( item.IsKindOf( "Edible_Base" ) ) )
			return true;
		
		return false;
	}

	override bool CanReleaseAttachment( EntityAI attachment )
	{
		if( !super.CanReleaseAttachment( attachment ) )
			return false;
		
		ItemBase item = ItemBase.Cast( attachment );
		
		//fuel items
		if ( IsFuel( item ) && !IsIgnited() )
		{
			return true;
		}
		
		//has last attachment and there are still items in cargo
		if ( GetInventory().AttachmentCount() == 1 && GetInventory().GetCargo().GetItemCount() != 0 )
		{
			return false;
		}
		
		//direct cooking slots
		if ( ( item.Type() == ATTACHMENT_COOKING_POT ) || ( item.Type() == ATTACHMENT_FRYING_PAN ) || ( item.IsKindOf( "Edible_Base" ) ) )
		{
			return true;
		}
		
		return false;
	}

	// ------------------------------------------------------------
	// EEItemAttached
	// ------------------------------------------------------------
	override void EEItemAttached( EntityAI item, string slot_name ) 
	{
		super.EEItemAttached( item, slot_name );
		
		ItemBase item_base = ItemBase.Cast( item );
		
		//kindling / fuel
		if ( IsFuel( item_base ) )
		{
			//remove from consumables
			RemoveFromFireConsumables( GetFireConsumableByItem( item_base ) );
		}
		
		// direct cooking slots
		switch ( slot_name )
		{
			case "DirectCookingA":
				m_DirectCookingSlots[0] = item_base;
				break;

			case "DirectCookingB":
				m_DirectCookingSlots[1] = item_base;
				break;
		}

		// smoking slots
		switch ( slot_name )
		{
			case "SmokingA":
				m_SmokingSlots[0] = item_base;
				break;

			case "SmokingB":
				m_SmokingSlots[1] = item_base;
				break;

			case "SmokingC":
				m_SmokingSlots[2] = item_base;
				break;
		}
	}

	// ------------------------------------------------------------
	// EEItemDetached
	// ------------------------------------------------------------
	override void EEItemDetached( EntityAI item, string slot_name ) 
	{
		super.EEItemDetached ( item, slot_name );
		
		ItemBase item_base = ItemBase.Cast( item );
		
		//kindling / fuel
		if ( IsFuel( item_base ) )
		{
			//remove from consumables
			RemoveFromFireConsumables( GetFireConsumableByItem( item_base ) );
		}

		// direct cooking slots
		switch ( slot_name )
		{
			case "DirectCookingA":
				m_DirectCookingSlots[0] = NULL;
				break;

			case "DirectCookingB":
				m_DirectCookingSlots[1] = NULL;
				break;
		}

		// smoking slots
		switch ( slot_name )
		{
			case "SmokingA":
				m_SmokingSlots[0] = NULL;
				break;

			case "SmokingB":
				m_SmokingSlots[1] = NULL;
				break;

			case "SmokingC":
				m_SmokingSlots[2] = NULL;
				break;
		}

		// food on direct cooking slots (removal of sound effects)
		if ( item_base.IsKindOf( "Edible_Base" ) )
		{
			Edible_Base food_on_dcs = Edible_Base.Cast( item_base );
			food_on_dcs.MakeSoundsOnClient( false );
		}
		
		// cookware-specifics (remove audio visuals)
		if ( item_base.Type() == ATTACHMENT_COOKING_POT )
		{	
			Bottle_Base cooking_pot = Bottle_Base.Cast( item );
			cooking_pot.RemoveAudioVisualsOnClient();	
		}
		if ( item_base.Type() == ATTACHMENT_FRYING_PAN )
		{	
			FryingPan frying_pan = FryingPan.Cast( item );
			frying_pan.RemoveAudioVisualsOnClient();
		}
	}

	//Do heating
	override protected void Heating()
	{
		float temperature = GetTemperature();
		float temperature_modifier = 0;
		
		//calculate and set temperature
		AddTemperatureToFireplace( PARAM_TEMPERATURE_INCREASE - temperature_modifier );
		
		//damage cargo items
		BurnItemsInFireplace();
		
		//manage cooking equipment (this applies only for case of cooking pot on a tripod)
		if ( m_CookingEquipment )
		{
			float cook_equip_temp = m_CookingEquipment.GetTemperature();
			
			if ( cook_equip_temp >= PARAM_COOKING_TEMP_THRESHOLD )
			{
				//start cooking
				CookWithEquipment();
			}

			//set temperature to cooking equipment
			cook_equip_temp = cook_equip_temp + PARAM_COOKING_EQUIP_TEMP_INCREASE;
			if ( temperature <= PARAM_SMALL_FIRE_TEMPERATURE )
			{
				cook_equip_temp = Math.Clamp ( cook_equip_temp, PARAM_ITEM_HEAT_MIN_TEMP, PARAM_SMALL_FIRE_TEMPERATURE );
			}
			else
			{
				cook_equip_temp = Math.Clamp ( cook_equip_temp, PARAM_ITEM_HEAT_MIN_TEMP, PARAM_COOKING_EQUIP_MAX_TEMP );
			}
			m_CookingEquipment.SetTemperature( cook_equip_temp );
		}

		float cook_item_temp;
		int i;
		// manage cooking on direct cooking slots
		if ( DirectCookingSlotsInUse() )
		{
			for ( i = 0; i < DIRECT_COOKING_SLOT_COUNT; i++ )
			{
				if ( m_DirectCookingSlots[i] )
				{
					cook_item_temp = m_DirectCookingSlots[i].GetTemperature();
					if ( cook_item_temp >= PARAM_COOKING_TEMP_THRESHOLD )
					{
						CookOnDirectSlot( m_DirectCookingSlots[i], cook_item_temp, temperature );
					}
				}
			}
		}

		// manage smoking slots
		if ( SmokingSlotsInUse() )
		{
			for ( i = 0; i < SMOKING_SLOT_COUNT; i++ )
			{
				if ( m_SmokingSlots[i] )
				{
					SmokeOnSmokingSlot( m_SmokingSlots[i], cook_item_temp, temperature );
				}
			}
		}
	}
	
	//================================================================
	// FIRE VICINITY
	//================================================================
	override protected void BurnItemsInFireplace()
	{
		for ( int j = 0; j < GetInventory().AttachmentCount(); ++j )
		{
			ItemBase attachment = ItemBase.Cast( GetInventory().GetAttachmentFromIndex( j ) );
			
			//set damage (fuel items only)
			if ( !IsFuel( attachment ) )
			{
				//add temperature
				AddTemperatureToItemByFire( attachment );
			
				//remove wetness
				AddWetnessToItem( attachment, -PARAM_WET_HEATING_DECREASE_COEF );		
			}
		}
	}

	// ------------------------------------------------------------
	// OnSwitchOn
	// ------------------------------------------------------------
	override void OnSwitchOn()
	{
		super.OnSwitchOn();
		
		StartHeating();
		SoundTurnOn();
	}

	// ------------------------------------------------------------
	// OnSwitchOff
	// ------------------------------------------------------------
	override void OnSwitchOff()
	{
		super.OnSwitchOff();
		
		StopHeating();
		SoundTurnOff();
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
	// CanPutInCargo
	// ------------------------------------------------------------
	override bool CanPutInCargo( EntityAI parent )
	{
		if( !super.CanPutInCargo( parent ) )
		{
			return false;
		}
		
		if( GetInventory().AttachmentCount() == 0 )
		{
			return !IsOpened() && !IsIgnited();
		}

		return false;
	}

	// ------------------------------------------------------------
	// CanPutIntoHands
	// ------------------------------------------------------------
	override bool CanPutIntoHands( EntityAI parent )
	{		
		if( !super.CanPutIntoHands( parent ) )
		{
			return false;
		}
		
		if( GetInventory().AttachmentCount() == 0 )
		{
			return !IsOpened() && !IsIgnited();
		}

		return false;
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
} 