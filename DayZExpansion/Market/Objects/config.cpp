#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Market_Objects
	{
		units[] = {"ExpansionTraderZmbM_JournalistSkinny","ExpansionTraderMirek","ExpansionTraderDenis","ExpansionTraderBoris","ExpansionTraderCyril","ExpansionTraderElias","ExpansionTraderFrancis","ExpansionTraderGuo","ExpansionTraderHassan","ExpansionTraderIndar","ExpansionTraderJose","ExpansionTraderKaito","ExpansionTraderLewis","ExpansionTraderManua","ExpansionTraderNiki","ExpansionTraderOliver","ExpansionTraderPeter","ExpansionTraderQuinn","ExpansionTraderRolf","ExpansionTraderSeth","ExpansionTraderTaiki","ExpansionTraderLinda","ExpansionTraderMaria","ExpansionTraderFrida","ExpansionTraderGabi","ExpansionTraderHelga","ExpansionTraderIrena","ExpansionTraderJudy","ExpansionTraderKeiko","ExpansionTraderEva","ExpansionTraderNaomi","ExpansionTraderAIMirek","ExpansionTraderAIDenis","ExpansionTraderAIBoris","ExpansionTraderAICyril","ExpansionTraderAIElias","ExpansionTraderAIFrancis","ExpansionTraderAIGuo","ExpansionTraderAIHassan","ExpansionTraderAIIndar","ExpansionTraderAIJose","ExpansionTraderAIKaito","ExpansionTraderAILewis","ExpansionTraderAIManua","ExpansionTraderAINiki","ExpansionTraderAIOliver","ExpansionTraderAIPeter","ExpansionTraderAIQuinn","ExpansionTraderAIRolf","ExpansionTraderAISeth","ExpansionTraderAITaiki","ExpansionTraderAILinda","ExpansionTraderAIMaria","ExpansionTraderAIFrida","ExpansionTraderAIGabi","ExpansionTraderAIHelga","ExpansionTraderAIIrena","ExpansionTraderAIJudy","ExpansionTraderAIKeiko","ExpansionTraderAIEva","ExpansionTraderAINaomi"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","DZ_Structures_Signs","DZ_Animals_ursus_arctos","DZ_Characters","DZ_Characters_Zombies"};
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
	class ZmbM_HermitSkinny_Base;
	class ZmbM_JournalistSkinny;
	class ExpansionTraderStaticBase: HouseNoDestruct
	{
		forceFarBubble = "true";
	};
	class ExpansionTraderPumpkin: ExpansionTraderStaticBase
	{
		displayName = "$STR_Pumpkin0";
		descriptionShort = "$STR_Pumpkin1";
		model = "\DZ\characters\headgear\PumpkinHelmet_g.p3d";
	};
	class ExpansionTraderZucchini: ExpansionTraderStaticBase
	{
		displayName = "$STR_Zucchini0";
		descriptionShort = "$STR_Zucchini1";
		model = "\dz\gear\food\zucchini.p3d";
		rotationFlags = 12;
	};
	class ExpansionExchangeMachine: ExpansionTraderStaticBase
	{
		displayName = "Exchange Machine";
		descriptionShort = "Exchange Machine";
		model = "\DayZExpansion\Market\Objects\Exchange_Machine\Exchange_Machine.p3d";
		rotationFlags = 12;
	};
	class ExpansionTraderLockerClosedBlueV1: ExpansionTraderStaticBase
	{
		displayName = "$STR_EXPANSION_VENDING_MACHINE";
		model = "\DZ\structures\furniture\cases\locker\locker_closed_blue_v1.p3d";
		rotationFlags = 12;
	};
	class ExpansionTraderLockerClosedBlueV2: ExpansionTraderStaticBase
	{
		displayName = "$STR_EXPANSION_VENDING_MACHINE";
		model = "\DZ\structures\furniture\cases\locker\locker_closed_blue_v2.p3d";
		rotationFlags = 12;
	};
	class ExpansionTraderLockerClosedBlueV3: ExpansionTraderStaticBase
	{
		displayName = "$STR_EXPANSION_VENDING_MACHINE";
		model = "\DZ\structures\furniture\cases\locker\locker_closed_blue_v3.p3d";
		rotationFlags = 12;
	};
	class ExpansionTraderLockerClosedV1: ExpansionTraderStaticBase
	{
		displayName = "$STR_EXPANSION_VENDING_MACHINE";
		model = "\DZ\structures\furniture\cases\locker\locker_closed_v1.p3d";
		rotationFlags = 12;
	};
	class ExpansionTraderLockerClosedV2: ExpansionTraderStaticBase
	{
		displayName = "$STR_EXPANSION_VENDING_MACHINE";
		model = "\DZ\structures\furniture\cases\locker\locker_closed_v2.p3d";
		rotationFlags = 12;
	};
	class ExpansionTraderLockerClosedV3: ExpansionTraderStaticBase
	{
		displayName = "$STR_EXPANSION_VENDING_MACHINE";
		model = "\DZ\structures\furniture\cases\locker\locker_closed_v3.p3d";
		rotationFlags = 12;
	};
	class Animal_UrsusArctos_Preview: HouseNoDestruct
	{
		displayName = "$STR_CfgVehicles_Animal_UrsusArctos0";
		descriptionShort = "$STR_CfgVehicles_Animal_UrsusArctos1";
		model = "\DZ\animals_bliss\ursus_arctos\ursus_arctos.p3d";
	};
	class ZmbM_HermitSkinny_Beige_Preview: HouseNoDestruct
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
	class ExpansionTraderAIMirek: ExpansionTraderMirek{};
	class ExpansionTraderAIDenis: ExpansionTraderDenis{};
	class ExpansionTraderAIBoris: ExpansionTraderBoris{};
	class ExpansionTraderAICyril: ExpansionTraderCyril{};
	class ExpansionTraderAIElias: ExpansionTraderElias{};
	class ExpansionTraderAIFrancis: ExpansionTraderFrancis{};
	class ExpansionTraderAIGuo: ExpansionTraderGuo{};
	class ExpansionTraderAIHassan: ExpansionTraderHassan{};
	class ExpansionTraderAIIndar: ExpansionTraderIndar{};
	class ExpansionTraderAIJose: ExpansionTraderJose{};
	class ExpansionTraderAIKaito: ExpansionTraderKaito{};
	class ExpansionTraderAILewis: ExpansionTraderLewis{};
	class ExpansionTraderAIManua: ExpansionTraderManua{};
	class ExpansionTraderAINiki: ExpansionTraderNiki{};
	class ExpansionTraderAIOliver: ExpansionTraderOliver{};
	class ExpansionTraderAIPeter: ExpansionTraderPeter{};
	class ExpansionTraderAIQuinn: ExpansionTraderQuinn{};
	class ExpansionTraderAIRolf: ExpansionTraderRolf{};
	class ExpansionTraderAISeth: ExpansionTraderSeth{};
	class ExpansionTraderAITaiki: ExpansionTraderTaiki{};
	class ExpansionTraderAILinda: ExpansionTraderLinda{};
	class ExpansionTraderAIMaria: ExpansionTraderMaria{};
	class ExpansionTraderAIFrida: ExpansionTraderFrida{};
	class ExpansionTraderAIGabi: ExpansionTraderGabi{};
	class ExpansionTraderAIHelga: ExpansionTraderHelga{};
	class ExpansionTraderAIIrena: ExpansionTraderIrena{};
	class ExpansionTraderAIJudy: ExpansionTraderJudy{};
	class ExpansionTraderAIKeiko: ExpansionTraderKeiko{};
	class ExpansionTraderAIEva: ExpansionTraderEva{};
	class ExpansionTraderAINaomi: ExpansionTraderNaomi{};
};
