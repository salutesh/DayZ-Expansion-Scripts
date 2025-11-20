class ExpansionBakedMapObject : House
{
	float m_ImpulseRequired;

	void ExpansionBakedMapObject()
	{
		m_ImpulseRequired = 100.0;
	}

	void Create(EntityAI other, vector position, Contact data)
	{
		if (data.Impulse < m_ImpulseRequired)
			return;

		ItemBase.ExpansionPhaseObject(other);

		if (g_Game.IsClient())
			return;

		Object object = g_Game.CreateObjectEx(ExpansionGetPhysicsType(), GetPosition(), ECE_IN_INVENTORY);

		ExpansionPhysicsStructure structure;
		if (Class.CastTo(structure, object))
		{
			structure.Create(this, other, position, data);
		}
	}

	string ExpansionGetPhysicsType()
	{
		return "ExpansionPhysics_" + GetType();
	}
};
