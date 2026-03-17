class ExpansionRollingAverage
{
	protected ref TFloatArray m_Values = {};
	protected int m_WindowSize;
	protected bool m_RollingMinMax;
	protected int m_Index;
	protected int m_Count;
	protected float m_Sum;
    protected float m_Min = float.MAX;
    protected float m_Max = -float.MAX;

	/**
	 * @brief rolling average
	 *
	 * @param windowSize
	 * @param rollingMinMax  rolling min/max (slow!)
	 */
	void ExpansionRollingAverage(int windowSize, bool rollingMinMax = false)
	{
		Resize(windowSize);
		SetRollingMinMax(rollingMinMax);
	}

	int GetWindowSize()
	{
		return m_WindowSize;
	}

	void SetRollingMinMax(bool rollingMinMax)
	{
		m_RollingMinMax = rollingMinMax;
	}

	bool GetRollingMinMax()
	{
		return m_RollingMinMax;
	}

	void Add(float value)
	{
		float oldestValue = m_Values[m_Index];

		if (m_Count == m_WindowSize)
			m_Sum -= oldestValue;
		else
			++m_Count;

		m_Values[m_Index] = value;
		m_Sum += value;

		++m_Index;
		if (m_Index == m_WindowSize)
			m_Index = 0;

		if (value < m_Min)
			m_Min = value;
		if (value > m_Max)
			m_Max = value;

		if (m_RollingMinMax && m_Count == m_WindowSize && value != oldestValue && (oldestValue == m_Min || oldestValue == m_Max))
			FindMinMax(m_Min, m_Max);
	}

	float Get()
	{
		if (m_Count == 0)
			return 0.0;

		return m_Sum / m_Count;
	}

	float GetMin()
	{
		return m_Min;
	}

	float GetMax()
	{
		return m_Max;
	}

	float FindMin()
	{
		float min = float.MAX;

		for (int i = 0; i < m_Count; ++i)
		{
			float value = m_Values[i];
			if (value < min)
				min = value;
		}

		return min;
	}

	float FindMax()
	{
		float max = -float.MAX;

		for (int i = 0; i < m_Count; ++i)
		{
			float value = m_Values[i];
			if (value > max)
				max = value;
		}

		return max;
	}

	void FindMinMax(out float min, out float max)
	{
		min = float.MAX;
		max = -float.MAX;

		for (int i = 0; i < m_Count; ++i)
		{
			float value = m_Values[i];
			if (value < min)
				min = value;
			if (value > max)
				max = value;
		}
	}

	void UpdateMinMax()
	{
		FindMinMax(m_Min, m_Max);
	}

	int Count()
	{
		return m_Count;
	}

	void Reset()
	{
		m_Index = 0;
		m_Count = 0;
		m_Sum = 0.0;

		for (int i = 0; i < m_Count; ++i)
		{
			m_Values[i] = 0.0;
		}
	}

	void Resize(int windowSize)
	{
		if (windowSize <= 1)
		{
			Error("Invalid window size " + windowSize);
			return;
		}

		if (m_Count > windowSize)
		{
			bool findMinMax;

			for (int i = windowSize; i < m_Count; ++i)
			{
				float value = m_Values[i];

				m_Sum -= value;

				if (m_RollingMinMax && (value == m_Min || value == m_Max))
					findMinMax = true;
			}

			m_Count = windowSize;

			if (findMinMax)
				FindMinMax(m_Min, m_Max);
		}

		m_Values.Resize(windowSize);
		m_WindowSize = windowSize;

		if (m_Index >= windowSize)
			m_Index = 0;
	}
}
