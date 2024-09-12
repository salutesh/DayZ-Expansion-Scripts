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
		auto vehicle = ExpansionVehicle.Get(player);

		if ( vehicle )
		{
			if ( vehicle.CrewMemberIndex( player ) == DayZPlayerConstants.VEHICLESEAT_DRIVER )
			{
				if ( !vehicle.IsScriptedLightsOn() )
				{
					EntityAI neededItem = null;
					EntityAI vehicleEntity = vehicle.GetEntity();

					if ( vehicle.IsVitalCarBattery() ) neededItem = vehicleEntity.FindAttachmentBySlotName("CarBattery");
					if ( vehicle.IsVitalTruckBattery() ) neededItem = vehicleEntity.FindAttachmentBySlotName("TruckBattery");
					if ( vehicle.IsVitalHelicopterBattery() ) neededItem = vehicleEntity.FindAttachmentBySlotName("ExpansionHelicopterBattery");
					if ( vehicle.IsVitalAircraftBattery() ) neededItem = vehicleEntity.FindAttachmentBySlotName("ExpansionAircraftBattery");

					if ( !neededItem || neededItem.IsRuined() || (neededItem.GetCompEM() && neededItem.GetCompEM().GetEnergy() <= 0) )
						return false;
					
					EntityAI item1 = vehicleEntity.FindAttachmentBySlotName("Reflector_1_1");
					EntityAI item2 = vehicleEntity.FindAttachmentBySlotName("Reflector_2_1");
	
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
		return false;
	}
};