#define _ARMA_

class CfgPatches
{
	class BuilderItems_StaticItems_Core
	{
		units[] = {"bldr_Canteen","bldr_WaterBottle","bldr_Vodka","bldr_SodaCan_Pipsi","bldr_SodaCan_Cola","bldr_SodaCan_Spite","bldr_SodaCan_Kvass","bldr_SodaCan_Empty","bldr_BoxCerealCrunchin","bldr_PowderedMilk","bldr_SmallGuts","bldr_Guts","bldr_Rice","bldr_Marmalade","bldr_BakedBeansCan","bldr_BakedBeansCan_Opened","bldr_PeachesCan","bldr_PeachesCan_Opened","bldr_TacticalBaconCan","bldr_TacticalBaconCan_Opened","bldr_SpaghettiCan","bldr_SpaghettiCan_Opened","bldr_SardinesCan","bldr_SardinesCan_Opened","bldr_TunaCan","bldr_TunaCan_Opened","bldr_Apple","bldr_Banana","bldr_Orange","bldr_Tomato","bldr_GreenBellPepper","bldr_Zucchini","bldr_Pumpkin","bldr_SlicedPumpkin","bldr_Potato","bldr_Kiwi","bldr_SambucusBerry","bldr_CaninaBerry","bldr_Cannabis","bldr_Plum","bldr_Pear","bldr_HumanSteakMeat","bldr_GoatSteakMeat","bldr_MouflonSteakMeat","bldr_BoarSteakMeat","bldr_PigSteakMeat","bldr_DeerSteakMeat","bldr_WolfSteakMeat","bldr_BearSteakMeat","bldr_CowSteakMeat","bldr_SheepSteakMeat","bldr_FoxSteakMeat","bldr_ChickenBreastMeat","bldr_RabbitLegMeat","bldr_CarpFilletMeat","bldr_MackerelFilletMeat","bldr_Lard","bldr_Carp","bldr_Sardines","bldr_Mackerel","bldr_Worm","bldr_AgaricusMushroom","bldr_AmanitaMushroom","bldr_MacrolepiotaMushroom","bldr_LactariusMushroom","bldr_PsilocybeMushroom","bldr_AuriculariaMushroom","bldr_BoletusMushroom","bldr_PleurotusMushroom","bldr_KitchenKnife","bldr_MeatTenderizer","bldr_Pot","bldr_MediumGasCanister","bldr_PortableGasStove","bldr_LargeGasCanister","bldr_SmallGasCanister","bldr_PortableGasLamp","bldr_DisinfectantSpray","bldr_DisinfectantAlcohol","bldr_VitaminBottle","bldr_CharcoalTablets","bldr_PainkillerTablets","bldr_TetracyclineAntibiotics","bldr_megaphone"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","DZ_Structures_Signs"};
	};
};
class CfgVehicles
{
	class HouseNoDestruct;
	class bldr_food: HouseNoDestruct
	{
		physLayer = "item_small";
		carveNavmesh = 0;
	};
	class bldr_Canteen: bldr_food
	{
		scope = 2;
		model = "\dz\gear\drinks\Canteen.p3d";
	};
	class bldr_WaterBottle: bldr_food
	{
		scope = 2;
		model = "\dz\gear\drinks\WaterBottle.p3d";
	};
	class bldr_Vodka: bldr_food
	{
		scope = 2;
		model = "\dz\gear\drinks\VodkaBottles.p3d";
	};
	class bldr_SodaCan_Pipsi: bldr_food
	{
		scope = 2;
		model = "\dz\gear\drinks\SodaCan.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"\dz\gear\drinks\Data\SodaCan_pipsi_co.paa"};
	};
	class bldr_SodaCan_Cola: bldr_food
	{
		scope = 2;
		model = "\dz\gear\drinks\SodaCan.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"\dz\gear\drinks\Data\SodaCan_cola_co.paa"};
	};
	class bldr_SodaCan_Spite: bldr_food
	{
		scope = 2;
		model = "\dz\gear\drinks\SodaCan.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"\dz\gear\drinks\Data\SodaCan_spite_co.paa"};
	};
	class bldr_SodaCan_Kvass: bldr_food
	{
		scope = 2;
		model = "\dz\gear\drinks\SodaCan.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"\dz\gear\drinks\Data\SodaCan_rasputin_kvass_co.paa"};
	};
	class bldr_SodaCan_Empty: bldr_food
	{
		scope = 2;
		model = "\dz\gear\drinks\SodaCan_Used.p3d";
	};
	class bldr_BoxCerealCrunchin: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\BoxCereal.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"\dz\gear\food\Data\CerealBox_01_CO.paa"};
	};
	class bldr_PowderedMilk: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\PowderedMilk.p3d";
	};
	class bldr_SmallGuts: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\animal_smallguts.p3d";
	};
	class bldr_Guts: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\animal_guts.p3d";
	};
	class bldr_Rice: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\Rice.p3d";
	};
	class bldr_Marmalade: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\Marmalade.p3d";
	};
	class bldr_BakedBeansCan: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\food_can.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"dz\gear\food\data\deez_beans_co.paa"};
	};
	class bldr_BakedBeansCan_Opened: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\food_can_open.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"dz\gear\food\data\deez_beans_co.paa"};
	};
	class bldr_PeachesCan: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\food_can.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"dz\gear\food\data\canned_peaches_co.paa"};
	};
	class bldr_PeachesCan_Opened: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\food_can_open.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"dz\gear\food\data\canned_peaches_co.paa"};
	};
	class bldr_TacticalBaconCan: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\food_can.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"dz\gear\food\data\tactical_bacon_co.paa"};
	};
	class bldr_TacticalBaconCan_Opened: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\food_can_open.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"dz\gear\food\data\tactical_bacon_co.paa"};
	};
	class bldr_SpaghettiCan: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\food_can.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"dz\gear\food\data\canned_spaghetti_co.paa"};
	};
	class bldr_SpaghettiCan_Opened: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\food_can_open.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"dz\gear\food\data\canned_spaghetti_co.paa"};
	};
	class bldr_SardinesCan: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\can_sardines.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"dz\gear\food\data\sardines_CO.paa"};
	};
	class bldr_SardinesCan_Opened: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\can_sardines_open.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"dz\gear\food\data\sardines_CO.paa"};
	};
	class bldr_TunaCan: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\Tuna.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"dz\gear\food\data\tuna_CO.paa"};
	};
	class bldr_TunaCan_Opened: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\tuna_open.p3d";
		hiddenSelections[] = {"camoGround"};
		hiddenSelectionsTextures[] = {"dz\gear\food\data\tuna_CO.paa"};
	};
	class bldr_Apple: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\apple.p3d";
		hiddenSelections[] = {"cs_raw","cs_boiled","cs_dried"};
		hiddenSelectionsTextures[] = {"dz\gear\food\data\apple_raw_CO.paa","dz\gear\food\data\apple_baked_CO.paa","dz\gear\food\data\apple_baked_CO.paa","dz\gear\food\data\apple_dried_CO.paa","dz\gear\food\data\apple_burnt_CO.paa"};
		hiddenSelectionsMaterials[] = {"dz\gear\food\data\apple_raw.rvmat","dz\gear\food\data\apple_baked.rvmat","dz\gear\food\data\apple_boiled.rvmat","dz\gear\food\data\apple_dried.rvmat","dz\gear\food\data\apple_burnt.rvmat","dz\gear\food\data\apple_rotten.rvmat"};
	};
	class bldr_Banana: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\banana_fresh.p3d";
		hiddenSelections[] = {"cs_raw","cs_rotten"};
		hiddenSelectionsTextures[] = {"dz\gear\food\data\banana_frash_CO.paa","dz\gear\food\data\ban_rotten_CO.paa"};
		hiddenSelectionsMaterials[] = {"dz\gear\food\data\banana_frash.rvmat","dz\gear\food\data\banana_rotten.rvmat"};
	};
	class bldr_Orange: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\orange_fresh.p3d";
		hiddenSelections[] = {"cs_raw","cs_rotten"};
		hiddenSelectionsTextures[] = {"dz\gear\food\data\orange_frash_CO.paa","dz\gear\food\data\orange_rotten_CO.paa"};
		hiddenSelectionsMaterials[] = {"dz\gear\food\data\orange_frash.rvmat","dz\gear\food\data\orange_rotten.rvmat"};
	};
	class bldr_Tomato: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\tomato.p3d";
		hiddenSelections[] = {"cs_raw","cs_burned"};
		hiddenSelectionsTextures[] = {"dz\gear\food\data\tomato_raw_CO.paa","dz\gear\food\data\tomato_baked_CO.paa","dz\gear\food\data\tomato_baked_CO.paa","dz\gear\food\data\tomato_dried_CO.paa","dz\gear\food\data\tomato_burnt_CO.paa"};
		hiddenSelectionsMaterials[] = {"dz\gear\food\data\tomato_raw.rvmat","dz\gear\food\data\tomato_baked.rvmat","dz\gear\food\data\tomato_boiled.rvmat","dz\gear\food\data\tomato_dried.rvmat","dz\gear\food\data\tomato_burnt.rvmat","dz\gear\food\data\tomato_rotten.rvmat"};
	};
	class bldr_GreenBellPepper: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\pepper_green.p3d";
		hiddenSelections[] = {"cs_raw","cs_dried"};
		hiddenSelectionsTextures[] = {"dz\gear\food\data\pepper_green_raw_CO.paa","dz\gear\food\data\pepper_green_baked_CO.paa","dz\gear\food\data\pepper_green_baked_CO.paa","dz\gear\food\data\pepper_green_dried_CO.paa","dz\gear\food\data\pepper_green_burnt_CO.paa"};
		hiddenSelectionsMaterials[] = {"dz\gear\food\data\pepper_green_raw.rvmat","dz\gear\food\data\pepper_green_boiled.rvmat","dz\gear\food\data\pepper_green_baked.rvmat","dz\gear\food\data\pepper_green_dried.rvmat","dz\gear\food\data\pepper_green_burnt.rvmat","dz\gear\food\data\pepper_green_rotten.rvmat"};
	};
	class bldr_Zucchini: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\zucchini.p3d";
		hiddenSelections[] = {"cs_raw","cs_burned"};
		hiddenSelectionsTextures[] = {"dz\gear\food\data\zucchini_raw_CO.paa","dz\gear\food\data\zucchini_baked_CO.paa","dz\gear\food\data\zucchini_baked_CO.paa","dz\gear\food\data\zucchini_dried_CO.paa","dz\gear\food\data\zucchini_burnt_CO.paa"};
		hiddenSelectionsMaterials[] = {"dz\gear\food\data\zucchini_raw.rvmat","dz\gear\food\data\zucchini_boiled.rvmat","dz\gear\food\data\zucchini_baked.rvmat","dz\gear\food\data\zucchini_dried.rvmat","dz\gear\food\data\zucchini_burnt.rvmat","dz\gear\food\data\zucchini_rotten.rvmat"};
	};
	class bldr_Pumpkin: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\Pumpkin_fresh.p3d";
		hiddenSelections[] = {"cs_raw","cs_rotten"};
		hiddenSelectionsTextures[] = {"dz\gear\food\data\pumpkin_fresh_CO.paa","dz\gear\food\data\pumpkin_rotten_CO.paa"};
		hiddenSelectionsMaterials[] = {"dz\gear\food\data\pumpkin_fresh.rvmat","dz\gear\food\data\pumpkin_rotten.rvmat"};
	};
	class bldr_SlicedPumpkin: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\pumpkin_sliced.p3d";
		hiddenSelections[] = {"cs_raw","cs_baked","cs_dried"};
		hiddenSelectionsTextures[] = {"dz\gear\food\data\pumpkin_sliced_raw_CO.paa","dz\gear\food\data\pumpkin_sliced_baked_CO.paa","dz\gear\food\data\pumpkin_sliced_baked_CO.paa","dz\gear\food\data\pumpkin_sliced_dried_CO.paa","dz\gear\food\data\pumpkin_sliced_burnt_CO.paa"};
		hiddenSelectionsMaterials[] = {"dz\gear\food\data\pumpkin_sliced_raw.rvmat","dz\gear\food\data\pumpkin_sliced_boiled.rvmat","dz\gear\food\data\pumpkin_sliced_baked.rvmat","dz\gear\food\data\pumpkin_sliced_dried.rvmat","dz\gear\food\data\pumpkin_sliced_burnt.rvmat","dz\gear\food\data\pumpkin_sliced_rotten.rvmat"};
	};
	class bldr_Potato: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\potato.p3d";
		hiddenSelections[] = {"cs_raw"};
		hiddenSelectionsTextures[] = {"dz\gear\food\data\potato_raw_CO.paa","dz\gear\food\data\potato_baked_CO.paa","dz\gear\food\data\potato_baked_CO.paa","dz\gear\food\data\potato_dried_CO.paa","dz\gear\food\data\potato_burnt_CO.paa"};
		hiddenSelectionsMaterials[] = {"dz\gear\food\data\potato_raw.rvmat","dz\gear\food\data\potato_boiled.rvmat","dz\gear\food\data\potato_baked.rvmat","dz\gear\food\data\potato_dried.rvmat","dz\gear\food\data\potato_burnt.rvmat","dz\gear\food\data\potato_rotten.rvmat"};
	};
	class bldr_Kiwi: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\kiwi_fresh.p3d";
		hiddenSelections[] = {"cs_raw","cs_rotten"};
		hiddenSelectionsTextures[] = {"dz\gear\food\data\kiwi_frash_CO.paa","dz\gear\food\data\kiwi_rotten_CO.paa"};
		hiddenSelectionsMaterials[] = {"dz\gear\food\data\kiwi_frash.rvmat","dz\gear\food\data\kiwi_rotten.rvmat"};
	};
	class bldr_SambucusBerry: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\Sambucus_nigra.p3d";
		hiddenSelections[] = {"cs_raw","cs_dried"};
		hiddenSelectionsTextures[] = {"dz\gear\food\data\sambucus_nigra_CO.paa","dz\gear\food\data\sambucus_nigra_dried_CO.paa","dz\gear\food\data\sambucus_nigra_dried_CO.paa","dz\gear\food\data\sambucus_nigra_dried_CO.paa","dz\gear\food\data\sambucus_nigra_burnt_CO.paa"};
		hiddenSelectionsMaterials[] = {"dz\gear\food\data\sambucus_nigra_raw.rvmat","dz\gear\food\data\sambucus_nigra_boiled.rvmat","dz\gear\food\data\sambucus_nigra_baked.rvmat","dz\gear\food\data\sambucus_nigra_dried.rvmat","dz\gear\food\data\sambucus_nigra_burnt.rvmat","dz\gear\food\data\sambucus_nigra_rotten.rvmat"};
	};
	class bldr_CaninaBerry: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\canina_raw.p3d";
		hiddenSelections[] = {"cs_raw"};
		hiddenSelectionsTextures[] = {"dz\gear\food\data\canina_raw_CO.paa","dz\gear\food\data\canina_baked_CO.paa","dz\gear\food\data\canina_raw_CO.paa","dz\gear\food\data\canina_dried_CO.paa","dz\gear\food\data\canina_baked_CO.paa"};
		hiddenSelectionsMaterials[] = {"dz\gear\food\data\canina_raw.rvmat","dz\gear\food\data\canina_baked.rvmat","dz\gear\food\data\canina_boiled.rvmat","dz\gear\food\data\canina_dried.rvmat","dz\gear\food\data\canina_burnt.rvmat","dz\gear\food\data\canina_rotten.rvmat"};
	};
	class bldr_Cannabis: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\cannabis_seedman.p3d";
		hiddenSelections[] = {"cs_raw"};
		hiddenSelectionsTextures[] = {"dz\gear\food\data\cannabis_seedman_raw_CO.paa","dz\gear\food\data\cannabis_seedman_baked_CO.paa","dz\gear\food\data\cannabis_seedman_raw_CO.paa","dz\gear\food\data\cannabis_seedman_raw_CO.paa","dz\gear\food\data\cannabis_seedman_baked_CO.paa"};
		hiddenSelectionsMaterials[] = {"dz\gear\food\data\cannabis_seedman_raw.rvmat","dz\gear\food\data\cannabis_seedman_baked.rvmat","dz\gear\food\data\cannabis_seedman_raw.rvmat","dz\gear\food\data\cannabis_seedman_dried.rvmat","dz\gear\food\data\cannabis_seedman_burnt.rvmat","dz\gear\food\data\cannabis_seedman_rotten.rvmat"};
	};
	class bldr_Plum: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\plum.p3d";
		hiddenSelections[] = {"cs_raw","cs_baked","cs_boiled","cs_dried","cs_burned"};
		hiddenSelectionsTextures[] = {"dz\gear\food\data\plum_raw_CO.paa","dz\gear\food\data\plum_baked_CO.paa","dz\gear\food\data\plum_boiled_CO.paa","dz\gear\food\data\plum_dried_CO.paa","dz\gear\food\data\plum_burnt_CO.paa"};
		hiddenSelectionsMaterials[] = {"dz\gear\food\data\plum_raw.rvmat","dz\gear\food\data\plum_boiled.rvmat","dz\gear\food\data\plum_baked.rvmat","dz\gear\food\data\plum_dried.rvmat","dz\gear\food\data\plum_burnt.rvmat","dz\gear\food\data\plum_rotten.rvmat"};
	};
	class bldr_Pear: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\pear.p3d";
		hiddenSelections[] = {"cs_raw","cs_dried"};
		hiddenSelectionsTextures[] = {"dz\gear\food\data\pear_raw_CO.paa","dz\gear\food\data\pear_baked_CO.paa","dz\gear\food\data\pear_baked_CO.paa","dz\gear\food\data\pear_dried_CO.paa","dz\gear\food\data\pear_burnt_CO.paa"};
		hiddenSelectionsMaterials[] = {"dz\gear\food\data\pear_raw.rvmat","dz\gear\food\data\pear_boiled.rvmat","dz\gear\food\data\pear_baked.rvmat","dz\gear\food\data\pear_dried.rvmat","dz\gear\food\data\pear_burnt.rvmat","dz\gear\food\data\pear_rotten.rvmat"};
	};
	class bldr_HumanSteakMeat: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\human_meat.p3d";
		hiddenSelections[] = {"cs_raw"};
		hiddenSelectionsTextures[] = {"dz\gear\food\data\meat_steak_raw_CO.paa","dz\gear\food\data\meat_steak_baked_CO.paa","dz\gear\food\data\meat_steak_baked_CO.paa","dz\gear\food\data\meat_steak_dried_CO.paa","dz\gear\food\data\meat_steak_burned_CO.paa"};
		hiddenSelectionsMaterials[] = {"dz\gear\food\data\meat_steak_raw.rvmat","dz\gear\food\data\meat_steak_baked.rvmat","dz\gear\food\data\meat_steak_boiled.rvmat","dz\gear\food\data\meat_steak_dried.rvmat","dz\gear\food\data\meat_steak_burnt.rvmat","dz\gear\food\data\meat_steak_rotten.rvmat"};
	};
	class bldr_GoatSteakMeat: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\meat_steak.p3d";
		hiddenSelections[] = {"cs_raw"};
		hiddenSelectionsTextures[] = {"dz\gear\food\data\meat_steak_raw_CO.paa","dz\gear\food\data\meat_steak_baked_CO.paa","dz\gear\food\data\meat_steak_baked_CO.paa","dz\gear\food\data\meat_steak_dried_CO.paa","dz\gear\food\data\meat_steak_burned_CO.paa"};
		hiddenSelectionsMaterials[] = {"dz\gear\food\data\meat_steak_raw.rvmat","dz\gear\food\data\meat_steak_baked.rvmat","dz\gear\food\data\meat_steak_boiled.rvmat","dz\gear\food\data\meat_steak_dried.rvmat","dz\gear\food\data\meat_steak_burnt.rvmat","dz\gear\food\data\meat_steak_rotten.rvmat"};
	};
	class bldr_MouflonSteakMeat: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\meat_steak.p3d";
		hiddenSelections[] = {"cs_raw"};
		hiddenSelectionsTextures[] = {"dz\gear\food\data\meat_steak_raw_CO.paa","dz\gear\food\data\meat_steak_baked_CO.paa","dz\gear\food\data\meat_steak_baked_CO.paa","dz\gear\food\data\meat_steak_dried_CO.paa","dz\gear\food\data\meat_steak_burned_CO.paa"};
		hiddenSelectionsMaterials[] = {"dz\gear\food\data\meat_steak_raw.rvmat","dz\gear\food\data\meat_steak_baked.rvmat","dz\gear\food\data\meat_steak_boiled.rvmat","dz\gear\food\data\meat_steak_dried.rvmat","dz\gear\food\data\meat_steak_burnt.rvmat","dz\gear\food\data\meat_steak_rotten.rvmat"};
	};
	class bldr_BoarSteakMeat: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\meat_steak.p3d";
		hiddenSelections[] = {"cs_raw"};
		hiddenSelectionsTextures[] = {"dz\gear\food\data\meat_steak_raw_CO.paa","dz\gear\food\data\meat_steak_baked_CO.paa","dz\gear\food\data\meat_steak_baked_CO.paa","dz\gear\food\data\meat_steak_dried_CO.paa","dz\gear\food\data\meat_steak_burned_CO.paa"};
		hiddenSelectionsMaterials[] = {"dz\gear\food\data\meat_steak_raw.rvmat","dz\gear\food\data\meat_steak_baked.rvmat","dz\gear\food\data\meat_steak_boiled.rvmat","dz\gear\food\data\meat_steak_dried.rvmat","dz\gear\food\data\meat_steak_burnt.rvmat","dz\gear\food\data\meat_steak_rotten.rvmat"};
	};
	class bldr_PigSteakMeat: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\meat_steak.p3d";
		hiddenSelections[] = {"cs_raw"};
		hiddenSelectionsTextures[] = {"dz\gear\food\data\meat_steak_raw_CO.paa","dz\gear\food\data\meat_steak_baked_CO.paa","dz\gear\food\data\meat_steak_baked_CO.paa","dz\gear\food\data\meat_steak_dried_CO.paa","dz\gear\food\data\meat_steak_burned_CO.paa"};
		hiddenSelectionsMaterials[] = {"dz\gear\food\data\meat_steak_raw.rvmat","dz\gear\food\data\meat_steak_baked.rvmat","dz\gear\food\data\meat_steak_boiled.rvmat","dz\gear\food\data\meat_steak_dried.rvmat","dz\gear\food\data\meat_steak_burnt.rvmat","dz\gear\food\data\meat_steak_rotten.rvmat"};
	};
	class bldr_DeerSteakMeat: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\meat_steak.p3d";
		hiddenSelections[] = {"cs_raw"};
		hiddenSelectionsTextures[] = {"dz\gear\food\data\meat_steak_raw_CO.paa","dz\gear\food\data\meat_steak_baked_CO.paa","dz\gear\food\data\meat_steak_baked_CO.paa","dz\gear\food\data\meat_steak_dried_CO.paa","dz\gear\food\data\meat_steak_burned_CO.paa"};
		hiddenSelectionsMaterials[] = {"dz\gear\food\data\meat_steak_raw.rvmat","dz\gear\food\data\meat_steak_baked.rvmat","dz\gear\food\data\meat_steak_boiled.rvmat","dz\gear\food\data\meat_steak_dried.rvmat","dz\gear\food\data\meat_steak_burnt.rvmat","dz\gear\food\data\meat_steak_rotten.rvmat"};
	};
	class bldr_WolfSteakMeat: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\meat_steak.p3d";
		hiddenSelections[] = {"cs_raw"};
		hiddenSelectionsTextures[] = {"dz\gear\food\data\meat_steak_raw_CO.paa","dz\gear\food\data\meat_steak_baked_CO.paa","dz\gear\food\data\meat_steak_baked_CO.paa","dz\gear\food\data\meat_steak_dried_CO.paa","dz\gear\food\data\meat_steak_burned_CO.paa"};
		hiddenSelectionsMaterials[] = {"dz\gear\food\data\meat_steak_raw.rvmat","dz\gear\food\data\meat_steak_baked.rvmat","dz\gear\food\data\meat_steak_boiled.rvmat","dz\gear\food\data\meat_steak_dried.rvmat","dz\gear\food\data\meat_steak_burnt.rvmat","dz\gear\food\data\meat_steak_rotten.rvmat"};
	};
	class bldr_BearSteakMeat: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\meat_steak.p3d";
		hiddenSelections[] = {"cs_raw"};
		hiddenSelectionsTextures[] = {"dz\gear\food\data\meat_steak_raw_CO.paa","dz\gear\food\data\meat_steak_baked_CO.paa","dz\gear\food\data\meat_steak_baked_CO.paa","dz\gear\food\data\meat_steak_dried_CO.paa","dz\gear\food\data\meat_steak_burned_CO.paa"};
		hiddenSelectionsMaterials[] = {"dz\gear\food\data\meat_steak_raw.rvmat","dz\gear\food\data\meat_steak_baked.rvmat","dz\gear\food\data\meat_steak_boiled.rvmat","dz\gear\food\data\meat_steak_dried.rvmat","dz\gear\food\data\meat_steak_burnt.rvmat","dz\gear\food\data\meat_steak_rotten.rvmat"};
	};
	class bldr_CowSteakMeat: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\meat_steak.p3d";
		hiddenSelections[] = {"cs_raw"};
		hiddenSelectionsTextures[] = {"dz\gear\food\data\meat_steak_raw_CO.paa","dz\gear\food\data\meat_steak_baked_CO.paa","dz\gear\food\data\meat_steak_baked_CO.paa","dz\gear\food\data\meat_steak_dried_CO.paa","dz\gear\food\data\meat_steak_burned_CO.paa"};
		hiddenSelectionsMaterials[] = {"dz\gear\food\data\meat_steak_raw.rvmat","dz\gear\food\data\meat_steak_baked.rvmat","dz\gear\food\data\meat_steak_boiled.rvmat","dz\gear\food\data\meat_steak_dried.rvmat","dz\gear\food\data\meat_steak_burnt.rvmat","dz\gear\food\data\meat_steak_rotten.rvmat"};
	};
	class bldr_SheepSteakMeat: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\meat_steak.p3d";
		hiddenSelections[] = {"cs_raw"};
		hiddenSelectionsTextures[] = {"dz\gear\food\data\meat_steak_raw_CO.paa","dz\gear\food\data\meat_steak_baked_CO.paa","dz\gear\food\data\meat_steak_baked_CO.paa","dz\gear\food\data\meat_steak_dried_CO.paa","dz\gear\food\data\meat_steak_burned_CO.paa"};
		hiddenSelectionsMaterials[] = {"dz\gear\food\data\meat_steak_raw.rvmat","dz\gear\food\data\meat_steak_baked.rvmat","dz\gear\food\data\meat_steak_boiled.rvmat","dz\gear\food\data\meat_steak_dried.rvmat","dz\gear\food\data\meat_steak_burnt.rvmat","dz\gear\food\data\meat_steak_rotten.rvmat"};
	};
	class bldr_FoxSteakMeat: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\meat_steak.p3d";
		hiddenSelections[] = {"cs_raw"};
		hiddenSelectionsTextures[] = {"dz\gear\food\data\meat_steak_raw_CO.paa","dz\gear\food\data\meat_steak_baked_CO.paa","dz\gear\food\data\meat_steak_baked_CO.paa","dz\gear\food\data\meat_steak_dried_CO.paa","dz\gear\food\data\meat_steak_burned_CO.paa"};
		hiddenSelectionsMaterials[] = {"dz\gear\food\data\meat_steak_raw.rvmat","dz\gear\food\data\meat_steak_baked.rvmat","dz\gear\food\data\meat_steak_boiled.rvmat","dz\gear\food\data\meat_steak_dried.rvmat","dz\gear\food\data\meat_steak_burnt.rvmat","dz\gear\food\data\meat_steak_rotten.rvmat"};
	};
	class bldr_ChickenBreastMeat: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\meat_breast.p3d";
		hiddenSelections[] = {"cs_raw"};
		hiddenSelectionsTextures[] = {"dz\gear\food\data\meat_breast_raw_CO.paa","dz\gear\food\data\meat_breast_baked_CO.paa","dz\gear\food\data\meat_breast_baked_CO.paa","dz\gear\food\data\meat_breast_dried_CO.paa","dz\gear\food\data\meat_breast_burnt_CO.paa"};
		hiddenSelectionsMaterials[] = {"dz\gear\food\data\meat_breast_raw.rvmat","dz\gear\food\data\meat_breast_baked.rvmat","dz\gear\food\data\meat_breast_boiled.rvmat","dz\gear\food\data\meat_breast_dried.rvmat","dz\gear\food\data\meat_breast_burnt.rvmat","dz\gear\food\data\meat_breast_rotten.rvmat"};
	};
	class bldr_RabbitLegMeat: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\meat_leg.p3d";
		hiddenSelections[] = {"cs_raw"};
		hiddenSelectionsTextures[] = {"dz\gear\food\data\meat_leg_raw_CO.paa","dz\gear\food\data\meat_leg_baked_CO.paa","dz\gear\food\data\meat_leg_baked_CO.paa","dz\gear\food\data\meat_leg_dried_CO.paa","dz\gear\food\data\meat_leg_burned_CO.paa"};
		hiddenSelectionsMaterials[] = {"dz\gear\food\data\meat_leg_raw.rvmat","dz\gear\food\data\meat_leg_baked.rvmat","dz\gear\food\data\meat_leg_boiled.rvmat","dz\gear\food\data\meat_leg_dried.rvmat","dz\gear\food\data\meat_leg_burnt.rvmat","dz\gear\food\data\meat_leg_rotten.rvmat"};
	};
	class bldr_CarpFilletMeat: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\carp_fillet.p3d";
		hiddenSelections[] = {"cs_raw"};
		hiddenSelectionsTextures[] = {"dz\gear\food\data\carp_fillet_raw_CO.paa","dz\gear\food\data\carp_fillet_baked_CO.paa","dz\gear\food\data\carp_fillet_boiled_CO.paa","dz\gear\food\data\carp_fillet_dried_CO.paa","dz\gear\food\data\carp_fillet_burnt_CO.paa"};
		hiddenSelectionsMaterials[] = {"dz\gear\food\data\carp_fillet_raw.rvmat","dz\gear\food\data\carp_fillet_baked.rvmat","dz\gear\food\data\carp_fillet_boiled.rvmat","dz\gear\food\data\carp_fillet_dried.rvmat","dz\gear\food\data\carp_fillet_burnt.rvmat","dz\gear\food\data\carp_fillet_rotten.rvmat"};
	};
	class bldr_MackerelFilletMeat: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\mackerel_fillet.p3d";
		hiddenSelections[] = {"cs_raw"};
		hiddenSelectionsTextures[] = {"dz\gear\food\data\mackerel_fillet_raw_CO.paa","dz\gear\food\data\mackerel_fillet_baked_CO.paa","dz\gear\food\data\mackerel_fillet_boiled_CO.paa","dz\gear\food\data\mackerel_fillet_dried_CO.paa","dz\gear\food\data\mackerel_fillet_burnt_CO.paa"};
		hiddenSelectionsMaterials[] = {"dz\gear\food\data\mackerel_fillet_raw.rvmat","dz\gear\food\data\mackerel_fillet_baked.rvmat","dz\gear\food\data\mackerel_fillet_boiled.rvmat","dz\gear\food\data\mackerel_fillet_dried.rvmat","dz\gear\food\data\mackerel_fillet_burnt.rvmat","dz\gear\food\data\mackerel_fillet_rotten.rvmat"};
	};
	class bldr_Lard: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\lard.p3d";
		hiddenSelections[] = {"cs_raw","cs_dried"};
		hiddenSelectionsTextures[] = {"dz\gear\food\data\lard_raw_CO.paa","dz\gear\food\data\lard_baked_CO.paa","dz\gear\food\data\lard_boiled_CO.paa","dz\gear\food\data\lard_dried_CO.paa","dz\gear\food\data\lard_burnt_CO.paa"};
		hiddenSelectionsMaterials[] = {"dz\gear\food\data\lard_raw.rvmat","dz\gear\food\data\lard_baked.rvmat","dz\gear\food\data\lard_boiled.rvmat","dz\gear\food\data\lard_dried.rvmat","dz\gear\food\data\lard_burnt.rvmat","dz\gear\food\data\meat_steak_rotten.rvmat"};
	};
	class bldr_Carp: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\carp_live.p3d";
		hiddenSelections[] = {"cs_raw"};
		hiddenSelectionsTextures[] = {"dz\gear\food\data\carp_live_co.paa"};
		hiddenSelectionsMaterials[] = {"dz\gear\food\data\carp_live.rvmat"};
	};
	class bldr_Sardines: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\sardines_live.p3d";
		hiddenSelections[] = {"cs_raw","cs_dried"};
		hiddenSelectionsTextures[] = {"dz\gear\food\data\sardines_raw_CO.paa","dz\gear\food\data\sardines_baked_CO.paa","dz\gear\food\data\sardines_raw_CO.paa","dz\gear\food\data\sardines_baked_CO.paa","dz\gear\food\data\sardines_burnt_CO.paa"};
		hiddenSelectionsMaterials[] = {"dz\gear\food\data\sardines_raw.rvmat","dz\gear\food\data\sardines_baked.rvmat","dz\gear\food\data\sardines_boiled.rvmat","dz\gear\food\data\sardines_dried.rvmat","dz\gear\food\data\sardines_burnt.rvmat","dz\gear\food\data\sardines_rotten.rvmat"};
	};
	class bldr_Mackerel: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\mackerel_live.p3d";
		hiddenSelections[] = {"cs_raw"};
		hiddenSelectionsTextures[] = {"dz\gear\food\data\mackerel_live_co.paa"};
		hiddenSelectionsMaterials[] = {"dz\gear\food\data\mackerel_live.rvmat"};
	};
	class bldr_Worm: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\bait_worm.p3d";
		hiddenSelections[] = {"cs_raw"};
		hiddenSelectionsTextures[] = {"dz\gear\food\data\bait_co.paa"};
		hiddenSelectionsMaterials[] = {"dz\gear\food\data\bait.rvmat"};
	};
	class bldr_AgaricusMushroom: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\mushroom_agaricus.p3d";
		hiddenSelections[] = {"cs_raw","cs_dried"};
		hiddenSelectionsTextures[] = {"dz\gear\food\data\mushroom_agaricus_raw_CO.paa","dz\gear\food\data\mushroom_agaricus_baked_CO.paa","dz\gear\food\data\mushroom_agaricus_baked_CO.paa","dz\gear\food\data\mushroom_agaricus_dried_CO.paa","dz\gear\food\data\mushroom_agaricus_burnt_CO.paa"};
		hiddenSelectionsMaterials[] = {"dz\gear\food\data\mushroom_agaricus_raw.rvmat","dz\gear\food\data\mushroom_agaricus_baked.rvmat","dz\gear\food\data\mushroom_agaricus_boiled.rvmat","dz\gear\food\data\mushroom_agaricus_dried.rvmat","dz\gear\food\data\mushroom_agaricus_burnt.rvmat","dz\gear\food\data\mushroom_agaricus_rotten.rvmat"};
	};
	class bldr_AmanitaMushroom: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\mushroom_amanita.p3d";
		hiddenSelections[] = {"cs_raw","cs_boiled"};
		hiddenSelectionsTextures[] = {"dz\gear\food\data\mushroom_amanita_raw_CO.paa","dz\gear\food\data\mushroom_amanita_boiled_CO.paa","dz\gear\food\data\mushroom_amanita_baked_CO.paa","dz\gear\food\data\mushroom_amanita_dried_CO.paa","dz\gear\food\data\mushroom_amanita_burnt_CO.paa"};
		hiddenSelectionsMaterials[] = {"dz\gear\food\data\mushroom_amanita_raw.rvmat","dz\gear\food\data\mushroom_amanita_baked.rvmat","dz\gear\food\data\mushroom_amanita_boiled.rvmat","dz\gear\food\data\mushroom_amanita_dried.rvmat","dz\gear\food\data\mushroom_amanita_burnt.rvmat","dz\gear\food\data\mushroom_amanita_rotten.rvmat"};
	};
	class bldr_MacrolepiotaMushroom: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\mushroom_macrolepiota.p3d";
		hiddenSelections[] = {"cs_raw","cs_baked","cs_dried"};
		hiddenSelectionsTextures[] = {"dz\gear\food\data\mushroom_macrolepiota_raw_CO.paa","dz\gear\food\data\mushroom_macrolepiota_baked_CO.paa","dz\gear\food\data\mushroom_macrolepiota_baked_CO.paa","dz\gear\food\data\mushroom_macrolepiota_dried_CO.paa","dz\gear\food\data\mushroom_macrolepiota_burnt_CO.paa"};
		hiddenSelectionsMaterials[] = {"dz\gear\food\data\mushroom_macrolepiota_raw.rvmat","dz\gear\food\data\mushroom_macrolepiota_baked.rvmat","dz\gear\food\data\mushroom_macrolepiota_boiled.rvmat","dz\gear\food\data\mushroom_macrolepiota_dried.rvmat","dz\gear\food\data\mushroom_macrolepiota_burnt.rvmat","dz\gear\food\data\mushroom_macrolepiota_rotten.rvmat"};
	};
	class bldr_LactariusMushroom: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\mushroom_lactarius.p3d";
		hiddenSelections[] = {"cs_raw","cs_dried"};
		hiddenSelectionsTextures[] = {"dz\gear\food\data\mushroom_lactarius_raw_CO.paa","dz\gear\food\data\mushroom_lactarius_baked_CO.paa","dz\gear\food\data\mushroom_lactarius_baked_CO.paa","dz\gear\food\data\mushroom_lactarius_dried_CO.paa","dz\gear\food\data\mushroom_lactarius_burnt_CO.paa"};
		hiddenSelectionsMaterials[] = {"dz\gear\food\data\mushroom_lactarius_raw.rvmat","dz\gear\food\data\mushroom_lactarius_baked.rvmat","dz\gear\food\data\mushroom_lactarius_boiled.rvmat","dz\gear\food\data\mushroom_lactarius_dried.rvmat","dz\gear\food\data\mushroom_lactarius_burnt.rvmat","dz\gear\food\data\mushroom_lactarius_rotten.rvmat"};
	};
	class bldr_PsilocybeMushroom: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\Psilocybe_semilanceata.p3d";
		hiddenSelections[] = {"cs_raw","cs_dried"};
		hiddenSelectionsTextures[] = {"dz\gear\food\data\Psilocybe_semilanceata_CO.paa","dz\gear\food\data\Psilocybe_semilanceata_dried_CO.paa","dz\gear\food\data\Psilocybe_semilanceata_dried_CO.paa","dz\gear\food\data\Psilocybe_semilanceata_dried_CO.paa","dz\gear\food\data\Psilocybe_semilanceata_burnt_CO.paa"};
		hiddenSelectionsMaterials[] = {"dz\gear\food\data\Psilocybe_semilanceata_raw.rvmat","dz\gear\food\data\Psilocybe_semilanceata_baked.rvmat","dz\gear\food\data\Psilocybe_semilanceata_boiled.rvmat","dz\gear\food\data\Psilocybe_semilanceata_dried.rvmat","dz\gear\food\data\Psilocybe_semilanceata_burnt.rvmat","dz\gear\food\data\Psilocybe_semilanceata_rotten.rvmat"};
	};
	class bldr_AuriculariaMushroom: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\mushroom_auricularia.p3d";
		hiddenSelections[] = {"cs_raw"};
		hiddenSelectionsTextures[] = {"dz\gear\food\data\mushroom_auricularia_raw_CO.paa","dz\gear\food\data\mushroom_auricularia_baked_CO.paa","dz\gear\food\data\mushroom_auricularia_boiled_CO.paa","dz\gear\food\data\mushroom_auricularia_dried_CO.paa","dz\gear\food\data\mushroom_auricularia_burnt_CO.paa"};
		hiddenSelectionsMaterials[] = {"dz\gear\food\data\mushroom_auricularia_raw.rvmat","dz\gear\food\data\mushroom_auricularia_baked.rvmat","dz\gear\food\data\mushroom_auricularia_boiled.rvmat","dz\gear\food\data\mushroom_auricularia_dried.rvmat","dz\gear\food\data\mushroom_auricularia_burnt.rvmat","dz\gear\food\data\mushroom_auricularia_rotten.rvmat"};
	};
	class bldr_BoletusMushroom: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\Mushroom_boletus.p3d";
		hiddenSelections[] = {"cs_raw","cs_dried"};
		hiddenSelectionsTextures[] = {"dz\gear\food\data\mushroom_boletus_raw_CO.paa","dz\gear\food\data\mushroom_boletus_boiled_CO.paa","dz\gear\food\data\mushroom_boletus_boiled_CO.paa","dz\gear\food\data\mushroom_boletus_burnt_CO.paa"};
		hiddenSelectionsMaterials[] = {"dz\gear\food\data\mushroom_boletus_raw.rvmat","dz\gear\food\data\mushroom_boletus_baked.rvmat","dz\gear\food\data\mushroom_boletus_boiled.rvmat","dz\gear\food\data\mushroom_boletus_dried.rvmat","dz\gear\food\data\mushroom_boletus_burnt.rvmat","dz\gear\food\data\mushroom_boletus_rotten.rvmat"};
	};
	class bldr_PleurotusMushroom: bldr_food
	{
		scope = 2;
		model = "\dz\gear\food\mushroom_pleurotus.p3d";
		hiddenSelections[] = {"cs_raw","cs_boiled","cs_dried"};
		hiddenSelectionsTextures[] = {"dz\gear\food\data\mushroom_pleurotus_raw_CO.paa","dz\gear\food\data\mushroom_pleurotus_baked_CO.paa","dz\gear\food\data\mushroom_pleurotus_baked_CO.paa","dz\gear\food\data\mushroom_pleurotus_dried_CO.paa","dz\gear\food\data\mushroom_pleurotus_burnt_CO.paa"};
		hiddenSelectionsMaterials[] = {"dz\gear\food\data\mushroom_pleurotus_raw.rvmat","dz\gear\food\data\mushroom_pleurotus_baked.rvmat","dz\gear\food\data\mushroom_pleurotus_boiled.rvmat","dz\gear\food\data\mushroom_pleurotus_dried.rvmat","dz\gear\food\data\mushroom_pleurotus_burnt.rvmat","dz\gear\food\data\mushroom_pleurotus_rotten.rvmat"};
	};
	class bldr_KitchenKnife: bldr_food
	{
		scope = 2;
		model = "\dz\gear\tools\kitchenknife.p3d";
	};
	class bldr_MeatTenderizer: bldr_food
	{
		scope = 2;
		model = "\dz\gear\tools\Meat_Tenderizer.p3d";
	};
	class bldr_Pot: bldr_food
	{
		scope = 2;
		model = "\dz\gear\cooking\CookingPot.p3d";
	};
	class bldr_MediumGasCanister: bldr_food
	{
		scope = 2;
		model = "\dz\gear\consumables\GasCannisterMedium.p3d";
	};
	class bldr_PortableGasStove: bldr_food
	{
		scope = 2;
		model = "\dz\gear\cooking\GasCooker.p3d";
	};
	class bldr_LargeGasCanister: bldr_food
	{
		scope = 2;
		model = "\dz\gear\consumables\GasCannisterLarge.p3d";
	};
	class bldr_SmallGasCanister: bldr_food
	{
		scope = 2;
		model = "\dz\gear\consumables\GasCannisterSmall.p3d";
	};
	class bldr_PortableGasLamp: bldr_food
	{
		scope = 2;
		model = "\dz\gear\cooking\GasLight.p3d";
	};
	class bldr_DisinfectantSpray: bldr_food
	{
		scope = 2;
		model = "\dz\gear\medical\DisinfectantSpray.p3d";
	};
	class bldr_DisinfectantAlcohol: bldr_food
	{
		scope = 2;
		model = "\dz\gear\medical\alfa.p3d";
	};
	class bldr_VitaminBottle: bldr_food
	{
		scope = 2;
		model = "\dz\gear\medical\VitaminBottle.p3d";
	};
	class bldr_CharcoalTablets: bldr_food
	{
		scope = 2;
		model = "\dz\gear\medical\charcoal_tablets.p3d";
	};
	class bldr_PainkillerTablets: bldr_food
	{
		scope = 2;
		model = "\dz\gear\medical\painkillers2.p3d";
	};
	class bldr_TetracyclineAntibiotics: bldr_food
	{
		scope = 2;
		model = "\dz\gear\medical\tetracycline.p3d";
	};
	class bldr_megaphone: bldr_food
	{
		scope = 2;
		model = "\dz\gear\radio\megaphone.p3d";
	};
};
