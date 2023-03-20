modded class PlayerAgentPool
{
	override void AddAgent(int agent_id, float count)
	{
		//! Positive count means something bad is inserted (illness etc), negative count means something bad is removed (cured)
		//! If dmg is disabled, allow removal, but not insertion
		if (count > 0 && !m_Player.Expansion_CanBeDamaged())
			return;

		super.AddAgent(agent_id, count);
	}
}
