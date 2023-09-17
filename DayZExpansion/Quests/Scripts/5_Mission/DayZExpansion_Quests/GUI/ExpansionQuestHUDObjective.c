/**
 * ExpansionQuestHUDObjective.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONMODQUESTS_HUD_ENABLE
class ExpansionQuestHUDObjective: ExpansionScriptView
{
	protected ref ExpansionQuestHUDObjectiveController m_QuestHUDObjectiveController;
	protected ExpansionQuestObjectiveData m_Objective;
	protected ExpansionQuestConfig m_Quest;
	protected Widget Spacer;
	protected RichTextWidget ObjectiveName;
	protected RichTextWidget ObjectiveTime;
	protected WrapSpacerWidget ObjectiveWrapper;
	protected WrapSpacerWidget ObjectiveDeliveryEnties;

	void ExpansionQuestHUDObjective(ExpansionQuestObjectiveData objective, ExpansionQuestConfig questConfig)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		m_QuestHUDObjectiveController = ExpansionQuestHUDObjectiveController.Cast(GetController());
		m_Objective = objective;
		m_Quest = questConfig;
	}

	override string GetLayoutFile()
	{
		return "DayZExpansion/Quests/GUI/layouts/quests/expansion_quest_hud_objective.layout";
	}

	override typename GetControllerType()
	{
		return ExpansionQuestHUDObjectiveController;
	}

	void SetEntryObjective()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);

		int count;
		int amount;
		int i;

		ExpansionQuestObjectiveConfig objectiveConfig = ExpansionQuestObjectiveConfig.Cast(m_Quest.GetObjectives()[m_Objective.GetObjectiveIndex()]);
		if (!objectiveConfig)
			return;

		bool completed = m_Objective.IsCompleted();
		if (objectiveConfig.GetObjectiveText() != string.Empty)
		{
			ObjectiveName.Show(true);
			string objectiveState;
			if (completed)
			{
				string completeKey = "#STR_EXPANSION_QUEST_HUD_COMPLETE";
				objectiveState = "[" + completeKey + "] ";
			}
			else
			{
				string incompleteKey = "#STR_EXPANSION_QUEST_HUD_INCOMPLETE";
				objectiveState = "[" + incompleteKey + "] ";
			}

			m_QuestHUDObjectiveController.ObjectiveName = objectiveState + objectiveConfig.GetObjectiveText();
			m_QuestHUDObjectiveController.NotifyPropertyChanged("ObjectiveName");
		}

		UpdateTimeLimit();

		switch (objectiveConfig.GetObjectiveType())
		{
			case ExpansionQuestObjectiveType.TARGET:
		#ifdef EXPANSIONMODAI
			case ExpansionQuestObjectiveType.AIPATROL:
			case ExpansionQuestObjectiveType.AICAMP:
		#endif
			{
				UpdateTarget();
			}
			break;

			case  ExpansionQuestObjectiveType.TRAVEL:
			{
				ExpansionQuestObjectiveTravelConfig travelObjective;
				if (Class.CastTo(travelObjective, objectiveConfig))
				{
					m_QuestHUDObjectiveController.ObjectiveTarget = "#STR_EXPANSION_QUEST_HUD_TRAVEL";
					m_QuestHUDObjectiveController.NotifyPropertyChanged("ObjectiveTarget");
					if (travelObjective.ShowDistance())
					{
						UpdateDistance();
					}
					else
					{
						ObjectiveWrapper.Show(false);
					}
				}
			}
			break;

			case ExpansionQuestObjectiveType.DELIVERY:
			{
				ExpansionQuestObjectiveDeliveryConfig deliveryObjective;
				if (Class.CastTo(deliveryObjective, objectiveConfig))
				{
					m_QuestHUDObjectiveController.ObjectiveTarget = "#STR_EXPANSION_QUEST_HUD_DELIVER";
					m_QuestHUDObjectiveController.NotifyPropertyChanged("ObjectiveTarget");
					if (deliveryObjective.ShowDistance())
					{
						UpdateDistance();
					}
					else
					{
						ObjectiveWrapper.Show(false);
					}
 					
					ObjectiveDeliveryEnties.Show(true);
					array<ref ExpansionQuestObjectiveDelivery> deliveries = deliveryObjective.GetCollections();

					for (i = 0; i < deliveries.Count(); i++)
					{
						int currentDeliveryCount = m_Objective.GetDeliveryCountByIndex(i);
						ExpansionQuestObjectiveDelivery delivery = deliveries[i];
						ExpansionQuestHUDDeliveryEntry deliveryEntry = new ExpansionQuestHUDDeliveryEntry(delivery, currentDeliveryCount);
						m_QuestHUDObjectiveController.DeliveryEnties.Insert(deliveryEntry);
						
					}
				}
			}
			break;

			case ExpansionQuestObjectiveType.COLLECT:
			{
				ExpansionQuestObjectiveCollectionConfig collectionObjective;
				ExpansionQuestHUDDeliveryEntry collectionEntry;
				ExpansionQuestObjectiveDelivery collection;
				int currentCollectionCount;

				if (Class.CastTo(collectionObjective, objectiveConfig))
				{
					m_QuestHUDObjectiveController.ObjectiveTarget = "#STR_EXPANSION_QUEST_HUD_COLLECT";
					m_QuestHUDObjectiveController.NotifyPropertyChanged("ObjectiveTarget");
					if (collectionObjective.ShowDistance() && completed)
					{
						UpdateDistance();
					}
					else if (!collectionObjective.ShowDistance() || !completed)
					{
						ObjectiveWrapper.Show(false);
					}

					ObjectiveDeliveryEnties.Show(true);
					array<ref ExpansionQuestObjectiveDelivery> collections = collectionObjective.GetCollections();
					
					map<int, int> collectionsMap = new map<int, int>;
					int collectionsCount;
					
					for (i = 0; i < collections.Count(); i++)
					{
						currentCollectionCount = m_Objective.GetDeliveryCountByIndex(i);				
						collection = collections[i];									
						if (!collectionObjective.NeedAnyCollection())
						{
							collectionEntry = new ExpansionQuestHUDDeliveryEntry(collection, currentCollectionCount);					
							m_QuestHUDObjectiveController.DeliveryEnties.Insert(collectionEntry);
						}
						else if (collectionObjective.NeedAnyCollection() && currentCollectionCount >= collection.GetAmount())
						{
							collectionsCount += currentCollectionCount;
							collectionsMap.Insert(i, currentCollectionCount);	
						}
					}

					if (collectionObjective.NeedAnyCollection())
					{
						if (collectionsCount > 0)
						{
							foreach (int collectionIndex, int collectionCount: collectionsMap)
							{
								collection = collections[collectionIndex];
								if (!collection)
									continue;

								collectionEntry = new ExpansionQuestHUDDeliveryEntry(collection, collectionCount);					
								m_QuestHUDObjectiveController.DeliveryEnties.Insert(collectionEntry);
							}
						}
						else
						{
							for (i = 0; i < collections.Count(); i++)
							{
								currentCollectionCount = m_Objective.GetDeliveryCountByIndex(i);				
								collection = collections[i];

								collectionEntry = new ExpansionQuestHUDDeliveryEntry(collection, currentCollectionCount);					
								m_QuestHUDObjectiveController.DeliveryEnties.Insert(collectionEntry);
							}
						}
					}
				}
			}
			break;

			case ExpansionQuestObjectiveType.TREASUREHUNT:
			{
				ExpansionQuestObjectiveTreasureHuntConfig treasureObjective;
				if (Class.CastTo(treasureObjective, objectiveConfig))
				{
					m_QuestHUDObjectiveController.ObjectiveTarget = "#STR_EXPANSION_QUEST_HUD_TREASURE";
					m_QuestHUDObjectiveController.NotifyPropertyChanged("ObjectiveTarget");
					if (treasureObjective.ShowDistance())
					{
						UpdateDistance();
					}
					else
					{
						ObjectiveWrapper.Show(false);
					}
				}
			}
			break;

		#ifdef EXPANSIONMODAI
			case ExpansionQuestObjectiveType.AIESCORT:
			{
				ExpansionQuestObjectiveAIEscortConfig escortConfig;
				if (Class.CastTo(escortConfig, objectiveConfig))
				{
					m_QuestHUDObjectiveController.ObjectiveTarget = "#STR_EXPANSION_QUEST_HUD_TRAVEL";
					m_QuestHUDObjectiveController.NotifyPropertyChanged("ObjectiveTarget");
					if (escortConfig.ShowDistance())
					{
						UpdateDistance();
					}
					else
					{
						ObjectiveWrapper.Show(false);
					}
				}
			}
			break;
		#endif

			case ExpansionQuestObjectiveType.ACTION:
			{
				ExpansionQuestObjectiveActionConfig actionConfig;
				if (Class.CastTo(actionConfig, objectiveConfig))
					UpdateTarget();
			}
			break;

			case ExpansionQuestObjectiveType.CRAFTING:
			{
				ExpansionQuestObjectiveCraftingConfig craftingConfig;
				if (Class.CastTo(craftingConfig, objectiveConfig))
					UpdateTarget();
			}
			break;
		}
		
		if (!completed)
			Spacer.SetColor(ExpansionQuestModule.GetQuestColor(m_Quest));
		else
			Spacer.SetColor(ARGB(200, 160, 223, 59));
	}
	
	void UpdateTarget()
	{
		m_QuestHUDObjectiveController.ObjectiveTarget = "";
		m_QuestHUDObjectiveController.NotifyPropertyChanged("ObjectiveTarget");

		int count = m_Objective.GetObjectiveCount();
		int amount = m_Objective.GetObjectiveAmount();

		m_QuestHUDObjectiveController.ObjectiveValue = Math.Min(count, amount).ToString() + "/" + amount.ToString();
		m_QuestHUDObjectiveController.NotifyPropertyChanged("ObjectiveValue");
	}

	void UpdateObjectiveData(ExpansionQuestObjectiveData objectiveData)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
		
		m_Objective = objectiveData;
		
		m_QuestHUDObjectiveController.DeliveryEnties.Clear();
		
		SetEntryObjective();
	}
	
	override void Expansion_Update()
	{
		if (!m_Objective || !m_Quest || !IsVisible() || !GetGame().GetPlayer())
			return;
		
		UpdateTimeLimit();
		
		array<ref ExpansionQuestObjectiveConfigBase> questObjectives = m_Quest.GetObjectives();
		if (!questObjectives || questObjectives.Count() == 0)
			return;
		
		int objectiveIndex = -1;
		objectiveIndex = m_Objective.GetObjectiveIndex();
		
		if (objectiveIndex == -1)
			return;

		ExpansionQuestObjectiveConfig objectiveConfig = ExpansionQuestObjectiveConfig.Cast(questObjectives[objectiveIndex]);
		if (!objectiveConfig)
			return;
		
		bool completed = m_Objective.IsCompleted();
		
		//! @note: Update quest objective distance in HUD.
		switch (objectiveConfig.GetObjectiveType())
		{
			case  ExpansionQuestObjectiveType.TRAVEL:
			{
				ExpansionQuestObjectiveTravelConfig travelObjective;
				if (Class.CastTo(travelObjective, objectiveConfig) && travelObjective.ShowDistance())
					UpdateDistance();
			}
			break;

			case ExpansionQuestObjectiveType.DELIVERY:
			{
				ExpansionQuestObjectiveDeliveryConfig deliveryObjective;
				if (Class.CastTo(deliveryObjective, objectiveConfig) && deliveryObjective.ShowDistance())
					UpdateDistance();
			}
			break;

			case ExpansionQuestObjectiveType.COLLECT:
			{
				ExpansionQuestObjectiveCollectionConfig collectionObjective;
				if (Class.CastTo(collectionObjective, objectiveConfig) && collectionObjective.ShowDistance() && completed)
					UpdateDistance();
			}
			break;

			case ExpansionQuestObjectiveType.TREASUREHUNT:
			{
				ExpansionQuestObjectiveTreasureHuntConfig treasureObjective;
				if (Class.CastTo(treasureObjective, objectiveConfig) && treasureObjective.ShowDistance())
					UpdateDistance();
			}
			break;

		#ifdef EXPANSIONMODAI
			case ExpansionQuestObjectiveType.AIESCORT:
			{
				ExpansionQuestObjectiveAIEscortConfig escortConfig;
				if (Class.CastTo(escortConfig, objectiveConfig) && escortConfig.ShowDistance())
					UpdateDistance();
			}
			break;
		#endif
		}
	}
	
	void UpdateTimeLimit()
	{
		if (m_Objective.GetTimeLimit() > -1)
		{
			ObjectiveTime.Show(true);
			m_QuestHUDObjectiveController.ObjectiveTimeLimit = "#STR_EXPANSION_QUEST_HUD_TIME " + ExpansionStatic.FormatTime(m_Objective.GetTimeLimit(), false);
			m_QuestHUDObjectiveController.NotifyPropertyChanged("ObjectiveTimeLimit");

			if (m_Objective.GetTimeLimit() > 60)
			{
				ObjectiveTime.SetColor(COLOR_EXPANSION_NOTIFICATION_INFO);
			}
			else if (m_Objective.GetTimeLimit() <= 60)
			{
				ObjectiveTime.SetColor(COLOR_EXPANSION_NOTIFICATION_ORANGE);
			}
			else if (m_Objective.GetTimeLimit() <= 10)
			{
				ObjectiveTime.SetColor(COLOR_EXPANSION_NOTIFICATION_ERROR);
			}
		}
	}

	void UpdateDistance()
	{
		vector objectivePos = m_Objective.GetObjectivePosition();
		vector playerPos = GetGame().GetPlayer().GetPosition();
		int currentDistance = Math.Round(vector.Distance(playerPos, objectivePos));
		m_QuestHUDObjectiveController.ObjectiveValue = currentDistance.ToString() + " m";
		m_QuestHUDObjectiveController.NotifyPropertyChanged("ObjectiveValue");
	}

	override float GetUpdateTickRate()
	{
		return 1.0;
	}
};

class ExpansionQuestHUDObjectiveController: ExpansionViewController
{
	ref ObservableCollection<ref ExpansionQuestHUDDeliveryEntry> DeliveryEnties = new ObservableCollection<ref ExpansionQuestHUDDeliveryEntry>(this);
	string ObjectiveName;
	string ObjectiveTarget;
	string ObjectiveValue;
	string ObjectiveTimeLimit;
};
#endif
