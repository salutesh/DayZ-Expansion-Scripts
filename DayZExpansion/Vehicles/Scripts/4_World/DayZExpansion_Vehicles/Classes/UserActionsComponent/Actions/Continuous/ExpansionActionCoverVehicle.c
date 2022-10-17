/**
 * ExpansionActionCoverVehicle.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionCoverVehicleCB: ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(5);
	}
};

class ExpansionActionCoverVehicle: ActionContinuousBase
{
	void ExpansionActionCoverVehicle()
	{
		m_CallbackClass = ExpansionActionCoverVehicleCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_CRAFTING;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ALL;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionTarget = new CCTCursor;
		m_ConditionItem = new CCINone;
	}

	override bool HasTarget()
	{
		return true;
	}

	override bool HasProgress()
	{
		return true;
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		Object targetObject = target.GetParentOrObject();
		if (!targetObject)
			return false;

		if (targetObject.IsDamageDestroyed())
			return false;

		if (!GetExpansionSettings().GetVehicle().EnableVehicleCovers)
			return false;

		string placeholderType;
		string vehicleModel;

		CarScript vehicle;
		ExpansionVehicleBase exVehicle;
		if (Class.CastTo(vehicle, targetObject))
		{
			if (vehicle.Expansion_GetVehicleCrew().Count())
				return false;

			if (!GetGame().IsDedicatedServer())
			{
				placeholderType = vehicle.Expansion_GetPlaceholderType(item.GetType());
				vehicleModel = vehicle.ConfigGetString("model");
			}
		}
		else if (Class.CastTo(exVehicle, targetObject))
		{
			if (exVehicle.Expansion_GetVehicleCrew().Count())
				return false;

			if (!GetGame().IsDedicatedServer())
			{
				placeholderType = exVehicle.Expansion_GetPlaceholderType(item.GetType());
				vehicleModel = exVehicle.ConfigGetString("model");
			}
		}
		else
		{
			return false;
		}

		if (!GetGame().IsDedicatedServer())
		{
			string placeHolderModel = GetGame().ConfigGetTextOut("CfgVehicles " + placeholderType + " model");
			if (placeHolderModel == vehicleModel)
				m_Text = "#STR_EXPANSION_ACTION_STORE";
			else
				m_Text = "#STR_EXPANSION_ACTION_COVER";
		}

#ifdef EXPANSIONMODGARAGE
		//! Check if vehicle has any cargo items that are not attachments if the "CanStoreWithCargo" setting is enabled.
		if (!GetExpansionSettings().GetGarage().CanStoreWithCargo)
		{
			EntityAI entity;
			if (Class.CastTo(entity, targetObject) && MiscGameplayFunctions.Expansion_HasAnyCargo(entity))
			{
				ExpansionNotification(new StringLocaliser("STR_EXPANSION_GARAGE_ERROR"), new StringLocaliser("STR_EXPANSION_GARAGE_ERROR_CARGO", vehicle.GetDisplayName()), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_ERROR, 7, ExpansionNotificationType.GARAGE).Create();
				return false;
			}
		}
#endif

		return true;
	}
	
	override void OnFinishProgressServer(ActionData action_data)
	{
		EntityAI entity;
		if (!Class.CastTo(entity, action_data.m_Target.GetParentOrObject()))
			return;

		CarScript vehicle;
		ExpansionVehicleBase exVehicle;
		if (Class.CastTo(vehicle, entity))
			vehicle.Expansion_CoverVehicle(action_data.m_MainItem);
		else if (Class.CastTo(exVehicle, entity))
			exVehicle.Expansion_CoverVehicle(action_data.m_MainItem);
	}
}
