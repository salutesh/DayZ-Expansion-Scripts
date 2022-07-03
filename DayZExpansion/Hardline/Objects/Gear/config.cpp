#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Hardline_Objects_Gear
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
	};
};
class cfgVehicles
{
	class Inventory_Base;
	class Dogtag_Base: Inventory_Base
	{
		scope = 0;
	};
	class Dogtag_Survivor: Dogtag_Base{};
	class Dogtag_Scout: Dogtag_Survivor
	{
		displayName = "Scout Dogtag";
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(0.6,0.9,0,1.0,CO)"};
	};
	class Dogtag_Pathfinder: Dogtag_Survivor
	{
		displayName = "Pathfinder Dogtag";
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(0.6,0.9,1.0,1.0,CO)"};
	};
	class Dogtag_Superhero: Dogtag_Survivor
	{
		displayName = "Superhero Dogtag";
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(0.12,0.12,0.12,1.0,CO)"};
	};
	class Dogtag_Legend: Dogtag_Survivor
	{
		displayName = "Legend Dogtag";
		hiddenSelections[] = {"zbytek"};
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(0.9,0.5,0.1,1.0,CO)"};
	};
	class Dogtag_Kleptomaniac: Dogtag_Survivor
	{
		displayName = "Kleptomaniac Dogtag";
		hiddenSelections[] = {"zbytek"};
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(0.9,1.0,0.1,1.0,CO)"};
	};
	class Dogtag_Bully: Dogtag_Survivor
	{
		displayName = "Bully Dogtag";
		hiddenSelections[] = {"zbytek"};
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(0.8,0,0.7,1.0,CO)"};
	};
	class Dogtag_Killer: Dogtag_Survivor
	{
		displayName = "Killer Dogtag";
		hiddenSelections[] = {"zbytek"};
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(0.8,0.4,0,1.0,CO)"};
	};
	class Dogtag_Madman: Dogtag_Survivor
	{
		displayName = "Madman Dogtag";
		hiddenSelections[] = {"zbytek"};
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(1.0,0,0,1.0,CO)"};
	};
};
