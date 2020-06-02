/**
 * ExpansionStartingGear.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionStartingGear
 * @brief		Gear settings class
 **/
class ExpansionStartingGear
{
	bool UseStartingGear
	bool UsingUpperGear;
	bool UsingPantsGear;
	bool UsingBackpackGear;

	ref array<string> SpawnBackpacks;
	ref array<string> UpperGear;
	ref array<string> PantsGear;
	ref array<string> BackpackGear;

    // ------------------------------------------------------------
	// Expansion ExpansionStartingGear
	// ------------------------------------------------------------
	void ExpansionStartingGear()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionStartingGear::ExpansionStartingGear - Start");
		#endif
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionStartingGear::ExpansionStartingGear - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
    void Defaults()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionStartingGear::Defaults - Start");
		#endif
		
		SpawnBackpacks = new array<string>;
		UpperGear = new array<string>;
		PantsGear = new array<string>;
		BackpackGear = new array<string>;
		
		UseStartingGear = true;
        UsingUpperGear = true;
        UsingPantsGear = false;
		UsingBackpackGear = true;

		SpawnBackpacks.Insert("TaloonBag_Blue");
		SpawnBackpacks.Insert("TaloonBag_Green");
		SpawnBackpacks.Insert("TaloonBag_Orange");
		SpawnBackpacks.Insert("TaloonBag_Violet");
		
        UpperGear.Insert("Rag");
        UpperGear.Insert("Chemlight_White");
        UpperGear.Insert("StoneKnife");
		UpperGear.Insert("Apple");

		BackpackGear.Insert("SpaghettiCan");
		BackpackGear.Insert("BakedBeansCan");
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionStartingGear::Defaults - End");
		#endif
	}
}
