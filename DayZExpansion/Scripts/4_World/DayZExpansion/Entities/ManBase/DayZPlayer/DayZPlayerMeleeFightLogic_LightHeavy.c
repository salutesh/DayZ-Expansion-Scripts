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