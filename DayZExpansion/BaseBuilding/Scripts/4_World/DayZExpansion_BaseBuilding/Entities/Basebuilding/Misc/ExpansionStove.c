/**
 * ExpansionStove.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionStove
 * @brief		
 **/
class ExpansionStove: PortableGasStove 
{
	const int   DIRECT_COOKING_SLOT_COUNT			= 2;
	const int   SMOKING_SLOT_COUNT					= 4;

	ItemBase m_DirectCookingSlots[DIRECT_COOKING_SLOT_COUNT];
	ItemBase m_SmokingSlots[SMOKING_SLOT_COUNT];

	override void SetActions()
	{
		super.SetActions();

		AddAction(ActionTogglePlaceObject);
		AddAction(ActionPlaceObject);
	}

	void Expansion_AddTemperatureToItemByEnergy(ItemBase item, float consumed_energy)
	{
		if (item.GetTemperatureMax() >= FireplaceBase.PARAM_ITEM_HEAT_MIN_TEMP)
		{
			float temperature = item.GetTemperature() + (FireplaceBase.PARAM_ITEM_HEAT_TEMP_INCREASE_COEF * consumed_energy);
			temperature = Math.Clamp(temperature, FireplaceBase.PARAM_ITEM_HEAT_MIN_TEMP, FireplaceBase.PARAM_MAX_ITEM_HEAT_TEMP_INCREASE);
			item.SetTemperature(temperature);
		}
	}

	protected void CookOnDirectSlot(ItemBase slot_item, float temp_equip, float temp_ext)
	{
		if (m_CookingProcess == null)
		{
			m_CookingProcess = new Cooking();
		}
		
		m_CookingProcess.CookWithEquipment(slot_item);
	}
	
	protected void SmokeOnSmokingSlot( ItemBase slot_item, float temp_equip, float temp_ext )
	{
		if ( m_CookingProcess == NULL )
		{
			m_CookingProcess = new Cooking();
		}
		
		// smoking slots accept only individual meat/fruit/veg items
		Edible_Base ingr = Edible_Base.Cast( slot_item );
		if ( ingr )
		{
			m_CookingProcess.SmokeItem( ingr, FireplaceBase.TIMER_HEATING_UPDATE_INTERVAL * FireplaceBase.SMOKING_SPEED );
		}
	}

	override protected void RefreshFlameVisual(bool working = false, bool hasAttachment = false)
	{
	}

	override bool ExpansionCanOpen( PlayerBase player, string selection )
	{			
		if (selection == "doors1" && GetAnimationPhase("doors1") < 1.0 && ExpansionIsOpenable())
			return true;
		
		return false;
	}

	override bool ExpansionCanClose( PlayerBase player, string selection )
	{
		return CanClose( selection );
	}

	override bool CanClose( string selection )
	{
		if (selection == "doors1" && GetAnimationPhase("doors1") > 0.0 && ExpansionIsOpenable())
			return true;
		
		return false;
	}

	override void Open( string selection ) 
	{
		//! Door open animation
		if (selection == "doors1")
			SetAnimationPhase( "doors1", 1 );
		
		super.Open( selection );
	}
	
	override void Close( string selection ) 
	{
		//! Door open animation
		if (selection == "doors1")
			SetAnimationPhase( "doors1", 0 );
		
		super.Close( selection );
	}

	override void EEItemAttached(EntityAI item, string slot_name)
	{
		super.EEItemAttached(item, slot_name);
		
		ItemBase item_base = ItemBase.Cast(item);

		// direct cooking/smoking slots
		bool edible_base_attached = false;
		switch (slot_name)
		{
			case "DirectCookingA":
				m_DirectCookingSlots[0] = item_base;
				edible_base_attached = true;
				break;
			case "DirectCookingB":
				m_DirectCookingSlots[1] = item_base;
				edible_base_attached = true;
				break;

			case "SmokingA":
				m_SmokingSlots[0] = item_base;
				edible_base_attached = true;
				break;
			case "SmokingB":
				m_SmokingSlots[1] = item_base;
				edible_base_attached = true;
				break;
			case "SmokingC":
				m_SmokingSlots[2] = item_base;
				edible_base_attached = true;
				break;
			case "SmokingD":
				m_SmokingSlots[3] = item_base;
				edible_base_attached = true;
				break;
		}
		
		// reset cooking time (to prevent the cooking exploit)
		if (GetGame().IsServer() && edible_base_attached)
		{
			Edible_Base edBase = Edible_Base.Cast(item_base);
			if (edBase)
			{
				if (edBase.GetFoodStage())
				{
					edBase.SetCookingTime(0);
				}
			}
		}
	}

	override void EEItemDetached(EntityAI item, string slot_name)
	{
		super.EEItemDetached(item, slot_name);

		// direct cooking/smoking slots
		switch (slot_name)
		{
			case "DirectCookingA":
				m_DirectCookingSlots[0] = null;
				break;
			case "DirectCookingB":
				m_DirectCookingSlots[1] = null;
				break;

			case "SmokingA":
				m_SmokingSlots[0] = null;
				break;
			case "SmokingB":
				m_SmokingSlots[1] = null;
				break;
			case "SmokingC":
				m_SmokingSlots[2] = null;
				break;
			case "SmokingD":
				m_SmokingSlots[3] = null;
				break;
		}
	}
	
	override void OnWorkStart()
	{
		//sound (client only)
		SoundBurningStart();
	}

	override void OnWorkStop()
	{
		//stop steam particle
		RemoveCookingAudioVisuals();		
		//sound (client only)
		SoundBurningStop();
	}
	
	override void OnWork(float consumed_energy)
	{
		//manage cooking equipment
		super.OnWork(consumed_energy);

		if (!GetGame().IsServer())
			return;

		float temperature = GetTemperature();

		float cook_item_temp;
		int i;
		// manage cooking on direct cooking slots
		for ( i = 0; i < DIRECT_COOKING_SLOT_COUNT; i++ )
		{
			if ( m_DirectCookingSlots[i] )
			{
				CookOnDirectSlot( m_DirectCookingSlots[i], m_DirectCookingSlots[i].GetTemperature(), temperature );
				Expansion_AddTemperatureToItemByEnergy(m_DirectCookingSlots[i], consumed_energy);
			}
		}

		// manage smoking slots
		for ( i = 0; i < SMOKING_SLOT_COUNT; i++ )
		{
			if ( m_SmokingSlots[i] )
			{
				SmokeOnSmokingSlot( m_SmokingSlots[i], m_SmokingSlots[i].GetTemperature(), temperature );
				Expansion_AddTemperatureToItemByEnergy(m_SmokingSlots[i], consumed_energy);
			}
		}
	}

	override bool IsHeavyBehaviour() 
	{
		return true;
	}
	
	override bool CanPutInCargo( EntityAI parent )
	{
		if( !super.CanPutInCargo( parent ) )
		{
			return false;
		}
		
		if( GetInventory().AttachmentCount() == 0 )
		{
			return !ExpansionIsOpened() && !IsIgnited();
		}

		return false;
	}

	override bool CanPutIntoHands( EntityAI parent )
	{		
		if( !super.CanPutIntoHands( parent ) )
		{
			return false;
		}
		
		if( GetInventory().AttachmentCount() == 0 )
		{
			return !ExpansionIsOpened() && !IsIgnited();
		}

		return false;
	}

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
