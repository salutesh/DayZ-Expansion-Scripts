class ExpansionActionStartEngine: ActionContinuousBase
{
	private const float ROUGH_SPECIALTY_WEIGHT = 0.5;
	
	ExpansionVehicleScript m_Vehicle;

	void ExpansionActionStartEngine()
	{
		m_CallbackClass = ActionStartCarCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_STARTENGINE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ALL;
		m_SpecialtyWeight = ROUGH_SPECIALTY_WEIGHT;
		m_LockTargetOnUse = false;
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
			return "#STR_EXPANSION_UA_START_PLANE";
		} else if ( m_Vehicle.IsHelicopter() )
		{
			return "#STR_EXPANSION_UA_START_HELICOPTER";
		}

		return "#STR_EXPANSION_UA_START_CAR";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		ExpansionHumanCommandVehicle vehCmd = player.GetCommand_ExpansionVehicle();
		
		if ( vehCmd && vehCmd.GetVehicleSeat() == DayZPlayerConstants.VEHICLESEAT_DRIVER )
		{
			m_Vehicle = vehCmd.GetTransport();
			if ( m_Vehicle && !m_Vehicle.EngineIsOn() )
			{
				return true;
			}
		}

		return false;
	}

	override void OnFinishProgressServer( ActionData action_data )
	{
		m_Vehicle.EngineStart();
	}
	
	override bool CanBeUsedInVehicle()
	{
		return true;
	}
}