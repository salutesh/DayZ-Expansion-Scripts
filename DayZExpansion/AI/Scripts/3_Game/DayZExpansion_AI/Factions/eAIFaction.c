class eAIFaction
{
	protected string m_Name;  //! DEPRECATED
	protected string m_Loadout = "HumanLoadout";
	protected bool m_IsGuard;
	protected bool m_IsInvincible;
	protected bool m_IsPassive;
	protected bool m_IsObserver;
	protected ref TIntArray m_Modifiers;

	void eAIFaction()
	{
		typename type = Type();
		if (!eAIRegisterFaction.s_FactionIDs[type])
			Error("Faction type " + type + " is not registered! Please prepend [eAIRegisterFaction(" + type + ")]");
	}

	int GetTypeID()
	{
		typename type = Type();
		return eAIRegisterFaction.s_FactionIDs[type];
	}

	static typename GetTypeByID(int typeID)
	{
		return eAIRegisterFaction.s_FactionTypes[typeID];
	}

	string GetName()
	{
		string cls = Type().ToString();
		return cls.Substring(10, cls.Length() - 10);
	}

	string GetDisplayName()
	{
		string name = GetName();
		name.ToUpper();
		return "#STR_EXPANSION_AI_FACTION_" + name;
	}

	string GetDefaultLoadout()
	{
		return m_Loadout;
	}

	void AddModifier(int modifier)
	{
		if (!m_Modifiers)
			m_Modifiers = {};
		m_Modifiers.Insert(modifier);
	}

	TIntArray GetModifiers()
	{
    	return m_Modifiers;
	}

	bool HasModifiers() 
	{
		return m_Modifiers && m_Modifiers.Count() > 0;
	}
	
	bool IsFriendly(notnull eAIFaction other)
	{
		return false;
	}

	bool IsFriendly(EntityAI other, DayZPlayer factionMember)
	{
		return IsFriendly(other);  //! Fallback for legacy
	}

	bool IsFriendly(EntityAI other)
	{
		return false;
	}

	bool IsGuard()
	{
		return m_IsGuard;
	}

	bool IsInvincible()
	{
		return m_IsInvincible;
	}

	bool IsPassive()
	{
		return m_IsPassive;
	}

	bool IsObserver()
	{
		return m_IsObserver;
	}

	static typename GetType(string factionName)
	{
		return ("eAIFaction" + factionName).ToType();
	}

	static eAIFaction Create(string factionName)
	{
		typename faction = GetType(factionName);
		if (faction)
			//! @note w/o the cast to eAIFaction, the compiler warns about unsafe downcasting.
			//! Of course the compiler is wrong, because we're casting up, not down, so this cast here is just there to satisfy compiler shortcomings.
			//! Yes I wrote this comment for the sole reason that I'm annoyed by this.
			return eAIFaction.Cast(faction.Spawn());
		else
			Error("Invalid faction name " + factionName);

		return null;
	}
};
