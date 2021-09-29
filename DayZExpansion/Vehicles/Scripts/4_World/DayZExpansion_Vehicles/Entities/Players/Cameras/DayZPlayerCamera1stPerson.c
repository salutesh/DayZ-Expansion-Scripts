/**
 * DayZPlayerCamera1stPerson.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifndef EXPANSION_OBSOLTE_CAMERA
modded class DayZPlayerCamera1stPerson
{
	override void Ex_OnUpdate(float pDt, out DayZPlayerCameraResult pOutResult)
	{
		super.Ex_OnUpdate(pDt, pOutResult);

		ExpansionVehicleBase vehicle;
		if (Class.CastTo(vehicle, m_Ex_Player.GetParent()) || m_Ex_Player.IsAttached())
		{
			pOutResult.m_fInsideCamera = 0.0;
			m_Ex_ForceHideHead = true;
		}

		if (m_Ex_Player.GetCommand_Vehicle() || m_Ex_Player.GetCommand_ExpansionVehicle())
			return;

		if (m_Ex_Player.IsAttached())
		{
			pOutResult.m_CameraTM[3] = m_Ex_Player.GetBonePositionMS(m_iBoneIndex);

			pOutResult.m_CollisionIgnoreEntity = m_Ex_Player.GetParent();
			pOutResult.m_fIgnoreParentRoll = 1.0;
			pOutResult.m_fIgnoreParentPitch = 1.0;
			pOutResult.m_iDirectBone = -1.0;
			pOutResult.m_iDirectBoneMode = 0;
			pOutResult.m_fDistance = 0;
			pOutResult.m_fPositionModelSpace = 1.0;
		}
	}
};
#endif
