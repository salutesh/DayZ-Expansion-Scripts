/**
 * DayZPlayerCameraBase.c
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
modded class DayZPlayerCameraBase
{
	PlayerBase m_Ex_Player;

	void DayZPlayerCameraBase(DayZPlayer pPlayer, HumanInputController pInput)
	{
		Class.CastTo(m_Ex_Player, pPlayer);
	}

	override void OnUpdate(float pDt, out DayZPlayerCameraResult pOutResult)
	{
		super.OnUpdate(pDt, pOutResult);
		
		Ex_OnUpdate(pDt, pOutResult);
		Ex_OnPostUpdate(pDt, pOutResult);
	}

	void Ex_OnUpdate(float pDt, out DayZPlayerCameraResult pOutResult)
	{

	}

	void Ex_OnPostUpdate(float pDt, out DayZPlayerCameraResult pOutResult)
	{
		if (pOutResult.m_fInsideCamera <= 0.7)
		{
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).Call(m_Ex_Player.SetHeadInvisible_Ex, false);
		}
		else
		{
			m_Ex_Player.SetHeadInvisible_Ex(true);
		}
	}
};
