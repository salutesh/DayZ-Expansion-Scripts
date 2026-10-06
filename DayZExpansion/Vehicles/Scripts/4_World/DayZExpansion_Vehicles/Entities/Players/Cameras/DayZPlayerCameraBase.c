/**
 * DayZPlayerCameraBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class DayZPlayerCameraBase
{
	PlayerBase m_Ex_Player;
	bool m_ExIsFreeLook = true;
	ref ExpansionCameraHandler m_Expansion_CameraHandler;
	ref ExpansionVehicle m_ExpansionVehicle;

	void DayZPlayerCameraBase(DayZPlayer pPlayer, HumanInputController pInput)
	{
		Class.CastTo(m_Ex_Player, pPlayer);
	}

	override float UpdateUDAngleUnlocked(out float pAngle, out float pAngleAdd, float pMin, float pMax, float pDt)
	{
		if (m_Expansion_CameraHandler && (!m_ExIsFreeLook || m_Expansion_CameraHandler.m_CenterCamera || (m_ExpansionVehicle && m_ExpansionVehicle.IsHelicopter() && m_pInput.CameraIsTracking())))
			return m_Expansion_CameraHandler.UpdateUDAngleUnlocked(pAngle, pAngleAdd, pMin, pMax, pDt);
		else
			return super.UpdateUDAngleUnlocked(pAngle, pAngleAdd, pMin, pMax, pDt);
	}

	override float UpdateLRAngleUnlocked(out float pAngle, out float pAngleAdd, float pMin, float pMax, float pDt)
	{
		if (m_Expansion_CameraHandler && (!m_ExIsFreeLook || m_Expansion_CameraHandler.m_CenterCamera || (m_ExpansionVehicle && m_ExpansionVehicle.IsHelicopter() && m_pInput.CameraIsTracking())))
			return m_Expansion_CameraHandler.UpdateLRAngleUnlocked(pAngle, pAngleAdd, pMin, pMax, pDt);
		else
			return super.UpdateLRAngleUnlocked(pAngle, pAngleAdd, pMin, pMax, pDt);
	}

	override void StdFovUpdate(float pDt, out DayZPlayerCameraResult pOutResult)
	{
		if (!m_Expansion_CameraHandler || !m_Expansion_CameraHandler.StdFovUpdate(pDt, m_fFovAbsolute, m_fFovAbsVel, pOutResult))
			super.StdFovUpdate(pDt, pOutResult);
	}

	HumanInputController Expansion_GetInput()
	{
		return m_pInput;
	}

	void Expansion_GetUDAngleVel(out float vel[])
	{
		vel = m_fUDAngleVel;
	}

	void Expansion_GetLRAngleVel(out float vel[])
	{
		vel = m_fLRAngleVel;
	}

	HumanCommandWeapons Expansion_GetCommandWeapons()
	{
		return m_CommandWeapons;
	}

	void Expansion_GetUDAngles(out float pAngle, out float pAngleAdd)
	{
	}
};
