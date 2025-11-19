class eAIEntityTargetInformation: eAITargetInformation
{
	protected EntityAI m_Target;
	private string m_TargetDebugName;

	void eAIEntityTargetInformation(EntityAI target)
	{
		m_Target = target;
		m_TargetDebugName = "" + m_Target;  //! Useful for logging even after entity has been deleted
	}

	override string GetDebugName()
	{
		string str = super.GetDebugName();
		
		str += ", ";
		str += "entity=" + GetEntityDebugName();

		return str;
	}

	override EntityAI GetEntity()
	{
		return m_Target;
	}

	override IEntity GetParent()
	{
		return m_Target.GetParent();
	}

	override string GetEntityDebugName()
	{
		string str = m_TargetDebugName;

		if (m_Target)
		{
			str += ", ";
			str += "position=" + GetPosition();
		}
		else
		{
			str += "(deleted)";
		}
		
		return str;
	}

	override bool CanPutInCargo(EntityAI parent)
	{
		return m_Target.CanPutInCargo(parent);
	}

	override bool IsEntity()
	{
		return true;
	}

	override bool IsActive()
	{
		if (!m_Target)
			return false;

		return !m_Target.IsDamageDestroyed();
	}

	override vector GetPosition(eAIBase ai = null, bool actual = false, eAITargetInformationState state = null)
	{
		if (actual || !ai)
			return m_Target.GetPosition();

		bool created;

		if (!state)
			state = GetTargetForAIEx(ai, true, created);

		if (!created)
			state.UpdatePosition();

		return state.m_SearchPosition;
	}

	override float GetThreat(eAIBase ai = null, eAITargetInformationState state = null)
	{
		if (!IsActive())
			return 0.0;

		return super.GetThreat(ai, state);
	}

	override vector GetDirection(eAIBase ai, bool actual = false, eAITargetInformationState state = null)
	{
		vector position = GetPosition(ai, actual, state);
		return vector.Direction(ai.GetPosition(), position);
	}

	override float GetDistance(eAIBase ai, bool actual = false, eAITargetInformationState state = null)
	{
		vector position = GetPosition(ai, actual, state);
		return vector.Distance(ai.GetPosition(), position);
	}

	override float GetDistanceSq(eAIBase ai, bool actual = false, eAITargetInformationState state = null)
	{
		vector position = GetPosition(ai, actual, state);
		return vector.DistanceSq(ai.GetPosition(), position);
	}

	override void OnHit(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		eAIBase ai;
		if (source && Class.CastTo(ai, source.GetHierarchyRoot()))
			ai.m_eAI_HitObject = m_Target;
	}

	override void OnHealthLevelChanged(int oldLevel, int newLevel, string zone)
	{
		if (g_Game.IsClient() && m_Target.m_Initialized)
			GetDayZGame().GetExpansionGame().m_FirearmFXHitObject = m_Target;
	}
};

class eAIEntity<Class T>
{
	static eAIEntityTargetInformation GetTargetInformation(T entity)
	{
		return entity.GetTargetInformation();
	}
};
