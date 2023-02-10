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

		auto settings = GetExpansionSettings().GetVehicle();

		if (!settings.EnableVehicleCovers)
			return false;

		string placeholderType;
		string vehicleModel;

		CarScript vehicle;
		ExpansionVehicleBase exVehicle;
		if (Class.CastTo(vehicle, targetObject))
		{
			if (!vehicle.m_Expansion_HasLifetime && !settings.AllowCoveringDEVehicles)
				return false;

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
			if (!vehicle.m_Expansion_HasLifetime && !settings.AllowCoveringDEVehicles)
				return false;

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

		//! Check if vehicle has any cargo items that are not attachments if the "CanStoreWithCargo" setting is enabled.
		if (!settings.CanCoverWithCargo)
		{
			EntityAI entity;
			if (Class.CastTo(entity, targetObject) && MiscGameplayFunctions.Expansion_HasAnyCargo(entity))
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

		return true;
	}
	
	override void OnFinishProgressServer(ActionData action_data)
	{
		EntityAI entity;
		if (!Class.CastTo(entity, action_data.m_Target.GetParentOrObject()))
			return;

		string id;
		string type = entity.GetType();

		CarScript vehicle;
		ExpansionVehicleBase exVehicle;
		ExpansionEntityStoragePlaceholder placeholder;
		bool result;
		if (Class.CastTo(vehicle, entity))
		{
			id = vehicle.m_Expansion_GlobalID.IDToHex();
			result = vehicle.Expansion_CoverVehicle(action_data.m_MainItem, placeholder);
		}
		else if (Class.CastTo(exVehicle, entity))
		{
			id = exVehicle.m_Expansion_GlobalID.IDToHex();
			result = exVehicle.Expansion_CoverVehicle(action_data.m_MainItem, placeholder);
		}
		
		if (GetExpansionSettings().GetLog().VehicleCover)
		{
			if (result)
				GetExpansionSettings().GetLog().PrintLog("[VehicleCover] Player \"%1\" (id=%2 pos=%3) covered vehicle \"%4\" (GlobalID=%5 pos=%6)!", action_data.m_Player.GetIdentity().GetName(), action_data.m_Player.GetIdentity().GetId(), action_data.m_Player.GetPosition().ToString(), type, id, placeholder.GetPosition().ToString());
			else
				GetExpansionSettings().GetLog().PrintLog("[VehicleCover] ERROR: Player \"%1\" (id=%2 pos=%3) tried to cover vehicle \"%4\" (GlobalID=%5 pos=%6) but it failed!", action_data.m_Player.GetIdentity().GetName(), action_data.m_Player.GetIdentity().GetId(), action_data.m_Player.GetPosition().ToString(), entity.GetType(), id, entity.GetPosition().ToString());
		}
	}
}
