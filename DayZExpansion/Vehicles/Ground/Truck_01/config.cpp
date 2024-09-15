class CfgPatches
{
	class DayZExpansion_Vehicles_Ground_Truck_01
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
class CfgVehicles
{
	class ExpansionWheelBase;
	class ExpansionVehicleCarBase;
	class CarDoor;
	class CarScript;
	class Crew;
	class Driver;
	class CoDriver;
	class SimulationModule;
	class Engine;
	class Axles;
	class Front;
	class Rear;
	class Wheels;
	class Left;
	class Right;
	class AnimationSources;
	class GUIInventoryAttachmentsProps;
	class Body;
	class Truck_01_Base;
	class Truck_01_Covered: Truck_01_Base
	{
		doors[]=
		{
			"Truck_01_Door_1_1",
			"Truck_01_Door_2_1",
			"Truck_01_Hood"
		};
		defaultSkin="Green";
		attachments[]+=
		{
			"CamoNet",
			"KeyChain"
		};
		class GUIInventoryAttachmentsProps: GUIInventoryAttachmentsProps
		{
			class Body: Body
			{
				attachmentSlots[]+=
				{
					"CamoNet",
					"KeyChain"
				};
			};
		};
	};
	class Vehicle_Truck_01_Base: ExpansionVehicleCarBase
	{
		scope=0;
		displayName="Truck 01 Base";
		attachments[]=
		{
			"TruckBattery",
			"Reflector_1_1",
			"Reflector_2_1",
			"Truck_01_Door_1_1",
			"Truck_01_Door_2_1",
			"Truck_01_Hood",
			"Truck_01_Wheel_1_1",
			"Truck_01_Wheel_1_2",
			"Truck_01_Wheel_1_3",
			"Truck_01_Wheel_2_1",
			"Truck_01_Wheel_2_2",
			"Truck_01_Wheel_2_3",
			"Truck_01_Wheel_Spare_1",
			"Truck_01_Wheel_Spare_2"
		};
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
			"dmgZone_chassis",
			"dmgZone_front",
			"dmgZone_back",
			"dmgZone_roof",
			"dmgZone_fueltank",
			"dmgZone_fender_1_1",
			"dmgZone_fender_1_2",
			"dmgZone_fender_2_1",
			"dmgZone_fender_2_2",
			"dmgZone_wood"
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
			"#(argb,8,8,3)color(0.521569,0.427451,0.054902,1.0,co)",
			"#(argb,8,8,3)color(0.521569,0.427451,0.054902,1.0,co)",
			"#(argb,8,8,3)color(0.521569,0.427451,0.054902,1.0,co)",
			"#(argb,8,8,3)color(0.521569,0.427451,0.054902,1.0,co)",
			"#(argb,8,8,3)color(0.521569,0.427451,0.054902,1.0,co)",
			"#(argb,8,8,3)color(0.521569,0.427451,0.054902,1.0,co)",
			"#(argb,8,8,3)color(0.521569,0.427451,0.054902,1.0,co)",
			"#(argb,8,8,3)color(0.521569,0.427451,0.054902,1.0,co)",
			"#(argb,8,8,3)color(0.521569,0.427451,0.054902,1.0,co)",
			"#(argb,8,8,3)color(0.521569,0.427451,0.054902,1.0,co)"
		};
		dashboardMatOn="dz\vehicles\wheeled\Truck_01\data\Truck_01_dashboard_light.rvmat";
		dashboardMatOff="dz\vehicles\wheeled\Truck_01\data\Truck_01_dashboard.rvmat";
		frontReflectorMatOn="dz\vehicles\wheeled\Truck_01\data\Truck_01_cab_lights.rvmat";
		frontReflectorMatOff="dz\vehicles\wheeled\Truck_01\data\Truck_01_cab.rvmat";
		brakeReflectorMatOn="dz\vehicles\wheeled\civiliansedan\data\Sedan_TailLights_e.rvmat";
		brakeReflectorMatOff="dz\vehicles\wheeled\civiliansedan\data\Sedan_TailLights.rvmat";
		ReverseReflectorMatOn="dz\vehicles\wheeled\civiliansedan\data\Sedan_TailLights_e.rvmat";
		ReverseReflectorMatOff="dz\vehicles\wheeled\civiliansedan\data\Sedan_TailLights.rvmat";
		TailReflectorMatOn="dz\vehicles\wheeled\civiliansedan\data\Sedan_TailLights_e.rvmat";
		TailReflectorMatOff="dz\vehicles\wheeled\civiliansedan\data\Sedan_TailLights.rvmat";
		fuelCapacity=120;
		fuelConsumption=30;
		class SimulationModule: SimulationModule
		{
			class Steering
			{
				maxSteeringAngle=35;
				increaseSpeed[]={0,25,50,15};
				decreaseSpeed[]={0,50,50,40};
				centeringSpeed[]={0,25,50,15};
			};
			class Throttle
			{
				reactionTime=0.30000001;
				defaultThrust=0.5;
				gentleThrust=0.2;
				turboCoef=1.25;
				gentleCoef=0.5;
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
			drive="DRIVE_642";
			class Engine
			{
				torqueCurve[]={525,0,1000,330,1400,360,1900,340,3000,0};
				inertia=1.15;
				frictionTorque=100;
				rollingFriction=4;
				viscousFriction=2.5;
				rpmIdle=650;
				rpmMin=750;
				rpmClutch=850;
				rpmRedline=2300;
			};
			class Clutch
			{
				maxTorqueTransfer=720;
				uncoupleTime=0.2;
				coupleTime=0.80000001;
			};
			class Gearbox
			{
				type="GEARBOX_MANUAL";
				reverse=6.2800002;
				ratios[]={6.1900001,3.1300001,1.75,1};
			};
			class CentralDifferential
			{
				ratio=0.75;
				type="DIFFERENTIAL_LOCKED";
			};
			class Axles: Axles
			{
				class Front: Front
				{
					maxBrakeTorque=5500;
					wheelHubMass=25;
					wheelHubRadius=0.30000001;
					class Suspension
					{
						stiffness=65000;
						compression=3000;
						damping=7000;
						travelMaxUp=0.14;
						travelMaxDown=0.15000001;
					};
					class Wheels: Wheels
					{
						class Left: Left
						{
							animDamper="damper_1_1";
							inventorySlot="Truck_01_Wheel_1_1";
						};
						class Right: Right
						{
							animDamper="damper_2_1";
							inventorySlot="Truck_01_Wheel_2_1";
						};
					};
				};
				class Middle: Rear
				{
					maxBrakeTorque=5000;
					maxHandbrakeTorque=6000;
					wheelHubMass=45;
					wheelHubRadius=0.30000001;
					class Differential
					{
						ratio=8.3500004;
						type="DIFFERENTIAL_LOCKED";
					};
					class Suspension
					{
						stiffness=55000;
						compression=2500;
						damping=8000;
						travelMaxUp=0.094999999;
						travelMaxDown=0.125;
					};
					class Wheels: Wheels
					{
						class Left
						{
							inventorySlot="Truck_01_Wheel_1_2";
							animTurn="turnmiddleleft";
							animRotation="wheelmiddleleft";
							animDamper="damper_1_2";
							wheelHub="wheel_1_2_damper_land";
						};
						class Right
						{
							inventorySlot="Truck_01_Wheel_2_2";
							animTurn="turnmiddletright";
							animRotation="wheelmiddleright";
							animDamper="damper_2_2";
							wheelHub="wheel_2_2_damper_land";
						};
					};
				};
				class Rear: Rear
				{
					maxBrakeTorque=5000;
					maxHandbrakeTorque=6000;
					wheelHubMass=45;
					wheelHubRadius=0.30000001;
					class Differential
					{
						ratio=8.3500004;
						type="DIFFERENTIAL_LOCKED";
					};
					class Suspension
					{
						stiffness=55000;
						compression=2500;
						damping=8000;
						travelMaxUp=0.094999999;
						travelMaxDown=0.125;
					};
					class Wheels: Wheels
					{
						class Left: Left
						{
							animDamper="damper_1_3";
							inventorySlot="Truck_01_Wheel_1_3";
							wheelHub="wheel_1_3_damper_land";
						};
						class Right: Right
						{
							animDamper="damper_2_3";
							inventorySlot="Truck_01_Wheel_2_3";
							wheelHub="wheel_2_3_damper_land";
						};
					};
				};
			};
		};
		class Cargo
		{
			itemsCargoSize[]={10,40};
			allowOwnedCargoManipulation=1;
			openable=0;
		};
		class AnimationSources: AnimationSources
		{
			class WheelSidePlate1
			{
				source="user";
				initPhase=0;
				animPeriod=1.3;
			};
			class WheelSidePlate2: WheelSidePlate1
			{
			};
			class Damper_1_1
			{
				source="user";
				initPhase=0.48280001;
				animPeriod=1;
			};
			class Damper_2_1: Damper_1_1
			{
			};
			class Damper_1_2
			{
				source="user";
				initPhase=0.43180001;
				animPeriod=1;
			};
			class Damper_2_2: Damper_1_2
			{
			};
			class Damper_1_3: Damper_1_2
			{
			};
			class Damper_2_3: Damper_1_2
			{
			};
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=1500;
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
					fatalInjuryCoef=-1;
					memoryPoints[]={};
					componentNames[]=
					{
						"dmgZone_chassis"
					};
					class Health
					{
						hitpoints=3000;
						transferToGlobalCoef=0;
					};
					transferToZonesNames[]={};
					transferToZonesCoefs[]={};
					inventorySlots[]={};
					inventorySlotsCoefs[]={};
				};
				class Engine
				{
					fatalInjuryCoef=0.001;
					memoryPoints[]=
					{
						"dmgZone_engine"
					};
					componentNames[]=
					{
						"dmgZone_engine"
					};
					class Health
					{
						hitpoints=1500;
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
					transferToZonesNames[]={};
					transferToZonesCoefs[]={};
					inventorySlots[]={};
					inventorySlotsCoefs[]={};
				};
				class Front
				{
					fatalInjuryCoef=-1;
					memoryPoints[]=
					{
						"dmgZone_front"
					};
					componentNames[]=
					{
						"dmgZone_front"
					};
					class Health
					{
						hitpoints=2500;
						transferToGlobalCoef=0;
						healthLevels[]=
						{
							
							{
								1,
								
								{
									"dz\vehicles\wheeled\Truck_01\data\Truck_01_cab.rvmat"
								}
							},
							
							{
								0.69999999,
								
								{
									"dz\vehicles\wheeled\Truck_01\data\Truck_01_cab.rvmat"
								}
							},
							
							{
								0.5,
								
								{
									"dz\vehicles\wheeled\Truck_01\data\Truck_01_cab_damage.rvmat"
								}
							},
							
							{
								0.30000001,
								
								{
									"dz\vehicles\wheeled\Truck_01\data\Truck_01_cab_damage.rvmat"
								}
							},
							
							{
								0,
								
								{
									"dz\vehicles\wheeled\Truck_01\data\Truck_01_cab_destruct.rvmat"
								}
							}
						};
					};
					transferToZonesNames[]=
					{
						"Fender_1_1",
						"Fender_2_1",
						"Engine"
					};
					transferToZonesCoefs[]={0.2,0.2,0.1};
					inventorySlots[]=
					{
						"Truck_01_Hood"
					};
					inventorySlotsCoefs[]={0.1};
				};
				class BackWood
				{
					fatalInjuryCoef=-1;
					memoryPoints[]=
					{
						"dmgZone_wood"
					};
					componentNames[]=
					{
						"dmgZone_wood"
					};
					class Health
					{
						hitpoints=3000;
						transferToGlobalCoef=0;
						healthLevels[]=
						{
							
							{
								1,
								
								{
									"dz\vehicles\wheeled\Truck_01\data\truck_01_cargo_super.rvmat"
								}
							},
							
							{
								0.69999999,
								
								{
									"dz\vehicles\wheeled\Truck_01\data\truck_01_cargo_super.rvmat"
								}
							},
							
							{
								0.5,
								
								{
									"dz\vehicles\wheeled\Truck_01\data\truck_01_cargo_super_damage.rvmat"
								}
							},
							
							{
								0.30000001,
								
								{
									"dz\vehicles\wheeled\Truck_01\data\truck_01_cargo_super_damage.rvmat"
								}
							},
							
							{
								0,
								
								{
									"dz\vehicles\wheeled\Truck_01\data\truck_01_cargo_super_destruct.rvmat"
								}
							}
						};
					};
					transferToZonesNames[]={};
					transferToZonesCoefs[]={};
					inventorySlots[]={};
					inventorySlotsCoefs[]={};
				};
				class BackTarp
				{
					fatalInjuryCoef=-1;
					memoryPoints[]=
					{
						"dmgZone_tarp"
					};
					componentNames[]=
					{
						"dmgZone_tarp"
					};
					class Health
					{
						hitpoints=1000;
						transferToGlobalCoef=0;
						healthLevels[]=
						{
							
							{
								1,
								
								{
									"dz\gear\camping\Data\tent_pristine_co.paa"
								}
							},
							
							{
								0.69999999,
								
								{
									"dz\gear\camping\data\tent_worn_co.paa"
								}
							},
							
							{
								0.5,
								
								{
									"dz\gear\camping\data\tent_damage_co.paa"
								}
							},
							
							{
								0.30000001,
								
								{
									"dz\gear\camping\data\tent_destruct_co.paa"
								}
							},
							
							{
								0,
								
								{
									"dz\gear\camping\data\tent_destruct_co.paa"
								}
							}
						};
					};
					transferToZonesNames[]={};
					transferToZonesCoefs[]={};
					inventorySlots[]={};
					inventorySlotsCoefs[]={};
				};
				class Roof
				{
					fatalInjuryCoef=-1;
					memoryPoints[]=
					{
						"dmgZone_roof"
					};
					componentNames[]=
					{
						"dmgZone_roof"
					};
					class Health
					{
						hitpoints=500;
						transferToGlobalCoef=0;
						healthLevels[]=
						{
							
							{
								1,
								
								{
									"dz\vehicles\wheeled\Truck_01\data\truck_01_cab.rvmat",
									"dz\vehicles\wheeled\Truck_01\data\truck_01_cargo.rvmat"
								}
							},
							
							{
								0.69999999,
								
								{
									"dz\vehicles\wheeled\Truck_01\data\truck_01_cab.rvmat",
									"dz\vehicles\wheeled\Truck_01\data\truck_01_cargo.rvmat"
								}
							},
							
							{
								0.5,
								
								{
									"dz\vehicles\wheeled\Truck_01\data\truck_01_cab_damage.rvmat",
									"dz\vehicles\wheeled\Truck_01\data\truck_01_cargo_damage.rvmat"
								}
							},
							
							{
								0.30000001,
								
								{
									"dz\vehicles\wheeled\Truck_01\data\truck_01_cab_damage.rvmat",
									"dz\vehicles\wheeled\Truck_01\data\truck_01_cargo_damage.rvmat"
								}
							},
							
							{
								0,
								
								{
									"dz\vehicles\wheeled\Truck_01\data\truck_01_cab_destruct.rvmat",
									"dz\vehicles\wheeled\Truck_01\data\truck_01_cargo_destruct.rvmat"
								}
							}
						};
					};
					transferToZonesNames[]={};
					transferToZonesCoefs[]={};
					inventorySlots[]={};
					inventorySlotsCoefs[]={};
				};
				class Fender_1_1
				{
					fatalInjuryCoef=-1;
					memoryPoints[]=
					{
						"dmgZone_fender_1_1"
					};
					componentNames[]=
					{
						"dmgZone_fender_1_1"
					};
					class Health
					{
						hitpoints=800;
						transferToGlobalCoef=0;
						healthLevels[]=
						{
							
							{
								1,
								
								{
									"dz\vehicles\wheeled\Truck_01\data\Truck_01_cab.rvmat"
								}
							},
							
							{
								0.69999999,
								
								{
									"dz\vehicles\wheeled\Truck_01\data\Truck_01_cab.rvmat"
								}
							},
							
							{
								0.5,
								
								{
									"dz\vehicles\wheeled\Truck_01\data\Truck_01_cab_damage.rvmat"
								}
							},
							
							{
								0.30000001,
								
								{
									"dz\vehicles\wheeled\Truck_01\data\Truck_01_cab_damage.rvmat"
								}
							},
							
							{
								0,
								
								{
									"dz\vehicles\wheeled\Truck_01\data\Truck_01_cab_destruct.rvmat"
								}
							}
						};
					};
					transferToZonesNames[]=
					{
						"Front",
						"Engine",
						"Roof"
					};
					transferToZonesCoefs[]={0.1,0.050000001,0.15000001};
					inventorySlots[]=
					{
						"Truck_01_Hood"
					};
					inventorySlotsCoefs[]={0.1};
				};
				class Fender_2_1: Fender_1_1
				{
					memoryPoints[]=
					{
						"dmgZone_fender_2_1"
					};
					componentNames[]=
					{
						"dmgZone_fender_2_1"
					};
				};
				class WindowLeft
				{
					fatalInjuryCoef=-1;
					memoryPoints[]=
					{
						"dmgZone_windowLeft"
					};
					componentNames[]=
					{
						"dmgZone_windowLeft"
					};
					class Health
					{
						hitpoints=50;
						transferToGlobalCoef=0;
						healthLevels[]=
						{
							
							{
								1,
								
								{
									"dz\vehicles\wheeled\Truck_01\data\glass_i.rvmat"
								}
							},
							
							{
								0.69999999,
								{}
							},
							
							{
								0.5,
								
								{
									"dz\vehicles\wheeled\Truck_01\data\glass_i_damage.rvmat"
								}
							},
							
							{
								0.30000001,
								
								{
									"dz\vehicles\wheeled\Truck_01\data\glass_i_destruct.rvmat"
								}
							},
							
							{
								0,
								"hidden"
							}
						};
					};
					transferToZonesNames[]={};
					transferToZonesCoefs[]={};
					inventorySlots[]={};
					inventorySlotsCoefs[]={};
				};
				class WindowRight: WindowLeft
				{
					fatalInjuryCoef=-1;
					memoryPoints[]=
					{
						"dmgZone_windowRight"
					};
					componentNames[]=
					{
						"dmgZone_windowRight"
					};
					inventorySlots[]={};
					inventorySlotsCoefs[]={};
				};
				class WindowBack: WindowLeft
				{
					fatalInjuryCoef=-1;
					memoryPoints[]=
					{
						"dmgZone_windowBack"
					};
					componentNames[]=
					{
						"dmgZone_windowBack"
					};
					inventorySlots[]={};
				};
				class FuelTank
				{
					fatalInjuryCoef=-1;
					memoryPoints[]=
					{
						"dmgZone_fuelTank"
					};
					componentNames[]=
					{
						"dmgZone_fuelTank"
					};
					class Health
					{
						hitpoints=300;
						transferToGlobalCoef=0;
						healthLevels[]=
						{
							
							{
								1,
								
								{
									"dz\vehicles\wheeled\Truck_01\data\truck_01_chassis.rvmat"
								}
							},
							
							{
								0.69999999,
								
								{
									"dz\vehicles\wheeled\Truck_01\data\truck_01_chassis.rvmat"
								}
							},
							
							{
								0.5,
								
								{
									"dz\vehicles\wheeled\Truck_01\data\truck_01_chassis_damage.rvmat"
								}
							},
							
							{
								0.30000001,
								
								{
									"dz\vehicles\wheeled\Truck_01\data\truck_01_chassis_damage.rvmat"
								}
							},
							
							{
								0,
								
								{
									"dz\vehicles\wheeled\Truck_01\data\truck_01_chassis_destruct.rvmat"
								}
							}
						};
					};
					transferToZonesNames[]={};
					transferToZonesCoefs[]={};
					inventorySlots[]={};
					inventorySlotsCoefs[]={};
				};
				class Reflector_1_1
				{
					fatalInjuryCoef=-1;
					memoryPoints[]=
					{
						"dmgZone_lights_1_1"
					};
					componentNames[]=
					{
						"dmgZone_lights_1_1"
					};
					class Health
					{
						hitpoints=50;
						transferToGlobalCoef=0;
						healthLevels[]=
						{
							
							{
								1,
								
								{
									"dz\vehicles\wheeled\Truck_01\data\truck_01_glass.rvmat"
								}
							},
							
							{
								0.69999999,
								{}
							},
							
							{
								0.5,
								
								{
									"dz\vehicles\wheeled\Truck_01\data\truck_01_glass_destruct.rvmat"
								}
							},
							
							{
								0.30000001,
								{}
							},
							
							{
								0,
								
								{
									"dz\vehicles\wheeled\Truck_01\data\truck_01_glass_destruct.rvmat"
								}
							}
						};
					};
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
						radius=1;
						height=1.5;
						center[]={0,0,1.5};
					};
					class Cyl2
					{
						radius=1;
						height=1.5;
						center[]={0,0,0};
					};
					class Cyl3
					{
						radius=1;
						height=1.5;
						center[]={0,0,-1.5};
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
					"TruckBattery"
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
					"Truck_01_Door_1_1",
					"Truck_01_Door_2_1",
					"Truck_01_Hood"
				};
			};
			class Chassis
			{
				name="$STR_attachment_Chassis0";
				description="";
				icon="cat_vehicle_chassis";
				attachmentSlots[]=
				{
					"Truck_01_Wheel_1_1",
					"Truck_01_Wheel_1_2",
					"Truck_01_Wheel_1_3",
					"Truck_01_Wheel_2_1",
					"Truck_01_Wheel_2_2",
					"Truck_01_Wheel_2_3",
					"Truck_01_Wheel_Spare_1",
					"Truck_01_Wheel_Spare_2"
				};
			};
		};
	};
	class Vehicle_Truck_01_Covered: Vehicle_Truck_01_Base
	{
		scope=0;
		displayname="$STR_v3s_covered0";
		model="\dz\vehicles\wheeled\Truck_01\Truck_01_Covered.p3d";
		attachments[]=
		{
			"TruckBattery",
			"Reflector_1_1",
			"Reflector_2_1",
			"Truck_01_Door_1_1",
			"Truck_01_Door_2_1",
			"Truck_01_Hood",
			"Truck_01_Wheel_1_1",
			"Truck_01_Wheel_1_2",
			"Truck_01_Wheel_1_3",
			"Truck_01_Wheel_2_1",
			"Truck_01_Wheel_2_2",
			"Truck_01_Wheel_2_3",
			"Truck_01_Wheel_Spare_1",
			"Truck_01_Wheel_Spare_2",
			"Truck_01_WoodenLogs",
			"Truck_01_WoodenPlanks",
			"Truck_01_MetalSheets",
			"Truck_01_Barrel1",
			"Truck_01_Barrel2",
			"Truck_01_Barrel3",
			"Truck_01_Barrel4",
			"Truck_01_WoodenCrate1",
			"Truck_01_WoodenCrate2",
			"Truck_01_WoodenCrate3",
			"Truck_01_WoodenCrate4"
		};
		class Cargo
		{
			itemsCargoSize[]={10,60};
			allowOwnedCargoManipulation=1;
			openable=0;
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
					"TruckBattery"
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
					"Truck_01_Door_1_1",
					"Truck_01_Door_2_1",
					"Truck_01_Hood"
				};
			};
			class Chassis
			{
				name="$STR_attachment_Chassis0";
				description="";
				icon="cat_vehicle_chassis";
				attachmentSlots[]=
				{
					"Truck_01_Wheel_1_1",
					"Truck_01_Wheel_1_2",
					"Truck_01_Wheel_1_3",
					"Truck_01_Wheel_2_1",
					"Truck_01_Wheel_2_2",
					"Truck_01_Wheel_2_3",
					"Truck_01_Wheel_Spare_1",
					"Truck_01_Wheel_Spare_2"
				};
			};
			class BaseBuildingAttachments
			{
				name="$STR_cfgvehicles_fence_att_category_materials";
				description="";
				icon="cat_common_cargo";
				attachmentSlots[]=
				{
					"Truck_01_WoodenLogs",
					"Truck_01_WoodenPlanks",
					"Truck_01_MetalSheets"
				};
			};
			class BaseBuildingContainers
			{
				name="$STR_attachment_containers0";
				description="";
				icon="cat_common_cargo";
				attachmentSlots[]=
				{
					"Truck_01_Barrel1",
					"Truck_01_Barrel2",
					"Truck_01_Barrel3",
					"Truck_01_Barrel4",
					"Truck_01_WoodenCrate1",
					"Truck_01_WoodenCrate2",
					"Truck_01_WoodenCrate3",
					"Truck_01_WoodenCrate4"
				};
			};
		};
	};
	class Vehicle_Truck_01_Wheel: ExpansionWheelBase
	{
		scope=0;
		displayName="$STR_V3SWheel0";
		descriptionShort="$STR_V3SWheel1";
		model="\DZ\vehicles\wheeled\Truck_01\proxy\Truck_01_Wheel.p3d";
		weight=30000;
		inventorySlot[]=
		{
			"Truck_01_Wheel_1_1",
			"Truck_01_Wheel_2_1",
			"Truck_01_Wheel_Spare_1",
			"Truck_01_Wheel_Spare_2"
		};
		rotationFlags=12;
		radiusByDamage[]={0,0.465,0.30000001,0.38,0.99980003,0.34999999,0.99989998,0.28};
		radius=0.465;
		width=0.2;
		tyreRollResistance=0.07;
		tyreTread=0.94999999;
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
								"DZ\vehicles\wheeled\Truck_01\data\Truck_01_tire.rvmat",
								"DZ\vehicles\wheeled\Truck_01\data\Truck_01_rim.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"DZ\vehicles\wheeled\Truck_01\data\Truck_01_tire.rvmat",
								"DZ\vehicles\wheeled\Truck_01\data\Truck_01_rim.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"DZ\vehicles\wheeled\Truck_01\data\Truck_01_tire_damage.rvmat",
								"DZ\vehicles\wheeled\Truck_01\data\Truck_01_rim_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"DZ\vehicles\wheeled\Truck_01\data\Truck_01_tire_damage.rvmat",
								"DZ\vehicles\wheeled\Truck_01\data\Truck_01_rim_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"DZ\vehicles\wheeled\Truck_01\data\Truck_01_tire_destruct.rvmat",
								"DZ\vehicles\wheeled\Truck_01\data\Truck_01_rim_destruct.rvmat"
							}
						}
					};
				};
			};
		};
	};
	class Vehicle_Truck_01_Wheel_Ruined: Vehicle_Truck_01_Wheel
	{
		scope=0;
		model="\DZ\vehicles\wheeled\Truck_01\proxy\Truck_01_Wheel_ruined.p3d";
		weight=25000;
		tyreRollResistance=1;
		tyreRollDrag=80;
		tyreRoughness=1.35;
		tyreTread=0.44999999;
		radius=0.30000001;
		friction=-1;
		width=0.2;
	};
	class Vehicle_Truck_01_WheelDouble: Vehicle_Truck_01_Wheel
	{
		scope=0;
		displayName="$STR_V3SWheelDouble0";
		descriptionShort="$STR_V3SWheelDouble1";
		model="\DZ\vehicles\wheeled\Truck_01\proxy\Truck_01_WheelDouble.p3d";
		weight=50000;
		inventorySlot[]=
		{
			"Truck_01_Wheel_1_2",
			"Truck_01_Wheel_1_3",
			"Truck_01_Wheel_2_2",
			"Truck_01_Wheel_2_3"
		};
		width=0.40000001;
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
								"DZ\vehicles\wheeled\Truck_01\data\Truck_01_tire.rvmat",
								"DZ\vehicles\wheeled\Truck_01\data\Truck_01_rim.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"DZ\vehicles\wheeled\Truck_01\data\Truck_01_tire.rvmat",
								"DZ\vehicles\wheeled\Truck_01\data\Truck_01_rim.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"DZ\vehicles\wheeled\Truck_01\data\Truck_01_tire_damage.rvmat",
								"DZ\vehicles\wheeled\Truck_01\data\Truck_01_rim_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"DZ\vehicles\wheeled\Truck_01\data\Truck_01_tire_damage.rvmat",
								"DZ\vehicles\wheeled\Truck_01\data\Truck_01_rim_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"DZ\vehicles\wheeled\Truck_01\data\Truck_01_tire_destruct.rvmat",
								"DZ\vehicles\wheeled\Truck_01\data\Truck_01_rim_destruct.rvmat"
							}
						}
					};
				};
			};
		};
	};
	class Vehicle_Truck_01_WheelDouble_Ruined: Vehicle_Truck_01_WheelDouble
	{
		scope=0;
		model="\DZ\vehicles\wheeled\Truck_01\proxy\Truck_01_WheelDouble_ruined.p3d";
		weight=40000;
		tyreRollResistance=1;
		tyreRollDrag=80;
		tyreRoughness=1.35;
		tyreTread=0.44999999;
		radius=0.30000001;
		friction=-1;
	};
	class Truck_01_Door_1_1: CarDoor
	{
		defaultSkin="Green";
	};
	class Truck_01_Door_2_1: Truck_01_Door_1_1
	{
		defaultSkin="Green";
	};
	class Truck_01_Hood: CarDoor
	{
		defaultSkin="Green";
	};
	class Truck_01_Covered_Green: Truck_01_Covered
	{
		skinBase="Truck_01_Covered";
		skinName="Green";
	};
	class Truck_01_Door_1_1_Green: Truck_01_Door_1_1
	{
		skinBase="Truck_01_Door_1_1";
		skinName="Green";
	};
	class Truck_01_Door_2_1_Green: Truck_01_Door_2_1
	{
		skinBase="Truck_01_Door_2_1";
		skinName="Green";
	};
	class Truck_01_Hood_Green: Truck_01_Hood
	{
		skinBase="Truck_01_Hood";
		skinName="Green";
	};
	class Truck_01_Covered_Orange: Truck_01_Covered
	{
		skinBase="Truck_01_Covered";
		skinName="Orange";
	};
	class Truck_01_Door_1_1_Orange: Truck_01_Door_1_1
	{
		skinBase="Truck_01_Door_1_1";
		skinName="Orange";
	};
	class Truck_01_Door_2_1_Orange: Truck_01_Door_2_1
	{
		skinBase="Truck_01_Door_2_1";
		skinName="Orange";
	};
	class Truck_01_Hood_Orange: Truck_01_Hood
	{
		skinBase="Truck_01_Hood";
		skinName="Orange";
	};
	class Truck_01_Covered_Blue: Truck_01_Covered
	{
		skinBase="Truck_01_Covered";
		skinName="Blue";
	};
	class Truck_01_Door_1_1_Blue: Truck_01_Door_1_1
	{
		skinBase="Truck_01_Door_1_1";
		skinName="Blue";
	};
	class Truck_01_Door_2_1_Blue: Truck_01_Door_2_1
	{
		skinBase="Truck_01_Door_2_1";
		skinName="Blue";
	};
	class Truck_01_Hood_Blue: Truck_01_Hood
	{
		skinBase="Truck_01_Hood";
		skinName="Blue";
	};
	class Truck_01_Covered_Red: Truck_01_Covered
	{
		skinBase="Truck_01_Covered";
		skinName="Red";
	};
	class Truck_01_Door_1_1_Red: Truck_01_Door_1_1
	{
		skinBase="Truck_01_Door_1_1";
		skinName="Red";
	};
	class Truck_01_Door_2_1_Red: Truck_01_Door_2_1
	{
		skinBase="Truck_01_Door_2_1";
		skinName="Red";
	};
	class Truck_01_Hood_Red: Truck_01_Hood
	{
		skinBase="Truck_01_Hood";
		skinName="Red";
	};
	class Truck_01_Covered_White: Truck_01_Covered
	{
		skinBase="Truck_01_Covered";
		skinName="White";
	};
	class Truck_01_Door_1_1_White: Truck_01_Door_1_1
	{
		skinBase="Truck_01_Door_1_1";
		skinName="White";
	};
	class Truck_01_Door_2_1_White: Truck_01_Door_2_1
	{
		skinBase="Truck_01_Door_2_1";
		skinName="White";
	};
	class Truck_01_Hood_White: Truck_01_Hood
	{
		skinBase="Truck_01_Hood";
		skinName="White";
	};
	class Truck_01_Covered_Black: Truck_01_Covered
	{
		skinBase="Truck_01_Covered";
		skinName="Black";
	};
	class Truck_01_Door_1_1_Black: Truck_01_Door_1_1
	{
		skinBase="Truck_01_Door_1_1";
		skinName="Black";
	};
	class Truck_01_Door_2_1_Black: Truck_01_Door_2_1
	{
		skinBase="Truck_01_Door_2_1";
		skinName="Black";
	};
	class Truck_01_Hood_Black: Truck_01_Hood
	{
		skinBase="Truck_01_Hood";
		skinName="Black";
	};
	class Truck_01_Covered_Wine: Truck_01_Covered
	{
		skinBase="Truck_01_Covered";
		skinName="Wine";
	};
	class Truck_01_Door_1_1_Wine: Truck_01_Door_1_1
	{
		skinBase="Truck_01_Door_1_1";
		skinName="Wine";
	};
	class Truck_01_Door_2_1_Wine: Truck_01_Door_2_1
	{
		skinBase="Truck_01_Door_2_1";
		skinName="Wine";
	};
	class Truck_01_Hood_Wine: Truck_01_Hood
	{
		skinBase="Truck_01_Hood";
		skinName="Wine";
	};
	class Truck_01_Covered_Lime: Truck_01_Covered
	{
		skinBase="Truck_01_Covered";
		skinName="Lime";
	};
	class Truck_01_Door_1_1_Lime: Truck_01_Door_1_1
	{
		skinBase="Truck_01_Door_1_1";
		skinName="Lime";
	};
	class Truck_01_Door_2_1_Lime: Truck_01_Door_2_1
	{
		skinBase="Truck_01_Door_2_1";
		skinName="Lime";
	};
	class Truck_01_Hood_Lime: Truck_01_Hood
	{
		skinBase="Truck_01_Hood";
		skinName="Lime";
	};
	class Truck_01_Covered_Yellow: Truck_01_Covered
	{
		skinBase="Truck_01_Covered";
		skinName="Yellow";
	};
	class Truck_01_Door_1_1_Yellow: Truck_01_Door_1_1
	{
		skinBase="Truck_01_Door_1_1";
		skinName="Yellow";
	};
	class Truck_01_Door_2_1_Yellow: Truck_01_Door_2_1
	{
		skinBase="Truck_01_Door_2_1";
		skinName="Yellow";
	};
	class Truck_01_Hood_Yellow: Truck_01_Hood
	{
		skinBase="Truck_01_Hood";
		skinName="Yellow";
	};
	class Truck_01_Covered_YellowBright: Truck_01_Covered
	{
		skinBase="Truck_01_Covered";
		skinName="YellowBright";
	};
	class Truck_01_Door_1_1_YellowBright: Truck_01_Door_1_1
	{
		skinBase="Truck_01_Door_1_1";
		skinName="YellowBright";
	};
	class Truck_01_Door_2_1_YellowBright: Truck_01_Door_2_1
	{
		skinBase="Truck_01_Door_2_1";
		skinName="YellowBright";
	};
	class Truck_01_Hood_YellowBright: Truck_01_Hood
	{
		skinBase="Truck_01_Hood";
		skinName="YellowBright";
	};
};
