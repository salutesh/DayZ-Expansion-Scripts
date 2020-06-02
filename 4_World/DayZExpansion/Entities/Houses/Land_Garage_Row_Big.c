/**
 * Land_Garage_Row_Big.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class Land_Garage_Row_Big extends BuildingSuper
{
	override bool HasInterior()
	{
		return true;
	}
	
	override void SpawnInterior()
	{
		SpawnInteriorObject("bldr_locker_open_v2", "-12.303287 -3.221405 6.173843", "-89.000038 0.000000 0.000000");
		SpawnInteriorObject("bldr_locker_closed_v1", "-12.300000 -3.221397 5.767724", "-90.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_locker_closed_v2", "-12.295894 -3.221405 5.366910", "-90.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_locker_open_v3", "-12.195716 -3.221405 4.866096", "-87.000008 0.000000 0.000000");
		SpawnInteriorObject("bldr_wreck_ural", "9.359804 -1.980370 -0.024831", "0.000000 -1.000000 -2.000000", 0.70);
		SpawnInteriorObject("Land_Wreck_Ikarus", "-3.276053 -2.033897 0.743488", "0.000000 0.000000 -6.000000", 0.70);
		SpawnInteriorObject("bldr_wreck_hmmwv", "3.061383 -2.314301 -0.036369", "0.000000 0.000000 0.000000", 0.70);
		SpawnInteriorObject("bldr_wreck_s120_red", "-8.866181 -2.638710 -2.547040", "-3.000000 0.000000 0.000000", 0.70);
		SpawnInteriorObject("Land_Wreck_S1023_Blue", "-9.211387 -2.239975 3.182245", "1.999999 0.000000 0.000000", 0.70);
		SpawnInteriorObject("bldr_metalcrate_02", "5.117751 -3.221405 5.577970", "10.000004 0.000000 0.000000", 0.70);
		SpawnInteriorObject("bldr_box_c_multi", "1.945283 -3.221405 6.052656", "174.000015 0.000000 0.000000", 0.85);
		SpawnInteriorObject("bldr_lekarnicka", "-11.891003 -1.871323 0.306234", "-90.000000 0.000000 0.000000", 0.85);
		SpawnInteriorObject("bldr_extinguisher", "-5.929112 -2.563950 -0.532151", "-179.999985 0.000000 0.000000", 0.85);
		SpawnInteriorObject("bldr_extinguisher", "-0.144739 -3.150002 -0.682151", "139.000107 62.000011 90.000000", 0.85);
		SpawnInteriorObject("bldr_bucket", "-12.263155 -3.221405 4.408345", "0.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_bag_dz", "12.282002 -2.804276 0.863438", "57.999992 0.000000 0.000000");
		SpawnInteriorObject("bldr_dirtpile_small_house", "6.179417 -3.221405 3.661344", "0.000000 0.000000 0.000000", 0.70);
		SpawnInteriorObject("bldr_dirtpile_small_house", "-5.759854 -3.221405 -3.657071", "-28.999763 0.000000 0.000000", 0.70);
		SpawnInteriorObject("bldr_dirtpile_small_house", "-5.885588 -3.221405 4.203327", "-133.311432 0.000000 0.000000", 0.70);
		SpawnInteriorObject("bldr_debris_small_house", "-6.202047 -3.221405 2.639532", "0.000000 0.000000 0.000000", 0.70);
		SpawnInteriorObject("bldr_debris_small_house", "-8.180562 -3.260002 0.306878", "0.000000 0.000000 0.000000", 0.70);
		SpawnInteriorObject("bldr_debris_small_house", "0.763274 -3.260002 -2.857629", "0.000000 0.000000 0.000000", 0.70);
		SpawnInteriorObject("bldr_debris_small_house", "0.933699 -3.260002 -3.695600", "165.000000 0.000000 0.000000", 0.70);
		SpawnInteriorObject("bldr_debris_small_house", "3.482568 -3.260002 3.968148", "-36.000004 0.000000 0.000000", 0.70);
		SpawnInteriorObject("bldr_drill", "6.806962 -1.870003 5.756141", "-81.000000 0.000000 0.000000", 0.90);
		SpawnInteriorObject("bldr_metal_cutting_saw", "5.565401 -1.930000 5.625682", "-115.999954 0.000000 0.000000", 0.90);
		SpawnInteriorObject("bldr_sawhorse_dz", "0.889224 -3.221405 0.894277", "31.000006 0.000000 0.000000", 0.90);
		SpawnInteriorObject("bldr_shelf_dz", "5.688835 -3.221405 -2.958295", "21.000008 0.000000 0.000000", 0.90);
		SpawnInteriorObject("bldr_vise", "4.852347 -2.239998 5.408761", "89.999992 0.000000 0.000000", 0.90);
		SpawnInteriorObject("bldr_Garbage_Bin", "8.317609 -2.655678 5.992742", "0.000000 0.000000 0.000000", 0.90);
		SpawnInteriorObject("bldr_Garbage_Pile3", "-6.223182 -2.952217 1.807807", "45.999996 0.000000 0.000000", 0.90);
		SpawnInteriorObject("ExpansionPropCarJack", "11.699100 -2.757004 -4.112050", "0.000000 0.000000 0.000000", 0.80);
		SpawnInteriorObject("ExpansionPropCase1", "12.330100 -2.730003 -3.763050", "0.000000 0.000000 0.000000", 0.80);
		SpawnInteriorObject("ExpansionPropCase2", "5.944529 -2.195000 6.059432", "-94.999954 0.000000 0.000000", 0.80);
		SpawnInteriorObject("ExpansionPropCarFilter", "5.920300 -2.239998 5.908780", "0.000000 0.000000 0.000000", 0.80);
		SpawnInteriorObject("ExpansionPropDrill", "4.943660 -2.250000 5.862320", "-29.999998 0.000000 0.000000", 0.80);
		SpawnInteriorObject("ExpansionPropGlue", "12.365800 -2.989998 -3.803930", "101.000000 0.000000 0.000000", 0.80);
		SpawnInteriorObject("ExpansionPropGlue", "12.343800 -2.989998 -3.558880", "-2.000000 0.000000 0.000000", 0.80);
		SpawnInteriorObject("ExpansionPropJigsaw", "12.313200 -2.705002 -2.812940", "-57.000004 0.000000 0.000000", 0.80);
		SpawnInteriorObject("ExpansionPropPliers", "12.351200 -2.348000 -2.811870", "71.000000 0.000000 0.000000", 0.80);
		SpawnInteriorObject("ExpansionPropPliersSet1", "12.280500 -2.355003 -2.555930", "2.000000 2.000000 0.000000", 0.80);
		SpawnInteriorObject("ExpansionPropPropaneTorch", "11.960100 -2.702003 -5.188470", "-162.999985 47.000000 90.000000", 0.80);
		SpawnInteriorObject("ExpansionPropBin", "12.364000 -3.040001 -4.603870", "84.000000 0.000000 0.000000", 0.80);
		SpawnInteriorObject("ExpansionPropRespirator", "11.868400 -2.303001 -5.197280", "66.000000 -1.000000 0.000000", 0.80);
		SpawnInteriorObject("ExpansionPropTackleBox2", "12.336600 -1.871300 -3.663630", "0.000000 0.000000 0.000000", 0.80);
		SpawnInteriorObject("ExpansionPropTackleBox1", "12.341300 -3.000000 -1.848450", "-91.000015 0.000000 0.000000", 0.80);
		SpawnInteriorObject("ExpansionPropToolBox1", "4.028880 -3.080002 5.616930", "-59.999996 0.000000 0.000000", 0.80);
		SpawnInteriorObject("ExpansionPropToolChest", "-11.172500 -2.623001 -1.696390", "-49.000000 0.000000 0.000000");
		SpawnInteriorObject("ExpansionPropToolShelf", "-12.227700 -2.471397 -3.258930", "-179.999985 0.000000 0.000000", 0.80);
		SpawnInteriorObject("ExpansionPropGloves", "3.580370 -2.349998 6.151440", "-28.999998 0.000000 0.000000", 0.80);
		SpawnInteriorObject("ExpansionPropCarFilter", "12.351600 -2.330002 -1.085160", "0.000000 0.000000 0.000000", 0.80);
		SpawnInteriorObject("ExpansionPropCarFilter", "12.363000 -2.330002 -0.937551", "66.000000 0.000000 0.000000", 0.80);
		SpawnInteriorObject("ExpansionPropCarFilter", "12.304300 -2.330002 -0.774184", "-45.050598 0.000000 0.000000", 0.80);
		SpawnInteriorObject("ExpansionPropCarFilter", "12.434600 -2.330002 -0.800162", "0.000000 0.000000 0.000000", 0.80);
		SpawnInteriorObject("ExpansionPropCarFilter", "12.366900 -2.330002 -0.593209", "144.033997 0.000000 0.000000", 0.80);
		SpawnInteriorObject("ExpansionPropBleach", "12.365300 -2.654999 -1.535880", "-145.000000 0.000000 0.000000", 0.80);
		SpawnInteriorObject("ExpansionPropBleach", "12.330000 -2.650002 -1.669120", "37.000000 0.000000 0.000000", 0.80);
		SpawnInteriorObject("ExpansionPropBattery", "12.391300 -2.296997 -1.753000", "-90.000000 0.000000 0.000000", 0.80);
		SpawnInteriorObject("ExpansionPropBattery", "12.416800 -2.296997 -2.036950", "76.000000 0.000000 0.000000", 0.80);
		SpawnInteriorObject("ExpansionPropBattery", "12.292700 -2.296997 -1.594870", "-115.000000 0.000000 0.000000", 0.80);
		SpawnInteriorObject("ExpansionPropAirtank", "-11.765700 -2.888000 -5.005960", "167.000000 0.000000 0.000000", 0.80);
		SpawnInteriorObject("ExpansionPropTrashBarrel2", "-6.866020 -2.830002 -2.179530", "-28.000000 78.000008 0.000000", 0.80);
		SpawnInteriorObject("ExpansionPropWelder", "-11.283501 -1.906990 -1.579120", "-105.999969 0.000000 0.000000");
		SpawnInteriorObject("ExpansionPropHeadphones", "3.958820 -1.940000 6.128620", "17.000006 0.000000 0.000000");

	}
}
