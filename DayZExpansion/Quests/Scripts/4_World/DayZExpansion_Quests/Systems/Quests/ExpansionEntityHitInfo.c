/**
 * ExpansionEntityHitInfo.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2024 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionEntityHitInfo
{
	string m_Zone;
	string m_Source;
	float m_Damage;
	
	void ExpansionEntityHitInfo(string zone, string source, float damage)
	{
		Set(zone, source, damage);
	}

	void Set(string zone, string source, float damage)
	{
		m_Zone = zone;
		m_Source = source;
		m_Damage = damage;
	}

	string GetZone()
	{
		return m_Zone;
	}
	
	string GetSounce()
	{
		return m_Source;
	}
	
	float GetDamage()
	{
		return m_Damage;
	}
};
