/**
 * ExpansionMarketMedical.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketMedical: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 40;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_MEDICALS";
		m_FileName = "Medical";

		AddItem("charcoaltablets", 	350, 	580,	1,100);
		AddItem("bandagedressing", 	855, 	1425,	1,100);
		AddItem("disinfectantalcohol", 	105, 	170,	1,100);
		AddItem("purificationtablets", 	310, 	520,	1,100);
		AddItem("bloodtestkit", 	895, 	1490,	1,100);
		AddItem("gasmask_filter", 	360, 	595,	1,100);
		#ifndef DAYZ_1_25
		AddItem("chelatingtablets", 	930, 	1555,	1,100);
		#endif
		//AddItem("InjectionVial", 				5,		10,		1,		100);
		AddItem("thermometer", 	365, 	610,	1,100);
		AddItem("vitaminbottle", 	40, 	65,	1,100);
		AddItem("disinfectantspray", 	145, 	245,	1,100);
		AddItem("tetracyclineantibiotics", 	350, 	580,	1,100);
		AddItem("painkillertablets", 	350, 	580,	1,100);
		//AddItem("Rag",						5,		10,		1,		100);
		//AddItem("Syringe", 					8,		16,		1,		100);
		AddItem("startkitiv", 	855, 	1425,	1,100);
		AddItem("heatpack", 	775, 	1295,	1,100);
		AddItem("salinebag", 	855, 	1425,	1,100);
		AddItem("bloodbagempty", 	855, 	1425,	1,100);
		AddItem("firstaidkit", 	700, 	1170,	1,100);
		AddItem("epinephrine", 	855, 	1425,	1,100);
		AddItem("morphine", 	880, 	1465,	1,100);
		//AddItem("Splint", 					5,		10,		1,		100);
		AddItem("iodinetincture", 	145, 	245,	1,100);
		//AddItem("Defibrillator", 				35,		70,		1,		100,	{"Battery9V"});
		AddItem("anticheminjector", 	2015, 	3360,	1,100);
	}
};