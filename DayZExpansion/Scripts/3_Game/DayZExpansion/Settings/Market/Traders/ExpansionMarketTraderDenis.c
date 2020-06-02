/**
 * ExpansionMarketTraderDenis.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketTraderDenis: ExpansionMarketTrader
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();
		
		TraderName = "ExpansionTraderDenis";
		DisplayName = "#STR_EXPANSION_MARKET_TRADER_WEAPONS";
		m_FileName = "Weapons_Trader_Denis";

		// Assault Rifles
		AddItem("FAL");
		AddItem("AKM");
		AddItem("M4A1");
		AddItem("AK101");
		AddItem("AK74");
		AddItem("AKS74U");
		AddItem("Expansion_M16");
		
		// Submachine Guns
		AddItem("CZ61");
		AddItem("UMP45");
		AddItem("MP5K");
		AddItem("Expansion_Kedr");
		AddItem("Expansion_MPX");
		AddItem("Expansion_MP5");
		AddItem("Expansion_MP7");
		AddItem("Expansion_G36");
		
		// Rifles
		AddItem("Izh18");
		AddItem("Mosin9130");
		AddItem("CZ527");
		AddItem("Winchester70");
		AddItem("SKS");
		AddItem("Expansion_DT11");
		
		// Sidearms
		AddItem("MakarovIJ70");
		AddItem("CZ75");
		AddItem("FNX45");
		AddItem("Glock19");
		AddItem("MKII");
		AddItem("Expansion_M9");

		// Sniper Rifles
		AddItem("VSS");
		AddItem("SVD");
		AddItem("Expansion_AWM");
		
		// Shotguns
		AddItem("Mp133Shotgun");
		AddItem("Expansion_BenelliM4");
		
		// Launchers
		AddItem("ExpansionFlaregun");
		AddItem("ExpansionLAW");
		AddItem("ExpansionRPG7");
		AddItem("Expansion_M79");
		
		// Crossbows
		AddItem("ExpansionCrossbow");
		
		// Melee
		AddItem("BrassKnuckles_Shiny");
		AddItem("StunBaton");
		AddItem("Pipe");
		AddItem("CattleProd");
		AddItem("BaseballBat");
		AddItem("NailedBaseballBat");
		
		// Ammo
		AddItem("Ammo_308Win");
		AddItem("Ammo_308WinTracer");
		AddItem("Ammo_22");
		//AddItem("Ammo_357");
		AddItem("Ammo_762x39");
		AddItem("Ammo_762x54");
		AddItem("Ammo_12gaPellets");
		AddItem("Ammo_12gaSlug");
		AddItem("Ammo_545x39");
		AddItem("Ammo_45ACP");
		AddItem("Ammo_9x19");
		AddItem("Ammo_9x39");
		AddItem("Ammo_9x39AP");
		AddItem("Ammo_380");
		AddItem("Ammo_556x45");
		AddItem("Ammo_556x45");
		//AddItem("Ammo_Flare");
		//AddItem("Ammo_DartSyringe");
		AddItem("Ammo_ArrowComposite");
		
		AddItem("Ammo_Expansion_46x30");
		AddItem("Ammo_Expansion_338");
		
		AddItem("Ammo_Expansion_M203_Smoke_White");
		AddItem("Ammo_Expansion_M203_Smoke_Red");
		AddItem("Ammo_Expansion_M203_Smoke_Green");
		AddItem("Ammo_Expansion_M203_Smoke_Yellow");
		AddItem("Ammo_Expansion_M203_Smoke_Purple");
		AddItem("Ammo_Expansion_M203_Smoke_Teargas");
		AddItem("Ammo_Expansion_M203_Sticky_Smoke_White");
		AddItem("Ammo_Expansion_M203_Sticky_Smoke_Red");
		AddItem("Ammo_Expansion_M203_Sticky_Smoke_Green");
		AddItem("Ammo_Expansion_M203_Sticky_Smoke_Yellow");
		AddItem("Ammo_Expansion_M203_Sticky_Smoke_Purple");
		AddItem("Ammo_Expansion_M203_Sticky_Smoke_Teargas");
		AddItem("Ammo_Expansion_M203_HE");
		
		AddItem("ExpansionAmmoFlareLight");
		AddItem("ExpansionAmmoFlare");
		
		AddItem("ExpansionAmmoLAW");
		AddItem("ExpansionAmmoRPG");
		
		// Ammo Boxes
		AddItem("AmmoBox_556x45_20Rnd");
		AddItem("AmmoBox_556x45Tracer_20Rnd");
		AddItem("AmmoBox_308Win_20Rnd");
		AddItem("AmmoBox_762x54_20Rnd");
		AddItem("AmmoBox_762x54Tracer_20Rnd");
		AddItem("AmmoBox_762x39_20Rnd");
		AddItem("AmmoBox_762x39Tracer_20Rnd");
		AddItem("AmmoBox_22_50Rnd");
		//AddItem("AmmoBox_357_20Rnd");
		AddItem("AmmoBox_45ACP_25rnd");
		AddItem("AmmoBox_9x19_25rnd");
		AddItem("AmmoBox_9x39_20rnd");
		AddItem("AmmoBox_9x39AP_20rnd");
		AddItem("AmmoBox_380_35rnd");
		AddItem("AmmoBox_00buck_10rnd");
		AddItem("AmmoBox_12gaSlug_10Rnd");
		AddItem("AmmoBox_545x39_20Rnd");
		AddItem("AmmoBox_9x39_20Rnd");	
		
		AddItem("AmmoBox_Expansion_46x30_25rnd");	
		AddItem("AmmoBox_Expansion_338_15rnd");
	}
}