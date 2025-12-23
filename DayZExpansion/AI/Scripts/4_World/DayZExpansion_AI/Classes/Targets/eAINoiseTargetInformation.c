class eAINoiseTargetInformation: eAITargetInformation
{
	protected EntityAI m_Source;
	protected vector m_Position;
	protected float m_Strength;
	protected float m_Lifetime = -1.0;
	protected float m_ThreatLevel;

	override string GetDebugName()
	{
		string str = super.GetDebugName();

		str += ", source=" + m_Source;
		str += ", position=" + m_Position;

		return str;
	}

	override bool IsNoise()
	{
		return true;
	}

	void SetNoiseParams(EntityAI source, vector position, float strength, float lifetime, float threatLevel)
	{
		m_Source = source;
		m_Position = position;
		m_Strength = strength;
		m_Lifetime = lifetime;
		m_ThreatLevel = threatLevel;
	}

	EntityAI GetSource()
	{
		return m_Source;
	}

	eAINoiseType GetNoiseType()
	{
		if (m_Source)
		{
			if (m_Source.IsWeapon())
				return eAINoiseType.SHOT;
			else if (m_Source.IsInherited(ExplosivesBase))
				return eAINoiseType.EXPLOSION;
		}

		return eAINoiseType.SOUND;
	}

	float GetStrength()
	{
		return m_Strength;
	}

	override float GetLifetime()
	{
		return m_Lifetime;
	}

	override float CalculateThreat(eAIBase ai = null, eAITargetInformationState state = null)
	{
		return m_ThreatLevel;
	}

	override vector GetPosition(eAIBase ai = null, bool actual = false, eAITargetInformationState state = null)
	{
		if (actual || !ai)
			return m_Position;

		bool created;

		if (!state)
			state = GetTargetForAIEx(ai, true, created);

		if (!created)
			state.UpdatePosition();

		return state.m_SearchPosition;
	}

	override vector GetDirection(eAIBase ai, bool actual = false, eAITargetInformationState state = null)
	{
		return vector.Direction(ai.GetPosition(), m_Position);
	}

	override float GetDistance(eAIBase ai, bool actual = false, eAITargetInformationState state = null)
	{
		return vector.Distance(ai.GetPosition(), m_Position);
	}

	override float GetDistanceSq(eAIBase ai, bool actual = false, eAITargetInformationState state = null)
	{
		return vector.DistanceSq(ai.GetPosition(), m_Position);
	}

	override bool ShouldRemove(eAIBase ai = null)
	{
		//! Will remove when lifetime is up
		return true;
	}

	override vector GetAimOffset(eAIBase ai = null)
	{
		vector pos;
		Human player;
		if (m_Source && Class.CastTo(player, m_Source.GetHierarchyRoot()))
		{
			pos = player.GetBonePositionWS(player.GetBoneIndexByName("neck"));
			pos = pos - player.GetPosition();
		}
		return pos;
	}
};
