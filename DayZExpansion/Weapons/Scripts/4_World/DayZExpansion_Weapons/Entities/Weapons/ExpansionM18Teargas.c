/**
 * ExpansionM18Teargas.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class Expansion_M18SmokeGrenade_Teargas: M18SmokeGrenade_White
{
	protected ref ExpansionTeargasHelper m_ExpansionTeargasHelper;

	void Expansion_M18SmokeGrenade_Teargas()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.WEAPONS, this, "Expansion_M18SmokeGrenade_Teargas");
#endif

		m_ExpansionTeargasHelper = new ExpansionTeargasHelper( this );
	}

	override void OnWorkStart()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.WEAPONS, this, "OnWorkStart");
#endif

		super.OnWorkStart();

		m_ExpansionTeargasHelper.OnWorkStart();
	}

	override void OnWorkStop()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.WEAPONS, this, "OnWorkStop");
#endif

		super.OnWorkStop();

		m_ExpansionTeargasHelper.OnWorkStop();
	}
};
