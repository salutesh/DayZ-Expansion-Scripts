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
	bool UseStartingGear				// If enabled, will use the starting gear configured in this file
	bool UsingUpperGear;				// If enabled, use the upper gear configured
	bool UsingPantsGear;				// If enabled, use the pants gear configured
	bool UsingBackpackGear;				// If enabled, use the backpack gear configured

	ref array<string> SpawnBackpacks;	// Array of classnames used for the items spawning inside the backpack
	ref array<string> UpperGear;		// Array of classnames used for the upper gear
	ref array<string> PantsGear;		// Array of classnames used for the pants gear
	ref array<string> BackpackGear;		// Array of classnames used for the backpack gear

	// ------------------------------------------------------------
	// ExpansionStartingGear Constructor
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
	// ExpansionStartingGear Defaults
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


class ExpansionStartingClothing
{
	ref array<string> Headgear;
	ref array<string> Glasses;
	ref array<string> Masks;
	ref array<string> Tops;
	ref array<string> Vests;
	ref array<string> Gloves;
	ref array<string> Pants;
	ref array<string> Belts;
	ref array<string> Shoes;
	ref array<string> Armbands;
	
	// ------------------------------------------------------------
	// ExpansionStartingClothing Defaults
	// ------------------------------------------------------------
	void Defaults()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionStartingClothing::Defaults - Start");
		#endif
		
		Headgear = new array<string>;
		Glasses = new array<string>;
		Masks = new array<string>;
		Tops = new array<string>;
		Vests = new array<string>;
		Gloves = new array<string>;
		Pants = new array<string>;
		Belts = new array<string>;
		Shoes = new array<string>;
		Armbands = new array<string>;
		
		Tops.Insert("ExpansionTee");
		
		Pants.Insert("TrackSuitPants_Black");
		Pants.Insert("TrackSuitPants_Red");
		Pants.Insert("TrackSuitPants_Green");
		
		Shoes.Insert("AthleticShoes_Blue");
		Shoes.Insert("AthleticShoes_Grey");
		Shoes.Insert("AthleticShoes_Brown");
		Shoes.Insert("AthleticShoes_Green");
		Shoes.Insert("AthleticShoes_Black");
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionStartingClothing::Defaults - End");
		#endif
	}
}