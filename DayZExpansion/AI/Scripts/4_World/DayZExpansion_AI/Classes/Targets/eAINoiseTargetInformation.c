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

	void SetNoiseParams(EntityAI source, vector position, float strength, float lifetime, float threatLevel)
	{
		m_Source = source;
		m_Position = position;
		m_Strength = strength;
		m_Lifetime = lifetime;
		m_ThreatLevel = threatLevel;
	}

	float GetStrength()
	{
		return m_Strength;
	}

	float GetLifetime()
	{
		return m_Lifetime;
	}

	override float CalculateThreat(eAIBase ai = null)
	{
		return m_ThreatLevel;
	}

	override vector GetPosition(eAIBase ai = null, bool actual = false)
	{
		if (actual || !ai)
			return m_Position;

		auto state = ai.eAI_GetTargetInformationState(this);
		state.UpdatePosition();

		return state.m_SearchPosition;
	}

	override vector GetDirection(eAIBase ai, bool actual = false)
	{
		return vector.Direction(ai.GetPosition(), m_Position);
	}

	override float GetDistance(eAIBase ai, bool actual = false)
	{
		return vector.Distance(ai.GetPosition(), m_Position);
	}

	override float GetDistanceSq(eAIBase ai, bool actual = false)
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
