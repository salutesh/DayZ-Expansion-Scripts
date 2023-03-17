class ExpansionPowerSwitch: ItemBase
{
	override bool ExpansionIsPowerSource()
	{
		return true;
	}
	
	override void OnSwitchOn()
	{
		super.OnSwitchOn();

		SetAnimationPhase( "switch", 1 );
	}
	
	override void OnSwitchOff()
	{
		super.OnSwitchOff();

		SetAnimationPhase( "switch", 0 );
	}

	override void SetActions()
	{
		super.SetActions();

		AddAction( ExpansionActionConnectElectricityToSource );
		AddAction( ExpansionActionDisconnectElectricityToSource );
		
		AddAction( ExpansionActionTogglePowerSwitch );
	}
	
	override bool IsDeployable()
	{
		return true;
	}	

	override bool IsOneHandedBehaviour()
	{
		return true;
	}
};