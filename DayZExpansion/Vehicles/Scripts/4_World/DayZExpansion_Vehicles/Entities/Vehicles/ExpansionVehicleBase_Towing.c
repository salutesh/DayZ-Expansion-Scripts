class ExpansionVehicleBaseBase : Transport
{
	void Expansion_CreateTow(Object tow, int index)
	{
	}

	EntityAI Expansion_GetTowedEntity()
	{
		return NULL;
	}

	void Expansion_DestroyTow()
	{
	}

	bool Expansion_IsTowing()
	{
		return false;
	}

	int Expansion_NumberTowConnections()
	{
		return 0;
	}

	bool Expansion_GetOverlappingTowConnection(vector towPosition, float towRadius, out int index)
	{
		return false;
	}

	vector Expansion_GetTowPosition()
	{
		return vector.Zero;
	}

	vector Expansion_GetTowDirection()
	{
		return -GetDirection();
	}

	float Expansion_GetTowLength()
	{
		return 0.4;
	}
};
