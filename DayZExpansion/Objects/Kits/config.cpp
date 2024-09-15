class CfgPatches
{
	class DayZExpansion_Objects_Kits
	{
		units[]=
		{
			"ExpansionParkingMeterKit",
			"ExpansionParkingMeterKitPlacing"
		};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data"
		};
	};
};
class CfgVehicles
{
	class Inventory_Base;
	class ExpansionParkingMeter;
	class ExpansionConstructionKitBase: Inventory_Base
	{
		scope=1;
		displayName="Expansion Construction Kit Base";
		descriptionShort="Expansion Construction Kit Base";
		hiddenSelections[]=
		{
			"camo"
		};
		hiddenSelectionsTextures[]=
		{
			"DayZExpansion\Objects\Kits\data\box_co.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"DayZExpansion\Objects\Kits\data\box.rvmat"
		};
		rotationFlags=2;
		weight=3000;
		itemSize[]={5,5};
		itemBehaviour=1;
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
				class GlobalArmor
				{
					class Projectile
					{
						class Health
						{
							damage=0;
						};
						class Blood
						{
							damage=0;
						};
						class Shock
						{
							damage=0;
						};
					};
					class FragGrenade
					{
						class Health
						{
							damage=0;
						};
						class Blood
						{
							damage=0;
						};
						class Shock
						{
							damage=0;
						};
					};
				};
			};
		};
		class AnimationSources
		{
			class AnimSourceShown
			{
				source="user";
				animPeriod=0.0099999998;
				initPhase=0;
			};
			class AnimSourceHidden
			{
				source="user";
				animPeriod=0.0099999998;
				initPhase=1;
			};
			class Inventory: AnimSourceHidden
			{
			};
			class Placing: AnimSourceHidden
			{
			};
		};
		soundImpactType="wood";
	};
	class ExpansionConstructionKitLarge: ExpansionConstructionKitBase
	{
		scope=1;
		displayName="Construction-Kit Large";
		model="DayZExpansion\Objects\Kits\box_large.p3d";
		hiddenSelections[]=
		{
			"camo"
		};
		hiddenSelectionsTextures[]=
		{
			"DayZExpansion\Objects\Kits\data\box_red_co.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"DayZExpansion\Objects\Kits\data\box.rvmat"
		};
	};
	class ExpansionConstructionKitSmall: ExpansionConstructionKitBase
	{
		scope=1;
		displayName="Construction-Kit Large";
		model="DayZExpansion\Objects\Kits\box_small.p3d";
		hiddenSelections[]=
		{
			"camo"
		};
		hiddenSelectionsTextures[]=
		{
			"DayZExpansion\Objects\Kits\data\box_co.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"DayZExpansion\Objects\Kits\data\box.rvmat"
		};
	};
	class ExpansionParkingMeterKit: ExpansionConstructionKitLarge
	{
		scope=2;
		displayName="Parking Meter Construction-Kit";
		descriptionShort="A construction kit to deploy a access point for the Virtual Garage.";
	};
	class ExpansionParkingMeterKitPlacing: ExpansionParkingMeter
	{
		scope=2;
		displayName="This is a hologram";
		descriptionShort="Nothing to see here, move along";
		model="DayZExpansion\Objects\Misc\ParkingMeter.p3d";
		storageCategory=10;
		hiddenSelections[]=
		{
			"camo"
		};
		hiddenSelectionsTextures[]=
		{
			"#(argb,8,8,3)color(0.5,0.5,0.5,0.75,ca)"
		};
	};
};
