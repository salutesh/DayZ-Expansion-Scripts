modded class Ammunition_Base
{
	static ref map<string, float> s_Expansion_HealthDamage = new map<string, float>();
	float m_Expansion_HealthDamage;

	void Ammunition_Base()
	{
		string ammoType = GetType();

		if (!s_Expansion_HealthDamage.Find(ammoType, m_Expansion_HealthDamage))
		{
			string bulletType;
			if (g_Game.ConfigGetText(CFG_MAGAZINESPATH + " " + ammoType + " ammo", bulletType))
				m_Expansion_HealthDamage = g_Game.ConfigGetFloat(CFG_AMMO + " " + bulletType + " DamageApplied Health damage");

		#ifdef DIAG_DEVELOPER
			EXTrace.Print(EXTrace.AI, null, ammoType + " " + bulletType + " health dmg " + m_Expansion_HealthDamage);
		#endif

			s_Expansion_HealthDamage[ammoType] = m_Expansion_HealthDamage;
		}
	}

	override float Expansion_GetHealthDamage()
	{
		return m_Expansion_HealthDamage;
	}
}
