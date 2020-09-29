/**
 * ExpansionSettingsRPC.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

enum ExpansionSettingsRPC
{
	INVALID = 30000,
	ListToLoad,
	
	Debug,
	Book,
	BaseBuilding,
	Map,
	Market,
	Notification,
	Party,
	Raid,
	Spawn,
	Territory,
	Vehicle,
	General,

	// when an admin requests these, not send under normal circumstances
	SafeZone,
	Mission,
	AirDrop,
	
	COUNT
};

enum ExpansionCOTBookModuleRPC
{
	INVALID = 30020,
	Update,
	COUNT
};

enum ExpansionCOTBaseBuildingModuleRPC
{
	INVALID = 30025,
	Update,
	COUNT
};

enum ExpansionCOTMapModuleRPC
{
	INVALID = 30030,
	Update,
	COUNT
};

enum ExpansionCOTMarketModuleRPC
{
	INVALID = 30035,
	Update,
	COUNT
};

enum ExpansionCOTNotificationModuleRPC
{
	INVALID = 30040,
	Update,
	COUNT
};

enum ExpansionCOTPartyModuleRPC
{
	INVALID = 30045,
	Update,
	COUNT
};

enum ExpansionCOTRaidModuleRPC
{
	INVALID = 30050,
	Update,
	COUNT
};

enum ExpansionCOTSpawnModuleRPC
{
	INVALID = 30055,
	Update,
	COUNT
};

enum ExpansionCOTTerritoryModuleRPC
{
	INVALID = 30060,
	Update,
	COUNT
};

enum ExpansionCOTVehicleModuleRPC
{
	INVALID = 30065,
	Update,
	COUNT
};

enum ExpansionCOTGeneralModuleRPC
{
	INVALID = 30070,
	Update,
	COUNT
};

enum ExpansionCOTSafeZoneModuleRPC
{
	INVALID = 30080,
	Update,
	COUNT
};

enum ExpansionCOTMissionModuleRPC
{
	INVALID = 30085,
	Update,
	COUNT
};

enum ExpansionCOTAirDropModuleRPC
{
	INVALID = 30090,
	Update,
	COUNT
};