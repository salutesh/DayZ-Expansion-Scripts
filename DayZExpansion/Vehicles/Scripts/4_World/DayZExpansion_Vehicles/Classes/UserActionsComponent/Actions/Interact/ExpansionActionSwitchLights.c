class ExpansionActionSwitchLights: ActionInteractBase
{
	void ExpansionActionSwitchLights()
	{
		m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_HEADLIGHT;
		m_StanceMask        = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_HUDCursorIcon     = CursorIcons.LootCorpse;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}

	override string GetText()
	{
		return "#switch_lights";
	}

	override typename GetInputType()
	{
		return ExpansionToggleLightsActionInput;
	}
	
	override bool HasTarget()
	{
		return false;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		ExpansionHumanCommandVehicle vehCommand = player.GetCommand_ExpansionVehicle();

		if ( vehCommand )
		{
			ExpansionVehicleBase car = vehCommand.GetExpansionVehicle();
			if ( car )
			{
				if ( car.CrewMemberIndex( player ) == DayZPlayerConstants.VEHICLESEAT_DRIVER )
				{
					if ( !car.IsScriptedLightsOn() )
					{
						EntityAI neededItem = null;

						if ( car.IsVitalCarBattery() ) neededItem = car.FindAttachmentBySlotName("CarBattery");
						if ( car.IsVitalTruckBattery() ) neededItem = car.FindAttachmentBySlotName("TruckBattery");
						if ( car.IsVitalHelicopterBattery() ) neededItem = car.FindAttachmentBySlotName("ExpansionHelicopterBattery");
						if ( car.IsVitalAircraftBattery() ) neededItem = car.FindAttachmentBySlotName("ExpansionAircraftBattery");

						if (neededItem && !neededItem.IsRuined())
						{
							return neededItem.GetCompEM() && neededItem.GetCompEM().GetEnergy() > 0;
						}
					}
					else
					{
						return true;
					}
				}
			}
		}
		return false;
	}

	override void OnExecuteServer( ActionData action_data )
	{
		ExpansionHumanCommandVehicle vehCommand = action_data.m_Player.GetCommand_ExpansionVehicle();
		if ( vehCommand )
		{
			ExpansionVehicleBase trans = vehCommand.GetExpansionVehicle();
			if ( trans )
			{
				trans.ToggleHeadlights();
			}
		}
	}
	
	override bool CanBeUsedInVehicle()
	{
		return true;
	}
};