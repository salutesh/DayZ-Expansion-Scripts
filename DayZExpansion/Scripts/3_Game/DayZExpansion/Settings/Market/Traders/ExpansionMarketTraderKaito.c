/**
 * ExpansionMarketTraderKaito.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketTraderKaito: ExpansionMarketTrader
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();
		
		TraderName = "ExpansionTraderKaito";
		DisplayName = "#STR_EXPANSION_MARKET_TRADER_COMPONENTS";
		m_FileName = "Components_Trader_Kaito";
		
		// Tools
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
		AddItem("Crowbar");
		AddItem("Shovel");
		AddItem("Pickaxe");
		AddItem("SledgeHammer");
		AddItem("WoodAxe");
		AddItem("FirefighterAxe");
		
		// Suplies
		AddItem("Paper");
		AddItem("Pen_Black");
		AddItem("Pen_Blue");
		AddItem("Pen_Red");
		AddItem("Pen_Green");
		AddItem("Rope");
		AddItem("TannedLeather");
		AddItem("MetalWire");
		AddItem("DuctTape");
		AddItem("XmasLights");
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
		AddItem("Camonet");
		AddItem("HescoBox");
		AddItem("PowerGenerator");
		
		// Locks
		AddItem("CombinationLock");
		AddItem("CombinationLock4");
		
		AddItem("ExpansionCodeLock");
		
		// Containers
		AddItem("SmallProtectorCase");
		AddItem("AmmoBox");
		AddItem("BarrelHoles_Blue");
		AddItem("BarrelHoles_Green");
		AddItem("BarrelHoles_Red");
		AddItem("BarrelHoles_Yellow");
		AddItem("Barrel_Blue");
		AddItem("Barrel_Green");
		AddItem("Barrel_Red");
		AddItem("Barrel_Yellow");
		AddItem("SeaChest");
		
		// Tents
		AddItem("MediumTent");
		AddItem("MediumTent_Green");
		AddItem("MediumTent_Orange");
		AddItem("CarTent");
		AddItem("LargeTent");
		AddItem("PartyTent");
		AddItem("PartyTent_Blue");
		AddItem("PartyTent_Brown");
		AddItem("PartyTent_Lunapark");
		
		// Kits
		AddItem("SewingKit");
		AddItem("LeatherSewingKit");
		AddItem("WeaponCleaningKit");
		
		// Knifes
		AddItem("KitchenKnife");
		AddItem("SteakKnife");
		AddItem("CombatKnife");
		AddItem("Machete");
		AddItem("HuntingKnife");
		
		// Navigation
		AddItem("ChernarusMap");
		AddItem("Binoculars");	
		AddItem("Compass");
		AddItem("OrienteeringCompass");
		AddItem("ExpansionGPS");
		
		// Electronic
		AddItem("Battery9V");
		AddItem("PersonalRadio");
		AddItem("Megaphone");
		AddItem("ElectronicRepairKit");
		AddItem("CableReel");
		AddItem("BatteryCharger");
		AddItem("BaseRadio");
		AddItem("Rangefinder");
		
		// Lights
		AddItem("Chemlight_Blue");
		AddItem("Chemlight_Green");
		AddItem("Chemlight_Red");
		AddItem("Chemlight_White");
		AddItem("Chemlight_Yellow");
		AddItem("Roadflare");
		AddItem("Matchbox");
		AddItem("Heatpack");
		AddItem("Flashlight");
		AddItem("PortableGasLamp");
		AddItem("PortableGasStove");
		AddItem("Headtorch_Black");
		AddItem("Headtorch_Grey");
		AddItem("Spotlight");
	}
}