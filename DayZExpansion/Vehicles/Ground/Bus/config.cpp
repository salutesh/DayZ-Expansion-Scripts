class CfgPatches
{
	class DayZExpansion_Vehicles_Ground_Ikarus
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
	class Slot_busmegaphone
	{
		name="busmegaphone";
		displayName="Bus Megaphone";
		selection="busmegaphone";
		ghostIcon="walkietalkie";
	};
};
class CfgVehicles
{
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
	class TransitBusWheel;
	class TransitBusWheelDouble;
	class CarScript;
	class CarWheel;
	class ExpansionBusWheel: CarWheel
	{
		scope=2;
		model="\DZ\vehicles\wheeled\TransitBus\proxy\ikarusWheel.p3d";
		displayName="$STR_TransitBusWheel0";
		itemSize[]={6,6};
		weight=25000;
		physLayer="item_large";
		rotationFlags=12;
		inventorySlot[]=
		{
			"BusWheel_1_1",
			"BusWheel_2_1"
		};
		radiusByDamage[]={0,0.50999999,0.30000001,0.44999999,0.99980003,0.38,0.99989998,0.30000001};
		radius=0.49599999;
		friction=0.99900001;
		width=0.213;
		tyreRollResistance=0.015;
		tyreTread=0.80000001;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=700;
					healthLevels[]=
					{
						
						{
							1,
							
							{
								"dz\vehicles\wheeled\transitbus\data\ikarus_wheel.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"dz\vehicles\wheeled\transitbus\data\ikarus_wheel.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"dz\vehicles\wheeled\transitbus\data\ikarus_wheel_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"dz\vehicles\wheeled\transitbus\data\ikarus_wheel_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"dz\vehicles\wheeled\transitbus\data\ikarus_wheel_destruct.rvmat"
							}
						}
					};
				};
			};
		};
	};
	class ExpansionBusWheel_Ruined: ExpansionBusWheel
	{
		model="\DZ\vehicles\wheeled\TransitBus\proxy\ikarusWheel_destroyed.p3d";
		displayName="$STR_TransitBusWheel_Ruined0";
		radius=0.30000001;
		friction=-1;
		width=0.213;
		tyreRollResistance=0.015;
		tyreTread=0.80000001;
	};
	class ExpansionBusWheelDouble: CarWheel
	{
		scope=2;
		model="\DZ\vehicles\wheeled\TransitBus\proxy\ikarusWheel_rear.p3d";
		displayName="$STR_TransitBusWheelDouble0";
		itemSize[]={6,6};
		weight=25000;
		physLayer="item_large";
		rotationFlags=12;
		width=0.315;
		radiusByDamage[]={0,0.50999999,0.30000001,0.44999999,0.99980003,0.38,0.99989998,0.30000001};
		radius=0.49599999;
		friction=0.99900001;
		inventorySlot[]=
		{
			"BusWheel_1_2",
			"BusWheel_2_2"
		};
		tyreRollResistance=0.015;
		tyreTread=0.80000001;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=700;
					healthLevels[]=
					{
						
						{
							1,
							
							{
								"dz\vehicles\wheeled\transitbus\data\ikarus_wheel.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"dz\vehicles\wheeled\transitbus\data\ikarus_wheel.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"dz\vehicles\wheeled\transitbus\data\ikarus_wheel_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"dz\vehicles\wheeled\transitbus\data\ikarus_wheel_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"dz\vehicles\wheeled\transitbus\data\ikarus_wheel_destruct.rvmat"
							}
						}
					};
				};
			};
		};
	};
	class ExpansionBusWheelDouble_Ruined: ExpansionBusWheelDouble
	{
		model="\DZ\vehicles\wheeled\TransitBus\proxy\ikarusWheel_rear_destroyed.p3d";
		displayName="$STR_TransitBusWheelDouble_Ruined0";
		width=0.426;
		radius=0.30000001;
		friction=-1;
		inventorySlot[]=
		{
			"BusWheel_1_2",
			"BusWheel_2_2"
		};
		tyreRollResistance=0.015;
		tyreTread=0.80000001;
	};
	class ExpansionBus: CarScript
	{
		scope=2;
		displayname="$STR_EXPANSION_VEHICLE_BUS";
		model="\DayZExpansion\Vehicles\Ground\Bus\Ikarus.p3d";
		modelZeroPointDistanceFromGround=0;
		vehicleClass="Expansion_Car";
		animPhysDetachSpeed=100;
		attachments[]=
		{
			"TruckBattery",
			"Reflector_1_1",
			"Reflector_2_1",
			"CarRadiator",
			"GlowPlug",
			"BusWheel_1_1",
			"BusWheel_1_2",
			"BusWheel_2_1",
			"BusWheel_2_2",
			"CamoNet",
			"KeyChain"
		};
		doors[]={};
		fuelCapacity=62;
		fuelConsumption=11;
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
			"camo",
			"dmgzone_BL",
			"dmgzone_BR",
			"dmgzone_FL",
			"dmgzone_ML",
			"dmgzone_FR",
			"dmgzone_front",
			"dmgzone_back",
			"dmgzone_roof"
		};
		hiddenSelectionsTextures[]=
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
			"DayZExpansion\Vehicles\Ground\Bus\Data\bus_exterior_co.paa",
			"",
			"",
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
			"DayZExpansion\Vehicles\Ground\Bus\Data\bus_exterior.rvmat",
			"DayZExpansion\Vehicles\Ground\Bus\Data\bus_exterior.rvmat",
			"DayZExpansion\Vehicles\Ground\Bus\Data\bus_exterior.rvmat",
			"DayZExpansion\Vehicles\Ground\Bus\Data\bus_exterior.rvmat",
			"DayZExpansion\Vehicles\Ground\Bus\Data\bus_exterior.rvmat",
			"DayZExpansion\Vehicles\Ground\Bus\Data\bus_exterior.rvmat",
			"DayZExpansion\Vehicles\Ground\Bus\Data\bus_exterior.rvmat",
			"DayZExpansion\Vehicles\Ground\Bus\Data\bus_exterior.rvmat",
			"DayZExpansion\Vehicles\Ground\Bus\Data\bus_exterior.rvmat"
		};
		defaultSkin="Blue";
		dashboardMatOn="dz\vehicles\wheeled\transitbus\data\ikarus_dashboarde.rvmat";
		dashboardMatOff="dz\vehicles\wheeled\transitbus\data\ikarus_dashboard.rvmat";
		frontReflectorMatOn="dayzexpansion\vehicles\ground\bus\data\bus_exterior_on.rvmat";
		frontReflectorMatOff="dayzexpansion\vehicles\ground\bus\data\bus_exterior.rvmat";
		brakeReflectorMatOn="dayzexpansion\vehicles\ground\bus\data\bus_exterior_on_red.rvmat";
		brakeReflectorMatOff="dayzexpansion\vehicles\ground\bus\data\bus_exterior.rvmat";
		ReverseReflectorMatOn="dayzexpansion\vehicles\ground\bus\data\bus_exterior_on.rvmat";
		ReverseReflectorMatOff="dayzexpansion\vehicles\ground\bus\data\bus_exterior.rvmat";
		TailReflectorMatOn="dayzexpansion\vehicles\ground\bus\data\bus_exterior_on.rvmat";
		TailReflectorMatOff="dayzexpansion\vehicles\ground\bus\data\bus_exterior.rvmat";
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
			class Cargo3
			{
				actionSel="seat_cargo3";
				proxyPos="crewCargo3";
				getInPos="pos_cargo3";
				getInDir="pos_cargo3_dir";
			};
			class Cargo4
			{
				actionSel="seat_cargo4";
				proxyPos="crewCargo4";
				getInPos="pos_cargo4";
				getInDir="pos_cargo4_dir";
			};
			class Cargo5
			{
				actionSel="seat_cargo5";
				proxyPos="crewCargo5";
				getInPos="pos_cargo5";
				getInDir="pos_cargo5_dir";
			};
			class Cargo6
			{
				actionSel="seat_cargo6";
				proxyPos="crewCargo6";
				getInPos="pos_cargo6";
				getInDir="pos_cargo6_dir";
			};
			class Cargo7
			{
				actionSel="seat_cargo7";
				proxyPos="crewCargo7";
				getInPos="pos_cargo7";
				getInDir="pos_cargo7_dir";
			};
			class Cargo8
			{
				actionSel="seat_cargo8";
				proxyPos="crewCargo8";
				getInPos="pos_cargo8";
				getInDir="pos_cargo8_dir";
			};
			class Cargo9
			{
				actionSel="seat_cargo9";
				proxyPos="crewCargo9";
				getInPos="pos_cargo9";
				getInDir="pos_cargo9_dir";
			};
			class Cargo10
			{
				actionSel="seat_cargo10";
				proxyPos="crewCargo10";
				getInPos="pos_cargo10";
				getInDir="pos_cargo10_dir";
			};
			class Cargo11
			{
				actionSel="seat_cargo11";
				proxyPos="crewCargo11";
				getInPos="pos_cargo11";
				getInDir="pos_cargo11_dir";
			};
			class Cargo12
			{
				actionSel="seat_cargo12";
				proxyPos="crewCargo12";
				getInPos="pos_cargo12";
				getInDir="pos_cargo12_dir";
			};
			class Cargo13
			{
				actionSel="seat_cargo13";
				proxyPos="crewCargo13";
				getInPos="pos_cargo13";
				getInDir="pos_cargo13_dir";
			};
			class Cargo14
			{
				actionSel="seat_cargo14";
				proxyPos="crewCargo14";
				getInPos="pos_cargo14";
				getInDir="pos_cargo14_dir";
			};
			class Cargo15
			{
				actionSel="seat_cargo15";
				proxyPos="crewCargo15";
				getInPos="pos_cargo15";
				getInDir="pos_cargo15_dir";
			};
			class Cargo16
			{
				actionSel="seat_cargo16";
				proxyPos="crewCargo16";
				getInPos="pos_cargo16";
				getInDir="pos_cargo16_dir";
			};
			class Cargo17
			{
				actionSel="seat_cargo17";
				proxyPos="crewCargo17";
				getInPos="pos_cargo17";
				getInDir="pos_cargo17_dir";
			};
			class Cargo18
			{
				actionSel="seat_cargo18";
				proxyPos="crewCargo18";
				getInPos="pos_cargo18";
				getInDir="pos_cargo18_dir";
			};
			class Cargo19
			{
				actionSel="seat_cargo19";
				proxyPos="crewCargo19";
				getInPos="pos_cargo19";
				getInDir="pos_cargo19_dir";
			};
			class Cargo20
			{
				actionSel="seat_cargo20";
				proxyPos="crewCargo20";
				getInPos="pos_cargo20";
				getInDir="pos_cargo20_dir";
			};
		};
		class SimulationModule: SimulationModule
		{
			drive="DRIVE_AWD";
			class Steering
			{
				maxSteeringAngle=30;
				increaseSpeed[]={0,45,60,23,100,12};
				decreaseSpeed[]={0,80,60,40,90,20};
				centeringSpeed[]={0,0,15,25,60,40,100,60};
			};
			class Throttle
			{
				reactionTime=1.2;
				defaultThrust=0.85000002;
				gentleThrust=0.69999999;
				turboCoef=5;
				gentleCoef=0.75;
			};
			class Brake
			{
				pressureBySpeed[]={0,0.5,10,0.46000001,30,0.43000001,40,0.40000001,60,0.5,80,0.60000002};
				reactionTime=0.30000001;
				driverless=0.1;
			};
			class Aerodynamics
			{
				frontalArea=5.75;
				dragCoefficient=0.52999997;
			};
			class Engine
			{
				torqueCurve[]={650,0,750,73,1400,125,3400,150,5400,130,8000,0};
				inertia=0.2;
				frictionTorque=90;
				rollingFriction=0.69999999;
				viscousFriction=1;
				rpmIdle=850;
				rpmMin=900;
				rpmClutch=1350;
				rpmRedline=6000;
			};
			class Clutch
			{
				maxTorqueTransfer=660;
				uncoupleTime=0.30000001;
				coupleTime=0.44999999;
			};
			class Gearbox
			{
				type="GEARBOX_MANUAL";
				reverse=5.526;
				ratios[]={5.6669998,3.5,2.8610001,1.5};
			};
			class CentralDifferential
			{
				ratio=2.1210001;
				type="DIFFERENTIAL_LOCKED";
			};
			class Axles: Axles
			{
				class Front: Front
				{
					wheelHubMass=5;
					wheelHubRadius=0.15000001;
					maxBrakeTorque=5000;
					maxHandbrakeTorque=6000;
					class Differential
					{
						ratio=2.73;
						type="DIFFERENTIAL_LOCKED";
					};
					class Suspension
					{
						stiffness=49000;
						compression=2400;
						damping=8090;
						travelMaxUp=0.050000001;
						travelMaxDown=0.15000001;
					};
					class Wheels: Wheels
					{
						class Left: Left
						{
							inventorySlot="BusWheel_1_1";
							animTurn="turnfrontleft";
							animRotation="wheelfrontleft";
							animDamper="damper_1_1";
							wheelHub="wheel_1_1_damper_land";
						};
						class Right: Right
						{
							inventorySlot="BusWheel_2_1";
							animTurn="turnfrontright";
							animRotation="wheelfrontright";
							animDamper="damper_2_1";
							wheelHub="wheel_2_1_damper_land";
						};
					};
				};
				class Rear: Rear
				{
					wheelHubMass=5;
					wheelHubRadius=0.15000001;
					maxBrakeTorque=5000;
					maxHandbrakeTorque=6000;
					class Differential
					{
						ratio=2.73;
						type="DIFFERENTIAL_LOCKED";
					};
					class Suspension
					{
						stiffness=50000;
						compression=2700;
						damping=8290;
						travelMaxUp=0.050000001;
						travelMaxDown=0.15000001;
					};
					class Wheels: Wheels
					{
						class Left: Left
						{
							inventorySlot="BusWheel_1_2";
							animTurn="turnbackleft";
							animRotation="wheelbackleft";
							animDamper="damper_1_2";
							wheelHub="wheel_1_2_damper_land";
						};
						class Right: Right
						{
							inventorySlot="BusWheel_2_2";
							animTurn="turnbackright";
							animRotation="wheelbackright";
							animDamper="damper_2_2";
							wheelHub="wheel_2_2_damper_land";
						};
					};
				};
			};
		};
		class Cargo
		{
			itemsCargoSize[]={10,50};
			allowOwnedCargoManipulation=1;
			openable=1;
		};
		class AnimationSources: AnimationSources
		{
			class damper_1_1
			{
				source="user";
				initPhase=0.48570001;
				animPeriod=9.9999997e-06;
			};
			class damper_2_1: damper_1_1
			{
			};
			class damper_1_2
			{
				source="user";
				initPhase=0.40020001;
				animPeriod=9.9999997e-06;
			};
			class damper_2_2: damper_1_2
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
				"offroad_02_Engine_Offload_Ext_Rpm1_SoundSet",
				"offroad_02_Engine_Offload_Ext_Rpm2_SoundSet",
				"offroad_02_Engine_Offload_Ext_Rpm3_SoundSet",
				"offroad_02_Engine_Offload_Ext_Rpm4_SoundSet",
				"offroad_02_Engine_Offload_Ext_Rpm5_SoundSet",
				"offroad_02_Engine_Ext_Rpm0_SoundSet",
				"offroad_02_Engine_Ext_Rpm1_SoundSet",
				"offroad_02_Engine_Ext_Rpm2_SoundSet",
				"offroad_02_Engine_Ext_Rpm3_SoundSet",
				"offroad_02_Engine_Ext_Rpm4_SoundSet",
				"offroad_02_Engine_Ext_Rpm5_SoundSet",
				"offroad_02_Engine_Ext_Broken_SoundSet",
				"offroad_Tires_ice_slow_Ext_SoundSet",
				"offroad_Tires_ice_fast_Ext_SoundSet",
				"offroad_Tires_snow_slow_Ext_SoundSet",
				"offroad_Tires_snow_fast_Ext_SoundSet",
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
				"offroad_skid_dirt_SoundSet",
				"offroad_dirt_turn_SoundSet",
				"offroad_Rain_Ext_SoundSet",
				"offroad_damper_left_SoundSet",
				"offroad_damper_right_SoundSet"
			};
			soundSetsInt[]=
			{
				"Offroad_Tires_Asphalt_Fast_General_Int_SoundSet",
				"Offroad_Wind_SoundSet"
			};
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=5000;
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
				class Front: Chassis
				{
					memoryPoints[]=
					{
						"dmgzone_front"
					};
					transferToZonesNames[]=
					{
						"Engine",
						"Reflector_1_1",
						"Reflector_2_1"
					};
					transferToZonesThreshold[]={0.80000001,1,1};
					transferToZonesCoefs[]={0.60000002,0.5,0.5};
					fatalInjuryCoef=-1;
					componentNames[]=
					{
						"dmgzone_front"
					};
				};
				class FrontLeft: Chassis
				{
					transferToZonesNames[]={};
					transferToZonesThreshold[]={};
					transferToZonesCoefs[]={};
					memoryPoints[]=
					{
						"dmgzone_FL"
					};
					componentNames[]=
					{
						"dmgzone_FL"
					};
				};
				class MidLeft: Chassis
				{
					transferToZonesNames[]={};
					transferToZonesThreshold[]={};
					transferToZonesCoefs[]={};
					memoryPoints[]=
					{
						"dmgzone_ML"
					};
					componentNames[]=
					{
						"dmgzone_ML"
					};
				};
				class BackLeft: Chassis
				{
					transferToZonesNames[]={};
					transferToZonesThreshold[]={};
					transferToZonesCoefs[]={};
					memoryPoints[]=
					{
						"dmgzone_BL"
					};
					componentNames[]=
					{
						"dmgzone_BL"
					};
				};
				class FrontRight: Chassis
				{
					transferToZonesNames[]={};
					transferToZonesThreshold[]={};
					transferToZonesCoefs[]={};
					memoryPoints[]=
					{
						"dmgzone_FR"
					};
					componentNames[]=
					{
						"dmgzone_FR"
					};
				};
				class BackRight: Chassis
				{
					transferToZonesNames[]={};
					transferToZonesThreshold[]={};
					transferToZonesCoefs[]={};
					memoryPoints[]=
					{
						"dmgzone_BR"
					};
					componentNames[]=
					{
						"dmgzone_BR"
					};
				};
				class Back: Chassis
				{
					transferToZonesNames[]=
					{
						"Engine",
						"Radiator"
					};
					transferToZonesThreshold[]={};
					transferToZonesCoefs[]={0.2,0.2};
					inventorySlots[]=
					{
						"TruckBattery",
						"SparkPlug"
					};
					inventorySlotsCoefs[]={0.2,0.2};
					memoryPoints[]=
					{
						"dmgzone_back"
					};
					componentNames[]=
					{
						"dmgzone_back"
					};
				};
				class Roof: Chassis
				{
					transferToZonesNames[]={};
					transferToZonesThreshold[]={};
					transferToZonesCoefs[]={};
					memoryPoints[]=
					{
						"dmgzone_roof"
					};
					componentNames[]=
					{
						"dmgzone_roof"
					};
				};
				class WindowFront
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
									"dayzexpansion\vehicles\ground\bus\data\bus_windows.rvmat",
									"DayZExpansion\Vehicles\Ground\Bus\Data\bus_windows_in.rvmat"
								}
							},
							
							{
								0.69999999,
								
								{
									"dayzexpansion\vehicles\ground\bus\data\bus_windows.rvmat",
									"DayZExpansion\Vehicles\Ground\Bus\Data\bus_windows_in.rvmat"
								}
							},
							
							{
								0.5,
								
								{
									"dayzexpansion\vehicles\ground\bus\data\bus_windows_damage.rvmat",
									"DayZExpansion\Vehicles\Ground\Bus\Data\bus_windows_in_damage.rvmat"
								}
							},
							
							{
								0.30000001,
								
								{
									"dayzexpansion\vehicles\ground\bus\data\bus_windows_damage.rvmat",
									"DayZExpansion\Vehicles\Ground\Bus\Data\bus_windows_in_damage.rvmat"
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
					fatalInjuryCoef=-1;
					inventorySlots[]={};
				};
				class WindowBack: WindowFront
				{
					memoryPoints[]=
					{
						"dmgZone_windowback"
					};
					componentNames[]=
					{
						"dmgZone_windowback"
					};
				};
				class Window_1_1
				{
					class Health
					{
						hitpoints=500;
						transferToGlobalCoef=0;
						healthLevels[]=
						{
							
							{
								1,
								
								{
									"dayzexpansion\vehicles\ground\bus\data\bus_windows.rvmat",
									"DayZExpansion\Vehicles\Ground\Bus\Data\bus_windows_in.rvmat"
								}
							},
							
							{
								0.69999999,
								
								{
									"dayzexpansion\vehicles\ground\bus\data\bus_windows.rvmat",
									"DayZExpansion\Vehicles\Ground\Bus\Data\bus_windows_in.rvmat"
								}
							},
							
							{
								0.5,
								
								{
									"dayzexpansion\vehicles\ground\bus\data\bus_windows_damage.rvmat",
									"DayZExpansion\Vehicles\Ground\Bus\Data\bus_windows_in_damage.rvmat"
								}
							},
							
							{
								0.30000001,
								
								{
									"dayzexpansion\vehicles\ground\bus\data\bus_windows_damage.rvmat",
									"DayZExpansion\Vehicles\Ground\Bus\Data\bus_windows_in_damage.rvmat"
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
						"dmgZone_window_1_1"
					};
					componentNames[]=
					{
						"dmgZone_window_1_1"
					};
					fatalInjuryCoef=-1;
					inventorySlots[]={};
				};
				class Window_1_2: Window_1_1
				{
					memoryPoints[]=
					{
						"dmgZone_window_1_2"
					};
					componentNames[]=
					{
						"dmgZone_window_1_2"
					};
				};
				class Window_1_3: Window_1_1
				{
					memoryPoints[]=
					{
						"dmgZone_window_1_3"
					};
					componentNames[]=
					{
						"dmgZone_window_1_3"
					};
				};
				class Window_1_4: Window_1_1
				{
					memoryPoints[]=
					{
						"dmgZone_window_1_4"
					};
					componentNames[]=
					{
						"dmgZone_window_1_4"
					};
				};
				class Window_1_5: Window_1_1
				{
					memoryPoints[]=
					{
						"dmgZone_window_1_5"
					};
					componentNames[]=
					{
						"dmgZone_window_1_5"
					};
				};
				class Window_1_6: Window_1_1
				{
					memoryPoints[]=
					{
						"dmgZone_window_1_6"
					};
					componentNames[]=
					{
						"dmgZone_window_1_6"
					};
				};
				class Window_1_7: Window_1_1
				{
					memoryPoints[]=
					{
						"dmgZone_window_1_7"
					};
					componentNames[]=
					{
						"dmgZone_window_1_7"
					};
				};
				class Window_2_1: Window_1_1
				{
					memoryPoints[]=
					{
						"dmgZone_window_2_1"
					};
					componentNames[]=
					{
						"dmgZone_window_2_1"
					};
				};
				class Window_2_2: Window_1_1
				{
					memoryPoints[]=
					{
						"dmgZone_window_2_2"
					};
					componentNames[]=
					{
						"dmgZone_window_2_2"
					};
				};
				class Window_2_3: Window_1_1
				{
					memoryPoints[]=
					{
						"dmgZone_window_2_3"
					};
					componentNames[]=
					{
						"dmgZone_window_2_3"
					};
				};
				class Window_2_4: Window_1_1
				{
					memoryPoints[]=
					{
						"dmgZone_window_2_4"
					};
					componentNames[]=
					{
						"dmgZone_window_2_4"
					};
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
						"TruckBattery",
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
						"Front"
					};
					transferToZonesCoefs[]={1};
					inventorySlots[]=
					{
						"Reflector_1_1"
					};
					inventorySlotsCoefs[]={1};
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
						"Front"
					};
					inventorySlots[]=
					{
						"Reflector_2_1"
					};
				};
			};
		};
		class ObstacleGenerator
		{
			carve=1;
			timeToStationary=5;
			moveThreshold=0.5;
			class Shapes
			{
				class Cylindric
				{
					class Cyl1
					{
						radius=1.5;
						height=2.5;
						center[]={0,0,3};
					};
					class Cyl2
					{
						radius=1.5;
						height=2.5;
						center[]={0,0,1.5};
					};
					class Cyl3
					{
						radius=1.5;
						height=2.5;
						center[]={0,0,0};
					};
					class Cyl4
					{
						radius=1.5;
						height=2.5;
						center[]={0,0,-1.5};
					};
					class Cyl5
					{
						radius=1.5;
						height=2.5;
						center[]={0,0,-3};
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
					"TruckBattery",
					"CarRadiator",
					"GlowPlug"
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
					"BusWheel_1_1",
					"BusWheel_1_2",
					"BusWheel_2_1",
					"BusWheel_2_2"
				};
			};
		};
	};
	class ExpansionVehicleCarBase;
	class ExpansionWheelBase;
	class Vehicle_ExpansionBusWheel: ExpansionWheelBase
	{
		scope=0;
		model="\DZ\vehicles\wheeled\TransitBus\proxy\ikarusWheel.p3d";
		displayName="$STR_TransitBusWheel0";
		itemSize[]={6,6};
		weight=25000;
		physLayer="item_large";
		rotationFlags=12;
		inventorySlot[]=
		{
			"BusWheel_1_1",
			"BusWheel_2_1"
		};
		radiusByDamage[]={0,0.50999999,0.30000001,0.44999999,0.99980003,0.38,0.99989998,0.30000001};
		radius=0.5;
		friction=0.99900001;
		width=0.213;
		tyreRollResistance=0.015;
		tyreTread=0.80000001;
	};
	class Vehicle_ExpansionBusWheel_Ruined: Vehicle_ExpansionBusWheel
	{
		scope=0;
		model="\DZ\vehicles\wheeled\TransitBus\proxy\ikarusWheel_destroyed.p3d";
		displayName="$STR_TransitBusWheel_Ruined0";
		itemSize[]={6,6};
		weight=25000;
		physLayer="item_large";
		rotationFlags=12;
		inventorySlot[]=
		{
			"BusWheel_1_1",
			"BusWheel_2_1"
		};
		radius=0.30000001;
		friction=-1;
		width=0.213;
	};
	class Vehicle_ExpansionBusWheelDouble: Vehicle_ExpansionBusWheel
	{
		model="\DZ\vehicles\wheeled\TransitBus\proxy\ikarusWheel_rear.p3d";
		displayName="$STR_TransitBusWheelDouble0";
		width=0.426;
		inventorySlot[]=
		{
			"BusWheel_1_2",
			"BusWheel_2_2"
		};
		tyreRollResistance=0.015;
		tyreTread=0.80000001;
	};
	class Vehicle_ExpansionBusWheelDouble_Ruined: Vehicle_ExpansionBusWheelDouble
	{
		model="\DZ\vehicles\wheeled\TransitBus\proxy\ikarusWheel_rear_destroyed.p3d";
		displayName="$STR_TransitBusWheelDouble_Ruined0";
		width=0.426;
		inventorySlot[]=
		{
			"BusWheel_1_2",
			"BusWheel_2_2"
		};
	};
	class Vehicle_ExpansionBus: ExpansionVehicleCarBase
	{
		scope=0;
		displayname="$STR_EXPANSION_VEHICLE_BUS";
		model="\DayZExpansion\Vehicles\Ground\Bus\Ikarus.p3d";
		modelZeroPointDistanceFromGround=0;
		vehicleClass="Expansion_Car";
		attachments[]=
		{
			"TruckBattery",
			"Reflector_1_1",
			"Reflector_2_1",
			"CarRadiator",
			"GlowPlug",
			"BusWheel_1_1",
			"BusWheel_1_2",
			"BusWheel_2_1",
			"BusWheel_2_2"
		};
		doors[]={};
		fuelCapacity=42;
		fuelConsumption=11;
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
			"camo"
		};
		hiddenSelectionsTextures[]=
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
			"DayZExpansion\Vehicles\Ground\Bus\Data\bus_exterior_co.paa"
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
			"DayZExpansion\Vehicles\Ground\Bus\Data\bus_exterior.rvmat"
		};
		defaultSkin="Blue";
		dashboardMatOn="dz\vehicles\wheeled\transitbus\data\ikarus_dashboarde.rvmat";
		dashboardMatOff="dz\vehicles\wheeled\transitbus\data\ikarus_dashboard.rvmat";
		frontReflectorMatOn="dayzexpansion\vehicles\ground\bus\data\bus_exterior_on.rvmat";
		frontReflectorMatOff="dayzexpansion\vehicles\ground\bus\data\bus_exterior.rvmat";
		brakeReflectorMatOn="dayzexpansion\vehicles\ground\bus\data\bus_exterior_on_red.rvmat";
		brakeReflectorMatOff="dayzexpansion\vehicles\ground\bus\data\bus_exterior.rvmat";
		ReverseReflectorMatOn="dayzexpansion\vehicles\ground\bus\data\bus_exterior_on.rvmat";
		ReverseReflectorMatOff="dayzexpansion\vehicles\ground\bus\data\bus_exterior.rvmat";
		TailReflectorMatOn="dayzexpansion\vehicles\ground\bus\data\bus_exterior_on.rvmat";
		TailReflectorMatOff="dayzexpansion\vehicles\ground\bus\data\bus_exterior.rvmat";
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
			class Cargo1
			{
				actionSel="seat_cargo1";
				proxyPos="crewCargo1";
				getInPos="pos_codriver";
				getInDir="pos_codriver_dir";
			};
			class Cargo2
			{
				actionSel="seat_cargo2";
				proxyPos="crewCargo2";
				getInPos="pos_codriver";
				getInDir="pos_codriver_dir";
			};
			class Cargo3
			{
				actionSel="seat_cargo3";
				proxyPos="crewCargo3";
				getInPos="pos_codriver";
				getInDir="pos_codriver_dir";
			};
			class Cargo4
			{
				actionSel="seat_cargo4";
				proxyPos="crewCargo4";
				getInPos="pos_codriver";
				getInDir="pos_codriver_dir";
			};
			class Cargo5
			{
				actionSel="seat_cargo5";
				proxyPos="crewCargo5";
				getInPos="pos_codriver";
				getInDir="pos_codriver_dir";
			};
			class Cargo6
			{
				actionSel="seat_cargo6";
				proxyPos="crewCargo6";
				getInPos="pos_codriver";
				getInDir="pos_codriver_dir";
			};
			class Cargo7
			{
				actionSel="seat_cargo7";
				proxyPos="crewCargo7";
				getInPos="pos_codriver";
				getInDir="pos_codriver_dir";
			};
			class Cargo8
			{
				actionSel="seat_cargo8";
				proxyPos="crewCargo8";
				getInPos="pos_codriver";
				getInDir="pos_codriver_dir";
			};
			class Cargo9
			{
				actionSel="seat_cargo9";
				proxyPos="crewCargo9";
				getInPos="pos_codriver";
				getInDir="pos_codriver_dir";
			};
			class Cargo10
			{
				actionSel="seat_cargo10";
				proxyPos="crewCargo10";
				getInPos="pos_codriver";
				getInDir="pos_codriver_dir";
			};
			class Cargo11
			{
				actionSel="seat_cargo11";
				proxyPos="crewCargo11";
				getInPos="pos_codriver";
				getInDir="pos_codriver_dir";
			};
			class Cargo12
			{
				actionSel="seat_cargo12";
				proxyPos="crewCargo12";
				getInPos="pos_codriver";
				getInDir="pos_codriver_dir";
			};
			class Cargo13
			{
				actionSel="seat_cargo13";
				proxyPos="crewCargo13";
				getInPos="pos_codriver";
				getInDir="pos_codriver_dir";
			};
			class Cargo14
			{
				actionSel="seat_cargo14";
				proxyPos="crewCargo14";
				getInPos="pos_codriver";
				getInDir="pos_codriver_dir";
			};
			class Cargo15
			{
				actionSel="seat_cargo15";
				proxyPos="crewCargo15";
				getInPos="pos_codriver";
				getInDir="pos_codriver_dir";
			};
			class Cargo16
			{
				actionSel="seat_cargo16";
				proxyPos="crewCargo16";
				getInPos="pos_codriver";
				getInDir="pos_codriver_dir";
			};
			class Cargo17
			{
				actionSel="seat_cargo17";
				proxyPos="crewCargo17";
				getInPos="pos_codriver";
				getInDir="pos_codriver_dir";
			};
			class Cargo18
			{
				actionSel="seat_cargo18";
				proxyPos="crewCargo18";
				getInPos="pos_codriver";
				getInDir="pos_codriver_dir";
			};
			class Cargo19
			{
				actionSel="seat_cargo19";
				proxyPos="crewCargo19";
				getInPos="pos_codriver";
				getInDir="pos_codriver_dir";
			};
			class Cargo20
			{
				actionSel="seat_cargo20";
				proxyPos="crewCargo20";
				getInPos="pos_codriver";
				getInDir="pos_codriver_dir";
			};
		};
		class SimulationModule: SimulationModule
		{
			drive="DRIVE_AWD";
			airDragFrontTotal=0.79500002;
			class Steering
			{
				increaseSpeed[]={0,45,60,23,100,12};
				decreaseSpeed[]={0,80,60,40,90,20};
				centeringSpeed[]={0,0,15,25,60,40,100,60};
			};
			class Throttle
			{
				reactionTime=1;
				defaultThrust=0.85000002;
				gentleThrust=0.69999999;
				turboCoef=4;
				gentleCoef=0.75;
			};
			braking[]={0,0.1,1,0.80000001,2.5,0.89999998,3,1};
			class Engine
			{
				inertia=0.15000001;
				steepness=6;
				torqueMax=134;
				torqueRpm=3400;
				powerMax=63.700001;
				powerRpm=5400;
				rpmIdle=850;
				rpmMin=900;
				rpmClutch=1350;
				rpmRedline=6000;
				rpmMax=8000;
			};
			class Gearbox
			{
				reverse=5.526;
				ratios[]={5.6669998,2.5,1.8609999,1.3};
				timeToUncoupleClutch=0.30000001;
				timeToCoupleClutch=0.44999999;
				maxClutchTorque=660;
			};
			class Axles: Axles
			{
				class Front: Front
				{
					maxSteeringAngle=30;
					finalRatio=2.0999999;
					brakeBias=0.40000001;
					brakeForce=10000;
					wheelHubMass=5;
					wheelHubRadius=0.15000001;
					class Suspension
					{
						swayBar=1700;
						stiffness=45000;
						compression=2100;
						damping=8500;
						travelMaxUp=0.088200003;
						travelMaxDown=0.083300002;
					};
					class Wheels: Wheels
					{
						class Left: Left
						{
							inventorySlot="BusWheel_1_1";
							animTurn="turnfrontleft";
							animRotation="wheelfrontleft";
							animDamper="damper_1_1";
							wheelHub="wheel_1_1_damper_land";
						};
						class Right: Right
						{
							inventorySlot="BusWheel_2_1";
							animTurn="turnfrontright";
							animRotation="wheelfrontright";
							animDamper="damper_2_1";
							wheelHub="wheel_2_1_damper_land";
						};
					};
				};
				class Rear: Rear
				{
					maxSteeringAngle=0;
					finalRatio=4.0999999;
					brakeBias=0.60000002;
					brakeForce=12000;
					wheelHubMass=5;
					wheelHubRadius=0.15000001;
					class Suspension
					{
						swayBar=1800;
						stiffness=45000;
						compression=2200;
						damping=8600;
						travelMaxUp=0.1587;
						travelMaxDown=0.1059;
					};
					class Wheels: Wheels
					{
						class Left: Left
						{
							inventorySlot="BusWheel_1_2";
							animTurn="turnbackleft";
							animRotation="wheelbackleft";
							animDamper="damper_1_2";
							wheelHub="wheel_1_2_damper_land";
						};
						class Right: Right
						{
							inventorySlot="BusWheel_2_2";
							animTurn="turnbackright";
							animRotation="wheelbackright";
							animDamper="damper_2_2";
							wheelHub="wheel_2_2_damper_land";
						};
					};
				};
			};
		};
		class Cargo
		{
			itemsCargoSize[]={10,50};
			allowOwnedCargoManipulation=1;
			openable=0;
		};
		class AnimationSources: AnimationSources
		{
			class damper_1_1
			{
				source="user";
				initPhase=0.48570001;
				animPeriod=9.9999997e-06;
			};
			class damper_2_1: damper_1_1
			{
			};
			class damper_1_2
			{
				source="user";
				initPhase=0.40020001;
				animPeriod=9.9999997e-06;
			};
			class damper_2_2: damper_1_2
			{
			};
		};
		class ExpansionSounds
		{
			thrust=0.60000002;
			thrustTurbo=1;
			thrustGentle=0.30000001;
			thrustSmoothCoef=0.02;
			camposSmoothCoef=0.029999999;
			soundSetsFilter[]=
			{
				"offroad_02_Engine_Offload_Ext_Rpm1_SoundSet",
				"offroad_02_Engine_Offload_Ext_Rpm2_SoundSet",
				"offroad_02_Engine_Offload_Ext_Rpm3_SoundSet",
				"offroad_02_Engine_Offload_Ext_Rpm4_SoundSet",
				"offroad_02_Engine_Offload_Ext_Rpm5_SoundSet",
				"offroad_02_Engine_Ext_Rpm0_SoundSet",
				"offroad_02_Engine_Ext_Rpm1_SoundSet",
				"offroad_02_Engine_Ext_Rpm2_SoundSet",
				"offroad_02_Engine_Ext_Rpm3_SoundSet",
				"offroad_02_Engine_Ext_Rpm4_SoundSet",
				"offroad_02_Engine_Ext_Rpm5_SoundSet",
				"offroad_02_Engine_Ext_Broken_SoundSet",
				"offroad_Tires_ice_slow_Ext_SoundSet",
				"offroad_Tires_ice_fast_Ext_SoundSet",
				"offroad_Tires_snow_slow_Ext_SoundSet",
				"offroad_Tires_snow_fast_Ext_SoundSet",
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
				"offroad_skid_dirt_SoundSet",
				"offroad_dirt_turn_SoundSet",
				"offroad_Rain_Ext_SoundSet",
				"offroad_damper_left_SoundSet",
				"offroad_damper_right_SoundSet"
			};
			soundSetsInt[]=
			{
				"Offroad_Tires_Asphalt_Fast_General_Int_SoundSet",
				"Offroad_Wind_SoundSet"
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
					"TruckBattery",
					"CarRadiator",
					"GlowPlug"
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
					"Reflector_2_1"
				};
			};
			class Chassis
			{
				name="$STR_attachment_Chassis0";
				description="";
				icon="cat_vehicle_chassis";
				attachmentSlots[]=
				{
					"BusWheel_1_1",
					"BusWheel_1_2",
					"BusWheel_2_1",
					"BusWheel_2_2"
				};
			};
		};
	};
	class ExpansionSpraycanBase;
	class ExpansionSpraycanBlue: ExpansionSpraycanBase
	{
		scope=2;
		displayName="$STR_EXPANSION_SPRAYCAN_Blue_NAME";
		descriptionShort="$STR_EXPANSION_SPRAYCAN_Blue_DESC";
		hiddenSelectionsTextures[]=
		{
			"\DayZExpansion\Objects\Gear\Spraycans\data\spraycan_blue_co.paa"
		};
		skinName="Blue";
	};
	class ExpansionBus_Blue: ExpansionBus
	{
		skinBase="ExpansionBus";
		skinName="Blue";
	};
	class Vehicle_ExpansionBus_Blue: Vehicle_ExpansionBus
	{
		skinBase="Vehicle_ExpansionBus";
		skinName="Blue";
	};
	class ExpansionSpraycanWeeb: ExpansionSpraycanBase
	{
		scope=2;
		displayName="$STR_EXPANSION_SPRAYCAN_Weeb_NAME";
		descriptionShort="$STR_EXPANSION_SPRAYCAN_Weeb_DESC";
		hiddenSelectionsTextures[]=
		{
			"DZ\gear\consumables\data\spraycan_black_co.paa"
		};
		skinName="Weeb";
	};
	class ExpansionBus_Weeb: ExpansionBus
	{
		skinBase="ExpansionBus";
		skinName="Weeb";
	};
	class Vehicle_ExpansionBus_Weeb: Vehicle_ExpansionBus
	{
		skinBase="Vehicle_ExpansionBus";
		skinName="Weeb";
	};
	class ExpansionSpraycanRed: ExpansionSpraycanBase
	{
		scope=2;
		displayName="$STR_EXPANSION_SPRAYCAN_Red_NAME";
		descriptionShort="$STR_EXPANSION_SPRAYCAN_Red_DESC";
		hiddenSelectionsTextures[]=
		{
			"\DayZExpansion\Objects\Gear\Spraycans\data\spraycan_red_co.paa"
		};
		skinName="Red";
	};
	class ExpansionBus_Red: ExpansionBus
	{
		skinBase="ExpansionBus";
		skinName="Red";
	};
	class Vehicle_ExpansionBus_Red: Vehicle_ExpansionBus
	{
		skinBase="Vehicle_ExpansionBus";
		skinName="Red";
	};
};
class CfgNonAIVehicles
{
	class ProxyVehiclePart;
	class Proxymegaphone: ProxyVehiclePart
	{
		model="\DZ\gear\radio\megaphone.p3d";
		inventorySlot[]=
		{
			"busmegaphone"
		};
	};
};
