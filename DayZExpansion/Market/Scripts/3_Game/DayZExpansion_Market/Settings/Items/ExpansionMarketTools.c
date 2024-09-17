/**
 * ExpansionMarketComponents.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketTools: ExpansionMarketCategory
{
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 55;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_TOOLS";
		m_FileName = "Tools";
		
		AddItem("screwdriver", 	325, 	545,	1,100);
		AddItem("wrench", 	325, 	545,	1,100);
		AddItem("pliers", 	325, 	545,	1,100);
		AddItem("whetstone", 	375, 	625,	1,100);
		AddItem("hammer", 	520, 	865,	1,100);
		AddItem("hacksaw", 	450, 	750,	1,100);
		AddItem("handsaw", 	450, 	750,	1,100);
		AddItem("canopener", 	260, 	430,	1,100);
		AddItem("hatchet", 	250, 	420,	1,100);
		AddItem("lockpick", 	175, 	295,	1,100);
		
		AddItem("lugwrench", 	610, 	1015,	1,100);
		AddItem("pipewrench", 	440, 	735,	1,100);
		AddItem("fryingpan", 	270, 	450,	1,100);
		AddItem("sickle", 	1125, 	1875,	1,100);
		AddItem("crowbar", 	610, 	1015,	1,100);
		AddItem("shovel", 	2390, 	3980,	1,100);
		AddItem("pickaxe", 	2700, 	4495,	1,100);
		AddItem("sledgehammer", 	4485, 	7470,	1,100);
		AddItem("farminghoe", 	265, 	440,	1,100);
		AddItem("woodaxe", 	1955, 	3255,	1,100);
		AddItem("firefighteraxe", 	490, 	820,	1,100);
		AddItem("pitchfork", 	395, 	655,	1,100);
		AddItem("blowtorch", 	670, 	1115,	1,100,{"SmallGasCanister"});

		AddItem("fieldshovel", 	1425, 	2375,	1,100);
		AddItem("meattenderizer", 	285, 	475,	1,100);
		#ifndef DAYZ_1_25
		AddItem("iceaxe", 	245, 	410,	1,100);
		#endif
	#ifdef EXPANSIONMODBASEBUILDING
		AddItem("ExpansionPropaneTorch", 		950, 	1500,	1,		100);
		AddItem("ExpansionBoltCutters", 		390, 	620,	1,		100);
	#endif
	}
};