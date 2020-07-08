
static CarScript com_lastSpawnedCar;

class GameMenu extends PopupMenu
{
	TextListboxWidget m_gameScriptList;
	Widget 			  m_checkboxPanel;
	ButtonWidget 	  m_gameScriptButton;

	protected ref map< string, string > checkBoxMap = new map< string, string >; // store widget name

	string checkboxLayout = "$CurrentDir:\\Missions\\ExpansionCOM.enoch\\core\\modules\\ComEditor\\gui\\layouts\\CheckboxTemplate.layout";

	void GameMenu()
	{
	}

	void ~GameMenu()
	{
	}

	override void Init()
	{
		m_checkboxPanel = layoutRoot.FindAnyWidget("game_checkbox_panel");
		m_gameScriptList = TextListboxWidget.Cast(layoutRoot.FindAnyWidget("game_list_box"));
		m_gameScriptButton = ButtonWidget.Cast(layoutRoot.FindAnyWidget("game_script_button"));

		m_gameScriptList.AddItem( "Spawn Hatchback",		new Param1<string>("SpawnHatchback"),					0 );
		m_gameScriptList.AddItem( "Spawn Gunter",			new Param1<string>("SpawnHatchback_02"),				0 );
		m_gameScriptList.AddItem( "Spawn Sedan", 			new Param1<string>("SpawnSedan"), 	 					0 );
		m_gameScriptList.AddItem( "Spawn Sarka", 			new Param1<string>("SpawnSedan_02"), 	 				0 );
		m_gameScriptList.AddItem( "Spawn V3S Chassis",		new Param1<string>("SpawnTruck_01_Chassis"), 			0 );
		m_gameScriptList.AddItem( "Spawn V3S Cargo",		new Param1<string>("SpawnTruck_01_Cargo"), 				0 );
		m_gameScriptList.AddItem( "Spawn Utility",			new Param1<string>("SpawnTruck_02"), 	  				0 );
		m_gameScriptList.AddItem( "Spawn Van",				new Param1<string>("SpawnVan_01"), 	  					0 );
		m_gameScriptList.AddItem( "Spawn Merlin",			new Param1<string>("DefaultExpansionMerlin"),			0 );
		m_gameScriptList.AddItem( "Spawn Mh6",				new Param1<string>("DefaultExpansionMh6"),				0 );
		m_gameScriptList.AddItem( "Spawn Uh1h",				new Param1<string>("DefaultExpansionUh1h"),				0 );
		m_gameScriptList.AddItem( "Spawn UAZ",				new Param1<string>("DefaultExpansionUAZ"),				0 );
		m_gameScriptList.AddItem( "Spawn UAZRoofless",		new Param1<string>("DefaultExpansionUAZRoofless"),		0 );
		m_gameScriptList.AddItem( "Spawn UAZCargoRoofless",	new Param1<string>("DefaultExpansionUAZCargoRoofless"),	0 );
		m_gameScriptList.AddItem( "Spawn UAZCargo",			new Param1<string>("DefaultExpansionUAZCargo"),			0 );
		m_gameScriptList.AddItem( "Spawn Vodnik",			new Param1<string>("DefaultExpansionVodnik"),			0 );
		m_gameScriptList.AddItem( "Spawn Bus",			 	new Param1<string>("DefaultExpansionBus"),				0 );
		m_gameScriptList.AddItem( "Spawn ZodiacBoat",		new Param1<string>("DefaultExpansionZodiacBoat"),		0 );
		m_gameScriptList.AddItem( "Spawn UtilityBoat",		new Param1<string>("DefaultExpansionUtilityBoat"),		0 );
		//! m_gameScriptList.AddItem( "Spawn AmphibiaBoat",	new Param1<string>("DefaultExpansionAmphibiaBoat"),		0 );
		m_gameScriptList.AddItem( "Spawn AN2",				new Param1<string>("DefaultExpansionAN2"),				0 );
		//! m_gameScriptList.AddItem( "Spawn Su34",			new Param1<string>("DefaultExpansionSu34"),				0 );
		m_gameScriptList.AddItem( "Spawn C130J",			new Param1<string>("DefaultExpansionC130J"),			0 );
		//! m_gameScriptList.AddItem( "Spawn Forklift",		new Param1<string>("DefaultExpansionForklift"),			0 );
		m_gameScriptList.AddItem( "Spawn Tractor",			new Param1<string>("DefaultExpansionTractor"),			0 );
		m_gameScriptList.AddItem( "Spawn Mi8",				new Param1<string>("DefaultExpansionMi8"),				0 );

		CheckBoxWidget checkBoxGodmode = CheckBoxWidget.Cast(GetGame().GetWorkspace().CreateWidgets( checkboxLayout, m_checkboxPanel ));
		checkBoxGodmode.SetName( "Godmode" );
		checkBoxGodmode.SetText( "Godmode" );

//		CheckBoxWidget checkBoxAiming = CheckBoxWidget.Cast(GetGame().GetWorkspace().CreateWidgets( checkboxLayout, m_checkboxPanel ));
//		checkBoxAiming.SetName("OldAiming");
//		checkBoxAiming.SetText("Old Aiming");
//
//		CheckBoxWidget checkBoxLaser = CheckBoxWidget.Cast(GetGame().GetWorkspace().CreateWidgets( checkboxLayout, m_checkboxPanel ));
//		checkBoxLaser.SetName("LaserPointer");
//		checkBoxLaser.SetText("Laser Pointer");

		checkBoxMap.Insert( checkBoxGodmode.GetName(), "ToggleGodMode" );
	}

	override void OnShow()
	{
		// Update checkbox checked
		UpdateCheckBoxes();
	}

	override void OnHide() 
	{
		
	}

	void UpdateCheckBoxes() 
	{
		foreach( string widgetName, string function : checkBoxMap ) 
		{
			Widget widget = m_checkboxPanel.FindWidget( widgetName );

			if ( widget.IsInherited( CheckBoxWidget )) 
			{
				CheckBoxWidget checkbox = CheckBoxWidget.Cast( widget );
				bool checked;
				GetGame().GameScript.CallFunction( this , function , checked, NULL );
				checkbox.SetChecked( checked );
			}
		}
	}

	void Update() 
	{
	}

	bool ToggleGodMode( CheckBoxWidget widget ) 
	{
		if ( widget ) 
		{
			m_comGodMode = !m_comGodMode;

			GetPlayer().SetAllowDamage( !m_comGodMode );
		}
		return m_comGodMode;
	}

	override bool OnClick( Widget w, int x, int y, int button )
	{

		string param;
		Param1<string> param1;

		if ( w == m_gameScriptButton ) 
		{
			int selectRow = m_gameScriptList.GetSelectedRow();

			if ( selectRow == -1 ) return false;

			m_gameScriptList.GetItemData( selectRow, 0, param1 );

			if ( param1 ) 
			{
				GetGame().GameScript.CallFunction( this , param1.param1 , NULL, 0 );
			}
		}

		if ( w.IsInherited( CheckBoxWidget ) ) 
		{
			param = checkBoxMap.Get( w.GetName() );

			if ( param ) 
			{
				GetGame().GameScript.CallFunction( this , param , NULL, CheckBoxWidget.Cast( w ) );
			}
		}

		return false;
	}

	static CarScript SpawnHatchback() 
	{
		TStringArray attArr = {
		"HatchbackWheel","HatchbackWheel","HatchbackWheel","HatchbackWheel",
		"CarBattery","CarRadiator","EngineBelt","SparkPlug","Expansion_HatchbackHood",
		"Expansion_HatchbackTrunk","Expansion_HatchbackDoors_Driver","Expansion_HatchbackDoors_CoDriver",
		};

		return SpawnVehicle( "OffroadHatchback", attArr );
	}

	static CarScript SpawnHatchback_02() 
	{
		TStringArray attArr = {
		"Hatchback_02_Wheel","Hatchback_02_Wheel","Hatchback_02_Wheel","Hatchback_02_Wheel",
		"CarBattery","CarRadiator","EngineBelt","SparkPlug","Expansion_Hatchback_02_Hood",
		"Expansion_Hatchback_02_Trunk","Expansion_Hatchback_02_Door_1_1",
		"Expansion_Hatchback_02_Door_1_2","Expansion_Hatchback_02_Door_2_1",
		"Expansion_Hatchback_02_Door_2_2",
		};

		return SpawnVehicle( "Hatchback_02", attArr );
	}

	static CarScript SpawnSedan() 
	{
		TStringArray attArr = {
		"CivSedanWheel","CivSedanWheel","CivSedanWheel","CivSedanWheel",
		"CarBattery", "CarRadiator","EngineBelt", "SparkPlug","Expansion_CivSedanHood",
		"Expansion_CivSedanTrunk", "Expansion_CivSedanDoors_Driver",
		"Expansion_CivSedanDoors_CoDriver", "Expansion_CivSedanDoors_BackLeft", 
		"Expansion_CivSedanDoors_BackRight",
		};

		return SpawnVehicle( "CivilianSedan", attArr );
	}

	static CarScript SpawnSedan_02() 
	{
		TStringArray attArr = {
		"Sedan_02_Wheel","Sedan_02_Wheel","Sedan_02_Wheel","Sedan_02_Wheel",
		"CarBattery","CarRadiator","EngineBelt","SparkPlug","Expansion_Sedan_02_Hood",
		"Expansion_Sedan_02_Trunk","Expansion_Sedan_02_Door_1_1",
		"Expansion_Sedan_02_Door_1_2","Expansion_Sedan_02_Door_2_1",
		"Expansion_Sedan_02_Door_2_2",
		};

		return SpawnVehicle( "Sedan_02", attArr );
	}

	static CarScript SpawnV3SCargo() 
	{
		TStringArray attArr = {
		"V3SWheel","V3SWheel", "V3SWheel","V3SWheel", "V3SWheelDouble",
		"V3SWheelDouble", "V3SWheelDouble","V3SWheelDouble",
		"TruckBattery","TruckRadiator","EngineBelt","GlowPlug","V3SHood",
		"V3SDoors_Driver_Orange","V3SDoors_CoDriver_Orange",
		};

		return SpawnVehicle( "V3S_Cargo_Blue", attArr );
	}

	static CarScript SpawnTruck_01_Chassis() 
	{
		TStringArray attArr = {
		"Truck_01_Wheel","Truck_01_Wheel", "Truck_01_Wheel","Truck_01_Wheel",
		"Truck_01_WheelDouble","Truck_01_WheelDouble", "Truck_01_WheelDouble",
		"Truck_01_WheelDouble","TruckBattery","TruckRadiator","EngineBelt",
		"GlowPlug","Truck_01_Hood_Orange","Truck_01_Door_1_1_Orange",
		"Truck_01_Door_2_1_Orange",
		};

		return SpawnVehicle( "Truck_01_Chassis_Orange", attArr );
	}

	static CarScript SpawnTruck_01_Cargo() 
	{
		TStringArray attArr = {
		"Truck_01_Wheel","Truck_01_Wheel", "Truck_01_Wheel","Truck_01_Wheel",
		"Truck_01_WheelDouble","Truck_01_WheelDouble", "Truck_01_WheelDouble",
		"Truck_01_WheelDouble","TruckBattery","TruckRadiator","EngineBelt",
		"GlowPlug","Truck_01_Hood_Orange","Truck_01_Door_1_1_Orange",
		"Truck_01_Door_2_1_Orange",
		};

		return SpawnVehicle( "Truck_01_Cargo_Blue", attArr );
	}

	static CarScript SpawnTruck_02() 
	{
		TStringArray attArr = {
		"Truck_02_Wheel","Truck_02_Wheel", "Truck_02_Wheel","Truck_02_Wheel",
		"TruckBattery","TruckRadiator","EngineBelt","GlowPlug",
		"Truck_02_Door_1_1","Truck_02_Door_2_1",
		};

		return SpawnVehicle( "Truck_02", attArr );
	}

	static CarScript DefaultExpansionMerlin()
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

		return SpawnVehicle( "ExpansionMerlin", attArr );
	}

	static CarScript DefaultExpansionMi8()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("ExpansionHelicopterBattery");
		attArr.Insert("ExpansionHydraulicHoses");
		attArr.Insert("ExpansionIgniterPlug");
		attArr.Insert("HeadlightH7");
		attArr.Insert("HeadlightH7");

		attArr.Insert("ExpansionMi8FrontWheel");
		attArr.Insert("ExpansionMi8FrontWheel");
		attArr.Insert("ExpansionMi8BackWheel");
		attArr.Insert("ExpansionMi8BackWheel");

		return SpawnVehicle( "ExpansionMi8", attArr );

	}

	static CarScript DefaultExpansionMh6()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("ExpansionHelicopterBattery");
		attArr.Insert("ExpansionHydraulicHoses");
		attArr.Insert("ExpansionIgniterPlug");
		attArr.Insert("HeadlightH7");

		attArr.Insert("Expansion_Mh6_Door_1_1");
		attArr.Insert("Expansion_Mh6_Door_2_1");
		attArr.Insert("Expansion_Mh6_Door_1_2");
		attArr.Insert("Expansion_Mh6_Door_2_2");

		return SpawnVehicle( "ExpansionMh6", attArr );
	}

	static CarScript DefaultExpansionUh1h()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("ExpansionHelicopterBattery");
		attArr.Insert("ExpansionHydraulicHoses");
		attArr.Insert("ExpansionIgniterPlug");
		attArr.Insert("HeadlightH7");

		//attArr.Insert("ExpansionUh1hDoor_1_1");
		//attArr.Insert("ExpansionUh1hDoor_1_2");
		return SpawnVehicle( "ExpansionUh1h", attArr );
	}

	static CarScript DefaultExpansionQuad()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("ExpansionHelicopterBattery");
		attArr.Insert("CarRadiator");
		attArr.Insert("GlowPlug");

		return SpawnVehicle( "ExpansionQuad", attArr );
	}

	static CarScript DefaultExpansionUAZ()
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

		return SpawnVehicle( "ExpansionUAZ", attArr );
	}

	static CarScript DefaultExpansionUAZRoofless()
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

		return SpawnVehicle( "ExpansionUAZRoofless", attArr );
	}

	static CarScript DefaultExpansionUAZCargoRoofless()
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

		return SpawnVehicle( "ExpansionUAZCargoRoofless", attArr );
	}

	static CarScript DefaultExpansionUAZCargo()
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

		return SpawnVehicle( "ExpansionUAZCargo", attArr );
	}

	/*
	static CarScript DefaultExpansionForklift()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("ExpansionForkliftWheel");
		attArr.Insert("ExpansionForkliftWheel");
		attArr.Insert("ExpansionForkliftWheel");
		attArr.Insert("ExpansionForkliftWheel");
		attArr.Insert("HeadlightH7");
		attArr.Insert("HeadlightH7");

		attArr.Insert("CarBattery");
		attArr.Insert("CarRadiator");
		attArr.Insert("SparkPlug");

		return SpawnVehicle( "ExpansionForklift", attArr );
	}
	*/

	static CarScript DefaultExpansionTractor()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("ExpansionTractorFrontWheel");
		attArr.Insert("ExpansionTractorFrontWheel");
		attArr.Insert("ExpansionTractorBackWheel");
		attArr.Insert("ExpansionTractorBackWheel");
		attArr.Insert("ExpansionTractorDoorsDriver");
		attArr.Insert("ExpansionTractorDoorsCodriver");
		attArr.Insert("HeadlightH7");
		attArr.Insert("HeadlightH7");

		attArr.Insert("CarBattery");
		attArr.Insert("CarRadiator");
		attArr.Insert("SparkPlug");

		return SpawnVehicle( "ExpansionTractor", attArr );
	}

	static CarScript DefaultExpansionVodnik()
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

		attArr.Insert("CarBattery");
		attArr.Insert("CarRadiator");
		attArr.Insert("SparkPlug");

		return SpawnVehicle( "ExpansionVodnik", attArr );
	}

	static CarScript DefaultExpansionBus( vector position = "0 0 0" )
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

		return SpawnVehicle( "ExpansionBus", attArr, position );
	}

	static CarScript DefaultExpansionZodiacBoat()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("CarRadiator");
		attArr.Insert("GlowPlug");
		attArr.Insert("TruckBattery");
		attArr.Insert("HeadlightH7");
		attArr.Insert("HeadlightH7");

		return SpawnVehicle( "ExpansionZodiacBoat", attArr );
	}

	static CarScript DefaultExpansionUtilityBoat()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("CarRadiator");
		attArr.Insert("GlowPlug");
		attArr.Insert("TruckBattery");
		attArr.Insert("HeadlightH7");
		attArr.Insert("HeadlightH7");

		return SpawnVehicle( "ExpansionUtilityBoat", attArr );
	}

	/*
	static CarScript DefaultExpansionAmphibiaBoat()
	{
		array< string> attArr = new array< string>;

		attArr.Insert("CarRadiator");
		attArr.Insert("GlowPlug");
		attArr.Insert("TruckBattery");
		attArr.Insert("HeadlightH7");
		attArr.Insert("HeadlightH7");
		attArr.Insert("ExpansionUtilityBoatWheel");
		attArr.Insert("ExpansionUtilityBoatWheel");
		attArr.Insert("ExpansionUtilityBoatWheel");
		attArr.Insert("ExpansionUtilityBoatWheel");

		return SpawnVehicle( "ExpansionAmphibiaBoat", attArr );
	}
	*/

	/*
	static CarScript DefaultExpansionSu34()
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

		return SpawnVehicle( "ExpansionSu34", attArr );
	}
	*/

	static CarScript DefaultExpansionAN2()
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

		return SpawnVehicle( "ExpansionAn2", attArr );
	}

	static CarScript DefaultExpansionC130J()
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

		return SpawnVehicle( "ExpansionC130J", attArr );
	}

	static CarScript SpawnBus() 
	{
		TStringArray attArr = {
		"TransitBusWheel","TransitBusWheel", "TransitBusWheelDouble","TransitBusWheelDouble",
		"TruckBattery","TruckRadiator","EngineBelt","GlowPlug","BusHood",
		"BusDoors_Left","BusDoors_Right", "BusDoors_Left","BusDoors_Right", "BusDoors_Left","BusDoors_Right",
		};

		return SpawnVehicle( "TransitBus", attArr );
	}

	static CarScript SpawnVan_01() 
	{
		TStringArray attArr = {
			"Van_01_Wheel","Van_01_Wheel","Van_01_Wheel","Van_01_Wheel",
			"CarBattery","CarRadiator","EngineBelt","SparkPlug",
			"Van_01_Door_1_1","Van_01_Door_2_1","Van_01_Door_2_2",
			"Van_01_Trunk_1", "Van_01_Trunk_2",
		};

		return SpawnVehicle( "Van_01", attArr );
	}

	static CarScript SpawnVehicle( string vehicle, TStringArray attachments, vector position = "0 0 0" ) 
	{
		if ( position == "0 0 0" )
			position = GetCursorPos();
		com_lastSpawnedCar = CarScript.Cast( GetGame().CreateObject( vehicle, position, false, false ) );

		for (int j = 0; j < attachments.Count(); j++) { com_lastSpawnedCar.GetInventory().CreateAttachment( attachments.Get(j) ); }

		com_lastSpawnedCar.Fill( CarFluid.FUEL, 1000 );
		com_lastSpawnedCar.Fill( CarFluid.OIL, 1000 );
		com_lastSpawnedCar.Fill( CarFluid.BRAKE, 1000 );
		com_lastSpawnedCar.Fill( CarFluid.COOLANT, 1000 );

		//com_lastSpawnedCar.EngineStart();
		//com_lastSpawnedCar.ToggleHeadlights();

		return com_lastSpawnedCar;
	}
}