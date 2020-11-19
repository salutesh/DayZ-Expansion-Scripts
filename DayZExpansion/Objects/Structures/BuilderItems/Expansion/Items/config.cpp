#define _ARMA_

class CfgPatches
{
	class BuilderItems_Expansion_Items
	{
		units[] = {"bldr_expansion_Gold_Nugget","bldr_expansion_Gold_Bar","bldr_expansion_Silver_Nugget","bldr_expansion_Silver_Bar"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Structures_Signs"};
	};
};
class CfgVehicles
{
	class HouseNoDestruct;
	class bldr_expansion_Gold_Nugget: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Gear\Traders\nugget_gold.p3d";
	};
	class bldr_expansion_Gold_Bar: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Gear\Traders\gold_bar.p3d";
	};
	class bldr_expansion_Silver_Nugget: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Gear\Traders\nugget_silver.p3d";
	};
	class bldr_expansion_Silver_Bar: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Gear\Traders\silver_bar.p3d";
	};
};
