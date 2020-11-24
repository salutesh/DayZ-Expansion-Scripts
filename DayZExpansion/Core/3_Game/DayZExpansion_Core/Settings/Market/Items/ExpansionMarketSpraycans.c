/**
 * ExpansionMarketSpraycans.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketSpraycans: ExpansionMarketCategory
{
	// ------------------------------------------------------------
	// Expansion Defaults
	// ------------------------------------------------------------
	override void Defaults()
	{
		super.Defaults();

		CategoryID = 52;
		DisplayName = "#STR_EXPANSION_MARKET_CATEGORY_SPRAYCANS";
		m_FileName = "Spraycans";

		AddItem( "ExpansionSpraycanGreen",			100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanBlue",			100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanWhite",			100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanRed",			100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanBlack",			100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanGrey",			100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanWine",			100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanYellow",			100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanWeeb",			100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanDahoman",		100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanIon",			100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanPresident",		100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanVrana",			100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanWave",			100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanCivilian",		100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanMedical",		100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanUber",			100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanBlueline",		100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanDigital",		100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanElliptical",		100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanFurious",		100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanGrayWatcher",	100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanJeans",			100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanShadow",			100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanSpeedy",			100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanSunset",			100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanWasp",			100,	120,	1,		250,	0,	{} );
	}
};