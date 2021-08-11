/**
 * Land_House_1W09.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class Land_House_1W09
{
	override bool HasInterior()
	{
		return true;
	}
	
	override void SpawnInterior()
	{
		/*
		SpawnInteriorObject( "bldr_light_kitchen_03", "1.572565 -0.604424 2.367278", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_case_cans_b", "2.546235 -1.954506 0.991109", "-90.000038 0.000000 0.000000" );
		SpawnInteriorObject( "ExpansionPropBag1", "1.702037 0.247002 4.138688", "32.000004 0.000000 0.000000", 0.7 );
		SpawnInteriorObject( "ExpansionPropGrill1", "0.640566 -1.453003 4.168621", "-24.986521 1.680066 0.458564", 0.1 );
		SpawnInteriorObject( "bldr_switch_lights", "2.860000 -0.754433 1.604139", "179.998993 0.000000 0.000000" );
		SpawnInteriorObject( "ExpansionPropPaintBrush", "1.858372 -1.871002 0.711321", "-18.000006 0.000000 0.000000", 0.4 );
		SpawnInteriorObject( "ExpansionPropChair1", "1.170000 -1.309998 4.134262", "0.000000 0.000000 10.000004" );
		SpawnInteriorObject( "bldr_kitchendesk_a", "-4.856218 -0.900002 0.585000", "-90.000015 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_kitchendesk_b", "-3.460000 -0.899902 0.537000", "-90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_kitchenstove_elec", "-6.584237 -1.959999 1.179849", "-103.999969 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_switch_lights", "0.077000 -0.704430 1.456751", "179.998993 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_picture_a_03", "-3.559272 -1.350021 4.450000", "179.998993 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_carpet_big", "-1.811782 -1.950058 -2.085584", "0.000000 0.000000 0.000000", 0.9 );
		SpawnInteriorObject( "bldr_couch_beige84x213x75_dz", "-3.147839 -1.954506 -2.208054", "-92.000015 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_case_a", "-2.964501 -1.954506 -0.230000", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_flowers_02", "-0.376865 -1.954506 -2.942820", "-110.000038 0.000000 0.000000", 0.9 );
		SpawnInteriorObject( "bldr_flowers_01", "-0.352872 -1.480003 -1.337517", "0.000000 0.000000 0.000000", 0.9 );
		SpawnInteriorObject( "bldr_rack_dz", "-2.019881 -1.954506 -4.200000", "-90.000000 0.000000 0.000000" );

		if ( Math.RandomFloatInclusive(0, 1) <= 0.7 )
		{
			SpawnInteriorObject( "bldr_BookTheShunnedHouse", "-1.379000 -0.720001 -4.259225", "90.000000 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_BookTheHoundOfTheBaskervilles", "-1.435000 -0.720001 -4.272530", "90.000000 -0.400000 0.000000" );
			SpawnInteriorObject( "bldr_BookAndersen", "-1.490000 -0.720001 -4.253277", "90.000000 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_BookTonyAndTheBeetles", "-1.545000 -0.720001 -4.266625", "90.000000 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_BookTheCosmicComputer", "-1.599000 -0.720001 -4.235653", "90.000000 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_BookATravelerInTime", "-2.077157 -0.834999 -4.236760", "90.000000 89.000046 -0.000009" );
			SpawnInteriorObject( "bldr_BookTheVariableMan", "-1.947377 -0.775002 -4.247050", "90.000000 61.000015 0.000000" );
			SpawnInteriorObject( "bldr_BookTheOutlet", "-1.841974 -0.761604 -4.261828", "90.000000 55.000019 0.000000" );
			SpawnInteriorObject( "bldr_BookTheBarrier", "-1.740000 -0.745003 -4.233519", "90.000000 42.999992 0.000000" );
			SpawnInteriorObject( "bldr_BookFlyingURanch", "-1.380000 -1.044998 -4.250000", "90.000038 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_BookCassidysRustlerRoundUp", "-1.435000 -1.044998 -4.240000", "90.000000 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_BookLonesomeLand", "-1.490000 -1.044998 -4.231027", "90.000000 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_BookTrailin", "-1.545000 -1.044998 -4.245000", "90.000000 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_BookBettyZane", "-1.600000 -1.044998 -4.225868", "90.000000 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_BookATexasMatchmaker", "-1.655000 -1.044998 -4.259460", "90.000000 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_BookTheThunderBird", "-1.710000 -1.044998 -4.232437", "90.000000 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_BookBlackJack", "-1.765000 -1.044998 -4.219049", "90.000000 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_BookTheLastTrail", "-1.820000 -1.044998 -4.253143", "90.000000 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_BookSelectedStories", "-1.875000 -1.044998 -4.269049", "90.000000 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_BookTheHeritageOfTheSioux", "-1.930000 -1.044998 -4.260000", "90.000000 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_BookDesertGold", "-1.985000 -1.044998 -4.250001", "90.000000 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_BookTheHiddenChildren", "-2.039612 -1.044998 -4.281088", "90.000000 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_BookTheLastOfThePlainsmen", "-2.095000 -1.044998 -4.249544", "90.000000 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_BookGunmansReckoning", "-2.150000 -1.044998 -4.250001", "90.000000 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_BookSkyrider", "-2.203000 -1.044998 -4.234354", "90.000000 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_BookSpinifexAndSand", "-2.325000 -1.053001 -4.250001", "90.000000 -31.999987 0.000000" );
			SpawnInteriorObject( "bldr_BookSunsetPass", "-2.552676 -1.135002 -4.254829", "90.000000 -81.000000 -0.000009" );
			SpawnInteriorObject( "bldr_BookThreeMenInABoat", "-2.662198 -1.400002 -4.244796", "90.000000 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_Book20000LieuesSousLesMers", "-2.608000 -1.400002 -4.240000", "90.000000 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_BookLesCorneilles", "-2.554000 -1.400002 -4.249547", "90.000000 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_BookLaComdieHumaine", "-2.500000 -1.400002 -4.252687", "90.000000 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_BookLleMystrieuse", "-2.446000 -1.400002 -4.231800", "90.000000 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_BookDeLaTerreLaLune", "-2.392000 -1.400002 -4.250000", "90.000000 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_BookFrankenstein", "-2.340000 -1.400002 -4.253479", "90.000000 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_BookRomeoUndJulia", "-2.286000 -1.400002 -4.232485", "90.000000 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_BookVoyageAuCentreDeLaTerre", "-2.232000 -1.400002 -4.250580", "90.000000 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_BookLenfer1Of2", "-2.179000 -1.400002 -4.247495", "90.000000 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_BookFaustErsterTeil", "-2.108805 -1.400002 -4.220000", "90.000000 7.000000 0.000000" );
			SpawnInteriorObject( "bldr_BookTheLastMan", "-1.826142 -1.513000 -4.211511", "23.999968 -90.000000 -0.000009" );
			SpawnInteriorObject( "bldr_BookRomeoUndJulia", "-1.380000 -1.724998 -4.220641", "90.000000 -1.000000 0.000000" );
			SpawnInteriorObject( "bldr_BookDracula", "-2.394730 -1.836998 -4.195959", "174.999924 -90.000000 -0.000009" );
			SpawnInteriorObject( "bldr_BookWutheringHeights", "-1.473000 -1.724998 -4.239610", "90.000000 -18.999992 0.000000" );
			SpawnInteriorObject( "bldr_BookRussianCheatSheet", "-1.793502 -1.929443 -3.770924", "40.999969 -90.000000 179.999985" );
		}

		SpawnInteriorObject( "bldr_BookTonyAndTheBeetles", "-3.165016 -1.483101 -2.873083", "26.999937 90.000000 -0.000009", 0.4 );

		//if ( Math.RandomFloatInclusive(0, 1) <= 0.75 )
		//{
		//	SpawnInteriorObject( "bldr_case_a_open", "-4.915122 -1.954506 -4.050000", "179.998993 0.000000 0.000000" );
		//}
		//else
		{
			SpawnInteriorObject( "bldr_case_a", "-4.915122 -1.954506 -4.050000", "179.998993 0.000000 0.000000" );
		}
		
		SpawnInteriorObject( "bldr_radiator", "-4.654672 -1.804497 0.141289", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_radiator", "-3.673578 -1.820000 4.470000", "0.036726 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_switch_lights", "-2.330562 -0.654427 0.092000", "90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_flowers_02", "-0.346023 -1.950058 0.847006", "-94.963440 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_flowers_01", "-0.157250 -0.554420 4.185798", "162.000046 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_kitchen_unit_a_dz", "-1.150362 -1.959999 4.190000", "0.000000 0.000000 0.000000", 0.75 );
		SpawnInteriorObject( "bldr_debris_small_house", "-5.230020 -1.980003 1.720814", "-131.000122 0.000000 0.000000", -1 );
		SpawnInteriorObject( "bldr_debris_small_house", "-0.816571 -1.980003 2.432009", "-15.999955 0.000000 0.000000", -1 );

		if ( Math.RandomFloatInclusive(0, 1) <= 0.7 )
		{
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "2.076464 -1.928123 2.365288", "0.000000 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "1.120832 -1.949997 1.765288", "2.030414 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "1.657081 -1.949997 3.115287", "44.000000 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-3.570196 -1.928123 -3.635503", "95.000031 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-4.122046 -1.949997 -2.832530", "49.000000 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-2.610860 -1.928123 3.910599", "0.000000 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-4.510860 -1.928123 4.091320", "51.030415 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-3.660859 -1.949997 3.243356", "51.030415 0.000000 0.000000" );
		}
		
		SpawnInteriorObject( "bldr_mil_radar_tall_planks", "-4.739001 -0.200012 4.650000", "90.000000 -4.000000 90.000000", -1 );
		SpawnInteriorObject( "bldr_mil_radar_tall_planks", "-2.599999 -0.450012 4.650000", "90.000000 0.000000 -90.000000", -1 );
		SpawnInteriorObject( "bldr_mil_radar_tall_planks", "-7.126760 -0.800034 -0.088524", "-179.999924 0.000000 90.000000", -1 );
		SpawnInteriorObject( "bldr_mil_radar_tall_planks", "-4.399990 -0.849991 -4.599999", "90.000000 40.999996 90.000000", -1 );
		*/


	}

	override bool HasIvys()
	{
		return true;
	}
	
	override void SpawnIvys()
	{
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-3.383186 3.245811 -1.459863", "89.999969 -40.000011 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_K", "-3.425065 3.518639 4.104299", "90.000000 -40.000015 -0.000009" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "-3.107292 3.145805 2.226408", "88.000023 -40.000011 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-3.660125 3.318665 -4.719347", "-90.000038 -34.999992 179.999924" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-3.763582 3.338005 1.553750", "-90.000000 -40.999996 178.000046" );
		SpawnInteriorIvy( "bldr_plnt_ivy_roof_B", "-7.427958 0.912537 4.836975", "142.000061 -68.000053 -93.000229" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_A", "-6.246202 1.570137 1.583366", "179.999954 -5.000000 -54.999989" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_A", "-0.839552 1.750107 2.174736", "-12.000016 4.000000 -79.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_H", "-0.889199 1.795723 -0.595681", "57.000214 -8.000001 -173.000031" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-1.678373 1.895729 -4.836409", "0.000000 0.000000 -167.000015" );
		SpawnInteriorIvy( "bldr_plnt_ivy_G", "0.383100 1.645714 3.597863", "-13.000230 0.000000 -168.999969" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-7.562963 -1.850113 4.708426", "89.000015 86.000015 -178.000031" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-7.312962 0.500031 1.123248", "90.000000 -89.000046 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_G", "-7.212961 -2.600159 1.625574", "90.000038 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "-7.212961 -3.400208 3.286091", "90.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_A", "-7.312962 0.450027 -4.828626", "10.000006 0.000000 90.000038" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_B", "-7.212961 -2.149826 -3.861436", "-90.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "3.638582 0.450027 -1.228045", "-90.000000 -89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_H", "-3.590033 1.500092 -4.865680", "-90.000038 -28.999990 -90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "1.051785 0.550034 -4.915680", "-89.000000 -83.000015 90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_K", "-5.935234 0.650040 -4.732983", "179.999985 -89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "3.210000 -3.300201 -4.042555", "-90.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_G", "3.238024 -3.200195 -3.312565", "-90.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_G", "2.600138 -2.850174 -4.800000", "0.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_A", "3.226426 0.700043 4.223977", "-174.999939 0.000000 90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_H", "-1.661875 0.700043 4.901750", "-0.000009 -89.000046 -0.000009" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "0.814565 1.100067 5.031040", "-89.999954 -79.000015 -89.999992" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "-6.533463 -2.800171 4.751749", "179.999954 0.000000 0.000000" );
	}

}