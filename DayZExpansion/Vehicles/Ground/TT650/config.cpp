class CfgPatches
{
	class DayZExpansion_Vehicles_Ground_TT650
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
	class Slot_expansion_tt650_wheel_1
	{
		name="expansion_tt650_wheel_1";
		displayName="$STR_EXPANSION_VEHICLE_TT650_WHEEL";
		selection="wheel_1";
		ghostIcon="wheel";
	};
	class Slot_expansion_tt650_wheel_2
	{
		name="expansion_tt650_wheel_2";
		displayName="$STR_EXPANSION_VEHICLE_TT650_WHEEL";
		selection="wheel_2";
		ghostIcon="wheel";
	};
};
class CfgVehicles
{
	class ExpansionWheelBase;
	class Vehicle_ExpansionTT650Wheel: ExpansionWheelBase
	{
		scope=0;
		displayName="$STR_EXPANSION_VEHICLE_TT650_WHEEL";
		descriptionShort="$STR_EXPANSION_VEHICLE_TT650_WHEEL_DESC";
		model="\DayZExpansion\Vehicles\Ground\TT650\proxy\expansion_tt650_wheel.p3d";
		weight=500;
		inventorySlot[]=
		{
			"expansion_tt650_wheel_1",
			"expansion_tt650_wheel_2"
		};
		mass=80;
		radius=0.333;
		width=0.31600001;
		tyreRollResistance=0.015;
		tyreTread=1;
	};
	class ExpansionVehicleBikeBase;
	class Vehicle_ExpansionTT650: ExpansionVehicleBikeBase
	{
		scope=0;
		displayname="$STR_EXPANSION_VEHICLE_TT650";
		model="\DayZExpansion\Vehicles\Ground\TT650\tt650.p3d";
		attachments[]=
		{
			"CarBattery",
			"Reflector_1_1",
			"Reflector_2_1",
			"CarRadiator",
			"GlowPlug",
			"expansion_tt650_wheel_1",
			"expansion_tt650_wheel_2"
		};
		soundController="";
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
					"expansion_tt650_wheel_1",
					"expansion_tt650_wheel_2"
				};
			};
		};
		class Crew
		{
			class Driver
			{
				isDriver=1;
				actionSel="seat_driver";
				proxyPos="crewDriver";
				getInPos="pos_driver";
				getInDir="pos_driver_dir";
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
						hitpoints=1000;
						transferToGlobalCoef=1;
						healthLevels[]=
						{
							
							{
								1,
								
								{
									"dz\vehicles\wheeled\offroadhatchback\data\engine_niva.rvmat"
								}
							},
							
							{
								0.69999999,
								
								{
									"dz\vehicles\wheeled\offroadhatchback\data\engine_niva.rvmat"
								}
							},
							
							{
								0.5,
								
								{
									"dz\vehicles\wheeled\offroadhatchback\data\engine_niva_destruct.rvmat"
								}
							},
							
							{
								0.30000001,
								
								{
									"dz\vehicles\wheeled\offroadhatchback\data\engine_niva_destruct.rvmat"
								}
							},
							
							{
								0,
								
								{
									"dz\vehicles\wheeled\offroadhatchback\data\engine_niva_destruct.rvmat"
								}
							}
						};
					};
					inventorySlots[]=
					{
						"GlowPlug",
						"CarRadiator"
					};
					inventorySlotsCoefs[]={0.2,0.2,0.40000001};
				};
				class FuelTank
				{
					fatalInjuryCoef=-1;
					componentNames[]=
					{
						"dmgZone_fuelTank"
					};
					class Health
					{
						hitpoints=600;
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
					inventorySlots[]={};
					inventorySlotsCoefs[]={};
				};
			};
		};
		class SimulationModule
		{
			class Steering
			{
				increaseSpeed[]={8};
				decreaseSpeed[]={70};
				centeringSpeed[]={50};
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
				inertia=0.44999999;
				torqueMax=187;
				torqueRpm=2400;
				powerMax=73.5;
				powerRpm=4500;
				rpmIdle=1000;
				rpmMin=1050;
				rpmClutch=1250;
				rpmRedline=4700;
				rpmMax=6800;
			};
			class Gearbox
			{
				reverse=3.51;
				ratios[]={3.5,2.26,1.45,1};
				timeToUncoupleClutch=0.30000001;
				timeToCoupleClutch=0.30000001;
				maxClutchTorque=180;
			};
			class Axles
			{
				class Front
				{
					maxSteeringAngle=25;
					finalRatio=0;
					brakeBias=0.69999999;
					brakeForce=4000;
					wheelHubMass=10;
					wheelHubRadius=0.15000001;
					class Suspension
					{
						swayBar=0;
						stiffness=8200;
						compression=2150;
						damping=3400;
						travelMaxUp=0.16;
						travelMaxDown=0.16;
					};
					class Wheels
					{
						class Center
						{
							inventorySlot="expansion_tt650_wheel_1";
							animTurn="turnfront";
							animRotation="wheelfront";
							animDamper="damper_1";
							wheelHub="wheel_1_damper_land";
							axis_start="suspension_1_start";
							axis_end="suspension_1_end";
						};
					};
				};
				class Rear
				{
					maxSteeringAngle=0;
					finalRatio=3.3180001;
					brakeBias=0.69999999;
					brakeForce=4000;
					wheelHubMass=10;
					wheelHubRadius=0.15000001;
					class Suspension
					{
						swayBar=0;
						stiffness=8000;
						compression=2100;
						damping=3300;
						travelMaxUp=0.16;
						travelMaxDown=0.16;
					};
					class Wheels
					{
						class Center
						{
							inventorySlot="expansion_tt650_wheel_2";
							animTurn="";
							animRotation="wheelback";
							animDamper="damper_2";
							wheelHub="wheel_2_damper_land";
						};
					};
				};
			};
		};
		class AnimationSources
		{
			class Damper_1
			{
				source="user";
				initPhase=0;
				animPeriod=1e-06;
			};
			class Damper_2: Damper_1
			{
			};
			class wheelfront
			{
				source="user";
				initPhase=0;
				animPeriod=1e-06;
			};
			class wheelback: wheelfront
			{
			};
			class steeringwheel
			{
				source="user";
				initPhase=0;
				animPeriod=1e-06;
			};
		};
	};
};
class CfgNonAIVehicles
{
	class ProxyAttachment;
	class ProxyVehiclePart: ProxyAttachment
	{
		scope=2;
		simulation="ProxyInventory";
		autocenter=0;
		animated=0;
		shadow=1;
		reversed=0;
	};
	class Proxyexpansion_tt650_wheel: ProxyVehiclePart
	{
		model="DayZExpansion\Vehicles\Ground\TT650\proxy\expansion_tt650_wheel.p3d";
		inventorySlot[]=
		{
			"expansion_tt650_wheel_1",
			"expansion_tt650_wheel_2"
		};
	};
};
