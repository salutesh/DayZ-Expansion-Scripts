/**
 * ExpansionActionRestoreEntity.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionRestoreEntityCB: ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(5);
	}
};

[RegisterAction(ExpansionActionRestoreEntity)]
class ExpansionActionRestoreEntity: ActionContinuousBase
{
	void ExpansionActionRestoreEntity()
	{
		m_CallbackClass = ExpansionActionRestoreEntityCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INTERACT;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ALL;
		m_Text = "#STR_EXPANSION_ACTION_RESTORE";
	}

	override void CreateConditionComponents()  
	{
		m_ConditionTarget = new CCTCursor;
		m_ConditionItem = new CCINone;
	}

	override typename GetInputType()
	{
		return ContinuousInteractActionInput;
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
		auto placeholder = ExpansionEntityStoragePlaceholder.Cast(target.GetParentOrObject());
		if (!placeholder)
			return false;

		if (!placeholder.Expansion_HasStoredEntity())
			return false;

		return true;
	}

	bool Expansion_OnFinishProgressServer(ActionData action_data, bool deletePlaceholder = true, out ExpansionEntityStoragePlaceholder placeholder = null, out EntityAI entity = null, bool log = true)
	{
		if (!Class.CastTo(placeholder, action_data.m_Target.GetParentOrObject()))
			return false;

		string id = placeholder.m_Expansion_StoredEntityGlobalID.IDToHex();

		vector position = placeholder.GetPosition();
		placeholder.SetPosition("0 0 0");
		if (ExpansionEntityStorageModule.RestoreFromFile(placeholder.Expansion_GetEntityStorageFileName(), entity, placeholder))
		{
			if (deletePlaceholder)
				GetGame().ObjectDelete(placeholder);
		}
		else
		{
			placeholder.SetPosition(position);
			string type;
			placeholder.m_Expansion_NetsyncData.Get(0, type);
			Error(ToString() + "::Expansion_OnFinishProgressServer - Could not restore vehicle " + type + " from file " + placeholder.Expansion_GetEntityStorageFileName());
			ExpansionNotification("Entity Storage", "Could not restore " + placeholder.Expansion_GetStoredEntityDisplayName()).Error(action_data.m_Player.GetIdentity());
			if (log && GetExpansionSettings().GetLog().EntityStorage)
				GetExpansionSettings().GetLog().PrintLog("[EntityStorage]::ERROR:: Player \"%1\" (id=%2 pos=%3) tried to restore a entity \"%4\" (GlobalID=%5 pos=%6) but it failed!", action_data.m_Player.GetIdentity().GetName(), action_data.m_Player.GetIdentity().GetId(), action_data.m_Player.GetPosition().ToString(), type, id, placeholder.GetPosition().ToString());
			return false;
		}
		
		if (log && GetExpansionSettings().GetLog().EntityStorage)
			GetExpansionSettings().GetLog().PrintLog("[EntityStorage] Player \"%1\" (id=%2 pos=%3) restored a entity \"%4\" (GlobalID=%5 pos=%6)", action_data.m_Player.GetIdentity().GetName(), action_data.m_Player.GetIdentity().GetId(), action_data.m_Player.GetPosition().ToString(), entity.GetType(), id, entity.GetPosition().ToString());

		return true;
	}

	override void OnFinishProgressServer(ActionData action_data)
	{
		Expansion_OnFinishProgressServer(action_data);
	}
}
