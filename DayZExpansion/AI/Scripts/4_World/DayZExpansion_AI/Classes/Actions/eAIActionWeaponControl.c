[RegisterAction(eAIActionWeaponControl)]
class eAIActionWeaponControl : ActionBase
{
	override typename GetInputType()
	{
		return DefaultActionInput;
	}

	override bool HasTarget()
	{
		return false;
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINonRuined();
		m_ConditionTarget = new CCTSelf;
	}

	override bool HasProgress()
	{
		return false;
	}
	
	override bool CanBePerformedFromInventory()
	{
		return true;
	}
	
	override bool CanBeUsedOnBack()
	{
		return true;
	}
	
	override bool CanBeUsedRaised()
	{
		return true;
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		// return false so normal players can't run this action. AI does not call this method
		return false;
	}

	override void Start(ActionData action_data)
	{
		super.Start(action_data);
		
		action_data.m_State = UA_PROCESSING;
	}

	override void OnUpdate(ActionData action_data)
	{
		super.OnUpdate(action_data);

		Weapon_Base wpn = Weapon_Base.Cast(action_data.m_MainItem);
		
		if (!wpn || action_data.m_Player.GetItemInHands() != wpn || !action_data.m_Player.GetWeaponManager().IsRunning())
		{
			End(action_data);
		}
	}
};
