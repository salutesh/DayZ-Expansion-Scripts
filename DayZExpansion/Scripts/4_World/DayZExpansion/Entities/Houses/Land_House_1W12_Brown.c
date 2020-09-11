/**
 * Land_House_1W12_Brown.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class Land_House_1W12_Brown extends BuildingWithFireplace
{
	override bool HasInterior()
	{
		return false;
	}

    override bool HasIvys()
    {
        return true;
    }
	
	override void SpawnIvys()
	{
		//Ivy
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "4.249397 4.599846 -2.048816", "91.000015 -45.999992 178.999939" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "3.536237 4.631638 -2.778364", "-91.000015 -43.999989 -179.999954" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "2.033077 3.379906 1.174461", "-151.000076 -29.999989 -146.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "1.905101 3.125595 1.610770", "62.000004 -29.999987 146.000031" );
		SpawnInteriorIvy( "bldr_plnt_ivy_H", "2.389829 3.694786 -4.244061", "0.209472 -0.255324 -39.366241" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "0.683118 2.262848 -7.345118", "-30.000006 -23.999985 147.000015" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "1.825775 3.187347 -6.599743", "113.000023 -30.000006 -145.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_roof_B", "3.935647 4.787445 -4.432101", "-32.000122 -40.000050 119.000130" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "3.783759 4.431328 -0.196419", "-58.999985 -29.999989 -149.000031" );
		SpawnInteriorIvy( "bldr_plnt_ivy_H", "5.375822 3.555153 -0.671678", "166.000031 -11.000002 -38.000011" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "4.996798 4.000000 -4.952877", "31.000002 -27.999992 -146.000061" );
		SpawnInteriorIvy( "bldr_plnt_ivy_roof_A", "-1.983188 0.350021 2.459854", "-89.999992 -89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_H", "2.760308 2.511963 2.401227", "-0.352706 -38.060596 179.613922" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_A", "5.092026 2.700165 -6.753773", "125.999931 -25.999994 -60.999981" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-1.103289 -2.450150 -9.360670", "-4.000009 89.000046 179.999954" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "2.288680 -0.250015 -9.210670", "0.000000 -85.000008 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_K", "5.715554 -0.200012 -9.260670", "-3.000004 -89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "8.821516 -0.100006 -9.210670", "1.000000 -89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "8.961796 -2.300140 -8.971944", "87.000046 89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "8.861796 -2.250137 -1.889773", "-89.999992 86.000015 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "8.811795 -0.100006 4.233508", "-90.000038 -89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "8.861796 -0.300018 -1.629885", "-90.000000 -89.000046 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-1.208204 -2.250137 4.553281", "-92.000015 88.000015 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-1.308204 -0.200012 1.442573", "90.000000 -89.000046 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-1.258204 -0.150009 -9.210316", "92.000031 -89.000046 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "-1.022013 -0.200012 -3.410948", "-89.999954 -89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_A", "-0.908204 -0.100006 -5.075114", "4.000000 0.000000 91.000015" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_D", "-1.800599 0.000000 -1.663023", "179.999985 -89.000046 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_U", "-2.089858 -0.150009 -3.985945", "0.000000 0.000000 90.000038" );
		SpawnInteriorIvy( "bldr_plnt_ivy_V", "-3.492629 -1.700104 -1.605647", "0.000000 37.000008 -90.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_N", "-1.212873 -0.050003 4.819332", "-177.000015 -85.000008 0.000063" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "5.469091 -0.150009 4.619331", "0.013450 -88.319603 -179.541260" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "2.262696 -2.300140 4.569331", "179.999985 81.000000 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_special_B", "-0.808204 -0.950058 -1.497338", "-179.999954 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "-1.008204 -3.900238 -4.347733", "90.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_G", "-1.008204 -3.200195 -5.551357", "90.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_B", "0.604361 -2.650162 -9.080000", "0.007812 0.980125 0.456656" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_G", "8.739905 -2.700165 -9.110669", "0.000000 0.000000 -74.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_G", "8.711796 -2.200134 -8.048508", "90.000000 0.000000 -76.000038" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "8.711796 -3.550217 -7.244067", "90.000015 0.000000 25.000013" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "8.711796 -3.550217 0.317703", "90.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_G", "4.149137 -2.900177 4.419331", "0.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_dry_C", "2.988876 -3.900238 4.419331", "0.000000 0.000000 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "-2.010724 -0.100006 -6.963589", "90.000000 -89.000046 -0.000005" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "-1.928497 0.350021 -5.431330", "89.999992 36.000027 179.999985" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "2.040532 0.000000 5.581701", "179.999985 -87.000038 0.000009" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "3.838353 0.250015 5.436375", "179.999985 33.000011 178.999969" );
		SpawnInteriorIvy( "bldr_plnt_ivy_M", "9.761800 -0.045708 0.790404", "-90.000000 -89.000046 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "9.706438 -0.150009 -3.948161", "-90.000000 -89.000046 0.000000" );
		SpawnInteriorIvy( "bldr_plnt_ivy_L", "9.644018 0.350021 -2.867815", "-89.999992 33.000011 179.999985" );
	}
}