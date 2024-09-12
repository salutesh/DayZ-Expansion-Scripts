modded class Wreck_Mi8_Crashed
{
	override void EEOnCECreate()
	{
		super.EEOnCECreate();

		m_Expansion_Node = s_Expansion_HeliCrashes.Add(this);
	}

	override void AfterStoreLoad()
	{
		super.AfterStoreLoad();

		m_Expansion_Node = s_Expansion_HeliCrashes.Add(this);
	}

	void ~Wreck_Mi8_Crashed()
	{
		if (!GetGame())
			return;

		if (s_Expansion_HeliCrashes)
			s_Expansion_HeliCrashes.Remove(m_Expansion_Node);
	}
}
