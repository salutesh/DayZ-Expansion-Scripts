/**
 * ExpansionQuestConfig.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

enum ExpansionQuestRewardBehavior
{
	RANDOMIZED_ON_COMPLETION = 0,
	RANDOMIZED_ON_START = 1
};

class ExpansionQuestConfigBase
{
	int ConfigVersion;
	//! Main parameters
	int ID = -1; //! Unique quest ID.
	int Type = ExpansionQuestType.NORMAL; //! Quest type from ExpansionQuestType.
	string Title;	//! Quest title.
	ref array<string> Descriptions; //! 0 - Description on getting quest | 1 - Description while quest is active | 2 - Description when take in quest.
	string ObjectiveText; //! Short objective desctiption.
	int FollowUpQuest = -1; //! Follow-up Quest ID.

	//! Additional quest logic controll parameters
	bool Repeatable = false; //! Quest is a repeatable quest. Currently every player has a individual cooldown that resets at the exact same time when the timestamp is created. Might want to change that to a fixed reset time for every player.
	bool IsDailyQuest = false; //! Quest is daylie quest and has a daylie reset. Currently every player has a individual cooldown that resets at the exact same time when the timestamp is created. Might want to change that to a fixed reset time for every player.
	bool IsWeeklyQuest = false; //! Quest is a weekly quest and has a weekly cooldown.
	bool CancelQuestOnPlayerDeath = false; //! Quest will be cancled if the quest player (or one of his group members when its a group quest) dies.
	bool Autocomplete = false; //! Quest will be autocompleted when all quest ojectives are completed.
	bool IsGroupQuest = false; //! Quest is a group quest.

	string ObjectSetFileName = string.Empty; //! File name of the .map file that will get loaded

	ref array<ref ExpansionQuestItemConfig> QuestItems; //! Quest items that the player will recive when starting the quest.
	ref array<ref ExpansionQuestRewardConfig> Rewards; //! Quest rewards that the player will revice when turning in the quest and all objectives are completed.

	bool NeedToSelectReward = false; //! If there is more then one item in the Rewards array and this config param is set to true the player needs to select a reward item on quest competion from the given items in the Rewards array.	
	bool RandomReward = false; //! Only one reward is randomly selected and given to the player from the configured reward items on quest completion.
	int RandomRewardAmount = -1; //! Controlls amount of reward items that get randomly selected and given to the player on quest completion if RandomReward boolean is true.
	bool RewardsForGroupOwnerOnly = true; //! If the quest is a group quest this option controlls if all group players get the reward or ownly the group owner.
	ExpansionQuestRewardBehavior RewardBehavior = 0;

	ref array<int> QuestGiverIDs; //! NPC IDs of the NPCs that will head out the quest.
	ref array<int> QuestTurnInIDs;	//! NPC IDs of the NPCs where players can turn in the quest when completed.

	void ExpansionQuestConfigBase()
	{
		Descriptions = new array<string>;
		QuestItems = new array<ref ExpansionQuestItemConfig>;
		Rewards = new array<ref ExpansionQuestRewardConfig>;
		QuestGiverIDs = new array<int>;
		QuestTurnInIDs = new array<int>;
	}
};

class ExpansionQuestConfigV5Base: ExpansionQuestConfigBase
{
	bool IsAchivement = false;
}

class ExpansionQuestConfigV5: ExpansionQuestConfigV5Base
{
	int PreQuest = -1; //! Pre-Quest Quest ID.

	bool IsBanditQuest = false; //! Quest for bandits only
	bool IsHeroQuest = false; //! Quest for heros only

	ref array<ref ExpansionQuestObjectiveConfig> Objectives; //! Quest objectives that the player need to complete to get the quest rewards.

	int HumanityReward = 0; //! Reputation reward when completing the quest.

	void ExpansionQuestConfigV5()
	{
		Objectives = new array<ref ExpansionQuestObjectiveConfig>;
	}
};

class ExpansionQuestConfigV15Base: ExpansionQuestConfigBase
{
	bool IsAchievement = false;

	ref array<ref ExpansionQuestObjectiveConfigBase> Objectives; //! Quest objectives that the player need to complete to get the quest rewards.

	int QuestColor = 0;  //! Main color that will be used in all the quest menus and interfaces for this quest.

	int ReputationReward = 0; //! Reputation reward when completing the quest.
	int ReputationRequirement = -1; //! Reputation needed to see and start the quest.

	ref array<int> PreQuestIDs; //! Pre-Quest Quest IDs of the quest the player need to have completed to accept this quest.

	string RequiredFaction = ""; //! Player need to be part of this faction to start this quest.
	string FactionReward = ""; //! Player faction will be set to the given faction on quest completion.

	bool PlayerNeedQuestItems = true; //! Quest will be cancled if the quest player is missing one of the quest items on relog/reconnect.
	bool DeleteQuestItems = true; //! Controlls if the quest items will be deleted when the quest is completed or not. They still always get deleted when the quest is cancled.
	
	void ExpansionQuestConfigV15Base()
	{
		Objectives = new array<ref ExpansionQuestObjectiveConfigBase>;
		PreQuestIDs = new array<int>;
	}
};
	
class ExpansionQuestConfigV19Base: ExpansionQuestConfigV15Base
{
	bool SequentialObjectives = true;
};

class ExpansionQuestConfigV20Base: ExpansionQuestConfigV19Base
{
	ref map<string, int> FactionReputationRequirements; //! Faction reputation points requirement the players will need to see and accept the quest.
	ref map<string, int> FactionReputationRewards; //! Faction reputation points the players will gain on quest completion.

	void ExpansionQuestConfigV20Base()
	{
		FactionReputationRequirements = new map<string, int>;
		FactionReputationRewards = new map<string, int>;
	}
};

class ExpansionQuestConfig: ExpansionQuestConfigV20Base
{
	static const int CONFIGVERSION = 22;
	
	bool SuppressQuestLogOnCompetion = false;
	bool Active = true;
	
	void ExpansionQuestConfig()
	{
		ConfigVersion = CONFIGVERSION;
		
		FactionReputationRequirements = new map<string, int>;
		FactionReputationRewards = new map<string, int>;
	}

	void ~ExpansionQuestConfig()
	{
		if (GetGame())
			EXPrint(ToString() + "~ExpansionQuestConfig");
	}

	void SetType(int type)
	{
		Type = type;
	}

	int GetType()
	{
		return Type;
	}

	void SetID(int id)
	{
		ID = id;
	}

	int GetID()
	{
		return ID;
	}

	void SetTitle(string title)
	{
		Title = title;
	}

	string GetTitle()
	{
		return Title;
	}

	void SetDescriptions(array<string> descriptions)
	{
		Descriptions.InsertAll(descriptions);
	}

	void SetObjectiveText(string text)
	{
		ObjectiveText = text;
	}

	string GetObjectiveText()
	{
		return ObjectiveText;
	}

	array<string> GetDescriptions()
	{
		return Descriptions;
	}

	void AddObjectiveConfig(ExpansionQuestObjectiveConfigBase config)
	{
		int type = config.GetObjectiveType();
		switch (type)
		{
			case ExpansionQuestObjectiveType.DELIVERY:
			{
				ExpansionQuestObjectiveDeliveryConfig deliveryConfig;
				if (Class.CastTo(deliveryConfig, config))
					Objectives.Insert(deliveryConfig);
			}
			break;

			case ExpansionQuestObjectiveType.TRAVEL:
			{
				ExpansionQuestObjectiveTravelConfig travelConfig;
				if (Class.CastTo(travelConfig, config))
					Objectives.Insert(travelConfig);
			}
			break;

			case ExpansionQuestObjectiveType.TARGET:
			{
				ExpansionQuestObjectiveTargetConfig targetConfig;
				if (Class.CastTo(targetConfig, config))
					Objectives.Insert(targetConfig);
			}
			break;

			case ExpansionQuestObjectiveType.COLLECT:
			{
				ExpansionQuestObjectiveCollectionConfig collectionConfig;
				if (Class.CastTo(collectionConfig, config))
					Objectives.Insert(collectionConfig);
			}
			break;

			case ExpansionQuestObjectiveType.TREASUREHUNT:
			{
				ExpansionQuestObjectiveTreasureHuntConfig treasureHuntConfig;
				if (Class.CastTo(treasureHuntConfig, config))
					Objectives.Insert(treasureHuntConfig);
			}
			break;

			case ExpansionQuestObjectiveType.ACTION:
			{
				ExpansionQuestObjectiveActionConfig actionConfig;
				if (Class.CastTo(actionConfig, config))
					Objectives.Insert(actionConfig);
			}
			break;

			case ExpansionQuestObjectiveType.CRAFTING:
			{
				ExpansionQuestObjectiveCraftingConfig craftingConfig;
				if (Class.CastTo(craftingConfig, config))
					Objectives.Insert(craftingConfig);
			}
			break;

		#ifdef EXPANSIONMODAI
			case ExpansionQuestObjectiveType.AIPATROL:
			{
				ExpansionQuestObjectiveAIPatrolConfig aiPatrolConfig;
				if (Class.CastTo(aiPatrolConfig, config))
					Objectives.Insert(aiPatrolConfig);
			}
			break;

			case ExpansionQuestObjectiveType.AICAMP:
			{
				ExpansionQuestObjectiveAICampConfig aiCampConfig;
				if (Class.CastTo(aiCampConfig, config))
					Objectives.Insert(aiCampConfig);
			}
			break;

			case ExpansionQuestObjectiveType.AIESCORT:
			{
				ExpansionQuestObjectiveAIEscortConfig aiEscortConfig;
				if (Class.CastTo(aiEscortConfig, config))
					Objectives.Insert(aiEscortConfig);
			}
			break;
		#endif
		}
	}

	array<ref ExpansionQuestObjectiveConfigBase> GetObjectives()
	{
		return Objectives;
	}

	void SetQuestItems(array<ExpansionQuestItemConfig> items)
	{
		QuestItems.InsertAll(items);
	}

	array<ref ExpansionQuestItemConfig> GetQuestItems()
	{
		return QuestItems;
	}

	void SetRewards(array<ExpansionQuestRewardConfig> rewards)
	{
		Rewards.InsertAll(rewards);
	}

	array<ref ExpansionQuestRewardConfig> GetRewards()
	{
		return Rewards;
	}

	void AddPreQuestID(int id)
	{
		if (PreQuestIDs.Find(id) == -1)
			PreQuestIDs.Insert(id);
	}

	array<int> GetPreQuestIDs()
	{
		return PreQuestIDs;
	}

	void SetFollowUpQuestID(int id)
	{
		FollowUpQuest = id;
	}

	int GetFollowUpQuestID()
	{
		return FollowUpQuest;
	}

	void AddQuestGiverID(int questGiverID)
	{
		if (QuestGiverIDs.Find(questGiverID) == -1)
			QuestGiverIDs.Insert(questGiverID);
	}

	array<int> GetQuestGiverIDs()
	{
		return QuestGiverIDs;
	}

	void AddQuestTurnInID(int questTurnInID)
	{
		if (QuestTurnInIDs.Find(questTurnInID) == -1)
			QuestTurnInIDs.Insert(questTurnInID);
	}

	array<int> GetQuestTurnInIDs()
	{
		return QuestTurnInIDs;
	}

	void SetIsAchievement(bool achievement)
	{
		IsAchievement = achievement;
	}

	bool IsAchievement()
	{
		return IsAchievement;
	}

	void SetIsRepeatable(bool repeatable)
	{
		Repeatable = repeatable;
	}

	bool IsRepeatable()
	{
		return Repeatable;
	}

	void SetIsGroupQuest(bool groupQuest)
	{
		IsGroupQuest = groupQuest;
	}

	bool IsGroupQuest()
	{
	#ifdef EXPANSIONMODGROUPS
		return IsGroupQuest;
	#else
		return false;
	#endif
	}

	void SetIsDailyQuest(bool daylie)
	{
		IsDailyQuest = daylie;
	}

	bool IsDailyQuest()
	{
		return IsDailyQuest;
	}

	void SetIsWeeklyQuest(bool weekly)
	{
		IsWeeklyQuest = weekly;
	}

	bool IsWeeklyQuest()
	{
		return IsWeeklyQuest;
	}

	void SetObjectSetFileName(string fileName)
	{
		ObjectSetFileName = fileName;
	}

	string GetObjectSetFileName()
	{
		return ObjectSetFileName;
	}

	void SetAutocomplete(bool state)
	{
		Autocomplete = state;
	}

	bool IsAutocomplete()
	{
		return Autocomplete;
	}

	void SetCancelQuestOnPlayerDeath(bool state)
	{
		CancelQuestOnPlayerDeath = state;
	}

	bool CancelQuestOnPlayerDeath()
	{
		return CancelQuestOnPlayerDeath;
	}

	void SetNeedToSelectReward(bool state)
	{
		NeedToSelectReward = state;
	}

	bool NeedToSelectReward()
	{
		return NeedToSelectReward;
	}
	
	void SetRandomReward(bool state)
	{
		RandomReward = state;
	}
	
	bool RandomReward()
	{
		return RandomReward;
	}
	
	void SetRandomRewardAmount(int amount)
	{
		RandomRewardAmount = amount;
	}
	
	int GetRandomRewardAmount()
	{
		return RandomRewardAmount;
	}
	
	void SetRewardBehavior(ExpansionQuestRewardBehavior rewardBehavior)
	{
		RewardBehavior = rewardBehavior;
	}
	
	ExpansionQuestRewardBehavior GetRewardBehavior()
	{
		return RewardBehavior;
	}

	void SetRewardForGroupOwnerOnly(bool state)
	{
		RewardsForGroupOwnerOnly = state;
	}

	bool RewardsForGroupOwnerOnly()
	{
		return RewardsForGroupOwnerOnly;
	}

	void SetReputationReward(int rep)
	{
		ReputationReward = rep;
	}

	int GetReputationReward()
	{
		return ReputationReward;
	}

	void SetReputationRequirement(int rep)
	{
		ReputationRequirement = rep;
	}

	int GetReputationRequirement()
	{
		return ReputationRequirement;
	}

	void SetQuestColor(int color)
	{
		QuestColor = color;
	}

	int GetQuestColor()
	{
		return QuestColor;
	}

	void SetRequiredFaction(string factionName)
	{
		RequiredFaction = factionName;
	}

	string GetRequiredFaction()
	{
		return RequiredFaction;
	}

	void SetFactionReward(string factionName)
	{
		FactionReward = factionName;
	}

	string GetFactionReward()
	{
		return FactionReward;
	}
	
	void AddFactionReputationRequirement(string factionType, int reputation)
	{
		if (!FactionReputationRequirements[factionType])
			FactionReputationRequirements[factionType] = reputation;
	}
	
	map<string, int> GetFactionReputationRequirements()
	{
		return FactionReputationRequirements;
	}
	
	void AddFactionReputationReward(string factionType, int reputation)
	{
		if (!FactionReputationRewards[factionType])
			FactionReputationRewards[factionType] = reputation;
	}
	
	map<string, int> GetFactionReputationRewards()
	{
		return FactionReputationRewards;
	}

	void SetNeedQuestItems(bool state)
	{
		PlayerNeedQuestItems = state;
	}

	bool NeedQuestItems()
	{
		return PlayerNeedQuestItems;
	}

	void SetDeleteQuestItems(bool state)
	{
		DeleteQuestItems = state;
	}

	bool DeleteQuestItems()
	{
		return DeleteQuestItems;
	}
	
	void SetSequentialObjectives(bool state)
	{
		SequentialObjectives = state;
	}
	
	bool SequentialObjectives()
	{
		return SequentialObjectives;
	}
	
	bool IsActive()
	{
		return Active;
	}
	
	bool SuppressQuestLogOnCompetion()
	{
		return SuppressQuestLogOnCompetion;
	}

	static ExpansionQuestConfig Load(string fileName)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, ExpansionQuestConfig);

		bool save;
		EXTrace.Print(EXTrace.QUESTS, null, "Load existing configuration file:" + fileName);

		ExpansionQuestConfig questConfig;
		ExpansionQuestConfigV20Base questConfigBase;
		int j;

		if (!ExpansionJsonFileParser<ExpansionQuestConfigV20Base>.Load(EXPANSION_QUESTS_QUESTS_FOLDER + fileName, questConfigBase))
			return NULL;

		if (questConfigBase.ConfigVersion < CONFIGVERSION)
		{
			EXTrace.Print(EXTrace.QUESTS, null, "Convert existing configuration file:" + EXPANSION_QUESTS_QUESTS_FOLDER + fileName + " to version " + CONFIGVERSION);
			questConfig = new ExpansionQuestConfig();

			//! Copy over old configuration that haven't changed
			questConfig.CopyConfig(questConfigBase);

			if (questConfigBase.ConfigVersion < 6)
			{
				ExpansionQuestConfigV5 questConfigv5;
				if (ExpansionJsonFileParser<ExpansionQuestConfigV5>.Load(EXPANSION_QUESTS_QUESTS_FOLDER + fileName, questConfigv5))
				{
					questConfig.ReputationReward = questConfigv5.HumanityReward;

					if (questConfigv5.PreQuest != -1)
						questConfig.AddPreQuestID(questConfigv5.PreQuest);
				}

				questConfig.SetType(ExpansionQuestType.NORMAL);
			}

			if (questConfigBase.ConfigVersion == 7)
			{
				//! v7 variable HealthPercent was treated as damage (so 100% HealthPercent actually meant 100% damage or 0% health).
				//! Variable was renamed to DamagePercent to clarify usage.
				for (j = questConfig.Rewards.Count() - 1; j >= 0; j--)
				{
					ExpansionQuestRewardConfigV1 rewardV1 = questConfig.Rewards[j];
					ExpansionQuestRewardConfig convertedRewardV8 = new ExpansionQuestRewardConfig();
					convertedRewardV8.Copy(rewardV1);

					convertedRewardV8.DamagePercent = rewardV1.HealthPercent;

					questConfig.Rewards.RemoveOrdered(j);
					questConfig.Rewards.InsertAt(convertedRewardV8, j);
				}
			}

			if (questConfigBase.ConfigVersion < 13)
			{
				foreach (ExpansionQuestRewardConfig rewardConfig: questConfig.Rewards)
				{
					rewardConfig.QuestID = -1;
				}
			}

			if (questConfigBase.ConfigVersion < 15)
			{
				ExpansionQuestConfigV5Base questConfigV5Base;
				if (ExpansionJsonFileParser<ExpansionQuestConfigV5Base>.Load(EXPANSION_QUESTS_QUESTS_FOLDER + fileName, questConfigV5Base))
					questConfig.IsAchievement = questConfigV5Base.IsAchivement;
			}
			
			if (questConfigBase.ConfigVersion < 18)
			{
				for (j = questConfig.Rewards.Count() - 1; j >= 0; j--)
				{
					ExpansionQuestRewardConfig rewardConfigV17 = questConfig.Rewards[j];
					rewardConfigV17.SetChance(1.0);
				}
			}
			
			if (questConfigBase.ConfigVersion < 21)
			{
				questConfig.SuppressQuestLogOnCompetion = false;
				questConfig.Active = true;
			}

			//! Update quest configuration objectives config version.
			UpdateQuestObjectivesConfigs(questConfig);

			questConfig.ConfigVersion = CONFIGVERSION;
			save = true;
		}
		else
		{
			if (!ExpansionJsonFileParser<ExpansionQuestConfig>.Load(EXPANSION_QUESTS_QUESTS_FOLDER + fileName, questConfig))
				return NULL;

			//! Update quest configuration objectives config version.
			UpdateQuestObjectivesConfigs(questConfig, fileName);
		}

		if (save)
			questConfig.Save(fileName);
		
		//! Preperations for config behavior handling after data was loaded
		OnLoad(questConfig);

		return questConfig;
	}
	
	protected static void OnLoad(ExpansionQuestConfig questConfig)
	{
		if (questConfig.GetRewardBehavior() == ExpansionQuestRewardBehavior.RANDOMIZED_ON_START)
		{
			array<ref ExpansionQuestRewardConfig> questRewards = questConfig.GetRewards();
			array<ExpansionQuestRewardConfig> randomRewards = new array<ExpansionQuestRewardConfig>;
			
			int index = -1;
			int lootItemsSelected = 0;
			array<float> chances = new array<float>;
			foreach (ExpansionQuestRewardConfig rewardConfig: questRewards)
			{
				chances.Insert(rewardConfig.GetChance());
			}

			int itemCount = questConfig.GetRandomRewardAmount();
			ExpansionQuestRewardConfig radomReward;
	
			if (itemCount > 0)
			{
				while (lootItemsSelected < itemCount)
				{
					index = ExpansionStatic.GetWeightedRandom(chances);
					if (index > -1)
					{
						radomReward = questRewards[index];
						randomRewards.Insert(radomReward);
						lootItemsSelected++;
					}
					else
					{
						break;
					}
				}
			}
			else
			{
				index = ExpansionStatic.GetWeightedRandom(chances);
				if (index > -1)
				{
					radomReward = questRewards[index];
					randomRewards.Insert(radomReward);
				}
			}
			
			questConfig.Rewards.Clear();
			
			foreach (ExpansionQuestRewardConfig selectedReward: randomRewards)
			{
				questConfig.Rewards.Insert(selectedReward);
			}
		}
	}

	protected static void UpdateQuestObjectivesConfigs(ExpansionQuestConfig questConfig, string fileName = string.Empty)
	{
		if (questConfig.Objectives && questConfig.Objectives.Count() > 0)
		{
			bool updated;
			foreach (ExpansionQuestObjectiveConfigBase objectiveConfig: questConfig.Objectives)
			{
				if (objectiveConfig.ConfigVersion < ExpansionQuestObjectiveConfig.CONFIGVERSION)
				{
					objectiveConfig.ConfigVersion = ExpansionQuestObjectiveConfig.CONFIGVERSION;
					updated = true;
				}
			}

			if (updated && fileName != string.Empty)
				questConfig.Save(fileName);
		}
	}

	bool ValidateQuestConfiguration(string fileName)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		bool save;

		array<ref ExpansionQuestObjectiveConfigBase> questObjectives = GetObjectives();
		//! Validate quest objectives
		if (!questObjectives || questObjectives && questObjectives.Count() == 0)
		{
			Error("ExpansionQuestConfig::ValidateQuestConfiguration - Quest configuration with ID:" + GetID() + " has no configured objectives!");
			return false;
		}

		bool objectivesValid = true;
		int aiObjectivesCount;
		for (int i = 0; i < questObjectives.Count(); i++)
		{
			ExpansionQuestObjectiveConfigBase objectiveConfig = questObjectives[i];
			int objectiveID = objectiveConfig.GetID();
			ExpansionQuestObjectiveType objectiveType = objectiveConfig.GetObjectiveType();
			string objectiveTypeName = typename.EnumToString(ExpansionQuestObjectiveType, objectiveType);

			switch (objectiveType)
			{
				case ExpansionQuestObjectiveType.DELIVERY:
				{
					ExpansionQuestObjectiveDeliveryConfig deliveryConfig;
					if (Class.CastTo(deliveryConfig, objectiveConfig))
					{
						if (!deliveryConfig.Validate())
						{
							Error("ExpansionQuestConfig::ValidateQuestConfiguration - Validation check for quest objective configuration with ID:" + objectiveID + " and type " + objectiveTypeName + " failed! Please check the objective configuration!");
							deliveryConfig.QuestDebug();
							objectivesValid = false;
						}

						ExpansionQuestObjectiveDeliveryConfig moduleDeliveryConfig = ExpansionQuestModule.GetModuleInstance().GetDeliveryObjectiveConfigByID(objectiveID);
						if (!moduleDeliveryConfig || !moduleDeliveryConfig.Validate())
						{
							Error("ExpansionQuestConfig::ValidateQuestConfiguration - Validation check for quest objective configuration with ID:" + objectiveID + " and type " + objectiveTypeName + " failed! Please check the objective configuration!");
							moduleDeliveryConfig.QuestDebug();
							objectivesValid = false;
						}
					}
				}
				break;

				case ExpansionQuestObjectiveType.TRAVEL:
				{
					ExpansionQuestObjectiveTravelConfig travelConfig;
					if (Class.CastTo(travelConfig, objectiveConfig))
					{
						if (!travelConfig.Validate())
						{
							Error("ExpansionQuestConfig::ValidateQuestConfiguration - Validation check for quest objective configuration with ID:" + objectiveID + " and type " + objectiveTypeName + " failed! Please check the objective configuration!");
							travelConfig.QuestDebug();
							objectivesValid = false;
						}

						ExpansionQuestObjectiveTravelConfig moduleTravelConfig = ExpansionQuestModule.GetModuleInstance().GetTravelObjectiveConfigByID(objectiveID);
						if (!moduleTravelConfig || !moduleTravelConfig.Validate())
						{
							Error("ExpansionQuestConfig::ValidateQuestConfiguration - Validation check for quest objective configuration with ID:" + objectiveID + " and type " + objectiveTypeName + " failed! Please check the objective configuration!");
							moduleTravelConfig.QuestDebug();
							objectivesValid = false;
						}
					}
				}
				break;

				case ExpansionQuestObjectiveType.TARGET:
				{
					ExpansionQuestObjectiveTargetConfig targetConfig;
					if (Class.CastTo(targetConfig, objectiveConfig))
					{
						if (!targetConfig.Validate())
						{
							Error("ExpansionQuestConfig::ValidateQuestConfiguration - Validation check for quest objective configuration with ID:" + objectiveID + " and type " + objectiveTypeName + " failed! Please check the objective configuration!");
							targetConfig.QuestDebug();
							objectivesValid = false;
						}

						ExpansionQuestObjectiveTargetConfig moduleTargetConfig = ExpansionQuestModule.GetModuleInstance().GetTargetObjectiveConfigByID(objectiveID);
						if (!moduleTargetConfig || !moduleTargetConfig.Validate())
						{
							Error("ExpansionQuestConfig::ValidateQuestConfiguration - Validation check for quest objective configuration with ID:" + objectiveID + " and type " + objectiveTypeName + " failed! Please check the objective configuration!");
							moduleTargetConfig.QuestDebug();
							objectivesValid = false;
						}
					}
				}
				break;

				case ExpansionQuestObjectiveType.COLLECT:
				{
					ExpansionQuestObjectiveCollectionConfig collectionConfig;
					if (Class.CastTo(collectionConfig, objectiveConfig))
					{
						if (!collectionConfig.Validate())
						{
							Error("ExpansionQuestConfig::ValidateQuestConfiguration - Validation check for quest objective configuration with ID:" + objectiveID + " and type " + objectiveTypeName + " failed! Please check the objective configuration!");
							collectionConfig.QuestDebug();
							objectivesValid = false;
						}

						ExpansionQuestObjectiveCollectionConfig moduleCollectConfig = ExpansionQuestModule.GetModuleInstance().GetCollectionObjectiveConfigByID(objectiveID);
						if (!moduleCollectConfig || !moduleCollectConfig.Validate())
						{
							Error("ExpansionQuestConfig::ValidateQuestConfiguration - Validation check for quest objective configuration with ID:" + objectiveID + " and type " + objectiveTypeName + " failed! Please check the objective configuration!");
							moduleCollectConfig.QuestDebug();
							objectivesValid = false;
						}
					}
				}
				break;

				case ExpansionQuestObjectiveType.TREASUREHUNT:
				{
					ExpansionQuestObjectiveTreasureHuntConfig treasureHuntConfig;
					if (Class.CastTo(treasureHuntConfig, objectiveConfig))
					{
						if (!treasureHuntConfig.Validate())
						{
							Error("ExpansionQuestConfig::ValidateQuestConfiguration - Validation check for quest objective configuration with ID:" + objectiveID + " and type " + objectiveTypeName + " failed! Please check the objective configuration!");
							treasureHuntConfig.QuestDebug();
							objectivesValid = false;
						}

						ExpansionQuestObjectiveTreasureHuntConfig moduleTreasureHuntConfig = ExpansionQuestModule.GetModuleInstance().GetTreasureHuntObjectiveConfigByID(objectiveID);
						if (!moduleTreasureHuntConfig || !moduleTreasureHuntConfig.Validate())
						{
							Error("ExpansionQuestConfig::ValidateQuestConfiguration - Validation check for quest objective configuration with ID:" + objectiveID + " and type " + objectiveTypeName + " failed! Please check the objective configuration!");
							moduleTreasureHuntConfig.QuestDebug();
							objectivesValid = false;
						}
					}
				}
				break;

				case ExpansionQuestObjectiveType.ACTION:
				{
					ExpansionQuestObjectiveActionConfig actionConfig;
					if (Class.CastTo(actionConfig, objectiveConfig))
					{
						if (!actionConfig.Validate())
						{
							Error("ExpansionQuestConfig::ValidateQuestConfiguration - Validation check for quest objective configuration with ID:" + objectiveID + " and type " + objectiveTypeName + " failed! Please check the objective configuration!");
							actionConfig.QuestDebug();
							objectivesValid = false;
						}

						ExpansionQuestObjectiveActionConfig moduleActionConfig = ExpansionQuestModule.GetModuleInstance().GetActionObjectiveConfigByID(objectiveID);
						if (!moduleActionConfig || !moduleActionConfig.Validate())
						{
							Error("ExpansionQuestConfig::ValidateQuestConfiguration - Validation check for quest objective configuration with ID:" + objectiveID + " and type " + objectiveTypeName + " failed! Please check the objective configuration!");
							moduleActionConfig.QuestDebug();
							objectivesValid = false;
						}
					}
				}
				break;

				case ExpansionQuestObjectiveType.CRAFTING:
				{
					ExpansionQuestObjectiveCraftingConfig craftingConfig;
					if (Class.CastTo(craftingConfig, objectiveConfig))
					{
						if (!craftingConfig.Validate())
						{
							Error("ExpansionQuestConfig::ValidateQuestConfiguration - Validation check for quest objective configuration with ID:" + objectiveID + " and type " + objectiveTypeName + " failed! Please check the objective configuration!");
							craftingConfig.QuestDebug();
							objectivesValid = false;
						}

						ExpansionQuestObjectiveCraftingConfig moduleCraftingConfig = ExpansionQuestModule.GetModuleInstance().GetCraftingObjectiveConfigByID(objectiveID);
						if (!moduleCraftingConfig || !moduleCraftingConfig.Validate())
						{
							Error("ExpansionQuestConfig::ValidateQuestConfiguration - Validation check for quest objective configuration with ID:" + objectiveID + " and type " + objectiveTypeName + " failed! Please check the objective configuration!");
							moduleCraftingConfig.QuestDebug();
							objectivesValid = false;
						}
					}
				}
				break;

			#ifdef EXPANSIONMODAI
				case ExpansionQuestObjectiveType.AIPATROL:
				{
					ExpansionQuestObjectiveAIPatrolConfig aiPatrolConfig;
					if (Class.CastTo(aiPatrolConfig, objectiveConfig))
					{
						if (!aiPatrolConfig.Validate())
						{
							Error("ExpansionQuestConfig::ValidateQuestConfiguration - Validation check for quest objective configuration with ID:" + objectiveID + " and type " + objectiveTypeName + " failed! Please check the objective configuration!");
							aiPatrolConfig.QuestDebug();
							objectivesValid = false;
						}

						ExpansionQuestObjectiveAIPatrolConfig moduleAIPatrolConfig = ExpansionQuestModule.GetModuleInstance().GetAIPatrolObjectiveConfigByID(objectiveID);
						if (!moduleAIPatrolConfig || !moduleAIPatrolConfig.Validate())
						{
							Error("ExpansionQuestConfig::ValidateQuestConfiguration - Validation check for quest objective configuration with ID:" + objectiveID + " and type " + objectiveTypeName + " failed! Please check the objective configuration!");
							moduleAIPatrolConfig.QuestDebug();
							objectivesValid = false;
						}

						aiObjectivesCount++;
					}
				}
				break;

				case ExpansionQuestObjectiveType.AICAMP:
				{
					ExpansionQuestObjectiveAICampConfig aiCampConfig;
					if (Class.CastTo(aiCampConfig, objectiveConfig))
					{
						if (!aiCampConfig.Validate())
						{
							Error("ExpansionQuestConfig::ValidateQuestConfiguration - Validation check for quest objective configuration with ID: " + objectiveID + " and type " + objectiveTypeName + " failed! Please check the objective configuration!");
							aiCampConfig.QuestDebug();
							objectivesValid = false;
						}

						ExpansionQuestObjectiveAICampConfig moduleAICampConfig = ExpansionQuestModule.GetModuleInstance().GetAICampObjectiveConfigByID(objectiveID);
						if (!moduleAICampConfig || !moduleAICampConfig.Validate())
						{
							Error("ExpansionQuestConfig::ValidateQuestConfiguration - Validation check for quest objective configuration with ID:" + objectiveID + " and type " + objectiveTypeName + " failed! Please check the objective configuration!");
							moduleAICampConfig.QuestDebug();
							objectivesValid = false;
						}

						aiObjectivesCount++;
					}
				}
				break;

				case ExpansionQuestObjectiveType.AIESCORT:
				{
					ExpansionQuestObjectiveAIEscortConfig aiEscortConfig;
					if (Class.CastTo(aiEscortConfig, objectiveConfig))
					{
						if (!aiEscortConfig.Validate())
						{
							Error("ExpansionQuestConfig::ValidateQuestConfiguration - Validation check for quest objective configuration with ID: " + objectiveID + " and type " + objectiveTypeName + " failed! Please check the objective configuration!");
							aiEscortConfig.QuestDebug();
							objectivesValid = false;
						}

						ExpansionQuestObjectiveAIEscortConfig moduleAIEscortConfig = ExpansionQuestModule.GetModuleInstance().GetAIEscortObjectiveConfigByID(objectiveID);
						if (!moduleAIEscortConfig || !moduleAIEscortConfig.Validate())
						{
							Error("ExpansionQuestConfig::ValidateQuestConfiguration - Validation check for quest objective configuration with ID:" + objectiveID + " and type " + objectiveTypeName + " failed! Please check the objective configuration!");
							moduleAIEscortConfig.QuestDebug();
							objectivesValid = false;
						}

						aiObjectivesCount++;
					}
				}
				break;
			#endif
			}
		}

		if (!objectivesValid)
			return false;

	#ifdef EXPANSIONMODAI
		if (aiObjectivesCount > 1)
		{
			Error("ExpansionQuestConfig::ValidateQuestConfiguration - Validation check for quest with ID: " + GetID() + " failed! Multiple AI objectives in one quest configuration are not allowed!");
			return false;
		}
	#endif

		//! Make sure quest has repeatable parameter if it is a weekly/daily quest.
		if ((IsWeeklyQuest() || IsDailyQuest()) && !IsRepeatable())
		{
			EXTrace.Print(EXTrace.QUESTS, this, "Quest configuration with ID:" + GetID() + " is a Daily/Weekly quest but not flaged as repeatable. Repeatable flag gets applied and file saved!");
			SetIsRepeatable(true);
			save = true;
		}

		if (save)
			Save(fileName);

		return true;
	}

	void Save(string fileName)
	{
		if (!ExpansionString.EndsWithIgnoreCase(fileName, ".json"))
			fileName += ".json";

		ExpansionJsonFileParser<ExpansionQuestConfig>.Save(EXPANSION_QUESTS_QUESTS_FOLDER + fileName, this);
	}

	void CopyConfig(ExpansionQuestConfigV20Base questConfigBase)
	{
		ConfigVersion = questConfigBase.ConfigVersion;
		ID = questConfigBase.ID;
		Type = questConfigBase.Type;
		Title = questConfigBase.Title;
		Descriptions = questConfigBase.Descriptions;
		ObjectiveText = questConfigBase.ObjectiveText;
		FollowUpQuest = questConfigBase.FollowUpQuest;
		IsAchievement = questConfigBase.IsAchievement;
		Repeatable = questConfigBase.Repeatable;
		IsDailyQuest = questConfigBase.IsDailyQuest;
		IsWeeklyQuest = questConfigBase.IsWeeklyQuest;
		CancelQuestOnPlayerDeath = questConfigBase.CancelQuestOnPlayerDeath;
		Autocomplete = questConfigBase.Autocomplete;
		IsGroupQuest = questConfigBase.IsGroupQuest;
		ObjectSetFileName = questConfigBase.ObjectSetFileName;
		Objectives = questConfigBase.Objectives;
		QuestItems = questConfigBase.QuestItems;
		Rewards = questConfigBase.Rewards;
		NeedToSelectReward = questConfigBase.NeedToSelectReward;
		RandomReward = questConfigBase.RandomReward;
		RandomRewardAmount = questConfigBase.RandomRewardAmount;
		RewardBehavior = questConfigBase.RewardBehavior;

		RewardsForGroupOwnerOnly = questConfigBase.RewardsForGroupOwnerOnly;

		QuestGiverIDs = questConfigBase.QuestGiverIDs;
		QuestTurnInIDs = questConfigBase.QuestTurnInIDs;
		QuestColor = questConfigBase.QuestColor;
	
		ReputationReward = questConfigBase.ReputationReward;
		ReputationRequirement = questConfigBase.ReputationRequirement;

		PreQuestIDs = questConfigBase.PreQuestIDs;

		RequiredFaction = questConfigBase.RequiredFaction;
		FactionReward = questConfigBase.FactionReward;

		PlayerNeedQuestItems = questConfigBase.PlayerNeedQuestItems;
		DeleteQuestItems = questConfigBase.DeleteQuestItems;
		
		SequentialObjectives = questConfigBase.SequentialObjectives;
	
		FactionReputationRequirements = questConfigBase.FactionReputationRequirements;
		FactionReputationRewards = questConfigBase.FactionReputationRewards;
	}

	void OnSend(ParamsWriteContext ctx)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		int i;

		ctx.Write(ID);
		ctx.Write(Type);
		ctx.Write(Title);

		int descCount = Descriptions.Count();
		ctx.Write(descCount);

		for (i = 0; i < descCount; i++)
		{
			ctx.Write(Descriptions[i]);
		}

		ctx.Write(ObjectiveText);
		ctx.Write(FollowUpQuest);
		ctx.Write(IsAchievement);
		ctx.Write(Repeatable);
		ctx.Write(IsDailyQuest);
		ctx.Write(IsWeeklyQuest);
		ctx.Write(IsGroupQuest);

		//! Objectives
		ctx.Write(Objectives.Count());

		ExpansionQuestModule questModule = ExpansionQuestModule.Cast(CF_ModuleCoreManager.Get(ExpansionQuestModule));
		if (!questModule)
			return;

		for (i = 0; i < Objectives.Count(); i++)
		{
			int objectiveType = Objectives[i].GetObjectiveType();
			int objectiveID = Objectives[i].GetID();
			ctx.Write(objectiveType);

			switch (objectiveType)
			{
				case ExpansionQuestObjectiveType.DELIVERY:
				{
					ExpansionQuestObjectiveDeliveryConfig deliveryConfig = questModule.GetDeliveryObjectiveConfigByID(objectiveID);

					if (!deliveryConfig)
					{
						QuestPrint("DELIVER - Unable to find quest by ID: " + objectiveID);
						QuestPrint("DELIVER - FAILED");
						break;
					}

					deliveryConfig.OnSend(ctx);
					QuestPrint("DELIVER - SUCCESS");
					break;
				}
				case ExpansionQuestObjectiveType.TRAVEL:
				{
					ExpansionQuestObjectiveTravelConfig travelConfig = questModule.GetTravelObjectiveConfigByID(objectiveID);

					if (!travelConfig)
					{
						QuestPrint("TRAVEL - Unable to find quest by ID: " + objectiveID);
						QuestPrint("TRAVEL - FAILED");
						break;
					}

					travelConfig.OnSend(ctx);
					QuestPrint("TRAVEL - SUCCESS");
					break;
				}
				case ExpansionQuestObjectiveType.TARGET:
				{
					ExpansionQuestObjectiveTargetConfig targetConfig = questModule.GetTargetObjectiveConfigByID(objectiveID);

					if (!targetConfig)
					{
						QuestPrint("TARGET - Unable to find quest by ID: " + objectiveID);
						QuestPrint("TARGET - FAILED");
						break;
					}

					targetConfig.OnSend(ctx);
					QuestPrint("TARGET - SUCCESS");
					break;
				}
				case ExpansionQuestObjectiveType.COLLECT:
				{
					ExpansionQuestObjectiveCollectionConfig collectionConfig = questModule.GetCollectionObjectiveConfigByID(objectiveID);

					if (!collectionConfig)
					{
						QuestPrint("COLLECT - Unable to find quest by ID: " + objectiveID);
						QuestPrint("COLLECT - FAILED");
						break;
					}

					collectionConfig.OnSend(ctx);
					QuestPrint("COLLECT - SUCCESS");
					break;
				}
				case ExpansionQuestObjectiveType.TREASUREHUNT:
				{
					ExpansionQuestObjectiveTreasureHuntConfig treasureHuntConfig = questModule.GetTreasureHuntObjectiveConfigByID(objectiveID);

					if (!treasureHuntConfig)
					{
						QuestPrint("TREASUREHUNT - Unable to find quest by ID: " + objectiveID);
						QuestPrint("TREASUREHUNT - FAILED");
						break;
					}

					treasureHuntConfig.OnSend(ctx);
					QuestPrint("TREASUREHUNT - SUCCESS");
					break;
				}
				case ExpansionQuestObjectiveType.ACTION:
				{
					ExpansionQuestObjectiveActionConfig actionConfig = questModule.GetActionObjectiveConfigByID(objectiveID);

					if (!actionConfig)
					{
						QuestPrint("ACTION - Unable to find quest by ID: " + objectiveID);
						QuestPrint("ACTION - FAILED");
						break;
					}

					actionConfig.OnSend(ctx);
					QuestPrint("ACTION - SUCCESS");
					break;
				}
				case ExpansionQuestObjectiveType.CRAFTING:
				{
					ExpansionQuestObjectiveCraftingConfig craftingConfig = questModule.GetCraftingObjectiveConfigByID(objectiveID);

					if (!craftingConfig)
					{
						QuestPrint("CRAFTING - Unable to find quest by ID: " + objectiveID);
						QuestPrint("CRAFTING - FAILED");
						break;
					}

					craftingConfig.OnSend(ctx);
					QuestPrint("CRAFTING - SUCCESS");
					break;
				}
			#ifdef EXPANSIONMODAI
				case ExpansionQuestObjectiveType.AIPATROL:
				{
					ExpansionQuestObjectiveAIPatrolConfig aiPatrolConfig = questModule.GetAIPatrolObjectiveConfigByID(objectiveID);

					if (!aiPatrolConfig)
					{
						QuestPrint("AIPATROL - Unable to find quest by ID: " + objectiveID);
						QuestPrint("AIPATROL - FAILED");
						break;
					}

					aiPatrolConfig.OnSend(ctx);
					QuestPrint("AIPATROL - SUCCESS");
					break;
				}
				case ExpansionQuestObjectiveType.AICAMP:
				{
					ExpansionQuestObjectiveAICampConfig aiCampConfig = questModule.GetAICampObjectiveConfigByID(objectiveID);

					if (!aiCampConfig)
					{
						QuestPrint("AICAMP - Unable to find quest by ID: " + objectiveID);
						QuestPrint("AICAMP - FAILED");
						break;
					}

					aiCampConfig.OnSend(ctx);
					QuestPrint("AICAMP - SUCCESS");
					break;
				}
				case ExpansionQuestObjectiveType.AIESCORT:
				{
					ExpansionQuestObjectiveAIEscortConfig aiEscortConfig = questModule.GetAIEscortObjectiveConfigByID(objectiveID);

					if (!aiEscortConfig)
					{
						QuestPrint("AIESCORT - Unable to find quest by ID: " + objectiveID);
						QuestPrint("AIESCORT - FAILED");
						break;
					}

					aiEscortConfig.OnSend(ctx);
					QuestPrint("AIVIP - SUCCESS");
					break;
				}
			#endif
			}
		}

		//! Quest items
		int itemsCount = QuestItems.Count();
		ctx.Write(itemsCount);
		for (i = 0; i < itemsCount; i++)
		{
			QuestItems[i].OnSend(ctx);
		}

		//! Reward items
		int rewardsCount = Rewards.Count();
		ctx.Write(rewardsCount);
		for (i = 0; i < rewardsCount; i++)
		{
			Rewards[i].OnSend(ctx);
		}

		ctx.Write(NeedToSelectReward);
		ctx.Write(RandomReward);
		ctx.Write(RandomRewardAmount);
		ctx.Write(RewardBehavior);

		ctx.Write(QuestGiverIDs.Count());

		for (i = 0; i < QuestGiverIDs.Count(); i++)
		{
			ctx.Write(QuestGiverIDs[i]);
		}

		ctx.Write(QuestTurnInIDs.Count());

		for (i = 0; i < QuestTurnInIDs.Count(); i++)
		{
			ctx.Write(QuestTurnInIDs[i]);
		}

		ctx.Write(QuestColor);
	#ifdef EXPANSIONMODHARDLINE
		ctx.Write(ReputationReward);
		ctx.Write(ReputationRequirement);
	#endif

		int preQuestCount = PreQuestIDs.Count();
		ctx.Write(preQuestCount);

		for (i = 0; i < preQuestCount; i++)
		{
			ctx.Write(PreQuestIDs[i]);
		}

	#ifdef EXPANSIONMODAI
		ctx.Write(RequiredFaction);
		ctx.Write(FactionReward);
		
	#ifdef EXPANSIONMODHARDLINE
		//! Faction reputation requirements
		string factionType;
		int rep;
		int repReqCount = FactionReputationRequirements.Count();
		ctx.Write(repReqCount);
		for (i = 0; i < repReqCount; i++)
		{
			factionType = FactionReputationRequirements.GetKey(i);
			rep = FactionReputationRequirements[factionType];
			ctx.Write(factionType);
			ctx.Write(rep);
		}
		
		//! Faction reputation rewards
		int repRewCount = FactionReputationRewards.Count();
		ctx.Write(repRewCount);
		for (i = 0; i < repRewCount; i++)
		{
			factionType = FactionReputationRewards.GetKey(i);
			rep = FactionReputationRewards[factionType];
			ctx.Write(factionType);
			ctx.Write(rep);
		}
	#endif
	#endif
	}

	bool OnRecieve(ParamsReadContext ctx)
	{
		int i;

		if (!ctx.Read(ID))
		{
			Error(ToString() + "::OnRecieve - ID");
			return false;
		}

		if (!ctx.Read(Type))
		{
			Error(ToString() + "::OnRecieve - Type");
			return false;
		}

		if (!ctx.Read(Title))
		{
			Error(ToString() + "::OnRecieve - Title");
			return false;
		}

		int descCount;
		if (!ctx.Read(descCount))
		{
			Error(ToString() + "::OnRecieve - descCount");
			return false;
		}

		for (i = 0; i < descCount; i++)
		{
			string desc;
			if (!ctx.Read(desc))
			{
				Error(ToString() + "::OnRecieve - desc[" + i + "]");
				return false;
			}

			Descriptions.Insert(desc);
		}

		if (!ctx.Read(ObjectiveText))
		{
			Error(ToString() + "::OnRecieve - ObjectiveText");
			return false;
		}

		if (!ctx.Read(FollowUpQuest))
		{
			Error(ToString() + "::OnRecieve - FollowUpQuest");
			return false;
		}

		if (!ctx.Read(IsAchievement))
		{
			Error(ToString() + "::OnRecieve - IsAchievement");
			return false;
		}

		if (!ctx.Read(Repeatable))
		{
			Error(ToString() + "::OnRecieve - Repeatable");
			return false;
		}

		if (!ctx.Read(IsDailyQuest))
		{
			Error(ToString() + "::OnRecieve - IsDailyQuest");
			return false;
		}

		if (!ctx.Read(IsWeeklyQuest))
		{
			Error(ToString() + "::OnRecieve - IsWeeklyQuest");
			return false;
		}

		if (!ctx.Read(IsGroupQuest))
		{
			Error(ToString() + "::OnRecieve - IsGroupQuest");
			return false;
		}

		int objectivesCount;
		if (!ctx.Read(objectivesCount))
		{
			Error(ToString() + "::OnRecieve - objectivesCount");
			return false;
		}

		//! Objectives
		if (!Objectives)
			Objectives = new array<ref ExpansionQuestObjectiveConfigBase>;

		for (i = 0; i < objectivesCount; i++)
		{
			int objectiveType;
			if (!ctx.Read(objectiveType))
			{
				Error(ToString() + "::OnRecieve - objectiveType");
				return false;
			}

			if (objectiveType == ExpansionQuestObjectiveType.DELIVERY)
			{
				ExpansionQuestObjectiveDeliveryConfig deliveryConfig = new ExpansionQuestObjectiveDeliveryConfig();
				if (!deliveryConfig.OnRecieve(ctx))
				{
					Error("ExpansionQuestConfig::OnRecieve - DELIVER");
					return false;
				}

				Objectives.Insert(deliveryConfig);
			}
			else if (objectiveType == ExpansionQuestObjectiveType.TRAVEL)
			{
				ExpansionQuestObjectiveTravelConfig travelConfig = new ExpansionQuestObjectiveTravelConfig();
				if (!travelConfig.OnRecieve(ctx))
				{
					Error("ExpansionQuestConfig::OnRecieve - TRAVEL");
					return false;
				}

				Objectives.Insert(travelConfig);
			}
			else if (objectiveType == ExpansionQuestObjectiveType.TARGET)
			{
				ExpansionQuestObjectiveTargetConfig targetConfig = new ExpansionQuestObjectiveTargetConfig();
				if (!targetConfig.OnRecieve(ctx))
				{
					Error("ExpansionQuestConfig::OnRecieve - TARGET");
					return false;
				}

				Objectives.Insert(targetConfig);
			}
			else if (objectiveType == ExpansionQuestObjectiveType.COLLECT)
			{
				ExpansionQuestObjectiveCollectionConfig collectionConfig = new ExpansionQuestObjectiveCollectionConfig();
				if (!collectionConfig.OnRecieve(ctx))
				{
					Error("ExpansionQuestConfig::OnRecieve - COLLECT");
					return false;
				}

				Objectives.Insert(collectionConfig);
			}
			else if (objectiveType == ExpansionQuestObjectiveType.TREASUREHUNT)
			{
				ExpansionQuestObjectiveTreasureHuntConfig treasureHuntConfig = new ExpansionQuestObjectiveTreasureHuntConfig();
				if (!treasureHuntConfig.OnRecieve(ctx))
				{
					Error("ExpansionQuestConfig::OnRecieve - TREASUREHUNT");
					return false;
				}

				Objectives.Insert(treasureHuntConfig);
			}
			else if (objectiveType == ExpansionQuestObjectiveType.ACTION)
			{
				ExpansionQuestObjectiveActionConfig actionConfig = new ExpansionQuestObjectiveActionConfig();
				if (!actionConfig.OnRecieve(ctx))
				{
					Error("ExpansionQuestConfig::OnRecieve - ACTION");
					return false;
				}

				Objectives.Insert(actionConfig);
			}
			else if (objectiveType == ExpansionQuestObjectiveType.CRAFTING)
			{
				ExpansionQuestObjectiveCraftingConfig craftingConfig = new ExpansionQuestObjectiveCraftingConfig();
				if (!craftingConfig.OnRecieve(ctx))
				{
					Error("ExpansionQuestConfig::OnRecieve - CRAFTING");
					return false;
				}

				Objectives.Insert(craftingConfig);
			}
		#ifdef EXPANSIONMODAI
			else if (objectiveType == ExpansionQuestObjectiveType.AIPATROL)
			{
				ExpansionQuestObjectiveAIPatrolConfig aiPatrolConfig = new ExpansionQuestObjectiveAIPatrolConfig();
				if (!aiPatrolConfig.OnRecieve(ctx))
				{
					Error("ExpansionQuestConfig::OnRecieve - AIPATROL");
					return false;
				}

				Objectives.Insert(aiPatrolConfig);
			}
			else if (objectiveType == ExpansionQuestObjectiveType.AICAMP)
			{
				ExpansionQuestObjectiveAICampConfig aiCampConfig = new ExpansionQuestObjectiveAICampConfig();
				if (!aiCampConfig.OnRecieve(ctx))
				{
					Error("ExpansionQuestConfig::OnRecieve - AICAMP");
					return false;
				}

				Objectives.Insert(aiCampConfig);
			}
			else if (objectiveType == ExpansionQuestObjectiveType.AIESCORT)
			{
				ExpansionQuestObjectiveAIEscortConfig aiEscortConfig = new ExpansionQuestObjectiveAIEscortConfig();
				if (!aiEscortConfig.OnRecieve(ctx))
				{
					Error("ExpansionQuestConfig::OnRecieve - AIESCORT");
					return false;
				}

				Objectives.Insert(aiEscortConfig);
			}
		#endif
		}

		//! Quest items
		int itemsCount;
		if (!ctx.Read(itemsCount))
		{
			Error("ExpansionQuestConfig::OnRecieve - itemsCount");
			return false;
		}

		if (!QuestItems)
			QuestItems = new array<ref ExpansionQuestItemConfig>;

		QuestItems.Clear();
		for (i = 0; i < itemsCount; i++)
		{
			ExpansionQuestItemConfig item = new ExpansionQuestItemConfig();
			if (!item.OnRecieve(ctx))
			{
				Error("ExpansionQuestConfig::OnRecieve - item");
				return false;
			}

			QuestItems.Insert(item);
		}

		//! Quest rewards
		int rewardsCount;
		if (!ctx.Read(rewardsCount))
		{
			Error("ExpansionQuestConfig::OnRecieve - rewardsCount");
			return false;
		}

		if (!Rewards)
			Rewards = new array<ref ExpansionQuestRewardConfig>;

		for (i = 0; i < rewardsCount; i++)
		{
			ExpansionQuestRewardConfig reward = new ExpansionQuestRewardConfig();
			if (!reward.OnRecieve(ctx))
			{
				Error("ExpansionQuestConfig::OnRecieve - reward");
				return false;
			}

			Rewards.Insert(reward);
		}

		if (!ctx.Read(NeedToSelectReward))
		{
			Error(ToString() + "::OnRecieve - NeedToSelectReward");
			return false;
		}
		
		if (!ctx.Read(RandomReward))
		{
			Error(ToString() + "::OnRecieve - RandomReward");
			return false;
		}
		
		if (!ctx.Read(RandomRewardAmount))
		{
			Error(ToString() + "::OnRecieve - RandomRewardAmount");
			return false;
		}
		
		if (!ctx.Read(RewardBehavior))
		{
			Error(ToString() + "::OnRecieve - RewardBehavior");
			return false;
		}

		int giverIDsCount;
		if (!ctx.Read(giverIDsCount))
		{
			Error(ToString() + "::OnRecieve - giverIDsCount");
			return false;
		}

		for (i = 0; i < giverIDsCount; i++)
		{
			int giverID;
			if (!ctx.Read(giverID))
			{
				Error(ToString() + "::OnRecieve - giverID[" + i + "]");
				return false;
			}

			QuestGiverIDs.Insert(giverID);
		}

		int turnInIDsCount;
		if (!ctx.Read(turnInIDsCount))
		{
			Error(ToString() + "::OnRecieve - turnInIDsCount");
			return false;
		}

		for (i = 0; i < turnInIDsCount; i++)
		{
			int turnInID;
			if (!ctx.Read(turnInID))
			{
				Error(ToString() + "::OnRecieve - turnInID[" + i + "]");
				return false;
			}

			QuestTurnInIDs.Insert(turnInID);
		}

		if (!ctx.Read(QuestColor))
		{
			Error(ToString() + "::OnRecieve - QuestColor");
			return false;
		}

	#ifdef EXPANSIONMODHARDLINE
		if (!ctx.Read(ReputationReward))
		{
			Error(ToString() + "::OnRecieve - ReputationReward");
			return false;
		}

		if (!ctx.Read(ReputationRequirement))
		{
			Error(ToString() + "::OnRecieve - ReputationRequirement");
			return false;
		}
	#endif

		int preQuestCount;
		if (!ctx.Read(preQuestCount))
		{
			Error(ToString() + "::OnRecieve - preQuestCount");
			return false;
		}

		for (i = 0; i < preQuestCount; i++)
		{
			int preQuest;
			if (!ctx.Read(preQuest))
			{
				Error(ToString() + "::OnRecieve - preQuest[" + i + "]");
				return false;
			}

			PreQuestIDs.Insert(preQuest);
		}

	#ifdef EXPANSIONMODAI
		if (!ctx.Read(RequiredFaction))
		{
			Error(ToString() + "::OnRecieve - RequiredFaction");
			return false;
		}

		if (!ctx.Read(FactionReward))
		{
			Error(ToString() + "::OnRecieve - FactionReward");
			return false;
		}
		
	#ifdef EXPANSIONMODHARDLINE
		string factionName;
		int reputation;
		
		int repReqCount;
		if (!ctx.Read(repReqCount))
		{
			Error(ToString() + "::OnRecieve - repReqCount");
			return false;
		}
		
		if (repReqCount > 0 && !FactionReputationRequirements)
			FactionReputationRequirements = new map<string, int>;

		for (i = 0; i < repReqCount; i++)
		{
			if (!ctx.Read(factionName))
			{
				Error(ToString() + "::OnRecieve - factionName[" + i + "]");
				return false;
			}
			
			if (!ctx.Read(reputation))
			{
				Error(ToString() + "::OnRecieve - reputation[" + i + "]");
				return false;
			}

			FactionReputationRequirements.Insert(factionName, reputation);
		}
				
		int repRewCount;
		if (!ctx.Read(repRewCount))
		{
			Error(ToString() + "::OnRecieve - repRewCount");
			return false;
		}
		
		if (repRewCount > 0 && !FactionReputationRewards)
			FactionReputationRewards = new map<string, int>;
		
		for (i = 0; i < repRewCount; i++)
		{
			if (!ctx.Read(factionName))
			{
				Error(ToString() + "::OnRecieve - factionName[" + i + "]");
				return false;
			}
			
			if (!ctx.Read(reputation))
			{
				Error(ToString() + "::OnRecieve - reputation[" + i + "]");
				return false;
			}

			FactionReputationRewards.Insert(factionName, reputation);
		}
	#endif
	#endif

		return true;
	}

	void QuestPrint(string text)
	{
	#ifdef EXPANSIONMODQUESTSINSTANCEDEBUG
		EXTrace.Print(EXTrace.QUESTS, this, text);
	#endif
	}

	void QuestDebug()
	{
	#ifdef EXPANSIONMODQUESTSINSTANCEDEBUG
		Print("------------------------------------------------------------");
		Print(ToString() + "::QuestDebug - ID: " + ID);
		Print(ToString() + "::QuestDebug - Type: " + Type);
		Print(ToString() + "::QuestDebug - Title: " + Title);
		Print(ToString() + "::QuestDebug - Description 1: " + Descriptions[0]);
		Print(ToString() + "::QuestDebug - Description 2: " + Descriptions[1]);
		Print(ToString() + "::QuestDebug - Description 3: " + Descriptions[2]);
		Print(ToString() + "::QuestDebug - ObjectiveText: " + ObjectiveText);
		Print(ToString() + "::QuestDebug - FollowUpQuest: " + FollowUpQuest);
		Print(ToString() + "::QuestDebug - Repeatable: " + Repeatable);

		int i;
		for (i = 0; i < Objectives.Count(); ++i)
		{
			ExpansionQuestObjectiveConfigBase objectiveConfigBase = Objectives[i];
			ExpansionQuestObjectiveConfig objectiveConfig;
			if (Class.CastTo(objectiveConfig, objectiveConfigBase))
				objectiveConfig.QuestDebug();
		}

		for (i = 0; i < QuestItems.Count(); ++i)
		{
			ExpansionQuestItemConfig itemConfig = QuestItems[i];
			if (itemConfig)
				itemConfig.QuestDebug();
		}

		for (i = 0; i < Rewards.Count(); ++i)
		{
			ExpansionQuestRewardConfig rewardConfig = Rewards[i];
			if (rewardConfig)
				rewardConfig.QuestDebug();
		}

		Print(ToString() + "::QuestDebug - IsGroupQuest: " + IsGroupQuest);
		Print(ToString() + "::QuestDebug - IsDailyQuest: " + IsDailyQuest);
		Print(ToString() + "::QuestDebug - IsWeeklyQuest: " + IsWeeklyQuest);
		Print(ToString() + "::QuestDebug - CancelQuestOnPlayerDeath: " + CancelQuestOnPlayerDeath);
		Print(ToString() + "::QuestDebug - Autocomplete: " + Autocomplete);
		Print(ToString() + "::QuestDebug - IsAchievement: " + IsAchievement);
		Print(ToString() + "::QuestDebug - ObjectSetFileName: " + ObjectSetFileName);
		Print(ToString() + "::QuestDebug - NeedToSelectReward: " + NeedToSelectReward);
		Print(ToString() + "::QuestDebug - RewardsForGroupOwnerOnly: " + RewardsForGroupOwnerOnly);
		Print(ToString() + "::QuestDebug - QuestColor: " + QuestColor);
	#ifdef EXPANSIONMODHARDLINE
		Print(ToString() + "::QuestDebug - ReputationReward: " + ReputationReward);
		Print(ToString() + "::QuestDebug - ReputationRequirement: " + ReputationRequirement);
	
	#ifdef EXPANSIONMODAI
		for (i = 0; i < FactionReputationRewards.Count(); ++i)
		{
			string factionName = FactionReputationRewards.GetKey(i);
			int rep = FactionReputationRewards[factionName];
			Print(ToString() + "::QuestDebug - Faction reward " + i + " - " + factionName + " | " + rep);
		}
	#endif
	#endif
		Print("------------------------------------------------------------");
	#endif
	}
};
