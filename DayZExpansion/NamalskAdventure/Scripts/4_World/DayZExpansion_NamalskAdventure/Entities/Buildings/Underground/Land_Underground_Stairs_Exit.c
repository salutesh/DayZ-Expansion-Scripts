/**
 * Land_Underground_Stairs_Exit.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class Land_Underground_Stairs_Exit
{
	void Land_Underground_Stairs_Exit()
	{
	#ifndef SERVER
		GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).Remove(CreateLights);
	#endif
	}

	override void CreateLights()
	{
		//! Don`t create any lights.
	}
};