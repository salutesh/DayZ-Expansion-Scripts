class CfgPatches
{
	class DayZExpansion_Vehicles_Air_HatchBird
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DayZExpansion_Vehicles_Ground_Hatchback"
		};
	};
};
class CfgVehicles
{
	class Inventory_Base;
	class CarDoor;
	class ExpansionWreck;
	class SimulationModule;
	class Axles;
	class Suspension;
	class Front;
	class Rear;
	class AnimationSources;
	class Crew;
	class Driver;
	class CoDriver;
	class ExpansionHelicopterScript;
	class GUIInventoryAttachmentsProps;
	class Engine;
	class Body;
	class Chassis;
	class DamageSystem;
	class DamageZones;
	class GlobalHealth;
	class Window;
	class Health;
	class Doors;
	class OffroadHatchback;
	class ExpansionHatchBirdWreck: ExpansionWreck
	{
		scope=2;
		displayName="If it quacks like a duck...";
		model="DZ\structures\Wrecks\Vehicles\Wreck_hb01_aban1_white";
		class Cargo
		{
			itemsCargoSize[]={10,30};
			allowOwnedCargoManipulation=1;
			openable=0;
		};
	};
	class ExpansionHatchBird: OffroadHatchback
	{
		displayName="If it quacks like a duck...";
		descriptionShort="...";
		modelZeroPointDistanceFromGround=0;
		vehicleClass="Expansion_Helicopter";
		wreck="ExpansionHatchBirdWreck";
		fuelCapacity=192;
		fuelConsumption=91;
		attachments[]=
		{
			"ExpansionHelicopterBattery",
			"Reflector_1_1",
			"Reflector_2_1",
			"ExpansionIgniterPlug",
			"ExpansionHydraulicHoses",
			"NivaDriverDoors",
			"NivaCoDriverDoors",
			"NivaHood",
			"NivaTrunk",
			"NivaWheel_1_1",
			"NivaWheel_1_2",
			"NivaWheel_2_1",
			"NivaWheel_2_2",
			"NivaWheel_Spare_1",
			"CamoNet",
			"KeyChain"
		};
		wheels[]=
		{
			"HatchbackWheel",
			"HatchbackWheel",
			"HatchbackWheel",
			"HatchbackWheel"
		};
		defaultSkin="HatchBird";
		hiddenSelectionsMaterials[]=
		{
			"dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat",
			"dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat",
			"dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat",
			"dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat",
			"dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat",
			"dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat",
			"dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat",
			"dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat",
			"dz\vehicles\wheeled\offroadhatchback\data\offroadhatchback_lights.rvmat",
			"DayZExpansion\Vehicles\Air\HatchBird\data\niva_body.rvmat",
			"DayZExpansion\Vehicles\Air\HatchBird\data\niva_body.rvmat",
			"DayZExpansion\Vehicles\Air\HatchBird\data\niva_body.rvmat",
			"DayZExpansion\Vehicles\Air\HatchBird\data\niva_body.rvmat",
			"DayZExpansion\Vehicles\Air\HatchBird\data\niva_body.rvmat",
			"DayZExpansion\Vehicles\Air\HatchBird\data\niva_body.rvmat",
			"DayZExpansion\Vehicles\Air\HatchBird\data\niva_body.rvmat",
			"DayZExpansion\Vehicles\Air\HatchBird\data\niva_body.rvmat",
			"DayZExpansion\Vehicles\Air\HatchBird\data\niva_body.rvmat",
			"DayZExpansion\Vehicles\Air\HatchBird\data\niva_body.rvmat"
		};
		class GUIInventoryAttachmentsProps: GUIInventoryAttachmentsProps
		{
			class Engine: Engine
			{
				attachmentSlots[]=
				{
					"ExpansionHelicopterBattery",
					"ExpansionIgniterPlug",
					"ExpansionHydraulicHoses"
				};
			};
			class Body: Body
			{
				attachmentSlots[]=
				{
					"Reflector_1_1",
					"Reflector_2_1",
					"NivaHood",
					"NivaTrunk",
					"NivaDriverDoors",
					"NivaCoDriverDoors",
					"NivaWheel_Spare_1",
					"CamoNet",
					"KeyChain"
				};
			};
			class Chassis: Chassis
			{
				attachmentSlots[]=
				{
					"NivaWheel_1_1",
					"NivaWheel_1_2",
					"NivaWheel_2_1",
					"NivaWheel_2_2"
				};
			};
		};
		class Sounds
		{
			thrust=0.60000002;
			thrustTurbo=1;
			thrustGentle=0.30000001;
			thrustSmoothCoef=0.1;
			camposSmoothCoef=0.029999999;
			soundSetsFilter[]=
			{
				"Expansion_HatchBird_Engine_Ext_SoundSet",
				"Expansion_HatchBird_Rotor_Ext_SoundSet"
			};
			soundSetsInt[]=
			{
				"Expansion_HatchBird_Engine_Int_SoundSet",
				"Expansion_HatchBird_Rotor_Int_SoundSet"
			};
		};
		class SimulationModule: SimulationModule
		{
			maxSpeed=220;
			altitudeFullForce=1000;
			altitudeNoForce=2000;
			bodyFrictionCoef=1.1;
			liftForceCoef=1.3;
			bankForceCoef=0.050000001;
			tailForceCoef=2;
			linearFrictionCoef[]={16,0.039999999,0.039999999};
			angularFrictionCoef=1.5;
			class Rotor
			{
				minAutoRotateSpeed=2;
				maxAutoRotateSpeed=10;
				startUpTime=15;
			};
			class AntiTorque
			{
				speed=1.5;
				max=0.16;
			};
			class Cyclic
			{
				forceCoefficient=1.3;
				class Forward
				{
					speed=10;
					max=0.69999999;
					coefficient=1;
					animation="cyclicForward";
				};
				class Side
				{
					speed=10;
					max=0.69999999;
					coefficient=1;
					animation="cyclicAside";
				};
			};
		};
		class DamageSystem: DamageSystem
		{
			class DamageZones: DamageZones
			{
				class Front: Front
				{
					class Health: Health
					{
						healthLevels[]=
						{
							
							{
								1,
								
								{
									"DayZExpansion\Vehicles\Air\HatchBird\data\niva_body.rvmat"
								}
							},
							
							{
								0.69999999,
								
								{
									"DayZExpansion\Vehicles\Air\HatchBird\data\niva_body.rvmat"
								}
							},
							
							{
								0.5,
								
								{
									"DayZExpansion\Vehicles\Air\HatchBird\data\niva_body_damage.rvmat"
								}
							},
							
							{
								0.30000001,
								
								{
									"DayZExpansion\Vehicles\Air\HatchBird\data\niva_body_damage.rvmat"
								}
							},
							
							{
								0,
								
								{
									"DayZExpansion\Vehicles\Air\HatchBird\data\niva_body_destruct.rvmat"
								}
							}
						};
					};
				};
			};
		};
	};
	class ExpansionHatchBirdDoors_Driver: CarDoor
	{
		scope=2;
		displayName="$STR_HatchbackDoors_Driver0";
		descriptionShort="$STR_HatchbackDoors_Driver1";
		model="\DZ\vehicles\wheeled\OffroadHatchback\proxy\nivaDoors_Driver.p3d";
		weight=15000;
		inventorySlot[]=
		{
			"NivaDriverDoors"
		};
		rotationFlags=8;
		hiddenSelectionsMaterials[]=
		{
			"DayZExpansion\Vehicles\Air\HatchBird\data\niva_door.rvmat"
		};
		defaultSkin="HatchBird";
		class DamageSystem: DamageSystem
		{
			class GlobalHealth: GlobalHealth
			{
			};
			class DamageZones: DamageZones
			{
				class Window: Window
				{
					class Health: Health
					{
						healthLevels[]=
						{
							
							{
								1,
								
								{
									"dz\vehicles\wheeled\offroadhatchback\data\niva_glass.rvmat"
								}
							},
							
							{
								0.69999999,
								{}
							},
							
							{
								0.5,
								
								{
									"dz\vehicles\wheeled\offroadhatchback\data\niva_glass_damage.rvmat"
								}
							},
							
							{
								0.30000001,
								
								{
									"dz\vehicles\wheeled\offroadhatchback\data\niva_glass_destruct.rvmat"
								}
							},
							
							{
								0,
								"hidden"
							}
						};
					};
				};
				class Doors: Doors
				{
					class Health: Health
					{
						RefTexsMats[]=
						{
							"DayZExpansion\Vehicles\Air\HatchBird\data\niva_door.rvmat"
						};
						healthLevels[]=
						{
							
							{
								1,
								
								{
									"DayZExpansion\Vehicles\Air\HatchBird\data\niva_door.rvmat"
								}
							},
							
							{
								0.69999999,
								
								{
									"DayZExpansion\Vehicles\Air\HatchBird\data\niva_door.rvmat"
								}
							},
							
							{
								0.5,
								
								{
									"DayZExpansion\Vehicles\Air\HatchBird\data\niva_door_damage.rvmat"
								}
							},
							
							{
								0.30000001,
								
								{
									"DayZExpansion\Vehicles\Air\HatchBird\data\niva_door_damage.rvmat"
								}
							},
							
							{
								0,
								
								{
									"DayZExpansion\Vehicles\Air\HatchBird\data\niva_door_destruct.rvmat"
								}
							}
						};
					};
				};
			};
		};
	};
	class ExpansionHatchBirdDoors_CoDriver: ExpansionHatchBirdDoors_Driver
	{
		displayName="$STR_HatchbackDoors_CoDriver0";
		descriptionShort="$STR_HatchbackDoors_CoDriver1";
		model="\DZ\vehicles\wheeled\OffroadHatchback\proxy\nivaDoors_coDriver.p3d";
		inventorySlot[]=
		{
			"NivaCoDriverDoors"
		};
		rotationFlags=4;
	};
	class ExpansionHatchBirdHood: CarDoor
	{
		scope=2;
		displayName="$STR_HatchbackHood0";
		descriptionShort="$STR_HatchbackHood1";
		model="\DZ\vehicles\wheeled\OffroadHatchback\proxy\nivaDoors_Hood.p3d";
		weight=15000;
		inventorySlot[]=
		{
			"NivaHood"
		};
		rotationFlags=2;
		hiddenSelectionsMaterials[]=
		{
			"DayZExpansion\Vehicles\Air\HatchBird\data\niva_hood.rvmat"
		};
		defaultSkin="HatchBird";
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=500;
					RefTexsMats[]=
					{
						"DayZExpansion\Vehicles\Air\HatchBird\data\niva_hood.rvmat"
					};
					healthLevels[]=
					{
						
						{
							1,
							
							{
								"DayZExpansion\Vehicles\Air\HatchBird\data\niva_hood.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"DayZExpansion\Vehicles\Air\HatchBird\data\niva_hood.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"DayZExpansion\Vehicles\Air\HatchBird\data\niva_hood_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"DayZExpansion\Vehicles\Air\HatchBird\data\niva_hood_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"DayZExpansion\Vehicles\Air\HatchBird\data\niva_hood_destruct.rvmat"
							}
						}
					};
				};
			};
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class pickUp
				{
					soundSet="hatchbackdoors_driver_pickup_SoundSet";
					id=797;
				};
				class drop
				{
					soundset="hatchbackhood_drop_SoundSet";
					id=898;
				};
			};
		};
	};
	class ExpansionHatchBirdTrunk: CarDoor
	{
		scope=2;
		displayName="$STR_HatchbackTrunk0";
		descriptionShort="$STR_HatchbackTrunk1";
		model="\DZ\vehicles\wheeled\OffroadHatchback\proxy\nivaDoors_Trunk.p3d";
		weight=15000;
		inventorySlot[]=
		{
			"NivaTrunk"
		};
		rotationFlags=1;
		hiddenSelectionsMaterials[]=
		{
			"DayZExpansion\Vehicles\Air\HatchBird\data\niva_trunk.rvmat"
		};
		defaultSkin="HatchBird";
		class DamageSystem: DamageSystem
		{
			class DamageZones: DamageZones
			{
				class Window: Window
				{
					class Health: Health
					{
						healthLevels[]=
						{
							
							{
								1,
								
								{
									"dz\vehicles\wheeled\offroadhatchback\data\niva_glass.rvmat"
								}
							},
							
							{
								0.69999999,
								{}
							},
							
							{
								0.5,
								
								{
									"dz\vehicles\wheeled\offroadhatchback\data\niva_glass_damage.rvmat"
								}
							},
							
							{
								0.30000001,
								
								{
									"dz\vehicles\wheeled\offroadhatchback\data\niva_glass_destruct.rvmat"
								}
							},
							
							{
								0,
								"hidden"
							}
						};
					};
				};
				class Doors: Doors
				{
					class Health: Health
					{
						RefTexsMats[]=
						{
							"DayZExpansion\Vehicles\Air\HatchBird\data\niva_trunk.rvmat"
						};
						healthLevels[]=
						{
							
							{
								1,
								
								{
									"DayZExpansion\Vehicles\Air\HatchBird\data\niva_trunk.rvmat"
								}
							},
							
							{
								0.69999999,
								
								{
									"DayZExpansion\Vehicles\Air\HatchBird\data\niva_trunk.rvmat"
								}
							},
							
							{
								0.5,
								
								{
									"DayZExpansion\Vehicles\Air\HatchBird\data\niva_trunk_damage.rvmat"
								}
							},
							
							{
								0.30000001,
								
								{
									"DayZExpansion\Vehicles\Air\HatchBird\data\niva_trunk_damage.rvmat"
								}
							},
							
							{
								0,
								
								{
									"DayZExpansion\Vehicles\Air\HatchBird\data\niva_trunk_destruct.rvmat"
								}
							}
						};
					};
				};
			};
		};
	};
};
