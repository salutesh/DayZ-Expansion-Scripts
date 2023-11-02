/**
 * DayZExpansion.c
 * 
 * Partly based on Enfusion AI Project Copyright 2021 William Bowers
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class DayZExpansion
{
	private static DayZExpansion m_Instance_5; //! weak ref
	
    void DayZExpansion()
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0(this, "DayZExpansion");
		#endif

		m_Instance_5 = this;
    }

	static DayZExpansion Get5()
	{
		#ifdef EAI_TRACE
		auto trace = CF_Trace_0("DayZExpansion", "Get5");
		#endif
		
		return m_Instance_5;
	}
};
