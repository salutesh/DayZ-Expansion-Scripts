/**
 * ExpansionMarketTraderComponents.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketTraderComponents: ExpansionMarketTrader
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();
		
		TraderName = "Components";
		DisplayName = "#STR_EXPANSION_MARKET_TRADER_COMPONENTS";
		m_FileName = "Components";
		TraderIcon = "Hammer";
		
		// TOOLS
		AddItem("Screwdriver");
		AddItem("Wrench");
		AddItem("Pliers");
		AddItem("Whetstone");
		AddItem("Hammer");
		AddItem("Hacksaw");
		AddItem("HandSaw");
		AddItem("CanOpener");
		AddItem("Hatchet");
		AddItem("Lockpick");
		
		AddItem("LugWrench");
		AddItem("PipeWrench");
		AddItem("FryingPan");
		AddItem("Sickle");
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
		AddItem("Paper");
		AddItem("Pen_Black");
		AddItem("Pen_Blue");
		AddItem("Pen_Green");
		AddItem("Pen_Red");
		AddItem("Rope");
		AddItem("TannedLeather");
		AddItem("AntiPestsSpray");
		AddItem("MetalWire");
		AddItem("EpoxyPutty");
		AddItem("DuctTape");
		AddItem("Pot");
		AddItem("HandcuffKeys");
		AddItem("Handcuffs");
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
		
		// KITS
		AddItem("SewingKit");
		AddItem("LeatherSewingKit");
		AddItem("WeaponCleaningKit");
				
		// Navigation
		AddItem("Compass");
		AddItem("OrienteeringCompass");
		
		if ( IsMapNamalsk() )
		{
			AddItem("dzn_map_namalsk");
		}
		else
		{
			AddItem("ChernarusMap");
		}

		AddItem("Binoculars");
	#ifdef EXPANSIONMOD
		AddItem("ExpansionGPS");
	#endif
		
		// ELECTRONICS
		AddItem("PersonalRadio");
		AddItem("Megaphone");
		AddItem("ElectronicRepairKit");
		AddItem("CableReel");
		AddItem("BatteryCharger");
		AddItem("BaseRadio");
		
		// LIGHTS
		AddItem("Chemlight_Blue");
		//AddItem("Chemlight_Green");
		//AddItem("Chemlight_Red");
		//AddItem("Chemlight_White");
		//AddItem("Chemlight_Yellow");

		AddItem("Roadflare");
		AddItem("Matchbox");
		AddItem("PetrolLighter");
		AddItem("Flashlight");
		AddItem("XmasLights");
		AddItem("PortableGasLamp");
		AddItem("PortableGasStove");
		AddItem("Headtorch_Black");
		AddItem("Headtorch_Grey");
		AddItem("Spotlight");
	}
}