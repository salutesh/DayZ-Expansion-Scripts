/**
 * ExpansionTerritoryModuleRPC.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

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
	AddTerritoryFlagToPlayer,
	PromoteMember,
	DemoteMember,
	KickMember,
	RequestServerTerritories,
	SendServerTerritories,
	Leave,
	PlayerEnteredTerritory,
    COUNT
}