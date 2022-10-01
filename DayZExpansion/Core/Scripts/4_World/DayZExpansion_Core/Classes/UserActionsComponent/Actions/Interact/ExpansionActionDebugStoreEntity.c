/**
 * ExpansionActionDebugStoreEntity.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionDebugStoreEntityCB: ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(3);
	}
};

class ExpansionActionDebugStoreEntity: ActionContinuousBase
{
	string m_EntityName;

	void ExpansionActionDebugStoreEntity()
	{
		m_CallbackClass = ExpansionActionDebugStoreEntityCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INTERACT;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ALL;
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

	override string GetText()
	{
		return "[DEBUG] Store " + m_EntityName;
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		auto entity = EntityAI.Cast(target.GetParentOrObject());
		if (!entity)
			return false;

		m_EntityName = entity.GetDisplayName();

		return true;
	}
	
	override void OnFinishProgressServer(ActionData action_data)
	{
		auto entity = EntityAI.Cast(action_data.m_Target.GetParentOrObject());
		if (!entity)
			return;

		string type = entity.GetType();
		int id;
		auto placeholder = ExpansionDebugGoat.Cast(ExpansionEntityStorageModule.SaveToFileAndReplace(entity, "ExpansionDebugGoat", id));
		if (placeholder)
		{
			placeholder.m_Expansion_EntityStorageID = id;
			placeholder.m_Expansion_NetsyncData.Set(1, type);
		}
	}
}
