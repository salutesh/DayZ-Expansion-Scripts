/**
 * ExpansionM18Teargas.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class Expansion_M18SmokeGrenade_Teargas : M18SmokeGrenade_White
{
	protected ref ExpansionTeargasHelper m_ExpansionTeargasHelper;

	void Expansion_M18SmokeGrenade_Teargas()
	{
		m_ExpansionTeargasHelper = new ExpansionTeargasHelper( this );
	}

	override void OnWorkStart()
	{
		super.OnWorkStart();

		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_M18SmokeGrenade_Teargas::OnWorkStart Start");
		#endif

		m_ExpansionTeargasHelper.OnWorkStart();

		#ifdef EXPANSIONEXPRINT
		EXPrint("Expansion_M18SmokeGrenade_Teargas::OnWorkStart End");
		#endif
	}

	override void OnWorkStop()
	{
		super.OnWorkStop();

		m_ExpansionTeargasHelper.OnWorkStop();
	}
}