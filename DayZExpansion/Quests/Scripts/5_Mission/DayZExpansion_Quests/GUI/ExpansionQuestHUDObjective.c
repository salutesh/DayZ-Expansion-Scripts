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
		if (!m_QuestHUDObjectiveController)
			m_QuestHUDObjectiveController = ExpansionQuestHUDObjectiveController.Cast(GetController());

		vector objectivePos;
		vector playerPos;
		int currentDistance;
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

		if (m_Objective.GetTimeLimit() > -1)
		{
			ObjectiveTime.Show(true);
			m_QuestHUDObjectiveController.ObjectiveTimeLimit = "#STR_EXPANSION_QUEST_HUD_TIME " + ExpansionStatic.FormatTimestamp(m_Objective.GetTimeLimit(), false);
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

		switch (objectiveConfig.GetObjectiveType())
		{
			case ExpansionQuestObjectiveType.TARGET:
			{
				m_QuestHUDObjectiveController.ObjectiveTarget = "#STR_EXPANSION_QUEST_HUD_KILLED";
				m_QuestHUDObjectiveController.NotifyPropertyChanged("ObjectiveTarget");
				count = m_Objective.GetObjectiveCount();
				amount = m_Objective.GetObjectiveAmount();
				m_QuestHUDObjectiveController.ObjectiveValue = Math.Min(count, amount).ToString() + "/" + amount.ToString();
				m_QuestHUDObjectiveController.NotifyPropertyChanged("ObjectiveValue");
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
						objectivePos = m_Objective.GetObjectivePosition();
						playerPos = GetGame().GetPlayer().GetPosition();
						currentDistance = Math.Round(vector.Distance(playerPos, objectivePos));
						m_QuestHUDObjectiveController.ObjectiveValue = currentDistance.ToString() + " m";
						m_QuestHUDObjectiveController.NotifyPropertyChanged("ObjectiveValue");
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
						objectivePos = m_Objective.GetObjectivePosition();
						playerPos = GetGame().GetPlayer().GetPosition();
						currentDistance = Math.Round(vector.Distance(playerPos, objectivePos));
						m_QuestHUDObjectiveController.ObjectiveValue = currentDistance.ToString() + " m";
						m_QuestHUDObjectiveController.NotifyPropertyChanged("ObjectiveValue");
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
						ExpansionQuestHUDDeliveryObjective deliveryEntry = new ExpansionQuestHUDDeliveryObjective(delivery, currentDeliveryCount);
						m_QuestHUDObjectiveController.DeliveryEnties.Insert(deliveryEntry);
						
					}
				}
			}
			break;

			case ExpansionQuestObjectiveType.COLLECT:
			{
				ExpansionQuestObjectiveCollectionConfig collectionObjective;
				ExpansionQuestHUDDeliveryObjective collectionEntry;
				ExpansionQuestObjectiveDelivery collection;
				int currentCollectionCount;

				if (Class.CastTo(collectionObjective, objectiveConfig))
				{
					m_QuestHUDObjectiveController.ObjectiveTarget = "#STR_EXPANSION_QUEST_HUD_COLLECT";
					m_QuestHUDObjectiveController.NotifyPropertyChanged("ObjectiveTarget");
					if (collectionObjective.ShowDistance() && completed)
					{
						objectivePos = m_Objective.GetObjectivePosition();
						playerPos = GetGame().GetPlayer().GetPosition();
						currentDistance = Math.Round(vector.Distance(playerPos, objectivePos));
						m_QuestHUDObjectiveController.ObjectiveValue = currentDistance.ToString() + " m";
						m_QuestHUDObjectiveController.NotifyPropertyChanged("ObjectiveValue");
					}
					else if (!collectionObjective.ShowDistance() || !completed)
					{
						ObjectiveWrapper.Show(false);
					}

					ObjectiveDeliveryEnties.Show(true);
					array<ref ExpansionQuestObjectiveDelivery> collections = collectionObjective.GetDeliveries();
					
					map<int, int> collectionsMap = new map<int, int>;
					int collectionsCount;
					for (i = 0; i < collections.Count(); i++)
					{
						currentCollectionCount = m_Objective.GetDeliveryCountByIndex(i);				
						collection = collections[i];
						
						if (!collectionObjective.NeedAnyCollection())
						{
							collectionEntry = new ExpansionQuestHUDDeliveryObjective(collection, currentCollectionCount);					
							m_QuestHUDObjectiveController.DeliveryEnties.Insert(collectionEntry);
						}
						else if (collectionObjective.NeedAnyCollection() && currentCollectionCount >= collection.GetAmount())
						{
							collectionsCount += currentCollectionCount;
							collectionsMap.Insert(i, currentCollectionCount);	
						}
					}
					
					//! Steve: This is a mess. Don't look at it :(
					if (collectionObjective.NeedAnyCollection())
					{
						if (collectionsCount > 0)
						{
							foreach (int collectionIndex, int collectionCount: collectionsMap)
							{
								collection = collections[collectionIndex];
								if (!collection)
									continue;

								collectionEntry = new ExpansionQuestHUDDeliveryObjective(collection, collectionCount);					
								m_QuestHUDObjectiveController.DeliveryEnties.Insert(collectionEntry);
							}
						}
						else
						{
							for (i = 0; i < collections.Count(); i++)
							{
								currentCollectionCount = m_Objective.GetDeliveryCountByIndex(i);				
								collection = collections[i];
								collectionEntry = new ExpansionQuestHUDDeliveryObjective(collection, currentCollectionCount);					
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
						objectivePos = m_Objective.GetObjectivePosition();
						playerPos = GetGame().GetPlayer().GetPosition();
						currentDistance = Math.Round(vector.Distance(playerPos, objectivePos));
						m_QuestHUDObjectiveController.ObjectiveValue = currentDistance.ToString() + " m";
						m_QuestHUDObjectiveController.NotifyPropertyChanged("ObjectiveValue");
					}
					else
					{
						ObjectiveWrapper.Show(false);
					}
				}
			}
			break;

		#ifdef EXPANSIONMODAI
			case ExpansionQuestObjectiveType.AIPATROL:
			{
				m_QuestHUDObjectiveController.ObjectiveTarget = "#STR_EXPANSION_QUEST_HUD_KILLED";
				m_QuestHUDObjectiveController.NotifyPropertyChanged("ObjectiveTarget");

				count = m_Objective.GetObjectiveCount();
				amount = m_Objective.GetObjectiveAmount();

				m_QuestHUDObjectiveController.ObjectiveValue = Math.Min(count, amount).ToString() + "/" + amount.ToString();
				m_QuestHUDObjectiveController.NotifyPropertyChanged("ObjectiveValue");
			}
			break;

			case ExpansionQuestObjectiveType.AICAMP:
			{
				m_QuestHUDObjectiveController.ObjectiveTarget = "#STR_EXPANSION_QUEST_HUD_KILLED";
				m_QuestHUDObjectiveController.NotifyPropertyChanged("ObjectiveTarget");

				count = m_Objective.GetObjectiveCount();
				amount = m_Objective.GetObjectiveAmount();

				m_QuestHUDObjectiveController.ObjectiveValue = Math.Min(count, amount).ToString() + "/" + amount.ToString();
				m_QuestHUDObjectiveController.NotifyPropertyChanged("ObjectiveValue");
			}
			break;

			case ExpansionQuestObjectiveType.AIESCORT:
			{
				ExpansionQuestObjectiveAIEscortConfig escortConfig;
				if (Class.CastTo(escortConfig, objectiveConfig))
				{
					m_QuestHUDObjectiveController.ObjectiveTarget = "#STR_EXPANSION_QUEST_HUD_TRAVEL";
					m_QuestHUDObjectiveController.NotifyPropertyChanged("ObjectiveTarget");
					if (escortConfig.ShowDistance())
					{
						objectivePos = m_Objective.GetObjectivePosition();
						playerPos = GetGame().GetPlayer().GetPosition();
						currentDistance = Math.Round(vector.Distance(playerPos, objectivePos));
						m_QuestHUDObjectiveController.ObjectiveValue = currentDistance.ToString() + " m";
						m_QuestHUDObjectiveController.NotifyPropertyChanged("ObjectiveValue");
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
				{
					m_QuestHUDObjectiveController.ObjectiveTarget = actionConfig.ObjectiveText;
					m_QuestHUDObjectiveController.NotifyPropertyChanged("ObjectiveTarget");

					count = m_Objective.GetObjectiveCount();
					amount = m_Objective.GetObjectiveAmount();

					m_QuestHUDObjectiveController.ObjectiveValue = Math.Min(count, amount).ToString() + "/" + amount.ToString();
					m_QuestHUDObjectiveController.NotifyPropertyChanged("ObjectiveValue");
				}
			}
			break;

			case ExpansionQuestObjectiveType.CRAFTING:
			{
				ExpansionQuestObjectiveCraftingConfig craftingConfig;
				if (Class.CastTo(craftingConfig, objectiveConfig))
				{
					m_QuestHUDObjectiveController.ObjectiveTarget = craftingConfig.ObjectiveText;
					m_QuestHUDObjectiveController.NotifyPropertyChanged("ObjectiveTarget");

					count = m_Objective.GetObjectiveCount();
					amount = m_Objective.GetObjectiveAmount();

					m_QuestHUDObjectiveController.ObjectiveValue = Math.Min(count, amount).ToString() + "/" + amount.ToString();
					m_QuestHUDObjectiveController.NotifyPropertyChanged("ObjectiveValue");
				}
			}
			break;
		}
		
		if (!completed)
			Spacer.SetColor(ExpansionQuestModule.GetQuestColor(m_Quest));
		else
			Spacer.SetColor(ARGB(200, 160, 223, 59));
	}
};

class ExpansionQuestHUDObjectiveController: ExpansionViewController
{
	ref ObservableCollection<ref ExpansionQuestHUDDeliveryObjective> DeliveryEnties = new ObservableCollection<ref ExpansionQuestHUDDeliveryObjective>(this);
	string ObjectiveName;
	string ObjectiveTarget;
	string ObjectiveValue;
	string ObjectiveTimeLimit;
};
#endif
