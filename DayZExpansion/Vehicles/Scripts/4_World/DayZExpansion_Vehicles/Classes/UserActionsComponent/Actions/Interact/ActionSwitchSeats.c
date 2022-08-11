/**
 * ActionSwitchSeats.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionSwitchSeats
{
	override void Start(ActionData action_data)
	{
		super.Start(action_data);

		if (action_data.m_Player && action_data.m_Player.GetIdentity() && GetExpansionSettings().GetLog().VehicleEnter)
		{
			CarScript car;
			if (Class.CastTo(car, action_data.m_Target.GetObject()))
			{
				int componentIndex = action_data.m_Target.GetComponentIndex();
				int crew_index = car.CrewPositionIndex(componentIndex);
				string seat;
				if (crew_index == DayZPlayerConstants.VEHICLESEAT_DRIVER)
				{
					if (car.IsHelicopter() || car.IsPlane())
						seat = "pilot";
					else
						seat = "driver";
				}
				else
				{
					seat = "passenger";
				}

				GetExpansionSettings().GetLog().PrintLog("[VehicleEnter] Player " + action_data.m_Player.GetIdentity().GetName() + " [uid=" + action_data.m_Player.GetIdentity().GetId() + "] switched seats in vehicle " + car.GetDisplayName() + " (pos=" + car.GetPosition() + ", type=" + car.GetType() + ") to " + seat);
			}
		}
	}
}
