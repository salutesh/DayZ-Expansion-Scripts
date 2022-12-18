[RegisterAction(ExpansionVehicleActionStopEngine)]
class ExpansionVehicleActionStopEngine : ActionSingleUseBase
{
	ExpansionVehicleBase m_Vehicle;

	void ExpansionVehicleActionStopEngine()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_STOPENGINE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}

	override string GetText()
	{
		return "Stop " + m_Vehicle.EngineGetName() + " Engine";
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		ExpansionHumanCommandVehicle vehCmd = player.GetCommand_ExpansionVehicle();

		if (vehCmd && Class.CastTo(m_Vehicle, vehCmd.GetExpansionVehicle()))
		{
			if (vehCmd.GetVehicleSeat() == DayZPlayerConstants.VEHICLESEAT_DRIVER)
			{
				m_CommandUID = m_Vehicle.EngineStopAnimation();

				if (Math.AbsFloat(m_Vehicle.GetSpeedometer()) <= 8 && m_Vehicle.EngineIsOn())
					return true;
			}
		}

		return false;
	}

	override void OnExecuteServer(ActionData action_data)
	{
		ExpansionHumanCommandVehicle vehCmd = action_data.m_Player.GetCommand_ExpansionVehicle();
		ExpansionVehicleBase car;

		if (vehCmd && Class.CastTo(car, vehCmd.GetExpansionVehicle()))
		{
			car.EngineStop();

			if (!GetGame().IsMultiplayer())
				SEffectManager.PlaySound(car.m_EngineStopFuel, car.GetPosition());
		}
	}

	override void OnExecuteClient(ActionData action_data)
	{
		ExpansionHumanCommandVehicle vehCmd = action_data.m_Player.GetCommand_ExpansionVehicle();
		ExpansionVehicleBase car;

		if (vehCmd && Class.CastTo(car, vehCmd.GetExpansionVehicle()))
		{
			car.EngineStop();

			SEffectManager.PlaySound(car.m_EngineStopFuel, car.GetPosition());
		}
	}

	override bool CanBeUsedInVehicle()
	{
		return true;
	}

	override bool UseMainItem()
	{
		return false;
	}
};
