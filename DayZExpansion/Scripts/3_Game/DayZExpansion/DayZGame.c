/**
 * DayZGame.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class DayZGame
{
	void DayZGame()
	{
		//! Has to be here if we want to avoid storage load

		bool failsafe;
#ifndef EXPANSIONMODBASEBUILDING
		failsafe = true;
#endif
#ifndef EXPANSIONMODWEAPONS
		failsafe = true;
#endif
		if (failsafe)
		{
			FileHandle file = OpenFile( "$profile:EXPANSION_UPDATE_README.txt", FileMode.WRITE );
			FPrintln(file, "Dayz-Expansion was updated!");
			FPrintln(file, "The basebuilding system and weapons are now separate mods.");
			FPrintln(file, "Please install:");
			FPrintln(file, "@DayZ-Expansion-BaseBuilding");
			FPrintln(file, "@DayZ-Expansion-Weapons");
			FPrintln(file, "This requirement will be removed sometime after the release of DayZ 1.17");
			CloseFile(file);
			RequestExit(1);
		}
	}
}
