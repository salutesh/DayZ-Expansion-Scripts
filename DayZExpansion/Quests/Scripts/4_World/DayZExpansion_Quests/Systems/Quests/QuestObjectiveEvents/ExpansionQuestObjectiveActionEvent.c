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
	private bool m_ActionState;
	private bool m_CallLater;
	private EntityAI m_LastEntity;

	override void OnStart()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnStart");
	#endif

		super.OnStart();
	}

	override void OnContinue()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.QUESTS, this, "OnContinue");
	#endif

		super.OnContinue();
	}
	
	void OnActionUsed(ActionBase actionBase, ActionData actionData)
	{
		QuestPrint(ToString() + "::OnActionUsed - Start");
		QuestPrint(ToString() + "::OnActionUsed - Action name: " + actionBase.ClassName());
		QuestPrint(ToString() + "::OnActionUsed - Action base: " + actionBase.ToString());
		QuestPrint(ToString() + "::OnActionUsed - Action data: " + actionData.ToString());

		if (GetObjectiveConfig().GetActionNames().Find(actionBase.ClassName()) == -1)
			return;
		
		bool isContolledAction = false;
	#ifdef EXPANSIONMODVEHICLE
		if (actionBase.ClassName() == "ExpansionActionPickVehicleLock")
		{
			isContolledAction = true;
			CarScript carScript = CarScript.Cast(actionData.m_Target.GetParentOrObject());
			
			if (m_LastEntity != carScript)
			{
				m_LastEntity = carScript;
				if (m_CallLater)
				{
					GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(CheckEntity);
					m_CallLater = false;
				}
			}
			
			QuestPrint(ToString() + "::OnActionUsed - Action was: ExpansionActionPickVehicleLock");						
			if (carScript && GetExpansionSettings().GetVehicle().CanPickLock && !m_CallLater)
			{
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(CheckEntity, 1000, true, carScript, actionBase.ClassName());
				m_CallLater = true;
			}
		}
		else if (actionBase.ClassName() == "ExpansionVehicleActionPickLock")
		{
			isContolledAction = true;
			ExpansionVehicleBase vehicleBase = ExpansionVehicleBase.Cast(actionData.m_Target.GetParentOrObject());
			
				if (m_LastEntity != vehicleBase)
			{
				m_LastEntity = vehicleBase;
				if (m_CallLater)
				{
					GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(CheckEntity);
					m_CallLater = false;
				}
			}
			
			QuestPrint(ToString() + "::OnActionUsed - Action was: ExpansionVehicleActionPickLock");		
			if (vehicleBase && GetExpansionSettings().GetVehicle().CanPickLock && !m_CallLater)
			{
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(CheckEntity, 1000, true, vehicleBase, actionBase.ClassName());
				m_CallLater = true;
			}
		}
	#endif
		
		if (!isContolledAction)
		{
			m_ActionState = true;
			SetCompleted(true);
			OnComplete();
		}
				
		QuestPrint(ToString() + "::OnActionUsed - End");
	}
	
	void CheckEntity(EntityAI enity, string actionName)
	{
		QuestPrint(ToString() + "::CheckEntity - Start");
		
	#ifdef EXPANSIONMODVEHICLE
		CarScript carScript;
		ExpansionVehicleBase vehicleBase;
		if (Class.CastTo(carScript, enity) && actionName == "ExpansionActionPickVehicleLock")
		{
			QuestPrint(ToString() + "::CheckEntity - Car lock state: " + carScript.GetLockedState());
			QuestPrint(ToString() + "::CheckEntity - Car locked: " + carScript.IsLocked());
			
			if (carScript && (carScript.GetLockedState() == ExpansionVehicleLockState.FORCEDUNLOCKED || !carScript.IsLocked()))
			{
				m_ActionState = true;
				SetCompleted(true);
				OnComplete();
				
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(CheckEntity);
			}
		}
		else if (Class.CastTo(vehicleBase, enity) && actionName == "ExpansionVehicleActionPickLock")
		{
			QuestPrint(ToString() + "::CheckEntity - Vehicle lock state: " + vehicleBase.GetLockedState());
			QuestPrint(ToString() + "::CheckEntity - Vehicle locked: " + vehicleBase.IsLocked());
			
			if (vehicleBase && (vehicleBase.GetLockedState() == ExpansionVehicleLockState.FORCEDUNLOCKED || !vehicleBase.IsLocked()))
			{
				m_ActionState = true;
				SetCompleted(true);
				OnComplete();
				
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(CheckEntity);
			}
		}
	#endif
		
		QuestPrint(ToString() + "::CheckVehicle - End");
	}
	
	bool GetActionState()
	{
		return m_ActionState;
	}
	
	void SetActionState(bool state)
	{
		m_ActionState = state;
	}
	
	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);
	}

	override int GetObjectiveType()
	{
		return ExpansionQuestObjectiveType.ACTION;
	}
	
	void QuestPrint(string text)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		Print(text);
	#endif
	}
};