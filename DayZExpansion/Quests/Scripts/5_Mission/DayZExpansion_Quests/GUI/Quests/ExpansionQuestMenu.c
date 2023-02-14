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
	protected autoptr ExpansionQuestMenuController m_QuestMenuController;
	protected autoptr array<ref ExpansionQuestConfig> m_Quests = new array<ref ExpansionQuestConfig>;
	protected autoptr ExpansionQuestConfig m_SelectedQuest;
	protected autoptr ExpansionQuestRewardConfig m_SelectedReward;
	protected bool m_InDetailView;
	protected autoptr ExpansionDialog_QuestMenu_CancelQuest m_CancelQuestDialog;

	protected Widget QuestListPanel;
	protected Widget QuestDetailsPanel;
	protected ButtonWidget Accept;
	protected TextWidget AcceptLable;
	protected Widget AcceptBackground;
	protected ButtonWidget Complete;
	protected TextWidget CompleteLable;
	protected Widget CompleteBackground;
	protected ButtonWidget Cancel;
	protected TextWidget CancelLable;
	protected Widget CancelBackground;
	protected ButtonWidget Close;
	protected TextWidget CloseLable;
	protected ButtonWidget CloseMenu;
	protected TextWidget CloseMenuCharacter;
	protected ImageWidget CloseMenuImage;
	protected ButtonWidget Back;
	protected Widget BackBackground;
	protected ImageWidget BackImage;
	protected Widget CloseBackground;
	protected WrapSpacerWidget ButtonsPanel;

	protected TextWidget Reward;
	protected Widget RewardPanel;
	protected ScrollWidget ObjectiveSectionScroller;
	protected RichTextWidget Objective;
	protected WrapSpacerWidget QuestListContent;
	protected Widget DefaultPanel;
	protected Widget Reputation;

	void ExpansionQuestMenu()
	{
		Class.CastTo(m_QuestMenuController, GetController());

		ExpansionQuestModule.GetModuleInstance().GetQuestMenuSI().Insert(SetQuests);
		QuestListPanel.Show(true);
		QuestDetailsPanel.Show(false);
	}

	void ~ExpansionQuestMenu()
	{
		ExpansionQuestModule.GetModuleInstance().GetQuestMenuSI().Remove(SetQuests);
		m_QuestMenuController.Quests.Clear();
		m_Quests.Clear();

		m_SelectedQuest = NULL;
		m_SelectedReward = NULL;
	}

	override string GetLayoutFile()
	{
		return "DayZExpansion/Quests/GUI/layouts/quests/expansion_quest_menu.layout";
	}

	override typename GetControllerType()
	{
		return ExpansionQuestMenuController;
	}

	void SetQuests(string npcName, string defaultText, int questNPCID)
	{
		QuestDebug(ToString() + "::SetQuests - Start");
		QuestDebug(ToString() + "::SetQuests - NPC name: " + npcName);
		QuestDebug(ToString() + "::SetQuests - NPC default text: " + defaultText);

		m_SelectedReward = NULL;
		ButtonsPanel.Show(false);

		m_QuestMenuController.QuestNPCName = npcName;
		m_QuestMenuController.NotifyPropertyChanged("QuestNPCName");

		QuestListContent.Show(true);
		DefaultPanel.Show(false);

		ExpansionQuestPersistentData playerQuestData = ExpansionQuestModule.GetModuleInstance().GetClientQuestData();
		if (!playerQuestData)
		{
			Error(ToString() + "::SetQuests - Could not get players persistent quest data!");
			CloseMenu();
			return;
		}

		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());
		if (!player)
		{
			Error(ToString() + "::SetQuests - Could not player!");
			CloseMenu();
			return;
		}

		array<ref ExpansionQuestConfig> questConfigs = ExpansionQuestModule.GetModuleInstance().GetQuestConfigsClient();
		if (!questConfigs || questConfigs.Count() == 0)
		{
			Error(ToString() + "::SetQuests - Could not get any quest configurations!");
			CloseMenu();
			return;
		}
		//! Check quest configurations array for valid quests to display.
		foreach (ExpansionQuestConfig questConfig: questConfigs)
		{
			QuestDebug(ToString() + "::SetQuests - Checking if quest " + questConfig.GetID() + " should be displayed..");
			if (!QuestDisplayConditions(playerQuestData, questConfig, player, questNPCID))
			{
				QuestDebug(ToString() + "::SetQuests - Don't show quest " + questConfig.GetID() + ". Skip..");
				continue;
			}

			QuestDebug(ToString() + "::SetQuests - Show quest " + questConfig.GetID() + ". Add to menu quest.");
			m_Quests.Insert(questConfig);
			ExpansionQuestMenuListEntry questEntry = new ExpansionQuestMenuListEntry(questConfig, this);
			m_QuestMenuController.Quests.Insert(questEntry);
		}

		//! Display default NPC text if there are no quests to display.
		if (m_Quests.Count() == 0)
		{
			QuestListContent.Show(false);
			DefaultPanel.Show(true);

			m_QuestMenuController.DefaultText = defaultText;
			m_QuestMenuController.NotifyPropertyChanged("DefaultText");
		}

		QuestDebug(ToString() + "::SetQuests - End");
	}

	protected bool QuestDisplayConditions(ExpansionQuestPersistentData playerQuestData, ExpansionQuestConfig config, PlayerBase player, int questNPCID)
	{
		QuestDebug(ToString() + "::QuestDisplayConditions - Start");

		string playerUID = player.GetIdentity().GetId();

		//! Check if quest is completed if not repeatable.
		int questState = playerQuestData.GetQuestStateByQuestID(config.GetID());
		if (!config.IsRepeatable() && questState == ExpansionQuestState.COMPLETED)
		{
			QuestDebug(ToString() + "::QuestDisplayConditions - Return FALSE. Quest is already completed!");
			return false;
		}

		//! Check if this quest is a quest of the NPC the client is interacting with.
		array<int> questGiverIDs = config.GetQuestGiverIDs();
		if (questState != ExpansionQuestState.CAN_TURNIN && (!questGiverIDs || questGiverIDs.Count() == 0 || questGiverIDs.Find(questNPCID) == -1))
		{
			QuestDebug(ToString() + "::QuestDisplayConditions - Return FALSE. NPC is not quest giver NPC!");
			return false;
		}

		array<int> questTurnInIDs = config.GetQuestTurnInIDs();
		if (questState == ExpansionQuestState.CAN_TURNIN && (!questTurnInIDs || questTurnInIDs.Count() == 0 || questTurnInIDs.Find(questNPCID) == -1))
		{
			QuestDebug(ToString() + "::QuestDisplayConditions - Return FALSE. NPC is not quest turn-in NPC!");
			return false;
		}

		//! Check if client has a cooldown on this quest if quest is repeatable.
		int timestamp;
		bool hasCooldown = playerQuestData.HasCooldownOnQuest(config.GetID(), timestamp);
		if (config.IsRepeatable() && hasCooldown)
		{
			QuestDebug(ToString() + "::QuestDisplayConditions - Return FALSE. Quest is on cooldown!");
			return false;
		}

		//! Check if quest is a achivement quest.
		if (config.IsAchivement())
		{
			QuestDebug(ToString() + "::QuestDisplayConditions - Return FALSE. Quest is achiement quest!");
			return false;
		}

		//! Check if all pre-quests are completed.
		bool completedPreQuest = true;
		if (config.GetPreQuestIDs().Count() > 0)
		{
			array<int> preQuestIDs = config.GetPreQuestIDs();
			foreach (int preQuest: preQuestIDs)
			{
				if (!ExpansionQuestModule.GetModuleInstance().HasCompletedQuest(preQuest, playerUID))
				{
					completedPreQuest = false;
					break;
				}
			}
		}

		if (!completedPreQuest)
		{
			QuestDebug(ToString() + "::QuestDisplayConditions - Return FALSE. Not all pre-quests completed!");
			return false;
		}

	#ifdef EXPANSIONMODHARDLINE
		//! Check if for reputation requirement if config has a value but system is disabled.
		if (config.GetReputationRequirement() > 0 && !GetExpansionSettings().GetHardline().UseReputation)
		{
			QuestDebug(ToString() + "::QuestDisplayConditions - Return FALSE. Reputation system disabled!");
			return false;
		}

		//! Check if client meats the reputation requirement.
		if (GetExpansionSettings().GetHardline().UseReputation && config.GetReputationRequirement() > 0)
		{
			int reputation = player.Expansion_GetReputation();
			if (reputation < config.GetReputationRequirement());
			{
				QuestDebug(ToString() + "::QuestDisplayConditions - Return FALSE. Reputation requirements not met!");
				return false;
			}
		}
	#endif

		QuestDebug(ToString() + "::QuestDisplayConditions - End and return TRUE.");

		return true;
	}

	void OnAcceptButtonClick()
	{
		if (!m_SelectedQuest)
			return;

		ExpansionQuestModule.GetModuleInstance().RequestCreateQuestInstance(m_SelectedQuest.GetID());
		CloseMenu();
	}

	void OnCancelButtonClick()
	{
		if (!m_CancelQuestDialog)
			m_CancelQuestDialog = new ExpansionDialog_QuestMenu_CancelQuest(this);

		m_CancelQuestDialog.Show();
	}

	void OnCompleteButtonClick()
	{
		if (!m_SelectedQuest)
			return;

		if (m_SelectedQuest.NeedToSelectReward())
		{
			if (m_SelectedReward)
			{
				ExpansionQuestModule.GetModuleInstance().RequestTurnInQuestClient(m_SelectedQuest.GetID(), true, m_SelectedReward);
			}
			else
			{
				StringLocaliser title = new StringLocaliser("STR_EXPANSION_QUEST_TITLE", m_SelectedQuest.GetTitle());
				StringLocaliser text = new StringLocaliser("STR_EXPANSION_QUEST_MENU_ERROR_REWARD");
				ExpansionNotification(title, text, EXPANSION_NOTIFICATION_ICON_INFO, COLOR_EXPANSION_NOTIFICATION_ERROR).Create();
			}
		}
		else
		{
			ExpansionQuestModule.GetModuleInstance().RequestTurnInQuestClient(m_SelectedQuest.GetID());
		}

		ButtonsPanel.Show(false);
		CloseMenu();
	}

	void SetQuest(ExpansionQuestConfig quest)
	{
		QuestDebug(ToString() + "::SetQuest - Start");

		int questState = ExpansionQuestModule.GetModuleInstance().GetClientQuestData().GetQuestStateByQuestID(quest.GetID());

		QuestDebug(ToString() + "::SetQuest - Quest state: " + questState);
		QuestListPanel.Show(false);
		ButtonsPanel.Show(true);
		QuestDetailsPanel.Show(true);

		m_SelectedQuest = quest;
		m_QuestMenuController.QuestTitle = quest.GetTitle();

		m_SelectedQuest.QuestDebug();

		string description;
		if (questState == ExpansionQuestState.STARTED)
		{
			description = quest.GetDescriptions()[1];
			Accept.Show(false);
			Complete.Show(false);
			Cancel.Show(true);
		}
		else if (questState == ExpansionQuestState.CAN_TURNIN)
		{
			description = quest.GetDescriptions()[2];
			Accept.Show(false);
			Complete.Show(true);
			Cancel.Show(true);
		}
		else
		{
			description = quest.GetDescriptions()[0];
			Accept.Show(true);
			Complete.Show(false);
			Cancel.Show(false);
		}

		StringLocaliser descriptiontext = new StringLocaliser(description, GetGame().GetPlayer().GetIdentity().GetName());
		m_QuestMenuController.QuestDescription = descriptiontext.Format();
		m_QuestMenuController.QuestObjective = quest.GetObjectiveText();

		m_QuestMenuController.NotifyPropertiesChanged({"QuestTitle", "QuestDescription", "QuestObjective"});

		m_QuestMenuController.RewardEntries.Clear();
		int rewardsCount = quest.GetRewards().Count();
		int i;

		RewardPanel.Show(false);
	#ifdef EXPANSIONMODHARDLINE
		if (rewardsCount > 0 || quest.GetReputationReward() > 0 && GetExpansionSettings().GetHardline().UseReputation)
	#else
		if (rewardsCount > 0)
	#endif
		{
			RewardPanel.Show(true);
			if (quest.NeedToSelectReward() && quest.GetRewards().Count() > 1)
			{
				Reward.SetText("#STR_EXPANSION_QUEST_MENU_REWARD_LABEL");
			}
			else
			{
				Reward.SetText("#STR_EXPANSION_QUEST_MENU_REWARDS_LABEL");
			}

			for (i = 0; i < quest.GetRewards().Count(); i++)
			{
				ExpansionQuestRewardConfig reward = quest.GetRewards()[i];
				ExpansionQuestMenuItemEntry rewardEntry = new ExpansionQuestMenuItemEntry(reward.GetClassName(), reward.GetAmount(), reward.GetAttachments());
				rewardEntry.SetQuestRewardConfig(reward);
				rewardEntry.SetQuestMenu(this);
				rewardEntry.SetIsRewardEntry(true);
				m_QuestMenuController.RewardEntries.Insert(rewardEntry);
			}

			Reputation.Show(false);
		#ifdef EXPANSIONMODHARDLINE
			if (quest.GetReputationReward() > 0 && GetExpansionSettings().GetHardline().UseReputation)
			{
				Reputation.Show(true);
				m_QuestMenuController.ReputationVal = quest.GetReputationReward().ToString();
				m_QuestMenuController.NotifyPropertyChanged("ReputationVal");
			}
		#endif
		}

		m_QuestMenuController.ObjectiveItems.Clear();

		for (i = 0; i < quest.GetObjectives().Count(); i++)
		{
			ExpansionQuestObjectiveConfig objective = quest.GetObjectives()[i];
			int objectiveType = objective.GetObjectiveType();
			QuestDebug(ToString() + "::SetQuest - Objective type: " + objectiveType);
			switch (objectiveType)
			{
				case ExpansionQuestObjectiveType.COLLECT:
				{
					ExpansionQuestObjectiveCollectionConfig collectionDelivery = ExpansionQuestObjectiveCollectionConfig.Cast(objective);
					for (int j = 0; j < collectionDelivery.GetDeliveries().Count(); j++)
					{
						ExpansionQuestObjectiveDelivery collection = collectionDelivery.GetDeliveries()[j];
						string collectionClassName = collection.GetClassName();
						int collectionAmount = collection.GetAmount();
						ExpansionQuestMenuItemEntry collectionObjectiveEntry = new ExpansionQuestMenuItemEntry(collectionClassName, collectionAmount);
						m_QuestMenuController.ObjectiveItems.Insert(collectionObjectiveEntry);
						QuestDebug(ToString() + "::SetQuest - Add objective item entry for item: " + collectionClassName);
					}
				}
				break;
				case ExpansionQuestObjectiveType.DELIVERY:
				{
					ExpansionQuestObjectiveDeliveryConfig objectiveDelivery = ExpansionQuestObjectiveDeliveryConfig.Cast(objective);
					for (int k = 0; k < objectiveDelivery.GetDeliveries().Count(); k++)
					{
						ExpansionQuestObjectiveDelivery delivery = objectiveDelivery.GetDeliveries()[k];
						string deliveryClassName = delivery.GetClassName();
						int deliveryAmount = delivery.GetAmount();
						ExpansionQuestMenuItemEntry deliverObjectiveEntry = new ExpansionQuestMenuItemEntry(deliveryClassName, deliveryAmount);
						m_QuestMenuController.ObjectiveItems.Insert(deliverObjectiveEntry);
						QuestDebug(ToString() + "::SetQuest - Add objective item entry for item: " + deliveryClassName);
					}
				}
				break;
			}
		}

		m_InDetailView = true;

		QuestDebug(ToString() + "::SetQuest - End");
	}

	void SetSelectedReward(ExpansionQuestRewardConfig reward)
	{
		m_SelectedReward = reward;
	}

	void ResetRewardElements()
	{
		for (int i = 0; i < m_QuestMenuController.RewardEntries.Count(); i++)
		{
			ExpansionQuestMenuItemEntry entry = m_QuestMenuController.RewardEntries[i];
			entry.Reset();
		}
	}

	void OnCloseButtonClick()
	{
		if (!m_InDetailView)
		{
			CloseMenu();
		}
		else
		{
			QuestListPanel.Show(true);
			ButtonsPanel.Show(false);
			QuestDetailsPanel.Show(false);
			m_InDetailView = false;
		}
	}

	void OnConfirmCancelQuest()
	{
		if (!m_SelectedQuest)
			return;

		ExpansionQuestModule.GetModuleInstance().CancelQuest(m_SelectedQuest.GetID());
		OnCloseButtonClick();
	}

	ExpansionQuestConfig GetSelectedQuest()
	{
		return m_SelectedQuest;
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		if (w == Accept)
		{
			AcceptBackground.SetColor(ARGB(200, 220, 220, 220));
			AcceptLable.SetColor(ARGB(255, 0, 0, 0));
		}
		else if (w == Complete)
		{
			CompleteBackground.SetColor(ARGB(200, 220, 220, 220));
			CompleteLable.SetColor(ARGB(255, 0, 0, 0));
		}
		else if (w == Cancel)
		{
			CancelBackground.SetColor(ARGB(200, 220, 220, 220));
			CancelLable.SetColor(ARGB(255, 0, 0, 0));
		}
		else if (w == Close)
		{
			CloseBackground.SetColor(ARGB(200, 220, 220, 220));
			CloseLable.SetColor(ARGB(255, 0, 0, 0));
		}
		else if (w == CloseMenu)
		{
			CloseMenuCharacter.SetColor(ARGB(255, 0, 0, 0));
			CloseMenuImage.SetColor(ARGB(255, 0, 0, 0));
		}
		else if (w == Back)
		{
			BackBackground.SetColor(ARGB(200, 220, 220, 220));
			BackImage.SetColor(ARGB(255, 0, 0, 0));
		}

		return super.OnMouseEnter(w, x, y);;
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (w == Accept)
		{
			AcceptBackground.SetColor(ARGB(200, 0, 0, 0));
			AcceptLable.SetColor(ARGB(255, 220, 220, 220));
		}
		else if (w == Complete)
		{
			CompleteBackground.SetColor(ARGB(200, 0, 0, 0));
			CompleteLable.SetColor(ARGB(255, 220, 220, 220));
		}
		else if (w == Cancel)
		{
			CancelBackground.SetColor(ARGB(200, 0, 0, 0));
			CancelLable.SetColor(ARGB(255, 220, 220, 220));
		}
		else if (w == Close)
		{
			CloseBackground.SetColor(ARGB(200, 0, 0, 0));
			CloseLable.SetColor(ARGB(255, 220, 220, 220));
		}
		else if (w == CloseMenu)
		{
			CloseMenuCharacter.SetColor(ARGB(255, 220, 220, 220));
			CloseMenuImage.SetColor(ARGB(255, 220, 220, 220));
		}
		else if (w == Back)
		{
			BackBackground.SetColor(ARGB(200, 0, 0, 0));
			BackImage.SetColor(ARGB(255, 220, 220, 220));
		}

		return super.OnMouseLeave(w, enterW, x, y);
	}

	override void OnShow()
	{
		SetFocus(GetLayoutRoot());
	}

	override bool CanClose()
	{
		if (m_CancelQuestDialog)
			return !m_CancelQuestDialog.IsVisible();

		return true;
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
	string ReputationVal;
	string QuestNPCName;
	ref ObservableCollection<ref ExpansionQuestMenuListEntry> Quests = new ObservableCollection<ref ExpansionQuestMenuListEntry>(this);
	ref ObservableCollection<ref ExpansionQuestMenuItemEntry> RewardEntries = new ObservableCollection<ref ExpansionQuestMenuItemEntry>(this);
	ref ObservableCollection<ref ExpansionQuestMenuItemEntry> ObjectiveItems = new ObservableCollection<ref ExpansionQuestMenuItemEntry>(this);
};

class ExpansionDialog_QuestMenu_CancelQuest: ExpansionDialogBase
{
	ref ExpansionDialogContent_Text m_Text;
	ref ExpansionDialogButton_QuestMenu_CancelQuest_Accept m_AcceptButton;
	ref ExpansionDialogButton_QuestMenu_CancelQuest_Cancel m_CancelButton;
	ExpansionQuestMenu m_QuestMenu;

	void ExpansionDialog_QuestMenu_CancelQuest(ScriptView parentView)
	{
		m_ParentView = parentView;

		if (!m_QuestMenu)
			m_QuestMenu = ExpansionQuestMenu.Cast(m_ParentView);

		if (!m_Text)
		{
			m_Text = new ExpansionDialogContent_Text(this);
			AddContent(m_Text);
			m_Text.SetText("#STR_EXPANSION_QUEST_CONFIRM_CANCEL_TEXT");
			m_Text.SetTextColor(ARGB(255, 220, 220, 220));
			m_Text.Show();
		}

		if (!m_AcceptButton)
		{
			m_AcceptButton = new ExpansionDialogButton_QuestMenu_CancelQuest_Accept(this);
			AddButton(m_AcceptButton);
			m_AcceptButton.Show();
		}

		if (!m_CancelButton)
		{
			m_CancelButton = new ExpansionDialogButton_QuestMenu_CancelQuest_Cancel(this);
			AddButton(m_CancelButton);
			m_CancelButton.Show();
		}
	}

	override string GetDialogTitle()
	{
		return "#STR_EXPANSION_QUEST_MENU_CANCEL_QUEST";
	}
};

class ExpansionDialogButton_QuestMenu_CancelQuest_Accept: ExpansionDialogButton_Text
{
	ExpansionDialog_QuestMenu_CancelQuest m_CancelQuestDialog;
	ExpansionQuestMenu m_QuestMenu;

	void ExpansionDialogButton_QuestMenu_CancelQuest_Accept(ExpansionDialogBase dialog)
	{
		if (!m_CancelQuestDialog)
			m_CancelQuestDialog = ExpansionDialog_QuestMenu_CancelQuest.Cast(GetDialog());

		if (!m_QuestMenu)
			m_QuestMenu = ExpansionQuestMenu.Cast(m_CancelQuestDialog.GetParentView());

		SetButtonText("#STR_EXPANSION_ACCEPT");
		SetTextColor(ARGB(255, 220, 220, 220));
	}

	override void OnButtonClick()
	{
		if (m_QuestMenu)
		{
			m_QuestMenu.OnConfirmCancelQuest();
		}

		m_CancelQuestDialog.Hide();
	}
};

class ExpansionDialogButton_QuestMenu_CancelQuest_Cancel: ExpansionDialogButton_Text
{
	ExpansionDialog_QuestMenu_CancelQuest m_CancelQuestDialog;
	ExpansionQuestMenu m_QuestMenu;

	void ExpansionDialogButton_QuestMenu_CancelQuest_Cancel(ExpansionDialogBase dialog)
	{
		if (!m_CancelQuestDialog)
			m_CancelQuestDialog = ExpansionDialog_QuestMenu_CancelQuest.Cast(GetDialog());

		if (!m_QuestMenu)
			m_QuestMenu = ExpansionQuestMenu.Cast(m_CancelQuestDialog.GetParentView());

		SetButtonText("#STR_EXPANSION_CANCEL");
		SetTextColor(ARGB(255, 220, 220, 220));
	}

	override void OnButtonClick()
	{
		m_CancelQuestDialog.Hide();
	}
};
