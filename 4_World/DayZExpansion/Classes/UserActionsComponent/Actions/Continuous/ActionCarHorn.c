/**
 * ActionCarHorn.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionActionCarHornCB
 * @brief		
 **/
class ExpansionActionCarHornCB: ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousRepeat( 0 );
	}
}

/**@class		ExpansionActionCarHorn
 * @brief		
 **/
class ExpansionActionCarHorn: ActionContinuousBase
{
	void ExpansionActionCarHorn()
	{
		m_CallbackClass = ExpansionActionCarHornCB;

		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_TOOTHORN;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ALL;
		m_SpecialtyWeight = 0.5;
		m_LockTargetOnUse = false;
	}

	override void CreateConditionComponents()  
	{	
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}

	override typename GetInputType()
	{
		return ExpansionInputCarHorn;
	}
	
	override string GetText()
	{
		return "#STR_USRACT_USE_CARHORN";
	}
	
	override bool CanBeUsedInVehicle()
	{
		return true;
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if (!player) return false;
		
		HumanCommandVehicle vehCommand = player.GetCommand_Vehicle();
		if( vehCommand )
		{
			Transport trans = vehCommand.GetTransport();
			
			if ( trans )
			{
				CarScript car;
				if ( Class.CastTo(car, trans) )
				{
					if ( vehCommand.GetVehicleSeat() != DayZPlayerConstants.VEHICLESEAT_DRIVER )
						return false;
					
					ItemBase battery;
					
					if ( car.IsVitalCarBattery() )
					{
						battery = ItemBase.Cast(car.FindAttachmentBySlotName( "CarBattery" ));
						
						if ( !battery || battery.IsRuined() )
						{
							return false;
						}
					}
					else if ( car.IsVitalTruckBattery() )
					{
						battery = ItemBase.Cast(car.FindAttachmentBySlotName( "TruckBattery" ));
						
						if ( !battery || battery.IsRuined() )
						{
							return false;
						}
					}
					else 
					{
						return false;
					}
					
					return true;
				}
			}
		}
		
		return false;
	}

	override void OnStartServer( ActionData action_data )
	{
		super.OnStartServer( action_data );

		CarScript car;
		HumanCommandVehicle vehCommand = action_data.m_Player.GetCommand_Vehicle();

		if ( Class.CastTo(car, vehCommand.GetTransport()) )
		{
			car.PlayHonk();
		}
	}
	
	override void OnEndServer( ActionData action_data )
	{
		super.OnEndServer( action_data );

		CarScript car;
		HumanCommandVehicle vehCommand = action_data.m_Player.GetCommand_Vehicle();

		if ( Class.CastTo(car, vehCommand.GetTransport()) )
		{
			car.StopHonk();
		}
	}
}