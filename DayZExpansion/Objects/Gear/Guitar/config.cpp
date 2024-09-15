class CfgPatches
{
	class DayZExpansion_Objects_Gear_Guitar
	{
		units[]=
		{
			"Expansion_Guitar",
			"Expansion_Guitar_old"
		};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Weapons_Melee"
		};
	};
};
class CfgVehicles
{
	class Inventory_Base;
	class Expansion_Guitar: Inventory_Base
	{
		scope=2;
		displayName="Classic Guitar";
		descriptionShort="A Classic Guitar Signed";
		model="DayZExpansion\Objects\Gear\Guitar\Classic_Guitar.p3d";
		rotationFlags=17;
		openable=0;
		weight=1500;
		itemSize[]={3,6};
		itemsCargoSize[]={2,2};
		fragility=0.0099999998;
		allowOwnedCargoManipulation=1;
		absorbency=0.80000001;
		lootCategory="Crafted";
		attachments[]=
		{
			"MetalWire"
		};
		energyResources[]=
		{
			
			{
				"power",
				0.1
			}
		};
		inventorySlot[]=
		{
			"Shoulder",
			"Melee"
		};
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
								"DayZExpansion\Objects\Gear\Guitar\data\guitar_z.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"DayZExpansion\Objects\Gear\Guitar\data\guitar_z.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"DayZExpansion\Objects\Gear\Guitar\data\guitar_z_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"DayZExpansion\Objects\Gear\Guitar\data\guitar_z_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"DayZExpansion\Objects\Gear\Guitar\data\guitar_z_destruct.rvmat"
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
				class PlayGuitar_SoundSet
				{
					soundSet="PlayGuitar_SoundSet";
					id=206;
				};
			};
		};
		class AnimationSources
		{
			class AnimateGuitar
			{
				source="user";
				initPhase=0;
				animPeriod=1;
			};
		};
	};
	class Expansion_Guitar_old: Expansion_Guitar
	{
		scope=2;
		displayName="Classic Old Guitar";
		descriptionShort="A Classic Old Guitar ";
		model="DayZExpansion\Objects\Gear\Guitar\Classic_Guitar_old.p3d";
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
								"DayZExpansion\Objects\Gear\Guitar\data\old_guitar_z.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"DayZExpansion\Objects\Gear\Guitar\data\old_guitar_z.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"DayZExpansion\Objects\Gear\Guitar\data\old_guitar_z_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"DayZExpansion\Objects\Gear\Guitar\data\old_guitar_z_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"DayZExpansion\Objects\Gear\Guitar\data\old_guitar_z_destruct.rvmat"
							}
						}
					};
				};
			};
		};
	};
};
