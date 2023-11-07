class eAIActionDropItem : ActionSingleUseBase
{
	void eAIActionDropItem()
	{
		m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_DROPITEM_HANDS;
		m_CommandUIDProne	= DayZPlayerConstants.CMD_ACTIONFB_DROPITEM_HANDS;
		m_CallbackClass 	= ActionDropItemCB;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem 	= new CCINone();
		m_ConditionTarget 	= new CCTNone();
	}
	
	override typename GetInputType()
	{
		return DropActionInput;
	}
	
	override bool HasProneException()
	{
		return true;
	}
	
	override bool CanBeUsedThrowing()
	{
		return true;
	}
	
	override bool HasTarget()
	{
		return false;
	}
	
	override bool MainItemAlwaysInHands()
	{
		return false;
	}

	override bool ActionConditionContinue(ActionData action_data)
	{
		return true;
	}
	
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		return false;
	}
	
	override void OnExecuteServer(ActionData action_data)
	{
		eAIBase ai = eAIBase.Cast(action_data.m_Player);
		ai.eAI_DropItemImpl(action_data.m_MainItem, ai.eAI_GetItemThreatOverride(action_data.m_MainItem));
	}
}
