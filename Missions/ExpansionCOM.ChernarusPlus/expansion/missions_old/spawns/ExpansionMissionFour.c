/**
 * ExpansionMissionFour.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

static void ExpansionMissionFour()
{
	//! Notifications
	GetNotificationSystem().CreateNotification_Old("Mission", "A high tier train mission appeared near Lopatino castle", ARGB(255, 221, 38, 38), 7);

	//! Loot
	ref array< ref ExpansionMissionEventSoldierLoot > Loot = new array< ref ExpansionMissionEventSoldierLoot >;

	Loot.Insert( new ExpansionMissionEventSoldierLoot("AK74") );
	Loot.Insert( new ExpansionMissionEventSoldierLoot("AK101") );
	Loot.Insert( new ExpansionMissionEventSoldierLoot("SKS") );
	Loot.Insert( new ExpansionMissionEventSoldierLoot("M4A1") );
	Loot.Insert( new ExpansionMissionEventSoldierLoot("AKM") );
	Loot.Insert( new ExpansionMissionEventSoldierLoot("Mag_STANAG_30Rnd") );
	Loot.Insert( new ExpansionMissionEventSoldierLoot("Mag_AKM_30Rnd") );
	Loot.Insert( new ExpansionMissionEventSoldierLoot("Mag_SVD_10Rnd") );
	Loot.Insert( new ExpansionMissionEventSoldierLoot("Mag_VSS_10Rnd") );
	Loot.Insert( new ExpansionMissionEventSoldierLoot("PlateCarrierVest") );
	Loot.Insert( new ExpansionMissionEventSoldierLoot("PlaceCarrierHolster") );
	Loot.Insert( new ExpansionMissionEventSoldierLoot("PlaceCarrierPouches") );
	Loot.Insert( new ExpansionMissionEventSoldierLoot("Rangefinder") );

	Barrel_ColorBase Barrel1 = Barrel_ColorBase.Cast(GetDayZExpansion().SpawnObject("Barrel_Red", "9083.189453 295.262146 7105.940430", "65.000000 0.000000 0.000000", 60));
	Barrel1.Open();

	Barrel_ColorBase Barrel2 = Barrel_ColorBase.Cast(GetDayZExpansion().SpawnObject("Barrel_Red", "9074.731445 295.213837 7101.162109", "-58.999989 0.000000 -0.000000", 60));
	Barrel2.Open();

	GetDynamicMissions().SpawnMissionLoot(Loot, Barrel1, 30);
	GetDynamicMissions().SpawnMissionLoot(Loot, Barrel2, 30);

	//! Objects
	GetDayZExpansion().SpawnObject("bldr_rock_apart2", "1394.397461 213.456558 9430.752930", "-3.000026 -14.000000 4.999999", 60);
	GetDayZExpansion().SpawnObject("bldr_rock_apart2", "1388.645264 211.071671 9434.535156", "138.999985 70.999992 179.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Train_742_Red", "1393.399170 216.496826 9434.485352", "-109.999954 7.000001 32.999985", 60);
	GetDayZExpansion().SpawnObject("Land_Train_742_Blue", "1380.332153 216.551743 9431.770508", "-89.000107 -6.000001 17.000002", 60);
	GetDayZExpansion().SpawnObject("Land_Train_742_Red", "1367.453613 215.436172 9434.292969", "-63.999985 -2.000000 4.000000", 60);
	GetDayZExpansion().SpawnObject("ExpansionTrainWagonTanker", "1354.875366 214.299377 9435.237305", "67.000000 3.000000 -76.000038", 60);
	GetDayZExpansion().SpawnObject("ExpansionTrainWagonTanker", "1345.024658 214.667526 9429.237305", "47.334793 -7.000000 -50.000004", 60);
	GetDayZExpansion().SpawnObject("ExpansionTrainWagonTanker", "1335.459839 214.959488 9423.117188", "75.000000 3.000001 -13.000005", 60);
	GetDayZExpansion().SpawnObject("ExpansionTrainWagonTanker", "1325.085938 214.875000 9418.751953", "66.000000 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject("ExpansionTrainWagonTanker", "1314.818115 214.784622 9414.099609", "65.000015 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject("ExpansionTrainWagonTanker", "1304.956055 214.690735 9408.699219", "59.000000 1.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject("ExpansionTrainWagonTanker", "1295.647949 214.494751 9402.330078", "52.999996 1.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject("bldr_wreck_decal_small1", "1352.589722 212.995956 9435.648438", "8.871253 5.109500 -2.185469", 60);
	GetDayZExpansion().SpawnObject("bldr_wreck_decal_small1", "1355.060669 213.221893 9437.573242", "53.704365 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject("ExpansionTrainWagonTanker", "1287.108887 214.246368 9395.146484", "47.000000 1.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject("ExpansionTrainWagonTanker", "1279.298462 214.130707 9387.161133", "40.999996 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject("ExpansionTrainWagonTanker", "1272.259399 214.130707 9378.371094", "36.000000 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject("ExpansionTrainWagonTanker", "1266.203003 213.960007 9369.022461", "29.999998 1.500000 0.000000", 60);
	GetDayZExpansion().SpawnObject("ExpansionTrainWagonTanker", "1260.999878 213.720001 9359.072266", "25.000002 1.000000 -1.000000", 60);
	GetDayZExpansion().SpawnObject("ExpansionTrainWagonTanker", "1255.941650 213.600006 9349.040039", "29.999998 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject("ExpansionTrainWagonTanker", "1249.980591 213.529999 9339.583008", "35.000000 0.500000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Train_Wagon_Box", "1242.900757 213.320007 9330.431641", "40.999996 0.500000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Train_Wagon_Box", "1234.595337 213.275192 9321.666992", "44.999996 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Train_Wagon_Box", "1225.806763 213.270004 9313.392578", "48.000004 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Train_Wagon_Box", "1216.746704 213.270004 9305.421875", "49.309502 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Train_Wagon_Box", "1207.572998 213.270004 9297.560547", "49.309502 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Train_Wagon_Box", "1198.416748 213.279999 9289.690430", "49.309502 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Train_Wagon_Box", "1189.235962 213.229996 9281.817383", "49.499996 0.500000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Train_Wagon_Box", "1179.987183 213.079987 9274.027344", "50.000004 1.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Train_Wagon_Box", "1170.774414 212.919998 9266.295898", "50.000004 0.800000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Train_Wagon_Box", "1161.486084 212.769989 9258.513672", "50.000004 0.800000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Train_Wagon_Box", "1152.141968 212.619995 9250.920898", "51.999996 0.600000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Train_Wagon_Box", "1142.593262 212.470001 9243.752930", "54.300003 0.650000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Train_Wagon_Box", "1132.659912 212.380005 9236.943359", "56.900009 0.300000 0.500000", 60);
	GetDayZExpansion().SpawnObject("Land_Train_Wagon_Box", "1122.421875 212.339996 9230.595703", "59.500000 0.300000 0.500000", 60);
	GetDayZExpansion().SpawnObject("Land_Train_Wagon_Box", "1111.879761 212.199997 9224.680664", "61.900002 1.150000 0.300000", 60);
	GetDayZExpansion().SpawnObject("Land_Train_Wagon_Box", "1101.139282 211.960007 9219.270508", "64.300011 1.150000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Train_Wagon_Box", "1090.238770 211.639999 9214.196289", "65.300034 1.900000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Train_Wagon_Box", "1079.270020 211.235001 9209.180664", "65.500000 1.900000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Train_Wagon_Box", "1068.270020 210.820007 9204.169922", "65.500000 1.950000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Train_Wagon_Box", "1057.317749 210.399994 9199.230469", "66.200005 2.100000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Train_Wagon_Box", "1046.220703 209.960007 9194.601563", "68.699997 2.100000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Train_Wagon_Box", "1034.968262 209.479996 9190.468750", "71.000000 2.600000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Train_Wagon_Box", "1023.496948 208.919998 9186.821289", "73.500008 2.700000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Train_Wagon_Box", "1011.875000 208.360001 9183.650391", "76.000000 2.550000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Train_Wagon_Box", "1000.135498 207.830002 9181.034180", "78.599998 2.550000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Train_Wagon_Box", "988.328186 207.324997 9178.895508", "81.000000 2.300000 0.000000", 60);
	GetDayZExpansion().SpawnObject("ExpansionTrainWagonFlat", "973.937256 205.559998 9176.649414", "81.000000 2.250000 0.000000", 60);
	GetDayZExpansion().SpawnObject("ExpansionTrainWagonFlat", "957.001587 204.884995 9173.961914", "81.000000 2.250000 0.000000", 60);
	GetDayZExpansion().SpawnObject("ExpansionTrainWagonFlat", "940.176086 204.294998 9171.298828", "81.000000 1.800000 0.000000", 60);
	GetDayZExpansion().SpawnObject("ExpansionTrainWagonFlat", "923.293152 203.751465 9168.620117", "81.000000 1.820000 0.000000", 60);
	GetDayZExpansion().SpawnObject("ExpansionTrainWagonFlat", "906.413269 203.240005 9165.820313", "80.200005 1.600000 0.000000", 60);
	GetDayZExpansion().SpawnObject("ExpansionTrainWagonFlat", "889.626709 202.779999 9162.370117", "76.650002 1.400000 0.000000", 60);
	GetDayZExpansion().SpawnObject("ExpansionTrainWagonFlat", "873.101563 202.399994 9157.931641", "73.200005 1.200000 0.000000", 60);
	GetDayZExpansion().SpawnObject("ExpansionTrainWagonFlat", "856.884583 202.070007 9152.436523", "69.700005 0.900000 0.000000", 60);
	GetDayZExpansion().SpawnObject("ExpansionTrainWagonFlat", "841.029724 201.793716 9145.966797", "66.200005 0.900000 0.000000", 60);
	GetDayZExpansion().SpawnObject("ExpansionTrainWagonFlat", "825.587280 201.500000 9138.560547", "62.700008 1.050000 0.000000", 60);
	GetDayZExpansion().SpawnObject("ExpansionTrainWagonFlat", "810.649841 201.149994 9130.150391", "58.980000 1.050000 0.000000", 60);
	GetDayZExpansion().SpawnObject("ExpansionTrainWagonFlat", "796.298645 200.809998 9120.862305", "55.299992 1.200000 0.000000", 60);
	GetDayZExpansion().SpawnObject("ExpansionTrainWagonFlat", "782.490173 200.389999 9110.680664", "51.999989 1.520000 0.000000", 60);
	GetDayZExpansion().SpawnObject("ExpansionTrainWagonFlat", "769.340027 199.929993 9099.687500", "48.299995 1.300000 0.000000", 60);
	GetDayZExpansion().SpawnObject("ExpansionTrainWagonFlat", "756.950012 199.529999 9087.904297", "44.699997 1.350000 0.000000", 60);
	GetDayZExpansion().SpawnObject("ExpansionTrainWagonFlat", "745.319946 199.125000 9075.397461", "41.299995 1.350000 0.000000", 60);
	GetDayZExpansion().SpawnObject("ExpansionTrainWagonFlat", "734.440002 198.800003 9062.164063", "37.749996 0.950000 0.000000", 60);
	GetDayZExpansion().SpawnObject("ExpansionTrainWagonFlat", "724.559998 198.520004 9048.454102", "34.000000 1.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject("ExpansionTrainWagonFlat", "715.409973 198.229996 9033.952148", "30.500000 1.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject("ExpansionTrainWagonFlat", "707.179993 197.889999 9018.954102", "27.099998 1.150000 0.000000", 60);
	GetDayZExpansion().SpawnObject("ExpansionTrainWagonFlat", "699.890015 197.544998 9003.474609", "23.500000 1.150000 0.000000", 60);
	GetDayZExpansion().SpawnObject("ExpansionTrainWagonFlat", "693.619995 197.250000 8987.573242", "19.900002 1.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject("ExpansionTrainWagonFlat", "688.268982 196.983002 8971.315430", "16.400002 0.800000 0.000000", 60);
	GetDayZExpansion().SpawnObject("ExpansionTrainWagonFlat", "683.999023 196.729996 8954.737305", "12.900000 0.900000 0.000000", 60);
	GetDayZExpansion().SpawnObject("ExpansionTrainWagonFlat", "680.719971 196.470001 8937.894531", "9.400002 0.750000 0.000000", 60);
	GetDayZExpansion().SpawnObject("ExpansionTrainWagonFlat", "678.083008 196.339996 8920.952148", "8.600000 0.250000 0.000000", 60);
	GetDayZExpansion().SpawnObject("ExpansionTrainWagonFlat", "675.500000 196.263260 8903.967773", "8.600000 0.250000 0.000000", 60);
	GetDayZExpansion().SpawnObject("ExpansionTrainWagonFlat", "672.919983 196.195007 8886.965820", "8.600000 0.250000 0.000000", 60);
	GetDayZExpansion().SpawnObject("ExpansionTrainWagonTanker", "670.809570 197.509995 8872.848633", "8.600000 0.250000 0.000000", 60);
	GetDayZExpansion().SpawnObject("ExpansionTrainWagonTanker", "669.104065 197.459991 8861.624023", "8.600000 0.250000 0.000000", 60);
	GetDayZExpansion().SpawnObject("ExpansionTrainWagonTanker", "667.489990 197.382004 8850.432617", "7.300000 0.500000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Train_Wagon_Box", "666.270020 197.089005 8838.886719", "4.999999 0.600000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Train_Wagon_Box", "665.471497 196.970001 8826.889648", "2.500000 0.600000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Train_Wagon_Box", "665.210022 196.830002 8814.866211", "0.000000 0.600000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Train_Wagon_Box", "665.477173 196.679993 8802.801758", "-2.600000 1.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject("ExpansionTrainWagonTanker", "666.210022 196.649994 8791.113281", "-4.989999 1.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject("ExpansionTrainWagonTanker", "667.409973 196.475006 8779.899414", "-7.399997 0.600000 0.000000", 60);
	GetDayZExpansion().SpawnObject("ExpansionTrainWagonFlat", "669.619995 194.990005 8765.870117", "-10.199999 0.400000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Train_Wagon_Box", "672.580017 196.056000 8751.560547", "-13.300000 0.700000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Train_Wagon_Box", "675.619995 195.911163 8739.923828", "-16.000008 0.700000 0.000000", 60);
	GetDayZExpansion().SpawnObject("ExpansionTrainWagonTanker", "678.960449 196.009995 8728.759766", "-18.000002 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Train_Wagon_Box", "682.869995 195.835007 8717.774414", "-20.700001 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject("ExpansionTrainWagonFlat", "688.090027 194.630005 8704.097656", "-21.000000 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject("ExpansionTrainWagonFlat", "694.258850 194.630005 8688.048828", "-21.000000 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject("ExpansionTrainWagonFlat", "700.395325 194.630005 8672.012695", "-21.000000 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject("ExpansionTrainWagonTanker", "705.548218 196.009995 8658.681641", "-21.000000 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject("ExpansionTrainWagonTanker", "709.600037 196.009995 8648.159180", "-21.299999 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject("ExpansionTrainWagonTanker", "713.700012 196.009995 8637.666992", "-21.350000 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Train_Wagon_Box", "717.980530 195.832993 8626.801758", "-21.400000 0.050000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Container_1Mo", "978.045227 207.699997 9177.243164", "81.000000 2.250000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Container_1Mo", "969.984497 207.380005 9175.974609", "81.000000 2.250000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Container_1Mo", "961.067261 207.029343 9174.555664", "81.000000 2.250000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Container_1Mo", "952.995972 206.708145 9173.306641", "81.000000 2.250000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Container_1Mo", "944.198730 206.389999 9171.889648", "81.000000 1.800000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Container_1Mo", "936.225586 206.139999 9170.637695", "81.000000 1.800000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Container_1Mo", "927.361084 205.845398 9169.221680", "81.000000 1.820000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Container_1Mo", "919.294678 205.531036 9167.962891", "81.000000 0.820000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Container_1Mo", "910.527344 205.320435 9166.406250", "81.200012 1.600000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Container_1Mo", "902.426208 205.100006 9165.129883", "80.200035 1.600000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Container_1Mo", "893.705322 204.860001 9163.305664", "76.650002 1.400000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Container_1Mo", "885.668396 204.649994 9161.390625", "76.650002 1.400000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Container_1Mo", "877.113586 204.450226 9159.112305", "73.200005 1.200000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Container_1Mo", "869.215881 204.289993 9156.708008", "73.200005 1.200000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Container_1Mo", "860.856140 204.111420 9153.874023", "69.700005 0.900000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Container_1Mo", "853.089355 203.979996 9150.980469", "69.700005 0.900000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Container_1Mo", "844.921936 203.837326 9147.634766", "66.200005 0.900000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Container_1Mo", "837.322815 203.710007 9144.292969", "66.200005 0.900000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Container_1Mo", "829.384583 203.550003 9140.448242", "62.700008 1.050000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Container_1Mo", "821.978882 203.399994 9136.650391", "62.700008 1.050000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Container_1Mo", "814.267517 203.194809 9132.269531", "58.980000 1.050000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Container_1Mo", "807.222168 203.050003 9128.060547", "58.980000 1.050000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Container_1Mo", "799.763672 202.877151 9123.222656", "55.299992 1.200000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Container_1Mo", "793.005554 202.699997 9118.533203", "55.200001 1.200000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Wreck_Uaz", "787.077881 202.119995 9114.269531", "-127.999947 -1.520000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Wreck_Uaz", "783.734497 201.990005 9112.221680", "51.999996 1.520000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Wreck_Uaz", "780.942810 201.919998 9109.511719", "-127.000023 -1.520000 -0.000009", 60);
	GetDayZExpansion().SpawnObject("Land_Wreck_Uaz", "777.677856 201.800003 9107.494141", "51.999996 1.250000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Container_1Mo", "772.483276 201.993210 9102.425781", "48.299995 1.300000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Container_1Mo", "766.320740 201.809998 9096.957031", "48.299995 1.300000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Wreck_V3S", "759.703125 201.933655 9090.696289", "-133.300171 -2.350000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Wreck_V3S", "754.097534 201.723282 9085.032227", "44.699997 0.000000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Wreck_V3S", "748.044983 201.550003 9078.353516", "-139.700134 -2.350000 2.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Wreck_V3S", "742.500183 201.320007 9072.236328", "41.299992 0.350000 1.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Wreck_V3S", "737.113647 201.201584 9065.501953", "-142.300278 -1.950000 0.000000", 60);
	GetDayZExpansion().SpawnObject("Land_Wreck_V3S", "732.025696 201.000000 9058.996094", "39.750008 -0.050000 -0.000009", 60);
	GetDayZExpansion().SpawnObject("bldr_wreck_ural", "726.494690 200.550629 9051.530273", "-149.000031 -1.000000 -3.000004", 60);
	GetDayZExpansion().SpawnObject("bldr_wreck_ural", "722.692200 200.418060 9045.102539", "32.000004 1.000000 -3.000006", 60);
	GetDayZExpansion().SpawnObject("bldr_wreck_ural", "717.140320 200.256714 9037.068359", "-152.500229 -2.000000 -2.000000", 60);
	GetDayZExpansion().SpawnObject("bldr_wreck_ural", "713.573181 200.124100 9030.538086", "28.500002 0.000000 -2.000000", 60);
	GetDayZExpansion().SpawnObject("bldr_wreck_bmp2", "709.080505 199.789993 9021.863281", "-152.900040 -1.000000 8.000011", 60);
	GetDayZExpansion().SpawnObject("bldr_wreck_bmp2", "705.031433 199.612259 9015.364258", "27.000000 1.150000 8.000002", 60);
	GetDayZExpansion().SpawnObject( "bldr_wreck_bmp2", "701.930237 199.398544 9006.837891", "-155.500229 -1.150000 7.000008", 60);
	GetDayZExpansion().SpawnObject( "bldr_wreck_bmp2", "697.990356 199.267929 9000.054688", "23.500000 1.150000 7.000002", 60);
	GetDayZExpansion().SpawnObject( "bldr_wreck_bmp2", "695.275452 199.128754 8990.764648", "-160.000046 -1.000000 7.000006", 60);
	GetDayZExpansion().SpawnObject( "bldr_wreck_bmp2", "691.817688 198.946442 8983.959961", "19.900000 1.000000 5.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_wreck_brdm", "689.396240 198.518417 8974.712891", "-158.000031 1.200000 -16.000017", 60);
	GetDayZExpansion().SpawnObject( "bldr_wreck_brdm", "687.354980 198.415131 8967.767578", "-160.000000 2.200000 -16.000017", 60);
	GetDayZExpansion().SpawnObject( "bldr_wreck_brdm", "684.990662 198.319290 8958.612305", "-162.100449 3.000000 -15.000015", 60);
	GetDayZExpansion().SpawnObject( "bldr_wreck_brdm", "683.215515 198.253067 8951.123047", "-161.000000 0.000000 -14.000013", 60);
	GetDayZExpansion().SpawnObject( "bldr_mil_radar_tall_generator", "681.660950 198.588974 8943.569336", "9.400000 0.750000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_mil_radar_tall_generator", "680.803162 198.520004 8937.852539", "-171.000031 -0.750000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_mil_radar_tall_generator", "679.854004 198.452866 8932.145508", "-170.999969 -0.750000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_mil_radar_tall_generator", "678.950439 198.414993 8926.340820", "-172.000031 -0.250000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_mil_radar_tall_generator", "678.019836 198.399994 8920.492188", "8.600000 0.250000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_mil_radar_tall_generator", "677.248474 198.369995 8914.806641", "-172.000031 -0.250000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_tent_boxwooden", "676.054443 197.514648 8910.957031", "8.600000 0.250000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_tent_boxwooden", "676.054443 198.472448 8910.957031", "8.600000 0.250000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_tent_boxwooden", "677.047241 197.514648 8910.806641", "8.600000 0.250000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_tent_boxwooden", "677.047241 198.472448 8910.806641", "8.600000 0.250000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_tent_boxwooden", "675.904419 197.510254 8909.963867", "8.600000 0.250000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_tent_boxwooden", "675.904419 198.483254 8909.963867", "8.600000 0.250000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_tent_boxwooden", "676.897888 197.510254 8909.813477", "8.600000 0.250000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_tent_boxwooden", "676.897888 198.483254 8909.813477", "8.600000 0.250000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_tent_boxwooden", "675.756653 197.505890 8908.971680", "8.600000 0.250000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_tent_boxwooden", "675.756653 198.47869 8908.971680", "8.600000 0.250000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_tent_boxwooden", "676.744751 197.505875 8908.821289", "8.600000 0.250000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_tent_boxwooden", "676.744751 198.47869 8908.821289", "8.600000 0.250000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_tent_boxwooden", "675.608887 197.501495 8907.979492", "8.600000 0.250000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_tent_boxwooden", "676.596558 197.501495 8907.828125", "8.600000 0.250000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_tent_boxwooden", "675.457886 197.497131 8906.987305", "8.600000 0.250000 0.000000", 60);
	GetDayZExpansion().SpawnObject( "bldr_tent_boxwooden", "676.447449 197.497116 8906.835938", "8.600000 0.250000 0.000000", 60);
	GetDayZExpansion().SpawnObject(	"ExpansionUh1h", "687.670105 198.399872 8703.825195", "-6.032262 -0.117158 0.118282", 60);
}