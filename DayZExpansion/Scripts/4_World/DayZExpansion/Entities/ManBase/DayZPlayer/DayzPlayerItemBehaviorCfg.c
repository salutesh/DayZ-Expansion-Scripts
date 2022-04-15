/**
 * ExpansionDayzPlayerItemBehaviorCfg.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class DayzPlayerItemBehaviorCfg
{
	void SetChicken()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.CHICKEN, this, "SetChicken");
#endif

		m_iType = ItemBehaviorType.TWOHANDED;

		m_iStanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_StanceMovements[DayZPlayerConstants.STANCEIDX_ERECT] = DayZPlayerConstants.MOVEMENT_IDLE | DayZPlayerConstants.MOVEMENT_WALK | DayZPlayerConstants.MOVEMENT_RUN;
		m_iPerItemCameraUserData = DayZPlayerCameras.PERITEMUD_TWOHANDED;
	  
		SetIKTwoHanded();
		
		m_bAttackLean = true;
	}
};
