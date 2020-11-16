/**
 * JMVehicleSpawnerSettings.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class JMVehicleSpawnerSettings
{
	void JMVehicleSpawnerSettings()
	{
		#ifdef EXPANSION_COT_VEHICLE_FORCE_DEFAULTS
		Defaults();
		#endif
	}

	override void Defaults()
	{
		super.Defaults();

		DefaultExpansionMerlin();
		DefaultExpansionMh6();
		DefaultExpansionUh1h();
		DefaultExpansionGyro();

		DefaultExpansionTractor();
		DefaultExpansionBus();

		DefaultExpansionUAZ();
		DefaultExpansionUAZRoofless();
		DefaultExpansionUAZCargoRoofless();
		DefaultExpansionUAZCargo();

		DefaultExpansionVodnik();

		DefaultExpansionZodiacBoat();
		DefaultExpansionUtilityBoat();
		DefaultExpansionLHD();

		DefaultExpansionAN2();
		DefaultExpansionC130J();
		
		DefaultExpansionTT650();
		DefaultExpansion250N();
		DefaultExpansionOldBike();
	}

	void DefaultExpansionMerlin()
	{
		ref array< string> attArr = new array< string>;

		attArr.Insert("ExpansionHelicopterBattery");
		attArr.Insert("ExpansionHydraulicHoses");
		attArr.Insert("ExpansionIgniterPlug");
		attArr.Insert("HeadlightH7");

		attArr.Insert("Vehicle_ExpansionMerlinFrontWheel");
		attArr.Insert("Vehicle_ExpansionMerlinFrontWheel");
		attArr.Insert("Vehicle_ExpansionMerlinBackWheel");
		attArr.Insert("Vehicle_ExpansionMerlinBackWheel");

		ref JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "Vehicle_ExpansionMerlin";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultExpansionMh6()
	{
		ref array< string> attArr = new array< string>;

		attArr.Insert("ExpansionHelicopterBattery");
		attArr.Insert("ExpansionIgniterPlug");
		attArr.Insert("ExpansionHydraulicHoses");
		attArr.Insert("HeadlightH7");

		attArr.Insert("Expansion_Mh6_Door_1_1");
		attArr.Insert("Expansion_Mh6_Door_1_2");
		attArr.Insert("Expansion_Mh6_Door_2_1");
		attArr.Insert("Expansion_Mh6_Door_2_2");

		ref JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "Vehicle_ExpansionMh6";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultExpansionUh1h()
	{
		ref array< string> attArr = new array< string>;

		attArr.Insert("ExpansionHelicopterBattery");
		attArr.Insert("ExpansionHydraulicHoses");
		attArr.Insert("ExpansionIgniterPlug");
		attArr.Insert("HeadlightH7");

		attArr.Insert("ExpansionUh1hDoor_1_1");
		attArr.Insert("ExpansionUh1hDoor_1_2");

		ref JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "Vehicle_ExpansionUh1h";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}
	
	void DefaultExpansionGyro()
	{
		ref array< string> attArr = new array< string>;

		attArr.Insert("ExpansionHelicopterBattery");
		attArr.Insert("SparkPlug");
		attArr.Insert("HeadlightH7");

		ref JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "Vehicle_ExpansionGyrocopter";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultExpansionTractor()
	{
		ref array< string> attArr = new array< string>;

		attArr.Insert("Vehicle_ExpansionTractorFrontWheel");
		attArr.Insert("Vehicle_ExpansionTractorFrontWheel");
		attArr.Insert("Vehicle_ExpansionTractorBackWheel");
		attArr.Insert("Vehicle_ExpansionTractorBackWheel");
		attArr.Insert("HeadlightH7");
		attArr.Insert("HeadlightH7");

		attArr.Insert("ExpansionTractorDoorsDriver");
		attArr.Insert("ExpansionTractorDoorsCodriver");

		attArr.Insert("CarBattery");
		//attArr.Insert("CarRadiator");
		attArr.Insert("SparkPlug");

		ref JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "Vehicle_ExpansionTractor";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultExpansionUAZ()
	{
		ref array< string> attArr = new array< string>;

		attArr.Insert("Vehicle_ExpansionUAZWheel");
		attArr.Insert("Vehicle_ExpansionUAZWheel");
		attArr.Insert("Vehicle_ExpansionUAZWheel");
		attArr.Insert("Vehicle_ExpansionUAZWheel");
		attArr.Insert("HeadlightH7");
		attArr.Insert("HeadlightH7");

		attArr.Insert("ExpansionUAZDoorDriver");
		attArr.Insert("ExpansionUAZDoorCoDriver");
		attArr.Insert("ExpansionUAZDoorCargo1");
		attArr.Insert("ExpansionUAZDoorCargo2");

		attArr.Insert("CarBattery");
		attArr.Insert("CarRadiator");
		attArr.Insert("SparkPlug");

		ref JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "Vehicle_ExpansionUAZ";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultExpansionUAZRoofless()
	{
		ref array< string> attArr = new array< string>;

		attArr.Insert("Vehicle_ExpansionUAZWheel");
		attArr.Insert("Vehicle_ExpansionUAZWheel");
		attArr.Insert("Vehicle_ExpansionUAZWheel");
		attArr.Insert("Vehicle_ExpansionUAZWheel");
		attArr.Insert("HeadlightH7");
		attArr.Insert("HeadlightH7");

		attArr.Insert("ExpansionUAZDoorDriver");
		attArr.Insert("ExpansionUAZDoorCoDriver");
		attArr.Insert("ExpansionUAZDoorCargo1");
		attArr.Insert("ExpansionUAZDoorCargo2");

		attArr.Insert("CarBattery");
		attArr.Insert("CarRadiator");
		attArr.Insert("SparkPlug");

		ref JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "Vehicle_ExpansionUAZRoofless";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultExpansionUAZCargo()
	{
		ref array< string> attArr = new array< string>;

		attArr.Insert("Vehicle_ExpansionUAZWheel");
		attArr.Insert("Vehicle_ExpansionUAZWheel");
		attArr.Insert("Vehicle_ExpansionUAZWheel");
		attArr.Insert("Vehicle_ExpansionUAZWheel");
		attArr.Insert("HeadlightH7");
		attArr.Insert("HeadlightH7");

		attArr.Insert("ExpansionUAZDoorDriver");
		attArr.Insert("ExpansionUAZDoorCoDriver");
		attArr.Insert("ExpansionUAZDoorCargo1");
		attArr.Insert("ExpansionUAZDoorCargo2");

		attArr.Insert("CarBattery");
		attArr.Insert("CarRadiator");
		attArr.Insert("SparkPlug");

		ref JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "Vehicle_ExpansionUAZCargo";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultExpansionUAZCargoRoofless()
	{
		ref array< string> attArr = new array< string>;

		attArr.Insert("Vehicle_ExpansionUAZWheel");
		attArr.Insert("Vehicle_ExpansionUAZWheel");
		attArr.Insert("Vehicle_ExpansionUAZWheel");
		attArr.Insert("Vehicle_ExpansionUAZWheel");
		attArr.Insert("HeadlightH7");
		attArr.Insert("HeadlightH7");

		attArr.Insert("ExpansionUAZDoorDriver");
		attArr.Insert("ExpansionUAZDoorCoDriver");
		attArr.Insert("ExpansionUAZDoorCargo1");
		attArr.Insert("ExpansionUAZDoorCargo2");

		attArr.Insert("CarBattery");
		attArr.Insert("CarRadiator");
		attArr.Insert("SparkPlug");

		ref JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "Vehicle_ExpansionUAZCargoRoofless";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultExpansionVodnik()
	{
		ref array< string> attArr = new array< string>;

		attArr.Insert("Vehicle_ExpansionVodnikWheel");
		attArr.Insert("Vehicle_ExpansionVodnikWheel");
		attArr.Insert("Vehicle_ExpansionVodnikWheel");
		attArr.Insert("Vehicle_ExpansionVodnikWheel");
		attArr.Insert("HeadlightH7");
		attArr.Insert("HeadlightH7");

		attArr.Insert("ExpansionVodnikDoorDriver");
		attArr.Insert("ExpansionVodnikDoorCoDriver");

		attArr.Insert("TruckBattery");
		attArr.Insert("CarRadiator");
		attArr.Insert("SparkPlug");

		ref JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "Vehicle_ExpansionVodnik";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultExpansionAmphibiaBoat()
	{
		ref array< string> attArr = new array< string>;

		attArr.Insert("Vehicle_ExpansionUtilityBoatWheel");
		attArr.Insert("Vehicle_ExpansionUtilityBoatWheel");
		attArr.Insert("Vehicle_ExpansionUtilityBoatWheel");
		attArr.Insert("Vehicle_ExpansionUtilityBoatWheel");
		attArr.Insert("HeadlightH7");
		attArr.Insert("HeadlightH7");

		attArr.Insert("CarBattery");
		attArr.Insert("CarRadiator");
		attArr.Insert("SparkPlug");

		ref JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "Vehicle_ExpansionAmphibiaBoat";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultExpansionBus()
	{
		ref array< string> attArr = new array< string>;

		attArr.Insert("Vehicle_ExpansionBusWheel");
		attArr.Insert("Vehicle_ExpansionBusWheel");
		attArr.Insert("Vehicle_ExpansionBusWheelDouble");
		attArr.Insert("Vehicle_ExpansionBusWheelDouble");
		attArr.Insert("HeadlightH7");
		attArr.Insert("HeadlightH7");

		attArr.Insert("TruckBattery");
		attArr.Insert("CarRadiator");
		attArr.Insert("GlowPlug");

		ref JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "Vehicle_ExpansionBus";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultExpansionLHD()
	{
		ref array< string> attArr = new array< string>;

		attArr.Insert("CarRadiator");
		attArr.Insert("GlowPlug");
		attArr.Insert("TruckBattery");
		attArr.Insert("HeadlightH7");
		attArr.Insert("HeadlightH7");

		ref JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "Vehicle_ExpansionLHD";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultExpansionZodiacBoat()
	{
		ref array< string> attArr = new array< string>;

		attArr.Insert("CarRadiator");
		attArr.Insert("GlowPlug");
		attArr.Insert("TruckBattery");

		ref JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "Vehicle_ExpansionZodiacBoat";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultExpansionUtilityBoat()
	{
		ref array< string> attArr = new array< string>;

		attArr.Insert("CarRadiator");
		attArr.Insert("GlowPlug");
		attArr.Insert("TruckBattery");
		attArr.Insert("HeadlightH7");
		attArr.Insert("HeadlightH7");

		ref JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "Vehicle_ExpansionUtilityBoat";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultExpansionAN2()
	{
		ref array< string> attArr = new array< string>;

		attArr.Insert("Vehicle_ExpansionAnWheel");
		attArr.Insert("Vehicle_ExpansionAnWheel");
		attArr.Insert("Vehicle_ExpansionAnWheelSmall");
		attArr.Insert("Vehicle_ExpansionAnWheelSmall");
		
		attArr.Insert("ExpansionAircraftBattery");
		attArr.Insert("CarRadiator");
		attArr.Insert("GlowPlug");
		attArr.Insert("HeadlightH7");
		attArr.Insert("HeadlightH7");

		ref JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "Vehicle_ExpansionAn2";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultExpansionC130J()
	{
		ref array< string> attArr = new array< string>;

		attArr.Insert("Vehicle_ExpansionC130JWheel");
		attArr.Insert("Vehicle_ExpansionC130JWheel");
		attArr.Insert("Vehicle_ExpansionC130JWheel");
		attArr.Insert("Vehicle_ExpansionC130JWheel");
		
		attArr.Insert("ExpansionAircraftBattery");
		attArr.Insert("CarRadiator");
		attArr.Insert("GlowPlug");
		attArr.Insert("HeadlightH7");
		attArr.Insert("HeadlightH7");

		ref JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "Vehicle_ExpansionC130J";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultExpansionTT650()
	{
		ref array< string> attArr = new array< string>;

		attArr.Insert("Vehicle_ExpansionTT650Wheel");
		attArr.Insert("Vehicle_ExpansionTT650Wheel");
		attArr.Insert("CarBattery");
		attArr.Insert("CarRadiator");
		attArr.Insert("GlowPlug");
		attArr.Insert("HeadlightH7");

		ref JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "Vehicle_ExpansionTT650";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultExpansion250N()
	{
		ref array< string> attArr = new array< string>;

		attArr.Insert("Vehicle_Expansion250NWheel");
		attArr.Insert("Vehicle_Expansion250NWheel");

		ref JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "Vehicle_Expansion250N";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultExpansionOldBike()
	{
		ref array< string> attArr = new array< string>;

		attArr.Insert("Vehicle_ExpansionOldBikeWheel");
		attArr.Insert("Vehicle_ExpansionOldBikeWheel");

		ref JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "Vehicle_ExpansionOldBike";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	override void Default_CivilianSedan()
	{
		ref array< string> attArr = new array< string>;

		attArr.Insert("CivSedanHood");
		attArr.Insert("CivSedanTrunk");
		attArr.Insert("CivSedanDoors_Driver");
		attArr.Insert("CivSedanDoors_BackRight");
		attArr.Insert("CivSedanDoors_BackLeft");
		attArr.Insert("CivSedanDoors_CoDriver");
		attArr.Insert("Vehicle_CivSedanWheel");
		attArr.Insert("Vehicle_CivSedanWheel");
		attArr.Insert("Vehicle_CivSedanWheel");
		attArr.Insert("Vehicle_CivSedanWheel");
		attArr.Insert("CarBattery");
		attArr.Insert("CarRadiator");
		attArr.Insert("SparkPlug");
		attArr.Insert("HeadlightH7");
		attArr.Insert("HeadlightH7");

		ref JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "Vehicle_CivilianSedan";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	override void Default_OffroadHatchback()
	{
		ref array< string> attArr = new array< string>;

		attArr.Insert("HatchbackHood");
		attArr.Insert("HatchbackTrunk");
		attArr.Insert("HatchbackDoors_Driver");
		attArr.Insert("HatchbackDoors_CoDriver");
		attArr.Insert("Vehicle_HatchbackWheel");
		attArr.Insert("Vehicle_HatchbackWheel");
		attArr.Insert("Vehicle_HatchbackWheel");
		attArr.Insert("Vehicle_HatchbackWheel");
		attArr.Insert("CarBattery");
		attArr.Insert("CarRadiator");
		attArr.Insert("SparkPlug");
		attArr.Insert("HeadlightH7");
		attArr.Insert("HeadlightH7");

		ref JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "Vehicle_OffroadHatchback";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	override void Default_Hatchback_02()
	{
		ref array< string> attArr = new array< string>;

		attArr.Insert("Hatchback_02_Hood");
		attArr.Insert("Hatchback_02_Trunk");
		attArr.Insert("Hatchback_02_Door_1_1");
		attArr.Insert("Hatchback_02_Door_1_2");
		attArr.Insert("Hatchback_02_Door_2_1");
		attArr.Insert("Hatchback_02_Door_2_2");
		attArr.Insert("Vehicle_Hatchback_02_Wheel");
		attArr.Insert("Vehicle_Hatchback_02_Wheel");
		attArr.Insert("Vehicle_Hatchback_02_Wheel");
		attArr.Insert("Vehicle_Hatchback_02_Wheel");
		attArr.Insert("CarBattery");
		attArr.Insert("CarRadiator");
		attArr.Insert("SparkPlug");
		attArr.Insert("HeadlightH7");
		attArr.Insert("HeadlightH7");

		ref JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "Vehicle_Hatchback_02";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	override void Default_Sedan_02()
	{
		ref array< string> attArr = new array< string>;

		attArr.Insert("Sedan_02_Hood");
		attArr.Insert("Sedan_02_Trunk");
		attArr.Insert("Sedan_02_Door_1_1");
		attArr.Insert("Sedan_02_Door_1_2");
		attArr.Insert("Sedan_02_Door_2_1");
		attArr.Insert("Sedan_02_Door_2_2");
		attArr.Insert("Vehicle_Sedan_02_Wheel");
		attArr.Insert("Vehicle_Sedan_02_Wheel");
		attArr.Insert("Vehicle_Sedan_02_Wheel");
		attArr.Insert("Vehicle_Sedan_02_Wheel");
		attArr.Insert("CarBattery");
		attArr.Insert("CarRadiator");
		attArr.Insert("SparkPlug");
		attArr.Insert("HeadlightH7");
		attArr.Insert("HeadlightH7");

		ref JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "Vehicle_Sedan_02";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}
};