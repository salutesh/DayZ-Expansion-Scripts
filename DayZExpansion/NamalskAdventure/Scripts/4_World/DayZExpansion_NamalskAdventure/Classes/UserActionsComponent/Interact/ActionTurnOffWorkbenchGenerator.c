class ActionTurnOffWorkbenchGenerator: ActionInteractBase
{
	void ActionTurnOffWorkbenchGenerator()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_Text = "#switch_off";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		Object targetObject = target.GetObject();
		ItemBase target_IB = ItemBase.Cast( targetObject );

		if ( target_IB.HasEnergyManager()  &&  target_IB.GetCompEM().CanSwitchOff() )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	override void OnExecuteServer( ActionData action_data )
	{
		Object targetObject = action_data.m_Target.GetObject();
		if ( targetObject )
		{
			ItemBase target_IB = ItemBase.Cast( targetObject );
			target_IB.GetCompEM().SwitchOff();
			target_IB.GetCompEM().InteractBranch(target_IB);
		}
	}
};