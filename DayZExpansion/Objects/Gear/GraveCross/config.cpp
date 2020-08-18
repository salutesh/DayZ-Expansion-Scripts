#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Objects_Gravecross
	{
		units[] = {"Expansion_Gravecross","Expansion_Gravecross_LowLifetime"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
	};
};
class CfgVehicles
{
	class Container_Base;
	class Expansion_GraveBase: Container_Base
	{
		scope = 0;
		displayName = "$STR_EXPANSION_GRAVECROSS";
		descriptionShort = "$STR_EXPANSION_GRAVECROSS_DESC";
		forceFarBubble = "true";
		attachments[] = {"Shoulder","Melee","Headgear","Mask","Eyewear","Gloves","Armband","Vest","Body","Back","Hips","Legs","Feet"};
	};
	class Expansion_Gravecross: Expansion_GraveBase
	{
		scope = 2;
		model = "DZ\structures\specific\cemeteries\cemetery_tombstone10.p3d";
	};
	class Expansion_Gravecross_LowLifetime: Expansion_GraveBase
	{
		scope = 2;
		model = "DZ\structures\specific\cemeteries\cemetery_tombstone2.p3d";
	};
};
