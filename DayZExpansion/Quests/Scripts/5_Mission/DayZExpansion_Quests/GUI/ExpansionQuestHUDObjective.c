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
		QuestDebugPrint(ToString() + "::SetEntryObjective - Start");
		QuestDebugPrint(ToString() + "::SetEntryObjective - objective type is: " + m_Objective.GetObjectiveType());

		if (!m_QuestHUDObjectiveController)
			m_QuestHUDObjectiveController = ExpansionQuestHUDObjectiveController.Cast(GetController());

		Spacer.SetColor(ExpansionQuestModule.GetQuestColor(m_Quest));

		vector objectivePos;
		vector playerPos;
		int currentDistance;
		int count;
		int amount;

		ExpansionQuestObjectiveConfig objectiveConfigBase = m_Quest.GetObjectives()[m_Objective.GetObjectiveIndex()];
		if (!objectiveConfigBase)
			return;

		QuestDebugPrint(ToString() + "::SetEntryObjective - Objective config: " + objectiveConfigBase);

		bool completed = m_Objective.IsCompleted();
		if (objectiveConfigBase.GetObjectiveText() != string.Empty)
		{
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

			m_QuestHUDObjectiveController.ObjectiveName = objectiveState + objectiveConfigBase.GetObjectiveText();
			m_QuestHUDObjectiveController.NotifyPropertyChanged("ObjectiveName");
		}
		else
		{
			ObjectiveName.Show(false);
		}

		if (m_Objective.GetTimeLimit() > -1)
		{
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
		else
		{
			ObjectiveTime.Show(false);
		}

		switch (objectiveConfigBase.GetObjectiveType())
		{
			case ExpansionQuestObjectiveType.TARGET:
			{
				QuestDebugPrint(ToString() + "::SetEntryObjective - TARGET");
				m_QuestHUDObjectiveController.ObjectiveTarget = "#STR_EXPANSION_QUEST_HUD_KILLED";
				m_QuestHUDObjectiveController.NotifyPropertyChanged("ObjectiveTarget");
				count = m_Objective.GetObjectiveCount();
				amount = m_Objective.GetObjectiveAmount();
				QuestDebugPrint(ToString() + "::SetEntryObjective - Count: " + count);
				QuestDebugPrint(ToString() + "::SetEntryObjective - Amount: " + amount);
				m_QuestHUDObjectiveController.ObjectiveValue = Math.Min(count, amount).ToString() + "/" + amount.ToString();
				m_QuestHUDObjectiveController.NotifyPropertyChanged("ObjectiveValue");
				QuestDebugPrint(ToString() + "::SetEntryObjective - TARGET - ADDED");
			}
			break;

			case  ExpansionQuestObjectiveType.TRAVEL:
			{
				QuestDebugPrint(ToString() + "::SetEntryObjective - TRAVEL");
				ExpansionQuestObjectiveTravelConfig travelObjective;
				if (Class.CastTo(travelObjective, objectiveConfigBase))
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
					QuestDebugPrint(ToString() + "::SetEntryObjective - TRAVEL - ADDED");
				}
			}
			break;

			case ExpansionQuestObjectiveType.DELIVERY:
			{
				QuestDebugPrint(ToString() + "::SetEntryObjective - DELIVERY");
				ExpansionQuestObjectiveDeliveryConfig deliveryObjective;
				if (Class.CastTo(deliveryObjective, objectiveConfigBase))
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

					array<ref ExpansionQuestObjectiveDelivery> deliveries = deliveryObjective.GetDeliveries();

					int deliveryIndex = 0;
					foreach (ExpansionQuestObjectiveDelivery delivery: deliveries)
					{
						int currentDeliveryCount = m_Objective.GetDeliveryCountByIndex(deliveryIndex);
						ExpansionQuestHUDDeliveryObjective deliveryEntry = new ExpansionQuestHUDDeliveryObjective(delivery, currentDeliveryCount);
						m_QuestHUDObjectiveController.DeliveryEnties.Insert(deliveryEntry);
						deliveryIndex++;
					}
					QuestDebugPrint(ToString() + "::SetEntryObjective - DELIVERY - ADDED");
				}
			}
			break;

			case ExpansionQuestObjectiveType.COLLECT:
			{
				QuestDebugPrint(ToString() + "::SetEntryObjective - COLLECT");
				ExpansionQuestObjectiveCollectionConfig collectionObjective;
				if (Class.CastTo(collectionObjective, objectiveConfigBase))
				{
					m_QuestHUDObjectiveController.ObjectiveTarget = "#STR_EXPANSION_QUEST_HUD_COLLECT";
					m_QuestHUDObjectiveController.NotifyPropertyChanged("ObjectiveTarget");
					if (collectionObjective.ShowDistance())
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

					array<ref ExpansionQuestObjectiveDelivery> collections = collectionObjective.GetDeliveries();

					QuestDebugPrint(ToString() + "::SetEntryObjective - COLLECT - Collections data count: " + collections.Count());

					int collectionIndex = 0;
					foreach (ExpansionQuestObjectiveDelivery collection: collections)
					{
						int currentCollectionCount = m_Objective.GetDeliveryCountByIndex(collectionIndex);
						QuestDebugPrint(ToString() + "::SetEntryObjective - COLLECT - Collection index: " + collectionIndex + " | Count: " + currentCollectionCount);

						ExpansionQuestHUDDeliveryObjective collectionEntry = new ExpansionQuestHUDDeliveryObjective(collection, currentCollectionCount);
						m_QuestHUDObjectiveController.DeliveryEnties.Insert(collectionEntry);
						collectionIndex++;
					}
					QuestDebugPrint(ToString() + "::SetEntryObjective - COLLECT - ADDED");
				}
			}
			break;

			case ExpansionQuestObjectiveType.TREASUREHUNT:
			{
				QuestDebugPrint(ToString() + "::SetEntryObjective - TREASUREHUNT");
				ExpansionQuestObjectiveTreasureHuntConfig treasureObjective;
				if (Class.CastTo(treasureObjective, objectiveConfigBase))
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
					QuestDebugPrint(ToString() + "::SetEntryObjective - TREASUREHUNT - ADDED");
				}
			}
			break;

		#ifdef EXPANSIONMODAI
			case ExpansionQuestObjectiveType.AIPATROL:
			{
				QuestDebugPrint(ToString() + "::SetEntryObjective - AIPATROL");
				m_QuestHUDObjectiveController.ObjectiveTarget = "#STR_EXPANSION_QUEST_HUD_KILLED";
				m_QuestHUDObjectiveController.NotifyPropertyChanged("ObjectiveTarget");

				count = m_Objective.GetObjectiveCount();
				amount = m_Objective.GetObjectiveAmount();

				QuestDebugPrint(ToString() + "::SetEntryObjective - Count: " + count);
				QuestDebugPrint(ToString() + "::SetEntryObjective - Amount: " + amount);

				m_QuestHUDObjectiveController.ObjectiveValue = Math.Min(count, amount).ToString() + "/" + amount.ToString();
				m_QuestHUDObjectiveController.NotifyPropertyChanged("ObjectiveValue");

				QuestDebugPrint(ToString() + "::SetEntryObjective - AIPATROL - ADDED");
			}
			break;

			case ExpansionQuestObjectiveType.AICAMP:
			{
				QuestDebugPrint(ToString() + "::SetEntryObjective - AICAMP");
				m_QuestHUDObjectiveController.ObjectiveTarget = "#STR_EXPANSION_QUEST_HUD_KILLED";
				m_QuestHUDObjectiveController.NotifyPropertyChanged("ObjectiveTarget");

				count = m_Objective.GetObjectiveCount();
				amount = m_Objective.GetObjectiveAmount();

				QuestDebugPrint(ToString() + "::SetEntryObjective - Count: " + count);
				QuestDebugPrint(ToString() + "::SetEntryObjective - Amount: " + amount);

				m_QuestHUDObjectiveController.ObjectiveValue = Math.Min(count, amount).ToString() + "/" + amount.ToString();
				m_QuestHUDObjectiveController.NotifyPropertyChanged("ObjectiveValue");

				QuestDebugPrint(ToString() + "::SetEntryObjective - AICAMP - ADDED");
			}
			break;

			case ExpansionQuestObjectiveType.AIVIP:
			{
				QuestDebugPrint(ToString() + "::SetEntryObjective - AIVIP");
				ExpansionQuestObjectiveAIVIPConfig vipConfig;
				if (Class.CastTo(vipConfig, objectiveConfigBase))
				{
					m_QuestHUDObjectiveController.ObjectiveTarget = "#STR_EXPANSION_QUEST_HUD_TRAVEL";
					m_QuestHUDObjectiveController.NotifyPropertyChanged("ObjectiveTarget");
					if (vipConfig.ShowDistance())
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
					QuestDebugPrint(ToString() + "::SetEntryObjective - AIVIP - ADDED");
				}
			}
			break;
		#endif

			case ExpansionQuestObjectiveType.ACTION:
			{
				QuestDebugPrint(ToString() + "::SetEntryObjective - ACTION");
				m_QuestHUDObjectiveController.ObjectiveTarget = "Action used: ";
				m_QuestHUDObjectiveController.NotifyPropertyChanged("ObjectiveTarget");
				m_QuestHUDObjectiveController.ObjectiveValue = m_Objective.GetActionState().ToString();
				m_QuestHUDObjectiveController.NotifyPropertyChanged("ObjectiveValue");
				QuestDebugPrint(ToString() + "::SetEntryObjective - ACTION - ADDED");
			}
			break;

			case ExpansionQuestObjectiveType.CRAFTING:
			{
				QuestDebugPrint(ToString() + "::SetEntryObjective - CRAFTING");
				m_QuestHUDObjectiveController.ObjectiveTarget = "Item crafted: ";
				m_QuestHUDObjectiveController.NotifyPropertyChanged("ObjectiveTarget");
				m_QuestHUDObjectiveController.ObjectiveValue = m_Objective.GetActionState().ToString();
				m_QuestHUDObjectiveController.NotifyPropertyChanged("ObjectiveValue");
				QuestDebugPrint(ToString() + "::SetEntryObjective - CRAFTING - ADDED");
			}
			break;
		}

		QuestDebugPrint(ToString() + "::SetEntryObjective - End");
	}

	void QuestDebugPrint(string text)
	{
	#ifdef EXPANSIONMODQUESTSUIDEBUG
		Print(text);
	#endif
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
