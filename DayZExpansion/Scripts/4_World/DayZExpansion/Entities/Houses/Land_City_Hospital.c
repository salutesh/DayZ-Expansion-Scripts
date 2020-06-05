/**
 * Land_City_Hospital.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class Land_City_Hospital extends BuildingSuper
{
	override bool HasInterior()
	{
		return true;
	}
	
	override void SpawnInterior()
	{
		SpawnInteriorObject("bldr_hospital_transport_bed", "6.662666 -3.625000 -0.595863", "-38.999985 0.000000 0.000000");
		SpawnInteriorObject("bldr_hospital_transport_bed", "14.722669 -7.355003 -3.759297", "88.000015 0.000000 0.000000");
		SpawnInteriorObject("bldr_lobby_case", "21.750000 -7.306725 2.140083", "-179.999985 0.000000 0.000000");
		SpawnInteriorObject("bldr_locker_closed_blue_v1", "21.780001 -7.349846 5.805788", "0.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_locker_closed_blue_v2", "21.379000 -7.349991 5.800000", "0.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_locker_closed_blue_v3", "20.978001 -7.349998 5.800000", "0.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_locker_open_blue_v1", "20.577000 -7.349998 5.800000", "0.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_locker_closed_blue_v2", "20.177000 -7.349998 5.800000", "0.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_locker_closed_blue_v2", "19.800011 -7.254997 5.800000", "0.000000 0.000000 -29.299999");
		SpawnInteriorObject("bldr_Garbage_Pile5", "16.191998 -7.500565 2.902924", "-77.000000 8.000001 0.000000");
		SpawnInteriorObject("bldr_tent_stretcher", "-10.044797 -6.264999 -3.185241", "-179.999985 -78.000015 0.000000");
		SpawnInteriorObject("bldr_tent_stretcher", "-9.289997 -6.300003 -3.306000", "179.999985 -71.999992 0.000000");
		SpawnInteriorObject("bldr_tent_stretcher", "-9.132716 -7.139999 -3.075000", "89.999992 0.000000 65.000000");
		SpawnInteriorObject("bldr_Rubble_Glass", "-11.676440 -7.330002 -3.308930", "-40.999985 0.000000 0.000000");
		SpawnInteriorObject("bldr_hospital_bench", "-6.693476 -6.839996 -4.080720", "135.000061 43.000011 179.999985");
		SpawnInteriorObject("bldr_ch_office_b", "-0.902540 -7.352997 -3.300192", "0.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_ch_office_b", "0.982030 -7.252998 -3.323052", "-52.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_pc", "-1.331690 -6.360001 -3.758028", "100.000015 0.000000 0.000000");
		SpawnInteriorObject("bldr_ch_office_b", "0.939008 -7.050003 -4.823634", "-162.000031 -90.000000 -84.999985");
		SpawnInteriorObject("bldr_tent_stretcher", "4.710000 -6.264999 -3.289683", "-90.000000 -77.000107 0.000000");
		SpawnInteriorObject("bldr_hospital_transport_bed", "-14.351755 -7.355003 -3.759404", "-54.999992 0.000000 0.000000");
		SpawnInteriorObject("bldr_patient_monitor", "8.977751 -7.355003 -3.545898", "-29.000008 0.000000 0.000000");
		SpawnInteriorObject("bldr_medical_table", "18.231068 -7.352997 -1.191036", "176.999908 0.000000 0.000000");
		SpawnInteriorObject("bldr_lab_dropper", "18.180618 -6.291000 -1.194494", "-14.000003 0.000000 0.000000");
		SpawnInteriorObject("bldr_lab_microscope", "13.627949 -6.336998 0.328872", "-115.999962 0.000000 0.000000");
		SpawnInteriorObject("bldr_medical_table", "13.413227 -7.355003 0.349588", "-6.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_lab_beaker", "13.219695 -6.279999 0.319374", "0.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_lab_cylinder_beaker", "13.034000 -6.305000 0.275033", "-66.000000 0.000000 -94.000023");
		SpawnInteriorObject("bldr_lab_petri_dish", "18.322691 -6.325996 -1.255783", "0.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_lab_erlenmeyer_flask", "18.527784 -6.275002 -1.104080", "0.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_lab_teacher_bench", "20.107933 -7.339996 -1.105498", "-179.999985 0.000000 0.000000");
		SpawnInteriorObject("bldr_lab_volume_beaker", "21.177729 -6.389999 -0.968928", "179.999985 -65.000015 179.999985");
		SpawnInteriorObject("bldr_wall_board", "0.411482 -6.745628 -2.250000", "-179.999985 0.000000 2.000000");
		SpawnInteriorObject("bldr_case_bedroom_a", "5.860759 -7.149948 -4.055658", "-179.999985 0.000000 0.000000");
		SpawnInteriorObject("bldr_BookIvanhoe", "5.948526 -6.787003 -4.055871", "-78.999939 -90.000000 -0.000009");
		SpawnInteriorObject("bldr_BookLaChartreuseDeParme", "5.961479 -6.839996 -4.074384", "83.645996 89.934380 178.541138");
		SpawnInteriorObject("bldr_flowers_02", "5.982726 -7.349960 -0.746727", "46.999992 0.000000 0.000000");
		SpawnInteriorObject("bldr_flowers_01", "-7.837697 -7.348213 -1.188997", "0.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_Rubble_Glass", "-6.065307 -7.322578 -2.974261", "52.999996 0.000000 0.000000");
		SpawnInteriorObject("bldr_Rubble_Glass", "5.287378 -7.322701 -3.227968", "-36.999996 0.000000 0.000000");
		SpawnInteriorObject("bldr_Rubble_Glass", "10.419467 -7.322998 -3.159271", "-132.999954 0.000000 0.000000");
		SpawnInteriorObject("bldr_school_clock", "5.460000 -4.743858 -2.441180", "-90.000000 0.000000 2.000000");
		SpawnInteriorObject("bldr_school_clock", "-1.102509 -4.442413 -2.240000", "0.000000 0.000000 -2.000000");
		SpawnInteriorObject("bldr_tac_board_posters_level1", "-17.260000 -5.844345 -2.421566", "-90.000000 0.000000 1.000000");
		SpawnInteriorObject("bldr_hospital_bench", "-16.969471 -7.349960 -2.093777", "-88.000046 0.000000 0.000000");
		SpawnInteriorObject("bldr_hospital_bench", "-16.886604 -7.343636 -3.628737", "-79.000008 0.000000 0.000000");
		SpawnInteriorObject("bldr_wall_board_02", "3.474249 -6.547958 -0.270000", "-179.999985 0.000000 -1.000000");
		SpawnInteriorObject("bldr_hospital_transport_bed", "-5.632685 -7.355003 2.449999", "94.000031 0.000000 0.000000");
		SpawnInteriorObject("bldr_hospital_transport_bed", "-15.060718 -6.989998 2.752617", "-95.000031 0.000000 -92.500000");
		SpawnInteriorObject("bldr_fluor_lamp", "2.575140 -3.892166 -3.800000", "90.000038 0.000000 0.000000");
		SpawnInteriorObject("bldr_fluor_lamp", "-4.731559 -3.892998 -3.800000", "90.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_fluor_lamp", "7.784429 -3.892998 -2.788770", "0.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_fluor_lamp", "13.280000 -4.743050 -4.400002", "35.999992 -81.999985 77.999992");
		SpawnInteriorObject("bldr_fluor_lamp", "13.280000 -4.742996 -4.400000", "-143.999985 82.000008 101.999985");
		SpawnInteriorObject("bldr_pc", "-20.266596 -6.644501 0.473941", "-8.000002 0.000000 0.000000");
		SpawnInteriorObject("bldr_radio_b", "-20.120201 -6.639999 5.569616", "-145.000031 0.000000 0.000000");
		SpawnInteriorObject("bldr_school_clock", "-22.230000 -5.000885 2.272854", "-90.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_wall_board_03", "-22.219999 -7.201088 1.665529", "90.000000 0.000000 1.000000");
		SpawnInteriorObject("bldr_wall_board", "-22.219999 -7.270004 2.860000", "90.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_hospital_transport_bed", "16.981598 -6.563004 0.910399", "148.000168 -18.999985 100.999985");
		SpawnInteriorObject("bldr_office_desk", "17.852112 -3.769997 4.288498", "-18.000006 89.000046 0.000000");
		SpawnInteriorObject("bldr_office_chair", "21.624048 -3.960823 5.599986", "-18.000010 -84.000008 0.000000");
		SpawnInteriorObject("bldr_office_chair", "21.173269 -3.739998 5.612000", "62.999996 0.000000 0.000000");
		SpawnInteriorObject("bldr_office_chair", "21.612936 -3.739998 5.251245", "-27.000010 0.000000 0.000000");
		SpawnInteriorObject("bldr_office_chair", "21.620001 -3.399162 5.729029", "-14.000003 0.000000 53.000015");
		SpawnInteriorObject("bldr_office_chair", "20.592489 -3.754997 5.597434", "-176.999954 0.000000 128.999939");
		SpawnInteriorObject("bldr_office_chair", "21.013077 -3.960999 5.245245", "36.000011 -84.000008 0.000000");
		SpawnInteriorObject("bldr_phonebox", "17.470221 -3.675209 -1.014250", "179.999939 0.000000 0.000000");
		SpawnInteriorObject("bldr_phonebox", "18.819979 -3.675201 -1.014000", "-179.999985 0.000000 0.000000");
		SpawnInteriorObject("bldr_vending_machine", "21.387135 -3.190002 1.861689", "96.000038 0.000000 -101.399994");
		SpawnInteriorObject("bldr_office_table_a", "14.065847 -3.620003 0.200000", "94.000031 0.000000 0.000000");
		SpawnInteriorObject("bldr_case_bedroom_a", "12.309505 -3.449966 1.764018", "-87.000008 0.000000 0.000000");
		SpawnInteriorObject("bldr_hospital_transport_bed", "21.518728 -0.830002 3.239552", "-7.000000 -13.500000 0.000000");
		SpawnInteriorObject("bldr_extinguisher", "-8.090000 -6.613327 -1.994567", "89.000038 0.000000 0.000000");
		SpawnInteriorObject("bldr_extinguisher", "18.820000 -6.613998 -4.321878", "-91.000015 0.000000 0.000000");
		SpawnInteriorObject("bldr_extinguisher", "9.539177 -2.947136 -1.420000", "0.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_extinguisher", "21.909002 0.417213 -0.392760", "-94.000031 0.000000 0.000000");
		SpawnInteriorObject("bldr_debris_small_house", "16.033430 -0.995049 4.962839", "-54.999989 0.000000 0.000000");
		SpawnInteriorObject("bldr_anatomy_poster_1", "-3.160000 -5.249832 7.970597", "90.000015 0.000000 0.000000");
		SpawnInteriorObject("bldr_anatomy_poster_2", "-9.397601 -5.200966 7.709916", "-90.000038 0.000000 0.000000");
		SpawnInteriorObject("bldr_radar_electronics", "-6.364903 -6.456604 3.546791", "-179.999985 0.000000 0.000000");
		SpawnInteriorObject("bldr_radar_rack", "-5.437923 -7.351097 3.480243", "-179.999985 0.000000 0.000000");
		SpawnInteriorObject("bldr_hospital_bench", "17.494963 2.161507 5.781741", "0.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_hospital_bench", "18.700550 2.430000 5.073025", "46.999985 -90.000000 -0.000009");
		SpawnInteriorObject("bldr_case_bedroom_a", "16.477476 2.392921 5.544453", "-82.999992 0.000000 0.000000");
		SpawnInteriorObject("bldr_flowers_02", "20.311176 2.161507 5.701402", "36.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_flowers_01", "16.473728 2.667900 5.687332", "0.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_fluor_lamp", "15.315670 0.211388 0.860523", "-36.999996 17.999990 164.000015");
		SpawnInteriorObject("bldr_box_c_multi", "18.519211 3.330002 -1.186070", "0.000000 0.000000 0.000000", 0.40 );
		SpawnInteriorObject("bldr_hospital_transport_bed", "-6.556181 -6.985001 6.509215", "-20.000032 0.000000 -92.500000");
		SpawnInteriorObject("bldr_hospital_transport_bed", "-4.927142 -7.355003 6.231168", "14.000003 0.000000 0.000000");
		SpawnInteriorObject("bldr_Rubble_Glass", "-6.260639 -7.332108 7.168685", "153.000031 0.000000 0.000000");
		SpawnInteriorObject("bldr_pc", "-10.114962 -6.639999 7.788950", "-4.999999 0.000000 0.000000");
		SpawnInteriorObject("bldr_patient_monitor", "19.399519 0.239998 -1.031490", "35.999989 -17.999992 -100.000015");
		SpawnInteriorObject("bldr_anatomy_poster_1", "-9.660000 -5.250969 6.732413", "-90.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_anatomy_poster_2", "-9.660000 -5.250969 8.266364", "90.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_class_case_a_closed", "-14.640652 -7.351105 3.472095", "179.999954 0.000000 0.000000");
		SpawnInteriorObject("bldr_class_case_b_closed", "-15.774100 -7.351105 3.470000", "-179.999985 0.000000 0.000000");
		SpawnInteriorObject("bldr_desk_globe", "-14.320700 -5.970001 3.437326", "0.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_flowers_01", "-14.818839 -6.201035 3.442057", "0.000000 0.000000 0.000000");
		SpawnInteriorObject("bldr_extinguisher", "-12.674706 -7.268631 9.553238", "151.999878 58.000065 89.999992");
	}
}