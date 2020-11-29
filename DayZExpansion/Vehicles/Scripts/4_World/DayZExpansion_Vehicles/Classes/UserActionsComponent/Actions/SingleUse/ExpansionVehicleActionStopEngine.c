class ExpansionVehicleActionStopEngine : ActionSingleUseBase
{
	ExpansionVehicleBase m_Vehicle;

	void ExpansionVehicleActionStopEngine()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_STOPENGINE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		//m_HUDCursorIcon	 = CursorIcons.LootCorpse;
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}

	override string GetText()
	{
		if (m_Vehicle.IsPlane())
		{
			return "#STR_EXPANSION_UA_STOP_PLANE";
		}
		else if (m_Vehicle.IsHelicopter())
		{
			return "#STR_EXPANSION_UA_STOP_HELICOPTER";
		}

		return "#STR_EXPANSION_UA_STOP_CAR";
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		ExpansionHumanCommandVehicle vehCmd = player.GetCommand_ExpansionVehicle();

		if (vehCmd && Class.CastTo(m_Vehicle, vehCmd.GetTransport()))
		{
			if (vehCmd.GetVehicleSeat() == DayZPlayerConstants.VEHICLESEAT_DRIVER)
			{
				return m_Vehicle.EngineIsOn();
			}
		}

		return false;
	}

	override void OnExecuteServer(ActionData action_data)
	{
		ExpansionHumanCommandVehicle vehCmd = action_data.m_Player.GetCommand_ExpansionVehicle();
		ExpansionVehicleBase car;

		if (vehCmd && Class.CastTo(car, vehCmd.GetTransport()))
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

		if (vehCmd && Class.CastTo(car, vehCmd.GetTransport()))
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