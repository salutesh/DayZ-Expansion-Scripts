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
		m_Text = "#STR_EXPANSION_ACTION_COVER";
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

		CarScript vehicle;
		if (Class.CastTo(vehicle, targetObject) && vehicle.Expansion_GetVehicleCrew().Count())
			return false;

		ExpansionVehicleBase exVehicle;
		if (Class.CastTo(exVehicle, targetObject) && exVehicle.Expansion_GetVehicleCrew().Count())
			return false;

		if (!vehicle && !exVehicle)
			return false;

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

		string type = entity.GetType();
		string coverType = action_data.m_MainItem.GetType();

		string skinBase = entity.ConfigGetString("skinBase");
		if (skinBase)
			type = skinBase;

		string placeholderType = type + "_Cover";
	
		if (coverType.Contains("Civil"))
			placeholderType += "_Civil";
		else if (coverType.Contains("Desert"))
			placeholderType += "_Desert";
		else if (coverType.Contains("Winter"))
			placeholderType += "_Winter";

		if (!GetGame().ConfigIsExisting("CfgVehicles " + placeholderType))
			placeholderType = "Expansion_Generic_Vehicle_Cover";

		ExpansionEntityStoragePlaceholder placeholder;
		if (ExpansionEntityStoragePlaceholder.Expansion_StoreEntityAndReplace(entity, placeholderType, entity.GetPosition(), ECE_OBJECT_SWAP, placeholder))
		{
			bool result = action_data.m_Player.ServerTakeEntityToTargetAttachmentEx(placeholder, action_data.m_MainItem, InventorySlots.GetSlotIdFromString("CamoNet"));
			EXTrace.Print(EXTrace.GENERAL_ITEMS, this, "Moved " + action_data.m_MainItem + " to " + placeholder + "? " + result);
		}
	}
}
