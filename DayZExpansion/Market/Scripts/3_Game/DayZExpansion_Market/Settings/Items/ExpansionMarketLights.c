/**
 * ExpansionMarketLights.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketLights: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 34;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_LIGHTS";
		m_FileName = "Lights";
		
		AddItem("chemlight_blue", 	370, 	615,	1,100,null,{"Chemlight_Green","Chemlight_Red","Chemlight_White","Chemlight_Yellow"});

		AddItem("roadflare", 	5670, 	9445,	1,100);
		AddItem("matchbox", 	265, 	445,	1,100);
		AddItem("fireworkslauncher", 	495, 	825,	1,100);
		AddItem("petrollighter", 	270, 	450,	1,100);
		AddItem("flashlight", 	170, 	280,	1,100,{"Battery9V"});
		AddItem("xmaslights", 	260, 	430,	1,100);
		AddItem("portablegaslamp", 	750, 	1250,	1,100,{"MediumGasCanister"});
		AddItem("portablegasstove", 	365, 	605,	1,100,{"MediumGasCanister"});
		AddItem("headtorch_black", 	1770, 	2955,	1,100,{"Battery9V"},{"Headtorch_Grey"});
		AddItem("headtorch_grey", 	1770, 	2955,	1,100,{"Battery9V"});
		AddItem("spotlight", 	670, 	1115,	1,100);
		
		AddItem("universallight", 	2855, 	4760,	1,100);
		AddItem("tlrlight", 	2855, 	4760,	1,100);
	}
};