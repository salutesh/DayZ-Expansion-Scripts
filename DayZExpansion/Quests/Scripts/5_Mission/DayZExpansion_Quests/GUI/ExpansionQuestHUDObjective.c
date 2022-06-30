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
	private ref ExpansionQuestHUDObjectiveController m_QuestHUDObjectiveController;
	private ExpansionQuestObjectiveData m_Objective;
	private ExpansionQuestConfig m_Quest;
	private Widget Spacer;
	private RichTextWidget ObjectiveName;
	private RichTextWidget ObjectiveTime;
	private WrapSpacerWidget ObjectiveWrapper;

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
		QuestPrint(ToString() + "::SetEntryObjective - Start");
		QuestPrint(ToString() + "::SetEntryObjective - objective type is: " + m_Objective.GetObjectiveType());

		if (!m_QuestHUDObjectiveController)
			m_QuestHUDObjectiveController = ExpansionQuestHUDObjectiveController.Cast(GetController());

		Spacer.SetColor(GetQuestColor(m_Quest));

		vector objectivePos;
		vector playerPos;
		int currentDistance;
		int count;
		int amount;

		ExpansionQuestObjectiveConfig objectiveConfigBase = GetObjectiveConfigBaseByType(m_Quest, m_Objective.GetObjectiveType());
		if (!objectiveConfigBase)
			return;

		QuestPrint(ToString() + "::SetEntryObjective - Objective config: " + objectiveConfigBase);

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
				QuestPrint(ToString() + "::SetEntryObjective - TARGET");
				m_QuestHUDObjectiveController.ObjectiveTarget = "#STR_EXPANSION_QUEST_HUD_KILLED";
				m_QuestHUDObjectiveController.NotifyPropertyChanged("ObjectiveTarget");
				count = m_Objective.GetObjectiveCount();
				amount = m_Objective.GetObjectiveAmount();
				QuestPrint(ToString() + "::SetEntryObjective - Count: " + count);
				QuestPrint(ToString() + "::SetEntryObjective - Amount: " + amount);
				m_QuestHUDObjectiveController.ObjectiveValue = count.ToString() + "/" + amount.ToString();
				m_QuestHUDObjectiveController.NotifyPropertyChanged("ObjectiveValue");
				QuestPrint(ToString() + "::SetEntryObjective - TARGET - ADDED");
			}
			break;

			case  ExpansionQuestObjectiveType.TRAVEL:
			{
				QuestPrint(ToString() + "::SetEntryObjective - TRAVEL");
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
					QuestPrint(ToString() + "::SetEntryObjective - TRAVEL - ADDED");
				}
			}
			break;

			case ExpansionQuestObjectiveType.COLLECT:
			{
				QuestPrint(ToString() + "::SetEntryObjective - COLLECT");
				ExpansionQuestObjectiveCollectionConfig collectionObjective;
				if (Class.CastTo(collectionObjective, objectiveConfigBase))
				{
					ExpansionQuestObjectiveCollection collection = collectionObjective.GetCollection();
					if (collection)
					{
						string displayName = ExpansionStatic.GetItemDisplayNameWithType(collection.GetClassName());
						m_QuestHUDObjectiveController.ObjectiveTarget = "#STR_EXPANSION_QUEST_HUD_COLLECT " + displayName;
						m_QuestHUDObjectiveController.NotifyPropertyChanged("ObjectiveTarget");
						m_QuestHUDObjectiveController.ObjectiveValue = m_Objective.GetObjectiveCount().ToString() + "/" + collection.GetAmount().ToString();
						m_QuestHUDObjectiveController.NotifyPropertyChanged("ObjectiveValue");
						QuestPrint(ToString() + "::SetEntryObjective - COLLECT - ADDED");
					}
				}
			}
			break;

			case ExpansionQuestObjectiveType.TREASUREHUNT:
			{
				QuestPrint(ToString() + "::SetEntryObjective - TREASUREHUNT");
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
					QuestPrint(ToString() + "::SetEntryObjective - TREASUREHUNT - ADDED");
				}
			}
			break;

			case ExpansionQuestObjectiveType.DELIVERY:
			{
				QuestPrint(ToString() + "::SetEntryObjective - DELIVERY");
				m_QuestHUDObjectiveController.ObjectiveTarget = "#STR_EXPANSION_QUEST_HUD_DELIVER";
				m_QuestHUDObjectiveController.NotifyPropertyChanged("ObjectiveTarget");

				objectivePos = m_Objective.GetObjectivePosition();
				playerPos = GetGame().GetPlayer().GetPosition();
				currentDistance = Math.Round(vector.Distance(playerPos, objectivePos));
				m_QuestHUDObjectiveController.ObjectiveValue = currentDistance.ToString() + " m";
				m_QuestHUDObjectiveController.NotifyPropertyChanged("ObjectiveValue");
				ExpansionQuestObjectiveDeliveryConfig deliveryObjective;
				if (Class.CastTo(deliveryObjective, objectiveConfigBase))
				{
					for (int i = 0; i < deliveryObjective.GetDeliveries().Count(); i++)
					{
						ExpansionQuestObjectiveDelivery delivery = deliveryObjective.GetDeliveries()[i];
						if (delivery)
						{
							ExpansionQuestHUDDeliveryObjective deliveryEntry = new ExpansionQuestHUDDeliveryObjective(delivery);
							m_QuestHUDObjectiveController.DeliveryEnties.Insert(deliveryEntry);
						}
					}
					QuestPrint(ToString() + "::SetEntryObjective - DELIVERY - ADDED");
				}
			}
			break;

		#ifdef EXPANSIONMODAI
			case ExpansionQuestObjectiveType.AIPATROL:
			{
				QuestPrint(ToString() + "::SetEntryObjective - AIPATROL");
				m_QuestHUDObjectiveController.ObjectiveTarget = "#STR_EXPANSION_QUEST_HUD_KILLED";
				m_QuestHUDObjectiveController.NotifyPropertyChanged("ObjectiveTarget");

				count = m_Objective.GetObjectiveCount();
				amount = m_Objective.GetObjectiveAmount();

				QuestPrint(ToString() + "::SetEntryObjective - Count: " + count);
				QuestPrint(ToString() + "::SetEntryObjective - Amount: " + amount);

				m_QuestHUDObjectiveController.ObjectiveValue = count.ToString() + "/" + amount.ToString();
				m_QuestHUDObjectiveController.NotifyPropertyChanged("ObjectiveValue");

				QuestPrint(ToString() + "::SetEntryObjective - AIPATROL - ADDED");
			}
			break;

			case ExpansionQuestObjectiveType.AICAMP:
			{
				QuestPrint(ToString() + "::SetEntryObjective - AICAMP");
				m_QuestHUDObjectiveController.ObjectiveTarget = "#STR_EXPANSION_QUEST_HUD_KILLED";
				m_QuestHUDObjectiveController.NotifyPropertyChanged("ObjectiveTarget");

				count = m_Objective.GetObjectiveCount();
				amount = m_Objective.GetObjectiveAmount();

				QuestPrint(ToString() + "::SetEntryObjective - Count: " + count);
				QuestPrint(ToString() + "::SetEntryObjective - Amount: " + amount);

				m_QuestHUDObjectiveController.ObjectiveValue = count.ToString() + "/" + amount.ToString();
				m_QuestHUDObjectiveController.NotifyPropertyChanged("ObjectiveValue");

				QuestPrint(ToString() + "::SetEntryObjective - AICAMP - ADDED");
			}
			break;

			case ExpansionQuestObjectiveType.AIVIP:
			{
				QuestPrint(ToString() + "::SetEntryObjective - AIVIP");
				m_QuestHUDObjectiveController.ObjectiveTarget = "#STR_EXPANSION_QUEST_HUD_TRAVEL";
				m_QuestHUDObjectiveController.NotifyPropertyChanged("ObjectiveTarget");
				objectivePos = m_Objective.GetObjectivePosition();
				playerPos = GetGame().GetPlayer().GetPosition();
				currentDistance = Math.Round(vector.Distance(playerPos, objectivePos));
				m_QuestHUDObjectiveController.ObjectiveValue = currentDistance.ToString() + " m";
				m_QuestHUDObjectiveController.NotifyPropertyChanged("ObjectiveValue");
				QuestPrint(ToString() + "::SetEntryObjective - AIVIP - ADDED");
			}
			break;
		#endif
		}

		QuestPrint(ToString() + "::SetEntryObjective - End");
	}

	ExpansionQuestObjectiveConfig GetObjectiveConfigBaseByType(ExpansionQuestConfig questConfig, int objectiveType)
	{
		for (int i = 0; i < questConfig.GetObjectives().Count(); i++)
		{
			ExpansionQuestObjectiveConfig objectiveConfigBase = questConfig.GetObjectives()[i];
			if (!objectiveConfigBase)
				continue;

			if (objectiveConfigBase.GetObjectiveType() == objectiveType)
				return objectiveConfigBase;
		}

		return NULL;
	}

	private int GetQuestColor(ExpansionQuestConfig quest)
	{
		int color;
	#ifdef EXPANSIONMODHARDLINE
	#ifdef EXPANSIONMODAI
		if (quest.IsRepeatable() && !quest.IsGroupQuest() && !quest.IsBanditQuest() && !quest.IsHeroQuest() && quest.GetType() != ExpansionQuestType.AIPATROL && quest.GetType() != ExpansionQuestType.AICAMP)
	#else
		if (quest.IsRepeatable() && !quest.IsGroupQuest() && !quest.IsBanditQuest() && !quest.IsHeroQuest())
	#endif
	#else
		if (quest.IsRepeatable() && !quest.IsGroupQuest())
	#endif
		{
			color = ARGB(120, 52, 152, 219);
		}
		else if (quest.IsGroupQuest())
		{
			color = ARGB(120, 226, 65, 66);
		}
		else if (!quest.IsGroupQuest() && !quest.IsRepeatable())
		{
			color = ARGB(120, 241, 196, 15);
		}
	#ifdef EXPANSIONMODHARDLINE
		else if (quest.IsBanditQuest())
		{
			color = ARGB(120, 225, 112, 85);
		}
		else if (quest.IsHeroQuest())
		{
			color = ARGB(120, 26, 188, 156);
		}
	#endif
	#ifdef EXPANSIONMODAI
		else if (quest.GetType() == ExpansionQuestType.AIPATROL || quest.GetType() == ExpansionQuestType.AICAMP)
		{
			color = ARGB(120, 75, 101, 132);
		}
	#endif

		return color;
	}

	void QuestPrint(string text)
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
