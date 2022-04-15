/**
 * Land_House_1W06.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class Land_House_1W06
{
	override bool HasInterior()
	{
		return true;
	}
	
	override void SpawnInterior()
	{
		SpawnInteriorObject( "bldr_case_sink_a", "-2.314103 -0.849747 -0.299968", "-179.999985 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_kitchen_unit_a_dz", "-2.340000 -0.970001 0.775000", "-90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_light_kitchen_03", "-0.812399 0.750000 0.085730", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_carpet_big", "1.578589 -0.899750 -1.117179", "0.008217 0.000000 0.000000", -1 );

		//if ( Math.RandomFloatInclusive(0, 1) <= 0.7 )
		//{
		//	SpawnInteriorObject( "bldr_almara", "0.934000 -0.949997 -3.290503", "-90.000000 0.000000 0.000000" );
		//}
		//else
		{
			SpawnInteriorObject( "bldr_almara_open", "0.934000 -0.949997 -3.290000", "-90.000000 0.000000 0.000000" );
		}

		//SpawnInteriorObject( "bldr_sofacorner", "3.312014 -0.499725 -0.863802", "-89.991875 1.027026 0.458441", 0.35 );
		SpawnInteriorObject( "bldr_switch_lights", "0.372679 0.430786 2.050000", "90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_luxury_lamp", "4.955000 1.104851 -2.533125", "90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_flowers_01", "4.908828 -0.919304 1.100783", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_picture_d_02", "-2.600000 -0.169250 -0.153493", "90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_mil_radar_tall_planks", "-2.739435 0.700043 0.424320", "0.000000 0.000000 90.000000", -1 );
		SpawnInteriorObject( "bldr_mil_radar_tall_planks", "5.273593 0.700043 -1.754112", "0.000000 0.000000 -90.000000", -1 );
		SpawnInteriorObject( "bldr_mil_radar_tall_planks", "5.273589 0.400024 -3.481367", "-0.000009 65.000015 90.000000", -1 );
		SpawnInteriorObject( "bldr_debris_small_house", "3.023967 -0.898956 0.478242", "-69.000000 0.000000 0.000000", 0.4 );
		SpawnInteriorObject( "bldr_debris_small_house", "2.554142 -0.930000 -2.146894", "-147.977005 0.000000 0.000000", 0.4 );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-1.576723 -2.773422 2.942936", "81.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_picture_e_02", "2.050000 -0.319267 2.946911", "-90.000038 0.000000 0.000000", 0.85 );

		if ( Math.RandomFloatInclusive(0, 1) <= 0.15 )
		{
			SpawnInteriorObject( "bldr_picture_d_02", "1.513177 -0.449966 3.750000", "-179.999985 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_picture_a_02", "0.806986 -0.349960 3.750000", "-179.999985 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_picture_e_02", "-0.223334 -0.649979 3.750000", "-179.999985 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_picture_a", "1.208379 -1.050000 3.750000", "179.999924 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_picture_c", "0.533194 -0.929997 3.755000", "179.999985 0.000000 -0.000009" );
			SpawnInteriorObject( "bldr_picture_e", "0.298594 -1.450024 3.753000", "-179.998993 0.000000 -0.000005" );
			SpawnInteriorObject( "bldr_picture_f", "-0.381654 -1.650000 3.755000", "179.999985 0.000000 -0.001006" );
		}


	}
	
	override bool HasIvys()
	{
		return true;
	}
	
	override void SpawnIvys()
	{
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "2.319123 3.931000 -1.595183", "179.999985 -41.000011 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-2.767852 3.981003 -1.437786", "-179.000000 -26.000008 -0.000036" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-2.949998 -1.000061 -4.800003", "178.999924 89.000046 -0.000009" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-2.850000 1.800110 -4.599999", "-89.999954 -89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "-2.840074 2.650162 -2.818409", "0.000000 -65.000015 90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_K", "-1.500000 1.600159 -4.699995", "179.999985 -89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_K", "3.450000 0.538605 -4.699999", "179.999985 0.000000 90.000038" );
		SpawnInteriorIvy( "bldr_plnt_ivy_A", "3.400001 1.600098 -4.950004", "92.000031 0.000000 90.000038" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "2.450001 1.550095 4.200000", "90.000015 0.000000 -90.000122" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "0.500000 0.050003 3.999996", "179.999954 -89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_A", "5.285198 2.030876 -0.367841", "0.000000 -14.999999 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "0.804495 3.931000 -1.130464", "-3.999977 -44.000008 -177.999969" );
		SpawnInteriorIvy( "bldr_plnt_ivy_S", "4.732361 3.530968 -0.613080", "-105.999969 0.000000 -152.000015" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_A", "3.485373 2.930931 0.286625", "-88.000000 -3.000000 -58.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "0.049587 -0.300018 8.365353", "171.999954 -86.000015 4.999992" );
		SpawnInteriorIvy( "bldr_plnt_ivy_K", "1.561468 1.600098 4.459654", "0.013445 -29.999987 -179.999954" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "5.499993 -0.900055 -4.549999", "-90.000038 89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_A", "5.415384 -0.919296 -4.892275", "-89.999969 0.000000 -90.000046" );
		SpawnInteriorIvy( "bldr_plnt_ivy_A", "4.990726 -2.750168 2.220736", "-8.000003 0.000000 -92.000031" );
		SpawnInteriorIvy( "bldr_plnt_ivy_A", "-2.606766 1.700104 2.056493", "4.000000 0.000000 90.000015" );
	}
}