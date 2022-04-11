class eAIFormation
{
	private const float m_DirRecalcDistSq = 9.0;

	private vector m_Transform[4];
	private float m_Scale;

	private eAIGroup m_Group;

	void eAIFormation(eAIGroup group)
	{
		m_Group = group;
		m_Scale = 1.0;
	}

	// Abstract function that returns the position in local space relative to the formation transformation at any given time.
	vector GetPosition(int member_no);

	vector ToWorld(vector fs)
	{
		if (m_Group.Count() > 0)
			m_Transform[3] = m_Group.GetLeader().GetPosition();

		vector ws = fs.Multiply4(m_Transform);

		return ws;
	}

	void Update(float pDt)
	{
		if (!m_Group.GetLeader())
			return;

		vector newPos = m_Group.GetLeader().GetPosition();
		vector oldPos = m_Transform[3];

		if (vector.DistanceSq(newPos, oldPos) < m_DirRecalcDistSq)
			return;

		// Update the transformation matrix
		m_Transform[3] = newPos;
		m_Transform[2] = vector.Direction(newPos, oldPos).Normalized();
		m_Transform[1] = "0 1 0";
		m_Transform[0] = m_Transform[2].Perpend();
	}

	void SetScale(float separation)
	{
		m_Scale = separation;
	}

	float GetScale()
	{
		return m_Scale;
	}

	// Unused thus far
	void SetSize(int num_of_people) {}
};
