/**
 * eAIFactionNekrologists.c
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
[eAIRegisterFaction(eAIFactionNekrologists)]
class eAIFactionNekrologists : eAIFaction
{
	void eAIFactionNekrologists()
	{
		m_Loadout = "Namalsk_Nekrologists";
	}

	override bool IsFriendlyFaction(notnull eAIFaction other)
	{
		if (super.IsFriendlyFaction(other)) return true;
		if (other.IsInherited(eAIFactionNekrologists)) return true;
		return false;
	}
};
#endif
