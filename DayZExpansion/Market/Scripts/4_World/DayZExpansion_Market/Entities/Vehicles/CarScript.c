#ifdef EXPANSIONMODVEHICLE
modded class CarScript
{
	override void OnCEUpdate()
	{
		super.OnCEUpdate();

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
