class ExpansionActionFillHydraulicFluidCB: ActionContinuousBaseCB
{
	private const float TIME_TO_REPEAT = 0.05;

	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousExpansionFillHydraulicFluid(UAQuantityConsumed.FILL_LIQUID, TIME_TO_REPEAT);
	}
}

class ExpansionActionFillHydraulicFluid: ActionContinuousBase
{
	void ExpansionActionFillHydraulicFluid()
	{
		m_CallbackClass = ExpansionActionFillHydraulicFluidCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_EMPTY_VESSEL;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_FullBody = true;
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_LOW;
		m_LockTargetOnUse = false;
		m_Text = "#refill_car";
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{	
		if (!target || !IsTransport(target))
			return false;

		if (item.GetQuantity() <= 0)
			return false;

		ExpansionVehicle vehicle;
		if (ExpansionVehicle.Get(vehicle, target.GetObject()) && vehicle.IsHelicopter())
		{
			if (vehicle.GetFluidFraction(CarFluid.OIL) >= 0.98)
				return false;

			Transport transport = vehicle.GetTransport();

			TStringArray selections = {};
			transport.GetActionComponentNameList(target.GetComponentIndex(), selections);

			foreach (string selection: selections)
			{
				if (selection == transport.GetActionCompNameFuel())
				{
					float dist = vector.Distance(transport.GetRefillPointPosWS(), player.GetPosition());

					if (dist < transport.GetActionDistanceFuel())
						return true;
				}
			}
		}

		return false;
	}

	override void OnEndAnimationLoopServer(ActionData action_data)
	{	
		if (action_data.m_MainItem && action_data.m_MainItem.GetQuantity() <= 0.01)
		{
			action_data.m_MainItem.SetQuantity(0);
		}
	}
}
