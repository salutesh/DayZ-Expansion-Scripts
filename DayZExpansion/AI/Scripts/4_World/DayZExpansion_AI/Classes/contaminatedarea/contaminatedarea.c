modded class ContaminatedArea_Base
{
	override bool eAI_IsDangerousToAI(eAIBase ai)
	{
		if (ai.Expansion_CanBeDamaged() && ai.m_eAI_ProtectionLevels[DEF_CHEMICAL] < 6.0)
			return true;

		return false;
	}
}
