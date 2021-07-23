/**
 * ExpansionRPC.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

enum ExpansionPartyModuleRPC
{
	INVALID = 20020,
	Create,
	Dissolve,
	Leave,
	RemovePlayer,
	UpdateClient,

	CreateMarker,
	UpdateMarker,
	UpdatePositionMarker,
	DeleteMarker,
	
	InvitePlayer,
	DeclineInvite,
	AcceptInvite,
	
	SyncPlayersInvites,

	UpdateQuickMarker,
	
	UpdatePermissions,
	
	COUNT
};

enum ExpansionWorldMappingModuleRPC
{
	INVALID = 20040,
	TurnOn,
	TurnOff,
	Load,
	COUNT
};

enum ExpansionMarketModuleRPC
{
	INVALID = 20060,
	
	//! Market
	Callback,
	MoneyDenominations,
	RequestPurchase,
	ConfirmPurchase,
	CancelPurchase,
	RequestSell,
	ConfirmSell,
	CancelSell,
	RequestTraderData,
	LoadTraderData,
	
	//! ATM
	RequestPlayerATMData,
	SendPlayerATMData,
	RequestDepositMoney,
	ConfirmDepositMoney,
	RequestWithdrawMoney,
	ConfirmWithdrawMoney,
	RequestTransferMoneyToPlayer,
	ConfirmTransferMoneyToPlayer,
	RequestPartyTransferMoney,
	ConfirmPartyTransferMoney,
	RequestPartyWithdrawMoney,
	ConfirmPartyWithdrawMoney,
	
	// Admin
	GiveMoney,
	ReserveMoney,
	DeleteMoney,
	UnlockMoney,
	COUNT
};

enum ExpansionMissionCOTModuleRPC
{
	INVALID = 20120,
	Load,
	COUNT
}

enum ExpansionVehicleRPC
{
	INVALID = 20140,
	CrewSync,
	CrewSyncInit,
	RequestCrewSync,
	ClientSync,
	ControllerSync,
	PlayLockSound,
	COUNT
};

enum ExpansionMarkerRPC
{
	INVALID = 20160,
	COUNT
};

enum ExpansionEntityRPC
{
	INVALID = 20180,
	HasBeenKilled,
	COUNT
};

enum ExpansionMarketRPC
{
	INVALID = 20200,
	TraderObject,
	COUNT
};

enum ExpansionMonitorRPC
{
	INVALID = 20300,
	SyncStats,
	SendMessage,
	RequestPlayerStats,
	SendPlayerStats,
	RequestPlayerStates,
	SendPlayerStates
	COUNT
};

enum ExpansionTerritoryModuleRPC
{
	INVALID = 20400,
	OpenFlagMenu,
	CreateTerritory,
	ChangeTexture,
	DeleteTerritoryPlayer,
	DeleteTerritoryAdmin,
	UpdateClient,
	RequestInvitePlayer,
	SyncPlayersInvites,
	AcceptInvite,
	DeclineInvite,
	AddTerritoryFlagToPlayer,
	PromoteMember,
	DemoteMember,
	KickMember,
	RequestServerTerritories,
	SendServerTerritories,
	Leave,
	PlayerEnteredTerritory,
	COUNT
};

enum ExpansionCOTTerritoriesModuleRPC
{
	INVALID = 20450,
	DeleteObject,
	RequestServerTerritories,
	SendServerTerritories,
	RequestTerritoryObjects,
	SendTerritoryObjects,
	TeleportToTerritory,
	RequestUpdateObjectData,
	SendObjectData,
	COUNT
};

enum ExpansionCOTVehiclesModuleRPC
{
	INVALID = 20460,
	RequestServerVehicles,
	SendServerVehicles,
	DeleteVehicle,
	DeleteVehicleAll,
	DeleteVehicleUnclaimed,
	DeleteVehicleDestroyed,
	TeleportToVehicle,
	COUNT
};

enum ExpansionKillFeedModuleRPC
{
	INVALID = 20470,
	SendMessage,
	COUNT
};

enum ExpansionGlobalChatRPC
{
	INVALID = 20500,
	AddChatMessage,
	COUNT
};

enum ExpansionCarKeyRPC
{
	INVALID = 20600,
	RequestItemData,
	SendItemData,
	COUNT
}

enum ExpansionRespawnHandlerModuleRPC
{
	INVALID = 20700,
	ShowSpawnMenu,
	SelectSpawn,
	CloseSpawnMenu,
	COUNT
};

enum ExpansionESPModificationModuleRPC
{
	INVALID = 20720,
	RequestCode,
	RemoveCode,
	CarUnPair,
	CarUnLock,
	COUNT
};

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
	PlayerList,
	SocialMedia,
	Log,

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

enum ExpansionCOTDebugModuleRPC
{
	INVALID = 30095,
	Update,
	COUNT
};

enum ExpansionCraftingModuleRPC
{
	INVALID = 40000,
	RequestPrepareCrafting,
	COUNT
};

//! Misc stuff lives here
enum ExpansionRPC
{
	INVALID = 40010,
	SyncOwnedContainerUID,
	COUNT
};

enum ExpansionLockRPC
{
	LOCK = 5464150, 
	UNLOCK,
	SET,
	SERVERREPLY,
	CHANGE,
	KNOWNUSERS_REQUEST,
	KNOWNUSERS_REPLY
};

enum ExpansionAutoRunRPC
{
	INVALID = 9889130,
	AUTORUNSYNC,
	AUTORUNDISABLE,
	COUNT
};
