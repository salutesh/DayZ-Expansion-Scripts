/**
 * ExpansionQuestObjectiveActionEvent.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestObjectiveActionEvent: ExpansionQuestObjectiveEventBase
{
	protected bool m_ActionState;
	protected bool m_CallLater;

	void OnActionUsed(ActionBase actionBase, ActionData actionData)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnActionUsed");
	#endif

		ObjectivePrint(ToString() + "::OnActionUsed - Action name: " + actionBase.ClassName());
		ObjectivePrint(ToString() + "::OnActionUsed - Action base: " + actionBase.ToString());
		ObjectivePrint(ToString() + "::OnActionUsed - Action data: " + actionData.ToString());

		if (GetObjectiveConfig().GetActionNames().Find(actionBase.ClassName()) == -1)
			return;

		bool isContolledAction = false;

		PlayerBase player;
		PlayerBase target_player;
		if (actionBase.ClassName() == "ActionBandageSelf")
		{
			isContolledAction = true;
			player = PlayerBase.Cast(actionData.m_Player);

			if (m_CallLater)
			{
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(CheckEntity);
				m_CallLater = false;
			}

			if (!m_CallLater)
			{
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(CheckEntity, 1000, true, player, actionBase.ClassName());
				m_CallLater = true;
			}
		}
		else if (actionBase.ClassName() == "ActionBandageTarget")
		{
			isContolledAction = true;
			target_player = PlayerBase.Cast(actionData.m_Target.GetParentOrObject());

			if (m_CallLater)
			{
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(CheckEntity);
				m_CallLater = false;
			}

			if (!m_CallLater)
			{
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(CheckEntity, 1000, true, target_player, actionBase.ClassName());
				m_CallLater = true;
			}
		}

	#ifdef EXPANSIONMODVEHICLE
		if (actionBase.ClassName() == "ExpansionActionPickVehicleLock")
		{
			isContolledAction = true;
			CarScript carScript = CarScript.Cast(actionData.m_Target.GetParentOrObject());

			if (GetObjectiveConfig().GetAllowedClassNames().Count() > 0 && GetObjectiveConfig().GetAllowedClassNames().Find(carScript.GetType()) > -1 || GetObjectiveConfig().GetExcludedClassNames().Count() > 0 && GetObjectiveConfig().GetExcludedClassNames().Find(carScript.GetType()) == -1)
			{
				if (m_CallLater)
				{
					GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(CheckEntity);
					m_CallLater = false;
				}

				ObjectivePrint(ToString() + "::OnActionUsed - Action was: ExpansionActionPickVehicleLock");
				if (carScript && GetExpansionSettings().GetVehicle().CanPickLock && !m_CallLater)
				{
					GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(CheckEntity, 1000, true, carScript, actionBase.ClassName());
					m_CallLater = true;
				}
			}
		}
		else if (actionBase.ClassName() == "ExpansionVehicleActionPickLock")
		{
			isContolledAction = true;
			ExpansionVehicleBase vehicleBase = ExpansionVehicleBase.Cast(actionData.m_Target.GetParentOrObject());

			if (GetObjectiveConfig().GetAllowedClassNames().Count() > 0 && GetObjectiveConfig().GetAllowedClassNames().Find(vehicleBase.GetType()) > -1 || GetObjectiveConfig().GetExcludedClassNames().Count() > 0 && GetObjectiveConfig().GetExcludedClassNames().Find(vehicleBase.GetType()) == -1)
			{
				if (m_CallLater)
				{
					GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(CheckEntity);
					m_CallLater = false;
				}

				ObjectivePrint(ToString() + "::OnActionUsed - Action was: ExpansionVehicleActionPickLock");
				if (vehicleBase && GetExpansionSettings().GetVehicle().CanPickLock && !m_CallLater)
				{
					GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(CheckEntity, 1000, true, vehicleBase, actionBase.ClassName());
					m_CallLater = true;
				}
			}
		}
	#endif
		else if (actionBase.ClassName() == "ActionPlantSeed")
		{
			isContolledAction = true;
			ItemBase seed_IB = ItemBase.Cast(actionData.m_MainItem);
			if (GetObjectiveConfig().GetAllowedClassNames().Count() > 0 && GetObjectiveConfig().GetAllowedClassNames().Find(seed_IB.GetType()) > -1 || GetObjectiveConfig().GetExcludedClassNames().Count() > 0 && GetObjectiveConfig().GetExcludedClassNames().Find(seed_IB.GetType()) == -1)
			{
				m_ActionState = true;
				SetCompleted(true);
				OnComplete();
			}
		}
		else if (actionBase.ClassName() == "ActionDigOutStash")
		{
			isContolledAction = true;						
			UndergroundStash stash;
			array<Object> objects = new array<Object>;
			
			GetGame().GetObjectsAtPosition3D(actionData.m_Player.GetPosition(), 10.0, objects, NULL);
			
			foreach (Object obj: objects)
			{
				if (Class.CastTo(stash, obj) && stash.GetQuestID() == GetQuest().GetQuestConfig().GetID())
				{
					if (m_CallLater)
					{
						GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(CheckEntity);
						m_CallLater = false;
					}
					
					ObjectivePrint(ToString() + "::OnActionUsed - Action was: ActionDigOutStash | Stash: " + stash.GetType());
					ObjectivePrint(ToString() + "::OnActionUsed - Action was: ActionDigOutStash | Stashed Item: " + stash.GetStashedItem());
					if (stash.GetStashedItem() && stash.GetStashedItem().GetQuestID() == GetQuest().GetQuestConfig().GetID() && !m_CallLater)
					{
						GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(CheckEntity, 1000, true, stash.GetStashedItem(), actionBase.ClassName());
						m_CallLater = true;
					}
				}
			}
		}

		if (!isContolledAction)
		{
			m_ActionState = true;
			SetCompleted(true);
			OnComplete();
		}
	}

	void CheckEntity(EntityAI entity, string actionName)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "CheckEntity");
	#endif

		PlayerBase player;
		if (Class.CastTo(player, entity) && (actionName == "ActionBandageSelf" || actionName == "ActionBandageTarget"))
		{
			ObjectivePrint(ToString() + "::CheckEntity - Player bleeding state: " + player.IsBleeding());

			if (player && !player.IsBleeding())
			{
				m_ActionState = true;
				SetCompleted(true);
				OnComplete();

				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(CheckEntity);
			}
		}

	#ifdef EXPANSIONMODVEHICLE
		CarScript carScript;
		ExpansionVehicleBase vehicleBase;
		if (Class.CastTo(carScript, entity) && actionName == "ExpansionActionPickVehicleLock")
		{
			ObjectivePrint(ToString() + "::CheckEntity - Car lock state: " + carScript.GetLockedState());
			ObjectivePrint(ToString() + "::CheckEntity - Car locked: " + carScript.IsLocked());

			if (carScript && (carScript.GetLockedState() == ExpansionVehicleLockState.FORCEDUNLOCKED || !carScript.IsLocked()))
			{
				m_ActionState = true;
				SetCompleted(true);
				OnComplete();

				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(CheckEntity);
			}
		}
		else if (Class.CastTo(vehicleBase, entity) && actionName == "ExpansionVehicleActionPickLock")
		{
			ObjectivePrint(ToString() + "::CheckEntity - Vehicle lock state: " + vehicleBase.GetLockedState());
			ObjectivePrint(ToString() + "::CheckEntity - Vehicle locked: " + vehicleBase.IsLocked());

			if (vehicleBase && (vehicleBase.GetLockedState() == ExpansionVehicleLockState.FORCEDUNLOCKED || !vehicleBase.IsLocked()))
			{
				m_ActionState = true;
				SetCompleted(true);
				OnComplete();

				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(CheckEntity);
			}
		}
	#endif
		
		ItemBase item;
		if (Class.CastTo(item, entity) && actionName == "ActionDigOutStash")
		{
			ObjectivePrint(ToString() + "::CheckEntity - Stashed item: " + item.GetType());
			ObjectivePrint(ToString() + "::CheckEntity - Stashed item quest ID: " + item.GetQuestID());

			if (item.GetHierarchyRoot() == item && item.GetQuestID() == GetQuest().GetQuestConfig().GetID())
			{
				ObjectivePrint(ToString() + "::CheckEntity - Digged out stashed item " + item.GetType());
				m_ActionState = true;
				SetCompleted(true);
				OnComplete();
		
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(CheckEntity);
			}
		}
	}

	bool GetActionState()
	{
		return m_ActionState;
	}

	void SetActionState(bool state)
	{
		m_ActionState = state;
	}

	override int GetObjectiveType()
	{
		return ExpansionQuestObjectiveType.ACTION;
	}
};