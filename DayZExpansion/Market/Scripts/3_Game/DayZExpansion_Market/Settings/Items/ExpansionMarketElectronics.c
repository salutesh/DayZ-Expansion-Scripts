/**
 * ExpansionMarketElectronics.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketElectronics: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 18;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_ELECTRONICS";
		m_FileName = "Electronics";

		AddItem("PersonalRadio", 					10,		20,		1,		100,	0,	{"Battery9V"});
		AddItem("Megaphone", 						30,		60,		1,		100,	0,	{"Battery9V"});
		AddItem("ElectronicRepairKit", 				35,		70,		1,		100,	0);
		AddItem("CableReel", 						40,		80,		1,		100,	0);
		AddItem("BatteryCharger", 					40,		80,		1,		100,	0);
		AddItem("BaseRadio", 						50,		100,	1,		100,	0,	{"CarBattery"});
		AddItem("Rangefinder", 						60,		120,	1,		100,	0,	{"Battery9V"});
		AddItem("NVGoggles", 						400,	800,	1,		100,	0,	{"Battery9V"});
	}
};