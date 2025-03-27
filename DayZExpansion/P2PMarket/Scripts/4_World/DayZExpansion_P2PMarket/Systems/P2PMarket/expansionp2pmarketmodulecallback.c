/**
 * ExpansionP2PMarketModuleCallback.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2025 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

enum ExpansionP2PMarketModuleCallback
{
	None,
	Silent,  //! Silent callback w/o notification (just used to update menu)
	ItemListed,
	ItemPurchased,
	SaleRetrieved,
	AllSalesRetrieved,
	MsgItemGotSold,
	MsgTotalSold,
	Error,
	ErrorListingPriceTooLow,
	ErrorNotEnoughMoney,
	ErrorNotEnoughMoneyToList,
	ErrorVehicleMissingAttachment,
	ErrorVehicleRuinedAttachment,
	ErrorNoVehicleSpawnPosition,
	ErrorVehicleSpawnPositionBlocked,
	ErrorVehicleLockpicked,
	ErrorVehicleKeysMissing,
	ErrorPersistency
};