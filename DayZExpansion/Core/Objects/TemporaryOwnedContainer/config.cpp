class CfgPatches
{
	class DayZExpansion_Objects_TemporaryOwnedContainer
	{
		units[]=
		{
			"ExpansionTemporaryOwnedContainer"
		};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data"
		};
	};
};
class CfgVehicles
{
	class Container_Base;
	class ExpansionTemporaryOwnedContainer: Container_Base
	{
		scope=2;
		displayName="#STR_EXPANSION_TEMPORARY_STORAGE";
		descriptionShort="#STR_EXPANSION_TEMPORARY_STORAGE_DESC";
		model="\DZ\gear\camping\wooden_case.p3d";
		itemSize[]={10,5};
		attachments[]=
		{
			"Shoulder",
			"Melee",
			"Vest",
			"Body",
			"Hips",
			"Legs",
			"Back",
			"Headgear",
			"Mask",
			"Eyewear",
			"Gloves",
			"Feet",
			"Armband"
		};
		class Cargo
		{
			itemsCargoSize[]={10,100};
		};
	};
};
