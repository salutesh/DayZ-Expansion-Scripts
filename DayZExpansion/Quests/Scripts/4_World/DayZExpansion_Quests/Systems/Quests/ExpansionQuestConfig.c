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

class ExpansionQuestConfig
{
	//! Main parameters
	protected int ID = -1; //! Unique quest ID.
	protected int Type = -1;	//! Quest type from ExpansionQuestType.
	protected string Title;	//! Quest title.
	protected autoptr array<string> Descriptions; //! 0 - Description on getting quest | 1 - Description while quest is active | 2 - Description when take in quest.
	protected string ObjectiveText; //! Short objective desctiption.
	protected int PreQuest = -1; //! Pre-Quest Quest ID.
	protected int FollowUpQuest = -1; //! Follow-up Quest ID.
	protected int QuestGiverID = 1; //! Unique quest NPC ID of the NPC that will head out the quest.
	protected int QuestTurnInID = 1;	//! Unique quest NPC ID of the NPC that will turn-in the quest when completed.

	//! Additional quest logic controll parameters
	protected bool IsAchivement = false;
	protected bool Repeatable = false; //! Quest is a repeatable quest. Currently every player has a individual cooldown that resets at the exact same time when the timestamp is created. Might want to change that to a fixed reset time for every player.
	protected bool IsDaylieQuest = false; //! Quest is daylie quest and has a daylie reset. Currently every player has a individual cooldown that resets at the exact same time when the timestamp is created. Might want to change that to a fixed reset time for every player.
	protected bool IsWeeklyQuest = false; //! Quest is a weekly quest and has a weekly cooldown.
	protected bool CancelQuestOnPlayerDead = false; //! Quest will be cancled if the quest player (or one of his group members when its a group quest) dies.
	protected bool Autocomplet = false; //! Quest will be autocompleted when all quest ojectives are completed.
	protected bool IsGroupQuest = false; //! Quest is a group quest.

#ifdef EXPANSIONMODHARDLINE
	protected bool IsBanditQuest = false; //! Quest for bandits only
	protected bool IsHeroQuest = false; //! Quest for heros only
#endif

	//! Group quest parameters
	protected string ObjectSetFileName = string.Empty; //! File name of the .map file that will get loaded
	
	//! Crazy ideas
	protected string QuestClassName = string.Empty; //! Class name of the quest class used to create the quest instance if you want to use a cutomized quest class.
	
	protected autoptr array<ref ExpansionQuestObjectiveConfigBase> Objectives; //! Quest objectives that the player need to complete to get the quest rewards.
	protected autoptr array<ref ExpansionQuestItemConfig> QuestItems; //! Quest items that the player will recive when starting the quest.
	protected autoptr array<ref ExpansionQuestRewardConfig> Rewards; //! Quest rewards that the player will revice when turning in the quest and all objectives are completed.

	void ExpansionQuestConfig()
	{
		Descriptions = new array<string>;
		Objectives = new array<ref ExpansionQuestObjectiveConfigBase>;
		QuestItems = new array<ref ExpansionQuestItemConfig>;
		Rewards = new array<ref ExpansionQuestRewardConfig>;
	}

	void ~ExpansionQuestConfig()
	{
		delete Descriptions;
		delete Objectives;
		delete QuestItems;
		delete Rewards;
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

	void AddReward

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

	void SetIsDaylieQuest(bool daylie)
	{
		IsDaylieQuest = daylie;
	}

	bool IsDaylieQuest()
	{
		return IsDaylieQuest;
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

	void SetAutocomplet(bool state)
	{
		Autocomplet = state;
	}

	bool IsAutocomplet()
	{
		return Autocomplet;
	}

	void SetCancelQuestOnPlayerDead(bool state)
	{
		CancelQuestOnPlayerDead = state;
	}

	bool CancelQuestOnPlayerDead()
	{
		return CancelQuestOnPlayerDead;
	}

#ifdef EXPANSIONMODHARDLINE
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
#endif

	void Save(string fileName)
	{
		JsonFileLoader<ExpansionQuestConfig>.JsonSaveFile(EXPANSION_QUESTS_QUESTS_FOLDER + fileName + ".JSON", this);
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
		ctx.Write(IsDaylieQuest);
		ctx.Write(IsWeeklyQuest);
		ctx.Write(CancelQuestOnPlayerDead);
		ctx.Write(Autocomplet);
		ctx.Write(IsGroupQuest);
	#ifdef EXPANSIONMODHARDLINE
		ctx.Write(IsBanditQuest);
		ctx.Write(IsHeroQuest);
	#endif

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

			ExpansionQuestObjectiveConfigBase configBase = Objectives[i];
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

		if (!ctx.Read(IsDaylieQuest))
			return false;

		if (!ctx.Read(IsWeeklyQuest))
			return false;

		if (!ctx.Read(CancelQuestOnPlayerDead))
			return false;

		if (!ctx.Read(Autocomplet))
			return false;

		if (!ctx.Read(IsGroupQuest))
			return false;

	#ifdef EXPANSIONMODHARDLINE
		if (!ctx.Read(IsBanditQuest))
			return false;

		if (!ctx.Read(IsHeroQuest))
			return false;
	#endif

		int objectivesCount;
		if (!ctx.Read(objectivesCount))
			return false;

		//! Objectives
		if (!Objectives)
			Objectives = new array<ref ExpansionQuestObjectiveConfigBase>;

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
			ExpansionQuestObjectiveConfigBase objectiveConfig = Objectives[i];
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
		QuestPrint(ToString() + "::QuestDebug - IsDaylieQuest: " + IsDaylieQuest);
		QuestPrint(ToString() + "::QuestDebug - IsWeeklyQuest: " + IsWeeklyQuest);
		QuestPrint(ToString() + "::QuestDebug - CancelQuestOnPlayerDead: " + CancelQuestOnPlayerDead);
		QuestPrint(ToString() + "::QuestDebug - Autocomplet: " + Autocomplet);
		//Print(ToString() + "::QuestDebug - IsAchievement: " + IsAchievement);
		QuestPrint(ToString() + "::QuestDebug - ObjectSetFileName: " + ObjectSetFileName);
	#ifdef EXPANSIONMODHARDLINE
		QuestPrint(ToString() + "::QuestDebug - IsBanditQuest: " + IsBanditQuest);
		QuestPrint(ToString() + "::QuestDebug - IsHeroQuest: " + IsHeroQuest);
	#endif
		QuestPrint("------------------------------------------------------------");
	#endif
	}
};