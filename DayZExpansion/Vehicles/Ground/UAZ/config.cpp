class CfgPatches
{
	class DayZExpansion_Vehicles_Ground_UAZ
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DayZExpansion_Vehicles_Data"
		};
	};
};
class CfgSlots
{
	class Slot_uazwheel_1_1
	{
		name="uazwheel_1_1";
		displayName="$STR_EXPANSION_VEHICLE_UAZ_WHEEL";
		descriptionShort="$STR_EXPANSION_VEHICLE_UAZ_WHEEL_DESC";
		selection="wheel_1_1";
		ghostIcon="wheel";
	};
	class Slot_uazwheel_1_2
	{
		name="uazwheel_1_2";
		displayName="$STR_EXPANSION_VEHICLE_UAZ_WHEEL";
		descriptionShort="$STR_EXPANSION_VEHICLE_UAZ_WHEEL_DESC";
		selection="wheel_1_2";
		ghostIcon="wheel";
	};
	class Slot_uazwheel_2_1
	{
		name="uazwheel_2_1";
		displayName="$STR_EXPANSION_VEHICLE_UAZ_WHEEL";
		descriptionShort="$STR_EXPANSION_VEHICLE_UAZ_WHEEL_DESC";
		selection="wheel_2_1";
		ghostIcon="wheel";
	};
	class Slot_uazwheel_2_2
	{
		name="uazwheel_2_2";
		displayName="$STR_EXPANSION_VEHICLE_UAZ_WHEEL";
		descriptionShort="$STR_EXPANSION_VEHICLE_UAZ_WHEEL_DESC";
		selection="wheel_2_2";
		ghostIcon="wheel";
	};
	class Slot_uazdriverdoor
	{
		name="uazdriverdoor";
		displayName="$STR_EXPANSION_VEHICLE_UAZ_DOOR_FRONT_LEFT";
		descriptionShort="";
		selection="uazdriverdoor";
		ghostIcon="doorfront";
	};
	class Slot_uazcodriverdoor
	{
		name="uazcodriverdoor";
		displayName="$STR_EXPANSION_VEHICLE_UAZ_DOOR_FRONT_RIGHT";
		descriptionShort="";
		selection="uazcodriverdoor";
		ghostIcon="doorfront";
	};
	class Slot_uazcargo1door
	{
		name="uazcargo1door";
		displayName="$STR_EXPANSION_VEHICLE_UAZ_DOOR_BACK_LEFT";
		descriptionShort="DESCRIPTION";
		selection="uazcargo1door";
		ghostIcon="doorrear";
	};
	class Slot_uazcargo2door
	{
		name="uazcargo2door";
		displayName="$STR_EXPANSION_VEHICLE_UAZ_DOOR_BACK_RIGHT";
		descriptionShort="";
		selection="uazcargo2door";
		ghostIcon="doorrear";
	};
	class Slot_uazhooddoor
	{
		name="uazhooddoor";
		displayName="$STR_EXPANSION_VEHICLE_UAZ_DOOR_HOOD";
		descriptionShort="";
		selection="uazhooddoor";
		ghostIcon="hood";
	};
	class Slot_uaztrunkdoor
	{
		name="uaztrunkdoor";
		displayName="$STR_EXPANSION_VEHICLE_UAZ_DOOR_TRUNK";
		descriptionShort="";
		selection="uaztrunkdoor";
		ghostIcon="trunk";
	};
};
class CfgVehicles
{
	class CarScript;
	class HatchbackWheel;
	class ExpansionWheelBase;
	class CarWheel;
	class CarDoor;
	class Inventory_Base;
	class SimulationModule;
	class Axles;
	class Front;
	class Wheels;
	class Rear;
	class Left;
	class Right;
	class AnimationSources;
	class Crew;
	class Driver;
	class CoDriver;
	class ExpansionVehicleCarBase;
	class HouseNoDestruct;
	class ExpansionStaticWreckUAZ: HouseNoDestruct
	{
		scope=2;
		model="\DayZExpansion\Vehicles\Ground\UAZ\UAZ_Wreck_Roof.p3d";
		class AnimationSources: AnimationSources
		{
			class uazdriverdoor
			{
				source="user";
				initPhase=0;
				animPeriod=0.5;
			};
			class uazcodriverdoor
			{
				source="user";
				initPhase=0.2;
				animPeriod=0.5;
			};
			class uazcargo1door
			{
				source="user";
				initPhase=0.40000001;
				animPeriod=0.5;
			};
			class uazcargo2door
			{
				source="user";
				initPhase=0.33000001;
				animPeriod=0.5;
			};
			class uaztrunkdoor
			{
				source="user";
				initPhase=1;
				animPeriod=0.5;
			};
			class uazhooddoor
			{
				source="user";
				initPhase=0.68000001;
				animPeriod=0.5;
			};
			class steering_hub_1_1
			{
				source="user";
				initPhase=0.2;
				animPeriod=0.5;
			};
			class steering_hub_1_2
			{
				source="user";
				initPhase=0.2;
				animPeriod=0.5;
			};
		};
	};
	class ExpansionStaticWreckUAZRoofless: ExpansionStaticWreckUAZ
	{
		model="\DayZExpansion\Vehicles\Ground\UAZ\UAZ_Wreck_Roofless.p3d";
		class AnimationSources: AnimationSources
		{
			class backseats
			{
				source="user";
				animPeriod=0.0099999998;
				initPhase=1;
			};
			class uazdriverdoor
			{
				source="user";
				initPhase=1;
				animPeriod=0.5;
			};
			class uazcodriverdoor
			{
				source="user";
				initPhase=1;
				animPeriod=0.5;
			};
			class uazcargo1door
			{
				source="user";
				initPhase=1;
				animPeriod=0.5;
			};
			class uazcargo2door
			{
				source="user";
				initPhase=1;
				animPeriod=0.5;
			};
			class uaztrunkdoor
			{
				source="user";
				initPhase=1;
				animPeriod=0.5;
			};
			class uazhooddoor
			{
				source="user";
				initPhase=1;
				animPeriod=0.5;
			};
			class steering_hub_1_1
			{
				source="user";
				initPhase=0.40000001;
				animPeriod=0.5;
			};
			class steering_hub_1_2
			{
				source="user";
				initPhase=0.40000001;
				animPeriod=0.5;
			};
		};
	};
	class ExpansionUAZWheel: CarWheel
	{
		scope=2;
		displayName="$STR_EXPANSION_VEHICLE_UAZ_WHEEL";
		descriptionShort="$STR_EXPANSION_VEHICLE_UAZ_WHEEL_DESC";
		model="\DayZExpansion\Vehicles\Ground\UAZ\proxy\UAZWheel.p3d";
		weight=15000;
		inventorySlot[]=
		{
			"uazwheel_1_1",
			"uazwheel_1_2",
			"uazwheel_2_1",
			"uazwheel_2_2"
		};
		rotationFlags=8;
		physLayer="item_large";
		radiusByDamage[]={0,0.34299999,0.30000001,0.40000001,0.99980003,0.25,0.99989998,0.2};
		radius=0.384;
		width=0.214;
		tyreRollResistance=0.015;
		tyreTread=0.69999999;
		tyreOffroadResistance=0.25;
		tyreGrip=1;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=200;
					healthLevels[]=
					{
						
						{
							1,
							
							{
								"dayzexpansion\vehicles\ground\uaz\data\uaz_exterior_01.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"dayzexpansion\vehicles\ground\uaz\data\uaz_exterior_01.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_exterior_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_exterior_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_exterior_damage.rvmat"
							}
						}
					};
				};
			};
		};
	};
	class ExpansionUAZWheel_Ruined: ExpansionUAZWheel
	{
		model="\DayZExpansion\Vehicles\Ground\UAZ\proxy\UAZWheel_Destroyed.p3d";
		radiusByDamage[]={0.2};
		radius=0.2;
		width=0.31600001;
		tyreRollResistance=0.015;
		tyreTread=0.5;
	};
	class ExpansionUAZDoorDriver: CarDoor
	{
		scope=2;
		displayName="$STR_EXPANSION_VEHICLE_UAZ_DOOR_FRONT_LEFT";
		descriptionShort="$STR_EXPANSION_VEHICLE_UAZ_DOOR_FRONT_LEFT_DESC";
		model="\DayZExpansion\Vehicles\Ground\UAZ\proxy\uazdriverdoor.p3d";
		hiddenSelections[]=
		{
			"camo",
			"dmgZone_doors"
		};
		hiddenSelectionsTextures[]=
		{
			"",
			""
		};
		hiddenSelectionsMaterials[]=
		{
			"DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_exterior_01.rvmat",
			"DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_exterior_01.rvmat"
		};
		weight=15000;
		defaultSkin="Green";
		itemSize[]={10,10};
		itemBehaviour=0;
		inventorySlot="uazdriverdoor";
		rotationFlags=8;
		physLayer="item_large";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=2000;
					healthLevels[]=
					{
						
						{
							1,
							{}
						},
						
						{
							0.69999999,
							{}
						},
						
						{
							0.5,
							{}
						},
						
						{
							0.30000001,
							{}
						},
						
						{
							0,
							{}
						}
					};
				};
			};
			class DamageZones
			{
				class Window
				{
					class Health
					{
						hitpoints=500;
						healthLevels[]=
						{
							
							{
								1,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\hatchback_02_windows.rvmat"
								}
							},
							
							{
								0.69999999,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\hatchback_02_windows.rvmat"
								}
							},
							
							{
								0.5,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\glass_i_damage.rvmat"
								}
							},
							
							{
								0.30000001,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\glass_i_damage.rvmat"
								}
							},
							
							{
								0,
								"hidden"
							}
						};
					};
					fatalInjuryCoef=-1;
					inventorySlots[]={};
					transferToGlobalCoef=0;
					componentNames[]=
					{
						"dmgZone_doorwindowLeft"
					};
				};
				class Doors
				{
					class Health
					{
						hitpoints=2000;
						transferToGlobalCoef=1;
						healthLevels[]=
						{
							
							{
								1,
								{}
							},
							
							{
								0.69999999,
								{}
							},
							
							{
								0.5,
								{}
							},
							
							{
								0.30000001,
								{}
							},
							
							{
								0,
								{}
							}
						};
					};
					fatalInjuryCoef=-1;
					inventorySlots[]={};
					componentNames[]=
					{
						"dmgZone_doors"
					};
				};
			};
		};
	};
	class ExpansionUAZDoorCoDriver: CarDoor
	{
		scope=2;
		displayName="$STR_EXPANSION_VEHICLE_UAZ_DOOR_FRONT_RIGHT";
		descriptionShort="$STR_EXPANSION_VEHICLE_UAZ_DOOR_FRONT_RIGHT_DESC";
		model="\DayZExpansion\Vehicles\Ground\UAZ\proxy\uazcodriverdoor.p3d";
		hiddenSelections[]=
		{
			"camo",
			"dmgZone_doors"
		};
		hiddenSelectionsTextures[]=
		{
			"",
			""
		};
		hiddenSelectionsMaterials[]=
		{
			"DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_exterior_01.rvmat",
			"DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_exterior_01.rvmat"
		};
		weight=15000;
		defaultSkin="Green";
		itemSize[]={10,10};
		inventorySlot="uazcodriverdoor";
		rotationFlags=4;
		itemBehaviour=0;
		physLayer="item_large";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=2000;
					healthLevels[]=
					{
						
						{
							1,
							{}
						},
						
						{
							0.69999999,
							{}
						},
						
						{
							0.5,
							{}
						},
						
						{
							0.30000001,
							{}
						},
						
						{
							0,
							{}
						}
					};
				};
			};
			class DamageZones
			{
				class Window
				{
					class Health
					{
						hitpoints=500;
						healthLevels[]=
						{
							
							{
								1,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\hatchback_02_windows.rvmat"
								}
							},
							
							{
								0.69999999,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\hatchback_02_windows.rvmat"
								}
							},
							
							{
								0.5,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\glass_i_damage.rvmat"
								}
							},
							
							{
								0.30000001,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\glass_i_damage.rvmat"
								}
							},
							
							{
								0,
								"hidden"
							}
						};
					};
					fatalInjuryCoef=-1;
					inventorySlots[]={};
					transferToGlobalCoef=0;
					componentNames[]=
					{
						"dmgZone_doorwindowRight"
					};
				};
				class Doors
				{
					class Health
					{
						hitpoints=2000;
						transferToGlobalCoef=1;
						healthLevels[]=
						{
							
							{
								1,
								{}
							},
							
							{
								0.69999999,
								{}
							},
							
							{
								0.5,
								{}
							},
							
							{
								0.30000001,
								{}
							},
							
							{
								0,
								{}
							}
						};
					};
					fatalInjuryCoef=-1;
					inventorySlots[]={};
					componentNames[]=
					{
						"dmgZone_doors"
					};
				};
			};
		};
	};
	class ExpansionUAZDoorCargo1: CarDoor
	{
		scope=2;
		displayName="$STR_EXPANSION_VEHICLE_UAZ_DOOR_BACK_LEFT";
		descriptionShort="$STR_EXPANSION_VEHICLE_UAZ_DOOR_BACK_LEFT_DESC";
		model="\DayZExpansion\Vehicles\Ground\UAZ\proxy\uazcargo1door.p3d";
		hiddenSelections[]=
		{
			"camo",
			"dmgZone_doors"
		};
		hiddenSelectionsTextures[]=
		{
			"",
			""
		};
		hiddenSelectionsMaterials[]=
		{
			"DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_exterior_01.rvmat",
			"DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_exterior_01.rvmat"
		};
		weight=15000;
		defaultSkin="Green";
		itemSize[]={10,10};
		inventorySlot="uazcargo1door";
		rotationFlags=8;
		itemBehaviour=0;
		physLayer="item_large";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=2000;
					healthLevels[]=
					{
						
						{
							1,
							{}
						},
						
						{
							0.69999999,
							{}
						},
						
						{
							0.5,
							{}
						},
						
						{
							0.30000001,
							{}
						},
						
						{
							0,
							{}
						}
					};
				};
			};
			class DamageZones
			{
				class Window
				{
					class Health
					{
						hitpoints=500;
						healthLevels[]=
						{
							
							{
								1,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\hatchback_02_windows.rvmat"
								}
							},
							
							{
								0.69999999,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\hatchback_02_windows.rvmat"
								}
							},
							
							{
								0.5,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\glass_i_damage.rvmat"
								}
							},
							
							{
								0.30000001,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\glass_i_damage.rvmat"
								}
							},
							
							{
								0,
								"hidden"
							}
						};
					};
					fatalInjuryCoef=-1;
					inventorySlots[]={};
					transferToGlobalCoef=0;
					componentNames[]=
					{
						"dmgZone_doorwindowRLeft"
					};
				};
				class Doors
				{
					class Health
					{
						hitpoints=2000;
						transferToGlobalCoef=1;
						healthLevels[]=
						{
							
							{
								1,
								{}
							},
							
							{
								0.69999999,
								{}
							},
							
							{
								0.5,
								{}
							},
							
							{
								0.30000001,
								{}
							},
							
							{
								0,
								{}
							}
						};
					};
					fatalInjuryCoef=-1;
					inventorySlots[]={};
					componentNames[]=
					{
						"dmgZone_doors"
					};
				};
			};
		};
	};
	class ExpansionUAZDoorCargo2: CarDoor
	{
		scope=2;
		displayName="$STR_EXPANSION_VEHICLE_UAZ_DOOR_BACK_RIGHT";
		descriptionShort="$STR_EXPANSION_VEHICLE_UAZ_DOOR_BACK_RIGHT_DESC";
		model="\DayZExpansion\Vehicles\Ground\UAZ\proxy\uazcargo2door.p3d";
		hiddenSelections[]=
		{
			"camo",
			"dmgZone_doors"
		};
		hiddenSelectionsTextures[]=
		{
			"",
			""
		};
		hiddenSelectionsMaterials[]=
		{
			"DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_exterior_01.rvmat",
			"DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_exterior_01.rvmat"
		};
		weight=15000;
		defaultSkin="Green";
		itemSize[]={10,10};
		inventorySlot="uazcargo2door";
		rotationFlags=8;
		itemBehaviour=0;
		physLayer="item_large";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=2000;
					healthLevels[]=
					{
						
						{
							1,
							{}
						},
						
						{
							0.69999999,
							{}
						},
						
						{
							0.5,
							{}
						},
						
						{
							0.30000001,
							{}
						},
						
						{
							0,
							{}
						}
					};
				};
			};
			class DamageZones
			{
				class Window
				{
					class Health
					{
						hitpoints=500;
						healthLevels[]=
						{
							
							{
								1,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\hatchback_02_windows.rvmat"
								}
							},
							
							{
								0.69999999,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\hatchback_02_windows.rvmat"
								}
							},
							
							{
								0.5,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\glass_i_damage.rvmat"
								}
							},
							
							{
								0.30000001,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\glass_i_damage.rvmat"
								}
							},
							
							{
								0,
								"hidden"
							}
						};
					};
					fatalInjuryCoef=-1;
					inventorySlots[]={};
					transferToGlobalCoef=0;
					componentNames[]=
					{
						"dmgZone_doorwindowRRight"
					};
				};
				class Doors
				{
					class Health
					{
						hitpoints=2000;
						transferToGlobalCoef=1;
						healthLevels[]=
						{
							
							{
								1,
								{}
							},
							
							{
								0.69999999,
								{}
							},
							
							{
								0.5,
								{}
							},
							
							{
								0.30000001,
								{}
							},
							
							{
								0,
								{}
							}
						};
					};
					fatalInjuryCoef=-1;
					inventorySlots[]={};
					componentNames[]=
					{
						"dmgZone_doors"
					};
				};
			};
		};
	};
	class ExpansionUAZDoorTrunk: CarDoor
	{
		scope=2;
		displayName="$STR_EXPANSION_VEHICLE_UAZ_DOOR_TRUNK";
		descriptionShort="$STR_EXPANSION_VEHICLE_UAZ_DOOR_TRUNK_DESC";
		model="\DayZExpansion\Vehicles\Ground\UAZ\proxy\uazTrunkdoor.p3d";
		hiddenSelections[]=
		{
			"camo"
		};
		hiddenSelectionsTextures[]=
		{
			"DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_out_01_co.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_exterior_01.rvmat"
		};
		inventorySlot="uaztrunkdoor";
		rotationFlags=4;
		itemBehaviour=0;
	};
	class ExpansionUAZDoorHood: CarDoor
	{
		scope=2;
		displayName="$STR_EXPANSION_VEHICLE_UAZ_DOOR_HOOD";
		descriptionShort="$STR_EXPANSION_VEHICLE_UAZ_DOOR_HOOD_DESC";
		model="\DayZExpansion\Vehicles\Ground\UAZ\proxy\uazHooddoor.p3d";
		hiddenSelections[]=
		{
			"camo",
			"dmgZone_doors"
		};
		hiddenSelectionsTextures[]=
		{
			"",
			""
		};
		hiddenSelectionsMaterials[]=
		{
			"DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_exterior_01.rvmat",
			"DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_exterior_01.rvmat"
		};
		weight=15000;
		itemSize[]={10,10};
		defaultSkin="Green";
		inventorySlot="uazhooddoor";
		rotationFlags=2;
		itemBehaviour=0;
		physLayer="item_large";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=2000;
					healthLevels[]=
					{
						
						{
							1,
							{}
						},
						
						{
							0.69999999,
							{}
						},
						
						{
							0.5,
							{}
						},
						
						{
							0.30000001,
							{}
						},
						
						{
							0,
							{}
						}
					};
				};
			};
			class DamageZones
			{
				class Doors
				{
					class Health
					{
						hitpoints=2000;
						transferToGlobalCoef=1;
						healthLevels[]=
						{
							
							{
								1,
								{}
							},
							
							{
								0.69999999,
								{}
							},
							
							{
								0.5,
								{}
							},
							
							{
								0.30000001,
								{}
							},
							
							{
								0,
								{}
							}
						};
					};
					fatalInjuryCoef=-1;
					inventorySlots[]={};
					componentNames[]=
					{
						"dmgZone_doors"
					};
				};
			};
		};
	};
	class ExpansionUAZBase: CarScript
	{
		scope=0;
		displayName="";
		model="";
		vehicleClass="Expansion_Car";
		attachments[]=
		{
			"CarBattery",
			"Reflector_1_1",
			"Reflector_2_1",
			"CarRadiator",
			"SparkPlug",
			"uazwheel_1_1",
			"uazwheel_1_2",
			"uazwheel_2_1",
			"uazwheel_2_2",
			"uazdriverdoor",
			"uazcodriverdoor",
			"uazcargo1door",
			"uazcargo2door",
			"uazhooddoor",
			"CamoNet",
			"KeyChain"
		};
		doors[]=
		{
			"uazdriverdoor",
			"uazcodriverdoor",
			"uazcargo1door",
			"uazcargo2door",
			"uazhooddoor"
		};
		defaultSkin="Green";
		fuelCapacity=40;
		fuelConsumption=10;
		class Crew
		{
			class Driver: Driver
			{
				actionSel="seat_driver";
				proxyPos="crewDriver";
				getInPos="pos_driver";
				getInDir="pos_driver_dir";
				isDriver=1;
			};
			class CoDriver: CoDriver
			{
				actionSel="seat_codriver";
				proxyPos="crewCoDriver";
				getInPos="pos_codriver";
				getInDir="pos_codriver_dir";
			};
			class Cargo1
			{
				actionSel="seat_cargo1";
				proxyPos="crewCargo1";
				getInPos="pos_cargo1";
				getInDir="pos_cargo1_dir";
			};
			class Cargo2
			{
				actionSel="seat_cargo2";
				proxyPos="crewCargo2";
				getInPos="pos_cargo2";
				getInDir="pos_cargo2_dir";
			};
		};
		class SimulationModule: SimulationModule
		{
			class Steering
			{
				maxSteeringAngle=30;
				increaseSpeed[]={0,45,60,23,100,12};
				decreaseSpeed[]={0,80,60,40,90,20};
				centeringSpeed[]={0,0,15,25,60,40,100,60};
			};
			class Throttle
			{
				reactionTime=1.25;
				defaultThrust=0.85000002;
				gentleThrust=0.69999999;
				turboCoef=5;
				gentleCoef=0.75;
			};
			class Brake
			{
				pressureBySpeed[]={0,0.85000002,10,0.69999999,20,0.5,40,0.40000001,60,0.43000001,80,0.46000001,100,0.51999998,120,0.69999999};
				reactionTime=0.30000001;
				driverless=0.1;
			};
			class Aerodynamics
			{
				frontalArea=2.1800001;
				dragCoefficient=0.56;
			};
			drive="DRIVE_AWD";
			class Engine
			{
				torqueCurve[]={650,0,750,59,1400,117,2250,167,4500,101,6000,0};
				inertia=0.11;
				frictionTorque=100;
				rollingFriction=0.5;
				viscousFriction=0.5;
				rpmIdle=800;
				rpmMin=900;
				rpmClutch=1400;
				rpmRedline=5000;
			};
			class Clutch
			{
				maxTorqueTransfer=330;
				uncoupleTime=0.2;
				coupleTime=0.34999999;
			};
			class Gearbox
			{
				type="GEARBOX_MANUAL";
				reverse=3.526;
				ratios[]={3.6670001,2.0999999,1.3609999,1};
			};
			class CentralDifferential
			{
				ratio=1.45;
				type="DIFFERENTIAL_OPEN";
			};
			class Axles: Axles
			{
				class Front: Front
				{
					maxBrakeTorque=4500;
					maxHandbrakeTorque=5000;
					wheelHubMass=5;
					wheelHubRadius=0.15000001;
					class Differential
					{
						ratio=4.0999999;
						type="DIFFERENTIAL_OPEN";
					};
					class Suspension
					{
						stiffness=40000;
						compression=2100;
						damping=7500;
						travelMaxUp=0.088200003;
						travelMaxDown=0.083300002;
					};
					class Wheels: Wheels
					{
						class Left: Left
						{
							animTurn="turnfrontleft";
							animRotation="wheelfrontleft";
							wheelHub="wheel_1_1_damper_land";
							animDamper="damper_1_1";
							inventorySlot="uazwheel_1_1";
						};
						class Right: Right
						{
							animTurn="turnfrontright";
							animRotation="wheelfrontright";
							wheelHub="wheel_2_1_damper_land";
							animDamper="damper_2_1";
							inventorySlot="uazwheel_2_1";
						};
					};
				};
				class Rear: Rear
				{
					maxBrakeTorque=4200;
					maxHandbrakeTorque=4700;
					wheelHubMass=5;
					wheelHubRadius=0.15000001;
					class Differential
					{
						ratio=4.0999999;
						type="DIFFERENTIAL_OPEN";
					};
					class Suspension
					{
						stiffness=40000;
						compression=2200;
						damping=7600;
						travelMaxUp=0.1587;
						travelMaxDown=0.1059;
					};
					class Wheels: Wheels
					{
						class Left: Left
						{
							animTurn="turnbackleft";
							animRotation="wheelbackleft";
							wheelHub="wheel_1_2_damper_land";
							animDamper="damper_1_2";
							inventorySlot="uazwheel_1_2";
						};
						class Right: Right
						{
							animTurn="turnbacktright";
							animRotation="wheelbackright";
							wheelHub="wheel_2_2_damper_land";
							animDamper="damper_2_2";
							inventorySlot="uazwheel_2_2";
						};
					};
				};
			};
		};
		class Cargo
		{
			itemsCargoSize[]={10,30};
			allowOwnedCargoManipulation=1;
			openable=1;
		};
		class AnimationSources: AnimationSources
		{
			class damper_1_1
			{
				source="user";
				initPhase=0.48570001;
				animPeriod=1;
			};
			class damper_2_1: damper_1_1
			{
			};
			class damper_1_2
			{
				source="user";
				initPhase=0.40020001;
				animPeriod=1;
			};
			class damper_2_2: damper_1_2
			{
			};
			class uazdriverdoor
			{
				source="user";
				initPhase=0;
				animPeriod=0.5;
			};
			class uazcodriverdoor: uazdriverdoor
			{
			};
			class uazcargo1door: uazdriverdoor
			{
			};
			class uazcargo2door: uazdriverdoor
			{
			};
			class uaztrunkdoor: uazdriverdoor
			{
			};
			class uazhooddoor: uazdriverdoor
			{
			};
		};
		class Sounds
		{
			thrust=0.60000002;
			thrustTurbo=1;
			thrustGentle=0.30000001;
			thrustSmoothCoef=0.02;
			camposSmoothCoef=0.029999999;
			soundSetsFilter[]=
			{
				"offroad_Engine_Offload_Ext_Rpm1_SoundSet",
				"offroad_Engine_Offload_Ext_Rpm2_SoundSet",
				"offroad_Engine_Offload_Ext_Rpm3_SoundSet",
				"offroad_Engine_Offload_Ext_Rpm4_SoundSet",
				"offroad_Engine_Offload_Ext_Rpm5_SoundSet",
				"offroad_Engine_Ext_Rpm0_SoundSet",
				"offroad_Engine_Ext_Rpm1_SoundSet",
				"offroad_Engine_Ext_Rpm2_SoundSet",
				"offroad_Engine_Ext_Rpm3_SoundSet",
				"offroad_Engine_Ext_Rpm4_SoundSet",
				"offroad_Engine_Ext_Rpm5_SoundSet",
				"offroad_Engine_Ext_Broken_SoundSet",
				"offroad_Tires_rock_slow_Ext_SoundSet",
				"offroad_Tires_rock_fast_Ext_SoundSet",
				"offroad_Tires_grass_slow_Ext_SoundSet",
				"offroad_Tires_grass_fast_Ext_SoundSet",
				"offroad_Tires_gravel_slow_Ext_SoundSet",
				"offroad_Tires_gravel_fast_Ext_SoundSet",
				"offroad_Tires_gravel_dust_fast_Ext_SoundSet",
				"offroad_Tires_asphalt_slow_Ext_SoundSet",
				"offroad_Tires_asphalt_fast_Ext_SoundSet",
				"offroad_Tires_water_slow_Ext_SoundSet",
				"offroad_Tires_water_fast_Ext_SoundSet",
				"Offroad_skid_dirt_SoundSet",
				"offroad_dirt_turn_SoundSet",
				"offroad_Rain_Ext_SoundSet",
				"offroad_damper_left_SoundSet",
				"offroad_damper_right_SoundSet"
			};
			soundSetsInt[]=
			{
				"Offroad_Tires_Asphalt_Slow_General_Int_SoundSet",
				"Offroad_Tires_Asphalt_Fast_General_Int_SoundSet",
				"Offroad_Wind_SoundSet"
			};
		};
		class ObstacleGenerator
		{
			carve=1;
			timeToStationary=9.9999997e-06;
			moveThreshold=9.9999997e-06;
			class Shapes
			{
				class Cylindric
				{
					class Cyl1
					{
						radius=1;
						height=1.5;
						center[]={0,0,0.69999999};
					};
					class Cyl3
					{
						radius=1;
						height=1.5;
						center[]={0,0,-0.69999999};
					};
				};
			};
		};
		class GUIInventoryAttachmentsProps
		{
			class Engine
			{
				name="$STR_attachment_Engine0";
				description="";
				icon="cat_vehicle_engine";
				attachmentSlots[]=
				{
					"CarBattery",
					"CarRadiator",
					"SparkPlug"
				};
			};
			class Body
			{
				name="$STR_attachment_Body0";
				description="";
				icon="cat_vehicle_body";
				attachmentSlots[]=
				{
					"Reflector_1_1",
					"Reflector_2_1",
					"uazdriverdoor",
					"uazcodriverdoor",
					"uazcargo1door",
					"uazcargo2door",
					"uazhooddoor",
					"CamoNet",
					"KeyChain"
				};
			};
			class Chassis
			{
				name="$STR_attachment_Chassis0";
				description="";
				icon="cat_vehicle_chassis";
				attachmentSlots[]=
				{
					"uazwheel_1_1",
					"uazwheel_1_2",
					"uazwheel_2_1",
					"uazwheel_2_2"
				};
			};
		};
	};
	class ExpansionUAZ: ExpansionUAZBase
	{
		scope=2;
		displayname="$STR_EXPANSION_VEHICLE_UAZ";
		model="\DayZExpansion\Vehicles\Ground\UAZ\UAZ.p3d";
		modelZeroPointDistanceFromGround=0;
		vehicleClass="Expansion_Car";
		defaultSkin="Green";
		hiddenSelections[]=
		{
			"light_1_1",
			"light_2_1",
			"light_brake_1_2",
			"light_brake_2_2",
			"light_reverse_1_2",
			"light_reverse_2_2",
			"light_1_2",
			"light_2_2",
			"light_dashboard",
			"camo1",
			"camo2",
			"dmgZone_front",
			"dmgZone_back",
			"dmgZone_roof",
			"dmgzone_fender_FL",
			"dmgzone_fender_BL",
			"dmgzone_fender_FR",
			"dmgzone_fender_BR"
		};
		hiddenSelectionsTextures[]=
		{
			"dz\vehicles\wheeled\civiliansedan\data\sedan_glass_ca.paa",
			"dz\vehicles\wheeled\civiliansedan\data\sedan_glass_ca.paa",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_out_01_co.paa",
			"DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_int_01_co.paa",
			"",
			"",
			"DZ\\gear\\camping\\data\\tent_pristine_co.paa",
			"",
			"",
			"",
			""
		};
		hiddenSelectionsMaterials[]=
		{
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_exterior_01.rvmat",
			"DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_interior_01.rvmat",
			"DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_exterior_01.rvmat",
			"DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_exterior_01.rvmat",
			"",
			"DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_exterior_01.rvmat",
			"DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_exterior_01.rvmat",
			"DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_exterior_01.rvmat",
			"DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_exterior_01.rvmat"
		};
		dashboardMatOn="dz\vehicles\wheeled\transitbus\data\ikarus_dashboarde.rvmat";
		dashboardMatOff="dz\vehicles\wheeled\transitbus\data\ikarus_dashboard.rvmat";
		frontReflectorMatOn="dz\vehicles\wheeled\civiliansedan\data\civiliansedan_lights.rvmat";
		frontReflectorMatOff="dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat";
		brakeReflectorMatOn="dz\vehicles\wheeled\civiliansedan\data\Sedan_TailLights_e.rvmat";
		brakeReflectorMatOff="dz\vehicles\wheeled\civiliansedan\data\Sedan_TailLights.rvmat";
		ReverseReflectorMatOn="dz\vehicles\wheeled\civiliansedan\data\Sedan_TailLights_e.rvmat";
		ReverseReflectorMatOff="dz\vehicles\wheeled\civiliansedan\data\Sedan_TailLights.rvmat";
		TailReflectorMatOn="dz\vehicles\wheeled\civiliansedan\data\Sedan_TailLights_e.rvmat";
		TailReflectorMatOff="dz\vehicles\wheeled\civiliansedan\data\Sedan_TailLights.rvmat";
		attachments[]=
		{
			"CarBattery",
			"CarRadiator",
			"SparkPlug",
			"Material_Shelter_Fabric",
			"Reflector_1_1",
			"Reflector_2_1",
			"uazwheel_1_1",
			"uazwheel_1_2",
			"uazwheel_2_1",
			"uazwheel_2_2",
			"uazdriverdoor",
			"uazcodriverdoor",
			"uazcargo1door",
			"uazcargo2door",
			"uazhooddoor",
			"CamoNet",
			"KeyChain"
		};
		class AnimationSources: AnimationSources
		{
			class roof
			{
				source="user";
				animPeriod=0.0099999998;
				initPhase=1;
			};
			class frameroof
			{
				source="user";
				animPeriod=0.0099999998;
				initPhase=0;
			};
			class backseats
			{
				source="user";
				animPeriod=0.0099999998;
				initPhase=0;
			};
		};
		class GUIInventoryAttachmentsProps
		{
			class Engine
			{
				name="$STR_attachment_Engine0";
				description="";
				icon="cat_vehicle_engine";
				attachmentSlots[]=
				{
					"CarBattery",
					"CarRadiator",
					"SparkPlug"
				};
			};
			class Body
			{
				name="$STR_attachment_Body0";
				description="";
				icon="cat_vehicle_body";
				attachmentSlots[]=
				{
					"Reflector_1_1",
					"Reflector_2_1",
					"uazdriverdoor",
					"uazcodriverdoor",
					"uazcargo1door",
					"uazcargo2door",
					"uazhooddoor",
					"Material_Shelter_Fabric",
					"CamoNet",
					"KeyChain"
				};
			};
			class Chassis
			{
				name="$STR_attachment_Chassis0";
				description="";
				icon="cat_vehicle_chassis";
				attachmentSlots[]=
				{
					"uazwheel_1_1",
					"uazwheel_1_2",
					"uazwheel_2_1",
					"uazwheel_2_2"
				};
			};
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=1000;
					healthLevels[]=
					{
						
						{
							1,
							{}
						},
						
						{
							0.69999999,
							{}
						},
						
						{
							0.5,
							{}
						},
						
						{
							0.30000001,
							{}
						},
						
						{
							0,
							{}
						}
					};
				};
			};
			class DamageZones
			{
				class Chassis
				{
					class Health
					{
						hitpoints=3000;
						transferToGlobalCoef=0;
						healthLevels[]=
						{
							
							{
								1,
								{}
							},
							
							{
								0.69999999,
								{}
							},
							
							{
								0.5,
								{}
							},
							
							{
								0.30000001,
								{}
							},
							
							{
								0,
								{}
							}
						};
					};
					memoryPoints[]=
					{
						"dmgZone_chassis"
					};
					componentNames[]=
					{
						"dmgZone_chassis"
					};
					transferToZonesNames[]=
					{
						"Engine"
					};
					transferToZonesThreshold[]={0.40000001};
					transferToZonesCoefs[]={0.30000001};
					fatalInjuryCoef=-1;
					inventorySlots[]={};
				};
				class Front
				{
					class Health
					{
						hitpoints=1200;
						transferToGlobalCoef=1;
						healthLevels[]=
						{
							
							{
								1,
								{}
							},
							
							{
								0.69999999,
								{}
							},
							
							{
								0.5,
								{}
							},
							
							{
								0.30000001,
								{}
							},
							
							{
								0,
								{}
							}
						};
					};
					transferToZonesNames[]=
					{
						"Reflector_1_1",
						"Reflector_2_1",
						"WindowFront",
						"Engine",
						"Radiator"
					};
					transferToZonesThreshold[]={0.5,0.80000001,0.80000001};
					transferToZonesCoefs[]={0.80000001,0.60000002,0.60000002};
					memoryPoints[]=
					{
						"dmgZone_front"
					};
					componentNames[]=
					{
						"dmgZone_front"
					};
					fatalInjuryCoef=-1;
					inventorySlots[]=
					{
						"uazhooddoor",
						"CarRadiator",
						"uazwheel_1_1",
						"uazwheel_2_1"
					};
					inventorySlotsCoefs[]={0.80000001,0.5,0.40000001,0.40000001};
				};
				class Radiator
				{
					class Health
					{
						hitpoints=800;
						transferToGlobalCoef=0;
					};
					memoryPoints[]={};
					componentNames[]={};
					fatalInjuryCoef=-1;
					inventorySlots[]=
					{
						"CarRadiator"
					};
				};
				class Back
				{
					class Health
					{
						hitpoints=1500;
						transferToGlobalCoef=0;
						healthLevels[]=
						{
							
							{
								1,
								{}
							},
							
							{
								0.69999999,
								{}
							},
							
							{
								0.5,
								{}
							},
							
							{
								0.30000001,
								{}
							},
							
							{
								0,
								{}
							}
						};
					};
					transferToZonesNames[]=
					{
						"Fender_1_2",
						"Fender_2_2"
					};
					transferToZonesCoefs[]={0.2,0.2};
					memoryPoints[]=
					{
						"dmgZone_back"
					};
					componentNames[]=
					{
						"dmgZone_back"
					};
					fatalInjuryCoef=-1;
					inventorySlots[]=
					{
						"uazwheel_1_2",
						"uazwheel_2_2"
					};
				};
				class Roof
				{
					class Health
					{
						hitpoints=700;
						transferToGlobalCoef=0;
						healthLevels[]=
						{
							
							{
								1,
								
								{
									"DZ\gear\camping\data\tent_pristine_co.paa"
								}
							},
							
							{
								0.69999999,
								
								{
									"DZ\gear\camping\data\tent_worn_co.paa"
								}
							},
							
							{
								0.5,
								
								{
									"DZ\gear\camping\data\tent_worn_co.paa"
								}
							},
							
							{
								0.30000001,
								
								{
									"DZ\gear\camping\data\tent_damage_co.paa"
								}
							},
							
							{
								0,
								
								{
									"DZ\gear\camping\data\tent_destruct_co.paa"
								}
							}
						};
					};
					memoryPoints[]=
					{
						"dmgZone_roof"
					};
					componentNames[]=
					{
						"dmgZone_roof"
					};
					fatalInjuryCoef=-1;
					inventorySlots[]=
					{
						"Material_Shelter_Fabric"
					};
				};
				class Fender_1_1
				{
					class Health
					{
						hitpoints=1200;
						transferToGlobalCoef=0;
						healthLevels[]=
						{
							
							{
								1,
								{}
							},
							
							{
								0.69999999,
								{}
							},
							
							{
								0.5,
								{}
							},
							
							{
								0.30000001,
								{}
							},
							
							{
								0,
								{}
							}
						};
					};
					transferToZonesNames[]=
					{
						"Front",
						"Reflector_1_1",
						"Engine",
						"CarRadiator",
						"WindowFront",
						"Battery"
					};
					transferToZonesCoefs[]={0.30000001,0.30000001,0.60000002,0.5,0.40000001,0.69999999};
					memoryPoints[]=
					{
						"dmgzone_fender_FL"
					};
					componentNames[]=
					{
						"dmgzone_fender_FL"
					};
					fatalInjuryCoef=-1;
					inventorySlots[]=
					{
						"uazwheel_1_1",
						"uazhooddoor"
					};
				};
				class Fender_1_2: Fender_1_1
				{
					transferToZonesNames[]=
					{
						"Back",
						"FuelTank"
					};
					transferToZonesThreshold[]={0.2,0.2,0.2};
					transferToZonesCoefs[]={0.30000001,0.2,0.69999999};
					memoryPoints[]=
					{
						"dmgzone_fender_BL"
					};
					componentNames[]=
					{
						"dmgzone_fender_BL"
					};
					inventorySlots[]=
					{
						"uazwheel_1_2"
					};
				};
				class Fender_2_1: Fender_1_1
				{
					transferToZonesNames[]=
					{
						"Front",
						"Reflector_2_1",
						"Engine",
						"CarRadiator",
						"WindowFront",
						"Battery"
					};
					transferToZonesCoefs[]={0.30000001,0.30000001,0.60000002,0.5,0.40000001,0.69999999};
					memoryPoints[]=
					{
						"dmgzone_fender_FR"
					};
					componentNames[]=
					{
						"dmgzone_fender_FR"
					};
					inventorySlots[]=
					{
						"uazwheel_2_1",
						"uazhooddoor"
					};
				};
				class Fender_2_2: Fender_1_1
				{
					transferToZonesNames[]=
					{
						"Back",
						"FuelTank"
					};
					transferToZonesThreshold[]={0.2,0.2,0.2};
					transferToZonesCoefs[]={0.30000001,0.2,0.69999999};
					memoryPoints[]=
					{
						"dmgzone_fender_BR"
					};
					componentNames[]=
					{
						"dmgzone_fender_BR"
					};
					inventorySlots[]=
					{
						"uazwheel_2_2"
					};
				};
				class WindowFront
				{
					class Health
					{
						hitpoints=800;
						transferToGlobalCoef=0;
						healthLevels[]=
						{
							
							{
								1,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\hatchback_02_windows.rvmat"
								}
							},
							
							{
								0.69999999,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\hatchback_02_windows.rvmat"
								}
							},
							
							{
								0.5,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\glass_i_damage.rvmat"
								}
							},
							
							{
								0.30000001,
								
								{
									"dz\vehicles\wheeled\hatchback_02\data\glass_i_damage.rvmat"
								}
							},
							
							{
								0,
								"hidden"
							}
						};
					};
					memoryPoints[]=
					{
						"dmgZone_windowFront"
					};
					componentNames[]=
					{
						"dmgZone_windowFront"
					};
					inventorySlots[]={};
				};
				class Engine
				{
					class Health
					{
						hitpoints=1000;
						transferToGlobalCoef=1;
						healthLevels[]=
						{
							
							{
								1,
								{}
							},
							
							{
								0.69999999,
								{}
							},
							
							{
								0.5,
								{}
							},
							
							{
								0.30000001,
								{}
							},
							
							{
								0,
								{}
							}
						};
					};
					memoryPoints[]=
					{
						"dmgZone_engine"
					};
					componentNames[]=
					{
						"dmgZone_engine"
					};
					fatalInjuryCoef=0.001;
					inventorySlots[]=
					{
						"CarBattery",
						"SparkPlug"
					};
					inventorySlotsCoefs[]={0.2,0.1,0.2};
				};
				class FuelTank
				{
					class Health
					{
						hitpoints=2500;
						transferToGlobalCoef=0;
						healthLevels[]=
						{
							
							{
								1,
								{}
							},
							
							{
								0.69999999,
								{}
							},
							
							{
								0.5,
								{}
							},
							
							{
								0.30000001,
								{}
							},
							
							{
								0,
								{}
							}
						};
					};
					fatalInjuryCoef=-1;
					inventorySlots[]={};
				};
				class Reflector_1_1
				{
					fatalInjuryCoef=-1;
					componentNames[]=
					{
						"dmgZone_lights_1_1"
					};
					memoryPoints[]=
					{
						"dmgZone_lights_1_1"
					};
					class Health
					{
						hitpoints=10;
						transferToGlobalCoef=0;
						healthLevels[]=
						{
							
							{
								1,
								
								{
									"dz\vehicles\wheeled\offroadhatchback\data\headlights_glass.rvmat"
								}
							},
							
							{
								0.69999999,
								{}
							},
							
							{
								0.5,
								
								{
									"dz\vehicles\wheeled\offroadhatchback\data\glass_i_damage.rvmat"
								}
							},
							
							{
								0.30000001,
								{}
							},
							
							{
								0,
								
								{
									"dz\vehicles\wheeled\offroadhatchback\data\glass_i_destruct.rvmat"
								}
							}
						};
					};
					displayName="$STR_CfgVehicleDmg_Reflector0";
					transferToZonesNames[]=
					{
						"Front",
						"Fender_1_1"
					};
					transferToZonesCoefs[]={1,1};
					inventorySlots[]=
					{
						"Reflector_1_1"
					};
					inventorySlotsCoefs[]={1,0.30000001};
				};
				class Reflector_2_1: Reflector_1_1
				{
					memoryPoints[]=
					{
						"dmgZone_lights_2_1"
					};
					componentNames[]=
					{
						"dmgZone_lights_2_1"
					};
					transferToZonesNames[]=
					{
						"Front",
						"Fender_2_1"
					};
					inventorySlots[]=
					{
						"Reflector_2_1"
					};
				};
			};
		};
	};
	class ExpansionUAZRoofless: ExpansionUAZ
	{
		scope=2;
		displayname="$STR_EXPANSION_VEHICLE_UAZ_ROOFLESS";
		vehicleClass="Expansion_Car";
		hiddenSelections[]=
		{
			"light_1_1",
			"light_2_1",
			"light_brake_1_2",
			"light_brake_2_2",
			"light_reverse_1_2",
			"light_reverse_2_2",
			"light_1_2",
			"light_2_2",
			"light_dashboard",
			"camo1",
			"camo2",
			"dmgZone_front",
			"dmgZone_back",
			"dmgzone_fender_FL",
			"dmgzone_fender_BL",
			"dmgzone_fender_FR",
			"dmgzone_fender_BR"
		};
		hiddenSelectionsTextures[]=
		{
			"dz\vehicles\wheeled\civiliansedan\data\sedan_glass_ca.paa",
			"dz\vehicles\wheeled\civiliansedan\data\sedan_glass_ca.paa",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_out_01_co.paa",
			"DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_int_01_co.paa",
			"",
			"",
			"",
			"",
			"",
			""
		};
		hiddenSelectionsMaterials[]=
		{
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_exterior_01.rvmat",
			"DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_interior_01.rvmat",
			"DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_exterior_01.rvmat",
			"DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_exterior_01.rvmat",
			"DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_exterior_01.rvmat",
			"DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_exterior_01.rvmat",
			"DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_exterior_01.rvmat",
			"DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_exterior_01.rvmat"
		};
		attachments[]=
		{
			"CarBattery",
			"CarRadiator",
			"SparkPlug",
			"Reflector_1_1",
			"Reflector_2_1",
			"uazwheel_1_1",
			"uazwheel_1_2",
			"uazwheel_2_1",
			"uazwheel_2_2",
			"uazdriverdoor",
			"uazcodriverdoor",
			"uazcargo1door",
			"uazcargo2door",
			"uazhooddoor",
			"CamoNet",
			"KeyChain"
		};
		class AnimationSources: AnimationSources
		{
			class roof
			{
				source="user";
				animPeriod=0.0099999998;
				initPhase=1;
			};
			class frameroof
			{
				source="user";
				animPeriod=0.0099999998;
				initPhase=1;
			};
			class backseats
			{
				source="user";
				animPeriod=0.0099999998;
				initPhase=0;
			};
		};
		class GUIInventoryAttachmentsProps
		{
			class Engine
			{
				name="$STR_attachment_Engine0";
				description="";
				icon="cat_vehicle_engine";
				attachmentSlots[]=
				{
					"CarBattery",
					"CarRadiator",
					"SparkPlug"
				};
			};
			class Body
			{
				name="$STR_attachment_Body0";
				description="";
				icon="cat_vehicle_body";
				attachmentSlots[]=
				{
					"Reflector_1_1",
					"Reflector_2_1",
					"uazdriverdoor",
					"uazcodriverdoor",
					"uazcargo1door",
					"uazcargo2door",
					"uazhooddoor",
					"CamoNet",
					"KeyChain"
				};
			};
			class Chassis
			{
				name="$STR_attachment_Chassis0";
				description="";
				icon="cat_vehicle_chassis";
				attachmentSlots[]=
				{
					"uazwheel_1_1",
					"uazwheel_1_2",
					"uazwheel_2_1",
					"uazwheel_2_2"
				};
			};
		};
	};
	class ExpansionUAZCargoRoofless: ExpansionUAZ
	{
		scope=2;
		displayname="$STR_EXPANSION_VEHICLE_UAZ_CARGO_ROOFLESS";
		model="\DayZExpansion\Vehicles\Ground\UAZ\UAZCargoRoofless.p3d";
		vehicleClass="Expansion_Car";
		animPhysDetachSpeed=100;
		hiddenSelections[]=
		{
			"light_1_1",
			"light_2_1",
			"light_brake_1_2",
			"light_brake_2_2",
			"light_reverse_1_2",
			"light_reverse_2_2",
			"light_1_2",
			"light_2_2",
			"light_dashboard",
			"camo1",
			"camo2",
			"dmgZone_front",
			"dmgZone_back",
			"dmgzone_fender_FL",
			"dmgzone_fender_BL",
			"dmgzone_fender_FR",
			"dmgzone_fender_BR"
		};
		hiddenSelectionsTextures[]=
		{
			"dz\vehicles\wheeled\civiliansedan\data\sedan_glass_ca.paa",
			"dz\vehicles\wheeled\civiliansedan\data\sedan_glass_ca.paa",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_out_01_co.paa",
			"DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_int_01_co.paa",
			"",
			"",
			"",
			"",
			"",
			""
		};
		hiddenSelectionsMaterials[]=
		{
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"",
			"DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_exterior_01.rvmat",
			"DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_interior_01.rvmat",
			"DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_exterior_01.rvmat",
			"DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_exterior_01.rvmat",
			"DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_exterior_01.rvmat",
			"DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_exterior_01.rvmat",
			"DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_exterior_01.rvmat",
			"DayZExpansion\Vehicles\Ground\UAZ\new_data\uaz_exterior_01.rvmat"
		};
		attachments[]=
		{
			"CarBattery",
			"CarRadiator",
			"SparkPlug",
			"Reflector_1_1",
			"Reflector_2_1",
			"uazwheel_1_1",
			"uazwheel_1_2",
			"uazwheel_2_1",
			"uazwheel_2_2",
			"uazdriverdoor",
			"uazcodriverdoor",
			"uazcargo1door",
			"uazcargo2door",
			"uazhooddoor",
			"CamoNet",
			"KeyChain"
		};
		class AnimationSources: AnimationSources
		{
			class roof
			{
				source="user";
				animPeriod=0.0099999998;
				initPhase=1;
			};
			class backseats
			{
				source="user";
				animPeriod=0.0099999998;
				initPhase=1;
			};
		};
		class Cargo
		{
			itemsCargoSize[]={10,45};
			allowOwnedCargoManipulation=1;
			openable=1;
		};
		class Crew: Crew
		{
			class Driver: Driver
			{
				actionSel="seat_driver";
				proxyPos="crewDriver";
				getInPos="pos_driver";
				getInDir="pos_driver_dir";
			};
			class CoDriver: CoDriver
			{
				actionSel="seat_codriver";
				proxyPos="crewCoDriver";
				getInPos="pos_codriver";
				getInDir="pos_codriver_dir";
			};
		};
		class GUIInventoryAttachmentsProps
		{
			class Engine
			{
				name="$STR_attachment_Engine0";
				description="";
				icon="cat_vehicle_engine";
				attachmentSlots[]=
				{
					"CarBattery",
					"CarRadiator",
					"SparkPlug"
				};
			};
			class Body
			{
				name="$STR_attachment_Body0";
				description="";
				icon="cat_vehicle_body";
				attachmentSlots[]=
				{
					"Reflector_1_1",
					"Reflector_2_1",
					"uazdriverdoor",
					"uazcodriverdoor",
					"uazcargo1door",
					"uazcargo2door",
					"uazhooddoor",
					"CamoNet",
					"KeyChain"
				};
			};
			class Chassis
			{
				name="$STR_attachment_Chassis0";
				description="";
				icon="cat_vehicle_chassis";
				attachmentSlots[]=
				{
					"uazwheel_1_1",
					"uazwheel_1_2",
					"uazwheel_2_1",
					"uazwheel_2_2"
				};
			};
		};
	};
	class ExpansionUAZCargo: ExpansionUAZ
	{
		scope=2;
		displayname="$STR_EXPANSION_VEHICLE_UAZ_CARGO";
		vehicleClass="Expansion_Car";
		class AnimationSources: AnimationSources
		{
			class backseats
			{
				source="user";
				animPeriod=0.0099999998;
				initPhase=1;
			};
		};
		class Cargo
		{
			itemsCargoSize[]={10,45};
			allowOwnedCargoManipulation=1;
			openable=1;
		};
		class Crew: Crew
		{
			class Driver: Driver
			{
				actionSel="seat_driver";
				proxyPos="crewDriver";
				getInPos="pos_driver";
				getInDir="pos_driver_dir";
			};
			class CoDriver: CoDriver
			{
				actionSel="seat_codriver";
				proxyPos="crewCoDriver";
				getInPos="pos_codriver";
				getInDir="pos_codriver_dir";
			};
		};
	};
	class ExpansionSpraycanBase;
	class ExpansionSpraycanGreen: ExpansionSpraycanBase
	{
		scope=2;
		displayName="$STR_EXPANSION_SPRAYCAN_Green_NAME";
		descriptionShort="$STR_EXPANSION_SPRAYCAN_Green_DESC";
		hiddenSelectionsTextures[]=
		{
			"DZ\gear\consumables\data\spraycan_green_co.paa"
		};
		skinName="Green";
	};
	class ExpansionUAZ_Green: ExpansionUAZ
	{
		skinBase="ExpansionUAZ";
		skinName="Green";
	};
	class ExpansionUAZCargo_Green: ExpansionUAZCargo
	{
		skinBase="ExpansionUAZ";
		skinName="Green";
	};
	class ExpansionUAZRoofless_Green: ExpansionUAZRoofless
	{
		skinBase="ExpansionUAZ";
		skinName="Green";
	};
	class ExpansionUAZCargoRoofless_Green: ExpansionUAZCargoRoofless
	{
		skinBase="ExpansionUAZ";
		skinName="Green";
	};
	class ExpansionUAZDoorDriver_Green: ExpansionUAZDoorDriver
	{
		skinBase="ExpansionUAZDoorDriver";
		skinName="Green";
	};
	class ExpansionUAZDoorCoDriver_Green: ExpansionUAZDoorCoDriver
	{
		skinBase="ExpansionUAZDoorCoDriver";
		skinName="Green";
	};
	class ExpansionUAZDoorCargo1_Green: ExpansionUAZDoorCargo1
	{
		skinBase="ExpansionUAZDoorCargo1";
		skinName="Green";
	};
	class ExpansionUAZDoorCargo2_Green: ExpansionUAZDoorCargo2
	{
		skinBase="ExpansionUAZDoorCargo2";
		skinName="Green";
	};
	class ExpansionUAZDoorHood_Green: ExpansionUAZDoorHood
	{
		skinBase="ExpansionUAZDoorHood";
		skinName="Green";
	};
	class ExpansionSpraycanBlack: ExpansionSpraycanBase
	{
		scope=2;
		displayName="$STR_EXPANSION_SPRAYCAN_Black_NAME";
		descriptionShort="$STR_EXPANSION_SPRAYCAN_Black_DESC";
		hiddenSelectionsTextures[]=
		{
			"DZ\gear\consumables\data\spraycan_black_co.paa"
		};
		skinName="Black";
	};
	class ExpansionUAZ_Black: ExpansionUAZ
	{
		skinBase="ExpansionUAZ";
		skinName="Black";
	};
	class ExpansionUAZCargo_Black: ExpansionUAZCargo
	{
		skinBase="ExpansionUAZ";
		skinName="Black";
	};
	class ExpansionUAZRoofless_Black: ExpansionUAZRoofless
	{
		skinBase="ExpansionUAZ";
		skinName="Black";
	};
	class ExpansionUAZCargoRoofless_Black: ExpansionUAZCargoRoofless
	{
		skinBase="ExpansionUAZ";
		skinName="Black";
	};
	class ExpansionUAZDoorDriver_Black: ExpansionUAZDoorDriver
	{
		skinBase="ExpansionUAZDoorDriver";
		skinName="Black";
	};
	class ExpansionUAZDoorCoDriver_Black: ExpansionUAZDoorCoDriver
	{
		skinBase="ExpansionUAZDoorCoDriver";
		skinName="Black";
	};
	class ExpansionUAZDoorCargo1_Black: ExpansionUAZDoorCargo1
	{
		skinBase="ExpansionUAZDoorCargo1";
		skinName="Black";
	};
	class ExpansionUAZDoorCargo2_Black: ExpansionUAZDoorCargo2
	{
		skinBase="ExpansionUAZDoorCargo2";
		skinName="Black";
	};
	class ExpansionUAZDoorHood_Black: ExpansionUAZDoorHood
	{
		skinBase="ExpansionUAZDoorHood";
		skinName="Black";
	};
	class ExpansionSpraycanLightGreen: ExpansionSpraycanBase
	{
		scope=2;
		displayName="$STR_EXPANSION_SPRAYCAN_LightGreen_NAME";
		descriptionShort="$STR_EXPANSION_SPRAYCAN_LightGreen_DESC";
		hiddenSelectionsTextures[]=
		{
			"DZ\gear\consumables\data\spraycan_green_co.paa"
		};
		skinName="LightGreen";
	};
	class ExpansionUAZ_LightGreen: ExpansionUAZ
	{
		skinBase="ExpansionUAZ";
		skinName="LightGreen";
	};
	class ExpansionUAZCargo_LightGreen: ExpansionUAZCargo
	{
		skinBase="ExpansionUAZ";
		skinName="LightGreen";
	};
	class ExpansionUAZRoofless_LightGreen: ExpansionUAZRoofless
	{
		skinBase="ExpansionUAZ";
		skinName="LightGreen";
	};
	class ExpansionUAZCargoRoofless_LightGreen: ExpansionUAZCargoRoofless
	{
		skinBase="ExpansionUAZ";
		skinName="LightGreen";
	};
	class ExpansionUAZDoorDriver_LightGreen: ExpansionUAZDoorDriver
	{
		skinBase="ExpansionUAZDoorDriver";
		skinName="LightGreen";
	};
	class ExpansionUAZDoorCoDriver_LightGreen: ExpansionUAZDoorCoDriver
	{
		skinBase="ExpansionUAZDoorCoDriver";
		skinName="LightGreen";
	};
	class ExpansionUAZDoorCargo1_LightGreen: ExpansionUAZDoorCargo1
	{
		skinBase="ExpansionUAZDoorCargo1";
		skinName="LightGreen";
	};
	class ExpansionUAZDoorCargo2_LightGreen: ExpansionUAZDoorCargo2
	{
		skinBase="ExpansionUAZDoorCargo2";
		skinName="LightGreen";
	};
	class ExpansionUAZDoorHood_LightGreen: ExpansionUAZDoorHood
	{
		skinBase="ExpansionUAZDoorHood";
		skinName="LightGreen";
	};
};
class CfgNonAIVehicles
{
	class ProxyVehiclePart;
	class ProxyUAZWheel: ProxyVehiclePart
	{
		model="\DayZExpansion\Vehicles\Ground\UAZ\proxy\UAZWheel.p3d";
		inventorySlot[]=
		{
			"uazwheel_1_1",
			"uazwheel_1_2",
			"uazwheel_2_1",
			"uazwheel_2_2"
		};
	};
	class Proxyuazdriverdoor: ProxyVehiclePart
	{
		model="\DayZExpansion\Vehicles\Ground\UAZ\proxy\uazdriverdoor.p3d";
		inventorySlot="uazdriverdoor";
	};
	class Proxyuazcodriverdoor: ProxyVehiclePart
	{
		model="\DayZExpansion\Vehicles\Ground\UAZ\proxy\uazcodriverdoor.p3d";
		inventorySlot="uazcodriverdoor";
	};
	class Proxyuazcargo1door: ProxyVehiclePart
	{
		model="\DayZExpansion\Vehicles\Ground\UAZ\proxy\uazcargo1door.p3d";
		inventorySlot="uazcargo1door";
	};
	class Proxyuazcargo2door: ProxyVehiclePart
	{
		model="\DayZExpansion\Vehicles\Ground\UAZ\proxy\uazcargo2door.p3d";
		inventorySlot="uazcargo2door";
	};
	class Proxyuaztrunkdoor: ProxyVehiclePart
	{
		model="\DayZExpansion\Vehicles\Ground\UAZ\proxy\uaztrunkdoor.p3d";
		inventorySlot="uaztrunkdoor";
	};
	class Proxyuazhooddoor: ProxyVehiclePart
	{
		model="\DayZExpansion\Vehicles\Ground\UAZ\proxy\uazhooddoor.p3d";
		inventorySlot="uazhooddoor";
	};
};
