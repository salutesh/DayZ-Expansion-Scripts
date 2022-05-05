/**
 * DayzExpansion.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		DayZExpansion
 * @brief
 **/
modded class DayZExpansion
{
	void DayZExpansion()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SPAWNSELECTION, this, "DayZExpansion");
#endif
	}

	void ~DayZExpansion()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SPAWNSELECTION, this, "~DayZExpansion");
#endif

	}

	override void OnLoaded()
	{
		if ( IsMissionHost() )
		{
			ExpansionSettings.SI_Spawn.Invoke();
		}

		super.OnLoaded();
	}

	override void OnFinish()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SPAWNSELECTION, this, "OnFinish");
#endif

		super.OnFinish();
	}
};
