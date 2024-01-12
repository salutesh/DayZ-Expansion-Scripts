class eAIActionTakeItemToHands: ActionInteractBase
{
	void eAIActionTakeItemToHands()
	{
		m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_PICKUP_HANDS;
		m_CommandUIDProne	= DayZPlayerConstants.CMD_ACTIONFB_PICKUP_HANDS;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem 	= new CCINone();
		m_ConditionTarget 	= new CCTObject(UAMaxDistances.DEFAULT);
	}	
	
	override bool HasProneException()
	{
		return true;
	}
	
	override typename GetInputType()
	{
		return ContinuousInteractActionInput;
	}
	
	override bool HasProgress()
	{
		return false;
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		return false;
	}
	
	override bool CanContinue(ActionData action_data)
	{
		return true;
	}
	
	override void OnExecuteServer(ActionData action_data)
	{
		ItemBase ntarget = ItemBase.Cast(action_data.m_Target.GetObject());
		ClearInventoryReservationEx(action_data);

		if (!ntarget)
			return;
		
		float stackable = ntarget.GetTargetQuantityMax(-1);
		
		if (stackable == 0 || stackable >= ntarget.GetQuantity())
		{
			eAIBase ai = eAIBase.Cast(action_data.m_Player);
			//! Need to do actual take to hands in next frame to avoid segfault, don't ask why
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(ai.eAI_TakeItemToHandsImpl, ntarget);
		}
		else
		{
			InventoryLocation il = new InventoryLocation();
			il.SetHands(action_data.m_Player, ntarget);
			ntarget.SplitIntoStackMaxToInventoryLocationClient(il);
		}
	}
	
	override void CreateAndSetupActionCallback(ActionData action_data)
	{
		EntityAI target = EntityAI.Cast(action_data.m_Target.GetObject());
		if (!target)
			return;
		
		ActionBaseCB callback;
		
		if (target.IsHeavyBehaviour())
		{
			Class.CastTo(callback, action_data.m_Player.StartCommand_Action(DayZPlayerConstants.CMD_ACTIONFB_PICKUP_HEAVY,GetCallbackClassTypename(), DayZPlayerConstants.STANCEMASK_ERECT));
		}
		else
		{
			if (action_data.m_Player.IsPlayerInStance(DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT))
			{
				Class.CastTo(callback, action_data.m_Player.AddCommandModifier_Action(m_CommandUID,GetCallbackClassTypename()));
			}
			else
			{
				Class.CastTo(callback, action_data.m_Player.StartCommand_Action(m_CommandUIDProne,GetCallbackClassTypename(),DayZPlayerConstants.STANCEMASK_PRONE));
			}
		}

		callback.SetActionData(action_data); 
		callback.InitActionComponent();
		action_data.m_Callback = callback;
	}
	
		
	override bool CanBeUsedOnBack()
	{
		return true;
	}
}
