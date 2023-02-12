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

class ExpansionActionGetOutTransportReciveData : ActionReciveData
{
	bool m_KeepInVehicleSpaceAfterLeave;
};

modded class ActionGetOutTransport
{
	override void WriteToContext(ParamsWriteContext ctx, ActionData action_data)
	{
		super.WriteToContext(ctx, action_data);

		GetOutTransportActionData poActionData;
		poActionData = GetOutTransportActionData.Cast(action_data);

		ctx.Write(poActionData.m_KeepInVehicleSpaceAfterLeave);
	}

	override bool ReadFromContext(ParamsReadContext ctx, out ActionReciveData action_recive_data)
	{
		if (!action_recive_data)
		{
			action_recive_data = new ExpansionActionGetOutTransportReciveData;
		}

		super.ReadFromContext(ctx, action_recive_data);

		ExpansionActionGetOutTransportReciveData action_data_po = ExpansionActionGetOutTransportReciveData.Cast(action_recive_data);

		if (!ctx.Read(action_data_po.m_KeepInVehicleSpaceAfterLeave))
			return false;

		return true;
	}

	override void HandleReciveData(ActionReciveData action_recive_data, ActionData action_data)
	{
		super.HandleReciveData(action_recive_data, action_data);

		ExpansionActionGetOutTransportReciveData recive_data_po = ExpansionActionGetOutTransportReciveData.Cast(action_recive_data);
		GetOutTransportActionData action_data_po = GetOutTransportActionData.Cast(action_data);

		action_data_po.m_KeepInVehicleSpaceAfterLeave = recive_data_po.m_KeepInVehicleSpaceAfterLeave;
	}

#ifndef DAYZ_1_19
	//! 1.20+
	override void OnStart(ActionData action_data)
	{
		bool keepInVehicleSpaceAfterLeave;
		HumanCommandVehicle vehCommand = action_data.m_Player.GetCommand_Vehicle();
		if (vehCommand)
		{
			CarScript car;
			if (Class.CastTo(car, vehCommand.GetTransport()))
			{
				keepInVehicleSpaceAfterLeave = car.Expansion_CanObjectAttach(action_data.m_Player) && car.LeavingSeatDoesAttachment(vehCommand.GetVehicleSeat());
			}
		}

		if (keepInVehicleSpaceAfterLeave)
			Expansion_PerformGetOutTransport(action_data);
		else
			super.OnStart(action_data);
#else
	//! 1.19

	//! Can't call super in this function. Will cause crashes.
	override void Start(ActionData action_data)
	{
		//! Vanilla ActionBase::Start BEGIN
		action_data.m_State = UA_START;
		
		if ( LogManager.IsActionLogEnable() )
		{
			Debug.ActionLog("Time stamp: " + action_data.m_Player.GetSimulationTimeStamp(), this.ToString() , "n/a", "OnStart", action_data.m_Player.ToString() );
		}

		OnStart(action_data);

		if (GetGame().IsServer())
		{
			OnStartServer(action_data);
		}
		else
		{
			OnStartClient(action_data);
		}

		InformPlayers(action_data.m_Player, action_data.m_Target, UA_START);
		//! Vanilla ActionBase::Start END

		Expansion_PerformGetOutTransport(action_data);
#endif

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

	void Expansion_PerformGetOutTransport(ActionData action_data)
	{
		HumanCommandVehicle vehCommand = action_data.m_Player.GetCommand_Vehicle();
		if (vehCommand)
		{
			CarScript car;
			if (Class.CastTo(car, vehCommand.GetTransport()))
			{
				float speed = Math.AbsFloat(car.GetSpeedometer());
				bool keepInVehicleSpaceAfterLeave = car.Expansion_CanObjectAttach(action_data.m_Player) && car.LeavingSeatDoesAttachment(vehCommand.GetVehicleSeat());

				auto got_action_data = GetOutTransportActionData.Cast(action_data);
#ifndef DAYZ_1_19
				//! 1.20+
				ProcessGetOutActionData(car, got_action_data);
#else
				//! 1.19
				got_action_data.m_StartLocation = got_action_data.m_Player.GetPosition();
				got_action_data.m_Car = car;
				got_action_data.m_CarSpeed = speed;
				got_action_data.m_DmgTaken = (got_action_data.m_CarSpeed * got_action_data.m_CarSpeed) / m_DmgFactor; //When using multiplications, wrong value is returned
				got_action_data.m_ShockTaken = (got_action_data.m_CarSpeed * got_action_data.m_CarSpeed) / m_ShockFactor;
				got_action_data.m_WasJumpingOut = speed > 8.0;
#endif

				if (IsMissionClient())
					got_action_data.m_KeepInVehicleSpaceAfterLeave = keepInVehicleSpaceAfterLeave;

				vehCommand.KeepInVehicleSpaceAfterLeave(got_action_data.m_KeepInVehicleSpaceAfterLeave);

				if (got_action_data.m_KeepInVehicleSpaceAfterLeave)
				{
					action_data.m_Player.Expansion_GettingOutVehicle();
					vehCommand.GetOutVehicle();
				}
				else if (!got_action_data.m_WasJumpingOut)
				{
					vehCommand.GetOutVehicle();
				}
				else
				{
					vehCommand.JumpOutVehicle();
				}

				GetDayZGame().GetBacklit().OnLeaveCar();
				if (action_data.m_Player.GetInventory())
					action_data.m_Player.GetInventory().LockInventory(LOCK_FROM_SCRIPT);
			}
		}
	}

	override void OnEnd(ActionData action_data)
	{
		auto got_action_data = GetOutTransportActionData.Cast(action_data);
		CarScript car = CarScript.Cast(got_action_data.m_Car);

		if (car && !car.IsCar() && !car.IsDuck())
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(action_data.m_Player.GetInventory().UnlockInventory, 1500, false, LOCK_FROM_SCRIPT); //! Unlock after delay to fix hand desync bug
		else
			super.OnEnd(action_data);
	}

	//! Equivalent to vanilla ActionGetOutTransport::OnEndServer amended with player attachment and was in vehicle handling as well as proper raycast
	override void OnEndServer(ActionData action_data)
	{
		GetOutTransportActionData got_action_data = GetOutTransportActionData.Cast(action_data);

		CarScript car = CarScript.Cast(got_action_data.m_Car);

		vector playerPos = action_data.m_Player.GetPosition();

		if (car && car.IsBoat())
		{
			//! Prevent player glitching below boat
			float vehicleY = car.GetPosition()[1] - car.GetModelZeroPointDistanceFromGround();
			if (playerPos[1] < vehicleY + 1.0)
			{
				playerPos[1] = vehicleY + 1.0;
				action_data.m_Player.SetPosition(playerPos);
			}
		}

		if (got_action_data.m_KeepInVehicleSpaceAfterLeave)
			return;

		action_data.m_Player.Expansion_SetIsInVehicleSeatOrAttached(false);
		if (car && car.IsHelicopter())
			car.SetHasPilot(false);  //! So we are able to detect if pilot got disconnected or got out on own accord

		if (got_action_data.m_WasJumpingOut)
		{
			got_action_data.m_Player.OnJumpOutVehicleFinish(got_action_data.m_CarSpeed);

			//! Additional jump out damage should only apply for land vehicles (cars), not helis, planes or boats
			if (car && car.IsCar())
			{
				PlayerBase player = got_action_data.m_Player;

				ApplyJumpOutDmg(action_data);

				vector posMS = player.WorldToModel(player.GetPosition());
				player.DamageAllLegs(got_action_data.m_DmgTaken); //! Additional leg specific damage dealing
				player.ProcessDirectDamage(DT_CUSTOM, player, "", "FallDamage", posMS, got_action_data.m_DmgTaken);
			}
		}

		if (car)
			car.ForceUpdateLightsEnd();
	}
};