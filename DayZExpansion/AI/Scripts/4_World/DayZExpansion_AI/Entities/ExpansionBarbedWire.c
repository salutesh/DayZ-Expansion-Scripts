#ifdef DZ_Expansion_BaseBuilding
modded class ExpansionBarbedWire
{
	override protected eAIItemTargetInformation CreateTargetInformation()
	{
		auto info = new eAIItemHazardTargetInformation(this);
		info.m_MinDistance = 2.0;
		return info;
	}

	override bool Expansion_IsDanger()
	{
		return true;
	}
}
#endif
