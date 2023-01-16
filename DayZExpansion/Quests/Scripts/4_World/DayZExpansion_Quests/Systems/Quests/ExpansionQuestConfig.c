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

class ExpansionQuestConfigV5
{
	int ConfigVersion;
	//! Main parameters
	int ID = -1; //! Unique quest ID.
	int Type = ExpansionQuestType.NORMAL; //! Quest type from ExpansionQuestType.
	string Title;	//! Quest title.
	ref array<string> Descriptions; //! 0 - Description on getting quest | 1 - Description while quest is active | 2 - Description when take in quest.
	string ObjectiveText; //! Short objective desctiption.
	int PreQuest = -1; //! Pre-Quest Quest ID.
	int FollowUpQuest = -1; //! Follow-up Quest ID.

	//! Additional quest logic controll parameters
	bool IsAchivement = false;
	bool Repeatable = false; //! Quest is a repeatable quest. Currently every player has a individual cooldown that resets at the exact same time when the timestamp is created. Might want to change that to a fixed reset time for every player.
	bool IsDailyQuest = false; //! Quest is daylie quest and has a daylie reset. Currently every player has a individual cooldown that resets at the exact same time when the timestamp is created. Might want to change that to a fixed reset time for every player.
	bool IsWeeklyQuest = false; //! Quest is a weekly quest and has a weekly cooldown.
	bool CancelQuestOnPlayerDeath = false; //! Quest will be cancled if the quest player (or one of his group members when its a group quest) dies.
	bool Autocomplete = false; //! Quest will be autocompleted when all quest ojectives are completed.
	bool IsGroupQuest = false; //! Quest is a group quest.
	bool IsBanditQuest = false; //! Quest for bandits only
	bool IsHeroQuest = false; //! Quest for heros only

	string ObjectSetFileName = string.Empty; //! File name of the .map file that will get loaded

	//! Crazy ideas
	string QuestClassName = string.Empty; //! Class name of the quest class used to create the quest instance if you want to use a cutomized quest class.

	ref array<ref ExpansionQuestObjectiveConfig> Objectives; //! Quest objectives that the player need to complete to get the quest rewards.
	ref array<ref ExpansionQuestItemConfig> QuestItems; //! Quest items that the player will recive when starting the quest.
	ref array<ref ExpansionQuestRewardConfig> Rewards; //! Quest rewards that the player will revice when turning in the quest and all objectives are completed.

	//! Added with version 2
	bool NeedToSelectReward = false; //! If there is more then one item in the Rewards array and this config param is set to true the player needs to select a reward item on quest competion from the given items in the Rewards array.

	bool RewardsForGroupOwnerOnly = true; //! If the quest is a group quest this option controlls if all group players get the reward or ownly the group owner.
	int HumanityReward = 0; //! Reputation reward when completing the quest.

	ref array<int> QuestGiverIDs; //! NPC IDs of the NPCs that will head out the quest.
	ref array<int> QuestTurnInIDs;	//! NPC IDs of the NPCs where players can turn in the quest when completed.

	void ExpansionQuestConfigV5()
	{
		Descriptions = new array<string>;
		Objectives = new array<ref ExpansionQuestObjectiveConfig>;
		QuestItems = new array<ref ExpansionQuestItemConfig>;
		Rewards = new array<ref ExpansionQuestRewardConfig>;
		QuestGiverIDs = new array<int>;
		QuestTurnInIDs = new array<int>;
	}
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
	bool IsAchivement = false;
	bool Repeatable = false; //! Quest is a repeatable quest. Currently every player has a individual cooldown that resets at the exact same time when the timestamp is created. Might want to change that to a fixed reset time for every player.
	bool IsDailyQuest = false; //! Quest is daylie quest and has a daylie reset. Currently every player has a individual cooldown that resets at the exact same time when the timestamp is created. Might want to change that to a fixed reset time for every player.
	bool IsWeeklyQuest = false; //! Quest is a weekly quest and has a weekly cooldown.
	bool CancelQuestOnPlayerDeath = false; //! Quest will be cancled if the quest player (or one of his group members when its a group quest) dies.
	bool Autocomplete = false; //! Quest will be autocompleted when all quest ojectives are completed.
	bool IsGroupQuest = false; //! Quest is a group quest.

	string ObjectSetFileName = string.Empty; //! File name of the .map file that will get loaded

	//! Crazy ideas
	string QuestClassName = string.Empty; //! Class name of the quest class used to create the quest instance if you want to use a cutomized quest class.

	ref array<ref ExpansionQuestObjectiveConfig> Objectives; //! Quest objectives that the player need to complete to get the quest rewards.
	ref array<ref ExpansionQuestItemConfig> QuestItems; //! Quest items that the player will recive when starting the quest.
	ref array<ref ExpansionQuestRewardConfig> Rewards; //! Quest rewards that the player will revice when turning in the quest and all objectives are completed.

	//! Added with version 2
	bool NeedToSelectReward = false; //! If there is more then one item in the Rewards array and this config param is set to true the player needs to select a reward item on quest competion from the given items in the Rewards array.

	bool RewardsForGroupOwnerOnly = true; //! If the quest is a group quest this option controlls if all group players get the reward or ownly the group owner.

	ref array<int> QuestGiverIDs; //! NPC IDs of the NPCs that will head out the quest.
	ref array<int> QuestTurnInIDs;	//! NPC IDs of the NPCs where players can turn in the quest when completed.

	int QuestColor = 0;  //! Main color that will be used in all the quest menus and interfaces for this quest.
	int ReputationReward = 0; //! Reputation reward when completing the quest.
	int ReputationRequirement = -1; //! Reputation needed to see and start the quest.
	ref array<int> PreQuestIDs; //! Pre-Quest Quest IDs of the quest the player need to have completed to accept this quest.
	
	void ExpansionQuestConfigBase()
	{
		Descriptions = new array<string>;
		Objectives = new array<ref ExpansionQuestObjectiveConfig>;
		QuestItems = new array<ref ExpansionQuestItemConfig>;
		Rewards = new array<ref ExpansionQuestRewardConfig>;
		QuestGiverIDs = new array<int>;
		QuestTurnInIDs = new array<int>;
		PreQuestIDs = new array<int>;
	}
};

class ExpansionQuestConfig: ExpansionQuestConfigBase
{
	static const int CONFIGVERSION = 7;

	void ExpansionQuestConfig()
	{
		ConfigVersion = CONFIGVERSION;
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

	void AddObjectiveConfig(ExpansionQuestObjectiveConfig config)
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

			case ExpansionQuestObjectiveType.AIVIP:
			{
				ExpansionQuestObjectiveAIVIPConfig aiVIPConfig;
				if (Class.CastTo(aiVIPConfig, config))
					Objectives.Insert(aiVIPConfig);
			}
			break;
		#endif
		}
	}

	array<ref ExpansionQuestObjectiveConfig> GetObjectives()
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

	void SetIsAchivement(bool achivement)
	{
		IsAchivement = achivement;
	}

	bool IsAchivement()
	{
		return IsAchivement;
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
		return IsGroupQuest;
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

	void SetQuestClassName(string className)
	{
		QuestClassName = className;
	}

	string GetQuestClassName()
	{
		return QuestClassName:
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

	void SetNeedToSelectReward(bool sate)
	{
		NeedToSelectReward = sate;
	}

	bool NeedToSelectReward()
	{
		return NeedToSelectReward;
	}

	void SetRewardForGroupOwnerOnly(bool sate)
	{
		RewardsForGroupOwnerOnly = sate;
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

	static ExpansionQuestConfig Load(string fileName)
	{
		bool save;
		Print("ExpansionQuestConfig::Load - Load existing configuration file:" + fileName);

		ExpansionQuestConfig questConfig;
		ExpansionQuestConfigBase questConfigBase;

		if (!ExpansionJsonFileParser<ExpansionQuestConfigBase>.Load(EXPANSION_QUESTS_QUESTS_FOLDER + fileName, questConfigBase))
			return NULL;

		if (questConfigBase.ConfigVersion < CONFIGVERSION)
		{
			Print("ExpansionQuestConfig::Load - Convert existing configuration file:" + fileName + " to version " + CONFIGVERSION);
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
			
			if (questConfigBase.ConfigVersion < 7)
			{
				for (int i = questConfig.Rewards.Count() - 1; i >= 0; i--)
				{
					ExpansionQuestRewardConfigBase rewardBase = questConfig.Rewards[i];
					ExpansionQuestRewardConfig convertedReward = new ExpansionQuestRewardConfig();
					convertedReward.Copy(rewardBase);
					
					convertedReward.HealthPercent = 100;
					
					questConfig.Rewards.RemoveOrdered(i);
					questConfig.Rewards.InsertAt(convertedReward, i);
				}
			}

			questConfig.ConfigVersion = CONFIGVERSION;
			save = true;
		}
		else
		{
			if (!ExpansionJsonFileParser<ExpansionQuestConfig>.Load(EXPANSION_QUESTS_QUESTS_FOLDER + fileName, questConfig))
				return NULL;
		}

		if (save)
		{
			questConfig.Save(fileName);
		}

		return questConfig;
	}
	
	bool ValidateQuestConfiguration(string fileName)
	{
		bool save;
		//! Validate quest objectives
		if (!GetObjectives() || GetObjectives() && GetObjectives().Count() == 0)
		{
			Error("ExpansionQuestConfig::ValidateQuestConfiguration - Quest configuration with ID:" + GetID() + " has no configured objectives!");
			return false;
		}

		bool objectivesValid = true;
		int aiObjectivesCount;
		for (int i = 0; i < GetObjectives().Count(); i++)
		{
			ExpansionQuestObjectiveConfig objectiveConfig = GetObjectives()[i];
			switch (objectiveConfig.GetObjectiveType())
			{
				case ExpansionQuestObjectiveType.DELIVERY:
				{
					ExpansionQuestObjectiveDeliveryConfig deliveryConfig;
					if (Class.CastTo(deliveryConfig, objectiveConfig))
					{
						if (!deliveryConfig.Validate())
						{
							Error("ExpansionQuestConfig::ValidateQuestConfiguration - Validation check for quest objective configuration with ID:" + objectiveConfig.GetID() + " and type " + objectiveConfig.GetObjectiveType() + " failed! Please check the objective configuration!");
							deliveryConfig.QuestDebug();
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
							Error("ExpansionQuestConfig::ValidateQuestConfiguration - Validation check for quest objective configuration with ID:" + objectiveConfig.GetID() + " and type " + objectiveConfig.GetObjectiveType() + " failed! Please check the objective configuration!");
							travelConfig.QuestDebug();
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
							Error("ExpansionQuestConfig::ValidateQuestConfiguration - Validation check for quest objective configuration with ID:" + objectiveConfig.GetID() + " and type " + objectiveConfig.GetObjectiveType() + " failed! Please check the objective configuration!");
							targetConfig.QuestDebug();
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
							Error("ExpansionQuestConfig::ValidateQuestConfiguration - Validation check for quest objective configuration with ID:" + objectiveConfig.GetID() + " and type " + objectiveConfig.GetObjectiveType() + " failed! Please check the objective configuration!");
							collectionConfig.QuestDebug();
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
							Error("ExpansionQuestConfig::ValidateQuestConfiguration - Validation check for quest objective configuration with ID:" + objectiveConfig.GetID() + " and type " + objectiveConfig.GetObjectiveType() + " failed! Please check the objective configuration!");
							treasureHuntConfig.QuestDebug();
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
							Error("ExpansionQuestConfig::ValidateQuestConfiguration - Validation check for quest objective configuration with ID:" + objectiveConfig.GetID() + " and type " + objectiveConfig.GetObjectiveType() + " failed! Please check the objective configuration!");
							actionConfig.QuestDebug();
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
							Error("ExpansionQuestConfig::ValidateQuestConfiguration - Validation check for quest objective configuration with ID:" + objectiveConfig.GetID() + " and type " + objectiveConfig.GetObjectiveType() + " failed! Please check the objective configuration!");
							craftingConfig.QuestDebug();
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
							Error("ExpansionQuestConfig::ValidateQuestConfiguration - Validation check for quest objective configuration with ID:" + objectiveConfig.GetID() + " and type " + objectiveConfig.GetObjectiveType() + " failed! Please check the objective configuration!");
							aiPatrolConfig.QuestDebug();
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
							Error("ExpansionQuestConfig::ValidateQuestConfiguration - Validation check for quest objective configuration with ID: " + objectiveConfig.GetID() + " and type " + objectiveConfig.GetObjectiveType() + " failed! Please check the objective configuration!");
							aiCampConfig.QuestDebug();
							objectivesValid = false;
						}

						aiObjectivesCount++;
					}
				}
				break;

				case ExpansionQuestObjectiveType.AIVIP:
				{
					ExpansionQuestObjectiveAIVIPConfig aiVIPConfig;
					if (Class.CastTo(aiVIPConfig, objectiveConfig))
					{
						if (!aiVIPConfig.Validate())
						{
							Error("ExpansionQuestConfig::ValidateQuestConfiguration - Validation check for quest objective configuration with ID: " + objectiveConfig.GetID() + " and type " + objectiveConfig.GetObjectiveType() + " failed! Please check the objective configuration!");
							aiVIPConfig.QuestDebug();
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
			Print("ExpansionQuestConfig::ValidateQuestConfiguration - Quest configuration with ID:" + GetID() + " is a Daily/Weekly quest but not flaged as repeatable. Repeatable flag gets applied and file saved!");
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

	void CopyConfig(ExpansionQuestConfigBase questConfigBase)
	{
		ConfigVersion = questConfigBase.ConfigVersion;
		ID = questConfigBase.ID;
		Type = questConfigBase.Type;
		Title = questConfigBase.Title;
		Descriptions = questConfigBase.Descriptions;
		ObjectiveText = questConfigBase.ObjectiveText;
		FollowUpQuest = questConfigBase.FollowUpQuest;
		IsAchivement = questConfigBase.IsAchivement;
		Repeatable = questConfigBase.Repeatable;
		IsDailyQuest = questConfigBase.IsDailyQuest;
		IsWeeklyQuest = questConfigBase.IsWeeklyQuest;
		CancelQuestOnPlayerDeath = questConfigBase.CancelQuestOnPlayerDeath;
		Autocomplete = questConfigBase.Autocomplete;
		IsGroupQuest = questConfigBase.IsGroupQuest;
		ObjectSetFileName = questConfigBase.ObjectSetFileName;
		QuestClassName = questConfigBase.QuestClassName;
		Objectives = questConfigBase.Objectives;
		QuestItems = questConfigBase.QuestItems;
		Rewards = questConfigBase.Rewards;
		NeedToSelectReward = questConfigBase.NeedToSelectReward;
		RewardsForGroupOwnerOnly = questConfigBase.RewardsForGroupOwnerOnly;
		QuestGiverIDs = questConfigBase.QuestGiverIDs;
		QuestTurnInIDs = questConfigBase.QuestTurnInIDs;
		QuestColor = questConfigBase.QuestColor;
		ReputationReward = questConfigBase.ReputationReward;
		ReputationRequirement = questConfigBase.ReputationRequirement;
		PreQuestIDs = questConfigBase.PreQuestIDs;
	}

	void OnSend(ParamsWriteContext ctx)
	{
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
		ctx.Write(IsAchivement);
		ctx.Write(Repeatable);
		ctx.Write(IsDailyQuest);
		ctx.Write(IsWeeklyQuest);
		ctx.Write(CancelQuestOnPlayerDeath);
		ctx.Write(Autocomplete);
		ctx.Write(IsGroupQuest);

		//! Objectives
		int objectivesCount = Objectives.Count();
		ctx.Write(objectivesCount);

		ExpansionQuestModule questModule = ExpansionQuestModule.Cast(CF_ModuleCoreManager.Get(ExpansionQuestModule));
		if (!questModule)
			return;

		for (i = 0; i < objectivesCount; i++)
		{
			int objectiveType = Objectives[i].GetObjectiveType();
			int objectiveID = Objectives[i].GetID();
			ctx.Write(objectiveType);

			ExpansionQuestObjectiveConfig configBase = Objectives[i];
			if (objectiveType == ExpansionQuestObjectiveType.DELIVERY)
			{
				QuestPrint("ExpansionQuestConfig::OnSend - DELIVER");
				ExpansionQuestObjectiveDeliveryConfig deliveryConfig = questModule.GetDeliveryObjectiveConfigByID(objectiveID);
				deliveryConfig.OnSend(ctx);
				QuestPrint("ExpansionQuestConfig::OnRecieve - DELIVER - SUCCESS");
			}
			else if (objectiveType == ExpansionQuestObjectiveType.TRAVEL)
			{
				QuestPrint("ExpansionQuestConfig::OnSend - TRAVEL");
				ExpansionQuestObjectiveTravelConfig travelConfig = questModule.GetTravelObjectiveConfigByID(objectiveID);
				travelConfig.OnSend(ctx);
				QuestPrint("ExpansionQuestConfig::OnRecieve - TRAVEL - SUCCESS");
			}
			else if (objectiveType == ExpansionQuestObjectiveType.TARGET)
			{
				QuestPrint("ExpansionQuestConfig::OnSend - TARGET");
				ExpansionQuestObjectiveTargetConfig targetConfig = questModule.GetTargetObjectiveConfigByID(objectiveID);
				targetConfig.OnSend(ctx);
				QuestPrint("ExpansionQuestConfig::OnRecieve - TARGET - SUCCESS");
			}
			else if (objectiveType == ExpansionQuestObjectiveType.COLLECT)
			{
				QuestPrint("ExpansionQuestConfig::OnSend - COLLECT");
				ExpansionQuestObjectiveCollectionConfig collectionConfig = questModule.GetCollectionObjectiveConfigByID(objectiveID);
				collectionConfig.OnSend(ctx);
				QuestPrint("ExpansionQuestConfig::OnRecieve - COLLECT - SUCCESS");
			}
			else if (objectiveType == ExpansionQuestObjectiveType.TREASUREHUNT)
			{
				QuestPrint("ExpansionQuestConfig::OnSend - TREASUREHUNT");
				ExpansionQuestObjectiveTreasureHuntConfig treasureHuntConfig = questModule.GetTreasureHuntObjectiveConfigByID(objectiveID);
				treasureHuntConfig.OnSend(ctx);
				QuestPrint("ExpansionQuestConfig::OnRecieve - TREASUREHUNT - SUCCESS");
			}
			else if (objectiveType == ExpansionQuestObjectiveType.ACTION)
			{
				QuestPrint("ExpansionQuestConfig::OnSend - ACTION");
				ExpansionQuestObjectiveActionConfig actionConfig = questModule.GetActionObjectiveConfigByID(objectiveID);
				actionConfig.OnSend(ctx);
				QuestPrint("ExpansionQuestConfig::OnRecieve - ACTION - SUCCESS");
			}
			else if (objectiveType == ExpansionQuestObjectiveType.CRAFTING)
			{
				QuestPrint("ExpansionQuestConfig::OnSend - CRAFTING");
				ExpansionQuestObjectiveCraftingConfig craftingConfig = questModule.GetCraftingObjectiveConfigByID(objectiveID);
				craftingConfig.OnSend(ctx);
				QuestPrint("ExpansionQuestConfig::OnRecieve - CRAFTING - SUCCESS");
			}
		#ifdef EXPANSIONMODAI
			else if (objectiveType == ExpansionQuestObjectiveType.AIPATROL)
			{
				QuestPrint("ExpansionQuestConfig::OnSend - AIPATROL");
				ExpansionQuestObjectiveAIPatrolConfig aiPatrolConfig = questModule.GetAIPatrolObjectiveConfigByID(objectiveID);
				aiPatrolConfig.OnSend(ctx);
				QuestPrint("ExpansionQuestConfig::OnRecieve - AIPATROL - SUCCESS");
			}
			else if (objectiveType == ExpansionQuestObjectiveType.AICAMP)
			{
				QuestPrint("ExpansionQuestConfig::OnSend - AICAMP");
				ExpansionQuestObjectiveAICampConfig aiCampConfig = questModule.GetAICampObjectiveConfigByID(objectiveID);
				aiCampConfig.OnSend(ctx);
				QuestPrint("ExpansionQuestConfig::OnRecieve - AICAMP - SUCCESS");
			}
			else if (objectiveType == ExpansionQuestObjectiveType.AIVIP)
			{
				QuestPrint("ExpansionQuestConfig::OnSend - AIVIP");
				ExpansionQuestObjectiveAIVIPConfig aiVIPConfig = questModule.GetAIVIPObjectiveConfigByID(objectiveID);
				aiVIPConfig.OnSend(ctx);
				QuestPrint("ExpansionQuestConfig::OnRecieve - AIVIP - SUCCESS");
			}
		#endif
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
		ctx.Write(RewardsForGroupOwnerOnly);

		int giverIDsCount = QuestGiverIDs.Count();
		ctx.Write(giverIDsCount);

		for (i = 0; i < giverIDsCount; i++)
		{
			ctx.Write(QuestGiverIDs[i]);
		}

		int turnInIDsCount = QuestTurnInIDs.Count();
		ctx.Write(turnInIDsCount);

		for (i = 0; i < turnInIDsCount; i++)
		{
			ctx.Write(QuestTurnInIDs[i]);
		}

		ctx.Write(QuestColor);
		ctx.Write(ReputationReward);
		ctx.Write(ReputationRequirement);

		int preQuestCount = PreQuestIDs.Count();
		ctx.Write(preQuestCount);

		for (i = 0; i < preQuestCount; i++)
		{
			ctx.Write(PreQuestIDs[i]);
		}
	}

	bool OnRecieve(ParamsReadContext ctx)
	{
		int i;

		if (!ctx.Read(ID))
			return false;

		if (!ctx.Read(Type))
			return false;

		if (!ctx.Read(Title))
			return false;

		int descCount;
		if (!ctx.Read(descCount))
			return false;

		for (i = 0; i < descCount; i++)
		{
			string desc;
			if (!ctx.Read(desc))
				return false;

			Descriptions.Insert(desc);
		}

		if (!ctx.Read(ObjectiveText))
			return false;

		if (!ctx.Read(FollowUpQuest))
			return false;

		if (!ctx.Read(IsAchivement))
			return false;

		if (!ctx.Read(Repeatable))
			return false;

		if (!ctx.Read(IsDailyQuest))
			return false;

		if (!ctx.Read(IsWeeklyQuest))
			return false;

		if (!ctx.Read(CancelQuestOnPlayerDeath))
			return false;

		if (!ctx.Read(Autocomplete))
			return false;

		if (!ctx.Read(IsGroupQuest))
			return false;

		int objectivesCount;
		if (!ctx.Read(objectivesCount))
			return false;

		//! Objectives
		if (!Objectives)
			Objectives = new array<ref ExpansionQuestObjectiveConfig>;

		for (i = 0; i < objectivesCount; i++)
		{
			int objectiveType;
			if (!ctx.Read(objectiveType))
				return false;

			if (objectiveType == ExpansionQuestObjectiveType.DELIVERY)
			{
				QuestPrint("ExpansionQuestConfig::OnRecieve - DELIVER");
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
				QuestPrint("ExpansionQuestConfig::OnRecieve - TRAVEL");
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
				QuestPrint("ExpansionQuestConfig::OnRecieve - TARGET");
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
				QuestPrint("ExpansionQuestConfig::OnRecieve - COLLECT");
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
				QuestPrint("ExpansionQuestConfig::OnRecieve - TREASUREHUNT");
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
				QuestPrint("ExpansionQuestConfig::OnRecieve - ACTION");
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
				QuestPrint("ExpansionQuestConfig::OnRecieve - CRAFTING");
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
				QuestPrint("ExpansionQuestConfig::OnRecieve - AIPATROL");
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
				QuestPrint("ExpansionQuestConfig::OnRecieve - AICAMP");
				ExpansionQuestObjectiveAICampConfig aiCampConfig = new ExpansionQuestObjectiveAICampConfig();
				if (!aiCampConfig.OnRecieve(ctx))
				{
					Error("ExpansionQuestConfig::OnRecieve - AICAMP");
					return false;
				}

				Objectives.Insert(aiCampConfig);
			}
			else if (objectiveType == ExpansionQuestObjectiveType.AIVIP)
			{
				QuestPrint("ExpansionQuestConfig::OnRecieve - AIVIP");
				ExpansionQuestObjectiveAIVIPConfig aiVIPConfig = new ExpansionQuestObjectiveAIVIPConfig();
				if (!aiVIPConfig.OnRecieve(ctx))
				{
					Error("ExpansionQuestConfig::OnRecieve - AIVIP");
					return false;
				}

				Objectives.Insert(aiVIPConfig);
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

		// Quest rewards
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
			return false;

		if (!ctx.Read(RewardsForGroupOwnerOnly))
			return false;

		int giverIDsCount;
		if (!ctx.Read(giverIDsCount))
			return false;

		for (i = 0; i < giverIDsCount; i++)
		{
			int giverID;
			if (!ctx.Read(giverID))
				return false;

			QuestGiverIDs.Insert(giverID);
		}

		int turnInIDsCount;
		if (!ctx.Read(turnInIDsCount))
			return false;

		for (i = 0; i < turnInIDsCount; i++)
		{
			int turnInID;
			if (!ctx.Read(turnInID))
				return false;

			QuestTurnInIDs.Insert(turnInID);
		}

		if (!ctx.Read(QuestColor))
			return false;

		if (!ctx.Read(ReputationReward))
			return false;

		if (!ctx.Read(ReputationRequirement))
			return false;

		int preQuestCount;
		if (!ctx.Read(preQuestCount))
			return false;

		for (i = 0; i < preQuestCount; i++)
		{
			int preQuest;
			if (!ctx.Read(preQuest))
				return false;

			PreQuestIDs.Insert(preQuest);
		}

		return true;
	}

	void QuestPrint(string text)
	{
	#ifdef EXPANSIONMODQUESTSINSTANCEDEBUG
		Print(text);
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
			ExpansionQuestObjectiveConfig objectiveConfig = Objectives[i];
			objectiveConfig.QuestDebug();
		}

		for (i = 0; i < QuestItems.Count(); ++i)
		{
			ExpansionQuestItemConfig itemConfig = QuestItems[i];
			itemConfig.QuestDebug();
		}

		for (i = 0; i < Rewards.Count(); ++i)
		{
			ExpansionQuestRewardConfig rewardConfig = Rewards[i];
			rewardConfig.QuestDebug();
		}

		Print(ToString() + "::QuestDebug - IsGroupQuest: " + IsGroupQuest);
		Print(ToString() + "::QuestDebug - IsDailyQuest: " + IsDailyQuest);
		Print(ToString() + "::QuestDebug - IsWeeklyQuest: " + IsWeeklyQuest);
		Print(ToString() + "::QuestDebug - CancelQuestOnPlayerDeath: " + CancelQuestOnPlayerDeath);
		Print(ToString() + "::QuestDebug - Autocomplete: " + Autocomplete);
		Print(ToString() + "::QuestDebug - IsAchivement: " + IsAchivement);
		Print(ToString() + "::QuestDebug - ObjectSetFileName: " + ObjectSetFileName);
		Print(ToString() + "::QuestDebug - NeedToSelectReward: " + NeedToSelectReward);
		Print(ToString() + "::QuestDebug - RewardsForGroupOwnerOnly: " + RewardsForGroupOwnerOnly);
		Print(ToString() + "::QuestDebug - QuestColor: " + QuestColor);
		Print(ToString() + "::QuestDebug - ReputationReward: " + ReputationReward);
		Print(ToString() + "::QuestDebug -  ReputationRequirement: " + ReputationRequirement);
		Print("------------------------------------------------------------");
	#endif
	}
};