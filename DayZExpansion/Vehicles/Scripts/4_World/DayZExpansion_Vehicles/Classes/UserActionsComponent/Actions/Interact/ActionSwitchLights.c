/*
 * ActionSwitchLights.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionSwitchLights
{
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		HumanCommandVehicle vehCommand = player.GetCommand_Vehicle();

		if ( vehCommand )
		{
			Transport trans = vehCommand.GetTransport();
			if ( trans )
			{
				CarScript car;
				if ( Class.CastTo(car, trans) )
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

							if ( !neededItem || (neededItem && neededItem.IsRuined()) )
								return false;
							
							EntityAI item1 = car.FindAttachmentBySlotName("Reflector_1_1");
							EntityAI item2 = car.FindAttachmentBySlotName("Reflector_2_1");
			
							if ( !item1 && !item2 )
								return false;
							else if ( item1 && item1.IsRuined() && item2 && item2.IsRuined() )
								return false;

							return true;
						} else
						{
							return true;
						}
					}
				}
			}
		}
		return false;
	}
};