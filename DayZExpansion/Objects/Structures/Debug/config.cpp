#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Objects_Structures_Debug
	{
		units[] = {"ExpansionAdminHammer","ExpansionDebugRod","ExpansionDebugRod_Red","ExpansionDebugRod_Orange","ExpansionDebugRod_Blue","ExpansionDebugRod_Purple","ExpansionDebugRodBig","ExpansionDebugRodBig_Red","ExpansionDebugRodBig_Orange","ExpansionDebugRodBig_Blue","ExpansionDebugRodBig_Purple","ExpansionDebugBox","ExpansionDebugBox_Red","ExpansionDebugBox_Orange","ExpansionDebugBox_Blue","ExpansionDebugBox_Purple","ExpansionNetSyncDebugObject","ExpansionDebugLine","ExpansionDebugLine_Red","ExpansionDebugLine_Orange","ExpansionDebugLine_Blue","ExpansionDebugLine_Purple","ExpansionDebugNoticeMe","ExpansionDebugNoticeMe_Red","ExpansionDebugNoticeMe_Orange","ExpansionDebugNoticeMe_Blue","ExpansionDebugNoticeMe_Purple","ExpansionDebugArrow","ExpansionDebugArrow_Red","ExpansionDebugArrow_Orange","ExpansionDebugArrow_Blue","ExpansionDebugArrow_Purple"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","DZ_Gear_Tools"};
	};
};
class CfgVehicles
{
	class Hammer;
	class HouseNoDestruct;
	class ExpansionDebugCylinder: HouseNoDestruct
	{
		scope = 1;
		model = "DayZExpansion\Objects\Structures\Debug\DebugCylinder.p3d";
	};
	class ExpansionAdminHammer: Hammer
	{
		scope = 2;
		displayName = "Admin Hammer";
		expansionIsAdminTool = 1;
	};
	class ExpansionDebugRod: HouseNoDestruct
	{
		scope = 2;
		model = "DayZExpansion\Objects\Structures\Debug\DebugRod.p3d";
		hiddenSelections[] = {"texture"};
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(0,1,0,1,co)"};
	};
	class ExpansionDebugRod_Red: ExpansionDebugRod
	{
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(1,0,0,1,co)"};
	};
	class ExpansionDebugRod_Orange: ExpansionDebugRod
	{
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(1,0.5,0,1,co)"};
	};
	class ExpansionDebugRod_Blue: ExpansionDebugRod
	{
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(0,0,1,1,co)"};
	};
	class ExpansionDebugRod_Purple: ExpansionDebugRod
	{
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(0.5,0,1,1,co)"};
	};
	class ExpansionDebugRodBig: ExpansionDebugRod
	{
		model = "DayZExpansion\Objects\Structures\Debug\DebugRodBig.p3d";
	};
	class ExpansionDebugRodBig_Red: ExpansionDebugRodBig
	{
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(1,0,0,1,co)"};
	};
	class ExpansionDebugRodBig_Orange: ExpansionDebugRodBig
	{
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(1,0.5,0,1,co)"};
	};
	class ExpansionDebugRodBig_Blue: ExpansionDebugRodBig
	{
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(0,0,1,1,co)"};
	};
	class ExpansionDebugRodBig_Purple: ExpansionDebugRodBig
	{
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(0.5,0,1,1,co)"};
	};
	class ExpansionDebugBox: ExpansionDebugRod
	{
		model = "DayZExpansion\Objects\Structures\Debug\DebugBox.p3d";
	};
	class ExpansionDebugBox_Red: ExpansionDebugBox
	{
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(1,0,0,1,co)"};
	};
	class ExpansionDebugBox_Orange: ExpansionDebugBox
	{
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(1,0.5,0,1,co)"};
	};
	class ExpansionDebugBox_Blue: ExpansionDebugBox
	{
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(0,0,1,1,co)"};
	};
	class ExpansionDebugBox_Purple: ExpansionDebugBox
	{
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(0.5,0,1,1,co)"};
	};
	class ExpansionNetSyncDebugObject: ExpansionDebugBox
	{
		model = "DayZExpansion\Objects\Structures\Debug\DebugCube.p3d";
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(0.5,0,1,1,co)"};
	};
	class ExpansionDebugLine: ExpansionDebugRod
	{
		model = "DayZExpansion\Objects\Structures\Debug\DebugLine.p3d";
	};
	class ExpansionDebugLine_Red: ExpansionDebugLine
	{
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(1,0,0,1,co)"};
	};
	class ExpansionDebugLine_Orange: ExpansionDebugLine
	{
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(1,0.5,0,1,co)"};
	};
	class ExpansionDebugLine_Blue: ExpansionDebugLine
	{
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(0,0,1,1,co)"};
	};
	class ExpansionDebugLine_Purple: ExpansionDebugLine
	{
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(0.5,0,1,1,co)"};
	};
	class ExpansionDebugNoticeMe: ExpansionDebugRod
	{
		model = "DayZExpansion\Objects\Structures\Debug\DebugNoticeMe.p3d";
	};
	class ExpansionDebugNoticeMe_Red: ExpansionDebugNoticeMe
	{
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(1,0,0,1,co)"};
	};
	class ExpansionDebugNoticeMe_Orange: ExpansionDebugNoticeMe
	{
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(1,0.5,0,1,co)"};
	};
	class ExpansionDebugNoticeMe_Blue: ExpansionDebugNoticeMe
	{
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(0,0,1,1,co)"};
	};
	class ExpansionDebugNoticeMe_Purple: ExpansionDebugNoticeMe
	{
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(0.5,0,1,1,co)"};
	};
	class ExpansionDebugArrow: ExpansionDebugRod
	{
		model = "DayZExpansion\Objects\Structures\Debug\DebugArrow.p3d";
		class AnimationSources
		{
			class scale
			{
				source = "user";
				initPhase = 0;
				animPeriod = 0.001;
			};
		};
	};
	class ExpansionDebugArrow_Red: ExpansionDebugArrow
	{
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(1,0,0,1,co)"};
	};
	class ExpansionDebugArrow_Orange: ExpansionDebugArrow
	{
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(1,0.5,0,1,co)"};
	};
	class ExpansionDebugArrow_Blue: ExpansionDebugArrow
	{
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(0,0,1,1,co)"};
	};
	class ExpansionDebugArrow_Purple: ExpansionDebugArrow
	{
		hiddenSelectionsTextures[] = {"#(argb,8,8,3)color(0.5,0,1,1,co)"};
	};
	class ExpansionDebugGoat: HouseNoDestruct
	{
		scope = 1;
		model = "\dz\animals\capra_hircus\capra_hircus.p3d";
		displayName = "Expansion Debug Goat";
		descriptionShort = "Debug goat";
		hiddenSelections[] = {"Camo","CamoHair"};
		hiddenSelectionsTextures[] = {"dz\animals\capra_hircus\data\Black_Goat_CO.paa","dz\animals\capra_hircus\data\black_goat_fur_co.paa"};
		hiddenSelectionsMaterials[] = {"dz\animals\capra_hircus\data\Black_Goat.rvmat","dz\animals\capra_hircus\data\black_goat_fur.rvmat"};
	};
};
