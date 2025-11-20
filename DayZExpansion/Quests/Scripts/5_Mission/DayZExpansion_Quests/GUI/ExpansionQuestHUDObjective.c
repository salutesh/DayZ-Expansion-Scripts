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

class ExpansionQuestHUDObjective: ExpansionScriptView
{
	protected ref ExpansionQuestHUDObjectiveController m_QuestHUDObjectiveController;
	protected ExpansionQuestObjectiveData m_Objective;
	protected ExpansionQuestObjectiveConfig m_ObjectiveConfig;
	protected ExpansionQuestConfig m_QuestConfig;
	protected ExpansionQuestState m_QuestState;
	protected Widget Spacer;
	protected RichTextWidget ObjectiveName;
	protected RichTextWidget ObjectiveTime;
	protected WrapSpacerWidget ObjectiveWrapper;
	protected WrapSpacerWidget ObjectiveValueWrapper;
	protected WrapSpacerWidget ObjectiveDeliveryEnties;
	protected bool m_ObjectiveCompleted;
	protected int m_TimeLimit;
	protected int m_TimeCount;

	void ExpansionQuestHUDObjective(ExpansionQuestObjectiveData objective, ExpansionQuestConfig questConfig, ExpansionQuestState state)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif

		m_QuestHUDObjectiveController = ExpansionQuestHUDObjectiveController.Cast(GetController());
		m_Objective = objective;
		m_QuestConfig = questConfig;
		m_QuestState = state;
	}

	void ~ExpansionQuestHUDObjective()
	{
		if (g_Game && ObjectiveName)
			g_Game.GetCallQueue(CALL_CATEGORY_GUI).Remove(ObjectiveName.SetSize);
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
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif

		int count;
		int amount;
		int i;

		m_ObjectiveConfig = ExpansionQuestObjectiveConfig.Cast(m_QuestConfig.GetObjectives()[m_Objective.GetObjectiveIndex()]);
		if (!m_ObjectiveConfig)
			return;

		m_ObjectiveCompleted = m_Objective.IsCompleted();
		if (m_ObjectiveConfig.GetObjectiveText() != string.Empty)
		{
			ObjectiveName.Show(true);
			string objectiveState;
			if (m_ObjectiveCompleted)
			{
				string completeKey = "#STR_EXPANSION_QUEST_HUD_COMPLETE";
				objectiveState = "[" + completeKey + "] ";
			}
			else
			{
				string incompleteKey = "#STR_EXPANSION_QUEST_HUD_INCOMPLETE";
				objectiveState = "[" + incompleteKey + "] ";
			}

			m_QuestHUDObjectiveController.ObjectiveName = objectiveState + m_ObjectiveConfig.GetObjectiveText();
			m_QuestHUDObjectiveController.NotifyPropertyChanged("ObjectiveName");
			g_Game.GetCallQueue(CALL_CATEGORY_GUI).CallLater(ObjectiveName.SetSize, 100, false, 1.0, 1.0, true);
		}

		UpdateTimeLimit();

		switch (m_ObjectiveConfig.GetObjectiveType())
		{
			case ExpansionQuestObjectiveType.TARGET:
		#ifdef EXPANSIONMODAI
			case ExpansionQuestObjectiveType.AIPATROL:
			case ExpansionQuestObjectiveType.AICAMP:
		#endif
			{
				ObjectiveWrapper.Show(false);
				UpdateTarget();
			}
			break;
			case  ExpansionQuestObjectiveType.TRAVEL:
			{
				ExpansionQuestObjectiveTravelConfig travelObjective;
				if (Class.CastTo(travelObjective, m_ObjectiveConfig))
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
						ObjectiveValueWrapper.Show(false);
					}
				}
			}
			break;

			case ExpansionQuestObjectiveType.DELIVERY:
			{
				ExpansionQuestObjectiveDeliveryConfig deliveryObjective;
				if (Class.CastTo(deliveryObjective, m_ObjectiveConfig))
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
						ObjectiveValueWrapper.Show(false);
					}
 					
					ObjectiveDeliveryEnties.Show(true);
					array<ref ExpansionQuestObjectiveDelivery> deliveries = deliveryObjective.GetCollections();

					for (i = 0; i < deliveries.Count(); i++)
					{
						int currentDeliveryCount = m_Objective.GetDeliveryCountByIndex(i);
						ExpansionQuestObjectiveDelivery delivery = deliveries[i];
						ExpansionQuestHUDDeliveryEntry deliveryEntry = new ExpansionQuestHUDDeliveryEntry(delivery, currentDeliveryCount, this);
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

				if (Class.CastTo(collectionObjective, m_ObjectiveConfig))
				{
					m_QuestHUDObjectiveController.ObjectiveTarget = "#STR_EXPANSION_QUEST_HUD_COLLECT";
					m_QuestHUDObjectiveController.NotifyPropertyChanged("ObjectiveTarget");

					ObjectiveDeliveryEnties.Show(true);
					array<ref ExpansionQuestObjectiveDelivery> collections = collectionObjective.GetCollections();
					
					map<int, int> collectionsMap = new map<int, int>;
					int completedCollections;
					
					for (i = 0; i < collections.Count(); i++)
					{
						currentCollectionCount = m_Objective.GetDeliveryCountByIndex(i);				
						collection = collections[i];									
						if (!collectionObjective.NeedAnyCollection())
						{
							collectionEntry = new ExpansionQuestHUDDeliveryEntry(collection, currentCollectionCount, this);					
							m_QuestHUDObjectiveController.DeliveryEnties.Insert(collectionEntry);
							if (currentCollectionCount >= collection.GetAmount())
								completedCollections++;
						}
						else if (currentCollectionCount >= collection.GetAmount())
						{
							collectionsMap.Insert(i, currentCollectionCount);	
						}
					}

					if (collectionObjective.ShowDistance() && (collectionObjective.NeedAnyCollection() || completedCollections == collections.Count()))
					{
						UpdateDistance();
					}
					else
					{
						ObjectiveWrapper.Show(false);
						ObjectiveValueWrapper.Show(false);
					}

					if (collectionObjective.NeedAnyCollection())
					{
						if (collectionsMap.Count() > 0)
						{
							foreach (int collectionIndex, int collectionCount: collectionsMap)
							{
								collection = collections[collectionIndex];
								if (!collection)
									continue;

								collectionEntry = new ExpansionQuestHUDDeliveryEntry(collection, collectionCount, this);					
								m_QuestHUDObjectiveController.DeliveryEnties.Insert(collectionEntry);
							}
						}
						else
						{
							for (i = 0; i < collections.Count(); i++)
							{
								currentCollectionCount = m_Objective.GetDeliveryCountByIndex(i);				
								collection = collections[i];

								collectionEntry = new ExpansionQuestHUDDeliveryEntry(collection, currentCollectionCount, this);					
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
				if (Class.CastTo(treasureObjective, m_ObjectiveConfig))
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
						ObjectiveValueWrapper.Show(false);
					}
				}
			}
			break;

		#ifdef EXPANSIONMODAI
			case ExpansionQuestObjectiveType.AIESCORT:
			{
				ExpansionQuestObjectiveAIEscortConfig escortConfig;
				if (Class.CastTo(escortConfig, m_ObjectiveConfig))
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
						ObjectiveValueWrapper.Show(false);
					}
				}
			}
			break;
		#endif

			case ExpansionQuestObjectiveType.ACTION:
			{
				ObjectiveWrapper.Show(false);
				UpdateTarget();
			}
			break;

			case ExpansionQuestObjectiveType.CRAFTING:
			{
				ObjectiveWrapper.Show(false);
				UpdateTarget();
			}
			break;
		}
				
		if (m_ObjectiveConfig.GetObjectiveText() != string.Empty && m_QuestHUDObjectiveController.DeliveryEnties.Count() == 0)
		{
			if (!m_ObjectiveCompleted)
				Spacer.SetColor(ExpansionQuestModule.GetQuestColor(m_QuestConfig));
			else
				Spacer.SetColor(ARGB(200, 160, 223, 59));

			Spacer.Show(true);
		}
		else
		{
			Spacer.Show(false);
		}
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
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif
		
		m_Objective = objectiveData;
		
		m_QuestHUDObjectiveController.DeliveryEnties.Clear();
		
		SetEntryObjective();
	}
	
	override void Expansion_Update()
	{
		if (!m_Objective || !m_QuestConfig || !IsVisible() || !g_Game.GetPlayer())
			return;
		
		UpdateTimeLimit();
		
		if (!ObjectiveWrapper.IsVisible())
			return;

		array<ref ExpansionQuestObjectiveConfigBase> questObjectives = m_QuestConfig.GetObjectives();
		if (!questObjectives || questObjectives.Count() == 0)
			return;
		
		if (!m_ObjectiveConfig)
			return;
		
		//! @note: Update quest objective distance in HUD.
		switch (m_ObjectiveConfig.GetObjectiveType())
		{
			case  ExpansionQuestObjectiveType.TRAVEL:
			{
				ExpansionQuestObjectiveTravelConfig travelObjective;
				if (Class.CastTo(travelObjective, m_ObjectiveConfig) && travelObjective.ShowDistance())
					UpdateDistance();
			}
			break;

			case ExpansionQuestObjectiveType.DELIVERY:
			{
				ExpansionQuestObjectiveDeliveryConfig deliveryObjective;
				if (Class.CastTo(deliveryObjective, m_ObjectiveConfig) && deliveryObjective.ShowDistance())
					UpdateDistance();
			}
			break;

			case ExpansionQuestObjectiveType.COLLECT:
			{
				ExpansionQuestObjectiveCollectionConfig collectionObjective;
				if (Class.CastTo(collectionObjective, m_ObjectiveConfig) && collectionObjective.ShowDistance())
					UpdateDistance();
			}
			break;

			case ExpansionQuestObjectiveType.TREASUREHUNT:
			{
				ExpansionQuestObjectiveTreasureHuntConfig treasureObjective;
				if (Class.CastTo(treasureObjective, m_ObjectiveConfig) && treasureObjective.ShowDistance())
					UpdateDistance();
			}
			break;

		#ifdef EXPANSIONMODAI
			case ExpansionQuestObjectiveType.AIESCORT:
			{
				ExpansionQuestObjectiveAIEscortConfig escortConfig;
				if (Class.CastTo(escortConfig, m_ObjectiveConfig) && escortConfig.ShowDistance())
					UpdateDistance();
			}
			break;
		#endif
		}
	}
	
	void UpdateTimeLimit()
	{
		int timeLimit = m_Objective.GetTimeLimit();
		if (timeLimit > -1)
		{
			ObjectiveTime.Show(true);

			if (timeLimit > 0 && timeLimit == m_TimeLimit)
			{
				if (timeLimit - m_TimeCount > 0)
				{
					m_TimeCount++;
					timeLimit -= m_TimeCount;
				}
				else
				{
					timeLimit = 0;
				}
			}
			else
			{
				m_TimeCount = 0;
				m_TimeLimit = timeLimit;
			}

			EXPrint(string.Format("Time limit: %1 - %2 = %3", m_Objective.GetTimeLimit(), m_TimeCount, timeLimit));

			m_QuestHUDObjectiveController.ObjectiveTimeLimit = "#STR_EXPANSION_QUEST_HUD_TIME " + ExpansionStatic.FormatTime(timeLimit, false);
			m_QuestHUDObjectiveController.NotifyPropertyChanged("ObjectiveTimeLimit");

			if (timeLimit <= 10)
			{
				ObjectiveTime.SetColor(COLOR_EXPANSION_NOTIFICATION_ERROR);
			}
			else if (timeLimit <= 60)
			{
				ObjectiveTime.SetColor(COLOR_EXPANSION_NOTIFICATION_ORANGE);
			}
			else
			{
				ObjectiveTime.SetColor(COLOR_WHITE);
			}
		}
	}

	void UpdateDistance()
	{
		vector objectivePos = m_Objective.GetObjectivePosition();
	#ifdef EXPANSIONMODAI
		if (m_ObjectiveConfig.IsInherited(ExpansionQuestObjectiveAIEscortConfig))
			objectivePos = m_ObjectiveConfig.GetPosition();  //! We want pos of target location, not current objective (VIP) pos
	#endif
		vector playerPos = g_Game.GetPlayer().GetPosition();
		int currentDistance = Math.Round(vector.Distance(playerPos, objectivePos));
		m_QuestHUDObjectiveController.ObjectiveValue = currentDistance.ToString() + " m";
		m_QuestHUDObjectiveController.NotifyPropertyChanged("ObjectiveValue");
		ObjectiveWrapper.Show(true);
		ObjectiveValueWrapper.Show(true);
	}
	
	ExpansionQuestConfig GetQuestConfig()
	{
		return m_QuestConfig;
	}
	
	bool IsObjectiveCompleted()
	{
		return m_ObjectiveCompleted;
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
