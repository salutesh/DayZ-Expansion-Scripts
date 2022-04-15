/**
 * ExpansionStartingGear.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
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
	void ExpansionStartingGearItem(string className, int quantity = -1, array<string> attachments = null)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "ExpansionStartingGearItem");
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
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "Defaults");
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
	}
}

/**@class		ExpansionStartingGearV1
 * @brief		Gear settings version 1 class
 **/
class ExpansionStartingGearV1: ExpansionStartingGearBase
{
	autoptr TStringArray UpperGear = new TStringArray;				// Array of classnames used for the upper gear
	autoptr TStringArray PantsGear = new TStringArray;					// Array of classnames used for the pants gear
	autoptr TStringArray BackpackGear = new TStringArray;			// Array of classnames used for the backpack gear
	autoptr TStringArray VestGear = new TStringArray;					// Array of classnames used for the vest gear
	
	string PrimaryWeapon;
	autoptr TStringArray PrimaryWeaponAttachments = new TStringArray;
	string SecondaryWeapon;
	autoptr TStringArray SecondaryWeaponAttachments = new TStringArray;
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
	// ExpansionStartingGear Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "Defaults");
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
		
		string world_name = "empty";
		GetGame().GetWorldName(world_name);
		world_name.ToLower();
		
		if ( world_name.IndexOf("gloom") == world_name.Length() - 5 )
		{
			world_name = world_name.Substring(0, world_name.Length() - 5);
		}

		UpperGear.Insert(new ExpansionStartingGearItem("Rag", 4));
		UpperGear.Insert(new ExpansionStartingGearItem("Apple"));

		switch ( world_name )
		{
			case "barnov":
			case "chiemsee":
			case "esseker":
			case "enoch":
			case "deerisle":
			{
				UpperGear.Insert(new ExpansionStartingGearItem("Chemlight_White"));
				break;
			}
			case "namalsk":
			{
				UpperGear.Insert(new ExpansionStartingGearItem("RoadFlare", 2));
				PantsGear.Insert(new ExpansionStartingGearItem("Heatpack"));
				PantsGear.Insert(new ExpansionStartingGearItem("dzn_tool_watch"));
				UsePantsGear = true;
				break;
			}
			case "takistanplus":
			{
				UpperGear.Insert(new ExpansionStartingGearItem("Chemlight_White"));
				PantsGear.Insert(new ExpansionStartingGearItem("Compass"));
				UsePantsGear = true;
				break;
			}
		}
	}
};


class ExpansionStartingClothing
{
	bool EnableCustomClothing;
	bool SetRandomHealth;
	
	autoptr TStringArray Headgear = new TStringArray;
	autoptr TStringArray Glasses = new TStringArray;
	autoptr TStringArray Masks = new TStringArray;
	autoptr TStringArray Tops = new TStringArray;
	autoptr TStringArray Vests = new TStringArray;
	autoptr TStringArray Gloves = new TStringArray;
	autoptr TStringArray Pants = new TStringArray;
	autoptr TStringArray Belts = new TStringArray;
	autoptr TStringArray Shoes = new TStringArray;
	autoptr TStringArray Armbands = new TStringArray;
	autoptr TStringArray Backpacks = new TStringArray;
	
	// ------------------------------------------------------------
	// ExpansionStartingClothing Defaults
	// ------------------------------------------------------------
	void Defaults()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SETTINGS, this, "Defaults");
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
	}
};
