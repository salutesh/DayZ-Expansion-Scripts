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
#ifdef JM_COT

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

		//DefaultExpansionAN2();
		//DefaultExpansionC130J();

		//DefaultExpansionTT650();
		//DefaultExpansion250N();
		//DefaultExpansionOldBike();

		//#ifdef EXPANSION_COT_VEHICLE_JACOB
		DefaultVehicle_ExpansionMerlin();
		DefaultVehicle_ExpansionMh6();
		DefaultVehicle_ExpansionUh1h();
		DefaultVehicle_ExpansionGyro();

		DefaultVehicle_ExpansionTractor();
		DefaultVehicle_ExpansionBus();

		DefaultVehicle_ExpansionUAZ();
		DefaultVehicle_ExpansionUAZRoofless();
		DefaultVehicle_ExpansionUAZCargoRoofless();
		DefaultVehicle_ExpansionUAZCargo();

		DefaultVehicle_ExpansionVodnik();

		DefaultVehicle_ExpansionZodiacBoat();
		DefaultVehicle_ExpansionUtilityBoat();
		DefaultVehicle_ExpansionLHD();

		DefaultVehicle_ExpansionAN2();
		DefaultVehicle_ExpansionC130J();
		
		DefaultVehicle_ExpansionTT650();
		DefaultVehicle_Expansion250N();
		DefaultVehicle_ExpansionOldBike();

		DefaultVehicle_CivilianSedan();
		DefaultVehicle_OffroadHatchback();
		DefaultVehicle_Hatchback_02();
		DefaultVehicle_Sedan_02();
		DefaultVehicle_Truck_01_Covered();
		//DefaultVehicle_Truck_01_Chassis();
		//DefaultVehicle_Truck_01_Cargo();
		//DefaultVehicle_Truck_01_Command();
		//#endif
	}

	void DefaultVehicle_ExpansionMerlin()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("ExpansionHelicopterBattery");
		attArr.Insert("ExpansionHydraulicHoses");
		attArr.Insert("ExpansionIgniterPlug");
		attArr.Insert("HeadlightH7");

		attArr.Insert("Vehicle_ExpansionMerlinFrontWheel");
		attArr.Insert("Vehicle_ExpansionMerlinFrontWheel");
		attArr.Insert("Vehicle_ExpansionMerlinBackWheel");
		attArr.Insert("Vehicle_ExpansionMerlinBackWheel");

		JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "Vehicle_ExpansionMerlin";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultVehicle_ExpansionMh6()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("ExpansionHelicopterBattery");
		attArr.Insert("ExpansionIgniterPlug");
		attArr.Insert("ExpansionHydraulicHoses");
		attArr.Insert("HeadlightH7");

		attArr.Insert("Expansion_Mh6_Door_1_1");
		attArr.Insert("Expansion_Mh6_Door_1_2");
		attArr.Insert("Expansion_Mh6_Door_2_1");
		attArr.Insert("Expansion_Mh6_Door_2_2");

		JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "Vehicle_ExpansionMh6";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultVehicle_ExpansionUh1h()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("ExpansionHelicopterBattery");
		attArr.Insert("ExpansionHydraulicHoses");
		attArr.Insert("ExpansionIgniterPlug");
		attArr.Insert("HeadlightH7");

		attArr.Insert("ExpansionUh1hDoor_1_1");
		attArr.Insert("ExpansionUh1hDoor_1_2");

		JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "Vehicle_ExpansionUh1h";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}
	
	void DefaultVehicle_ExpansionGyro()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("ExpansionHelicopterBattery");
		attArr.Insert("SparkPlug");
		attArr.Insert("HeadlightH7");

		JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "Vehicle_ExpansionGyrocopter";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultVehicle_ExpansionTractor()
	{
		array< string> attArr = new array< string>;

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

		JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "Vehicle_ExpansionTractor";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultVehicle_ExpansionUAZ()
	{
		array< string> attArr = new array< string>;

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

		JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "Vehicle_ExpansionUAZ";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultVehicle_ExpansionUAZRoofless()
	{
		array< string> attArr = new array< string>;

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

		JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "Vehicle_ExpansionUAZRoofless";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultVehicle_ExpansionUAZCargo()
	{
		array< string> attArr = new array< string>;

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

		JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "Vehicle_ExpansionUAZCargo";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultVehicle_ExpansionUAZCargoRoofless()
	{
		array< string> attArr = new array< string>;

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

		JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "Vehicle_ExpansionUAZCargoRoofless";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultVehicle_ExpansionVodnik()
	{
		array< string> attArr = new array< string>;

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

		JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "Vehicle_ExpansionVodnik";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultVehicle_ExpansionAmphibiaBoat()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("Vehicle_ExpansionUtilityBoatWheel");
		attArr.Insert("Vehicle_ExpansionUtilityBoatWheel");
		attArr.Insert("Vehicle_ExpansionUtilityBoatWheel");
		attArr.Insert("Vehicle_ExpansionUtilityBoatWheel");
		attArr.Insert("HeadlightH7");
		attArr.Insert("HeadlightH7");

		attArr.Insert("CarBattery");
		attArr.Insert("CarRadiator");
		attArr.Insert("SparkPlug");

		JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "Vehicle_ExpansionAmphibiaBoat";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultVehicle_ExpansionBus()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("Vehicle_ExpansionBusWheel");
		attArr.Insert("Vehicle_ExpansionBusWheel");
		attArr.Insert("Vehicle_ExpansionBusWheelDouble");
		attArr.Insert("Vehicle_ExpansionBusWheelDouble");
		attArr.Insert("HeadlightH7");
		attArr.Insert("HeadlightH7");

		attArr.Insert("TruckBattery");
		attArr.Insert("CarRadiator");
		attArr.Insert("GlowPlug");

		JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "Vehicle_ExpansionBus";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultVehicle_ExpansionLHD()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("CarRadiator");
		attArr.Insert("GlowPlug");
		attArr.Insert("TruckBattery");
		attArr.Insert("HeadlightH7");
		attArr.Insert("HeadlightH7");

		JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "Vehicle_ExpansionLHD";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultVehicle_ExpansionZodiacBoat()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("CarRadiator");
		attArr.Insert("GlowPlug");
		attArr.Insert("TruckBattery");

		JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "Vehicle_ExpansionZodiacBoat";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultVehicle_ExpansionUtilityBoat()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("CarRadiator");
		attArr.Insert("GlowPlug");
		attArr.Insert("TruckBattery");
		attArr.Insert("HeadlightH7");
		attArr.Insert("HeadlightH7");

		JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "Vehicle_ExpansionUtilityBoat";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultVehicle_ExpansionAN2()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("Vehicle_ExpansionAnWheel");
		attArr.Insert("Vehicle_ExpansionAnWheel");
		attArr.Insert("Vehicle_ExpansionAnWheelSmall");
		attArr.Insert("Vehicle_ExpansionAnWheelSmall");
		
		attArr.Insert("ExpansionAircraftBattery");
		attArr.Insert("CarRadiator");
		attArr.Insert("GlowPlug");
		attArr.Insert("HeadlightH7");
		attArr.Insert("HeadlightH7");

		JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "Vehicle_ExpansionAn2";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultVehicle_ExpansionC130J()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("Vehicle_ExpansionC130JWheel");
		attArr.Insert("Vehicle_ExpansionC130JWheel");
		attArr.Insert("Vehicle_ExpansionC130JWheel");
		attArr.Insert("Vehicle_ExpansionC130JWheel");
		
		attArr.Insert("ExpansionAircraftBattery");
		attArr.Insert("CarRadiator");
		attArr.Insert("GlowPlug");
		attArr.Insert("HeadlightH7");
		attArr.Insert("HeadlightH7");

		JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "Vehicle_ExpansionC130J";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultVehicle_ExpansionTT650()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("Vehicle_ExpansionTT650Wheel");
		attArr.Insert("Vehicle_ExpansionTT650Wheel");
		attArr.Insert("CarBattery");
		attArr.Insert("CarRadiator");
		attArr.Insert("GlowPlug");
		attArr.Insert("HeadlightH7");

		JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "Vehicle_ExpansionTT650";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultVehicle_Expansion250N()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("Vehicle_Expansion250NWheel");
		attArr.Insert("Vehicle_Expansion250NWheel");

		JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "Vehicle_Expansion250N";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultVehicle_ExpansionOldBike()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("Vehicle_ExpansionOldBikeWheel");
		attArr.Insert("Vehicle_ExpansionOldBikeWheel");

		JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "Vehicle_ExpansionOldBike";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultVehicle_CivilianSedan()
	{
		array< string> attArr = new array< string>;

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

		JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "Vehicle_CivilianSedan";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultVehicle_OffroadHatchback()
	{
		array< string> attArr = new array< string>;

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

		JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "Vehicle_OffroadHatchback";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultVehicle_Hatchback_02()
	{
		array< string> attArr = new array< string>;

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

		JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "Vehicle_Hatchback_02";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultVehicle_Sedan_02()
	{
		array< string> attArr = new array< string>;

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

		JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "Vehicle_Sedan_02";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultVehicle_Truck_01_Covered()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("Truck_01_Hood");
		attArr.Insert("Truck_01_Door_1_1");
		attArr.Insert("Truck_01_Door_2_1");
		attArr.Insert("Vehicle_Truck_01_WheelDouble");
		attArr.Insert("Vehicle_Truck_01_WheelDouble");
		attArr.Insert("Vehicle_Truck_01_WheelDouble");
		attArr.Insert("Vehicle_Truck_01_WheelDouble");
		attArr.Insert("Vehicle_Truck_01_Wheel");
		attArr.Insert("Vehicle_Truck_01_Wheel");
		attArr.Insert("Vehicle_Truck_01_Wheel");
		attArr.Insert("Vehicle_Truck_01_Wheel");
		attArr.Insert("TruckBattery");
		attArr.Insert("HeadlightH7");
		attArr.Insert("HeadlightH7");

		JMVehicleSpawnerSerialize file = JMVehicleSpawnerSerialize.Create();

		file.VehicleName = "Vehicle_Truck_01_Covered";
		file.m_FileName = file.VehicleName;
		file.Parts.Copy( attArr );

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultVehicle_Truck_01_Chassis()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("Truck_01_Hood");
		attArr.Insert("Truck_01_Door_1_1");
		attArr.Insert("Truck_01_Door_2_1");
		attArr.Insert("Vehicle_Truck_01_WheelDouble");
		attArr.Insert("Vehicle_Truck_01_WheelDouble");
		attArr.Insert("Vehicle_Truck_01_WheelDouble");
		attArr.Insert("Vehicle_Truck_01_WheelDouble");
		attArr.Insert("Vehicle_Truck_01_Wheel");
		attArr.Insert("Vehicle_Truck_01_Wheel");
		attArr.Insert("Vehicle_Truck_01_Wheel");
		attArr.Insert("Vehicle_Truck_01_Wheel");
		attArr.Insert("TruckBattery");
		attArr.Insert("SparkPlug");

		JMVehicleSpawnerSerialize file = JMVehicleSpawnerSerialize.Create();

		file.VehicleName = "Vehicle_Truck_01_Chassis_Blue";
		file.m_FileName = file.VehicleName;
		file.Parts.Copy( attArr );

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultVehicle_Truck_01_Cargo()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("Truck_01_Hood");
		attArr.Insert("Truck_01_Door_1_1");
		attArr.Insert("Truck_01_Door_2_1");
		attArr.Insert("Vehicle_Truck_01_WheelDouble");
		attArr.Insert("Vehicle_Truck_01_WheelDouble");
		attArr.Insert("Vehicle_Truck_01_WheelDouble");
		attArr.Insert("Vehicle_Truck_01_WheelDouble");
		attArr.Insert("Vehicle_Truck_01_Wheel");
		attArr.Insert("Vehicle_Truck_01_Wheel");
		attArr.Insert("Vehicle_Truck_01_Wheel");
		attArr.Insert("Vehicle_Truck_01_Wheel");
		attArr.Insert("TruckBattery");
		attArr.Insert("SparkPlug");

		JMVehicleSpawnerSerialize file = JMVehicleSpawnerSerialize.Create();

		file.VehicleName = "Vehicle_Truck_01_Cargo_Blue";
		file.m_FileName = file.VehicleName;
		file.Parts.Copy( attArr );

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultVehicle_Truck_01_Command()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("Truck_01_Hood");
		attArr.Insert("Truck_01_Door_1_1");
		attArr.Insert("Truck_01_Door_2_1");
		attArr.Insert("Vehicle_Truck_01_WheelDouble");
		attArr.Insert("Vehicle_Truck_01_WheelDouble");
		attArr.Insert("Vehicle_Truck_01_WheelDouble");
		attArr.Insert("Vehicle_Truck_01_WheelDouble");
		attArr.Insert("Vehicle_Truck_01_Wheel");
		attArr.Insert("Vehicle_Truck_01_Wheel");
		attArr.Insert("Vehicle_Truck_01_Wheel");
		attArr.Insert("Vehicle_Truck_01_Wheel");
		attArr.Insert("TruckBattery");
		attArr.Insert("SparkPlug");

		JMVehicleSpawnerSerialize file = JMVehicleSpawnerSerialize.Create();

		file.VehicleName = "Vehicle_Truck_01_Command";
		file.m_FileName = file.VehicleName;
		file.Parts.Copy( attArr );

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultExpansionMerlin()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("ExpansionHelicopterBattery");
		attArr.Insert("ExpansionHydraulicHoses");
		attArr.Insert("ExpansionIgniterPlug");
		attArr.Insert("HeadlightH7");

		attArr.Insert("ExpansionMerlinFrontWheel");
		attArr.Insert("ExpansionMerlinFrontWheel");
		attArr.Insert("ExpansionMerlinBackWheel");
		attArr.Insert("ExpansionMerlinBackWheel");

		JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "ExpansionMerlin";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultExpansionMh6()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("ExpansionHelicopterBattery");
		attArr.Insert("ExpansionIgniterPlug");
		attArr.Insert("ExpansionHydraulicHoses");
		attArr.Insert("HeadlightH7");

		attArr.Insert("Expansion_Mh6_Door_1_1");
		attArr.Insert("Expansion_Mh6_Door_1_2");
		attArr.Insert("Expansion_Mh6_Door_2_1");
		attArr.Insert("Expansion_Mh6_Door_2_2");

		JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "ExpansionMh6";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultExpansionUh1h()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("ExpansionHelicopterBattery");
		attArr.Insert("ExpansionHydraulicHoses");
		attArr.Insert("ExpansionIgniterPlug");
		attArr.Insert("HeadlightH7");

		attArr.Insert("ExpansionUh1hDoor_1_1");
		attArr.Insert("ExpansionUh1hDoor_1_2");

		JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "ExpansionUh1h";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}
	
	void DefaultExpansionGyro()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("ExpansionHelicopterBattery");
		attArr.Insert("SparkPlug");
		attArr.Insert("HeadlightH7");

		JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "ExpansionGyrocopter";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultExpansionTractor()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("ExpansionTractorFrontWheel");
		attArr.Insert("ExpansionTractorFrontWheel");
		attArr.Insert("ExpansionTractorBackWheel");
		attArr.Insert("ExpansionTractorBackWheel");
		attArr.Insert("HeadlightH7");
		attArr.Insert("HeadlightH7");

		attArr.Insert("ExpansionTractorDoorsDriver");
		attArr.Insert("ExpansionTractorDoorsCodriver");

		attArr.Insert("CarBattery");
		//attArr.Insert("CarRadiator");
		attArr.Insert("SparkPlug");

		JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "ExpansionTractor";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultExpansionUAZ()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("ExpansionUAZWheel");
		attArr.Insert("ExpansionUAZWheel");
		attArr.Insert("ExpansionUAZWheel");
		attArr.Insert("ExpansionUAZWheel");
		attArr.Insert("HeadlightH7");
		attArr.Insert("HeadlightH7");

		attArr.Insert("ExpansionUAZDoorDriver");
		attArr.Insert("ExpansionUAZDoorCoDriver");
		attArr.Insert("ExpansionUAZDoorCargo1");
		attArr.Insert("ExpansionUAZDoorCargo2");

		attArr.Insert("CarBattery");
		attArr.Insert("CarRadiator");
		attArr.Insert("SparkPlug");

		JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "ExpansionUAZ";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultExpansionUAZRoofless()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("ExpansionUAZWheel");
		attArr.Insert("ExpansionUAZWheel");
		attArr.Insert("ExpansionUAZWheel");
		attArr.Insert("ExpansionUAZWheel");
		attArr.Insert("HeadlightH7");
		attArr.Insert("HeadlightH7");

		attArr.Insert("ExpansionUAZDoorDriver");
		attArr.Insert("ExpansionUAZDoorCoDriver");
		attArr.Insert("ExpansionUAZDoorCargo1");
		attArr.Insert("ExpansionUAZDoorCargo2");

		attArr.Insert("CarBattery");
		attArr.Insert("CarRadiator");
		attArr.Insert("SparkPlug");

		JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "ExpansionUAZRoofless";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultExpansionUAZCargo()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("ExpansionUAZWheel");
		attArr.Insert("ExpansionUAZWheel");
		attArr.Insert("ExpansionUAZWheel");
		attArr.Insert("ExpansionUAZWheel");
		attArr.Insert("HeadlightH7");
		attArr.Insert("HeadlightH7");

		attArr.Insert("ExpansionUAZDoorDriver");
		attArr.Insert("ExpansionUAZDoorCoDriver");
		attArr.Insert("ExpansionUAZDoorCargo1");
		attArr.Insert("ExpansionUAZDoorCargo2");

		attArr.Insert("CarBattery");
		attArr.Insert("CarRadiator");
		attArr.Insert("SparkPlug");

		JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "ExpansionUAZCargo";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultExpansionUAZCargoRoofless()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("ExpansionUAZWheel");
		attArr.Insert("ExpansionUAZWheel");
		attArr.Insert("ExpansionUAZWheel");
		attArr.Insert("ExpansionUAZWheel");
		attArr.Insert("HeadlightH7");
		attArr.Insert("HeadlightH7");

		attArr.Insert("ExpansionUAZDoorDriver");
		attArr.Insert("ExpansionUAZDoorCoDriver");
		attArr.Insert("ExpansionUAZDoorCargo1");
		attArr.Insert("ExpansionUAZDoorCargo2");

		attArr.Insert("CarBattery");
		attArr.Insert("CarRadiator");
		attArr.Insert("SparkPlug");

		JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "ExpansionUAZCargoRoofless";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultExpansionVodnik()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("ExpansionVodnikWheel");
		attArr.Insert("ExpansionVodnikWheel");
		attArr.Insert("ExpansionVodnikWheel");
		attArr.Insert("ExpansionVodnikWheel");
		attArr.Insert("HeadlightH7");
		attArr.Insert("HeadlightH7");

		attArr.Insert("ExpansionVodnikDoorDriver");
		attArr.Insert("ExpansionVodnikDoorCoDriver");

		attArr.Insert("TruckBattery");
		attArr.Insert("CarRadiator");
		attArr.Insert("SparkPlug");

		JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "ExpansionVodnik";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultExpansionAmphibiaBoat()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("ExpansionUtilityBoatWheel");
		attArr.Insert("ExpansionUtilityBoatWheel");
		attArr.Insert("ExpansionUtilityBoatWheel");
		attArr.Insert("ExpansionUtilityBoatWheel");
		attArr.Insert("HeadlightH7");
		attArr.Insert("HeadlightH7");

		attArr.Insert("CarBattery");
		attArr.Insert("CarRadiator");
		attArr.Insert("SparkPlug");

		JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "ExpansionAmphibiaBoat";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultExpansionBus()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("ExpansionBusWheel");
		attArr.Insert("ExpansionBusWheel");
		attArr.Insert("ExpansionBusWheelDouble");
		attArr.Insert("ExpansionBusWheelDouble");
		attArr.Insert("HeadlightH7");
		attArr.Insert("HeadlightH7");

		attArr.Insert("TruckBattery");
		attArr.Insert("CarRadiator");
		attArr.Insert("GlowPlug");

		JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "ExpansionBus";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultExpansionLHD()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("CarRadiator");
		attArr.Insert("GlowPlug");
		attArr.Insert("TruckBattery");
		attArr.Insert("HeadlightH7");
		attArr.Insert("HeadlightH7");

		JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "ExpansionLHD";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultExpansionZodiacBoat()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("CarRadiator");
		attArr.Insert("GlowPlug");
		attArr.Insert("TruckBattery");

		JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "ExpansionZodiacBoat";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultExpansionUtilityBoat()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("CarRadiator");
		attArr.Insert("GlowPlug");
		attArr.Insert("TruckBattery");
		attArr.Insert("HeadlightH7");
		attArr.Insert("HeadlightH7");

		JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "ExpansionUtilityBoat";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultExpansionAN2()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("ExpansionAnWheel");
		attArr.Insert("ExpansionAnWheel");
		attArr.Insert("ExpansionAnWheelSmall");
		attArr.Insert("ExpansionAnWheelSmall");
		
		attArr.Insert("ExpansionAircraftBattery");
		attArr.Insert("CarRadiator");
		attArr.Insert("GlowPlug");
		attArr.Insert("HeadlightH7");
		attArr.Insert("HeadlightH7");

		JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "ExpansionAn2";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultExpansionC130J()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("ExpansionC130JWheel");
		attArr.Insert("ExpansionC130JWheel");
		attArr.Insert("ExpansionC130JWheel");
		attArr.Insert("ExpansionC130JWheel");
		
		attArr.Insert("ExpansionAircraftBattery");
		attArr.Insert("CarRadiator");
		attArr.Insert("GlowPlug");
		attArr.Insert("HeadlightH7");
		attArr.Insert("HeadlightH7");

		JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "ExpansionC130J";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultExpansionTT650()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("ExpansionTT650Wheel");
		attArr.Insert("ExpansionTT650Wheel");
		attArr.Insert("CarBattery");
		attArr.Insert("CarRadiator");
		attArr.Insert("GlowPlug");
		attArr.Insert("HeadlightH7");

		JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "ExpansionTT650";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultExpansion250N()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("Expansion250NWheel");
		attArr.Insert("Expansion250NWheel");

		JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "Expansion250N";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}

	void DefaultExpansionOldBike()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("ExpansionOldBikeWheel");
		attArr.Insert("ExpansionOldBikeWheel");

		JMVehicleSpawnerSerialize file = new JMVehicleSpawnerSerialize;

		file.VehicleName = "ExpansionOldBike";
		file.m_FileName = file.VehicleName;
		file.Parts = attArr;

		Vehicles.Insert( file.VehicleName, file );
	}
};

#endif