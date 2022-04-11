#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Structures_Walls
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Structures_Walls","DayZExpansion_Core_Scripts"};
	};
};
class CfgVehicles
{
	class ExpansionPhysicsStructure;
	class ExpansionPhysics_Static_Lamp_Ind: ExpansionPhysicsStructure
	{
		model = "\DZ\structures\industrial\Misc\Lamp_Ind.p3d";
	};
	class ExpansionPhysics_Static_Lamp_Rail: ExpansionPhysicsStructure
	{
		model = "\DZ\structures\industrial\Misc\Lamp_Rail.p3d";
	};
};
class CfgNonAIVehicles
{
	class ExpansionLampLightBase;
	class Static_Lamp_Ind: ExpansionLampLightBase{};
	class Static_Lamp_Rail: ExpansionLampLightBase{};
};
