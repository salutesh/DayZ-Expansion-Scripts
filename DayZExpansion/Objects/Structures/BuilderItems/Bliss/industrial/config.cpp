#define _ARMA_

class CfgPatches
{
	class BuilderItems_Bliss_Industrial
	{
		units[] = {"bldr_Construction_Hopper","bldr_DieselPowerPlant_Cooling","bldr_DieselPowerPlant_Transformer","bldr_DieselPowerPlant_Building_Accessories_1","bldr_DieselPowerPlant_Building_Accessories_2","bldr_DieselPowerPlant_Building_Accessories_3","bldr_DieselPowerPlant_Building_Accessories_4","bldr_DieselPowerPlant_Cooling_Accessories","bldr_Farm_ManurePile","bldr_Farm_SilageStorage","bldr_Pier_Wooden2","bldr_Misc_CinderBlocks","bldr_Misc_Drainage","bldr_Misc_HayBale_Packed","bldr_Misc_HayBale_Packed_Stack","bldr_Misc_SurveyMarker_Cover","bldr_Misc_SurveyMarker_Post","bldr_Misc_SurveyMarker_Rod","bldr_Misc_Tire","bldr_Misc_TirePile_Large","bldr_Misc_Transformer","bldr_Pipe_Small2_24m","bldr_Pipe_Small2_8m","bldr_Pipe_Small2_Curve","bldr_Pipe_Small2_Ground","bldr_Pipe_Small2_High_24m","bldr_Pipe_Small2_High_8m","bldr_Pipe_Small2_High_Curve","bldr_Pipe_Small2_High_Ground","bldr_Power_Pole_Wood4","bldr_Power_Pole_Wood4_A","bldr_Power_Pole_Wood4_A_End","bldr_Power_Pole_Wood4_A_J","bldr_Power_Pole_Wood4_A_J_Nest","bldr_Power_Pole_Wood4_End","bldr_Power_Pole_Wood4_Lamp","bldr_Power_Pole_Wood5","bldr_Power_Pole_Wood5_End","bldr_Power_Pole_Wood5_J","bldr_Power_Pole_Wood6_End","bldr_Power_Pole_Wood6_H","bldr_Power_Pole_Wood6_J","bldr_Power_VHV_End","bldr_Power_VHV_Wires_Direct","bldr_Power_VHV_Wires_Direct_Short","bldr_Power_VHV_Wires_End","bldr_Power_VHV_Wires_Left","bldr_Power_VHV_Wires_Right","bldr_Power_WireTel_50_Main","bldr_Power_Wire_50","bldr_Power_Wire_50_Main","bldr_Power_VHV_Tower_Corner_Proxy","bldr_Power_VHV_Tower_Proxy","bldr_Shed_Open_Big2_Complete","bldr_Shed_Open_Big_Complete","bldr_Shed_Open_Small_Complete","bldr_Smokestack_Big_Ruin_Bottom","bldr_Smokestack_Big_Ruin_Low","bldr_Smokestack_Big_Ruin_Mid","bldr_Smokestack_Big_Ruin_Up"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","DZ_Structures_Signs"};
	};
};
class CfgVehicles
{
	class HouseNoDestruct;
	class bldr_Construction_Hopper: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\Construction\Construction_Hopper.p3d";
	};
	class bldr_DieselPowerPlant_Cooling: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\DieselPowerPlant\DieselPowerPlant_Cooling.p3d";
	};
	class bldr_DieselPowerPlant_Transformer: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\DieselPowerPlant\DieselPowerPlant_Transformer.p3d";
	};
	class bldr_DieselPowerPlant_Building_Accessories_1: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\DieselPowerPlant\Proxy\DieselPowerPlant_Building_Accessories_1.p3d";
	};
	class bldr_DieselPowerPlant_Building_Accessories_2: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\DieselPowerPlant\Proxy\DieselPowerPlant_Building_Accessories_2.p3d";
	};
	class bldr_DieselPowerPlant_Building_Accessories_3: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\DieselPowerPlant\Proxy\DieselPowerPlant_Building_Accessories_3.p3d";
	};
	class bldr_DieselPowerPlant_Building_Accessories_4: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\DieselPowerPlant\Proxy\DieselPowerPlant_Building_Accessories_4.p3d";
	};
	class bldr_DieselPowerPlant_Cooling_Accessories: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\DieselPowerPlant\Proxy\DieselPowerPlant_Cooling_Accessories.p3d";
	};
	class bldr_Farm_ManurePile: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\Farms\Farm_ManurePile.p3d";
	};
	class bldr_Farm_SilageStorage: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\Farms\Farm_SilageStorage.p3d";
	};
	class bldr_Pier_Wooden2: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\Harbour\Pier_Wooden2.p3d";
	};
	class bldr_Misc_CinderBlocks: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\Misc\Misc_CinderBlocks.p3d";
	};
	class bldr_Misc_Drainage: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\Misc\Misc_Drainage.p3d";
	};
	class bldr_Misc_HayBale_Packed: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\Misc\Misc_HayBale_Packed.p3d";
	};
	class bldr_Misc_HayBale_Packed_Stack: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\Misc\Misc_HayBale_Packed_Stack.p3d";
	};
	class bldr_Misc_SurveyMarker_Cover: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\Misc\Misc_SurveyMarker_Cover.p3d";
	};
	class bldr_Misc_SurveyMarker_Post: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\Misc\Misc_SurveyMarker_Post.p3d";
	};
	class bldr_Misc_SurveyMarker_Rod: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\Misc\Misc_SurveyMarker_Rod.p3d";
	};
	class bldr_Misc_Tire: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\Misc\Misc_Tire.p3d";
	};
	class bldr_Misc_TirePile_Large: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\Misc\Misc_TirePile_Large.p3d";
	};
	class bldr_Misc_Transformer: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\Misc\Misc_Transformer.p3d";
	};
	class bldr_Pipe_Small2_24m: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\Pipes\Pipe_Small2_24m.p3d";
	};
	class bldr_Pipe_Small2_8m: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\Pipes\Pipe_Small2_8m.p3d";
	};
	class bldr_Pipe_Small2_Curve: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\Pipes\Pipe_Small2_Curve.p3d";
	};
	class bldr_Pipe_Small2_Ground: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\Pipes\Pipe_Small2_Ground.p3d";
	};
	class bldr_Pipe_Small2_High_24m: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\Pipes\Pipe_Small2_High_24m.p3d";
	};
	class bldr_Pipe_Small2_High_8m: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\Pipes\Pipe_Small2_High_8m.p3d";
	};
	class bldr_Pipe_Small2_High_Curve: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\Pipes\Pipe_Small2_High_Curve.p3d";
	};
	class bldr_Pipe_Small2_High_Ground: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\Pipes\Pipe_Small2_High_Ground.p3d";
	};
	class bldr_Power_Pole_Wood4: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\Power\Power_Pole_Wood4.p3d";
	};
	class bldr_Power_Pole_Wood4_A: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\Power\Power_Pole_Wood4_A.p3d";
	};
	class bldr_Power_Pole_Wood4_A_End: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\Power\Power_Pole_Wood4_A_End.p3d";
	};
	class bldr_Power_Pole_Wood4_A_J: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\Power\Power_Pole_Wood4_A_J.p3d";
	};
	class bldr_Power_Pole_Wood4_A_J_Nest: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\Power\Power_Pole_Wood4_A_J_Nest.p3d";
	};
	class bldr_Power_Pole_Wood4_End: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\Power\Power_Pole_Wood4_End.p3d";
	};
	class bldr_Power_Pole_Wood4_Lamp: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\Power\Power_Pole_Wood4_Lamp.p3d";
	};
	class bldr_Power_Pole_Wood5: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\Power\Power_Pole_Wood5.p3d";
	};
	class bldr_Power_Pole_Wood5_End: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\Power\Power_Pole_Wood5_End.p3d";
	};
	class bldr_Power_Pole_Wood5_J: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\Power\Power_Pole_Wood5_J.p3d";
	};
	class bldr_Power_Pole_Wood6_End: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\Power\Power_Pole_Wood6_End.p3d";
	};
	class bldr_Power_Pole_Wood6_H: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\Power\Power_Pole_Wood6_H.p3d";
	};
	class bldr_Power_Pole_Wood6_J: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\Power\Power_Pole_Wood6_J.p3d";
	};
	class bldr_Power_VHV_End: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\Power\Power_VHV_End.p3d";
	};
	class bldr_Power_VHV_Wires_Direct: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\Power\Power_VHV_Wires_Direct.p3d";
	};
	class bldr_Power_VHV_Wires_Direct_Short: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\Power\Power_VHV_Wires_Direct_Short.p3d";
	};
	class bldr_Power_VHV_Wires_End: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\Power\Power_VHV_Wires_End.p3d";
	};
	class bldr_Power_VHV_Wires_Left: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\Power\Power_VHV_Wires_Left.p3d";
	};
	class bldr_Power_VHV_Wires_Right: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\Power\Power_VHV_Wires_Right.p3d";
	};
	class bldr_Power_WireTel_50_Main: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\Power\Power_WireTel_50_Main.p3d";
	};
	class bldr_Power_Wire_50: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\Power\Power_Wire_50.p3d";
	};
	class bldr_Power_Wire_50_Main: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\Power\Power_Wire_50_Main.p3d";
	};
	class bldr_Power_VHV_Tower_Corner_Proxy: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\Power\Proxy\Power_VHV_Tower_Corner_Proxy.p3d";
	};
	class bldr_Power_VHV_Tower_Proxy: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\Power\Proxy\Power_VHV_Tower_Proxy.p3d";
	};
	class bldr_Shed_Open_Big2_Complete: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\Sheds\Shed_Open_Big2_Complete.p3d";
	};
	class bldr_Shed_Open_Big_Complete: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\Sheds\Shed_Open_Big_Complete.p3d";
	};
	class bldr_Shed_Open_Small_Complete: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\Sheds\Shed_Open_Small_Complete.p3d";
	};
	class bldr_Smokestack_Big_Ruin_Bottom: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\Smokestacks\Smokestack_Big_Ruin_Bottom.p3d";
	};
	class bldr_Smokestack_Big_Ruin_Low: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\Smokestacks\Smokestack_Big_Ruin_Low.p3d";
	};
	class bldr_Smokestack_Big_Ruin_Mid: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\Smokestacks\Smokestack_Big_Ruin_Mid.p3d";
	};
	class bldr_Smokestack_Big_Ruin_Up: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\structures_bliss\industrial\Smokestacks\Smokestack_Big_Ruin_Up.p3d";
	};
};
