modded class CoughSymptom
{
	override void OnGetActivatedServer(PlayerBase player)
	{
		super.OnGetActivatedServer(player);
		
		if (IsContaminationActive() && m_Player.IsAI())
			m_Player.InsertAgent(eAgents.CHEMICAL_POISON, -COUGH_BLOOD_LOSS * ActionGiveBloodSelf.CHEM_AGENT_BLOOD_REMOVAL_MODIFIER);
	}
}
