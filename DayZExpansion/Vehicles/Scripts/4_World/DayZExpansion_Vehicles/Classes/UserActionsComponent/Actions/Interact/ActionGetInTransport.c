/**
 * ActionGetInTransport.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionGetInTransport
{
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (player.GetCommand_ExpansionVehicle())
			return false;

		if (!super.ActionCondition(player, target, item))
			return false;

		auto vehicle = ExpansionVehicle.Get(target.GetObject());
		if (!vehicle)
			return false;

		int componentIdx = target.GetComponentIndex();
		int seatIdx = vehicle.CrewPositionIndex(componentIdx);
		if (!vehicle.CanGetInSeat(player, seatIdx))
			return false;

		if (vehicle.IsLocked())
			return false;

		return true;
	}

	override void Start(ActionData action_data)
	{
		auto vehicle = ExpansionVehicle.Get(action_data.m_Target.GetObject());

		super.Start(action_data);

		if (vehicle)
		{
			if (IsMissionClient())
			{
				if (vehicle.IsHelicopter())
					g_Game.GetMission().AddActiveInputExcludes({"expansionhelicopter"});
				else if (vehicle.IsBoat())
					g_Game.GetMission().AddActiveInputExcludes({"expansionboat"});
				else if (vehicle.IsPlane())
					g_Game.GetMission().AddActiveInputExcludes({"expansionplane"});
			}
		}
	}

	override void OnEndServer(ActionData action_data)
	{
		super.OnEndServer(action_data);

		auto vehicle = ExpansionVehicle.Get(action_data.m_Target.GetObject());

		if (!vehicle)
			return;
		
		if (action_data.m_Player && action_data.m_Player.GetIdentity() && GetExpansionSettings().GetLog().VehicleEnter)
		{
			int componentIndex = action_data.m_Target.GetComponentIndex();
			int crew_index = vehicle.CrewPositionIndex(componentIndex);
			string seat;
			if (crew_index == DayZPlayerConstants.VEHICLESEAT_DRIVER)
			{
				if (vehicle.IsHelicopter() || vehicle.IsPlane())
					seat = "pilot";
				else
					seat = "driver";
			}
			else
			{
				seat = "passenger";
			}

			GetExpansionSettings().GetLog().PrintLog("[VehicleEnter] Player \"{1:name}\" (id={1:id}) entered vehicle {2:name} (id={2:persistent_id} pos={2:position}) as " + seat, action_data.m_Player, vehicle.GetEntity());
		}
	}
};