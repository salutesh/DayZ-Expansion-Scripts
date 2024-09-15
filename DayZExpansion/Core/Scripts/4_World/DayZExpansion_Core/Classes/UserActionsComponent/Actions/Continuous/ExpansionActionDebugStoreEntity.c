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
		m_ActionData.m_ActionComponent = new CAContinuousTime(5);
	}
};

class ExpansionActionDebugStoreEntity: ActionContinuousBase
{
	void ExpansionActionDebugStoreEntity()
	{
		m_CallbackClass = ExpansionActionDebugStoreEntityCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INTERACT;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_Text = "[ADMIN] Goatify";
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
#ifdef USER_jacob
		// fuck off
		return false;
#endif
		
		if (item)
			return false;

		auto entity = EntityAI.Cast(target.GetParentOrObject());
		if (!entity)
			return false;

		if (entity.IsInherited(ExpansionEntityStoragePlaceholder))
			return false;

		if (entity.IsInherited(ExpansionNPCBase))
			return false;

	#ifdef EXPANSIONMODAI
		if (entity.IsInherited(eAINPCBase))
			return false;
	#endif

		if (entity.ConfigGetInt("scope") != 2)
			return false;

		auto permissionsManager = GetPermissionsManager();
	#ifdef SERVER
		if (!permissionsManager.HasPermission("Expansion.EntityStorage.Goatify", player.GetIdentity()))
	#else
		if (!permissionsManager.IsAdminToolsToggledOn() || !permissionsManager.HasPermission("Expansion.EntityStorage.Goatify"))
	#endif
			return false;

		auto vehicle = ExpansionVehicle.Get(entity);
		if (vehicle)
		{
			if (vehicle.GetCrew().Count())
				return false;
		}

#ifdef EXPANSIONMODBASEBUILDING
		ItemBase targetItem;
		if (Class.CastTo(targetItem, entity) && targetItem.Expansion_HasEntityStorage())
			return false;
#endif

		return true;
	}

	override void OnStartServer(ActionData action_data)
	{
		Object targetObject = action_data.m_Target.GetParentOrObject();
		ExpansionSound.Play("cartent_deploy_SoundSet", targetObject.GetPosition());
	}

	override void OnEndServer(ActionData action_data)
	{
		Object targetObject = action_data.m_Target.GetParentOrObject();
		if (targetObject)
			ExpansionSound.Stop("cartent_deploy_SoundSet", targetObject.GetPosition(), 0.1);
	}
	
	override void OnFinishProgressServer(ActionData action_data)
	{
		auto entity = EntityAI.Cast(action_data.m_Target.GetParentOrObject());
		if (!entity)
			return;

		ExpansionEntityStoragePlaceholder placeholder;
		if (ExpansionEntityStoragePlaceholder.Expansion_StoreEntityAndReplace(entity, "ExpansionDebugGoat", entity.GetPosition(), ECE_OBJECT_SWAP, placeholder))
			ExpansionSound.Play(placeholder.GetPlaceSoundset(), placeholder);
	}
}
