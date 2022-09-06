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
	private autoptr ExpansionQuestMenuController m_QuestMenuController;
	private ExpansionQuestModule m_QuestModule;
	private autoptr array<ref ExpansionQuestConfig> m_Quests = new array<ref ExpansionQuestConfig>;
	private autoptr ExpansionQuestConfig m_SelectedQuest;
	private autoptr ExpansionQuestRewardConfig m_SelectedReward;
	private bool m_InDetailView;
	private autoptr ExpansionDialog_QuestMenu_CancelQuest m_CancelQuestDialog;
	
	private Widget QuestListPanel;
	private Widget QuestDetailsPanel;
	private ButtonWidget Accept;
	private TextWidget AcceptLable;
	private Widget AcceptBackground;
	private ButtonWidget Complete;
	private TextWidget CompleteLable;
	private Widget CompleteBackground;
	private ButtonWidget Cancel;
	private TextWidget CancelLable;
	private Widget CancelBackground;
	private ButtonWidget Close;
	private TextWidget CloseLable;
	private ButtonWidget CloseMenu;
	private TextWidget CloseMenuCharacter;
	private ImageWidget CloseMenuImage;
	private ButtonWidget Back;
	private Widget BackBackground;
	private ImageWidget BackImage;
	private Widget CloseBackground;
	private WrapSpacerWidget ButtonsPanel;
	
	private TextWidget Reward;
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

	void SetQuests(array<ref ExpansionQuestConfig> quests, string npcName, string defaultText)
	{
		QuestDebug(ToString() + "::SetQuests - Start");
		QuestDebug(ToString() + "::SetQuests - Quest: " + quests.ToString());
		QuestDebug(ToString() + "::SetQuests - NPC name: " + npcName);
		QuestDebug(ToString() + "::SetQuests - NPC default text: " + defaultText);

		if (!m_QuestModule)
			return;

		m_SelectedReward = NULL;
		ButtonsPanel.Show(false);
		
		m_QuestMenuController.QuestNPCName = npcName;
		m_QuestMenuController.NotifyPropertyChanged("QuestNPCName");

		if (quests.Count() > 0)
		{
			QuestListContent.Show(true);
			DefaultPanel.Show(false);

			for (int i = 0; i < quests.Count(); i++)
			{
				ExpansionQuestConfig quest = quests[i];
				if (!quest || quest.IsAchivement())
					continue;
				
			#ifdef EXPANSIONMODHARDLINE
				if (quest.IsBanditQuest() || quest.IsHeroQuest() && !GetExpansionSettings().GetHardline().UseHumanity)
					continue;
			#endif

				m_Quests.Insert(quest);
				ExpansionQuestMenuListEntry questEntry = new ExpansionQuestMenuListEntry(quest, this);
				m_QuestMenuController.Quests.Insert(questEntry);
			}
		}
		else if (quests.Count() == 0)
		{
			QuestListContent.Show(false);
			DefaultPanel.Show(true);

			m_QuestMenuController.DefaultText = defaultText;
			m_QuestMenuController.NotifyPropertyChanged("DefaultText");
		}

		QuestDebug(ToString() + "::SetQuests - End");
	}

	void OnAcceptButtonClick()
	{
		if (!m_SelectedQuest)
			return;

		m_QuestModule.CreateQuestInstance(m_SelectedQuest.GetID());
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
				m_QuestModule.RequestTurnInQuestClient(m_SelectedQuest.GetID(), true, m_SelectedReward);
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
			m_QuestModule.RequestTurnInQuestClient(m_SelectedQuest.GetID());
		}

		ButtonsPanel.Show(false);
		CloseMenu();
	}

	void SetQuest(ExpansionQuestConfig quest)
	{
		QuestDebug(ToString() + "::SetQuest - Start");

		if (!m_QuestModule)
			return;
		
		int questState = m_QuestModule.GetClientQuestData().GetQuestStateByQuestID(quest.GetID());

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
		if (rewardsCount > 0 || quest.GetHumanityReward() > 0 && GetExpansionSettings().GetHardline().UseHumanity)
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
				ExpansionQuestMenuItemEntry rewardEntry = new ExpansionQuestMenuItemEntry(reward.GetClassName(), reward.GetAmount());
				rewardEntry.SetQuestRewardConfig(reward);
				rewardEntry.SetQuestMenu(this);
				rewardEntry.SetIsRewardEntry(true);
				m_QuestMenuController.RewardEntries.Insert(rewardEntry);
			}
			
			Humanity.Show(false);
		#ifdef EXPANSIONMODHARDLINE
			if (quest.GetHumanityReward() > 0 && GetExpansionSettings().GetHardline().UseHumanity)
			{
				Humanity.Show(true);
				m_QuestMenuController.HumanityVal = quest.GetHumanityReward().ToString();
				m_QuestMenuController.NotifyPropertyChanged("HumanityVal");
			}
		#endif
		}

		m_QuestMenuController.ObjectiveItems.Clear();

		for (i = 0; i < quest.GetObjectives().Count(); i++)
		{
			ExpansionQuestObjectiveConfig objective = quest.GetObjectives()[i];
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
	
	void CloseMenu()
	{
		Hide();
		GetDayZGame().GetExpansionGame().GetExpansionUIManager().CloseMenu();
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

		if (!m_QuestModule)
			m_QuestModule = ExpansionQuestModule.Cast(CF_ModuleCoreManager.Get(ExpansionQuestModule));

		m_QuestModule.CancelQuest(m_SelectedQuest.GetID());
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
		if (!m_QuestModule)
			m_QuestModule =  ExpansionQuestModule.Cast(CF_ModuleCoreManager.Get(ExpansionQuestModule));

		SetFocus(GetLayoutRoot());
	}
	
	override bool CanClose()
	{
		return !m_CancelQuestDialog || !m_CancelQuestDialog.IsVisible();
	}

	void QuestDebug(string text)
	{
	//#ifdef EXPANSIONMODQUESTSUIDEBUG
		EXTrace.Print(EXTrace.QUESTS, null, text);
	//#endif
	}
};

class ExpansionQuestMenuController: ExpansionViewController
{
	string QuestTitle;
	string QuestDescription;
	string QuestObjective;
	string DefaultText;
	string HumanityVal;
	string QuestNPCName;
	ref ObservableCollection<ref ExpansionQuestMenuListEntry> Quests = new ObservableCollection<ref ExpansionQuestMenuListEntry>(this);
	ref ObservableCollection<ref ExpansionQuestMenuItemEntry> RewardEntries = new ObservableCollection<ref ExpansionQuestMenuItemEntry>(this);
	ref ObservableCollection<ref ExpansionQuestMenuItemEntry> ObjectiveItems = new ObservableCollection<ref ExpansionQuestMenuItemEntry>(this);
};

class ExpansionDialog_QuestMenu_CancelQuest: ExpansionMenuDialogBase
{
	ref ExpansionMenuDialogContent_Text m_Text;
	ref ExpansionDialogButton_QuestMenu_CancelQuest_Accept m_AcceptButton;
	ref ExpansionDialogButton_QuestMenu_CancelQuest_Cancel m_CancelButton;	
	ExpansionQuestMenu m_QuestMenu;
		
	void ExpansionDialog_QuestMenu_CancelQuest(ExpansionScriptViewMenu parentMenu)
	{
		m_ParentMenu = parentMenu;
		
		if (!m_QuestMenu)
			m_QuestMenu = ExpansionQuestMenu.Cast(m_ParentMenu);
		
		if (!m_Text)
		{
			m_Text = new ExpansionMenuDialogContent_Text(this);
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

class ExpansionDialogButton_QuestMenu_CancelQuest_Accept: ExpansionMenuDialogButton_Text
{
	ExpansionDialog_QuestMenu_CancelQuest m_CancelQuestDialog;
	ExpansionQuestMenu m_QuestMenu;
	ExpansionQuestModule m_QuestModule;
	
	void ExpansionDialogButton_QuestMenu_CancelQuest_Accept(ExpansionMenuDialogBase dialog)
	{
		if (!m_CancelQuestDialog)
			m_CancelQuestDialog = ExpansionDialog_QuestMenu_CancelQuest.Cast(GetDialog());
		
		if (!m_QuestMenu)
			m_QuestMenu = ExpansionQuestMenu.Cast(m_CancelQuestDialog.GetParentMenu());
		
		if (!m_QuestModule)
			m_QuestModule = ExpansionQuestModule.Cast(CF_ModuleCoreManager.Get(ExpansionQuestModule));
		
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

class ExpansionDialogButton_QuestMenu_CancelQuest_Cancel: ExpansionMenuDialogButton_Text
{
	ExpansionDialog_QuestMenu_CancelQuest m_CancelQuestDialog;
	ExpansionQuestMenu m_QuestMenu;
	
	void ExpansionDialogButton_QuestMenu_CancelQuest_Cancel(ExpansionMenuDialogBase dialog)
	{
		if (!m_CancelQuestDialog)
			m_CancelQuestDialog = ExpansionDialog_QuestMenu_CancelQuest.Cast(GetDialog());
		
		if (!m_QuestMenu)
			m_QuestMenu = ExpansionQuestMenu.Cast(m_CancelQuestDialog.GetParentMenu());
		
		SetButtonText("#STR_EXPANSION_CANCEL");
		SetTextColor(ARGB(255, 220, 220, 220));
	}
	
	override void OnButtonClick()
	{
		m_CancelQuestDialog.Hide();
	}
};