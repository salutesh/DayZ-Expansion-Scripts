class CfgPatches
{
	class DayZExpansion_Objects_Structures_Wreck
	{
		units[]={};
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
	class HouseNoDestruct;
	class Expansion_Wreck_Volha_Police: HouseNoDestruct
	{
		vehicleClass="Expansion_Static";
		scope=1;
		model="\DZ\structures\wrecks\Vehicles\Wreck_Volha_Police.p3d";
	};
};
