#define _ARMA_

class CfgPatches
{
	class DayZExpansion_P2PMarket_Objects
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","DZ_Characters","DayZExpansion_Dta_Core","DayZExpansion_Core_Objects_Bases"};
	};
};
class CfgVehicles
{
	class ExpansionStaticObjectBase;
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
	class ExpansionP2PMarketTraderStatic: ExpansionStaticObjectBase
	{
		scope = 0;
		vehicleClass = "Expansion_Static";
	};
	class ExpansionP2PMarketTraderLockerClosedBlueV1: ExpansionP2PMarketTraderStatic
	{
		scope = 1;
		model = "\DZ\structures\furniture\cases\locker\locker_closed_blue_v1.p3d";
		rotationFlags = 12;
	};
	class ExpansionP2PTraderMirek: SurvivorM_Mirek
	{
		scope = 2;
		displayName = "Mirek";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderDenis: SurvivorM_Denis
	{
		scope = 2;
		displayName = "Denis";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderBoris: SurvivorM_Boris
	{
		scope = 2;
		displayName = "Boris";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderCyril: SurvivorM_Cyril
	{
		scope = 2;
		displayName = "Cyril";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderElias: SurvivorM_Elias
	{
		scope = 2;
		displayName = "Elias";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderFrancis: SurvivorM_Francis
	{
		scope = 2;
		displayName = "Francis";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderGuo: SurvivorM_Guo
	{
		scope = 2;
		displayName = "Guo";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderHassan: SurvivorM_Hassan
	{
		scope = 2;
		displayName = "Hassan";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderIndar: SurvivorM_Indar
	{
		scope = 2;
		displayName = "Indar";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderJose: SurvivorM_Jose
	{
		scope = 2;
		displayName = "Jose";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderKaito: SurvivorM_Kaito
	{
		scope = 2;
		displayName = "Kaito";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderLewis: SurvivorM_Lewis
	{
		scope = 2;
		displayName = "Lewis";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderManua: SurvivorM_Manua
	{
		scope = 2;
		displayName = "Manua";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderNiki: SurvivorM_Niki
	{
		scope = 2;
		displayName = "Niki";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderOliver: SurvivorM_Oliver
	{
		scope = 2;
		displayName = "Oliver";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderPeter: SurvivorM_Peter
	{
		scope = 2;
		displayName = "Peter";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderQuinn: SurvivorM_Quinn
	{
		scope = 2;
		displayName = "Quinn";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderRolf: SurvivorM_Rolf
	{
		scope = 2;
		displayName = "Rolf";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderSeth: SurvivorM_Seth
	{
		scope = 2;
		displayName = "Seth";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderTaiki: SurvivorM_Taiki
	{
		scope = 2;
		displayName = "Taiki";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderLinda: SurvivorF_Linda
	{
		scope = 2;
		displayName = "Linda";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderMaria: SurvivorF_Maria
	{
		scope = 2;
		displayName = "Maria";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderFrida: SurvivorF_Frida
	{
		scope = 2;
		displayName = "Frida";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderGabi: SurvivorF_Gabi
	{
		scope = 2;
		displayName = "Gabi";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderHelga: SurvivorF_Helga
	{
		scope = 2;
		displayName = "Helga";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderIrena: SurvivorF_Irena
	{
		scope = 2;
		displayName = "Irena";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderJudy: SurvivorF_Judy
	{
		scope = 2;
		displayName = "Judy";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderKeiko: SurvivorF_Keiko
	{
		scope = 2;
		displayName = "Keiko";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderEva: SurvivorF_Eva
	{
		scope = 2;
		displayName = "Eva";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderNaomi: SurvivorF_Naomi
	{
		scope = 2;
		displayName = "Naomi";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderBaty: SurvivorF_Baty
	{
		scope = 2;
		displayName = "Baty";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderAIMirek: SurvivorM_Mirek
	{
		scope = 2;
		displayName = "Mirek";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderAIDenis: SurvivorM_Denis
	{
		scope = 2;
		displayName = "Denis";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderAIBoris: SurvivorM_Boris
	{
		scope = 2;
		displayName = "Boris";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderAICyril: SurvivorM_Cyril
	{
		scope = 2;
		displayName = "Cyril";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderAIElias: SurvivorM_Elias
	{
		scope = 2;
		displayName = "Elias";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderAIFrancis: SurvivorM_Francis
	{
		scope = 2;
		displayName = "Francis";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderAIGuo: SurvivorM_Guo
	{
		scope = 2;
		displayName = "Guo";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderAIHassan: SurvivorM_Hassan
	{
		scope = 2;
		displayName = "Hassan";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderAIIndar: SurvivorM_Indar
	{
		scope = 2;
		displayName = "Indar";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderAIJose: SurvivorM_Jose
	{
		scope = 2;
		displayName = "Jose";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderAIKaito: SurvivorM_Kaito
	{
		scope = 2;
		displayName = "Kaito";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderAILewis: SurvivorM_Lewis
	{
		scope = 2;
		displayName = "Lewis";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderAIManua: SurvivorM_Manua
	{
		scope = 2;
		displayName = "Manua";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderAINiki: SurvivorM_Niki
	{
		scope = 2;
		displayName = "Niki";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderAIOliver: SurvivorM_Oliver
	{
		scope = 2;
		displayName = "Oliver";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderAIPeter: SurvivorM_Peter
	{
		scope = 2;
		displayName = "Peter";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderAIQuinn: SurvivorM_Quinn
	{
		scope = 2;
		displayName = "Quinn";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderAIRolf: SurvivorM_Rolf
	{
		scope = 2;
		displayName = "Rolf";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderAISeth: SurvivorM_Seth
	{
		scope = 2;
		displayName = "Seth";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderAITaiki: SurvivorM_Taiki
	{
		scope = 2;
		displayName = "Taiki";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderAILinda: SurvivorF_Linda
	{
		scope = 2;
		displayName = "Linda";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderAIMaria: SurvivorF_Maria
	{
		scope = 2;
		displayName = "Maria";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderAIFrida: SurvivorF_Frida
	{
		scope = 2;
		displayName = "Frida";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderAIGabi: SurvivorF_Gabi
	{
		scope = 2;
		displayName = "Gabi";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderAIHelga: SurvivorF_Helga
	{
		scope = 2;
		displayName = "Helga";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderAIIrena: SurvivorF_Irena
	{
		scope = 2;
		displayName = "Irena";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderAIJudy: SurvivorF_Judy
	{
		scope = 2;
		displayName = "Judy";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderAIKeiko: SurvivorF_Keiko
	{
		scope = 2;
		displayName = "Keiko";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderAIEva: SurvivorF_Eva
	{
		scope = 2;
		displayName = "Eva";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderAINaomi: SurvivorF_Naomi
	{
		scope = 2;
		displayName = "Naomi";
		vehicleClass = "Expansion_Trader";
	};
	class ExpansionP2PTraderAIBaty: SurvivorF_Baty
	{
		scope = 2;
		displayName = "Baty";
		vehicleClass = "Expansion_Trader";
	};
};
