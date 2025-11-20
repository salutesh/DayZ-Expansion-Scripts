modded class Wreck_SantasSleigh
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

	void ~Wreck_SantasSleigh()
	{
	#ifndef DIAG_DEVELOPER
		if (!g_Game)
			return;
	#endif

		if (s_Expansion_HeliCrashes)
			s_Expansion_HeliCrashes.Remove(m_Expansion_Node);
	}
}
