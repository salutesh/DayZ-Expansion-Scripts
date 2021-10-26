/**
 * ExpansionMarketMedical.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
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

		AddItem("CharcoalTablets", 				2,		4,		1,		100);
		AddItem("BandageDressing", 				3,		6,		1,		100);
		AddItem("DisinfectantAlcohol", 			4,		8,		1,		100);
		AddItem("PurificationTablets", 			5,		10,		1,		100);
		AddItem("BloodTestKit", 				5,		10,		1,		100);
		//AddItem("InjectionVial", 				5,		10,		1,		100);
		AddItem("Thermometer", 					5,		10,		1,		100);
		AddItem("VitaminBottle", 				6,		12,		1,		100);
		AddItem("DisinfectantSpray", 			8,		16,		1,		100);
		AddItem("TetracyclineAntibiotics", 		8,		16,		1,		100);
		AddItem("PainkillerTablets", 			10,		20,		1,		100);
		//AddItem("Rag",						5,		10,		1,		100);
		//AddItem("Syringe", 					8,		16,		1,		100);
		AddItem("StartKitIV", 					10,		20,		1,		100);
		AddItem("Heatpack", 					10,		20,		1,		100);
		AddItem("SalineBag", 					15,		30,		1,		100);
		AddItem("BloodBagEmpty", 				18,		36,		1,		100);
		AddItem("FirstAidKit", 					20,		40,		1,		100);
		AddItem("Epinephrine", 					20,		40,		1,		100);
		AddItem("Morphine", 					25,		50,		1,		100);
		//AddItem("Splint", 					5,		10,		1,		100);
		AddItem("IodineTincture", 				30,		60,		1,		100);
		//AddItem("Defibrillator", 				35,		70,		1,		100,	{"Battery9V"});
		AddItem("AntiChemInjector", 				100,		200,		1,		100);
	}
};