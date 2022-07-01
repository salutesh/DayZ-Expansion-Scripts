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

class ExpansionQuestConfigBase
{
	int ConfigVersion;
	//! Main parameters
	int ID = -1; //! Unique quest ID.
	int Type = -1;	//! Quest type from ExpansionQuestType.
	string Title;	//! Quest title.
	ref array<string> Descriptions; //! 0 - Description on getting quest | 1 - Description while quest is active | 2 - Description when take in quest.
	string ObjectiveText; //! Short objective desctiption.
	int PreQuest = -1; //! Pre-Quest Quest ID.
	int FollowUpQuest = -1; //! Follow-up Quest ID.
	int QuestGiverID = -1; //! Unique quest NPC ID of the NPC that will head out the quest.
	int QuestTurnInID = -1;	//! Unique quest NPC ID of the NPC to turn in the quest when completed.

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
	
	void ExpansionQuestConfigBase()
	{
		Descriptions = new array<string>;
		Objectives = new array<ref ExpansionQuestObjectiveConfig>;
		QuestItems = new array<ref ExpansionQuestItemConfig>;
		Rewards = new array<ref ExpansionQuestRewardConfig>;
	}

	void ~ExpansionQuestConfigBase()
	{
		delete Descriptions;
		delete Objectives;
		delete QuestItems;
		delete Rewards;
	}
};

class ExpansionQuestConfig: ExpansionQuestConfigBase
{
	static const int CONFIGVERSION = 3;
	
	bool RewardsForGroupOwnerOnly = true; //! If the quest is a group quest this option controlls if all group players get the reward or ownly the group owner.
	int HumanityReward = 0; //! Humanity reward when completing the quest.
	
	void ExpansionQuestConfig()
	{
		ConfigVersion = 3;
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

	void SetPreQuestID(int id)
	{
		PreQuest = id;
	}

	int GetPreQuestID()
	{
		return PreQuest;
	}

	void SetFollowUpQuestID(int id)
	{
		FollowUpQuest = id;
	}

	int GetFollowUpQuestID()
	{
		return FollowUpQuest;
	}

	void SetQuestGiverID(int questGiverID)
	{
		QuestGiverID = questGiverID;
	}

	int GetQuestGiverID()
	{
		return QuestGiverID;
	}

	void SetQuestTurnInID(int questTurnInID)
	{
		QuestTurnInID = questTurnInID;
	}

	int GetQuestTurnInID()
	{
		return QuestTurnInID;
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

	void SetIsBanditQuest(bool state)
	{
		IsBanditQuest = state;
	}

	bool IsBanditQuest()
	{
		return IsBanditQuest;
	}

	void SetIsHeroQuest(bool state)
	{
		IsHeroQuest = state;
	}

	bool IsHeroQuest()
	{
		return IsHeroQuest;
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
	
	void SetHumanityReward(int humanity)
	{
		HumanityReward = humanity;
	}
	
	int GetHumanityReward()
	{
		return HumanityReward;
	}

	static ExpansionQuestConfig Load(string fileName)
	{
		bool save;
		CF_Log.Info("[ExpansionQuestConfig] Load existing configuration file:" + fileName);

		ExpansionQuestConfig questConfig;
		ExpansionQuestConfigBase questConfigBase;
		JsonFileLoader<ExpansionQuestConfigBase>.JsonLoadFile(EXPANSION_QUESTS_QUESTS_FOLDER + fileName, questConfigBase);

		if (questConfigBase.ConfigVersion < CONFIGVERSION)
		{
			CF_Log.Info("[ExpansionQuestConfig] Convert existing configuration file:" + fileName + " to version " + CONFIGVERSION);
			questConfig = new ExpansionQuestConfig();

			//! Copy over old configuration that haven't changed
			questConfig.CopyConfig(questConfigBase);

			if (questConfigBase.ConfigVersion < 2)
			{
				for (int i = 0; i < questConfigBase.Objectives.Count(); i++)
				{
					ExpansionQuestObjectiveConfig objective =  questConfigBase.Objectives[i];
					objective.TimeLimit = -1;
					objective.ConfigVersion = ExpansionQuestObjectiveConfig.CONFIGVERSION;
				}
			}

			questConfig.ConfigVersion = CONFIGVERSION;
			save = true;
		}
		else
		{
			JsonFileLoader<ExpansionQuestConfig>.JsonLoadFile(EXPANSION_QUESTS_QUESTS_FOLDER + fileName, questConfig);
		}

		if (save)
		{
			JsonFileLoader<ExpansionQuestConfig>.JsonSaveFile(EXPANSION_QUESTS_QUESTS_FOLDER + fileName, questConfig);
		}

		return questConfig;
	}

	void Save(string fileName)
	{
		JsonFileLoader<ExpansionQuestConfig>.JsonSaveFile(EXPANSION_QUESTS_QUESTS_FOLDER + fileName + ".JSON", this);
	}

	void CopyConfig(ExpansionQuestConfigBase questConfigBase)
	{
		ConfigVersion = questConfigBase.ConfigVersion;
		ID = questConfigBase.ID;
		Type = questConfigBase.Type;
		Title = questConfigBase.Title;
		Descriptions = questConfigBase.Descriptions;
		ObjectiveText = questConfigBase.ObjectiveText;
		PreQuest = questConfigBase.PreQuest;
		FollowUpQuest = questConfigBase.FollowUpQuest;
		QuestGiverID = questConfigBase.QuestGiverID;
		QuestTurnInID = questConfigBase.QuestTurnInID;
		IsAchivement = questConfigBase.IsAchivement;
		Repeatable = questConfigBase.Repeatable;
		IsDailyQuest = questConfigBase.IsDailyQuest;
		IsWeeklyQuest = questConfigBase.IsWeeklyQuest;
		CancelQuestOnPlayerDeath = questConfigBase.CancelQuestOnPlayerDeath;
		Autocomplete = questConfigBase.Autocomplete;
		IsGroupQuest = questConfigBase.IsGroupQuest;
		IsBanditQuest = questConfigBase.IsBanditQuest;
		IsHeroQuest = questConfigBase.IsHeroQuest;
		ObjectSetFileName = questConfigBase.ObjectSetFileName;
		QuestClassName = questConfigBase.QuestClassName;
		Objectives = questConfigBase.Objectives;
		QuestItems = questConfigBase.QuestItems;
		Rewards = questConfigBase.Rewards;
		NeedToSelectReward = questConfigBase.NeedToSelectReward;
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
		ctx.Write(PreQuest);
		ctx.Write(FollowUpQuest);
		ctx.Write(QuestGiverID);
		ctx.Write(QuestTurnInID);
		ctx.Write(IsAchivement);
		ctx.Write(Repeatable);
		ctx.Write(IsDailyQuest);
		ctx.Write(IsWeeklyQuest);
		ctx.Write(CancelQuestOnPlayerDeath);
		ctx.Write(Autocomplete);
		ctx.Write(IsGroupQuest);
		ctx.Write(IsBanditQuest);
		ctx.Write(IsHeroQuest);

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
		ctx.Write(HumanityReward);
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

		if (!ctx.Read(PreQuest))
			return false;

		if (!ctx.Read(FollowUpQuest))
			return false;

		if (!ctx.Read(QuestGiverID))
			return false;

		if (!ctx.Read(QuestTurnInID))
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

		if (!ctx.Read(IsBanditQuest))
			return false;

		if (!ctx.Read(IsHeroQuest))
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
		
		if (!ctx.Read(HumanityReward))
			return false;

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
		QuestPrint("------------------------------------------------------------");
		QuestPrint(ToString() + "::QuestDebug - ID: " + ID);
		QuestPrint(ToString() + "::QuestDebug - Type: " + Type);
		QuestPrint(ToString() + "::QuestDebug - Title: " + Title);
		QuestPrint(ToString() + "::QuestDebug - Description 1: " + Descriptions[0]);
		QuestPrint(ToString() + "::QuestDebug - Description 2: " + Descriptions[1]);
		QuestPrint(ToString() + "::QuestDebug - Description 3: " + Descriptions[2]);
		QuestPrint(ToString() + "::QuestDebug - ObjectiveText: " + ObjectiveText);
		QuestPrint(ToString() + "::QuestDebug - PreQuest: " + PreQuest);
		QuestPrint(ToString() + "::QuestDebug - FollowUpQuest: " + FollowUpQuest);
		QuestPrint(ToString() + "::QuestDebug - QuestGiverID: " + QuestGiverID);
		QuestPrint(ToString() + "::QuestDebug - QuestTurnInID: " + QuestTurnInID);
		QuestPrint(ToString() + "::QuestDebug - Repeatable: " + Repeatable);

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

		QuestPrint(ToString() + "::QuestDebug - IsGroupQuest: " + IsGroupQuest);
		QuestPrint(ToString() + "::QuestDebug - IsDailyQuest: " + IsDailyQuest);
		QuestPrint(ToString() + "::QuestDebug - IsWeeklyQuest: " + IsWeeklyQuest);
		QuestPrint(ToString() + "::QuestDebug - CancelQuestOnPlayerDeath: " + CancelQuestOnPlayerDeath);
		QuestPrint(ToString() + "::QuestDebug - Autocomplete: " + Autocomplete);
		QuestPrint(ToString() + "::QuestDebug - IsAchivement: " + IsAchivement);
		QuestPrint(ToString() + "::QuestDebug - ObjectSetFileName: " + ObjectSetFileName);
		QuestPrint(ToString() + "::QuestDebug - IsBanditQuest: " + IsBanditQuest);
		QuestPrint(ToString() + "::QuestDebug - IsHeroQuest: " + IsHeroQuest);
		QuestPrint(ToString() + "::QuestDebug - NeedToSelectReward: " + NeedToSelectReward);
		QuestPrint(ToString() + "::QuestDebug - RewardsForGroupOwnerOnly: " + RewardsForGroupOwnerOnly);
		QuestPrint(ToString() + "::QuestDebug - HumanityReward: " + HumanityReward);
		QuestPrint("------------------------------------------------------------");
	#endif
	}
};