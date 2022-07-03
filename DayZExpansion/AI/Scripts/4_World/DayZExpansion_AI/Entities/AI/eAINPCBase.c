class eAINPCBase: eAIBase
{
	override void Expansion_Init()
	{
		if (GetGame().IsServer())
		{
			m_eAI_FactionType = eAIFactionPassive;
			eAI_SetPassive();  //! In case this NPC joins another group that's part of a non-passive faction, we still want the NPC to be passive
		}

		super.Expansion_Init();

		m_Expansion_NetsyncData = new ExpansionNetsyncData(this);
	}

	//! Vanilla, prevent being targeted by Zs/animals
	override bool CanBeTargetedByAI(EntityAI ai)
	{
		return false;
	}
}
