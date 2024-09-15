class CfgPatches
{
	class DayZExpansion_Objects_BaseBuilding_Tents_Kits
	{
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data"
		};
	};
};
class CfgVehicles
{
	class ExpansionKitLarge;
	class ExpansionCamoBoxKit: ExpansionKitLarge
	{
		scope=2;
		displayName="$STR_EXPANSION_CAMO_BOX";
		descriptionShort="$STR_EXPANSION_CAMO_BOX_DESC";
		model="\DayZExpansion\Objects\BaseBuilding\Tents_Kits\atent.p3d";
		itemSize[]={8,8};
	};
	class ExpansionCamoBoxKitPlacing: ExpansionCamoBoxKit
	{
		scope=2;
		displayName="$STR_EXPANSION_CAMO_BOX";
		descriptionShort="$STR_EXPANSION_CAMO_BOX_DESC";
		model="\DayZExpansion\Objects\Basebuilding\Tents\Camo_Box2_preview.p3d";
		slopeTolerance=0.85000002;
	};
	class ExpansionCamoTentKit: ExpansionKitLarge
	{
		scope=2;
		displayName="$STR_EXPANSION_CAMO_TENT";
		descriptionShort="$STR_EXPANSION_CAMO_TENT_DESC";
		model="\DayZExpansion\Objects\BaseBuilding\Tents_Kits\atent.p3d";
		itemSize[]={8,4};
		attachments[]={};
	};
	class ExpansionCamoTentKitPlacing: ExpansionCamoTentKit
	{
		scope=2;
		displayName="$STR_EXPANSION_CAMO_TENT";
		descriptionShort="$STR_EXPANSION_CAMO_TENT_DESC";
		model="\DayZExpansion\Objects\Basebuilding\Tents\ATent_preview.p3d";
		slopeTolerance=0.85000002;
	};
};
