/**
 * eAIFactionInvincibleSurvivors.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
 */

#ifdef EXPANSIONMODAI
[eAIRegisterFaction(eAIFactionInvincibleSurvivors)]
class eAIFactionInvincibleSurvivors : eAIFactionInvincibleGuards
{
	void eAIFactionInvincibleSurvivors()
	{
		m_Loadout = "SurvivorLoadout";
	}

	override bool IsFriendly(notnull eAIFaction other)
	{
		if (super.IsFriendly(other)) return true;
		if (other.IsInherited(eAIFactionInvincibleSurvivors)) return true;
		if (other.IsInherited(eAIFactionScienceSociety)) return true;
		if (other.IsInherited(eAIFactionSurvivors)) return true;
		if (other.IsInherited(eAIFactionResistance)) return true;
		if (other.IsInherited(eAIFactionPassive)) return true;
		return false;
	}
	
	override string GetDisplayName()
	{
		return "#STR_EXPANSION_AI_FACTION_SURVIVORS";
	}
};
#endif
