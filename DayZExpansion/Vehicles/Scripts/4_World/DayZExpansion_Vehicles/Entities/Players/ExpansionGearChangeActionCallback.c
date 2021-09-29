class ExpansionGearChangeActionCallback : HumanCommandActionCallback
{
	ExpansionHumanCommandVehicle m_hcv;

	void SetVehicleCommand(ExpansionHumanCommandVehicle hcv)
	{
		m_hcv = hcv;
		m_hcv.SetClutchState(true);
	}

	override void OnFinish(bool pCanceled)
	{
		m_hcv.SetClutchState(false);
	}
};
