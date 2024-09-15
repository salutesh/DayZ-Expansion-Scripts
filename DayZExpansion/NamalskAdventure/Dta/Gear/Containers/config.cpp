class CfgPatches
{
	class DayZExpansion_NamalskAdventure_Gear_Containers
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data",
			"DZ_Characters",
			"DZ_Gear_Containers"
		};
	};
};
class CfgVehicles
{
	class Container_Base;
	class SmallProtectorCase;
	class Inventory_Base;
	class ExpansionAnomalyCoreProtectiveCase: SmallProtectorCase
	{
		displayName="EVR Protection Case";
		descriptionShort="This high-tech reinforced, weather-resistant container was developed by a team of scientists to withstand even the harshest conditions on the island. This case is the ultimate solution for protecting your valuable loot during EVR storms.";
		itemSize[]={3,4};
		storageCategory=10;
		hiddenSelections[]=
		{
			"zbytek"
		};
		hiddenSelectionsTextures[]=
		{
			"\DayZExpansion\NamalskAdventure\Dta\Gear\Containers\data\expansion_anomalycase_gray_co.paa"
		};
	};
	class ExpansionAnomalyDeconCase: Container_Base
	{
		scope=2;
		displayName="Anomaly Decon Case";
		descriptionShort="This high-tech reinforced, weather-resistant container was developed by a team of scientists to withstand even the harshest conditions on the island. This case is the ultimate solution for protecting your valuable loot during EVR storms or protect you from irradiated objects.";
		model="DZ\structures\Residential\Medical\Decon_CBRNCase_01_F.p3d";
		weight=2250;
		itemSize[]={5,4};
		itemsCargoSize[]={5,4};
		canBeDigged=1;
		allowOwnedCargoManipulation=1;
		randomQuantity=4;
		repairableWithKits[]={8};
		repairCosts[]={25};
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
							
							{
								"DZ\gear\containers\data\300RoundBox.rvmat"
							}
						},
						
						{
							0.69999999,
							
							{
								"DZ\gear\containers\data\300RoundBox.rvmat"
							}
						},
						
						{
							0.5,
							
							{
								"DZ\gear\containers\data\300RoundBox_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							
							{
								"DZ\gear\containers\data\300RoundBox_damage.rvmat"
							}
						},
						
						{
							0,
							
							{
								"DZ\gear\containers\data\300RoundBox_destruct.rvmat"
							}
						}
					};
				};
			};
		};
		soundImpactType="metal";
		class AnimEvents
		{
			class SoundWeapon
			{
				class pickup
				{
					soundSet="sewingkit_pickup_SoundSet";
					id=797;
				};
				class drop
				{
					soundset="ammobox_drop_SoundSet";
					id=898;
				};
			};
		};
	};
	class Expansion_Halloween_GiftBox_Small: Inventory_Base
	{
		scope=2;
		model="\DZ\gear\containers\ChristmasBox1.p3d";
		displayName="Halloween Gift";
		descriptionShort="A small treat from the Namalsk Adventure Team.";
		weight=100;
		itemSize[]={2,2};
		hiddenSelections[]=
		{
			"ChristmasBox1",
			"ChristmasBox1_ribbon"
		};
		hiddenSelectionsTextures[]=
		{
			"\DayZExpansion\NamalskAdventure\Dta\Gear\Containers\data\halloween_1_co.paa",
			"dz\gear\containers\data\christmas_paper_ribbon_CO.paa"
		};
		hiddenSelectionsMaterials[]=
		{
			"DZ\gear\containers\data\christmas_paper1.rvmat",
			"DZ\gear\containers\data\christmas_ribbon_GOLD.rvmat"
		};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints=30;
					healthLevels[]=
					{
						
						{
							1,
							
							{
								"DZ\gear\containers\data\christmas_paper1.rvmat",
								"DZ\gear\containers\data\christmas_ribbon_GOLD.rvmat"
							}
						},
						
						{
							0.69999999,
							{}
						},
						
						{
							0.5,
							
							{
								"DZ\gear\containers\data\christmas_paper1_damage.rvmat",
								"DZ\gear\containers\data\christmas_ribbon_GOLD_damage.rvmat"
							}
						},
						
						{
							0.30000001,
							{}
						},
						
						{
							0,
							
							{
								"DZ\gear\containers\data\christmas_paper1_destruct.rvmat",
								"DZ\gear\containers\data\christmas_ribbon_GOLD_destruct.rvmat"
							}
						}
					};
				};
			};
		};
	};
};
