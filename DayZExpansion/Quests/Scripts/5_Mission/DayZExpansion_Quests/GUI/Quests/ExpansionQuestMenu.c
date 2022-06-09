/**
 * ExpansionQuestMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestMenu: ExpansionScriptViewMenu
{
	private ref ExpansionQuestMenuController m_QuestMenuController;
	private ExpansionQuestModule m_QuestModule;
	private ref array<ref ExpansionQuestConfig> m_Quests = new array<ref ExpansionQuestConfig>;
	private ref ExpansionQuestConfig m_SelectedQuest;

	private Widget QuestListPanel;
	private Widget QuestDetailsPanel;
	private ButtonWidget Accept;
	private TextWidget AcceptLable;
	private ButtonWidget Complete;
	private TextWidget CompleteLable;
	private ButtonWidget Cancel;
	private TextWidget CancelLable;
	private ButtonWidget Close;
	private TextWidget CloseLable;
	private WrapSpacerWidget ButtonsPanel;

	private Widget RewardPanel;
	private ScrollWidget ObjectiveSectionScroller;
	private RichTextWidget Objective;
	private WrapSpacerWidget QuestListContent;
	private Widget DefaultPanel;
	private Widget Humanity;

	void ExpansionQuestMenu()
	{
		Class.CastTo(m_QuestModule, CF_ModuleCoreManager.Get(ExpansionQuestModule));
		Class.CastTo(m_QuestMenuController, GetController());

		m_QuestModule.GetQuestMenuSI().Insert(SetQuests);
		QuestListPanel.Show(true);
		QuestDetailsPanel.Show(false);
	}

	void ~ExpansionQuestMenu()
	{
		m_QuestModule.GetQuestMenuSI().Remove(SetQuests);
		m_QuestMenuController.Quests.Clear();
	}

	override string GetLayoutFile()
	{
		return "DayZExpansion/Quests/GUI/layouts/quests/expansion_quest_menu.layout";
	}

	override typename GetControllerType()
	{
		return ExpansionQuestMenuController;
	}

	void SetQuests(array<ref ExpansionQuestConfig> quests, ExpansionQuestNpcData questNPCData)
	{
		QuestDebug(ToString() + "::SetQuests - Start");
		QuestDebug(ToString() + "::SetQuests - Quest: " + quests.ToString());

		if (!m_QuestModule)
			return;

		ButtonsPanel.Show(false);
		
		if (quests.Count() > 0)
		{
			QuestListContent.Show(true);
			DefaultPanel.Show(false);

			for (int i = 0; i < quests.Count(); i++)
			{
				ExpansionQuestConfig quest = quests[i];
				if (!quest || quest.IsAchivement())
					continue;

				m_Quests.Insert(quest);
				ExpansionQuestMenuListEntry questEntry = new ExpansionQuestMenuListEntry(quest, this);
				m_QuestMenuController.Quests.Insert(questEntry);
			}
		}
		else if (quests.Count() == 0)
		{
			if (!questNPCData)
				return;

			QuestListContent.Show(false);
			DefaultPanel.Show(true);

			m_QuestMenuController.DefaultText = questNPCData.GetDefaultNPCText();
			m_QuestMenuController.NotifyPropertyChanged("DefaultText");
		}

		QuestDebug(ToString() + "::SetQuests - End");
	}

	void OnAcceptButtonClick()
	{
		if (!m_SelectedQuest)
			return;

		m_QuestModule.AcceptQuestClient(m_SelectedQuest);
		CloseMenu();
	}

	void OnCancelButtonClick()
	{
		QuestListPanel.Show(true);
		ButtonsPanel.Show(false);
		QuestDetailsPanel.Show(false);
	}

	void OnCompleteButtonClick()
	{
		m_QuestModule.RequestTurnInQuestClient(m_SelectedQuest.GetID());
		ButtonsPanel.Show(false);
		CloseMenu();
	}

	void SetQuest(ExpansionQuestConfig quest)
	{
		QuestDebug(ToString() + "::SetQuest - Start");

		if (!m_QuestModule)
			return;

		bool hasQuestState = false;
		ExpansionQuestState questState = m_QuestModule.GetClientQuestData().GetQuestStateByQuestID(quest.GetID());
		if (questState)
			hasQuestState = true;

		QuestDebug(ToString() + "::SetQuest - Quest state: " + questState);
		QuestListPanel.Show(false);
		ButtonsPanel.Show(true);
		QuestDetailsPanel.Show(true);
		Complete.Show(false);

		m_SelectedQuest = quest;
		m_QuestMenuController.QuestTitle = quest.GetTitle();

		string description;
		if (!hasQuestState || hasQuestState && questState == ExpansionQuestState.NONE)
		{
			description = quest.GetDescriptions()[0];
			Accept.Show(true);
		}
		else if (hasQuestState && questState == ExpansionQuestState.STARTED)
		{
			description = quest.GetDescriptions()[1];
			Accept.Show(false);
		}
		else if (hasQuestState && questState == ExpansionQuestState.CAN_TURNIN)
		{
			description = quest.GetDescriptions()[2];
			Accept.Show(false);
			Complete.Show(true);
		}

		m_QuestMenuController.QuestDescription = description;
		m_QuestMenuController.QuestObjective = quest.GetObjectiveText();

		m_QuestMenuController.NotifyPropertiesChanged({"QuestTitle", "QuestDescription", "QuestObjective"});

		m_QuestMenuController.RewardEntries.Clear();
		int rewardsCount = quest.GetRewards().Count();
		int i;

		if (rewardsCount== 0)
		{
			RewardPanel.Show(false);
		}
		else if (rewardsCount > 0)
		{
			RewardPanel.Show(true);
			int overallHumanityFromAllQuests;
			for (i = 0; i < quest.GetRewards().Count(); i++)
			{
				ExpansionQuestRewardConfig reward = quest.GetRewards()[i];
				ExpansionQuestMenuItemEntry rewardEntry = new ExpansionQuestMenuItemEntry(reward.GetClassName(), reward.GetAmount());
				m_QuestMenuController.RewardEntries.Insert(rewardEntry);

			#ifdef EXPANSIONMODHARDLINE
				overallHumanityFromAllQuests += reward.GetHumanity();
			#endif
			}

			if (overallHumanityFromAllQuests > 0)
			{
				Humanity.Show(true);
				m_QuestMenuController.HumanityVal = overallHumanityFromAllQuests.ToString();
				m_QuestMenuController.NotifyPropertyChanged("HumanityVal");
			}
			else
			{
				Humanity.Show(false);
			}
		}

		m_QuestMenuController.ObjectiveItems.Clear();

		for (i = 0; i < quest.GetObjectives().Count(); i++)
		{
			ExpansionQuestObjectiveConfigBase objective = quest.GetObjectives()[i];
			int objectiveType = objective.GetObjectiveType();
			QuestDebug(ToString() + "::SetQuest - Objective type: " + objectiveType);
			ExpansionQuestObjectiveTargetConfig objectiveTarget;
			ExpansionQuestObjectiveTravelConfig objectiveTravel;
			ExpansionQuestObjectiveDeliveryConfig objectiveDelivery;
			ExpansionQuestObjectiveCollectionConfig objectiveCollection;

			switch (objectiveType)
			{
				case ExpansionQuestObjectiveType.COLLECT:
				{
					objectiveCollection = ExpansionQuestObjectiveCollectionConfig.Cast(objective);
					string className = objectiveCollection.GetCollection().GetClassName();
					int amount = objectiveCollection.GetCollection().GetAmount();
					ExpansionQuestMenuItemEntry collectObjectiveEntry = new ExpansionQuestMenuItemEntry(className, amount);
					m_QuestMenuController.ObjectiveItems.Insert(collectObjectiveEntry);
					QuestDebug(ToString() + "::SetQuest - Add objective item entry for item: " + className);
					break;
				}
				case ExpansionQuestObjectiveType.DELIVERY:
				{
					objectiveDelivery = ExpansionQuestObjectiveDeliveryConfig.Cast(objective);
					for (int j = 0; j < objectiveDelivery.GetDeliveries().Count(); j++)
					{
						ExpansionQuestObjectiveDelivery delivery = objectiveDelivery.GetDeliveries()[j];
						className = delivery.GetClassName();
						amount = delivery.GetAmount();
						ExpansionQuestMenuItemEntry deliverObjectiveEntry = new ExpansionQuestMenuItemEntry(className, amount);
						m_QuestMenuController.ObjectiveItems.Insert(deliverObjectiveEntry);
						QuestDebug(ToString() + "::SetQuest - Add objective item entry for item: " + className);
					}
				}
			}
		}

		QuestDebug(ToString() + "::SetQuest - End");
	}

	void CloseMenu()
	{
		Hide();
		GetDayZGame().GetExpansionGame().GetExpansionUIManager().DestroySVMenu(this);
	}

	void OnCloseButtonClick()
	{
		CloseMenu();
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (w == Accept)
		{
			AcceptLable.SetColor(ARGB(255, 220, 220, 220));
		}
		else if (w == Complete)
		{
			CompleteLable.SetColor(ARGB(255, 220, 220, 220));
		}
		else if (w == Cancel)
		{
			CancelLable.SetColor(ARGB(255, 220, 220, 220));
		}
		else if (w == Close)
		{
			CloseLable.SetColor(ARGB(255, 220, 220, 220));
		}

		return super.OnMouseEnter(w, x, y);;
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (w == Accept)
		{
			AcceptLable.SetColor(ARGB(255, 0, 0, 0));
		}
		else if (w == Complete)
		{
			CompleteLable.SetColor(ARGB(255, 0, 0, 0));
		}
		else if (w == Cancel)
		{
			CancelLable.SetColor(ARGB(255, 0, 0, 0));
		}
		else if (w == Close)
		{
			CloseLable.SetColor(ARGB(255, 0, 0, 0));
		}

		return super.OnMouseLeave(w, enterW, x, y);
	}

	override void OnShow()
	{
		if (!m_QuestModule)
			m_QuestModule =  ExpansionQuestModule.Cast(CF_ModuleCoreManager.Get(ExpansionQuestModule));

		SetFocus(GetLayoutRoot());
	}

	void QuestDebug(string text)
	{
	#ifdef EXPANSIONMODQUESTSUIDEBUG
		Print(text);
	#endif
	}
};

class ExpansionQuestMenuController: ExpansionViewController
{
	string QuestTitle;
	string QuestDescription;
	string QuestObjective;
	string DefaultText;
	string HumanityVal;
	ref ObservableCollection<ref ExpansionQuestMenuListEntry> Quests = new ObservableCollection<ref ExpansionQuestMenuListEntry>(this);
	ref ObservableCollection<ref ExpansionQuestMenuItemEntry> RewardEntries = new ObservableCollection<ref ExpansionQuestMenuItemEntry>(this);
	ref ObservableCollection<ref ExpansionQuestMenuItemEntry> ObjectiveItems = new ObservableCollection<ref ExpansionQuestMenuItemEntry>(this);
};