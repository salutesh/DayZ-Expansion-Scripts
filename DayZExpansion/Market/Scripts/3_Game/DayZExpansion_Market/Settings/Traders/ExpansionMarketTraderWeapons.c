/**
 * ExpansionMarketTraderWeapons.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketTraderWeapons: ExpansionMarketTrader
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();
		
		DisplayName = "#STR_EXPANSION_MARKET_TRADER_WEAPONS";
		m_FileName = "Weapons";
		TraderIcon = "Shotgun";

		Categories.Insert("Assault_Rifles");
		Categories.Insert("Submachine_Guns");
		Categories.Insert("Rifles");
		Categories.Insert("Pistols");
		Categories.Insert("Sniper_Rifles");
		Categories.Insert("Shotguns");
		
		// LAUNCHERS
		AddItem("Flaregun");
	
	#ifdef EXPANSIONMODWEAPONS	
		Categories.Insert("Crossbows");

		//! Specify CanBuyAndSellAsAttachmentOnly to hide them from the "Ammo" category (these items are available in the "Special" trader)
		AddItem("ExpansionAmmoLAW", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("ExpansionAmmoRPG", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("Ammo_Expansion_M203_Smoke_White", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("Ammo_Expansion_M203_Smoke_Teargas", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("Ammo_Expansion_M203_Sticky_Smoke_White", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("Ammo_Expansion_M203_Sticky_Smoke_Teargas", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("Ammo_Expansion_M203_HE", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
	#endif
		
		Categories.Insert("Melee_Weapons");
		Categories.Insert("Ammo");

		Categories.Insert("Ammo_Boxes");
		Categories.Insert("Knifes");

		AddCategory("Magazines", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddCategory("Muzzles", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddCategory("Bayonets", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddCategory("Optics", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddCategory("Buttstocks", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddCategory("Handguards", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		
		// WEAPON LIGHTS
		AddItem("UniversalLight", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
		AddItem("TLRLight", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
	#ifdef EXPANSIONMODWEAPONS	
		AddItem("Expansion_Ammo_Taser", ExpansionMarketTraderBuySell.CanBuyAndSellAsAttachmentOnly);
	#endif
	}
};