/**
 * ExpansionMarketLights.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
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
		
		AddItem("Chemlight_Blue", 				2,		4,		1,		100,	0, null, {"Chemlight_Green", "Chemlight_Red", "Chemlight_White", "Chemlight_Yellow"});
		/*AddItem("Chemlight_Green", 				2,		4,		1,		100,	0);
		AddItem("Chemlight_Red", 				2,		4,		1,		100,	0);
		AddItem("Chemlight_White", 				2,		4,		1,		100,	0);
		AddItem("Chemlight_Yellow", 			2,		4,		1,		100,	0);*/

		AddItem("Roadflare", 					4,		8,		1,		100,	0);
		AddItem("Matchbox", 					6,		12,		1,		100,	0);
		AddItem("PetrolLighter", 				8,		16,		1,		100,	0);
		AddItem("Flashlight", 					15,		30,		1,		100,	0,	{"Battery9V"});
		AddItem("XmasLights", 					25,		50,		1,		100,	0);
		AddItem("PortableGasLamp", 				25,		50,		1,		100,	0,	{"MediumGasCanister"});
		AddItem("PortableGasStove", 			30,		60,		1,		100,	0,	{"MediumGasCanister"});
		AddItem("Headtorch_Black", 				40,		80,		1,		100,	0,	{"Battery9V"}, {"Headtorch_Grey"});
		//AddItem("Headtorch_Grey", 				40,		80,		1,		100,	0,	{"Battery9V"});
		AddItem("Spotlight", 					600,	1200,	1,		100,	0);
		
		AddItem("UniversalLight", 					120,	240,	1,		100,	0);
		AddItem("TLRLight", 					120,	240,	1,		100,	0);
	}
};