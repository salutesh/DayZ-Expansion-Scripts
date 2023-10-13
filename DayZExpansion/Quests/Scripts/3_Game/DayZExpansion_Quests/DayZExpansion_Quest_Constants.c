/**
 * DayZExpansion_Quest_Constants.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**
 * Expansion quest system folder paths
 * @{
 */

static const string EXPANSION_QUESTS_MISSION_FOLDER = "$mission:expansion\\quests\\";
static const string EXPANSION_QUESTS_OBJECTS_FOLDER = EXPANSION_QUESTS_MISSION_FOLDER + "\\objects\\";

static const string EXPANSION_QUESTS_FOLDER = EXPANSION_FOLDER + "Quests\\";
static const string EXPANSION_QUESTS_PERSISTENT_SERVER_DATA_FILE = EXPANSION_QUESTS_FOLDER + "PersistentServerData.json";
static const string EXPANSION_QUESTS_SCRIPTS_FILE = EXPANSION_QUESTS_FOLDER + "Scripts.c";
static const string EXPANSION_QUESTS_QUESTS_FOLDER = EXPANSION_QUESTS_FOLDER + "Quests\\";

static const string EXPANSION_QUESTS_OBJECTIVES_FOLDER = EXPANSION_QUESTS_FOLDER + "Objectives\\";
static const string EXPANSION_QUESTS_OBJECTIVES_TRAVEL_FOLDER = EXPANSION_QUESTS_OBJECTIVES_FOLDER + "Travel\\";
static const string EXPANSION_QUESTS_OBJECTIVES_DELIVERY_FOLDER = EXPANSION_QUESTS_OBJECTIVES_FOLDER + "Delivery\\";
static const string EXPANSION_QUESTS_OBJECTIVES_TARGET_FOLDER = EXPANSION_QUESTS_OBJECTIVES_FOLDER + "Target\\";
static const string EXPANSION_QUESTS_OBJECTIVES_COLLECTION_FOLDER = EXPANSION_QUESTS_OBJECTIVES_FOLDER + "Collection\\";
static const string EXPANSION_QUESTS_OBJECTIVES_TREASUREHUNT_FOLDER = EXPANSION_QUESTS_OBJECTIVES_FOLDER + "TreasureHunt\\";
static const string EXPANSION_QUESTS_OBJECTIVES_AIPATROL_FOLDER = EXPANSION_QUESTS_OBJECTIVES_FOLDER + "AIPatrol\\";
static const string EXPANSION_QUESTS_OBJECTIVES_AICAMP_FOLDER = EXPANSION_QUESTS_OBJECTIVES_FOLDER + "AICamp\\";
static const string EXPANSION_QUESTS_OBJECTIVES_AIVIP_FOLDER = EXPANSION_QUESTS_OBJECTIVES_FOLDER + "AIVIP\\";
static const string EXPANSION_QUESTS_OBJECTIVES_ACTION_FOLDER = EXPANSION_QUESTS_OBJECTIVES_FOLDER + "Action\\";
static const string EXPANSION_QUESTS_OBJECTIVES_CRAFTING_FOLDER = EXPANSION_QUESTS_OBJECTIVES_FOLDER + "Crafting\\";

static const string EXPANSION_QUESTS_AI_LAYOUTS_FOLDER = EXPANSION_QUESTS_FOLDER + "AILayouts\\"; // root directory for PatrolSettings
static const string EXPANSION_QUESTS_AI_SETTINGS = EXPANSION_QUESTS_AI_LAYOUTS_FOLDER + "QuestPatrolSettings.json"; // json for waypoints and loadouts

static const string EXPANSION_QUESTS_NPCS_FOLDER = EXPANSION_QUESTS_FOLDER + "NPCs\\";
static const string EXPANSION_QUESTS_LAYOUTS_FOLDER = EXPANSION_QUESTS_FOLDER + "Layouts\\";
static const string EXPANSION_QUESTS_PLAYERDATA_FOLDER = EXPANSION_QUESTS_FOLDER + "PlayerData\\";
static const string EXPANSION_QUESTS_GROUPDATA_FOLDER = EXPANSION_QUESTS_FOLDER + "GroupData\\";
static const string EXPANSION_QUEST_SETTINGS = EXPANSION_SETTINGS_FOLDER + "QuestSettings.json";