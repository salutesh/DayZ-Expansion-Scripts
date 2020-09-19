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

	ref TStringArray UpperGear;			// Array of classnames used for the upper gear
	ref TStringArray PantsGear;			// Array of classnames used for the pants gear
	ref TStringArray BackpackGear;		// Array of classnames used for the backpack gear

	// ------------------------------------------------------------
	// ExpansionStartingGear Constructor
	// ------------------------------------------------------------
	void ExpansionStartingGear()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionStartingGear::ExpansionStartingGear - Start");
		#endif
		
		UpperGear = new TStringArray;
		PantsGear = new TStringArray;
		BackpackGear = new TStringArray;
		
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
		
		UseStartingGear = true;
		UsingUpperGear = true;
		UsingPantsGear = false;
		UsingBackpackGear = true;
		
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
	ref TStringArray Headgear;
	ref TStringArray Glasses;
	ref TStringArray Masks;
	ref TStringArray Tops;
	ref TStringArray Vests;
	ref TStringArray Gloves;
	ref TStringArray Pants;
	ref TStringArray Belts;
	ref TStringArray Shoes;
	ref TStringArray Armbands;
	ref TStringArray Backpacks;
	
	// ------------------------------------------------------------
	// ExpansionStartingClothing Constructor
	// ------------------------------------------------------------
	void ExpansionStartingClothing()
	{
		Headgear = new TStringArray;
		Glasses = new TStringArray;
		Masks = new TStringArray;
		Tops = new TStringArray;
		Vests = new TStringArray;
		Gloves = new TStringArray;
		Pants = new TStringArray;
		Belts = new TStringArray;
		Shoes = new TStringArray;
		Armbands = new TStringArray;
		Backpacks = new TStringArray;
	}
	
	// ------------------------------------------------------------
	// ExpansionStartingClothing Defaults
	// ------------------------------------------------------------
	void Defaults()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionStartingClothing::Defaults - Start");
		#endif
		
		Tops.Insert("TShirt_Green");
		
		Pants.Insert("TrackSuitPants_Black");
		Pants.Insert("TrackSuitPants_Red");
		Pants.Insert("TrackSuitPants_Green");
		
		Shoes.Insert("AthleticShoes_Blue");
		Shoes.Insert("AthleticShoes_Grey");
		Shoes.Insert("AthleticShoes_Brown");
		Shoes.Insert("AthleticShoes_Green");
		Shoes.Insert("AthleticShoes_Black");
		
		Backpacks.Insert("TaloonBag_Blue");
		Backpacks.Insert("TaloonBag_Green");
		Backpacks.Insert("TaloonBag_Orange");
		Backpacks.Insert("TaloonBag_Violet");
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionStartingClothing::Defaults - End");
		#endif
	}
}