/**
 * Land_House_2W02.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class Land_House_2W02
{
	override bool HasInterior()
	{
		return true;
	}
	
	override bool HasIvys()
	{
		return true;
	}
	
	override void SpawnInterior()
	{
		SpawnInteriorObject( "bldr_couch_beige84x213x75_dz", "4.248044 -4.097450 -3.877409", "179.000000 0.000000 0.000000" );

		if ( false )
		{
			SpawnInteriorObject( "bldr_library_a", "6.545419 -1.295158 4.230604", "179.990005 0.000000 0.000000" );
		}
		else
		{
			SpawnInteriorObject( "bldr_library_a_open", "6.545419 -1.295158 4.230604", "179.990005 0.000000 0.000000" );
		}

		if ( false )
		{
			SpawnInteriorObject( "bldr_library_a", "5.642000 -1.295158 4.230500", "179.998993 0.000000 0.000000" );
		}
		else
		{
			SpawnInteriorObject( "bldr_library_a_open", "5.642000 -1.295158 4.230500", "179.998993 0.000000 0.000000" );
		}
		
		SpawnInteriorObject( "bldr_picture_g", "7.544000 -0.471901 0.670000", "-0.000009 0.000000 -0.000009" );
		SpawnInteriorObject( "bldr_picture_a_05", "9.070000 -0.595116 3.647130", "-90.000015 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_luxury_lamp", "8.481004 0.428978 4.278196", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_luxury_lamp", "8.481000 0.428993 0.815000", "179.998993 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_switch_lights", "5.043000 0.004921 2.292696", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_switch_lights", "-1.507000 -2.686554 1.934380", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_luxury_lamp", "-1.364500 -2.323311 2.450483", "-90.000038 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_luxury_lamp", "-1.364500 -2.323303 -0.445668", "-90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_picture_a_02", "0.750000 -3.497414 -1.585022", "90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_picture_d", "0.739000 -3.497414 -0.546726", "90.000038 0.000000 -4.000000" );


		if ( false )
		{
			SpawnInteriorObject( "bldr_library_a", "2.280000 -4.047447 -4.150000", "0.000000 0.000000 0.000000" );
		}
		else
		{
			SpawnInteriorObject( "bldr_library_a_open", "2.280000 -4.047447 -4.150000", "0.000000 0.000000 0.000000" );
		}

		SpawnInteriorObject( "bldr_carpet_2_dz", "7.126932 -1.217682 2.134963", "-3.000013 0.000000 0.000000", 0.6 );

		if ( false )
		{
			SpawnInteriorObject( "bldr_case_a", "6.170000 -1.300003 -2.070000", "179.990005 0.000000 0.000000" );
		}
		else
		{
			SpawnInteriorObject( "bldr_case_a_open", "6.170000 -1.300003 -2.070000", "179.998993 0.000000 0.000000" );
		}

		SpawnInteriorObject( "bldr_luxury_lamp", "8.290000 0.378975 0.335000", "0.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_luxury_lamp", "8.290000 0.378998 -2.229000", "179.998993 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_switch_lights", "6.642851 -0.045082 0.476000", "90.000000 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_picture_a", "7.429640 -0.495134 0.490000", "179.998993 0.000000 -0.000009" );
		SpawnInteriorObject( "bldr_case_cans_b", "2.612231 -1.295158 2.050000", "179.899979 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_picture_a_03", "4.869000 -0.845131 -1.576067", "-90.000038 0.000000 0.000000" );

		if ( false )
		{
			SpawnInteriorObject( "bldr_library_a", "4.710000 -1.300003 -0.152416", "-90.000000 0.000000 0.000000" );
		}
		else
		{
			SpawnInteriorObject( "bldr_library_a_open", "4.710000 -1.300003 -0.142400", "-90.000000 0.000000 0.000000" );
		}

		SpawnInteriorObject( "bldr_radiator", "6.860000 -3.897438 0.604682", "90.000038 0.000000 0.000000" );
		SpawnInteriorObject( "bldr_picture_c", "1.400000 -0.695125 -2.029875", "90.000000 0.000000 -0.000009" );
		SpawnInteriorObject( "bldr_picture_f", "4.295431 -3.347423 -4.320000", "-0.000000 0.000000 -0.000009" );


		//if ( Math.RandomFloatInclusive(0, 1) <= Math.RandomFloatInclusive(0, 1) )
		//{
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "2.108341 -1.270897 -2.917648", "-85.000053 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "3.141235 -1.269997 -2.692839", "156.000031 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "4.169188 -1.269997 -2.805341", "92.000015 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "2.147810 -1.268776 -2.103525", "0.000000 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "1.217381 -4.021065 -3.703478", "90.000000 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "2.089676 -4.021065 -3.419114", "-134.999954 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-1.850765 -4.010002 1.128104", "0.000000 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-1.110903 -4.019997 0.246711", "54.000000 0.000000 0.000000" );
			SpawnInteriorObject( "bldr_plnt_c_leafs_spread", "-0.627217 -4.019997 1.626612", "-154.724213 0.000000 0.000000" );
		//}
	
	}
	
	override void SpawnIvys()
	{
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "3.913041 4.357605 1.247453", "-0.000009 -36.000011 -179.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_A", "1.722698 3.655144 1.847986", "-91.000031 0.000000 -53.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "-1.334806 4.153053 1.574800", "0.000000 -43.000011 -179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-3.507106 4.350266 1.244304", "90.000038 0.000000 -37.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_A", "-3.430124 2.113739 2.092438", "0.000000 -24.999992 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_H", "-3.265430 1.207092 -0.956342", "-0.000036 -84.000008 89.999954" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-3.143021 1.350082 4.700359", "179.999954 0.000000 -90.000038" );
		SpawnInteriorIvy( "bldr_plnt_ivy_A", "-3.148500 -4.100250 -2.245879", "-4.999999 0.000000 -90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "-3.048570 -5.650345 -1.380015", "90.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_G", "-3.048569 1.300079 -0.655292", "-89.000000 -1.000000 -167.000015" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_G", "-3.053639 -4.900299 3.823895", "-90.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "-3.003640 -3.650223 3.758000", "-90.000000 -0.200000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_U", "-5.243145 -1.850113 2.459649", "0.000000 0.000000 90.000015" );
		SpawnInteriorIvy( "bldr_plnt_ivy_S", "-3.786013 -0.870247 0.603815", "179.999985 -42.000015 -177.999969" );
		SpawnInteriorIvy( "bldr_plnt_ivy_K", "-4.149733 -0.540222 1.220023", "0.000000 -36.999992 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_G", "-3.093237 -1.786499 2.591225", "-99.000015 -7.000000 -45.999992" );
		SpawnInteriorIvy( "bldr_plnt_ivy_H", "-1.792307 -0.900055 -2.748960", "-90.000000 0.000000 90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_D", "-3.070000 -4.100250 -2.642812", "90.000000 -1.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_A", "-0.540270 -1.600098 -2.671005", "-86.000008 -1.000000 89.000015" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_G", "-1.154595 -1.400085 -2.719160", "0.000000 0.000000 -179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_A", "-0.440273 -4.200256 -2.712239", "178.999954 0.000000 -90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "3.177833 4.039055 -2.021314", "-87.000008 -57.000008 178.000031" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "3.329475 4.000000 -1.738163", "87.000046 -57.000008 -179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_roof_B", "3.229927 4.255180 0.944834", "-52.999985 -57.000027 111.000572" );
		SpawnInteriorIvy( "bldr_plnt_ivy_roof_B", "3.467703 4.400024 0.999650", "147.000015 33.999992 37.999996" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-0.061586 4.302513 0.911391", "179.999985 -49.000015 177.999969" );
		SpawnInteriorIvy( "bldr_plnt_ivy_H", "-0.922456 3.049942 2.926136", "-176.000000 32.999989 -179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_H", "2.574688 3.449966 -3.351844", "125.000000 50.000008 -153.000137" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_G", "1.953039 2.399902 -3.786148", "-89.999992 36.000011 -171.999969" );
		SpawnInteriorIvy( "bldr_plnt_ivy_H", "-2.188015 2.099884 -2.199298", "-29.000082 30.999987 163.000015" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-3.436233 4.250259 0.841121", "90.000000 0.000000 -145.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_A", "-1.068937 -3.350204 4.822580", "89.999992 -7.000001 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_R", "-0.487248 -0.900055 4.972581", "-94.999985 0.000000 -89.999992" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-2.605822 1.100067 4.922581", "179.999954 -89.000046 0.000050" );
		SpawnInteriorIvy( "bldr_plnt_ivy_H", "0.242045 -0.400024 4.872581", "-88.999985 12.000000 -89.999992" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "4.728040 1.000061 4.872581", "179.999985 -88.000015 0.000009" );
		SpawnInteriorIvy( "bldr_plnt_ivy_J", "8.371510 -1.900116 4.918700", "-91.000031 0.000000 -92.000031" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "6.159181 -4.900299 4.872581", "0.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_G", "6.320898 -2.400146 4.872581", "0.000000 0.000000 -5.000009" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "2.351624 1.500092 4.872581", "0.000000 -1.000000 -179.999954" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "6.732035 0.900055 4.872500", "0.000000 0.000000 -96.999977" );
		SpawnInteriorIvy( "bldr_plnt_ivy_B", "5.860268 -4.000244 4.872581", "179.999985 89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_H", "9.900865 2.200134 0.396605", "0.000000 -27.000010 -91.999969" );
		SpawnInteriorIvy( "bldr_plnt_ivy_G", "9.750866 0.850052 1.301389", "-179.999985 -55.000004 90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "9.950868 4.050247 1.010849", "0.000009 -40.000011 -89.999992" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "9.728301 1.400085 3.202625", "-90.000000 -89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "9.678301 -4.200256 4.654539", "179.999954 0.000000 90.000038" );
		SpawnInteriorIvy( "bldr_plnt_ivy_J", "9.628301 -0.650040 1.558749", "0.000000 0.000000 90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_G", "9.502840 -2.050125 -2.361450", "-90.000000 2.000000 12.000010" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "9.552840 -4.500275 -2.535420", "90.000000 0.000000 -9.000002" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "9.460908 1.200073 -2.848960", "-90.000000 -87.000038 90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "6.523772 4.200012 0.803607", "179.999985 -36.000011 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "3.173532 -0.600037 -4.717586", "179.999985 -89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "3.616560 -1.400085 -4.930481", "-0.000005 -83.000015 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "1.944688 -4.100250 -4.630480", "90.000038 89.000046 -89.999992" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "1.267640 -4.100250 -4.630480", "179.999985 89.000046 0.000009" );
		SpawnInteriorIvy( "bldr_plnt_ivy_A", "-0.737480 -1.750107 -4.730480", "90.000015 0.000000 90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "4.502741 -5.050308 -4.530479", "0.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_A", "7.005630 -1.450089 -2.719980", "-176.999969 0.000000 90.000015" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_B", "7.307773 -4.450272 -2.713010", "0.000000 0.000000 0.000000" );
	}
}