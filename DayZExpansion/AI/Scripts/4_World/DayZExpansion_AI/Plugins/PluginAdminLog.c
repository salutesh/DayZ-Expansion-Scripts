modded class PluginAdminLog
{
	string m_Expansion_SurvivorDisplayName;
	bool m_eAI_LogAIHitBy;
	bool m_eAI_LogAIKilled;

	void PluginAdminLog()
	{
		m_Expansion_SurvivorDisplayName = GetGame().ConfigGetTextOut(CFG_VEHICLESPATH + " SurvivorBase displayName");
	}

	override void OnInit()
	{
		super.OnInit();

		//! Expansion settings get initialized in DayZExpansion constructor, which gets created in MissionBase constructor.
		//! Vanilla plugin init also happens in MissionBase constructor, so we have to delay accessing Expansion settings to next frame
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(eAI_OnAfterInit);
	}

	void eAI_OnAfterInit()
	{
		if (GetGame().IsServer())
		{
			auto settings = GetExpansionSettings().GetAI();
			m_eAI_LogAIHitBy = settings.LogAIHitBy;
			m_eAI_LogAIKilled = settings.LogAIKilled;
		}
	}

	override string GetPlayerPrefix(PlayerBase player, PlayerIdentity identity)
	{
		string playerPrefix = super.GetPlayerPrefix(player, identity);

		if (player.IsAI())
		{
			//! Replace "Player" with "AI"
			playerPrefix = "AI" + playerPrefix.Substring(6, playerPrefix.Length() - 6);
		}

		eAIGroup group = player.GetGroup();

		if (group)
		{
			string groupInfo = "group=" + group.GetID();
			string groupName = group.GetName();

			if (groupName)
				groupInfo += ":\"" + groupName + "\"";

			groupInfo += " faction=\"" + group.GetFaction().GetName() + "\"";

			if (player.IsAI())
				playerPrefix.Replace("id=", groupInfo);
			else
				playerPrefix.Replace("pos=", groupInfo + " pos=");
		}

		return playerPrefix;
	}

	override void PlayerHitBy(TotalDamageResult damageResult, int damageType, PlayerBase player, EntityAI source, int component, string dmgZone, string ammo)
	{
		if (player.IsAI() && !m_eAI_LogAIHitBy) return;
		
		super.PlayerHitBy(damageResult, damageType, player, source, component, dmgZone, ammo);
	}

	override void PlayerKilled(PlayerBase player, Object source)
	{
		if (player.IsAI() && !m_eAI_LogAIKilled) return;

		super.PlayerKilled(player, source);
	}
};
