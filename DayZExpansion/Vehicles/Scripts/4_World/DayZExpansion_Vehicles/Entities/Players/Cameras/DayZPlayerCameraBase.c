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
	bool m_Ex_ForceHideHead;
	bool m_Ex_PreviousForceHideHead;

	void DayZPlayerCameraBase(DayZPlayer pPlayer, HumanInputController pInput)
	{
		Class.CastTo(m_Ex_Player, pPlayer);
	}

#ifndef EXPANSION_OBSOLETE_CAMERA
	override void OnUpdate(float pDt, out DayZPlayerCameraResult pOutResult)
	{
		ForceFreelook(false);

		super.OnUpdate(pDt, pOutResult);

		Ex_OnUpdate(pDt, pOutResult);
		Ex_OnPostUpdate(pDt, pOutResult);
	}

	void Ex_OnUpdate(float pDt, out DayZPlayerCameraResult pOutResult)
	{
		m_Ex_ForceHideHead = false;
	}

	void Ex_OnPostUpdate(float pDt, out DayZPlayerCameraResult pOutResult)
	{
		// This is only a fix for crashing issues that are currently present
		if (pOutResult.m_CollisionIgnoreEntity && !dBodyIsDynamic(pOutResult.m_CollisionIgnoreEntity))
		{
			pOutResult.m_CollisionIgnoreEntity = NULL;
		}

		if (m_Ex_PreviousForceHideHead == m_Ex_ForceHideHead)
			return;

		m_Ex_PreviousForceHideHead = m_Ex_ForceHideHead;

		if (m_Ex_ForceHideHead)
		{
			m_Ex_Player.SetHeadInvisible_Ex(true);
		}
		else
		{
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Call(m_Ex_Player.SetHeadInvisible_Ex, false);
		}
	}
#endif
};
