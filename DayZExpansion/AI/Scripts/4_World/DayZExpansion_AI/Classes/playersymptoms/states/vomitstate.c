modded class VomitSymptom
{
	override void OnAnimationStart()
	{
		super.OnAnimationStart();

		//! AI can "puke out" chemical agents with the blood they vomit.
		//! Would make sense to have this for players as well, maybe as an option?
		if (IsContaminationActive() && m_Player.IsAI())
			m_Player.InsertAgent(eAgents.CHEMICAL_POISON, -BLOOD_LOSS * ActionGiveBloodSelf.CHEM_AGENT_BLOOD_REMOVAL_MODIFIER);
	}
}
