/**
 * DayZPlayerMeleeFightLogic_LightHeavy.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class DayZPlayerMeleeFightLogic_LightHeavy
{
	override void Init(DayZPlayerImplement player)
	{
		if (!player.IsInherited(ExpansionNotPlayerBase))
			super.Init(player);
	}

	override bool HandleFightLogic(int pCurrentCommandID, HumanInputController pInputs, EntityAI pEntityInHands, HumanMovementState pMovementState, out bool pContinueAttack)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.PLAYER, this, "HandleFightLogic");
#endif

		if (m_Player && m_Player.Expansion_IsInSafeZone())
			return false;

		return super.HandleFightLogic(pCurrentCommandID, pInputs, pEntityInHands, pMovementState, pContinueAttack);
	}
}