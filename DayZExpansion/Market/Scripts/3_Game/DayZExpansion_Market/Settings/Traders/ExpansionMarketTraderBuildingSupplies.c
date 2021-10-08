/**
 * ExpansionMarketTraderBuildingSupplies.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketTraderBuildingSupplies: ExpansionMarketTrader
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();
		
		TraderName = "BuildingSupplies";
		DisplayName = "#STR_EXPANSION_MARKET_TRADER_BUILDING_SUPPLIES";
		m_FileName = "BuildingSupplies";
		TraderIcon = "Nails";
		
		// TOOLS
		AddItem("Screwdriver");
		AddItem("Wrench");
		AddItem("Pliers");
		AddItem("Hammer");
		AddItem("Hacksaw");
		AddItem("HandSaw");
		AddItem("Hatchet");
		AddItem("LugWrench");
		AddItem("PipeWrench");
		AddItem("Crowbar");
		AddItem("Shovel");
		AddItem("Pickaxe");
		AddItem("SledgeHammer");
		AddItem("FarmingHoe");
		AddItem("WoodAxe");
		AddItem("FirefighterAxe");
	#ifdef EXPANSIONMOD
		AddItem("ExpansionPropaneTorch");
	#endif	
		
		// SUPPLIES
		AddItem("Rope");
		AddItem("TannedLeather");
		AddItem("MetalWire");
		AddItem("DuctTape");
		AddItem("Pot");
		AddItem("Netting");
		AddItem("BurlapSack");
		AddItem("WoodenPlank");
		AddItem("MetalPlate");
		AddItem("SmallGasCanister");
		AddItem("MediumGasCanister");
		AddItem("LargeGasCanister");
		AddItem("NailBox");
		AddItem("Nail");
		AddItem("BarbedWire");
		AddItem("Fabric");
		AddItem("Camonet");
		AddItem("HescoBox");
		AddItem("PowerGenerator");
	#ifdef EXPANSIONMOD
		//AddItem("ExpansionLumber0_5");
		AddItem("ExpansionLumber1");
		AddItem("ExpansionLumber1_5");
		AddItem("ExpansionLumber3");
		
		//! For the BaseBuilding update
		//AddItem("ExpansionCement");
		//AddItem("ExpansionBricks");
		
		//AddItem("ExpansionMixerKit");
		//AddItem("ExpansionMixerTank");
		//AddItem("ExpansionMixerMotor");
		//AddItem("ExpansionMixerHandle");
	#endif

		// LOCKS
		AddItem("CombinationLock");
		AddItem("CombinationLock4");
	#ifdef EXPANSIONMOD		
		AddItem("ExpansionCodeLock");
	#endif

		// CONTAINERS
		AddItem("SmallProtectorCase");
		AddItem("AmmoBox");
		AddItem("BarrelHoles_Blue");
		//AddItem("BarrelHoles_Green");
		//AddItem("BarrelHoles_Red");
		//AddItem("BarrelHoles_Yellow");
		AddItem("Barrel_Blue");
		//AddItem("Barrel_Green");
		//AddItem("Barrel_Red");
		//AddItem("Barrel_Yellow");
		AddItem("SeaChest");
	#ifdef EXPANSIONMOD
		AddItem("ExpansionToolBox");
		AddItem("ExpansionSafeMini");
		AddItem("ExpansionSafeMedium");
		AddItem("ExpansionSafeLarge");
	#endif
		
		// TENTS
		AddItem("PartyTent");
		//AddItem("PartyTent_Blue");
		//AddItem("PartyTent_Brown");
		//AddItem("PartyTent_Lunapark");
		//AddItem("MediumTent_Orange");
		AddItem("MediumTent");
		//AddItem("MediumTent_Green");
		AddItem("LargeTent");
		AddItem("CarTent");
	#ifdef EXPANSIONMOD
		AddItem("ExpansionCamoTentKit");
		AddItem("ExpansionCamoBoxKit");
	#endif

		// FURNISHINGS 
	#ifdef EXPANSIONMOD
		AddItem("ExpansionGunrack");
		AddItem("ExpansionStove");
	#endif

		// FLAGS
		AddItem("Flag_Chernarus");
		AddItem("Flag_Chedaki");
		AddItem("Flag_NAPA");
		AddItem("Flag_CDF");
		AddItem("Flag_Livonia");
		AddItem("Flag_Altis");
		AddItem("Flag_SSahrani");
		AddItem("Flag_NSahrani");
		AddItem("Flag_DayZ");
		AddItem("Flag_LivoniaArmy");
		AddItem("Flag_White");
		AddItem("Flag_Bohemia");
		AddItem("Flag_APA");
		AddItem("Flag_UEC");
		AddItem("Flag_Pirates");
		AddItem("Flag_Cannibals");
		AddItem("Flag_Bear");
		AddItem("Flag_Wolf");
		AddItem("Flag_BabyDeer");
		AddItem("Flag_Rooster");
		AddItem("Flag_LivoniaPolice");
		AddItem("Flag_CMC");
		AddItem("Flag_TEC");
		AddItem("Flag_CHEL");
		AddItem("Flag_Zenit");
		AddItem("Flag_HunterZ");
		AddItem("Flag_BrainZ");
		AddItem("Flag_Refuge");
		AddItem("Flag_RSTA");
		AddItem("Flag_Snake");
	#ifdef EXPANSIONMOD
		AddItem("Expansion_Flag_Expansion");
		AddItem("Expansion_Flag_White");
		AddItem("Expansion_Flag_Australia");
		AddItem("Expansion_Flag_Canada");
		AddItem("Expansion_Flag_Chernarus");
		AddItem("Expansion_Flag_France");
		AddItem("Expansion_Flag_Germany");
		AddItem("Expansion_Flag_Latvia");
		AddItem("Expansion_Flag_Luxembourg");
		AddItem("Expansion_Flag_Mexico");
		AddItem("Expansion_Flag_Netherlands");
		AddItem("Expansion_Flag_Newzealand");
		AddItem("Expansion_Flag_Norway");
		AddItem("Expansion_Flag_Poland");
		AddItem("Expansion_Flag_Russia");
		AddItem("Expansion_Flag_UK");
		AddItem("Expansion_Flag_USA");
		AddItem("Expansion_Flag_Scotland");
		AddItem("Expansion_Flag_Finland");
		AddItem("Expansion_Flag_Sweden");
		AddItem("Expansion_Flag_Spain");
		AddItem("Expansion_Flag_Brazil");
		AddItem("Expansion_Flag_Portugal");
		AddItem("Expansion_Flag_Belgium");
		AddItem("Expansion_Flag_Livonia");
		AddItem("Expansion_Flag_Japan");
		AddItem("Expansion_Flag_China");
		AddItem("Expansion_Flag_SouthKorea");
		AddItem("Expansion_Flag_UN");
		AddItem("Expansion_Flag_NATO");
		AddItem("Expansion_Flag_Pirate");
		AddItem("Expansion_Flag_Chedaki");
		AddItem("Expansion_Flag_Napa");
		AddItem("Expansion_Flag_Cdf");
		AddItem("Expansion_Flag_NuevoRico");
		AddItem("Expansion_Flag_Borduria");
		AddItem("Expansion_Flag_Biohazard");
		AddItem("Expansion_Flag_AnyoneInCherno");
		AddItem("Expansion_Flag_Ireland");
		AddItem("Expansion_Flag_Wales");
		AddItem("Expansion_Flag_Switzerland");
		AddItem("Expansion_Flag_Skilanka");
		AddItem("Expansion_Flag_SouthAfrica");
		AddItem("Expansion_Flag_Sicily");
		AddItem("Expansion_Flag_OffWithHead");
		AddItem("Expansion_Flag_Gubraltar");
		AddItem("Expansion_Flag_Czechia");
		AddItem("Expansion_Flag_Fari");
		AddItem("Expansion_Flag_DayZWhite");
		AddItem("Expansion_Flag_DayZBlack");
		AddItem("Expansion_Flag_DoubleAxe");
		AddItem("Expansion_Flag_Grenade");
		AddItem("Expansion_Flag_Red");
		AddItem("Expansion_Flag_Blue");
		AddItem("Expansion_Flag_Green");
		AddItem("Expansion_Flag_Yellow");
		AddItem("Expansion_Flag_Orange");
		AddItem("Expansion_Flag_Pink");
		AddItem("Expansion_Flag_Purple");
		AddItem("Expansion_Flag_Rainbow");
	#endif
	}
}