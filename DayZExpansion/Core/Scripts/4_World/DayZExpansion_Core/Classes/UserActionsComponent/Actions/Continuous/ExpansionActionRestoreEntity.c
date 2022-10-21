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

	bool Expansion_OnFinishProgressServer(ActionData action_data, bool deletePlaceholder = true, out ExpansionEntityStoragePlaceholder placeholder = null)
	{
		if (!Class.CastTo(placeholder, action_data.m_Target.GetParentOrObject()))
			return false;

		vector position = placeholder.GetPosition();
		placeholder.SetPosition("0 0 0");
		EntityAI entity;
		if (ExpansionEntityStorageModule.RestoreFromFile(placeholder.Expansion_GetEntityStorageFileName(), entity, placeholder, action_data.m_Player))
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
			return false;
		}

		return true;
	}

	override void OnFinishProgressServer(ActionData action_data)
	{
		Expansion_OnFinishProgressServer(action_data);
	}
}
