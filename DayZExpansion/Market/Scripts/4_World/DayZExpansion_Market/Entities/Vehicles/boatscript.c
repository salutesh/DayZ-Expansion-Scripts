#ifdef EXPANSIONMODVEHICLE
modded class BoatScript
{
	override void OnCEUpdate()
	{
		super.OnCEUpdate();

		m_ExpansionVehicle.EvaluateSafeZoneParkingFine();
	}
}
#endif