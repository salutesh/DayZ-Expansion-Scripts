/**
 * ActionGetOutTransport.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class GetOutTransportActionData
{
	int m_Expansion_SeatIdx;
}

modded class ActionGetOutTransport
{
	//! 1.26+
	override void ProcessGetOutTransportActionData(Transport veh, GetOutTransportActionData got_action_data)
	{
		super.ProcessGetOutTransportActionData(veh, got_action_data);

		auto vehicle = ExpansionVehicle.Get(veh);

		HumanCommandVehicle vehCommand = got_action_data.m_Player.GetCommand_Vehicle();

		if (vehicle)
		{
			if ( vehicle.CanObjectAttach(got_action_data.m_Player) && vehicle.LeavingSeatDoesAttachment(vehCommand.GetVehicleSeat()) )
			{
				got_action_data.m_WasJumpingOut = false;
				got_action_data.m_WasJumpingOutAnim = false;
			}

			got_action_data.m_Expansion_SeatIdx = vehicle.CrewMemberIndex(got_action_data.m_Player);

			// Should prevent a few issues related to towing and server crashes
			if (got_action_data.m_Expansion_SeatIdx == DayZPlayerConstants.VEHICLESEAT_DRIVER && vehicle.IsTowing())
			{
				vehicle.DestroyTow();
			}
		}
	}

	override void OnStart(ActionData action_data)
	{
		auto vehicle = ExpansionVehicle.Get(action_data.m_Target.GetObject());

		super.OnStart(action_data);

		if (vehicle)
		{
			if (IsMissionClient())
			{
				if (vehicle.IsHelicopter())
					g_Game.GetMission().RemoveActiveInputExcludes({"expansionhelicopter"});
				else if (vehicle.IsBoat())
					g_Game.GetMission().RemoveActiveInputExcludes({"expansionboat"});
				else if (vehicle.IsPlane())
					g_Game.GetMission().RemoveActiveInputExcludes({"expansionplane"});
			}
		}
	}

	[Obsolete("no replacement")]
	void Expansion_OnPerformGetInTransport(CarScript car)
	{
	}

	override void OnStartServer(ActionData action_data)
	{
		GetOutTransportActionData got_action_data = GetOutTransportActionData.Cast(action_data);

		auto vehicle = ExpansionVehicle.Get(got_action_data.m_Vehicle);

		if (vehicle)
		{
			if (vehicle.IsHelicopter())
			{
				if (!vehicle.IsAutoHover())
					vehicle.EngineStop();  //! If not in auto-hover, getting out stops engine
			}
		}

		super.OnStartServer(action_data);
	}

	override void OnEnd(ActionData action_data)
	{
		auto got_action_data = GetOutTransportActionData.Cast(action_data);

		auto vehicle = ExpansionVehicle.Get(got_action_data.m_Vehicle);

		if (vehicle && !vehicle.IsCar() && !vehicle.IsDuck())
			g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(action_data.m_Player.GetInventory().UnlockInventory, 1500, false, LOCK_FROM_SCRIPT); //! Unlock after delay to fix hand desync bug
		else
			super.OnEnd(action_data);
	}

	override void OnEndServer(ActionData action_data)
	{
		GetOutTransportActionData got_action_data = GetOutTransportActionData.Cast(action_data);

		//! 1.26+
		auto vehicle = ExpansionVehicle.Get(got_action_data.m_Vehicle);

		super.OnEndServer(action_data);

		if (vehicle)
			vehicle.OnGotOut(action_data.m_Player, got_action_data.m_Expansion_SeatIdx);

		if (vehicle && action_data.m_Player && action_data.m_Player.GetIdentity() && GetExpansionSettings().GetLog().VehicleLeave)
		{
			GetExpansionSettings().GetLog().PrintLog("[VehicleLeave] Player \"{1:name}\" (id={1:id}) left vehicle {2:name} (id={2:persistent_id} pos={2:position})", action_data.m_Player, vehicle.GetEntity());
		}
	}
};