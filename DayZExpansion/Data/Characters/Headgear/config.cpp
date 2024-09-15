class CfgPatches
{
	class Expansion_Headgear
	{
		units[]=
		{
			"ExpansionCowboyHatGator",
			"ExpansionCowboyHatSnake",
			"ExpansionCowboyHatRattleSnake",
			"ExpansionShemag",
			"ExpansionMich2001Desert"
		};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Characters_Headgear"
		};
	};
};
class CfgVehicles
{
	class Mich2001Helmet;
	class Ushanka_ColorBase;
	class Clothing;
	class BaseballCap_ColorBase;
	class CowboyHat_ColorBase;
	class ExpansionCowboyHatGator: CowboyHat_ColorBase
	{
		scope=2;
		visibilityModifier=0.94999999;
		hiddenSelectionsTextures[]=
		{
			"DayZExpansion\Data\characters\headgear\data\CowboyHat_blackGator.paa",
			"DayZExpansion\Data\characters\headgear\data\CowboyHat_blackGator.paa",
			"DayZExpansion\Data\characters\headgear\data\CowboyHat_blackGator.paa"
		};
	};
	class ExpansionCowboyHatSnake: CowboyHat_ColorBase
	{
		scope=2;
		visibilityModifier=0.94999999;
		hiddenSelectionsTextures[]=
		{
			"DayZExpansion\Data\characters\headgear\data\CowboyHat_Snake.paa",
			"DayZExpansion\Data\characters\headgear\data\CowboyHat_Snake.paa",
			"DayZExpansion\Data\characters\headgear\data\CowboyHat_Snake.paa"
		};
	};
	class ExpansionCowboyHatRattleSnake: CowboyHat_ColorBase
	{
		scope=2;
		visibilityModifier=0.94999999;
		hiddenSelectionsTextures[]=
		{
			"DayZExpansion\Data\characters\headgear\data\CowboyHat_RattleSnake.paa",
			"DayZExpansion\Data\characters\headgear\data\CowboyHat_RattleSnake.paa",
			"DayZExpansion\Data\characters\headgear\data\CowboyHat_RattleSnake.paa"
		};
	};
	class ExpansionShemag: Clothing
	{
		scope=2;
		displayName="Shemag";
		descriptionShort="It's a Shemag!";
		model="\DayZExpansion\Data\Characters\headgear\ExpansionShemag_g.p3d";
		inventorySlot="Mask";
		simulation="clothing";
		vehicleClass="Clothing";
		itemInfo[]=
		{
			"Clothing",
			"Back"
		};
		rotationFlags=0;
		itemSize[]={2,2};
		weight=100;
		absorbency=0.2;
		heatIsolation=0.13;
		repairableWithKits[]={5,3};
		repairCosts[]={30,25};
		soundAttType="Outdoor";
		class ClothingTypes
		{
			male="\DayZExpansion\Data\Characters\headgear\ExpansionShemag.p3d";
			female="\DayZExpansion\Data\Characters\headgear\ExpansionShemag.p3d";
		};
	};
	class ExpansionMich2001Desert: Mich2001Helmet
	{
		scope=2;
		hiddenSelections[]=
		{
			"camoGround",
			"camoMale",
			"camoFemale"
		};
		hiddenSelectionsTextures[]=
		{
			"DayZExpansion\Data\characters\headgear\data\mich2001_desertmarpat_co.paa",
			"DayZExpansion\Data\characters\headgear\data\mich2001_desertmarpat_co.paa",
			"DayZExpansion\Data\characters\headgear\data\mich2001_desertmarpat_co.paa"
		};
	};
};
