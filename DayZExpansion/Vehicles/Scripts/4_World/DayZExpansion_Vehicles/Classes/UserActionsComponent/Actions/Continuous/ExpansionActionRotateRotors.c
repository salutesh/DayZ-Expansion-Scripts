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
	static float EXPANSION_ROTOR_ROTATE_SPEED = 0.051333;

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

		ExpansionHelicopterScript heli;
		if (!Class.CastTo(heli, target.GetObject()))
			return false;

		if (heli.m_Simulation.m_RotorSpeed > EXPANSION_ROTOR_ROTATE_SPEED)
			return false;

		if (!GetPermissionsManager().IsAdminToolsToggledOn())
			return false;

		if (target.GetObject().GetActionComponentName(target.GetComponentIndex()) != "dmgzone_engine")
			return false;

		return true;
	}

#ifndef SERVER
	//! This way it works in SP/offline, too

	override void OnStart(ActionData action_data)
	{
		super.OnStart(action_data);

		ExpansionHelicopterScript heli;
		if (Class.CastTo(heli, action_data.m_Target.GetParentOrObject()))
		{
			heli.Expansion_RotateRotors(EXPANSION_ROTOR_ROTATE_SPEED);
		}
	}

	override void OnEnd(ActionData action_data)
	{
		super.OnEnd(action_data);

		ExpansionHelicopterScript heli;
		if (Class.CastTo(heli, action_data.m_Target.GetParentOrObject()))
		{
			heli.Expansion_RotateRotors(0.0);
		}
	}
#endif
}
