#define _ARMA_

class CfgPatches
{
	class BuilderItems_Bliss_Military
	{
		units[] = {"bldr_Mil_ControlTower_Acc","bldr_mil_enoch_map","bldr_Misc_DeconShower_Large","bldr_Misc_DragonTeeth_Multiple","bldr_Misc_DragonTeeth_Multiple_RedWhite","bldr_Misc_DragonTeeth_Single","bldr_Misc_DragonTeeth_Single_RedWhite","bldr_ShellCrater1","bldr_ShellCrater1_Decal","bldr_ShellCrater2_Debris","bldr_ShellCrater2_Decal","bldr_ShellCrater2_Large","bldr_ShellCrater2_Small","bldr_Target_Concrete1","bldr_Target_Concrete2","bldr_Target_Concrete_Block","bldr_Target_Concrete_Ramp","bldr_Target_Support"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Structures_Bliss_Military"};
	};
};
class CfgVehicles
{
	class HouseNoDestruct;
	class bldr_Mil_ControlTower_Acc: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\military\Houses\Proxy\Mil_ControlTower_Acc.p3d";
	};
	class bldr_mil_enoch_map: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\military\Houses\Proxy\mil_enoch_map.p3d";
	};
	class bldr_Misc_DeconShower_Large: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\military\Misc\Misc_DeconShower_Large.p3d";
	};
	class bldr_Misc_DragonTeeth_Multiple: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\military\Misc\Misc_DragonTeeth_Multiple.p3d";
	};
	class bldr_Misc_DragonTeeth_Multiple_RedWhite: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\military\Misc\Misc_DragonTeeth_Multiple_RedWhite.p3d";
	};
	class bldr_Misc_DragonTeeth_Single: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\military\Misc\Misc_DragonTeeth_Single.p3d";
	};
	class bldr_Misc_DragonTeeth_Single_RedWhite: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\military\Misc\Misc_DragonTeeth_Single_RedWhite.p3d";
	};
	class bldr_ShellCrater1: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\military\Training\ShellCrater1.p3d";
	};
	class bldr_ShellCrater1_Decal: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\military\Training\ShellCrater1_Decal.p3d";
	};
	class bldr_ShellCrater2_Debris: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\military\Training\ShellCrater2_Debris.p3d";
	};
	class bldr_ShellCrater2_Decal: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\military\Training\ShellCrater2_Decal.p3d";
	};
	class bldr_ShellCrater2_Large: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\military\Training\ShellCrater2_Large.p3d";
	};
	class bldr_ShellCrater2_Small: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\military\Training\ShellCrater2_Small.p3d";
	};
	class bldr_Target_Concrete1: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\military\Training\Target_Concrete1.p3d";
	};
	class bldr_Target_Concrete2: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\military\Training\Target_Concrete2.p3d";
	};
	class bldr_Target_Concrete_Block: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\military\Training\Target_Concrete_Block.p3d";
	};
	class bldr_Target_Concrete_Ramp: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\military\Training\Target_Concrete_Ramp.p3d";
	};
	class bldr_Target_Support: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\military\Training\Target_Support.p3d";
	};
};
