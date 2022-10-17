/**
 * ExpansionBookMenuTabQuests.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONMODBOOK
class ExpansionBookMenuTabQuests: ExpansionBookMenuTabBase
{
	protected ref ExpansionBookMenuTabQuestsController m_QuestTabController;
	protected ScrollWidget rules_list_scroller;
	protected ExpansionQuestModule m_QuestModule;
	protected ExpansionQuestConfig m_Quest;
	protected ref ExpansionDialog_CancelQuest m_CancelQuestDialog;

	protected Widget quest_info_panel;
	protected Widget reward_panel;
	protected ButtonWidget cancel_quest;
	protected TextWidget cancel_quest_label;
	protected ButtonWidget hide_questhud_button;
	protected TextWidget hide_questhud_button_label;
	protected Widget humanity;

	void ExpansionBookMenuTabQuests(ExpansionBookMenu book_menu)
	{
		m_BookMenu = book_menu;

		if (!m_QuestTabController)
			m_QuestTabController = ExpansionBookMenuTabQuestsController.Cast(GetController());

		if (!m_QuestModule)
			m_QuestModule = ExpansionQuestModule.Cast(CF_ModuleCoreManager.Get(ExpansionQuestModule));
	}

	override string GetLayoutFile()
	{
		return "DayZExpansion/Quests/GUI/layouts/tabs/quests/expansion_book_tab_quests.layout";
	}

	override typename GetControllerType()
	{
		return ExpansionBookMenuTabQuestsController;
	}

	override string GetTabIconName()
	{
		return "Questionmark 2";
	}

	override string GetTabName()
	{
		return "Quests";
	}

	override int GetTabColor()
	{
		return ARGB(255,0,0,0);
	}

	override bool CanShow()
	{
		return (GetExpansionSettings().GetQuest().EnableQuestLogTab && GetExpansionSettings().GetQuest().EnableQuests);
	}

	override void OnShow()
	{
		super.OnShow();

		if (!m_QuestModule)
			m_QuestModule = ExpansionQuestModule.Cast(CF_ModuleCoreManager.Get(ExpansionQuestModule));

		m_QuestModule.GetQuestLogSI().Insert(SetView);
		m_QuestModule.GetQuestLogCallbackSI().Insert(MenuCallback);

		m_QuestModule.RequestPlayerQuests();
	}

	override void OnHide()
	{
		super.OnHide();

		if (!m_QuestModule)
			m_QuestModule = ExpansionQuestModule.Cast(CF_ModuleCoreManager.Get(ExpansionQuestModule));

		m_QuestModule.GetQuestLogSI().Clear();
	}

	override bool IsParentTab()
	{
		return true;
	}

	void MenuCallback()
	{
		QuestPrint("ExpansionBookMenuTabQuests::MenuCallback - Start");

		ExpansionUIManager uiManager = GetDayZExpansion().GetExpansionUIManager();
		ExpansionBookMenu bookMenu = ExpansionBookMenu.Cast(uiManager.GetMenu());
		if (bookMenu)
			GetDayZExpansion().GetExpansionUIManager().CloseMenu();

		QuestPrint("ExpansionBookMenuTabQuests::SetQuest - End");
	}

	void SetView(map<int, ref ExpansionQuestConfig> quests)
	{
		quest_info_panel.Show(false);

		if (!m_QuestModule)
			return;

		m_QuestTabController.Quests.Clear();

		foreach (int questID, ExpansionQuestConfig questConfig: quests)
		{
			if (!questConfig || questConfig.IsAchivement())
				continue;

			int questState = m_QuestModule.GetClientQuestData().GetQuestStateByQuestID(questID);
			if (questState == ExpansionQuestState.STARTED || questState == ExpansionQuestState.CAN_TURNIN)
			{
				ExpansionBookMenuTabQuestsListEntry questEntry = new ExpansionBookMenuTabQuestsListEntry(questConfig, this);
				m_QuestTabController.Quests.Insert(questEntry);
			}
		}

	#ifdef EXPANSIONMODQUESTS_HUD_ENABLE
		hide_questhud_button.Show(true);

		MissionGameplay mission = MissionGameplay.Cast(GetDayZGame().GetMission());
		if (mission)
		{
			if (!mission.QuestHudState())
			{
				m_QuestTabController.HideButtonText = "#STR_EXPANSION_QUEST_MENU_HIDE_HUD";
			}
			else
			{
				m_QuestTabController.HideButtonText = "#STR_EXPANSION_QUEST_MENU_SHOW_HUD";
			}

			m_QuestTabController.NotifyPropertyChanged("HideButtonText");
		}
	#endif
	}

	void SetQuest(ExpansionQuestConfig quest)
	{
		if (!m_QuestModule)
			return;

		m_Quest = quest;
		quest_info_panel.Show(true);

		int questState = m_QuestModule.GetClientQuestData().GetQuestStateByQuestID(quest.GetID());
		m_QuestTabController.QuestTitle = quest.GetTitle();

		string description;

		QuestPrint("ExpansionBookMenuTabQuests::SetQuest - questState: " + questState);

		if (questState == ExpansionQuestState.STARTED)
		{
			description = quest.GetDescriptions().Get(0);
		}
		else if (questState == ExpansionQuestState.CAN_TURNIN)
		{
			description = quest.GetDescriptions().Get(2);
		}

		QuestPrint("ExpansionBookMenuTabQuests::SetQuest - description: " + description);

		StringLocaliser descriptiontext = new StringLocaliser(description, GetGame().GetPlayer().GetIdentity().GetName());
		m_QuestTabController.QuestDescription = descriptiontext.Format();
		m_QuestTabController.QuestObjective = quest.GetObjectiveText();

		m_QuestTabController.NotifyPropertiesChanged({"QuestTitle", "QuestDescription", "QuestObjective"});

		m_QuestTabController.RewardEntries.Clear();
		int rewardsCount = quest.GetRewards().Count();

		reward_panel.Show(false);
	#ifdef EXPANSIONMODHARDLINE
		if (rewardsCount > 0 || (quest.GetHumanityReward() > 0 || quest.GetHumanityReward() < 0) && GetExpansionSettings().GetHardline().UseHumanity)
	#else
		if (rewardsCount > 0)
	#endif
		{
			reward_panel.Show(true);
			array<ref ExpansionQuestRewardConfig> questRewards = quest.GetRewards();
			foreach (ExpansionQuestRewardConfig reward: questRewards)
			{
				ExpansionQuestMenuItemEntry rewardEntry = new ExpansionQuestMenuItemEntry(reward.GetClassName(), reward.GetAmount(), reward.GetAttachments());
				m_QuestTabController.RewardEntries.Insert(rewardEntry);
			}

			humanity.Show(false);
		#ifdef EXPANSIONMODHARDLINE
			if ((quest.GetHumanityReward() > 0 || quest.GetHumanityReward() < 0) && GetExpansionSettings().GetHardline().UseHumanity)
			{
				humanity.Show(true);
				m_QuestTabController.HumanityVal = quest.GetHumanityReward().ToString();
				m_QuestTabController.NotifyPropertyChanged("HumanityVal");
			}
		#endif
		}

		m_QuestTabController.ObjectiveItems.Clear();
		array<ref ExpansionQuestObjectiveConfig> questObjectives = quest.GetObjectives();
		foreach (ExpansionQuestObjectiveConfig objective: questObjectives)
		{
			int objectiveType = objective.GetObjectiveType();
			string className;
			int amount;
			QuestPrint(ToString() + "::SetQuest - Objective type: " + objectiveType);
			switch (objectiveType)
			{
				case ExpansionQuestObjectiveType.COLLECT:
				{
					ExpansionQuestObjectiveCollectionConfig objectiveCollection = ExpansionQuestObjectiveCollectionConfig.Cast(objective);
					className = objectiveCollection.GetCollection().GetClassName();
					amount = objectiveCollection.GetCollection().GetAmount();
					ExpansionQuestMenuItemEntry collectObjectiveEntry = new ExpansionQuestMenuItemEntry(className, amount);
					m_QuestTabController.ObjectiveItems.Insert(collectObjectiveEntry);
					QuestPrint(ToString() + "::SetQuest - Add objective item entry for item: " + className);
					break;
				}
				case ExpansionQuestObjectiveType.DELIVERY:
				{
					ExpansionQuestObjectiveDeliveryConfig objectiveDelivery = ExpansionQuestObjectiveDeliveryConfig.Cast(objective);
					array<ref ExpansionQuestObjectiveDelivery> deliveries = objectiveDelivery.GetDeliveries();
					foreach (ExpansionQuestObjectiveDelivery delivery: deliveries)
					{
						className = delivery.GetClassName();
						amount = delivery.GetAmount();
						ExpansionQuestMenuItemEntry deliverObjectiveEntry = new ExpansionQuestMenuItemEntry(className, amount);
						m_QuestTabController.ObjectiveItems.Insert(deliverObjectiveEntry);
						QuestPrint(ToString() + "::SetQuest - Add objective item entry for item: " + className);
					}
					break;
				}
			}
		}
	}

	void OnCancelQuestButtonClick()
	{
		if (!m_CancelQuestDialog)
			m_CancelQuestDialog = new ExpansionDialog_CancelQuest(this);

		m_CancelQuestDialog.Show();
	}

	void OnConfirmCancelQuest()
	{
		if (!m_Quest)
			return;

		if (!m_QuestModule)
			m_QuestModule = ExpansionQuestModule.Cast(CF_ModuleCoreManager.Get(ExpansionQuestModule));

		m_QuestModule.CancelQuest(m_Quest.GetID());

		ExpansionUIManager uiManager = GetDayZExpansion().GetExpansionUIManager();
		ExpansionBookMenu bookMenu = ExpansionBookMenu.Cast(uiManager.GetMenu());
		if (bookMenu)
			GetDayZExpansion().GetExpansionUIManager().CloseMenu();
	}

#ifdef EXPANSIONMODQUESTS_HUD_ENABLE
	void OnHideHUDButtonClick()
	{
		QuestPrint("ExpansionBookMenuTabQuests::OnHideHUDButtonClick - Start");

		MissionGameplay mission = MissionGameplay.Cast(GetDayZGame().GetMission());
		if (mission)
		{
			mission.ToggleQuestHUD();
			if (!mission.QuestHudState())
			{
				m_QuestTabController.HideButtonText = "Hide HUD";
			}
			else
			{
				m_QuestTabController.HideButtonText = "Show HUD";
			}

			m_QuestTabController.NotifyPropertyChanged("HideButtonText");
		}

		QuestPrint("ExpansionBookMenuTabQuests::OnHideHUDButtonClick - End");
	}
#endif

	ExpansionQuestModule GetQuestModule()
	{
		return m_QuestModule;
	}

	ExpansionQuestConfig GetQuest()
	{
		return m_Quest;
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (w == cancel_quest)
		{
			cancel_quest_label.SetColor(ARGB(255, 220, 220, 220));
		}
	#ifdef EXPANSIONMODQUESTS_HUD_ENABLE
		else if (w == hide_questhud_button)
		{
			hide_questhud_button_label.SetColor(ARGB(255, 220, 220, 220));
		}
	#endif

		return super.OnMouseEnter(w, x, y);;
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (w == cancel_quest)
		{
			cancel_quest_label.SetColor(ARGB(255, 0, 0, 0));
		}
	#ifdef EXPANSIONMODQUESTS_HUD_ENABLE
		else if (w == hide_questhud_button)
		{
			hide_questhud_button_label.SetColor(ARGB(255, 0, 0 ,0));
		}
	#endif

		return super.OnMouseLeave(w, enterW, x, y);
	}

	override bool CanClose()
	{
		if (m_CancelQuestDialog)
			return !m_CancelQuestDialog.IsVisible();

		return true;
	}

	void QuestPrint(string text)
	{
	#ifdef EXPANSIONMODQUESTSUIDEBUG
		Print(text);
	#endif
	}
};

class ExpansionBookMenuTabQuestsController: ExpansionViewController
{
	string QuestTitle;
	string QuestDescription;
	string QuestObjective;
	string HideButtonText;
	string HumanityVal;
	ref ObservableCollection<ref ExpansionBookMenuTabQuestsListEntry> Quests = new ObservableCollection<ref ExpansionBookMenuTabQuestsListEntry>(this);
	ref ObservableCollection<ref ExpansionQuestMenuItemEntry> RewardEntries = new ObservableCollection<ref ExpansionQuestMenuItemEntry>(this);
	ref ObservableCollection<ref ExpansionQuestMenuItemEntry> ObjectiveItems = new ObservableCollection<ref ExpansionQuestMenuItemEntry>(this);
};

class ExpansionDialog_CancelQuest: ExpansionDialogBookBase
{
	ref ExpansionDialogContent_Text m_Text;
	ref ExpansionDialogButton_Text_CancelQuest_Accept m_AcceptButton;
	ref ExpansionDialogButton_Text_CancelQuest_Cancel m_CancelButton;
	ExpansionBookMenuTabQuests m_QuestsTab;

	void ExpansionDialog_CancelQuest(ExpansionScriptView parentView)
	{
		m_ParentView = parentView;

		if (!m_QuestsTab)
			m_QuestsTab = ExpansionBookMenuTabQuests.Cast(GetParentView());

		if (!m_Text)
		{
			m_Text = new ExpansionDialogContent_Text(this);
			AddContent(m_Text);
			m_Text.SetText("#STR_EXPANSION_QUEST_CONFIRM_CANCEL_TEXT");
			m_Text.SetTextColor(ARGB(255,0,0,0));
			m_Text.Show();
		}

		if (!m_AcceptButton)
		{
			m_AcceptButton = new ExpansionDialogButton_Text_CancelQuest_Accept(this);
			AddButton(m_AcceptButton);
			m_AcceptButton.Show();
		}

		if (!m_CancelButton)
		{
			m_CancelButton = new ExpansionDialogButton_Text_CancelQuest_Cancel(this);
			AddButton(m_CancelButton);
			m_CancelButton.Show();
		}
	}

	override string GetDialogTitle()
	{
		return "#STR_EXPANSION_QUEST_MENU_CANCEL_QUEST";
	}
};

class ExpansionDialogButton_Text_CancelQuest_Accept: ExpansionDialogBookButton_Text
{
	ExpansionDialog_CancelQuest m_CancelQuestDialog;
	ExpansionBookMenuTabQuests m_QuestTab;
	ExpansionQuestModule m_QuestModule;

	void ExpansionDialogButton_Text_CancelQuest_Accept(ExpansionDialogBase dialog)
	{
		if (!m_CancelQuestDialog)
			m_CancelQuestDialog = ExpansionDialog_CancelQuest.Cast(GetDialog());

		if (!m_QuestTab)
			m_QuestTab = ExpansionBookMenuTabQuests.Cast(m_CancelQuestDialog.GetParentView());

		if (!m_QuestModule)
			m_QuestModule = ExpansionQuestModule.Cast(CF_ModuleCoreManager.Get(ExpansionQuestModule));

		SetButtonText("#STR_EXPANSION_ACCEPT");
		SetTextColor(ARGB(255,0,0,0));
	}

	override void OnButtonClick()
	{
		if (m_QuestTab)
		{
			m_QuestTab.OnConfirmCancelQuest();
		}

		m_CancelQuestDialog.Hide();

		ExpansionUIManager uiManager = GetDayZExpansion().GetExpansionUIManager();
		ExpansionBookMenu bookMenu = ExpansionBookMenu.Cast(uiManager.GetMenu());
		if (bookMenu)
			GetDayZExpansion().GetExpansionUIManager().CloseMenu();
	}
};

class ExpansionDialogButton_Text_CancelQuest_Cancel: ExpansionDialogBookButton_Text
{
	ExpansionDialog_CancelQuest m_CancelQuestDialog;
	ExpansionBookMenuTabQuests m_QuestTab;

	void ExpansionDialogButton_Text_CancelQuest_Cancel(ExpansionDialogBase dialog)
	{
		if (!m_CancelQuestDialog)
			m_CancelQuestDialog = ExpansionDialog_CancelQuest.Cast(GetDialog());

		if (!m_QuestTab)
			m_QuestTab = ExpansionBookMenuTabQuests.Cast(m_CancelQuestDialog.GetParentView());

		SetButtonText("#STR_EXPANSION_CANCEL");
		SetTextColor(ARGB(255,0,0,0));
	}

	override void OnButtonClick()
	{
		m_CancelQuestDialog.Hide();
		m_QuestTab.Show();
	}
};
#endif
