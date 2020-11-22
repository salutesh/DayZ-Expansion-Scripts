/**
 * DayzExpansion.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		DayZExpansion
 * @brief		
 **/
class DayZExpansion: ExpansionWorld
{

};

static ref DayZExpansion g_exDayZ;

static ref DayZExpansion GetDayZExpansion()
{
	return g_exDayZ;
}

static void CreateDayZExpansion()
{
	#ifdef EXPANSIONEXPRINT
	EXPrint("CreateDayZExpansion - Start");
	#endif
	
	if ( g_exDayZ )
	{
		DestroyDayZExpansion();
	}

	g_exDayZ = new DayZExpansion;

	GetDayZGame().SetExpansionGame( g_exDayZ );
	
	#ifdef EXPANSIONEXPRINT
	EXPrint("CreateDayZExpansion - End");
	#endif
}

static void DestroyDayZExpansion()
{
	#ifdef EXPANSIONEXPRINT
	EXPrint("DestroyDayZExpansion - Start");
	#endif
	
	delete g_exDayZ;
	
	#ifdef EXPANSIONEXPRINT
	EXPrint("DestroyDayZExpansion - End");
	#endif
}