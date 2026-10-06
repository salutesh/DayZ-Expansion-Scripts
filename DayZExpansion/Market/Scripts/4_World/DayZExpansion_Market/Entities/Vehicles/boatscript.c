#ifdef EXPANSIONMODVEHICLE
modded class BoatScript
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
}
#endif