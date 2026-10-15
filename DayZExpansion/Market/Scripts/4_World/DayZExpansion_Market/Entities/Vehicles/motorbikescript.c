#ifndef DAYZ_1_29
//! 1.30+
modded class MotorbikeScript
{
	override void OnCEIterate(float currentTime, float elapsedTime)
	{
		super.OnCEIterate(currentTime, elapsedTime);

		m_ExpansionVehicle.EvaluateSafeZoneParkingFine();
	}
}
#endif
