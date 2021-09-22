/**
 * ExpansionVehicleYoke.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionVehicleYoke : ExpansionVehicleSteering
{
	float m_YawTarget;
	float m_PitchTarget;
	float m_RollTarget;

	override void Control(ExpansionPhysicsState pState, DayZPlayerImplement pDriver)
	{
		UAInterface input = pDriver.GetInputInterface();

		#ifdef COMPONENT_SYSTEM
		float yaw_left = input.SyncedValue("UAExpansionPlaneRudderLeft");
		float yaw_right = input.SyncedValue("UAExpansionPlaneRudderRight");
		float pitch_left = input.SyncedValue("UAExpansionPlaneElevatorUp");
		float pitch_right = input.SyncedValue("UAExpansionPlaneElevatorDown");
		float roll_left = input.SyncedValue("UAExpansionPlaneAileronLeft");
		float roll_right = input.SyncedValue("UAExpansionPlaneAileronRight");
		#else
		float yaw_left = input.SyncedValue_ID(UAExpansionPlaneRudderLeft);
		float yaw_right = input.SyncedValue_ID(UAExpansionPlaneRudderRight);
		float pitch_left = input.SyncedValue_ID(UAExpansionPlaneElevatorUp);
		float pitch_right = input.SyncedValue_ID(UAExpansionPlaneElevatorDown);
		float roll_left = input.SyncedValue_ID(UAExpansionPlaneAileronLeft);
		float roll_right = input.SyncedValue_ID(UAExpansionPlaneAileronRight);
		#endif

		m_YawTarget = yaw_right - yaw_left;
		m_PitchTarget = pitch_right - pitch_left;
		m_RollTarget = roll_left - roll_right;

		m_Controller.m_Yaw += Math.Clamp(m_YawTarget - m_Controller.m_Yaw, -5.0 * pState.m_DeltaTime, 5.0 * pState.m_DeltaTime);
		m_Controller.m_Pitch += Math.Clamp(m_PitchTarget - m_Controller.m_Pitch, -5.0 * pState.m_DeltaTime, 5.0 * pState.m_DeltaTime);
		m_Controller.m_Roll += Math.Clamp(m_RollTarget - m_Controller.m_Roll, -5.0 * pState.m_DeltaTime, 5.0 * pState.m_DeltaTime);

		m_Controller.m_Yaw = Math.Clamp(m_Controller.m_Yaw, -1, 1);
		m_Controller.m_Pitch = Math.Clamp(m_Controller.m_Pitch, -1, 1);
		m_Controller.m_Roll = Math.Clamp(m_Controller.m_Roll, -1, 1);
	}
};
