modded class PluginAdminLog
{
	string m_Expansion_SurvivorDisplayName;

	void PluginAdminLog()
	{
		m_Expansion_SurvivorDisplayName = GetGame().ConfigGetTextOut(CFG_VEHICLESPATH + " SurvivorBase displayName");
	}

	override string GetPlayerPrefix( PlayerBase player, PlayerIdentity identity )  // player name + id + position prefix for log prints
	{	
		if (identity || !player.IsInherited(eAIBase))
			return super.GetPlayerPrefix(player, identity);

		m_Position = player.GetPosition();
		m_PosArray[3] = { m_Position[0].ToString(), m_Position[2].ToString(), m_Position[1].ToString() };
		
		for ( int i = 0; i < 3; i++ )	// trim position precision
		{
			m_DotIndex = m_PosArray[i].IndexOf(".");
			if ( m_DotIndex != -1 )
				m_PosArray[i] = m_PosArray[i].Substring( 0, m_DotIndex + 2 );
		}

		string name = player.GetDisplayName();
		if (name == m_Expansion_SurvivorDisplayName)
		{
			name = player.GetType();
			int index = ExpansionString.LastIndexOf(name, "_");
			if (index > -1)
				name = name.Substring(index + 1, name.Length() - index - 1);
		}

		eAIBase ai = eAIBase.Cast(player);

		return "AI \"" + name + "\" (group=\"" + ai.GetGroup().GetName() + "\" faction=\"" + ai.GetGroup().GetFaction().GetName() + "\" pos=<" +  m_PosArray[0] + ", " + m_PosArray[1] + ", " + m_PosArray[2] + ">)";
	}

	override void PlayerHitBy(TotalDamageResult damageResult, int damageType, PlayerBase player, EntityAI source, int component, string dmgZone, string ammo)
	{
		if (player.IsAI()) return;
		
		super.PlayerHitBy(damageResult, damageType, player, source, component, dmgZone, ammo);
	}

	override void PlayerKilled(PlayerBase player, Object source)
	{
		if (player.IsAI()) return;

		super.PlayerKilled(player, source);
	}
};
