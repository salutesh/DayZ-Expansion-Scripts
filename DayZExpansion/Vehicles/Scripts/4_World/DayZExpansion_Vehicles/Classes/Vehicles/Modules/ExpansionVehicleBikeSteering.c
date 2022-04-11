/*
 * ExpansionVehicleBikeSteering.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionVehicleBikeSteering : ExpansionVehicleSteering
{
	float m_Current;
	float m_Target;

	override void Control(ExpansionPhysicsState pState, DayZPlayerImplement pDriver)
	{
		m_Target = 0;

		if (pDriver)
		{
			UAInterface input = pDriver.GetInputInterface();

			float left = input.SyncedValue_ID(UACarLeft);
			float right = input.SyncedValue_ID(UACarRight);

			float velocity = Math.AbsFloat(pState.m_LinearVelocityMS[2]);

			m_Target = (right - left) / (Math.Max(velocity, 1.0));
		}

		m_Current += Math.Clamp(m_Target - m_Current, -20.0 * pState.m_DeltaTime, 20.0 * pState.m_DeltaTime);

		m_Controller.m_Yaw = m_Current;
	}
};
