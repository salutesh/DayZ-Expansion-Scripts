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

		AddItem("personalradio", 	710, 	1180,	1,100,{"Battery9V"});
		AddItem("megaphone", 	3030, 	5050,	1,100,{"Battery9V"});
		AddItem("electronicrepairkit", 	610, 	1015,	1,100);
		AddItem("cablereel", 	550, 	915,	1,100);
		AddItem("batterycharger", 	670, 	1115,	1,100);
		AddItem("baseradio", 	9695, 	16160,	1,100,{"CarBattery"});
		AddItem("rangefinder", 	815, 	1355,	1,100,{"Battery9V"});
		AddItem("nvgoggles", 	5415, 	9025,	1,100,{"Battery9V"});
		
		AddItem("alarmclock_blue", 	370, 	615,	1,100,null,{"AlarmClock_Green","AlarmClock_Red"});
		AddItem("kitchentimer", 	285, 	475,	1,100);
		
		#ifdef NAMALSK_SURVIVAL
		AddItem("dzn_tool_watch", 					40,		100,	1,		100, null, {"dzn_tool_watch2"});
		#endif
	}
};