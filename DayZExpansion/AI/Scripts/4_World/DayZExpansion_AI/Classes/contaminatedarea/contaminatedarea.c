modded class ContaminatedArea_Base
{
	override bool eAI_IsDangerousToAI(eAIBase ai)
	{
		//! @note we deliberately DON'T use Expansion_CanBeDamaged because we want to avoid (e.g.) going into an area that overlaps a safezone
		if (ai.m_eAI_ProtectionLevels[DEF_CHEMICAL] < 6.0 && !ai.GetGroup().GetFaction().IsInvincible())
			return true;

		return false;
	}
}
