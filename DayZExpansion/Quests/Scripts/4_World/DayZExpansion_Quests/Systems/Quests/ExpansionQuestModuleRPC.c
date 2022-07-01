/**
 * ExpansionQuestObjectiveType.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

enum ExpansionQuestModuleRPC
{
	INVALID = 60000,
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