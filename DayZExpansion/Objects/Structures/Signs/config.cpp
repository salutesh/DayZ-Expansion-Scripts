#define _ARMA_

class CfgPatches
{
	class DayZExpansion_Objects_Structures_Signs
	{
		units[] = {"ExpansionSign_Color","ExpansionSignBig_Color","ExpansionSign_Supplies","ExpansionSign_Trader_Accessoires","ExpansionSign_Trader_Aircraft","ExpansionSign_Trader_Attachments","ExpansionSign_Trader_Bandit","ExpansionSign_Trader_Cars","ExpansionSign_Trader_Clothing","ExpansionSign_Trader_Components","ExpansionSign_Trader_Consumables","ExpansionSign_Trader_Exchange","ExpansionSign_Trader_Hero","ExpansionSign_Trader_Medicals","ExpansionSign_Trader_Special","ExpansionSign_Trader_Vehicles","ExpansionSign_Trader_VehicleParts","ExpansionSign_Trader_Weapons","ExpansionSign_Trader_Boats","ExpansionSign_Trader_Fishing","ExpansionSign_Trader_SpawnZone","Expansion_Decal_WelcomeToHell"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data"};
	};
};
class CfgVehicles
{
	class HouseNoDestruct;
	class ExpansionSign_Color: HouseNoDestruct
	{
		scope = 2;
		displayName = "$STR_EXPANSION_TRADER_BOARD";
		descriptionShort = "$STR_EXPANSION_TRADER_BOARD_DESC";
		model = "\DayZExpansion\Objects\Structures\Signs\TraderBoard.p3d";
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Signs\data\tradersign_co.paa"};
		vehicleClass = "Expansion_Sign";
	};
	class ExpansionSignBig_Color: HouseNoDestruct
	{
		scope = 2;
		displayName = "$STR_EXPANSION_TRADER_BOARD";
		descriptionShort = "$STR_EXPANSION_TRADER_BOARD_DESC";
		model = "\DayZExpansion\Objects\Structures\Signs\TraderBoardBig.p3d";
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Signs\data\tradersign_co.paa"};
		vehicleClass = "Expansion_Sign";
	};
	class ExpansionSign_Supplies: ExpansionSign_Color
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SUPPLY_TRADER";
		descriptionShort = "$STR_EXPANSION_SUPPLY_TRADER";
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Signs\data\tradersupplies_co.paa"};
		vehicleClass = "Expansion_Sign";
	};
	class ExpansionSign_Trader_Accessoires: ExpansionSign_Color
	{
		scope = 2;
		displayName = "$STR_EXPANSION_ACCESSORIES_TRADER_SIGN";
		descriptionShort = "$STR_EXPANSION_ACCESSORIES_TRADER_SIGN";
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Signs\data\traders\sign_trader_accessoires.paa"};
		vehicleClass = "Expansion_Sign";
	};
	class ExpansionSign_Trader_Aircraft: ExpansionSign_Color
	{
		scope = 2;
		displayName = "$STR_EXPANSION_AIRCRAFT_TRADER_SIGN";
		descriptionShort = "$STR_EXPANSION_AIRCRAFT_TRADER_SIGN";
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Signs\data\traders\sign_trader_aircraft.paa"};
		vehicleClass = "Expansion_Sign";
	};
	class ExpansionSign_Trader_Attachments: ExpansionSign_Color
	{
		scope = 2;
		displayName = "$STR_EXPANSION_ATTACHMENT_TRADER_SIGN";
		descriptionShort = "$STR_EXPANSION_ATTACHMENT_TRADER_SIGN";
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Signs\data\traders\sign_trader_attachments.paa"};
		vehicleClass = "Expansion_Sign";
	};
	class ExpansionSign_Trader_Bandit: ExpansionSign_Color
	{
		scope = 2;
		displayName = "$STR_EXPANSION_BANDIT_ZONE_TRADER_SIGN";
		descriptionShort = "$STR_EXPANSION_BANDIT_ZONE_TRADER_SIGN";
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Signs\data\traders\sign_trader_bandit.paa"};
		vehicleClass = "Expansion_Sign";
	};
	class ExpansionSign_Trader_Cars: ExpansionSign_Color
	{
		scope = 2;
		displayName = "$STR_EXPANSION_CAR_TRADER_SIGN";
		descriptionShort = "$STR_EXPANSION_CAR_TRADER_SIGN";
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Signs\data\traders\sign_trader_cars.paa"};
		vehicleClass = "Expansion_Sign";
	};
	class ExpansionSign_Trader_Clothing: ExpansionSign_Color
	{
		scope = 2;
		displayName = "$STR_EXPANSION_CLOTHING_TRADER_SIGN";
		descriptionShort = "$STR_EXPANSION_CLOTHING_TRADER_SIGN";
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Signs\data\traders\sign_trader_clothing.paa"};
		vehicleClass = "Expansion_Sign";
	};
	class ExpansionSign_Trader_Components: ExpansionSign_Color
	{
		scope = 2;
		displayName = "$STR_EXPANSION_COMPONENTS_TRADER_SIGN";
		descriptionShort = "$STR_EXPANSION_COMPONENTS_TRADER_SIGN";
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Signs\data\traders\sign_trader_components.paa"};
		vehicleClass = "Expansion_Sign";
	};
	class ExpansionSign_Trader_Consumables: ExpansionSign_Color
	{
		scope = 2;
		displayName = "$STR_EXPANSION_CONSUMABLES_TRADER_SIGN";
		descriptionShort = "$STR_EXPANSION_CONSUMABLES_TRADER_SIGN";
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Signs\data\traders\sign_trader_consumables.paa"};
		vehicleClass = "Expansion_Sign";
	};
	class ExpansionSign_Trader_Exchange: ExpansionSign_Color
	{
		scope = 2;
		displayName = "$STR_EXPANSION_EXCHANGE_TRADER_SIGN";
		descriptionShort = "$STR_EXPANSION_EXCHANGE_TRADER_SIGN";
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Signs\data\traders\sign_trader_exchange.paa"};
		vehicleClass = "Expansion_Sign";
	};
	class ExpansionSign_Trader_Hero: ExpansionSign_Color
	{
		scope = 2;
		displayName = "$STR_EXPANSION_HERO_TRADER_SIGN";
		descriptionShort = "$STR_EXPANSION_HERO_TRADER_SIGN";
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Signs\data\traders\sign_trader_hero.paa"};
		vehicleClass = "Expansion_Sign";
	};
	class ExpansionSign_Trader_Medicals: ExpansionSign_Color
	{
		scope = 2;
		displayName = "$STR_EXPANSION_MEDICALS_TRADER_SIGN";
		descriptionShort = "$STR_EXPANSION_MEDICALS_TRADER_SIGN";
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Signs\data\traders\sign_trader_medicals.paa"};
		vehicleClass = "Expansion_Sign";
	};
	class ExpansionSign_Trader_Special: ExpansionSign_Color
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPECIAL_TRADER_SIGN";
		descriptionShort = "$STR_EXPANSION_SPECIAL_TRADER_SIGN";
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Signs\data\traders\sign_trader_special.paa"};
		vehicleClass = "Expansion_Sign";
	};
	class ExpansionSign_Trader_Vehicles: ExpansionSign_Color
	{
		scope = 2;
		displayName = "$STR_EXPANSION_VEHICLES_TRADER_SIGN";
		descriptionShort = "$STR_EXPANSION_VEHICLES_TRADER_SIGN";
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Signs\data\traders\sign_trader_vehicles.paa"};
		vehicleClass = "Expansion_Sign";
	};
	class ExpansionSign_Trader_VehicleParts: ExpansionSign_Color
	{
		scope = 2;
		displayName = "$STR_EXPANSION_VEHICLES_PARTS_TRADER_SIGN";
		descriptionShort = "$STR_EXPANSION_VEHICLES_PARTS_TRADER_SIGN";
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Signs\data\traders\sign_trader_vehicleparts.paa"};
		vehicleClass = "Expansion_Sign";
	};
	class ExpansionSign_Trader_Weapons: ExpansionSign_Color
	{
		scope = 2;
		displayName = "$STR_EXPANSION_WEAPONS_TRADER_SIGN";
		descriptionShort = "$STR_EXPANSION_WEAPONS_TRADER_SIGN";
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Signs\data\traders\sign_trader_weapons.paa"};
		vehicleClass = "Expansion_Sign";
	};
	class ExpansionSign_Trader_Boats: ExpansionSign_Color
	{
		scope = 2;
		displayName = "$STR_EXPANSION_BOATS_TRADER_SIGN";
		descriptionShort = "$STR_EXPANSION_BOATS_TRADER_SIGN";
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Signs\data\traders\sign_trader_boat.paa"};
		vehicleClass = "Expansion_Sign";
	};
	class ExpansionSign_Trader_Fishing: ExpansionSign_Color
	{
		scope = 2;
		displayName = "$STR_EXPANSION_FISHING_TRADER_SIGN";
		descriptionShort = "$STR_EXPANSION_FISHING_TRADER_SIGN";
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Signs\data\traders\sign_trader_fishing.paa"};
		vehicleClass = "Expansion_Sign";
	};
	class ExpansionSign_Trader_SpawnZone: ExpansionSign_Color
	{
		scope = 2;
		displayName = "$STR_EXPANSION_SPAWNZONE_TRADER_SIGN";
		descriptionShort = "$STR_EXPANSION_SPAWNZONE_TRADER_SIGN";
		hiddenSelections[] = {"camo"};
		hiddenSelectionsTextures[] = {"\DayZExpansion\Objects\Structures\Signs\data\traders\sign_trader_spawnzone.paa"};
		vehicleClass = "Expansion_Sign";
	};
	class Expansion_Decal_WelcomeToHell: HouseNoDestruct
	{
		scope = 2;
		model = "\DayZExpansion\Objects\Structures\Signs\Decal_WelcomeToHell.p3d";
		physLayer = "item_small";
		carveNavmesh = 0;
	};
};
