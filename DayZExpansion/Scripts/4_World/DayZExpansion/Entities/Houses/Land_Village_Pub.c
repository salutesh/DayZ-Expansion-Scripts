/**
 * Land_Village_Pub.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class Land_Village_Pub
{
	override bool HasInterior()
	{
		return true;
	}

	override bool HasIvys()
    {
        return true;
    }
	
	override void SpawnIvys()
	{
		//Ivy
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-4.528921 4.744614 -6.155187", "-89.999954 -32.000008 -178.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-4.657783 4.644608 -0.750388", "-90.000046 -39.999996 179.000061" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_A", "-3.431769 3.894562 0.057876", "0.000000 -3.000000 -55.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-4.255291 4.594604 -3.115151", "89.999969 -46.000015 177.999954" );
		SpawnInteriorIvy( "bldr_plnt_ivy_I", "-4.453269 4.150253 -5.464141", "-90.000000 57.000008 90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-0.979707 1.644424 -5.536901", "179.999985 -89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-7.861369 1.644424 -5.611596", "90.000000 2.000000 -90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_F", "-6.162527 -1.218575 -5.484004", "-0.000005 -89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_J", "-3.561240 -2.418648 -5.484004", "-90.000015 0.000000 90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "3.957843 -0.949989 -5.591140", "2.000000 -89.000046 -0.000009" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "1.361720 -3.368706 -5.488651", "-0.000009 89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "1.909947 -5.118813 -5.399000", "-179.999985 0.000000 -11.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_B", "1.466083 -3.168694 -5.410000", "0.000000 0.000000 -72.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_roof_B", "-0.324737 0.651146 -3.904300", "-33.999985 -44.000027 134.000427" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "1.726688 0.681541 -4.019339", "-125.000038 -32.000008 153.999924" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "2.016384 0.681541 -3.796813", "29.000006 -26.999983 -156.000015" );
		SpawnInteriorIvy( "bldr_plnt_ivy_H", "1.202545 1.131569 0.819153", "59.999992 -24.999996 -166.000015" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "1.987211 0.737549 3.076138", "155.000031 -19.999983 155.000031" );
		SpawnInteriorIvy( "bldr_plnt_ivy_H", "0.206946 0.787552 3.327626", "-76.000053 -10.000002 -146.000229" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_A", "3.816889 -1.750107 -0.101330", "1.000000 11.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "3.866889 -3.350204 -2.477821", "-89.999992 85.000008 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "3.916889 -1.300079 -3.416556", "90.000038 -89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_A", "3.866889 -0.762543 4.793162", "179.999954 0.000000 90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-0.861840 -1.050064 4.952507", "179.999985 -89.000046 -0.000063" );
		SpawnInteriorIvy( "bldr_plnt_ivy_I", "2.833345 -3.350204 4.920000", "-179.999939 -89.000046 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "2.672185 -3.449966 4.802507", "-179.999985 0.000000 88.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_K", "-2.052114 -2.584976 5.992797", "-179.999985 -87.000038 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_O", "-7.272727 -2.100128 5.959037", "90.000000 0.000000 -90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "-7.748501 -2.834991 3.871986", "-89.999954 -89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_O", "-1.241838 -3.785049 4.630765", "0.000000 0.000000 90.000015" );
		SpawnInteriorIvy( "bldr_plnt_ivy_G", "-6.655488 -3.500214 4.966588", "-132.999939 0.000000 -179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_F", "-2.060780 -3.349960 4.896287", "28.000010 0.000000 -179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-8.040045 -3.555893 -0.992667", "0.000000 0.000000 90.000038" );
		SpawnInteriorIvy( "bldr_plnt_ivy_K", "-8.067752 1.600098 0.541878", "-89.999992 -89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_I", "-8.018881 -3.449966 -2.002940", "-89.999992 -89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_J", "-8.017752 0.050003 -4.371258", "0.000000 0.000000 90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "-8.600000 -3.500214 -1.646808", "90.000000 0.000000 1.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_B", "-8.354943 -3.200195 -2.100000", "0.000000 0.000000 0.000000" );
	}
}