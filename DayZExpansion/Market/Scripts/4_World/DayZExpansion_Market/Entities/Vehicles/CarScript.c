#ifdef EXPANSIONMODVEHICLE
modded class CarScript
{
#ifndef DAYZ_1_29
	//! 1.30+
	override void OnCEIterate(float currentTime, float elapsedTime)
	{
		super.OnCEIterate(currentTime, elapsedTime);
#else
	//! 1.29
	override void OnCEUpdate()
	{
		super.OnCEUpdate();
#endif

		m_ExpansionVehicle.EvaluateSafeZoneParkingFine();
	}

	void Expansion_EvaluateSafeZoneParkingFine()
	{
		EXError.WarnOnce(this, "DEPRECATED");
		m_ExpansionVehicle.EvaluateSafeZoneParkingFine();
	}

	void Expansion_SetParkingFine(int fine)
	{
		EXError.WarnOnce(this, "DEPRECATED");
		m_ExpansionVehicle.SetParkingFine(fine);
	}

	int Expansion_GetParkingFine()
	{
		EXError.WarnOnce(this, "DEPRECATED");
		return m_ExpansionVehicle.GetParkingFine();
	}
}
#endif
