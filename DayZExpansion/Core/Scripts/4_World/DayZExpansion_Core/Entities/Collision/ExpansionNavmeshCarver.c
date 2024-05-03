class ExpansionNavmeshCarver: House
{
	protected Object m_Expansion_Object;

	void ExpansionNavmeshCarver()
	{
		if (GetGame().IsServer())
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(GetGame().UpdatePathgraphRegionByObject, 100, false, this);
	}

	override void EEDelete(EntityAI parent)
	{
		super.EEDelete(parent);

		if (GetGame() && GetGame().IsServer())
		{
			vector pos = GetPosition();
			vector minMax[2];
			float radius = ClippingInfo(minMax);
			vector min = Vector(pos[0] - radius, pos[1], pos[2] - radius);
			vector max = Vector(pos[0] + radius, pos[1], pos[2] + radius);
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(GetGame().UpdatePathgraphRegion, 100, false, min, max);
		}
	}

	void Expansion_SetObject(Object obj)
	{
		m_Expansion_Object = obj;
	}

	Object Expansion_GetObject()
	{
		return m_Expansion_Object;
	}

	override bool IsBuilding()
	{
		return false;
	}
}

class ExpansionLargeAnimalNavmeshCarver: ExpansionNavmeshCarver
{
	override void OnReceivedHit(ImpactEffectsData hitData)
	{
		hitData.m_Surface = "Hit_MeatBones";
	}
}
