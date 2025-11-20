class ExpansionNavmeshCarver: House
{
	protected Object m_Expansion_AssociatedObject;
	protected int m_Expansion_AssociatedObjectNetIdLow;
	protected int m_Expansion_AssociatedObjectNetIdHigh;

	void ExpansionNavmeshCarver()
	{
		RegisterNetSyncVariableInt("m_Expansion_AssociatedObjectNetIdLow");
		RegisterNetSyncVariableInt("m_Expansion_AssociatedObjectNetIdHigh");

		if (g_Game.IsServer())
			g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(g_Game.UpdatePathgraphRegionByObject, 100, false, this);
	}

	override void EEDelete(EntityAI parent)
	{
		super.EEDelete(parent);

		if (g_Game && g_Game.IsServer())
		{
			vector pos = GetPosition();
			vector minMax[2];
			float radius = ClippingInfo(minMax);
			vector min = Vector(pos[0] - radius, pos[1], pos[2] - radius);
			vector max = Vector(pos[0] + radius, pos[1], pos[2] + radius);
			g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(g_Game.UpdatePathgraphRegion, 100, false, min, max);
		}
	}

	void Expansion_SetAssociatedObject(Object obj)
	{
		EXTrace.Print(EXTrace.MISC, this, "Expansion_SetAssociatedObject - " + obj);

		m_Expansion_AssociatedObject = obj;

	#ifdef SERVER
		m_Expansion_AssociatedObject.GetNetworkID(m_Expansion_AssociatedObjectNetIdLow, m_Expansion_AssociatedObjectNetIdHigh);
		SetSynchDirty();
	#endif
	}

	Object Expansion_GetAssociatedObject()
	{
		return m_Expansion_AssociatedObject;
	}

	override bool IsBuilding()
	{
		return false;
	}

	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();

		if (!m_Expansion_AssociatedObject && (m_Expansion_AssociatedObjectNetIdLow != 0 || m_Expansion_AssociatedObjectNetIdHigh != 0))
			Expansion_SetAssociatedObject(g_Game.GetObjectByNetworkId(m_Expansion_AssociatedObjectNetIdLow, m_Expansion_AssociatedObjectNetIdHigh));
	}
}

class ExpansionLargeAnimalNavmeshCarver: ExpansionNavmeshCarver
{
	override void OnReceivedHit(ImpactEffectsData hitData)
	{
		hitData.m_Surface = "Hit_MeatBones";
	}
}
