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
	// ------------------------------------------------------------
	void AttachmentDebugPrint(Man player, string message)
	{
		// Done so EXPrint can be disabled and this can still print
		//#ifdef EXPANSION_PLAYER_ATTACHMENT_LOG
		//Print( "[ATTACHMENT] " + Expansion_Debug_Player( player ) + message );
		//#endif
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (player.GetCommand_ExpansionVehicle())
			return false;

		if (!super.ActionCondition(player, target, item))
			return false;

		CarScript car = CarScript.Cast(target.GetObject());

		// Temp fix for being able to enter Uh1h despite being locked
		ExpansionUh1h uh1h = ExpansionUh1h.Cast(target.GetObject());
		if (car.DoorCount() <= 0 || uh1h)
			return !car.IsLocked();

		return true;
	}

	override void Start(ActionData action_data)
	{
		//AttachmentDebugPrint(action_data.m_Player, "+ActionGetInTransport::Start");

		CarScript car = CarScript.Cast(action_data.m_Target.GetObject());
		//AttachmentDebugPrint(action_data.m_Player, "car=" + car);

		if (action_data.m_Player.Expansion_IsAttached() && action_data.m_Player.GetParent())
		{
			//AttachmentDebugPrint(action_data.m_Player, "parent=" + action_data.m_Player.GetParent());

			//AttachmentDebugPrint(action_data.m_Player, "Attaching Enabled");
			
			action_data.m_Player.Expansion_PrepareGettingInVehicle();

			//AttachmentDebugPrint(action_data.m_Player, "-ActionGetInTransport::Start");
			return;
		}

		//AttachmentDebugPrint(action_data.m_Player, "Has No Parent");
		super.Start(action_data);

#ifdef DAYZ_1_19
		//! Disable collision while getting in to prevent impulse being applied to vehicle
		//! https://feedback.bistudio.com/T168870
		dBodySetInteractionLayer(action_data.m_Player, PhxInteractionLayers.RAGDOLL);
#endif

		Expansion_OnPerformGetInTransport(car);
	}

	override void OnUpdate(ActionData action_data)
	{
		if (action_data.m_State == UA_START)
		{
			if (action_data.m_Player.Expansion_CanPerformVehicleGetIn())
			{
				//AttachmentDebugPrint(action_data.m_Player, "Expansion_CanPerformVehicleGetIn");
				if (GetGame().IsClient() && !ScriptInputUserData.CanStoreInputUserData())
				{
					//AttachmentDebugPrint(action_data.m_Player, "CanStoreInputUserData Fail");
					return;
				}

				action_data.m_Player.Expansion_EndVehiclePrep();

				//AttachmentDebugPrint(action_data.m_Player, "parent=" + action_data.m_Player.GetParent());

				if (action_data.m_Player.GetParent())
				{
					return;
				}

				Expansion_PerformGetInTransport(action_data);

				return;
			}

			if (action_data.m_Player.Expansion_IsPreparingVehicle())
			{
				//AttachmentDebugPrint(action_data.m_Player, "Expansion_IsPreparingVehicle");
				return;
			}
		}

		super.OnUpdate(action_data);
	}

	private void Expansion_PerformGetInTransport(ActionData action_data)
	{
		//AttachmentDebugPrint(action_data.m_Player, "+ActionGetInTransport::Expansion_PerformGetInTransport");
		vector playerPosition = action_data.m_Player.GetPosition();
		//AttachmentDebugPrint(action_data.m_Player, "playerPosition=" + playerPosition);

		CarScript car = CarScript.Cast(action_data.m_Target.GetObject());
		//AttachmentDebugPrint(action_data.m_Player, "car=" + car);
		if (!car)
		{
			//AttachmentDebugPrint(action_data.m_Player, "-ActionGetInTransport::Expansion_PerformGetInTransport");
			return;
		}
		int componentIndex = action_data.m_Target.GetComponentIndex();
		int crew_index = car.CrewPositionIndex(componentIndex);
		int seat = car.GetSeatAnimationType(crew_index);

		//AttachmentDebugPrint(action_data.m_Player, "componentIndex=" + componentIndex + " crew_index=" + crew_index + " seat=" + seat);

		HumanCommandVehicle vehCommand = action_data.m_Player.StartCommand_Vehicle(car, crew_index, seat, false);

#ifdef DAYZ_1_19
		//! Disable collision while getting in to prevent impulse being applied to vehicle
		//! https://feedback.bistudio.com/T168870
		dBodySetInteractionLayer(action_data.m_Player, PhxInteractionLayers.RAGDOLL);
#endif

		//AttachmentDebugPrint(action_data.m_Player, "vehCommand=" + vehCommand);
		if (vehCommand)
		{
			//AttachmentDebugPrint(action_data.m_Player, "vehCommand parent=" + action_data.m_Player.GetParent());

			vehCommand.SetVehicleType(car.GetAnimInstance());

			GetDayZGame().GetBacklit().OnEnterCar();
			if (action_data.m_Player.GetInventory())
				action_data.m_Player.GetInventory().LockInventory(LOCK_FROM_SCRIPT);

			Expansion_OnPerformGetInTransport(car);
		}
		else
		{
			// TODO: don't go back to 0 0 0

			//AttachmentDebugPrint(action_data.m_Player, "no vehCommand parent=" + action_data.m_Player.GetParent());
			if (action_data.m_Player.GetParent())
			{
				EXTrace.Print(EXTrace.VEHICLES, this, "::Expansion_PerformGetInTransport - detaching from " + action_data.m_Player.GetParent());
				action_data.m_Player.UnlinkFromLocalSpace();
			}

			action_data.m_Player.SetPosition(playerPosition);
		}
	}

	void Expansion_OnPerformGetInTransport(CarScript car)
	{
		if (car)
		{
			if (IsMissionClient())
			{
				bool isCar = car.IsCar();
				bool isBoat = car.IsBoat();
				bool isPlane = car.IsPlane();

				GetUApi().GetInputByName("UACarLeft").ForceDisable(!isCar);
				GetUApi().GetInputByName("UACarRight").ForceDisable(!isCar);

				if (isPlane)
				{
					GetUApi().GetInputByName("UACarShiftGearUp").ForceDisable(isPlane);
				}
				else
				{
					GetUApi().GetInputByName("UACarForward").ForceDisable(!isCar);
				}

				if (isBoat)
				{
					GetUApi().GetInputByName("UACarBack").ForceDisable(true);
					GetUApi().GetInputByName("UACarShiftGearUp").ForceDisable(false);
					GetUApi().GetInputByName("UACarShiftGearDown").ForceDisable(false);
				}
				else
				{
					GetUApi().GetInputByName("UACarShiftGearUp").ForceDisable(!isCar);
					GetUApi().GetInputByName("UACarShiftGearDown").ForceDisable(!isCar);
				}
			}
		}
	}

	override void OnEndServer(ActionData action_data)
	{
		CarScript car = CarScript.Cast(action_data.m_Target.GetObject());

		super.OnEndServer(action_data);

		action_data.m_Player.Expansion_SetIsInVehicleSeatOrAttached(true);
		if (!car)
			return;

		if (car.IsHelicopter())
			car.SetHasPilot(car.CrewMember(DayZPlayerConstants.VEHICLESEAT_DRIVER) != NULL);  //! So we are able to detect if pilot got disconnected or got out on own accord
		
		if (action_data.m_Player && action_data.m_Player.GetIdentity() && GetExpansionSettings().GetLog().VehicleEnter)
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

			GetExpansionSettings().GetLog().PrintLog("[VehicleEnter] Player " + action_data.m_Player.GetIdentity().GetName() + " [uid=" + action_data.m_Player.GetIdentity().GetId() + "] entered vehicle " + car.GetDisplayName() + " (id=" + car.GetVehiclePersistentIDString() + " pos=" + car.GetPosition() + ") as " + seat);
		}
	}
};