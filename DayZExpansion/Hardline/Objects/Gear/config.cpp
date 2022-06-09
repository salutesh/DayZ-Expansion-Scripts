#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Hardline_Objects_Gear
	{
		units[] = {"Dogtag_Scout","Dogtag_Pathfinder","Dogtag_Superhero","Dogtag_Legend","Dogtag_Kleptomaniac","Dogtag_Bully","Dogtag_Killer","Dogtag_Madman"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Scripts","DayZExpansion_Core","DayZExpansion_Scripts","DayZExpansion_Vehicles_Scripts","DayZExpansion_Book_Scripts","DayZExpansion_Market_Scripts","Dogtags_Gear"};
	};
};
class cfgVehicles
{
	class Inventory_Base;
	class Dogtag_Survivor;
	class Dogtag_Scout: Dogtag_Survivor
	{
		scope = 2;
		displayName = "Scout Dogtag";
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(0.6,0.9,0,1.0,CO)"};
	};
	class Dogtag_Pathfinder: Dogtag_Survivor
	{
		scope = 2;
		displayName = "Pathfinder Dogtag";
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(0.6,0.9,1.0,1.0,CO)"};
	};
	class Dogtag_Superhero: Dogtag_Survivor
	{
		scope = 2;
		displayName = "Superhero Dogtag";
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(0.12,0.12,0.12,1.0,CO)"};
	};
	class Dogtag_Legend: Dogtag_Survivor
	{
		scope = 2;
		displayName = "Legend Dogtag";
		hiddenSelections[] = {"zbytek"};
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(0.9,0.5,0.1,1.0,CO)"};
	};
	class Dogtag_Kleptomaniac: Dogtag_Survivor
	{
		scope = 2;
		displayName = "Kleptomaniac Dogtag";
		hiddenSelections[] = {"zbytek"};
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(0.9,1.0,0.1,1.0,CO)"};
	};
	class Dogtag_Bully: Dogtag_Survivor
	{
		scope = 2;
		displayName = "Bully Dogtag";
		hiddenSelections[] = {"zbytek"};
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(0.8,0,0.7,1.0,CO)"};
	};
	class Dogtag_Killer: Dogtag_Survivor
	{
		scope = 2;
		displayName = "Killer Dogtag";
		hiddenSelections[] = {"zbytek"};
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(0.8,0.4,0,1.0,CO)"};
	};
	class Dogtag_Madman: Dogtag_Survivor
	{
		scope = 2;
		displayName = "Madman Dogtag";
		hiddenSelections[] = {"zbytek"};
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(1.0,0,0,1.0,CO)"};
	};
};
