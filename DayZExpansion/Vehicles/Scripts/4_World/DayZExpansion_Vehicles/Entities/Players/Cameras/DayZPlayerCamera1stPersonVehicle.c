/**
 * DayZPlayerCamera1stPersonVehicle.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class DayZPlayerCamera1stPersonVehicle
{
	void DayZPlayerCamera1stPersonVehicle(DayZPlayer pPlayer, HumanInputController pInput)
	{
		m_Expansion_CameraHandler = new ExpansionCameraHandler(this);
		m_ExpansionVehicle = ExpansionVehicle.Get(m_Ex_Player);
	}

	override void OnActivate(DayZPlayerCamera pPrevCamera, DayZPlayerCameraResult pPrevCameraResult)
	{
		super.OnActivate(pPrevCamera, pPrevCameraResult);
		
		m_Expansion_CameraHandler.OnActivate(m_fLeftRightAngle, m_fLeftRightAngleAdd, m_fUpDownAngle, m_fUpDownAngleAdd, pPrevCamera, pPrevCameraResult);
	}

	override void OnUpdate( float pDt, out DayZPlayerCameraResult pOutResult )
	{
		bool isFreeLook = true;

		ExpansionVehicleHelicopter simulation;
		ExpansionPhysicsState pState;

		ExpansionHelicopterScript helicopter = m_ExpansionVehicle.GetExpansionHelicopter();
		if (helicopter)
		{
			if (m_Ex_Player.Expansion_IsDriver())
				isFreeLook = helicopter.IsFreeLook();

			simulation = helicopter.m_Simulation;
			pState = helicopter.m_State;
		}

		m_ExIsFreeLook = isFreeLook;

		m_Expansion_CameraHandler.OnUpdate(pDt, m_fLeftRightAngle, m_fLeftRightAngleAdd, m_fUpDownAngle, m_fUpDownAngleAdd, simulation, pState);

		super.OnUpdate( pDt, pOutResult );

		pOutResult.m_fUseHeading			= 0.0;
	}

	override void Expansion_GetUDAngles(out float pAngle, out float pAngleAdd)
	{
		pAngle = m_fUpDownAngle;
		pAngleAdd = m_fUpDownAngleAdd;
	}
};
