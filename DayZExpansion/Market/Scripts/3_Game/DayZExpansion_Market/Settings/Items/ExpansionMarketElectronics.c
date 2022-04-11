/**
 * ExpansionMarketElectronics.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
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

		AddItem("PersonalRadio", 					10,		20,		1,		100,	{"Battery9V"});
		AddItem("Megaphone", 						30,		60,		1,		100,	{"Battery9V"});
		AddItem("ElectronicRepairKit", 				35,		70,		1,		100);
		AddItem("CableReel", 						40,		80,		1,		100);
		AddItem("BatteryCharger", 					40,		80,		1,		100);
		AddItem("BaseRadio", 						50,		100,	1,		100,	{"CarBattery"});
		AddItem("Rangefinder", 						60,		120,	1,		100,	{"Battery9V"});
		AddItem("NVGoggles", 						400,	800,	1,		100,	{"Battery9V"});
		
		AddItem("AlarmClock_Blue", 					30,		80,		1,		100, null, {"AlarmClock_Green", "AlarmClock_Red"});
		#ifdef DAYZ_1_17
		AddItem("KitchenTimer", 					30,		80,		1,		100);
		#endif
		#ifdef NAMALSK_SURVIVAL
		AddItem("dzn_tool_watch", 					40,		100,	1,		100, null, {"dzn_tool_watch2"});
		#endif
	}
};