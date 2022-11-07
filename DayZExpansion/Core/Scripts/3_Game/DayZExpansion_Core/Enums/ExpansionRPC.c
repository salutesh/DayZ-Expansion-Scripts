/**
 * ExpansionRPC.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

enum ExpansionPartyModuleRPC
{
	INVALID = 20020,
	CreateParty,
	DissolveParty,
	LeaveParty,
	RemovePartyMember,
	UpdatePlayer,
	CreateMarker,
	UpdateMarker,
	UpdatePositionMarker,
	DeleteMarker,
	InvitePlayer,
	DeclineInvite,
	AcceptInvite,
	SyncPlayerInvites,
	UpdateQuickMarker,
	UpdatePermissions,
	COUNT
};

enum ExpansionWorldObjectsModuleRPC
{
	INVALID = 20040,
	RemoveObjects,
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
	RequestTraderItems,
	LoadTraderItems,
	ExitTrader,

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
	ClientPing,
	ControllerSync,
	PlayLockSound,
	COUNT
};

enum ExpansionMarkerRPC
{
	INVALID = 20160,
	CreateDeathMarker,
	COUNT
};

enum ExpansionEntityRPC
{
	INVALID = 20180,
	NetsyncData,
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
	SyncStates,
	SendMessage,
	RequestPlayerStats,
	SendPlayerStats,
	RequestPlayerStates,
	SendPlayerStates,
	RequestPlayerStatsAndStates,
	SendPlayerStatsAndStates,
	SyncLastDeathPos,
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
	SyncPlayerInvites,
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
	RequestPlacePlayerAtTempSafePosition,
	CheckPlayerCooldowns,
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
	Debug,
	Book,
	BaseBuilding,
	Map,
	AddServerMarker,
	RemoveServerMarker,
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
	Log,  //! Not used, settings don't need to be available on client
	NameTags,
	SafeZone,  //! Not used, settings don't need to be available on client
	Mission,  //! Not used, settings don't need to be available on client
	AirDrop,  //! Not used, settings don't need to be available on client
	Quest,
	Chat,
	AI,
	NotificationScheduler,  //! Not used, settings don't need to be available on client
	Hardline,
	Garage,
	COUNT  //! 29 INCREMENT FOLLOWING RPCs WHEN NEEDED FFS
};

enum ExpansionCOTBookModuleRPC
{
	INVALID = 30035,
	Update,
	COUNT
};

enum ExpansionCOTBaseBuildingModuleRPC
{
	INVALID = 30040,
	Update,
	COUNT
};

enum ExpansionCOTMapModuleRPC
{
	INVALID = 30045,
	Update,
	COUNT
};

enum ExpansionCOTMarketModuleRPC
{
	INVALID = 30050,
	Update,
	COUNT
};

enum ExpansionCOTNotificationModuleRPC
{
	INVALID = 30055,
	Update,
	COUNT
};

enum ExpansionCOTGroupModuleRPC
{
	INVALID = 30060,
	EditGroupName,
	EditGroupMemberPerm,
	DeleteGroup,
	ChangeOwner,
	UpdatePermissions,
	InvitePlayer,
	ChangeMoney,
	KickMember,
	SendGroupsToClient,
	RequestGroups,
	SendGroupUpdate,
	Callback,
	CreateMarker,
	COUNT
};

enum ExpansionCOTRaidModuleRPC
{
	INVALID = 30080,
	Update,
	COUNT
};

enum ExpansionCOTSpawnModuleRPC
{
	INVALID = 30100,
	Update,
	COUNT
};

enum ExpansionCOTTerritoryModuleRPC
{
	INVALID = 30120,
	Update,
	COUNT
};

enum ExpansionCOTVehicleModuleRPC
{
	INVALID = 30140,
	Update,
	COUNT
};

enum ExpansionCOTGeneralModuleRPC
{
	INVALID = 30160,
	Update,
	COUNT
};

enum ExpansionCOTSafeZoneModuleRPC
{
	INVALID = 30180,
	Update,
	COUNT
};

enum ExpansionCOTMissionModuleRPC
{
	INVALID = 30200,
	Update,
	COUNT
};

enum ExpansionCOTAirDropModuleRPC
{
	INVALID = 30220,
	Update,
	COUNT
};

enum ExpansionCOTDebugModuleRPC
{
	INVALID = 30240,
	Update,
	COUNT
};

enum ExpansionCraftingModuleRPC
{
	INVALID = 40000,
	RequestPrepareCrafting,
	COUNT
};

enum ExpansionItemBaseModuleRPC
{
	INVALID = 40005,
	PlaySound,
	COUNT
};

//! Misc stuff lives here
enum ExpansionRPC
{
	INVALID = 40010,
	SyncOwnedContainerUID,
	CreateNotification,
	COUNT
};

enum ExpansionQuestModuleRPC
{
	INVALID = 40020,
	SendQuestNPCData,
	RequestPlayerQuests,
	SendPlayerQuests,
	SendPlayerQuestData,
	RequestOpenQuestMenu,
	SetupClientData,
	UpdatePlayerQuestData,
	CreateQuestInstance,
	RequestCompleteQuest,
	CompleteQuest,
	CreateClientMarker,
	RemoveClientMarkers,
	CancelQuest,
	CallbackClient,
	COUNT
};

enum ExpansionGarageModuleRPC
{
	INVALID = 40040,
	RequestPlayerVehicles,
	SendPlayerVehicles,
	DepositVehicleRequest,
	RetrieveVehicleRequest,
	Callback,
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
