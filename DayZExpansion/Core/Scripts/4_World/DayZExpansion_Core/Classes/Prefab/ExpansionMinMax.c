class ExpansionMinMax
{
	float Min;
	float Max;

	void ExpansionMinMax(float min = 0, float max = 0)
	{
		Set(min, max);
	}

	void Set(float min, float max)
	{
		Min = min;
		Max = max;

		if (Max < Min)
			Max = Min;
	}

	float GetRandom()
	{
		if (Max <= 0)
		{
			return -1;
		}

		float random = Math.RandomFloatInclusive(Min, Max);
		if (random <= 0)
		{
			return 0;
		}

		return random;
	}
};

class ExpansionHealth: ExpansionMinMax
{
	string Zone;

	void ExpansionHealth(float min = 0, float max = 0, string zone = "")
	{
		Zone = zone;
	}
};
