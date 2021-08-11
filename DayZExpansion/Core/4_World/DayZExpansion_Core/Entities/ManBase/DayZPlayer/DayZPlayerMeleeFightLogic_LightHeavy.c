/**
 * DayZPlayerMeleeFightLogic_LightHeavy.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class DayZPlayerMeleeFightLogic_LightHeavy
{
	override bool HandleFightLogic(int pCurrentCommandID, HumanInputController pInputs, EntityAI pEntityInHands, HumanMovementState pMovementState, out bool pContinueAttack)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZPlayerMeleeFightLogic_LightHeavy::HandleFightLogic Start");
		#endif

		PlayerBase player = PlayerBase.Cast(m_DZPlayer);

		if (player && player.IsInSafeZone())
			return false;

		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZPlayerMeleeFightLogic_LightHeavy::HandleFightLogic End");
		#endif

		return super.HandleFightLogic(pCurrentCommandID, pInputs, pEntityInHands, pMovementState, pContinueAttack);
	}
}