class CfgPatches
{
	class DayZExpansion_P2PMarket_Objects
	{
		units[]={};
		weapons[]={};
		requiredVersion=0.1;
		requiredAddons[]=
		{
			"DZ_Data",
			"DZ_Characters",
			"DayZExpansion_Dta_Core",
			"DayZExpansion_Core_Objects_Bases"
		};
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
		scope=0;
		vehicleClass="Expansion_Static";
	};
	class ExpansionP2PMarketTraderLockerClosedBlueV1: ExpansionP2PMarketTraderStatic
	{
		scope=1;
		model="\DZ\structures\furniture\cases\locker\locker_closed_blue_v1.p3d";
		rotationFlags=12;
	};
	class ExpansionP2PTraderMirek: SurvivorM_Mirek
	{
		scope=2;
		displayName="Mirek";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderDenis: SurvivorM_Denis
	{
		scope=2;
		displayName="Denis";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderBoris: SurvivorM_Boris
	{
		scope=2;
		displayName="Boris";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderCyril: SurvivorM_Cyril
	{
		scope=2;
		displayName="Cyril";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderElias: SurvivorM_Elias
	{
		scope=2;
		displayName="Elias";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderFrancis: SurvivorM_Francis
	{
		scope=2;
		displayName="Francis";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderGuo: SurvivorM_Guo
	{
		scope=2;
		displayName="Guo";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderHassan: SurvivorM_Hassan
	{
		scope=2;
		displayName="Hassan";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderIndar: SurvivorM_Indar
	{
		scope=2;
		displayName="Indar";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderJose: SurvivorM_Jose
	{
		scope=2;
		displayName="Jose";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderKaito: SurvivorM_Kaito
	{
		scope=2;
		displayName="Kaito";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderLewis: SurvivorM_Lewis
	{
		scope=2;
		displayName="Lewis";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderManua: SurvivorM_Manua
	{
		scope=2;
		displayName="Manua";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderNiki: SurvivorM_Niki
	{
		scope=2;
		displayName="Niki";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderOliver: SurvivorM_Oliver
	{
		scope=2;
		displayName="Oliver";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderPeter: SurvivorM_Peter
	{
		scope=2;
		displayName="Peter";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderQuinn: SurvivorM_Quinn
	{
		scope=2;
		displayName="Quinn";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderRolf: SurvivorM_Rolf
	{
		scope=2;
		displayName="Rolf";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderSeth: SurvivorM_Seth
	{
		scope=2;
		displayName="Seth";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderTaiki: SurvivorM_Taiki
	{
		scope=2;
		displayName="Taiki";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderLinda: SurvivorF_Linda
	{
		scope=2;
		displayName="Linda";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderMaria: SurvivorF_Maria
	{
		scope=2;
		displayName="Maria";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderFrida: SurvivorF_Frida
	{
		scope=2;
		displayName="Frida";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderGabi: SurvivorF_Gabi
	{
		scope=2;
		displayName="Gabi";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderHelga: SurvivorF_Helga
	{
		scope=2;
		displayName="Helga";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderIrena: SurvivorF_Irena
	{
		scope=2;
		displayName="Irena";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderJudy: SurvivorF_Judy
	{
		scope=2;
		displayName="Judy";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderKeiko: SurvivorF_Keiko
	{
		scope=2;
		displayName="Keiko";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderEva: SurvivorF_Eva
	{
		scope=2;
		displayName="Eva";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderNaomi: SurvivorF_Naomi
	{
		scope=2;
		displayName="Naomi";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderBaty: SurvivorF_Baty
	{
		scope=2;
		displayName="Baty";
		vehicleClass="Expansion_Trader";
	};
	class eAI_SurvivorM_Mirek: SurvivorM_Mirek
	{
	};
	class eAI_SurvivorM_Denis: SurvivorM_Denis
	{
	};
	class eAI_SurvivorM_Boris: SurvivorM_Boris
	{
	};
	class eAI_SurvivorM_Cyril: SurvivorM_Cyril
	{
	};
	class eAI_SurvivorM_Elias: SurvivorM_Elias
	{
	};
	class eAI_SurvivorM_Francis: SurvivorM_Francis
	{
	};
	class eAI_SurvivorM_Guo: SurvivorM_Guo
	{
	};
	class eAI_SurvivorM_Hassan: SurvivorM_Hassan
	{
	};
	class eAI_SurvivorM_Indar: SurvivorM_Indar
	{
	};
	class eAI_SurvivorM_Jose: SurvivorM_Jose
	{
	};
	class eAI_SurvivorM_Kaito: SurvivorM_Kaito
	{
	};
	class eAI_SurvivorM_Lewis: SurvivorM_Lewis
	{
	};
	class eAI_SurvivorM_Manua: SurvivorM_Manua
	{
	};
	class eAI_SurvivorM_Niki: SurvivorM_Niki
	{
	};
	class eAI_SurvivorM_Oliver: SurvivorM_Oliver
	{
	};
	class eAI_SurvivorM_Peter: SurvivorM_Peter
	{
	};
	class eAI_SurvivorM_Quinn: SurvivorM_Quinn
	{
	};
	class eAI_SurvivorM_Rolf: SurvivorM_Rolf
	{
	};
	class eAI_SurvivorM_Seth: SurvivorM_Seth
	{
	};
	class eAI_SurvivorM_Taiki: SurvivorM_Taiki
	{
	};
	class eAI_SurvivorF_Linda: SurvivorF_Linda
	{
	};
	class eAI_SurvivorF_Maria: SurvivorF_Maria
	{
	};
	class eAI_SurvivorF_Frida: SurvivorF_Frida
	{
	};
	class eAI_SurvivorF_Gabi: SurvivorF_Gabi
	{
	};
	class eAI_SurvivorF_Helga: SurvivorF_Helga
	{
	};
	class eAI_SurvivorF_Irena: SurvivorF_Irena
	{
	};
	class eAI_SurvivorF_Judy: SurvivorF_Judy
	{
	};
	class eAI_SurvivorF_Keiko: SurvivorF_Keiko
	{
	};
	class eAI_SurvivorF_Eva: SurvivorF_Eva
	{
	};
	class eAI_SurvivorF_Naomi: SurvivorF_Naomi
	{
	};
	class eAI_SurvivorF_Baty: SurvivorF_Baty
	{
	};
	class ExpansionP2PTraderAIMirek: eAI_SurvivorM_Mirek
	{
		scope=2;
		displayName="Mirek";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderAIDenis: eAI_SurvivorM_Denis
	{
		scope=2;
		displayName="Denis";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderAIBoris: eAI_SurvivorM_Boris
	{
		scope=2;
		displayName="Boris";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderAICyril: eAI_SurvivorM_Cyril
	{
		scope=2;
		displayName="Cyril";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderAIElias: eAI_SurvivorM_Elias
	{
		scope=2;
		displayName="Elias";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderAIFrancis: eAI_SurvivorM_Francis
	{
		scope=2;
		displayName="Francis";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderAIGuo: eAI_SurvivorM_Guo
	{
		scope=2;
		displayName="Guo";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderAIHassan: eAI_SurvivorM_Hassan
	{
		scope=2;
		displayName="Hassan";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderAIIndar: eAI_SurvivorM_Indar
	{
		scope=2;
		displayName="Indar";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderAIJose: eAI_SurvivorM_Jose
	{
		scope=2;
		displayName="Jose";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderAIKaito: eAI_SurvivorM_Kaito
	{
		scope=2;
		displayName="Kaito";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderAILewis: eAI_SurvivorM_Lewis
	{
		scope=2;
		displayName="Lewis";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderAIManua: eAI_SurvivorM_Manua
	{
		scope=2;
		displayName="Manua";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderAINiki: eAI_SurvivorM_Niki
	{
		scope=2;
		displayName="Niki";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderAIOliver: eAI_SurvivorM_Oliver
	{
		scope=2;
		displayName="Oliver";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderAIPeter: eAI_SurvivorM_Peter
	{
		scope=2;
		displayName="Peter";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderAIQuinn: eAI_SurvivorM_Quinn
	{
		scope=2;
		displayName="Quinn";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderAIRolf: eAI_SurvivorM_Rolf
	{
		scope=2;
		displayName="Rolf";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderAISeth: eAI_SurvivorM_Seth
	{
		scope=2;
		displayName="Seth";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderAITaiki: eAI_SurvivorM_Taiki
	{
		scope=2;
		displayName="Taiki";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderAILinda: eAI_SurvivorF_Linda
	{
		scope=2;
		displayName="Linda";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderAIMaria: eAI_SurvivorF_Maria
	{
		scope=2;
		displayName="Maria";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderAIFrida: eAI_SurvivorF_Frida
	{
		scope=2;
		displayName="Frida";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderAIGabi: eAI_SurvivorF_Gabi
	{
		scope=2;
		displayName="Gabi";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderAIHelga: eAI_SurvivorF_Helga
	{
		scope=2;
		displayName="Helga";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderAIIrena: eAI_SurvivorF_Irena
	{
		scope=2;
		displayName="Irena";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderAIJudy: eAI_SurvivorF_Judy
	{
		scope=2;
		displayName="Judy";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderAIKeiko: eAI_SurvivorF_Keiko
	{
		scope=2;
		displayName="Keiko";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderAIEva: eAI_SurvivorF_Eva
	{
		scope=2;
		displayName="Eva";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderAINaomi: eAI_SurvivorF_Naomi
	{
		scope=2;
		displayName="Naomi";
		vehicleClass="Expansion_Trader";
	};
	class ExpansionP2PTraderAIBaty: eAI_SurvivorF_Baty
	{
		scope=2;
		displayName="Baty";
		vehicleClass="Expansion_Trader";
	};
};
