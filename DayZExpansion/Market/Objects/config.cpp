#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Market_Objects
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","DZ_Structures_Signs","DZ_Animals","DZ_Animals_Bliss","DZ_Characters","DZ_Characters_Zombies","DayZExpansion_Dta_Core","DayZExpansion_Core_Objects_Currencies","DayZExpansion_Core_Objects_Bases"};
	};
};
class CfgVehicles
{
	class AnimalBase;
	class Container_Base;
	class HouseNoDestruct;
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
	class SurvivorF_Eva;
	class SurvivorF_Naomi;
	class SurvivorF_Baty;
	class ZmbM_HermitSkinny_Base;
	class ZmbM_JournalistSkinny;
	class ExpansionATMBase;
	class ExpansionMoneyBanknote_Base;
	class ExpansionStaticObjectBase;
	class ExpansionTraderStaticBase: ExpansionStaticObjectBase
	{
		scope = 0;
		vehicleClass = "Expansion_Static";
	};
	class ExpansionTraderPumpkin: ExpansionTraderStaticBase
	{
		scope = 1;
		displayName = "$STR_Pumpkin0";
		descriptionShort = "$STR_Pumpkin1";
		model = "\DZ\characters\headgear\PumpkinHelmet_g.p3d";
	};
	class ExpansionTraderZucchini: ExpansionTraderStaticBase
	{
		scope = 1;
		displayName = "$STR_Zucchini0";
		descriptionShort = "$STR_Zucchini1";
		model = "\dz\gear\food\zucchini.p3d";
		rotationFlags = 12;
	};
	class ExpansionExchangeMachine: ExpansionTraderStaticBase
	{
		scope = 1;
		displayName = "Exchange Machine";
		descriptionShort = "Exchange Machine";
		model = "\DayZExpansion\Market\Objects\Exchange_Machine\Exchange_Machine.p3d";
		rotationFlags = 12;
	};
	class ExpansionTraderLockerClosedBlueV1: ExpansionTraderStaticBase
	{
		scope = 1;
		displayName = "$STR_EXPANSION_VENDING_MACHINE";
		model = "\DZ\structures\furniture\cases\locker\locker_closed_blue_v1.p3d";
		rotationFlags = 12;
	};
	class ExpansionTraderLockerClosedBlueV2: ExpansionTraderStaticBase
	{
		scope = 1;
		displayName = "$STR_EXPANSION_VENDING_MACHINE";
		model = "\DZ\structures\furniture\cases\locker\locker_closed_blue_v2.p3d";
		rotationFlags = 12;
	};
	class ExpansionTraderLockerClosedBlueV3: ExpansionTraderStaticBase
	{
		scope = 1;
		displayName = "$STR_EXPANSION_VENDING_MACHINE";
		model = "\DZ\structures\furniture\cases\locker\locker_closed_blue_v3.p3d";
		rotationFlags = 12;
	};
	class ExpansionTraderLockerClosedV1: ExpansionTraderStaticBase
	{
		scope = 1;
		displayName = "$STR_EXPANSION_VENDING_MACHINE";
		model = "\DZ\structures\furniture\cases\locker\locker_closed_v1.p3d";
		rotationFlags = 12;
	};
	class ExpansionTraderLockerClosedV2: ExpansionTraderStaticBase
	{
		scope = 1;
		displayName = "$STR_EXPANSION_VENDING_MACHINE";
		model = "\DZ\structures\furniture\cases\locker\locker_closed_v2.p3d";
		rotationFlags = 12;
	};
	class ExpansionTraderLockerClosedV3: ExpansionTraderStaticBase
	{
		scope = 1;
		displayName = "$STR_EXPANSION_VENDING_MACHINE";
		model = "\DZ\structures\furniture\cases\locker\locker_closed_v3.p3d";
		rotationFlags = 12;
	};
	class Animal_UrsusArctos_ExpansionMarketPreview: HouseNoDestruct
	{
		displayName = "$STR_CfgVehicles_Animal_UrsusArctos0";
		descriptionShort = "$STR_CfgVehicles_Animal_UrsusArctos1";
		model = "\DZ\animals_bliss\ursus_arctos\ursus_arctos.p3d";
	};
	class ZmbM_HermitSkinny_Beige_ExpansionMarketPreview: HouseNoDestruct
	{
		displayName = "$STR_cfgvehicles_infected0";
		descriptionShort = "$STR_cfgvehicles_infected1";
		model = "\DZ\characters\zombies\hermit_above0.p3d";
		hiddenSelectionsTextures[] = {"dz\characters\zombies\data\hermit_beige_co.paa"};
	};
	class ExpansionTraderZmbM_JournalistSkinny: ZmbM_JournalistSkinny
	{
		scope = 2;
		displayName = "...";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderMirek: SurvivorM_Mirek
	{
		scope = 2;
		displayName = "Mirek";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderDenis: SurvivorM_Denis
	{
		scope = 2;
		displayName = "Denis";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderBoris: SurvivorM_Boris
	{
		scope = 2;
		displayName = "Boris";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderCyril: SurvivorM_Cyril
	{
		scope = 2;
		displayName = "Cyril";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderElias: SurvivorM_Elias
	{
		scope = 2;
		displayName = "Elias";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderFrancis: SurvivorM_Francis
	{
		scope = 2;
		displayName = "Francis";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderGuo: SurvivorM_Guo
	{
		scope = 2;
		displayName = "Guo";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderHassan: SurvivorM_Hassan
	{
		scope = 2;
		displayName = "Hassan";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderIndar: SurvivorM_Indar
	{
		scope = 2;
		displayName = "Indar";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderJose: SurvivorM_Jose
	{
		scope = 2;
		displayName = "Jose";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderKaito: SurvivorM_Kaito
	{
		scope = 2;
		displayName = "Kaito";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderLewis: SurvivorM_Lewis
	{
		scope = 2;
		displayName = "Lewis";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderManua: SurvivorM_Manua
	{
		scope = 2;
		displayName = "Manua";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderNiki: SurvivorM_Niki
	{
		scope = 2;
		displayName = "Niki";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderOliver: SurvivorM_Oliver
	{
		scope = 2;
		displayName = "Oliver";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderPeter: SurvivorM_Peter
	{
		scope = 2;
		displayName = "Peter";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderQuinn: SurvivorM_Quinn
	{
		scope = 2;
		displayName = "Quinn";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderRolf: SurvivorM_Rolf
	{
		scope = 2;
		displayName = "Rolf";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderSeth: SurvivorM_Seth
	{
		scope = 2;
		displayName = "Seth";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderTaiki: SurvivorM_Taiki
	{
		scope = 2;
		displayName = "Taiki";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderLinda: SurvivorF_Linda
	{
		scope = 2;
		displayName = "Linda";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderMaria: SurvivorF_Maria
	{
		scope = 2;
		displayName = "Maria";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderFrida: SurvivorF_Frida
	{
		scope = 2;
		displayName = "Frida";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderGabi: SurvivorF_Gabi
	{
		scope = 2;
		displayName = "Gabi";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderHelga: SurvivorF_Helga
	{
		scope = 2;
		displayName = "Helga";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderIrena: SurvivorF_Irena
	{
		scope = 2;
		displayName = "Irena";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderJudy: SurvivorF_Judy
	{
		scope = 2;
		displayName = "Judy";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderKeiko: SurvivorF_Keiko
	{
		scope = 2;
		displayName = "Keiko";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderEva: SurvivorF_Eva
	{
		scope = 2;
		displayName = "Eva";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderNaomi: SurvivorF_Naomi
	{
		scope = 2;
		displayName = "Naomi";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderBaty: SurvivorF_Baty
	{
		scope = 2;
		displayName = "Baty";
		vehicleClass = "Expansion_Trader";
	};
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
	class ExpansionTraderAIMirek: eAI_SurvivorM_Mirek
	{
		displayName = "Mirek";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderAIDenis: eAI_SurvivorM_Denis
	{
		displayName = "Denis";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderAIBoris: eAI_SurvivorM_Boris
	{
		displayName = "Boris";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderAICyril: eAI_SurvivorM_Cyril
	{
		displayName = "Cyril";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderAIElias: eAI_SurvivorM_Elias
	{
		displayName = "Elias";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderAIFrancis: eAI_SurvivorM_Francis
	{
		displayName = "Francis";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderAIGuo: eAI_SurvivorM_Guo
	{
		displayName = "Guo";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderAIHassan: eAI_SurvivorM_Hassan
	{
		displayName = "Hassan";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderAIIndar: eAI_SurvivorM_Indar
	{
		displayName = "Indar";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderAIJose: eAI_SurvivorM_Jose
	{
		displayName = "Jose";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderAIKaito: eAI_SurvivorM_Kaito
	{
		displayName = "Kaito";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderAILewis: eAI_SurvivorM_Lewis
	{
		displayName = "Lewis";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderAIManua: eAI_SurvivorM_Manua
	{
		displayName = "Manua";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderAINiki: eAI_SurvivorM_Niki
	{
		displayName = "Niki";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderAIOliver: eAI_SurvivorM_Oliver
	{
		displayName = "Oliver";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderAIPeter: eAI_SurvivorM_Peter
	{
		displayName = "Peter";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderAIQuinn: eAI_SurvivorM_Quinn
	{
		displayName = "Quinn";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderAIRolf: eAI_SurvivorM_Rolf
	{
		displayName = "Rolf";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderAISeth: eAI_SurvivorM_Seth
	{
		displayName = "Seth";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderAITaiki: eAI_SurvivorM_Taiki
	{
		displayName = "Taiki";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderAILinda: eAI_SurvivorF_Linda
	{
		displayName = "Linda";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderAIMaria: eAI_SurvivorF_Maria
	{
		displayName = "Maria";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderAIFrida: eAI_SurvivorF_Frida
	{
		displayName = "Frida";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderAIGabi: eAI_SurvivorF_Gabi
	{
		displayName = "Gabi";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderAIHelga: eAI_SurvivorF_Helga
	{
		displayName = "Helga";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderAIIrena: eAI_SurvivorF_Irena
	{
		displayName = "Irena";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderAIJudy: eAI_SurvivorF_Judy
	{
		displayName = "Judy";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderAIKeiko: eAI_SurvivorF_Keiko
	{
		displayName = "Keiko";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderAIEva: eAI_SurvivorF_Eva
	{
		displayName = "Eva";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderAINaomi: eAI_SurvivorF_Naomi
	{
		displayName = "Naomi";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionTraderAIBaty: eAI_SurvivorF_Baty
	{
		displayName = "Baty";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionATM_Base: ExpansionATMBase
	{
		scope = 0;
		displayName = "$STR_EXPANSION_ATM";
		model = "dayzexpansion\market\objects\atm\expansion_atm.p3d";
		hiddenSelections[] = {"zbytek","screen"};
		hiddenSelectionsMaterials[] = {"dayzexpansion\market\objects\atm\data\expansion_atm.rvmat"};
		rotationFlags = 12;
	};
	class ExpansionATM_1: ExpansionATM_Base
	{
		scope = 1;
		hiddenSelectionsTextures[] = {"dayzexpansion\market\objects\atm\data\expansion_atm_01_co.paa","dayzexpansion\market\objects\atm\data\expansion_atm_01_co.paa"};
	};
	class ExpansionATM_2: ExpansionATM_Base
	{
		scope = 1;
		hiddenSelectionsTextures[] = {"dayzexpansion\market\objects\atm\data\expansion_atm_02_co.paa","dayzexpansion\market\objects\atm\data\expansion_atm_02_co.paa"};
	};
	class ExpansionATM_3: ExpansionATM_Base
	{
		scope = 1;
		hiddenSelectionsTextures[] = {"dayzexpansion\market\objects\atm\data\expansion_atm_03_co.paa","dayzexpansion\market\objects\atm\data\expansion_atm_03_co.paa"};
	};
	class ExpansionBanknoteUSD: ExpansionMoneyBanknote_Base
	{
		scope = 2;
		displayName = "$STR_EXPANSION_BANKNOTES_USD";
		descriptionShort = "$STR_EXPANSION_BANKNOTES_USD_DESC";
		model = "dayzexpansion\market\objects\currency\USD.p3d";
	};
	class ExpansionBanknoteEuro: ExpansionMoneyBanknote_Base
	{
		scope = 2;
		displayName = "$STR_EXPANSION_BANKNOTES_EURO";
		descriptionShort = "$STR_EXPANSION_BANKNOTES_EURO_DESC";
		model = "dayzexpansion\market\objects\currency\Euro.p3d";
	};
	class ExpansionBanknoteHryvnia: ExpansionMoneyBanknote_Base
	{
		scope = 2;
		displayName = "$STR_EXPANSION_BANKNOTES_HRYVNIA";
		descriptionShort = "$STR_EXPANSION_BANKNOTES_HRYVNIA_DESC";
		model = "dayzexpansion\market\objects\currency\Hryvnia.p3d";
	};
};
