modded class WorldData
{
	protected float m_AirDensity = 1.225;

	void WorldData()
	{
		g_Expansion_WorldData = this;
	}

	float GetDensity(vector position)
	{
		return m_AirDensity;
	}
};

static WorldData g_Expansion_WorldData;

static float Expansion_GetDensity(vector position)
{
	if (!g_Expansion_WorldData)
		return 0;

	return g_Expansion_WorldData.GetDensity(position);
}
