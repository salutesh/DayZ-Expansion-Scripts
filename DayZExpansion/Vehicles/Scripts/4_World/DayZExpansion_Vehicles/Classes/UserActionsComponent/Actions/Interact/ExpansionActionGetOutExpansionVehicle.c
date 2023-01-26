/**
 * ExpansionActionGetOutExpansionVehicle.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionGetOutExpansionVehicleActionData : ActionData
{
	ExpansionVehicleBase m_Car;
	vector m_StartLocation;
	float m_CarSpeed;
	bool m_WasJumpingOut = false;
	bool m_KeepInVehicleSpaceAfterLeave = false;
	float m_DmgTaken = 0;	// Damage taken by the player when jumping out of vehicle
	float m_ShockTaken = 0; // Shock inflicted to the player when jumping out of vehicle
}

/**@class		ExpansionActionGetOutExpansionVehicle
 * @brief		
 **/
class ExpansionActionGetOutExpansionVehicle : ActionInteractBase
{

	//For the two following variables -> The HIGHER the value, the LOWER the output
	int m_DmgFactor = 60;	//value used to translate impact strength into actual damage (impact strength -> velocity squared)
	int m_ShockFactor = 15; //Value used to translate impact strength into actual shock

	//Variables used to determine the different speed levels for bleeding checks
	const int LOW_SPEED_VALUE = 20;
	const int HIGH_SPEED_VALUE = 30;

	void ExpansionActionGetOutExpansionVehicle()
	{
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ALL;
		//m_HUDCursorIcon = "GetInDriver";
	}

	override ActionData CreateActionData()
	{
		ActionData action_data = new ExpansionActionGetOutExpansionVehicleActionData;
		return action_data;
	}
	
	override void WriteToContext(ParamsWriteContext ctx, ActionData action_data)
	{
		super.WriteToContext(ctx, action_data);

		ExpansionActionGetOutExpansionVehicleActionData poActionData;
		poActionData = ExpansionActionGetOutExpansionVehicleActionData.Cast(action_data);

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
		ExpansionActionGetOutExpansionVehicleActionData action_data_po = ExpansionActionGetOutExpansionVehicleActionData.Cast(action_data);

		action_data_po.m_KeepInVehicleSpaceAfterLeave = recive_data_po.m_KeepInVehicleSpaceAfterLeave;
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}

	override string GetText()
	{
		return "#leave_vehicle";
	}

	override typename GetInputType()
	{
		return ContinuousInteractActionInput;
	}

	override bool HasProgress()
	{
		return false;
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		ExpansionVehicleBase trans = null;
		int crew_index = -1;

		ExpansionHumanCommandVehicle vehCommand = player.GetCommand_ExpansionVehicle();
		if (vehCommand)
		{
			if (Class.CastTo(trans, vehCommand.GetObject()))
			{
				crew_index = trans.CrewMemberIndex(player);

				/*According today testing (12.06.2020) this hack is no longer needed
				//Hack for NIVA - disabling jumpng out from the back seats
				Car car;
				if ( Class.CastTo(car, trans) )
				{
					if ( car.GetSpeedometer() > 8 && crew_index >= 2 && trans.GetType() == "OffroadHatchback" )
						return false;
				}
*/

				if (crew_index >= 0 && trans.CrewCanGetThrough(crew_index) && trans.IsAreaAtDoorFree(crew_index))
					return true;
			}
		}

		return false;
	}

	override bool CanBeUsedInRestrain()
	{
		return true;
	}

	override bool CanBeUsedInVehicle()
	{
		return true;
	}

	override void OnUpdate(ActionData action_data)
	{
		if (action_data.m_State == UA_START)
		{
			if (!action_data.m_Player.GetCommand_ExpansionVehicle())
			{
				End(action_data);
			}
		}
	}

	override void Start(ActionData action_data)
	{
		super.Start(action_data);

		PerformGetOutTransport(action_data);
	}

	void PerformGetOutTransport(ActionData action_data)
	{
		ExpansionHumanCommandVehicle vehCommand = action_data.m_Player.GetCommand_ExpansionVehicle();
		if (vehCommand)
		{
			ExpansionVehicleBase veh = vehCommand.GetExpansionVehicle();

			float speed = Math.AbsFloat(veh.GetSpeedometer());
			bool keepInVehicleSpaceAfterLeave = veh.Expansion_CanObjectAttach(action_data.m_Player) && veh.LeavingSeatDoesAttachment(vehCommand.GetVehicleSeat());

			auto got_action_data = ExpansionActionGetOutExpansionVehicleActionData.Cast(action_data);
			got_action_data.m_StartLocation = got_action_data.m_Player.GetPosition();
			got_action_data.m_Car = veh;
			got_action_data.m_CarSpeed = speed;
			//TODO: Figure out what this comment even means... copied from vanilla - Jacob_Mango
			got_action_data.m_DmgTaken = (got_action_data.m_CarSpeed * got_action_data.m_CarSpeed) / m_DmgFactor; //When using multiplications, wrong value is returned
			got_action_data.m_ShockTaken = (got_action_data.m_CarSpeed * got_action_data.m_CarSpeed) / m_ShockFactor;
			
			if (IsMissionClient())
				got_action_data.m_KeepInVehicleSpaceAfterLeave = keepInVehicleSpaceAfterLeave;

			vehCommand.KeepInVehicleSpaceAfterLeave(got_action_data.m_KeepInVehicleSpaceAfterLeave);

			if (got_action_data.m_KeepInVehicleSpaceAfterLeave)
			{
				action_data.m_Player.m_ExAttachmentObject = veh;
				vehCommand.GetOutVehicle();
			}
			else if (speed <= 8)
			{
				vehCommand.GetOutVehicle();
			}
			else
			{
				got_action_data.m_WasJumpingOut = true;
				vehCommand.JumpOutVehicle();
			}

			GetDayZGame().GetBacklit().OnLeaveCar();
			if (action_data.m_Player.GetInventory())
				action_data.m_Player.GetInventory().LockInventory(LOCK_FROM_SCRIPT);
		}
	}

	override void End(ActionData action_data)
	{
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(Unhide, 500, false, (action_data.m_Player));
		super.End(action_data);
	}

	void Unhide(PlayerBase player)
	{
		player.GetItemAccessor().HideItemInHands(false);
	}

	override void OnEnd(ActionData action_data)
	{
		if (action_data.m_Player.GetInventory())
			action_data.m_Player.GetInventory().UnlockInventory(LOCK_FROM_SCRIPT);
	}

	override void OnEndServer(ActionData action_data)
	{
		ExpansionActionGetOutExpansionVehicleActionData got_action_data = ExpansionActionGetOutExpansionVehicleActionData.Cast(action_data);
		if (got_action_data.m_KeepInVehicleSpaceAfterLeave)
			return;

		action_data.m_Player.Expansion_SetIsInVehicleSeatOrAttached(false);
		if (got_action_data.m_Car && got_action_data.m_Car.IsHelicopter())
			got_action_data.m_Car.SetHasPilot(false);  //! So we are able to detect if pilot got disconnected or got out on own accord

		//! The following code is similar to vanilla ActionGetOutTransport::OnEndServer,
		//! except that we don't use RaycastRV(Proxy) because it can return results that aren't even in the rays path or radius.
		//! This fixes an exploit that was introduced with DayZ 1.11 where players could glitch through floors when getting out of a vehicle.

		vector endLocation = action_data.m_Player.GetPosition() + "0 0.5 0";

		PhxInteractionLayers layerMask = PhxInteractionLayers.BUILDING | PhxInteractionLayers.DOOR | PhxInteractionLayers.VEHICLE | PhxInteractionLayers.ROADWAY | PhxInteractionLayers.TERRAIN | PhxInteractionLayers.ITEM_SMALL | PhxInteractionLayers.ITEM_LARGE | PhxInteractionLayers.FENCE;

		Object hitObject;
		vector hitPosition;
		vector hitNormal;
		float hitFraction;

		if (DayZPhysics.SphereCastBullet(got_action_data.m_StartLocation, endLocation, 0.3, layerMask, got_action_data.m_Car, hitObject, hitPosition, hitNormal, hitFraction))
		{
			vector offset = got_action_data.m_StartLocation - hitPosition;
			offset.Normalize();
			offset = offset * 0.5; //! Reduce offset so there's less chance that player gets stuck in vehicle
			got_action_data.m_Player.SetPosition(hitPosition + offset);
		}

		if (got_action_data.m_WasJumpingOut)
		{
			got_action_data.m_Player.OnJumpOutVehicleFinish(got_action_data.m_CarSpeed);

			PlayerBase player = got_action_data.m_Player;

			ApplyJumpOutDmg(action_data);

			vector posMS = player.WorldToModel(player.GetPosition());
			player.DamageAllLegs(got_action_data.m_DmgTaken); //! Additional leg specific damage dealing
			player.ProcessDirectDamage(DT_CUSTOM, player, "", "FallDamage", posMS, got_action_data.m_DmgTaken);
		}

		if (got_action_data.m_Car)
		{
			CarScript car;
			if (Class.CastTo(car, got_action_data.m_Car))
			{
				car.ForceUpdateLightsEnd();
			}
		
			if (action_data.m_Player && action_data.m_Player.GetIdentity() && GetExpansionSettings().GetLog().VehicleLeave)
			{
				GetExpansionSettings().GetLog().PrintLog("[VehicleLeave] Player " + action_data.m_Player.GetIdentity().GetName() + " [uid=" + action_data.m_Player.GetIdentity().GetId() + "] left vehicle " + got_action_data.m_Car.GetDisplayName() + " (id=" + got_action_data.m_Car.GetVehiclePersistentIDString() + " pos=" + got_action_data.m_Car.GetPosition() + ")");
			}
		}
	}

	//Manage all jumping out of vehicle damage logic
	void ApplyJumpOutDmg(ActionData action_data)
	{
		ExpansionActionGetOutExpansionVehicleActionData got_action_data = ExpansionActionGetOutExpansionVehicleActionData.Cast(action_data);
		PlayerBase player = got_action_data.m_Player;

		//Get all currently equipped clothing
		// ---------------------------------------------

		ClothingBase trousers = ClothingBase.Cast(player.GetItemOnSlot("LEGS"));
		ClothingBase bag = ClothingBase.Cast(player.GetItemOnSlot("BACK"));
		ClothingBase vest = ClothingBase.Cast(player.GetItemOnSlot("VEST"));
		ClothingBase headGear = ClothingBase.Cast(player.GetItemOnSlot("HeadGear"));
		ClothingBase mask = ClothingBase.Cast(player.GetItemOnSlot("Mask"));
		ClothingBase shirt = ClothingBase.Cast(player.GetItemOnSlot("BODY"));
		ClothingBase shoes = ClothingBase.Cast(player.GetItemOnSlot("FEET"));
		ClothingBase gloves = ClothingBase.Cast(player.GetItemOnSlot("GLOVES"));

		//Array used to find all relevant information about currently equipped clothes
		array<ClothingBase> equippedClothes = new array<ClothingBase>;

		equippedClothes.Insert(trousers);
		equippedClothes.Insert(bag);
		equippedClothes.Insert(vest);
		equippedClothes.Insert(headGear);
		equippedClothes.Insert(mask);
		equippedClothes.Insert(shirt);
		equippedClothes.Insert(shoes);
		equippedClothes.Insert(gloves);

		// -----------------------------------------------

		//Lower shock taken if player uses a helmet
		headGear = ClothingBase.Cast(player.GetItemOnHead());
		HelmetBase helmet;

		if (Class.CastTo(helmet, headGear))
			got_action_data.m_ShockTaken *= 0.5;

		// -----------------------------------------------

		int randNum; //value used for probability evaluation
		randNum = Math.RandomInt(0, 100);
		if (got_action_data.m_CarSpeed < LOW_SPEED_VALUE)
		{
			if (randNum < 20)
				player.GiveShock(-got_action_data.m_ShockTaken); //To inflict shock, a negative value must be passed

			randNum = Math.RandomIntInclusive(0, PlayerBase.m_BleedingSourcesLow.Count() - 1);

			player.m_BleedingManagerServer.AttemptAddBleedingSourceBySelection(PlayerBase.m_BleedingSourcesLow[randNum]);
		}
		else if (got_action_data.m_CarSpeed >= LOW_SPEED_VALUE && got_action_data.m_CarSpeed < HIGH_SPEED_VALUE)
		{
			if (randNum < 50)
				player.GiveShock(-got_action_data.m_ShockTaken);

			randNum = Math.RandomInt(0, PlayerBase.m_BleedingSourcesUp.Count() - 1);

			player.m_BleedingManagerServer.AttemptAddBleedingSourceBySelection(PlayerBase.m_BleedingSourcesUp[randNum]);
		}
		else if (got_action_data.m_CarSpeed >= HIGH_SPEED_VALUE)
		{
			headGear = ClothingBase.Cast(player.GetItemOnHead());
			if (!Class.CastTo(helmet, headGear))
				player.m_BleedingManagerServer.AttemptAddBleedingSourceBySelection("Head");
			if (randNum < 75)
				player.GiveShock(-got_action_data.m_ShockTaken);
		}

		int nbClothes = 0;

		//Damage all currently equipped clothes
		for (int i = 0; i < equippedClothes.Count(); i++)
		{
			//If no item is equipped on slot, slot is ignored
			if (equippedClothes[i] == null)
				continue;

			equippedClothes[i].DecreaseHealth(got_action_data.m_DmgTaken, false);
			nbClothes++;
		}
	}
};
