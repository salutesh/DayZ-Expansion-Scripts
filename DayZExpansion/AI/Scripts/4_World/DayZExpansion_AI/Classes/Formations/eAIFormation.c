class eAIFormation
{
	static const float DIR_RECALC_DIST_SQ = 9.0;

	static autoptr TTypenameArray s_FormationTypes = {eAIFormationColumn, eAIFormationInvColumn, eAIFormationFile, eAIFormationInvFile, eAIFormationVee, eAIFormationInvVee, eAIFormationWall, eAIFormationCircle, eAIFormationCircleDot, eAIFormationStar, eAIFormationStarDot};

	private vector m_Transform[4];
	private float m_Scale;

	protected eAIGroup m_Group;

	private vector m_LastUpdatePosition;
	protected float m_Looseness;

	void eAIFormation(eAIGroup group = null)
	{
		m_Group = group;
		m_Scale = 1.0;

		UpdateTransform(vector.Zero, vector.Forward);
	}

	void SetGroup(eAIGroup group)
	{
		m_Group = group;
	}

	/**
	 * @brief Abstract function that returns the position in local space relative to the formation transformation at any given time.
	 */
	vector GetPosition(int member_no);

	/**
	 * @brief Returns the direction in local space the AI is supposed to look at when not in a fight
	 */
	vector GetDirection(int member_no)
	{
		return vector.Forward;
	}

	void SetLooseness(float looseness)
	{
		m_Looseness = looseness;
	}

	float GetLooseness()
	{
		return Math.RandomFloatInclusive(-m_Looseness, m_Looseness);
	}

	vector ToWorld(vector fs)
	{
		if (m_Group.Count() > 0 && m_Group.GetFormationLeader())
			m_Transform[3] = m_Group.GetFormationLeader().GetPosition();

		vector ws = fs.Multiply4(m_Transform);

		return ws;
	}

	void Update(float pDt)
	{
		if (!m_Group || !m_Group.GetFormationLeader())
			return;

		vector newPos = m_Group.GetFormationLeader().GetPosition();

		if (m_LastUpdatePosition == vector.Zero)
		{
			UpdateTransform(newPos, m_Group.GetFormationLeader().GetDirection());
			return;
		}

		vector dir = vector.Direction(m_LastUpdatePosition, newPos);
		float dist = dir.LengthSq();
		if (dist < DIR_RECALC_DIST_SQ)
		{
			return;
		}

		// Update the transformation matrix
		UpdateTransform(newPos, dir.Normalized());
	}

	void UpdateTransform(vector newPos, vector dir)
	{
		m_Transform[3] = newPos;
		m_Transform[2] = dir;
		m_Transform[1] = "0 1 0";
		m_Transform[0] = dir.Perpend();
		m_LastUpdatePosition = newPos;
	}

	void SetScale(float separation)
	{
		m_Scale = separation;
	}

	float GetScale()
	{
#ifdef DIAG_DEVELOPER
		m_Scale = GetExpansionSettings().GetAI().FormationScale;
#endif

		return m_Scale;
	}

	// Unused thus far
	void SetSize(int num_of_people) {};

	static typename GetType(string formationName)
	{
		if (formationName == "RANDOM")
			return s_FormationTypes.GetRandomElement();
		return ("eAIFormation" + formationName).ToType();
	}

	static eAIFormation Create(string formationName)
	{
		typename formation = GetType(formationName);
		if (formation)
			//! @note w/o the cast to eAIFormation, the compiler warns about unsafe downcasting.
			//! Of course the compiler is wrong, because we're casting up, not down, so this cast here is just there to satisfy compiler shortcomings.
			//! Yes I wrote this comment for the sole reason that I'm annoyed by this.
			return eAIFormation.Cast(formation.Spawn());
		else
			Error("Invalid formation name " + formationName);

		return new eAIFormationVee();
	}
};
