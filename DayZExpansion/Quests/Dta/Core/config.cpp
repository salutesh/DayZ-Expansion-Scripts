#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Quests_Dta_Core
	{
		units[] = {"ExpansionQuestNPC","ExpansionNpcDenis","ExpansionNpcBoris","ExpansionNpcCyril","ExpansionNpcElias","ExpansionNpcFrancis","ExpansionNpcGuo","ExpansionNpcHassan","ExpansionNpcIndar","ExpansionNpcJose","ExpansionNpcKaito","ExpansionNpcLewis","ExpansionNpcManua","ExpansionNpcNiki","ExpansionNpcOliver","ExpansionNpcPeter","ExpansionNpcQuinn","ExpansionNpcRolf","ExpansionNpcSeth","ExpansionNpcTaiki","ExpansionNpcLinda","ExpansionNpcMaria","ExpansionNpcFrida","ExpansionNpcGabi","ExpansionNpcHelga","ExpansionNpcIrena","ExpansionNpcJudy","ExpansionNpcKeiko","ExpansionNpcLina","ExpansionNpcNaomi","ExpansionQuestItemBase","ExpansionQuestItemPaper","ExpansionQuestItemPackage"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Vehicles_Wheeled","DZ_Characters","DZ_Scripts","DZ_AI","DayZExpansion_Dta_Core"};
	};
};
class CfgVehicleClasses
{
	class Expansion_Npc
	{
		displayName = "Expansion Npc";
	};
};
class CfgVehicles
{
	class DayZPlayer;
	class SurvivorM_Mirek;
	class SurvivorM_Denis;
	class SurvivorM_Boris;
	class SurvivorM_Cyril;
	class SurvivorM_Elias;
	class SurvivorM_Francis;
	class SurvivorM_Guo;
	class SurvivorM_Hassan;
	class SurvivorM_Indar;
	class SurvivorM_Jose;
	class SurvivorM_Kaito;
	class SurvivorM_Lewis;
	class SurvivorM_Manua;
	class SurvivorM_Niki;
	class SurvivorM_Oliver;
	class SurvivorM_Peter;
	class SurvivorM_Quinn;
	class SurvivorM_Rolf;
	class SurvivorM_Seth;
	class SurvivorM_Taiki;
	class SurvivorF_Linda;
	class SurvivorF_Maria;
	class SurvivorF_Frida;
	class SurvivorF_Gabi;
	class SurvivorF_Helga;
	class SurvivorF_Irena;
	class SurvivorF_Judy;
	class SurvivorF_Keiko;
	class SurvivorF_Lina;
	class SurvivorF_Naomi;
	class SurvivorF_Eva;
	class SurvivorF_Baty;
	class Inventory_Base;
	class Paper;
	class eAI_SurvivorM_Mirek: SurvivorM_Mirek{};
	class eAI_SurvivorM_Denis: SurvivorM_Denis{};
	class eAI_SurvivorM_Boris: SurvivorM_Boris{};
	class eAI_SurvivorM_Cyril: SurvivorM_Cyril{};
	class eAI_SurvivorM_Elias: SurvivorM_Elias{};
	class eAI_SurvivorM_Francis: SurvivorM_Francis{};
	class eAI_SurvivorM_Guo: SurvivorM_Guo{};
	class eAI_SurvivorM_Hassan: SurvivorM_Hassan{};
	class eAI_SurvivorM_Indar: SurvivorM_Indar{};
	class eAI_SurvivorM_Jose: SurvivorM_Jose{};
	class eAI_SurvivorM_Kaito: SurvivorM_Kaito{};
	class eAI_SurvivorM_Lewis: SurvivorM_Lewis{};
	class eAI_SurvivorM_Manua: SurvivorM_Manua{};
	class eAI_SurvivorM_Niki: SurvivorM_Niki{};
	class eAI_SurvivorM_Oliver: SurvivorM_Oliver{};
	class eAI_SurvivorM_Peter: SurvivorM_Peter{};
	class eAI_SurvivorM_Quinn: SurvivorM_Quinn{};
	class eAI_SurvivorM_Rolf: SurvivorM_Rolf{};
	class eAI_SurvivorM_Seth: SurvivorM_Seth{};
	class eAI_SurvivorM_Taiki: SurvivorM_Taiki{};
	class eAI_SurvivorF_Linda: SurvivorF_Linda{};
	class eAI_SurvivorF_Maria: SurvivorF_Maria{};
	class eAI_SurvivorF_Frida: SurvivorF_Frida{};
	class eAI_SurvivorF_Gabi: SurvivorF_Gabi{};
	class eAI_SurvivorF_Helga: SurvivorF_Helga{};
	class eAI_SurvivorF_Irena: SurvivorF_Irena{};
	class eAI_SurvivorF_Judy: SurvivorF_Judy{};
	class eAI_SurvivorF_Keiko: SurvivorF_Keiko{};
	class eAI_SurvivorF_Eva: SurvivorF_Eva{};
	class eAI_SurvivorF_Naomi: SurvivorF_Naomi{};
	class eAI_SurvivorF_Baty: SurvivorF_Baty{};
	class ExpansionQuestNpcAIMirek: eAI_SurvivorM_Mirek
	{
		displayName = "Mirek";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionQuestNpcAIDenis: eAI_SurvivorM_Denis
	{
		displayName = "Denis";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionQuestNpcAIBoris: eAI_SurvivorM_Boris
	{
		displayName = "Boris";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionQuestNpcAICyril: eAI_SurvivorM_Cyril
	{
		displayName = "Cyril";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionQuestNpcAIElias: eAI_SurvivorM_Elias
	{
		displayName = "Elias";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionQuestNpcAIFrancis: eAI_SurvivorM_Francis
	{
		displayName = "Francis";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionQuestNpcAIGuo: eAI_SurvivorM_Guo
	{
		displayName = "Guo";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionQuestNpcAIHassan: eAI_SurvivorM_Hassan
	{
		displayName = "Hassan";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionQuestNpcAIIndar: eAI_SurvivorM_Indar
	{
		displayName = "Indar";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionQuestNpcAIJose: eAI_SurvivorM_Jose
	{
		displayName = "Jose";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionQuestNpcAIKaito: eAI_SurvivorM_Kaito
	{
		displayName = "Kaito";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionQuestNpcAILewis: eAI_SurvivorM_Lewis
	{
		displayName = "Lewis";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionQuestNpcAIManua: eAI_SurvivorM_Manua
	{
		displayName = "Manua";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionQuestNpcAINiki: eAI_SurvivorM_Niki
	{
		displayName = "Niki";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionQuestNpcAIOliver: eAI_SurvivorM_Oliver
	{
		displayName = "Oliver";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionQuestNpcAIPeter: eAI_SurvivorM_Peter
	{
		displayName = "Peter";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionQuestNpcAIQuinn: eAI_SurvivorM_Quinn
	{
		displayName = "Quinn";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionQuestNpcAIRolf: eAI_SurvivorM_Rolf
	{
		displayName = "Rolf";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionQuestNpcAISeth: eAI_SurvivorM_Seth
	{
		displayName = "Seth";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionQuestNpcAITaiki: eAI_SurvivorM_Taiki
	{
		displayName = "Taiki";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionQuestNpcAILinda: eAI_SurvivorF_Linda
	{
		displayName = "Linda";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionQuestNpcAIMaria: eAI_SurvivorF_Maria
	{
		displayName = "Maria";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionQuestNpcAIFrida: eAI_SurvivorF_Frida
	{
		displayName = "Frida";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionQuestNpcAIGabi: eAI_SurvivorF_Gabi
	{
		displayName = "Gabi";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionQuestNpcAIHelga: eAI_SurvivorF_Helga
	{
		displayName = "Helga";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionQuestNpcAIIrena: eAI_SurvivorF_Irena
	{
		displayName = "Irena";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionQuestNpcAIJudy: eAI_SurvivorF_Judy
	{
		displayName = "Judy";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionQuestNpcAIKeiko: eAI_SurvivorF_Keiko
	{
		displayName = "Keiko";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionQuestNpcAIEva: eAI_SurvivorF_Eva
	{
		displayName = "Eva";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionQuestNpcAINaomi: eAI_SurvivorF_Naomi
	{
		displayName = "Naomi";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionQuestNpcAIBaty: eAI_SurvivorF_Baty
	{
		displayName = "Baty";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionQuestNPC: DayZPlayer
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNpcDenis: SurvivorM_Denis
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNpcBoris: SurvivorM_Boris
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNpcCyril: SurvivorM_Cyril
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNpcElias: SurvivorM_Elias
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNpcFrancis: SurvivorM_Francis
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNpcGuo: SurvivorM_Guo
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNpcHassan: SurvivorM_Hassan
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNpcIndar: SurvivorM_Indar
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNpcJose: SurvivorM_Jose
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNpcKaito: SurvivorM_Kaito
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNpcLewis: SurvivorM_Lewis
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNpcManua: SurvivorM_Manua
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNpcNiki: SurvivorM_Niki
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNpcOliver: SurvivorM_Oliver
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNpcPeter: SurvivorM_Peter
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNpcQuinn: SurvivorM_Quinn
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNpcRolf: SurvivorM_Rolf
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNpcSeth: SurvivorM_Seth
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNpcTaiki: SurvivorM_Taiki
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNpcLinda: SurvivorF_Linda
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNpcMaria: SurvivorF_Maria
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNpcFrida: SurvivorF_Frida
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNpcGabi: SurvivorF_Gabi
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNpcHelga: SurvivorF_Helga
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNpcIrena: SurvivorF_Irena
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNpcJudy: SurvivorF_Judy
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNpcKeiko: SurvivorF_Keiko
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNpcLina: SurvivorF_Lina
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNpcNaomi: SurvivorF_Naomi
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionQuestItemBase: Inventory_Base
	{
		scope = 2;
	};
	class ExpansionQuestItemPaper: ExpansionQuestItemBase
	{
		scope = 2;
		displayName = "Note";
		descriptionShort = "A questionable note that is hard to decipher..";
		model = "\dz\gear\consumables\Paper.p3d";
		isMeleeWeapon = 1;
		weight = 2;
		absorbency = 1;
		itemSize[] = {1,1};
		rotationFlags = 16;
		inventorySlot[] = {};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 15;
					healthLevels[] = {{1,{"DZ\gear\consumables\data\Loot_Paper.rvmat"}},{0.7,{"DZ\gear\consumables\data\Loot_Paper.rvmat"}},{0.5,{"DZ\gear\consumables\data\Loot_Paper_damage.rvmat"}},{0.3,{"DZ\gear\consumables\data\Loot_Paper_damage.rvmat"}},{0,{"DZ\gear\consumables\data\Loot_Paper_destruct.rvmat"}}};
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
	class ExpansionQuestItemPackage: ExpansionQuestItemBase
	{
		scope = 2;
		displayName = "Package";
		descriptionShort = "A package. Only the reciver will know what it contains.";
		model = "\DayZExpansion\Objects\Gear\Box\box.p3d";
		isMeleeWeapon = 1;
		weight = 4;
		absorbency = 1;
		itemSize[] = {2,2};
		rotationFlags = 16;
		inventorySlot[] = {};
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
};
