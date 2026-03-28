class ExpansionActionSwitchLights: ActionInteractBase
{
	void ExpansionActionSwitchLights()
	{
		m_CommandUID        = DayZPlayerConstants.CMD_ACTIONMOD_HEADLIGHT;
		m_StanceMask        = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_HUDCursorIcon     = CursorIcons.LootCorpse;
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNone;
	}

	override string GetText()
	{
		return "#switch_lights";
	}

	override typename GetInputType()
	{
		return ExpansionToggleLightsActionInput;
	}
	
	override bool HasTarget()
	{
		return false;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		return false;
	}
	
	override bool CanBeUsedInVehicle()
	{
		return true;
	}
};