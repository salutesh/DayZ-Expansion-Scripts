class CAContinuousExpansionRotateRotors: CAContinuousBase
{		
	override bool IsContinuousAction()
	{
		return true;
	}

	override int Execute(ActionData action_data)
	{
		return UA_INITIALIZE;  //! Makes it stay in "loading" circle w/o progress
	}
}

class ExpansionActionRotateRotorsCB: ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousExpansionRotateRotors;
	}
}

class ExpansionActionRotateRotors: ActionContinuousBase
{
	void ExpansionActionRotateRotors()
	{
		m_CallbackClass = ExpansionActionRotateRotorsCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INTERACT;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ALL;
		m_Text = "[ADMIN] Rotate Rotors";	
	}

	override typename GetInputType()
	{
		return ContinuousInteractActionInput;
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTCursor;
	}

	override bool IsLocal()
	{
		return true;
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (item)
			return false;

		if (!target.GetParentOrObject().IsInherited(ExpansionHelicopterScript))
			return false;

		if (!GetPermissionsManager().IsAdminToolsToggledOn())
			return false;

		return true;
	}

	override void OnStartClient(ActionData action_data)
	{
		ExpansionHelicopterScript heli;
		if (Class.CastTo(heli, action_data.m_Target.GetParentOrObject()))
		{
			heli.Expansion_RotateRotors(0.051333);
		}
	}

	override void OnEndClient(ActionData action_data)
	{
		ExpansionHelicopterScript heli;
		if (Class.CastTo(heli, action_data.m_Target.GetParentOrObject()))
		{
			heli.Expansion_RotateRotors(0.0);
		}
	}
}
