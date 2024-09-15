class CfgPatches
{
	class DayZExpansion_Objects_Structures_BMX
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
	class ExpansionBMXLoop: HouseNoDestruct
	{
		scope=1;
		model="DayZExpansion\Objects\Structures\BMX\BMXLoop.p3d";
		hiddenSelections[]=
		{
			"texture"
		};
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(1,0,0,1,co)"
		};
		bounding="BSphere";
		overrideDrawArea="8.0";
		forceFarBubble="true";
		physLayer="item_large";
		carveNavmesh=0;
	};
};
