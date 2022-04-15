class eAIGroupTargetInformation extends eAITargetInformation
{
	private eAIGroup m_Target;

	void eAIGroupTargetInformation(eAIGroup target)
	{
		m_Target = target;
	}

	override EntityAI GetEntity()
	{
		return m_Target.GetLeader();
	}

	override vector GetPosition(eAIBase ai = null)
	{
		if (!GetEntity())
			return "0 0 0";

		return GetEntity().GetPosition();
	}
};
