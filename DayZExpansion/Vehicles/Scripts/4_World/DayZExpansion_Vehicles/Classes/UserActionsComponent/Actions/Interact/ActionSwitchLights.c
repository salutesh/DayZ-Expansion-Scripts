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

				#ifdef DAYZ_1_29
					EntityAI vehicleEntity = vehicle.GetEntity();
					if ( vehicle.IsVitalCarBattery() ) neededItem = vehicleEntity.FindAttachmentBySlotName("CarBattery");
					if ( vehicle.IsVitalTruckBattery() ) neededItem = vehicleEntity.FindAttachmentBySlotName("TruckBattery");
					if ( vehicle.IsVitalHelicopterBattery() ) neededItem = vehicleEntity.FindAttachmentBySlotName("ExpansionHelicopterBattery");
					if ( vehicle.IsVitalAircraftBattery() ) neededItem = vehicleEntity.FindAttachmentBySlotName("ExpansionAircraftBattery");
				#else
					//! 1.30+
					Transport transport = vehicle.GetTransport();
					if (transport.NeedElectricitySourceDevice())
						neededItem = transport.GetElectricitySourceDevice();
					else
						return true;
				#endif

					if (neededItem && !neededItem.IsRuined())
						return neededItem.GetCompEM() && neededItem.GetCompEM().GetEnergy() > 0;
				}
				else
					return true;
			}
		}

		return false;
	}
};