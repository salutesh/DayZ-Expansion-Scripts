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

		AddItem( "ExpansionSpraycanOffroadHatchbackGreen",		100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanOffroadHatchbackBlue",		100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanOffroadHatchbackWhite",		100,	120,	1,		250,	0,	{} );

		AddItem( "ExpansionSpraycanHatchback_02Red",			100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanHatchback_02Blue",			100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanHatchback_02Black",			100,	120,	1,		250,	0,	{} );
		
		AddItem( "ExpansionSpraycanCivilianSedanWhite",			100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanCivilianSedanGrey",			100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanCivilianSedanWine",			100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanCivilianSedanLime",			100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanCivilianSedanBlack",			100,	120,	1,		250,	0,	{} );
		
		AddItem( "ExpansionSpraycanSedan_02Blue",				100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanSedan_02Grey",				100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanSedan_02Red",				100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanSedan_02Yellow",				100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanSedan_02YellowBright",		100,	120,	1,		250,	0,	{} );

		AddItem( "ExpansionSpraycanBusDefault",					100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanBusWeeb",					100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanMerlinDefault",				100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanMerlinDahoman",				100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanMerlinIon",					100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanMerlinPresident",			100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanMerlinVrana",				100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanMerlinWave",					100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanUh1hDefault",				100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanUh1hCivilian",				100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanUh1hMedical",				100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanUh1hUber",					100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanMh6Default",					100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanMh6Blueline",				100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanMh6Digital",					100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanMh6Elliptical",				100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanMh6Furious",					100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanMh6GrayWatcher",				100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanMh6Jeans",					100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanMh6Shadow",					100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanMh6Speedy",					100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanMh6Sunset",					100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanMh6Vrana",					100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanMh6Wasp",					100,	120,	1,		250,	0,	{} );
		AddItem( "ExpansionSpraycanMh6Wave",					100,	120,	1,		250,	0,	{} );
	}
}