/**
 * AnimalBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class AnimalBase
{
	override void EEKilled(Object killer)
	{
#ifdef DIAG
		auto trace = EXTrace.Start(EXTrace.MISC, this, "killer " + killer, "last dmg src " + m_Expansion_LastDamageSource);
#endif

		super.EEKilled(killer);

		//! Workaround for vanilla EEKilled returning the killed entity as killer since 1.20
		//! See VanillaFixes AnimalBase.c EEHitBy
		if (killer == this)
			killer = m_Expansion_LastDamageSource;

		auto settings = GetExpansionSettings().GetHardline();
		if  (settings.UseReputation)
			ExpansionHardlineModule.GetModuleInstance().OnEntityKilled(this, killer);
	}
};
