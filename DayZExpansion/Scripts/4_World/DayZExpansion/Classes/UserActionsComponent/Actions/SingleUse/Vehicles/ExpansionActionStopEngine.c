class ExpansionActionStopEngine: ActionSingleUseBase
{
    ExpansionVehicleScript m_Vehicle;

	void ExpansionActionStopEngine()
	{
		m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_STOPENGINE;
		m_StanceMask        = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		//m_HUDCursorIcon     = CursorIcons.LootCorpse;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}

	override string GetText()
	{
		if ( m_Vehicle.IsPlane() )
		{
			return "#STR_EXPANSION_UA_STOP_PLANE";
		} else if ( m_Vehicle.IsHelicopter() )
		{
			return "#STR_EXPANSION_UA_STOP_HELICOPTER";
		}

		return "#STR_EXPANSION_UA_STOP_CAR";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
        ExpansionHumanVehicleCommand vehCmd = player.GetCommand_ExpansionVehicle();
		
		if ( vehCmd && vehCmd.GetVehicleSeat() == DayZPlayerConstants.VEHICLESEAT_DRIVER )
		{
			m_Vehicle = vehCmd.GetTransport();
			if ( m_Vehicle && m_Vehicle.EngineIsOn() )
			{
				return true;
			}
		}

		return false;
	}

	override void OnExecuteServer( ActionData action_data )
	{
		m_Vehicle.EngineStop();
	}
	
	override bool CanBeUsedInVehicle()
	{
		return true;
	}
}