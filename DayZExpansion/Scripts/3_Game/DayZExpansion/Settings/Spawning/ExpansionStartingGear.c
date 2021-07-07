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
	bool EnableStartingGear;			// If enabled, will use the starting gear configured in this file
	bool UseUpperGear;					// If enabled, use the upper gear configured
	bool UsePantsGear;					// If enabled, use the pants gear configured
	bool UseBackpackGear;				// If enabled, use the backpack gear configured
	bool UseVestGear;					// If enabled, use the vest gear configured
	
	bool UsePrimaryWeapon;				// If enabled, use the primary weapon configured
	bool UseSecondaryWeapon;			// If enabled, use the secondary weapon configured
	
	ref TStringArray UpperGear;			// Array of classnames used for the upper gear
	ref TStringArray PantsGear;			// Array of classnames used for the pants gear
	ref TStringArray BackpackGear;		// Array of classnames used for the backpack gear
	ref TStringArray VestGear;			// Array of classnames used for the vest gear
	
	string PrimaryWeapon;
	ref TStringArray PrimaryWeaponAttachments;
	
	string SecondaryWeapon;
	ref TStringArray SecondaryWeaponAttachments;
	
	bool ApplyEnergySources;			// If enabled this will apply batteries to all items that can fit/need one.
	bool SetRandomHealth;				// If enabled this will add ramdom damage to all items that the player has.
	
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
		VestGear = new TStringArray;
		
		PrimaryWeaponAttachments = new TStringArray;
		SecondaryWeaponAttachments = new TStringArray;
		
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
		
		EnableStartingGear = true;
		UseUpperGear = true;
		UsePantsGear = false;
		UseBackpackGear = true;
		UseVestGear = false;
		UsePrimaryWeapon = false;
		UseSecondaryWeapon = false;
		
		ApplyEnergySources = true;
		SetRandomHealth = true;
		
		UpperGear.Insert("Rag");
		UpperGear.Insert("StoneKnife");
		UpperGear.Insert("Apple");

		BackpackGear.Insert("SpaghettiCan");
		BackpackGear.Insert("BakedBeansCan");
		BackpackGear.Insert("Chemlight_White");
	
	#ifdef EXPANSIONMODMARKET
		BackpackGear.Insert("ExpansionGoldBar");
		BackpackGear.Insert("ExpansionGoldBar");
		BackpackGear.Insert("ExpansionGoldBar");
	#endif
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionStartingGear::Defaults - End");
		#endif
	}
};


class ExpansionStartingClothing
{
	bool EnableCustomClothing;
	bool SetRandomHealth;
	
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
		
		EnableCustomClothing = true;
		SetRandomHealth = true;
		
		Tops.Insert("TShirt_Green");
		Tops.Insert("TShirt_Blue");
		Tops.Insert("TShirt_Black");
		Tops.Insert("TShirt_Beige");
		Tops.Insert("TShirt_Red");
		Tops.Insert("TShirt_OrangeWhiteStripes");
		Tops.Insert("TShirt_White");
		Tops.Insert("TShirt_Red");
		Tops.Insert("TShirt_Grey");
		Tops.Insert("TShirt_RedBlackStripes");
		
		Pants.Insert("CanvasPants_Beige");
		Pants.Insert("CanvasPants_Blue");
		Pants.Insert("CanvasPants_Grey");
		Pants.Insert("CanvasPants_Red");
		Pants.Insert("CanvasPants_Violet");
		Pants.Insert("CanvasPantsMidi_Beige");
		Pants.Insert("CanvasPantsMidi_Blue");
		Pants.Insert("CanvasPantsMidi_Grey");
		Pants.Insert("CanvasPantsMidi_Red");
		Pants.Insert("CanvasPantsMidi_Violet");
		
		Shoes.Insert("AthleticShoes_Blue");
		Shoes.Insert("AthleticShoes_Grey");
		Shoes.Insert("AthleticShoes_Brown");
		Shoes.Insert("AthleticShoes_Green");
		Shoes.Insert("AthleticShoes_Black");
		
		Backpacks.Insert("TaloonBag_Blue");
		Backpacks.Insert("TaloonBag_Green");
		Backpacks.Insert("TaloonBag_Orange");
		Backpacks.Insert("TaloonBag_Violet");
		
		/*
		Vests.Insert("HighCapacityVest_Black");
		Vests.Insert("HighCapacityVest_Olive");
		*/
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionStartingClothing::Defaults - End");
		#endif
	}
};