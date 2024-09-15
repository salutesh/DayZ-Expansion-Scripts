class CfgPatches
{
	class DayZExpansion_Core_Objects_Bases
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
	class ExpansionStaticObjectBase: HouseNoDestruct
	{
		scope=0;
		vehicleClass="Expansion_Static";
	};
};
