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
	bool m_KeepInVehicleSpaceAfterLeave = false;
};

modded class ActionGetOutTransport
{
#ifndef DAYZ_1_19
	//! 1.20
	override void ProcessGetOutActionData(Car car, GetOutTransportActionData got_action_data)
	{
		super.ProcessGetOutActionData(car, got_action_data);

		HumanCommandVehicle vehCommand = got_action_data.m_Player.GetCommand_Vehicle();

		CarScript cs;
		if (Class.CastTo(cs, car) && cs.Expansion_CanObjectAttach(got_action_data.m_Player) && cs.LeavingSeatDoesAttachment(vehCommand.GetVehicleSeat()))
		{
			got_action_data.m_WasJumpingOut = false;
			got_action_data.m_KeepInVehicleSpaceAfterLeave = true;
		}
	}

	override void OnStart(ActionData action_data)
	{
		super.OnStart(action_data);

		if (IsMissionClient())
		{
			GetUApi().GetInputByName("UACarLeft").ForceDisable(false);
			GetUApi().GetInputByName("UACarRight").ForceDisable(false);
			GetUApi().GetInputByName("UACarForward").ForceDisable(false);
			GetUApi().GetInputByName("UACarBack").ForceDisable(false);

			GetUApi().GetInputByName("UACarShiftGearUp").ForceDisable(false);
			GetUApi().GetInputByName("UACarShiftGearDown").ForceDisable(false);
		}
	}
#endif

	override void OnEnd(ActionData action_data)
	{
		auto got_action_data = GetOutTransportActionData.Cast(action_data);

		CarScript cs;
		if (Class.CastTo(cs, got_action_data.m_Car) && !cs.IsCar() && !cs.IsDuck())
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(action_data.m_Player.GetInventory().UnlockInventory, 1500, false, LOCK_FROM_SCRIPT); //! Unlock after delay to fix hand desync bug
		else
			super.OnEnd(action_data);
	}

	override void OnEndServer(ActionData action_data)
	{
		GetOutTransportActionData got_action_data = GetOutTransportActionData.Cast(action_data);

#ifdef DAYZ_1_19
		if (got_action_data.m_KeepInVehicleSpaceAfterLeave)
			got_action_data.m_WasJumpingOut = false;
#endif

		CarScript cs;

		if (Class.CastTo(cs, got_action_data.m_Car))
		{
			if (!got_action_data.m_WasJumpingOut)
			{
				vector playerPos = action_data.m_Player.GetPosition();

				if (cs.Expansion_IsBoat() && !cs.Expansion_IsCar())
				{
					//! Prevent player glitching below boat

					float vehicleY = cs.GetPosition()[1] - cs.GetModelZeroPointDistanceFromGround();
					if (playerPos[1] < vehicleY + 1.0)
					{
						playerPos[1] = vehicleY + 1.0;
						action_data.m_Player.SetPosition(playerPos);
					}
				}
				else if (cs.Expansion_IsHelicopter() && !got_action_data.m_KeepInVehicleSpaceAfterLeave)
				{
					//! Prevent player glitching below surface

					vector surfacePos = ExpansionStatic.GetSurfacePosition(playerPos);

					PhxInteractionLayers layerMask;
					layerMask |= PhxInteractionLayers.BUILDING;
					layerMask |= PhxInteractionLayers.VEHICLE;
					layerMask |= PhxInteractionLayers.ITEM_LARGE;
					layerMask |= PhxInteractionLayers.ROADWAY;
					layerMask |= PhxInteractionLayers.TERRAIN;
					layerMask |= PhxInteractionLayers.WATERLAYER;

					vector hitPosition;

					if (!DayZPhysics.RayCastBullet(playerPos + "0 1.5 0", surfacePos, layerMask, action_data.m_Player, NULL, hitPosition, NULL, NULL))
						hitPosition = surfacePos;

					if (Math.AbsFloat(playerPos[1] - hitPosition[1]) <= 1.5)
						action_data.m_Player.SetPosition(hitPosition);
				}
			}

			if (cs.Expansion_IsHelicopter())
				cs.SetHasPilot(false);  //! So we are able to detect if pilot got disconnected or got out on own accord
		}

		super.OnEndServer(action_data);

		if (action_data.m_Player && action_data.m_Player.GetIdentity() && GetExpansionSettings().GetLog().VehicleLeave)
		{
			GetExpansionSettings().GetLog().PrintLog("[VehicleLeave] Player " + action_data.m_Player.GetIdentity().GetName() + " [uid=" + action_data.m_Player.GetIdentity().GetId() + "] left vehicle " + got_action_data.m_Car.GetDisplayName() + " (id=" + ExpansionStatic.GetPersistentIDString(got_action_data.m_Car) + " pos=" + got_action_data.m_Car.GetPosition() + ")");
		}
	}
};