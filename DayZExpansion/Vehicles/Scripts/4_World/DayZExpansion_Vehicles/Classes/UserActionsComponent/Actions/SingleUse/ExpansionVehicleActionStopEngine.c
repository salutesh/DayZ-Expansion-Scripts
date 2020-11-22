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
		else if (m_Vehicle.IsBoat())
		{
			if (m_Vehicle.MotorIsOn())
			{
				return "#STR_EXPANSION_UA_STOP_BOAT";
			}
			else
			{
				return "#STR_EXPANSION_UA_STOP_CAR";
			}
		}

		return "#STR_EXPANSION_UA_STOP_CAR";
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		ExpansionHumanCommandVehicle vehCmd = player.GetCommand_ExpansionVehicle();

		if (vehCmd && vehCmd.GetVehicleSeat() == DayZPlayerConstants.VEHICLESEAT_DRIVER)
		{
			m_Vehicle = vehCmd.GetTransport();
			if (m_Vehicle && (m_Vehicle.EngineIsOn() || m_Vehicle.MotorIsOn()))
			{
				return true;
			}
		}

		return false;
	}

	override void OnExecuteServer(ActionData action_data)
	{
		if (!Class.CastTo(m_Vehicle, action_data.m_Player.GetParent()))
			return;

		if (m_Vehicle.EngineIsOn())
			m_Vehicle.EngineStop();
		
		if (m_Vehicle.MotorIsOn())
			m_Vehicle.MotorStop();
	}

	override bool CanBeUsedInVehicle()
	{
		return true;
	}
}