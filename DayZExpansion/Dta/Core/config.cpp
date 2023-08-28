#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Dta_Core
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Vehicles_Wheeled","DZ_Characters","DZ_Scripts","DZ_AI"};
	};
};
class CfgSurfaces
{
	class DZ_SurfacesExt;
	class expansion_test_surface: DZ_SurfacesExt
	{
		files = "lino_int*";
		friction = 5.0;
		restitution = 1.0;
		soundEnviron = "road";
		soundHit = "hard_ground";
		character = "Empty";
		audibility = 0.4;
		footDamage = 0.023;
		impact = "Hit_Rubber";
		isDigable = 0;
		isFertile = 0;
	};
};
class CfgVehicleClasses
{
	class Expansion_Gear
	{
		displayName = "Expansion Gear";
	};
	class Expansion_Launchers
	{
		displayName = "Expansion Launchers";
	};
	class Expansion_Flags
	{
		displayName = "Expansion Flags";
	};
	class Expansion_Items
	{
		displayName = "Expansion Items";
	};
	class Expansion_Explosives
	{
		displayName = "Expansion Explosives";
	};
	class Expansion_Barricading
	{
		displayName = "Expansion Barricading";
	};
	class Expansion_Airdrop
	{
		displayName = "Expansion Airdrop";
	};
	class Expansion_Trader
	{
		displayName = "Expansion Traders";
	};
	class Expansion_Currency
	{
		displayName = "Expansion Currency";
	};
	class Expansion_Npc
	{
		displayName = "Expansion Npc";
	};
	class Expansion_Static
	{
		displayName = "Expansion Static";
	};
	class Expansion_Construction
	{
		displayName = "Expansion Construction";
	};
	class Expansion_Sign
	{
		displayName = "Expansion Sign";
	};
};
class CfgVehicles
{
	class Inventory_Base;
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
	class ExpansionPickup: Inventory_Base
	{
		scope = 2;
	};
	class ExpansionPickupCone: ExpansionPickup
	{
		scope = 2;
		model = "\DayZExpansion\Dta\Core\cone.p3d";
	};
	class ExplosionPoint: HouseNoDestruct
	{
		scope = 2;
		model = "\DZ\data\lightpoint.p3d";
	};
	class ExplosionSmall: ExplosionPoint
	{
		scope = 2;
	};
	class ExpansionNPCMirek: SurvivorM_Mirek
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNPCDenis: SurvivorM_Denis
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNPCBoris: SurvivorM_Boris
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNPCCyril: SurvivorM_Cyril
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNPCElias: SurvivorM_Elias
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNPCFrancis: SurvivorM_Francis
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNPCGuo: SurvivorM_Guo
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNPCHassan: SurvivorM_Hassan
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNPCIndar: SurvivorM_Indar
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNPCJose: SurvivorM_Jose
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNPCKaito: SurvivorM_Kaito
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNPCLewis: SurvivorM_Lewis
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNPCManua: SurvivorM_Manua
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNPCNiki: SurvivorM_Niki
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNPCOliver: SurvivorM_Oliver
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNPCPeter: SurvivorM_Peter
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNPCQuinn: SurvivorM_Quinn
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNPCRolf: SurvivorM_Rolf
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNPCSeth: SurvivorM_Seth
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNPCTaiki: SurvivorM_Taiki
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNPCLinda: SurvivorF_Linda
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNPCMaria: SurvivorF_Maria
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNPCFrida: SurvivorF_Frida
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNPCGabi: SurvivorF_Gabi
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNPCHelga: SurvivorF_Helga
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNPCIrena: SurvivorF_Irena
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNPCJudy: SurvivorF_Judy
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNPCKeiko: SurvivorF_Keiko
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNPCEva: SurvivorF_Eva
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNPCNaomi: SurvivorF_Naomi
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionNPCBaty: SurvivorF_Baty
	{
		scope = 2;
		vehicleClass = "Expansion_Npc";
	};
	class ExpansionMoneyBase: Inventory_Base
	{
		scope = 0;
		vehicleClass = "Expansion_Currency";
	};
	class ExpansionATMBase: HouseNoDestruct
	{
		scope = 0;
		vehicleClass = "Expansion_Static";
	};
	class ExpansionATMLocker: ExpansionATMBase
	{
		scope = 1;
		displayName = "$STR_EXPANSION_ATM_LOCKER";
		model = "\DZ\structures\furniture\cases\locker\locker_closed_blue_v1.p3d";
	};
};
