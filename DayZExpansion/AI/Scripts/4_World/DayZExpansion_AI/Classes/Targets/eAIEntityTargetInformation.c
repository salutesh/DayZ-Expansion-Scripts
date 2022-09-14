class eAIEntityTargetInformation extends eAITargetInformation
{
	private EntityAI m_Target;
	vector m_LastKnownPosition;

	void eAIEntityTargetInformation(EntityAI target)
	{
		m_Target = target;
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DeferredInit, 34);
	}

	void DeferredInit()
	{
		m_LastKnownPosition = m_Target.GetPosition();
	}

	override string GetDebugName()
	{
		string str = super.GetDebugName();
		
		str += ", ";
		str += "target=" + Object.GetDebugName(m_Target);

		return str;
	}

	override EntityAI GetEntity()
	{
		return m_Target;
	}

	override bool IsActive()
	{
		if (!m_Target)
			return false;

		return m_Target.GetHealth() > 0.0;
	}

	override vector GetPosition(eAIBase ai = null, bool actual = false)
	{
		if (actual)
			return m_Target.GetPosition();

		if (!ai || ai.eAI_HasLOS())
			m_LastKnownPosition = m_Target.GetPosition();

		return m_LastKnownPosition;
	}

	override float GetDistance(eAIBase ai, bool actual = false)
	{
		vector position = GetPosition(ai, actual);
		return vector.Distance(ai.GetPosition(), position);
	}

	override float GetDistanceSq(eAIBase ai, bool actual = false)
	{
		vector position = GetPosition(ai, actual);
		return vector.DistanceSq(ai.GetPosition(), position);
	}
};

class eAIEntity<Class T>
{
	static eAITargetInformation GetTargetInformation(T entity)
	{
		return entity.GetTargetInformation();
	}
};
