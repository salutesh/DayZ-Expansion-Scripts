/**
 * Land_House_1W04.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class Land_House_1W04
{
	override bool HasInterior()
	{
		return true;
	}
	
	override void SpawnInterior()
	{
		/*
		SpawnInteriorObject( "bldr_almara_open", "0.683629 -2.660004 3.491054", "91.000015 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_flowers_02", "0.644099 -2.660004 0.250412", "-110.986610 0.000000 0.000000", 0.9 );
		SpawnInteriorObject( "bldr_kitchen_unit_a_dz", "-1.991846 -2.661865 -0.849195", "-0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_case_sink_a", "-3.070000 -2.667610 -0.900000", "-90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_couch_beige84x213x75_dz", "0.655243 -2.673088 -4.297978", "89.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_case_bedroom_a", "0.616695 -2.391617 -2.916242", "-4.999999 0.000000 0.000000" );

		if ( false )
		{
			SpawnInteriorObject( "bldr_radiator", "1.120000 -2.455536 -1.666310", "89.000000 0.000000 0.000000" );
		}
		else
		{
			SpawnInteriorObject( "bldr_radiator", "1.120000 -2.455536 -1.666310", "89.000000 0.000000 0.000000" );
		}

		SpawnInteriorObject( "bldr_radiator", "-4.878163 -2.459999 -2.965701", "90.999954 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_picture_b", "1.080000 -1.823509 -2.800000", "-90.009995 0.000000 -0.009981" );
		SpawnInteriorObject( "bldr_luxury_light", "-1.967442 -0.458290 -2.995982", "-0.000007 -0.005523 0.074255" );
		SpawnInteriorObject( "bldr_mil_radar_tall_planks", "1.214637 -1.400085 -4.357572", "0.000000 0.000000 90.000000", -1 );
		SpawnInteriorObject( "bldr_mil_radar_tall_planks", "1.220000 -1.410004 -1.450000", "0.000000 0.000000 -90.000000", -1 );
		SpawnInteriorObject( "ExpansionPropRadio", "0.702000 -2.071884 -3.670000", "159.000031 -0.933787 -31.045103" );
		SpawnInteriorObject( "ExpansionPropBag1", "0.812762 -2.399963 3.146035", "-75.000130 -1.443049 0.002358" );
		SpawnInteriorObject( "ExpansionPropGrill1", "-1.341195 -2.153999 4.533344", "28.000010 0.000000 0.000000", 0.15 );
		SpawnInteriorObject( "ExpansionPropLeafBlower", "-0.577252 -2.559998 4.827191", "11.000003 0.000000 -17.000006", 0.65 );
		SpawnInteriorObject( "ExpansionPropWateringCan", "0.547237 -2.447998 4.689959", "-151.000031 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "0.672255 -2.639999 1.614346", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_carpet_big", "0.017100 -2.621559 -3.890896", "2.999948 0.000000 0.000000", 0.5 );
		SpawnInteriorObject( "bldr_debris_small_house", "-1.350706 -2.649979 1.435943", "1.000000 0.000000 0.000000", 0.5 );
		SpawnInteriorObject( "bldr_toilet_b_02", "-3.497152 -2.669998 -0.002972", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "PissGround", "-3.730948 -2.660004 0.451986", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "ShitGround", "-3.662222 -2.650002 0.487338", "0.036557 1.701053 1.231318" );
		SpawnInteriorObject( "ShitGround", "-3.798539 -2.634476 0.406551", "65.000000 0.000000 0.000000" );
		SpawnInteriorObject( "ShitGround", "-3.510899 -2.481972 0.073470", "-0.000669 5.069169 40.446159" );
		*/


	}

	override bool HasIvys()
	{
		return true;
	}
	
	override void SpawnIvys()
	{
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "1.409184 -0.134506 0.354783", "-90.000015 -89.000046 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "1.400000 -2.550156 -3.100001", "-89.999954 84.000008 -179.999954" );
		SpawnInteriorIvy( "bldr_plnt_ivy_A", "1.649999 0.250015 -9.250010", "0.000000 0.000000 89.999977" );
		SpawnInteriorIvy( "bldr_plnt_ivy_K", "1.491185 -1.182076 -5.901031", "179.999954 0.000000 90.000015" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_A", "-0.967063 0.834297 -9.324474", "91.000031 -27.999992 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-5.000003 -2.600037 -9.500001", "0.000000 89.000046 179.999939" );
		SpawnInteriorIvy( "bldr_plnt_ivy_J", "0.750000 -1.650101 -9.500001", "-90.000000 0.000000 94.000031" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-5.250005 0.049942 -9.400009", "90.000000 -89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-5.150001 -0.100006 -2.450001", "-89.999992 -89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_A", "-5.159508 0.151886 1.354815", "8.000011 1.000000 89.999992" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "-4.299997 -2.600098 7.050000", "90.000038 88.000015 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_K", "-5.213439 -0.213188 4.122277", "90.000038 -89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_K", "0.300000 -0.149948 6.649999", "179.999954 -84.000008 90.000038" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "2.126521 0.201477 -9.462601", "-94.000031 34.000031 179.000061" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-1.649999 2.931259 -6.100004", "89.999992 -50.000015 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "-1.503403 2.711403 -1.277132", "90.000000 -38.000015 -179.999954" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_A", "-0.400000 1.700043 -0.500000", "0.000000 -3.000000 -55.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-1.733271 2.920593 2.150462", "90.000038 -50.000004 179.999954" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_A", "-2.949998 0.750107 4.999992", "-90.000000 -25.999992 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-1.938738 3.019592 5.432559", "-90.000000 -32.000008 -2.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-2.000001 2.850052 -0.150000", "-90.000015 -38.000015 178.999924" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-1.896445 3.065399 -5.564150", "-92.000031 -54.000004 179.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_roof_A", "-5.833691 0.167999 -0.023561", "179.999985 -86.000015 -96.999931" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "1.351174 -3.811775 3.310491", "90.000000 0.000000 0.000000" );
	}

}