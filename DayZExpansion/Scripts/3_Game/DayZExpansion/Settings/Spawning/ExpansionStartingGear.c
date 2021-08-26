/**
 * ExpansionStartingGear.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionStartingGearItem
 * @brief		Gear item settings class
 **/
class ExpansionStartingGearItem
{
	string ClassName;
	int Quantity;
	autoptr array<string> Attachments = new array<string>;
	
	// ------------------------------------------------------------
	// ExpansionStartingGearItem Constructor
	// ------------------------------------------------------------
	void ExpansionStartingGearItem(string className, int quantity, array<string> attachments = null)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionStartingGearItem::ExpansionStartingGearItem - Start");
		#endif
		
		ClassName = className;
		Quantity = quantity;
		
		if (attachments)
		{
			foreach (string attachment : attachments)
			{
				Attachments.Insert(attachment);
			}
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionStartingGearItem::ExpansionStartingGearItem - End");
		#endif
	}
}

/**@class		ExpansionStartingGearBase
 * @brief		Gear settings base class
 **/
class ExpansionStartingGearBase
{
	bool EnableStartingGear;				// If enabled, will use the starting gear configured in this file
	bool UseUpperGear;						// If enabled, use the upper gear configured
	bool UsePantsGear;						// If enabled, use the pants gear configured
	bool UseBackpackGear;					// If enabled, use the backpack gear configured
	bool UseVestGear;							// If enabled, use the vest gear configured
	bool UsePrimaryWeapon;				// If enabled, use the primary weapon configured
	bool UseSecondaryWeapon;			// If enabled, use the secondary weapon configured
	bool ApplyEnergySources;				// If enabled this will apply batteries to all items that can fit/need one.
	bool SetRandomHealth;					// If enabled this will add ramdom damage to all items that the player has.
	
	// ------------------------------------------------------------
	// ExpansionStartingGearBase Defaults
	// ------------------------------------------------------------
	void Defaults()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionStartingGearBase::~Defaults - Start");
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
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionStartingGearBase::~Defaults - End");
		#endif
	}
}

/**@class		ExpansionStartingGearV1
 * @brief		Gear settings version 1 class
 **/
class ExpansionStartingGearV1: ExpansionStartingGearBase
{
	ref TStringArray UpperGear = new TStringArray;				// Array of classnames used for the upper gear
	ref TStringArray PantsGear = new TStringArray;					// Array of classnames used for the pants gear
	ref TStringArray BackpackGear = new TStringArray;			// Array of classnames used for the backpack gear
	ref TStringArray VestGear = new TStringArray;					// Array of classnames used for the vest gear
	
	string PrimaryWeapon;
	ref TStringArray PrimaryWeaponAttachments = new TStringArray;
	string SecondaryWeapon;
	ref TStringArray SecondaryWeaponAttachments = new TStringArray;
}

/**@class		ExpansionStartingGear
 * @brief		Gear settings class
 **/
class ExpansionStartingGear: ExpansionStartingGearBase
{
	static const int VERSION = 2;
	
	ref array< ref ExpansionStartingGearItem > UpperGear = new  array< ref ExpansionStartingGearItem >;
	ref array< ref ExpansionStartingGearItem > PantsGear = new  array< ref ExpansionStartingGearItem >;
	ref array< ref ExpansionStartingGearItem > BackpackGear = new  array< ref ExpansionStartingGearItem >;
	ref array< ref ExpansionStartingGearItem > VestGear = new  array< ref ExpansionStartingGearItem >;
	
	ref ExpansionStartingGearItem PrimaryWeapon;
	ref ExpansionStartingGearItem SecondaryWeapon;
	
	// ------------------------------------------------------------
	// ExpansionStartingGear Destructor
	// ------------------------------------------------------------	
	void ~ExpansionStartingGear()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionStartingGear::~ExpansionStartingGear - Start");
		#endif
		
		UpperGear.Clear();
		delete UpperGear;	
		PantsGear.Clear();
		delete PantsGear;		
		BackpackGear.Clear();
		delete BackpackGear;	
		VestGear.Clear();
		delete VestGear;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionStartingGear::~ExpansionStartingGear - End");
		#endif
	}

	// ------------------------------------------------------------
	// ExpansionStartingGear Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionStartingGear::Defaults - Start");
		#endif
		
		super.Defaults();
		
		EnableStartingGear = true;
		UseUpperGear = true;
		UsePantsGear = false;
		UseBackpackGear = true;
		UseVestGear = false;
		UsePrimaryWeapon = false;
		UseSecondaryWeapon = false;
		
		ApplyEnergySources = true;
		SetRandomHealth = true;
		
		string worldName = "empty";
		GetGame().GetWorldName(worldName);
		worldName.ToLower();

		UpperGear.Insert(new ExpansionStartingGearItem("Rag", 4));
		UpperGear.Insert(new ExpansionStartingGearItem("Apple", 1));

		if (worldName == "barnov")
		{
			UpperGear.Insert(new ExpansionStartingGearItem("Chemlight_White", 1));
		}
		else if (worldName == "chiemsee")
		{
			UpperGear.Insert(new ExpansionStartingGearItem("Chemlight_White", 1));
		}
		else if (worldName == "esseker")
		{
			UpperGear.Insert(new ExpansionStartingGearItem("Chemlight_White", 1));
		}
		else if (worldName == "enoch" || worldName == "enochgloom")
		{
			UpperGear.Insert(new ExpansionStartingGearItem("Chemlight_White", 1));
		}
		else if (worldName == "deerisle")
		{
			UpperGear.Insert(new ExpansionStartingGearItem("Chemlight_White", 1));
		}
		else if (worldName == "namalsk")
		{
			UsePantsGear = true;
			UpperGear.Insert(new ExpansionStartingGearItem("RoadFlare", 2));
			PantsGear.Insert(new ExpansionStartingGearItem("Heatpack", 1));
			PantsGear.Insert(new ExpansionStartingGearItem("dzn_tool_watch", 1));
		}
		else if (worldName == "takistanplus")
		{
			UsePantsGear = true;
			UpperGear.Insert(new ExpansionStartingGearItem("Chemlight_White", 1));
			PantsGear.Insert(new ExpansionStartingGearItem("Compass", 1));
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionStartingGear::Defaults - End");
		#endif
	}
};


class ExpansionStartingClothing
{
	bool EnableCustomClothing;
	bool SetRandomHealth;
	
	ref TStringArray Headgear = new TStringArray;
	ref TStringArray Glasses = new TStringArray;
	ref TStringArray Masks = new TStringArray;
	ref TStringArray Tops = new TStringArray;
	ref TStringArray Vests = new TStringArray;
	ref TStringArray Gloves = new TStringArray;
	ref TStringArray Pants = new TStringArray;
	ref TStringArray Belts = new TStringArray;
	ref TStringArray Shoes = new TStringArray;
	ref TStringArray Armbands = new TStringArray;
	ref TStringArray Backpacks = new TStringArray;
	
	// ------------------------------------------------------------
	// ExpansionStartingClothing Destructor
	// ------------------------------------------------------------	
	void ~ExpansionStartingClothing()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionStartingClothing::~ExpansionStartingClothing - Start");
		#endif
		
		Headgear.Clear();
		delete Headgear;	
		Glasses.Clear();
		delete Glasses;		
		Masks.Clear();
		delete Masks;	
		Tops.Clear();
		delete Tops;
		Vests.Clear();
		delete Vests;
		Gloves.Clear();
		delete Gloves;
		Pants.Clear();
		delete Pants;
		Belts.Clear();
		delete Belts;
		Shoes.Clear();
		delete Shoes;
		Armbands.Clear();
		delete Armbands;
		Backpacks.Clear();
		delete Backpacks;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionStartingClothing::~ExpansionStartingClothing - End");
		#endif
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
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionStartingClothing::Defaults - End");
		#endif
	}
};