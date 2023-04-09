#define _ARMA_

class CfgPatches
{
	class DayZExpansion_NamalskAdventure_Items
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
	};
};
class CfgSlots
{
	class Slot_Att_ExpansionAnomalyCore
	{
		name = "Att_ExpansionAnomalyCore";
		displayName = "Anomaly Core";
		selection = "att_anomaly_core";
	};
};
class CfgVehicles
{
	class Inventory_Base;
	class CigarettePack_Chernamorka;
	class CigarettePack_Merkur;
	class CigarettePack_Partyzanka;
	class Expansion_CigarettePack_Chernamorka: CigarettePack_Chernamorka
	{
		scope = 2;
		displayName = "Cigarette Pack - Chernamorka";
		descriptionShort = "This pack of cigarettes bears the name Chernamorka, and features a design reminiscent of Soviet-era packaging. The brand is known for its strong, unfiltered cigarettes that are said to have a distinctive and bold flavor. The pack contains 20 cigarettes, each one wrapped in brown paper with a simple red logo stamped on it. Despite their popularity among some smokers, these cigarettes are known to be harsh on the throat and lungs, and are not recommended for those who are concerned about their health.";
		canBeSplit = 1;
		count = 20;
	};
	class Expansion_CigarettePack_Merkur: CigarettePack_Merkur
	{
		scope = 2;
		displayName = "Cigarette Pack - Chernamorka";
		descriptionShort = "Merkur cigarettes come in a sleek and stylish package with a white color. The logo features a golden crescent star with the brand name in back letters. The cigarettes themselves have a smooth and mild flavor, perfect for a relaxing break in the midst of a stressful and dangerous world. Each pack contains 20 cigarettes.";
		canBeSplit = 1;
		count = 20;
	};
	class Expansion_CigarettePack_Partyzanka: CigarettePack_Partyzanka
	{
		scope = 2;
		displayName = "Cigarette Pack - Partyzanka";
		descriptionShort = "This pack of cigarettes has a distinct aroma of sweet vanilla and a hint of nutmeg. It is said to be a popular brand among rebels and revolutionaries due to its association with the Partisans. Partyzanka cigarettes are known for their smooth and mellow taste, making them a preferred choice for those who enjoy a relaxed smoking experience.";
		canBeSplit = 1;
		count = 20;
	};
	class HumanSteakMeat;
	class InfectedSteakMeat: HumanSteakMeat
	{
		scope = 2;
		displayName = "Infected Flesh";
		descriptionShort = "This piece of meat has been torn from the flesh of an infected. It's rotten and not fit for human consumption, but some desperate survivors might try to eat it anyway.";
	};
	class Grenade_Base;
	class Expansion_AnomalyCore_Base: Grenade_Base
	{
		scope = 1;
		model = "\DayZExpansion\NamalskAdventure\Dta\Items\Anomaly_Core\Anomaly_Core.p3d";
		hiddenSelections[] = {"steel","sun"};
		hiddenSelectionsTextures[] = {"\DayZExpansion\NamalskAdventure\Dta\Items\Anomaly_Core\data\anomaly_core_co.paa","\DayZExpansion\NamalskAdventure\Dta\Items\Anomaly_Core\data\anomaly_core_energy_co.paa"};
		hiddenSelectionsMaterials[] = {"\DayZExpansion\NamalskAdventure\Dta\Items\data\Expansion_Anomaly_Core.rvmat","\DayZExpansion\NamalskAdventure\Dta\Items\data\Expansion_Anomaly_Core_Energy.rvmat"};
		weight = 500;
		itemSize[] = {1,1};
		inventorySlot[] = {"IEDExplosiveA","IEDExplosiveB","Att_ExpansionAnomalyCore"};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 5;
					healthLevels[] = {{1,{"\DayZExpansion\NamalskAdventure\Dta\Items\data\Anomaly_Core\data\Expansion_Anomaly_Core.rvmat"}},{0.7,{"\DayZExpansion\NamalskAdventure\Dta\Items\data\Anomaly_Core\data\Expansion_Anomaly_Core.rvmat"}},{0.5,{"\DayZExpansion\NamalskAdventure\Dta\Items\data\Anomaly_Core\data\Expansion_Anomaly_Core.rvmat"}},{0.3,{"\DayZExpansion\NamalskAdventure\Dta\Items\data\Anomaly_Core\data\Expansion_Anomaly_Core.rvmat"}},{0,{"\DayZExpansion\NamalskAdventure\Dta\Items\data\Anomaly_Core\data\Expansion_Anomaly_Core.rvmat"}}};
				};
			};
			class AnimEvents
			{
				class SoundWeapon
				{
					class pickUpItem
					{
						soundSet = "pickUpPaper_SoundSet";
						id = 797;
					};
				};
			};
		};
		class AnimationSources
		{
			class AnimSourceShown
			{
				source = "sun";
				animPeriod = 0.009999999;
				initPhase = 1;
			};
			class AnimSourceHidden
			{
				source = "sun";
				animPeriod = 0.009999999;
				initPhase = 0;
			};
			class AnimRotate
			{
				source = "sun";
				animPeriod = 0.018;
				initPhase = 0;
			};
			class Sun_Rotation: AnimRotate{};
			class Sun_Hidden: AnimSourceHidden{};
			class Sun_Shown: AnimSourceShown{};
		};
	};
	class Expansion_AnomalyCore_Ice: Expansion_AnomalyCore_Base
	{
		scope = 2;
		displayName = "Permafrost Gem";
		descriptionShort = "The Permafrost Gem is a rare and valuable artifact recovered from a frozen anomaly on Namalsk. It emits an eerie, otherworldly energy that is both captivating and chilling. Handle with care.";
		hiddenSelectionsTextures[] = {"\DayZExpansion\NamalskAdventure\Dta\Items\Anomaly_Core\data\anomaly_core_ice_co.paa","\DayZExpansion\NamalskAdventure\Dta\Items\Anomaly_Core\data\anomaly_core_energy_co.paa"};
	};
	class Expansion_AnomalyCore_Warper: Expansion_AnomalyCore_Base
	{
		scope = 2;
		displayName = "Warper Plasma";
		descriptionShort = "A mysterious device that emits an otherworldly energy signature. It is believed to be a key component of the teleport anomalies found on Namalsk.";
		hiddenSelectionsTextures[] = {"\DayZExpansion\NamalskAdventure\Dta\Items\Anomaly_Core\data\anomaly_core_co.paa","\DayZExpansion\NamalskAdventure\Dta\Items\Anomaly_Core\data\anomaly_core_energy_warper_co.paa"};
		hiddenSelectionsMaterials[] = {"\DayZExpansion\NamalskAdventure\Dta\Items\data\Expansion_Anomaly_Core.rvmat","\DayZExpansion\NamalskAdventure\Dta\Items\data\Expansion_Anomaly_Core_Warper_Energy.rvmat"};
	};
	class EffectArea;
	class ExpansionAnomalyAreaBase_Dynamic: EffectArea
	{
		scope = 2;
	};
	class ExpansionAnomalyAreaSingularity_Local: ExpansionAnomalyAreaBase_Dynamic
	{
		scope = 2;
	};
	class ExpansionAnomalyAreaWarper_Local: ExpansionAnomalyAreaBase_Dynamic
	{
		scope = 2;
	};
	class Expansion_KeyCard_Base: Inventory_Base
	{
		scope = 1;
		itemSize[] = {1,1};
		weight = 30;
		rotationFlags = 1;
		model = "\DayZExpansion\NamalskAdventure\Dta\Items\Key_Cards\Key_Card.p3d";
		hiddenSelections[] = {"body"};
		hiddenSelectionsTextures[] = {"\DayZExpansion\NamalskAdventure\Dta\Items\Key_Cards\data\key_card_co.paa"};
		hiddenSelectionsMaterials[] = {"\DayZExpansion\NamalskAdventure\Dta\Items\Key_Cards\data\Expansion_Key_Card.rvmat"};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 15;
					healthLabels[] = {1.0,0.7,0.5,0.3,0.0};
					healthLevels[] = {{1.0,{"\DayZExpansion\NamalskAdventure\Dta\Items\Key_Cards\data\Expansion_Key_Card.rvmat"}},{0.7,{"\DayZExpansion\NamalskAdventure\Dta\Items\Key_Cards\data\Expansion_Key_Card.rvmat"}},{0.5,{"\DayZExpansion\NamalskAdventure\Dta\Items\Key_Cards\data\Expansion_Key_Card.rvmat"}},{0.3,{"\DayZExpansion\NamalskAdventure\Dta\Items\Key_Cards\data\Expansion_Key_Card.rvmat"}},{0.0,{"\DayZExpansion\NamalskAdventure\Dta\Items\Key_Cards\data\Expansion_Key_Card.rvmat"}}};
				};
			};
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class pickUpItem
				{
					soundSet = "pickUpPaper_SoundSet";
					id = 797;
				};
			};
		};
	};
	class Expansion_KeyCard_NA_Antenna: Expansion_KeyCard_Base
	{
		scope = 2;
		displayName = "Satellite Control Access Card";
		descriptionShort = "PLACEHOLDER";
		hiddenSelections[] = {"body"};
		hiddenSelectionsTextures[] = {"\DayZExpansion\NamalskAdventure\Dta\Items\Key_Cards\data\key_card_satellite_co.paa"};
		hiddenSelectionsMaterials[] = {"\DayZExpansion\NamalskAdventure\Dta\Items\Key_Cards\data\Expansion_Key_Card.rvmat"};
	};
};
