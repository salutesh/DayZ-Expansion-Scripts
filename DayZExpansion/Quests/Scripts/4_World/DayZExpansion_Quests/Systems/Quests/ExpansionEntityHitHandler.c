class ExpansionEntityHitHandler
{
	protected EntityAI m_Entity;
	protected ref map<Man, ref ExpansionEntityHitInfo> m_HitMap = new map<Man, ref ExpansionEntityHitInfo>;

	void ExpansionEntityHitHandler(EntityAI entity)
	{
		m_Entity = entity;
	}
	
	void ~ExpansionEntityHitHandler()
	{
		if (m_HitMap)
			m_HitMap.Clear();
	}

	map<Man, ref ExpansionEntityHitInfo> GetHitMap()
	{
		return m_HitMap;
	}

	void OnHit(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		Man sourcePlayer;

		if (source)  //! Source can be null if actual source is not an EntityAI but (e.g.) a static object during a vehicle collision
			sourcePlayer = source.GetHierarchyRootPlayer();
		
		if (sourcePlayer)
		{
			float damage = damageResult.GetDamage("", "");
			string sourceType = source.GetType();

		#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
			string sourceUID;
			if (sourcePlayer.GetIdentity())
				sourceUID = sourcePlayer.GetIdentity().GetId();
			EXTrace.Print(EXTrace.QUESTS, m_Entity, "ExpansionEntityHitHandler::OnHit - Entity hit by player! Player UID: " + sourceUID + " | Source: " + sourceType + " | Damage: " + damage.ToString() + " | Zone: " + dmgZone);
		#endif
			
			ExpansionEntityHitInfo hitMap;
			if (m_HitMap.Find(sourcePlayer, hitMap))
			{
				hitMap.Set(dmgZone, sourceType, damage);
			}
			else
			{
				hitMap = new ExpansionEntityHitInfo(dmgZone, sourceType, damage);
				m_HitMap.Insert(sourcePlayer, hitMap);
			}
		}
	}

	bool WasHitBy(Man player)
	{
		return m_HitMap.Contains(player);
	}
}
