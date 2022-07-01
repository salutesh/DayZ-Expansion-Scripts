class eAINPCBase: eAIBase
{
	override void Init()
	{
		super.Init();

		if (GetGame().IsServer())
		{
			GetGroup().SetFaction(new eAIFactionPassive());
		}

		m_Expansion_NameOverride = new ExpansionNameOverride(this);
	}
}
