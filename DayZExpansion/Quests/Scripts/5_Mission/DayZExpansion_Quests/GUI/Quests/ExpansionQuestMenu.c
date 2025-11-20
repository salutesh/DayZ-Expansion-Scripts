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
	
	protected autoptr ExpansionQuestPersistentData m_PlayerData;
	protected autoptr ExpansionQuestPersistentQuestData m_CurrentPlayerQuestData;
	
	protected int m_SelectedObjectiveItemIndex = -1;
	protected bool m_NeedToSelectObjItem;
	protected bool m_InDetailView;
	protected autoptr ExpansionDialog_QuestMenu_CancelQuest m_CancelQuestDialog;
	protected autoptr ExpansionQuestPersistentData m_ClientQuestData;
	protected ref map<int, ref ExpansionQuestConfig> m_ClientQuestConfigs;
	
	protected string m_CurrentNPCName;
	protected string m_CurrentNPCText;
	protected int m_CurrentNPCID = -1;
	protected int m_CurrentQuestID = -1;
	
	protected bool m_QuestLogMode;

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

	protected WrapSpacerWidget QuestItemsPanel;
	protected WrapSpacerWidget RewardSelection;
	protected TextWidget Reward;
	protected Widget RewardPanel;
	protected WrapSpacerWidget ObjectivePanel;
	protected WrapSpacerWidget QuestListContent;
	protected Widget DefaultPanel;
	protected Widget Reputation;
	
	protected WrapSpacerWidget MenuButtonsPanel;	
	protected ButtonWidget HideHud;
	protected TextWidget HideHudLable;
	protected Widget HideHudBackground;
	
	protected WrapSpacerWidget FactionReputation;

	protected ButtonWidget Share;
	protected TextWidget ShareLable;
	protected Widget ShareBackground;

	void ExpansionQuestMenu()
	{
		Class.CastTo(m_QuestMenuController, GetController());

		ExpansionQuestModule.GetModuleInstance().GetQuestMenuSI().Insert(SetQuests);
		ExpansionQuestModule.GetModuleInstance().GetQuestMenuCallbackSI().Insert(MenuCallback);
		QuestListPanel.Show(true);
		QuestDetailsPanel.Show(false);
	}

	void ~ExpansionQuestMenu()
	{
		ExpansionQuestModule.GetModuleInstance().GetQuestMenuSI().Remove(SetQuests);
		ExpansionQuestModule.GetModuleInstance().GetQuestMenuCallbackSI().Remove(MenuCallback);
		
		m_QuestMenuController.Quests.Clear();
		m_QuestMenuController.QuestLogs.Clear();
		m_QuestMenuController.RewardEntries.Clear();
		m_QuestMenuController.ObjectiveItems.Clear();
		m_QuestMenuController.QuestItemEntries.Clear();
		m_Quests.Clear();

		m_QuestLogMode = false;
		
		m_SelectedQuest = null;
		m_SelectedReward = null;
		
		m_ClientQuestData = null;
		m_ClientQuestConfigs = null;
		
		m_CurrentNPCName = "";
		m_CurrentNPCText = "";
		m_CurrentNPCID = -1;
		m_CurrentQuestID = -1;
	}

	override string GetLayoutFile()
	{
		return "DayZExpansion/Quests/GUI/layouts/quests/expansion_quest_menu.layout";
	}

	override typename GetControllerType()
	{
		return ExpansionQuestMenuController;
	}
	
	void MenuCallback()
	{
		m_ClientQuestData = ExpansionQuestModule.GetModuleInstance().GetClientQuestData();
		m_ClientQuestConfigs = ExpansionQuestModule.GetModuleInstance().GetQuestConfigs();
		
		m_QuestMenuController.Quests.Clear();
		
		if (!m_QuestLogMode)
		{
			SetQuests(m_CurrentNPCName, m_CurrentNPCText, m_CurrentNPCID, m_CurrentQuestID);
		}
		else
		{
			SetQuestLogView();
		}
	}
	
	void SetQuestLogView()
	{
		QuestDebug(ToString() + "::SetQuestLogView - Start");
		
		m_QuestMenuController.QuestLogs.Clear();
		
		ButtonsPanel.Show(false);
		HideHud.Show(true);
		MenuButtonsPanel.Show(true);

		MissionGameplay mission = MissionGameplay.Cast(GetDayZGame().GetMission());
		if (mission)
		{
			if (!mission.QuestHudState())
			{
				m_QuestMenuController.HideHudLable = "#STR_EXPANSION_QUEST_MENU_HIDE_HUD";
			}
			else
			{
				m_QuestMenuController.HideHudLable = "#STR_EXPANSION_QUEST_MENU_SHOW_HUD";
			}

			m_QuestMenuController.NotifyPropertyChanged("HideHudLable");
		}
		
		m_QuestMenuController.QuestNPCName = "Quest Log";
		m_QuestMenuController.NotifyPropertyChanged("QuestNPCName");
		
		QuestListContent.Show(true);
		DefaultPanel.Show(false);
		
		if (!m_ClientQuestData)
		{
			QuestDebug(ToString() + "::SetQuestLogView - Could not get players persistent quest data! Not created yet?!");
		}
		
		PlayerBase player = PlayerBase.Cast(g_Game.GetPlayer());
		if (!player)
		{
			Error(ToString() + "::SetQuests - Could not player!");
			CloseMenu();
			return;
		}

		if (!m_ClientQuestConfigs || m_ClientQuestConfigs.Count() == 0)
		{
			Error(ToString() + "::SetQuests - Could not get any quest configurations!");
			CloseMenu();
			return;
		}
		
		//! Check quest configurations array for valid quests to display.
		ExpansionQuestMenuLogEntry questEntry;
		foreach (ExpansionQuestConfig questConfig: m_ClientQuestConfigs)
		{
			//! We only want to display quests here that can be started when the quest has no quest-giver ID, active quests the player is currently progressing thrue and quests that are ready for turn-in (excludes achievement quest).
			int questID = questConfig.GetID();
			ExpansionQuestState questState = ExpansionQuestState.NONE;
			questState = m_ClientQuestData.GetQuestStateByQuestID(questID);
			string stateText = typename.EnumToString(ExpansionQuestState, questState);
			QuestDebug(ToString() + "::SetQuests - Quest state for quest " + questID + " is " + stateText);
			
			if (!questConfig.IsAchievement() && (questState == ExpansionQuestState.STARTED || questState == ExpansionQuestState.CAN_TURNIN || (questState == ExpansionQuestState.NONE && !questConfig.GetQuestGiverIDs().Count() && ExpansionQuestModule.GetModuleInstance().QuestDisplayConditions(questConfig, player, m_ClientQuestData, -1, false))))
			{
				QuestDebug(ToString() + "::SetQuests - Show quest " + questID + ". Add to menu quest.");
				m_Quests.Insert(questConfig);
				questEntry = new ExpansionQuestMenuLogEntry(questConfig, this);
				m_QuestMenuController.QuestLogs.Insert(questEntry);
			}
		}
		
		QuestDebug(ToString() + "::SetQuestLogView - End");
	}
	
	/**
	 * Client/server handshake
	 * 
	 * Server: ExpansionActionOpenQuestMenu::OnExecuteServer
	 * Server: ExpansionQuestModule::RequestOpenQuestMenu
	 * Client: ExpansionQuestModule::RPC_RequestOpenQuestMenu
	 * Client: Invoke ExpansionQuestMenu::SetQuests
	 **/
	void SetQuests(string npcName = "", string defaultText = "", int questNPCID = -1, int questID = -1, int serverTime = 0)
	{
		QuestDebug(ToString() + "::SetQuests - Start");
		QuestDebug(ToString() + "::SetQuests - NPC name: " + npcName);
		QuestDebug(ToString() + "::SetQuests - NPC default text: " + defaultText);
		QuestDebug(ToString() + "::SetQuests - NPC ID: " + questNPCID);
		QuestDebug(ToString() + "::SetQuests - Quest ID: " + questID);
		QuestDebug(ToString() + "::SetQuests - Server time: " + serverTime);

		m_QuestMenuController.Quests.Clear();
		
		m_CurrentNPCName = npcName;
		m_CurrentNPCText = defaultText;
		m_CurrentNPCID = questNPCID;
		m_CurrentQuestID = questID;
		
		m_SelectedReward = NULL;
		m_PlayerData = ExpansionQuestModule.GetModuleInstance().GetClientQuestData();

		ButtonsPanel.Show(false);
		HideHud.Show(false);
		MenuButtonsPanel.Show(true);

		m_QuestMenuController.QuestNPCName = npcName;
		m_QuestMenuController.NotifyPropertyChanged("QuestNPCName");

		QuestListContent.Show(true);
		DefaultPanel.Show(false);

		if (!m_ClientQuestData)
		{
			QuestDebug(ToString() + "::SetQuests - Could not get players persistent quest data! Not created yet?!");
		}

		PlayerBase player = PlayerBase.Cast(g_Game.GetPlayer());
		if (!player)
		{
			Error(ToString() + "::SetQuests - Could not player!");
			CloseMenu();
			return;
		}

		if (!m_ClientQuestConfigs || m_ClientQuestConfigs.Count() == 0)
		{
			Error(ToString() + "::SetQuests - Could not get any quest configurations!");
			CloseMenu();
			return;
		}
		
		//ExpansionQuestConfig questToShow;
		ExpansionQuestMenuListEntry questEntry;
		//! Check quest configurations array for valid quests to display.
		foreach (ExpansionQuestConfig questConfig: m_ClientQuestConfigs)
		{
			QuestDebug(ToString() + "::SetQuests - Checking if quest " + questConfig.GetID() + " should be displayed..");
			if (!ExpansionQuestModule.GetModuleInstance().QuestDisplayConditions(questConfig, player, m_ClientQuestData, questNPCID, true))
			{
				QuestDebug(ToString() + "::SetQuests - Don't show quest " + questConfig.GetID() + ". Skip..");
				continue;
			}
			
			if (questID == -1)
			{
				QuestDebug(ToString() + "::SetQuests - Show quest " + questConfig.GetID() + ". Add to menu quest.");
				m_Quests.Insert(questConfig);
				questEntry = new ExpansionQuestMenuListEntry(questConfig, this, serverTime);
				m_QuestMenuController.Quests.Insert(questEntry);
			}
			else if (questID > -1 && questConfig.GetID() == questID)
			{
				//questToShow = questConfig;
				QuestDebug(ToString() + "::SetQuests - Show quest " + questConfig.GetID() + ". Add to menu quest.");
				m_Quests.Insert(questConfig);
				questEntry = new ExpansionQuestMenuListEntry(questConfig, this, serverTime);
				m_QuestMenuController.Quests.Insert(questEntry);
			}
		}

		//! Display default NPC text if there are no quests to display.
		if (m_Quests.Count() == 0)
		{
			QuestListContent.Show(false);
			DefaultPanel.Show(true);
			
			StringLocaliser defaultNPCText = new StringLocaliser(defaultText, g_Game.GetPlayer().GetIdentity().GetName());
			m_QuestMenuController.DefaultText = defaultNPCText.Format();
			m_QuestMenuController.NotifyPropertyChanged("DefaultText");
		}
		
		/*if (questID > -1 && questToShow)
		{
			SetQuest(questToShow);
		}*/

		QuestDebug(ToString() + "::SetQuests - End");
	}

	void SetQuest(ExpansionQuestConfig quest)
	{
		QuestDebug(ToString() + "::SetQuest - Start");
		
		ExpansionQuestState questState = ExpansionQuestState.NONE;
		m_PlayerData = ExpansionQuestModule.GetModuleInstance().GetClientQuestData();
		
		if (m_PlayerData)
			m_CurrentPlayerQuestData = m_PlayerData.GetQuestDataByQuestID(quest.GetID());
		
		if (m_CurrentPlayerQuestData)
			questState = m_CurrentPlayerQuestData.State;

		QuestDebug(ToString() + "::SetQuest - Quest state: " + questState);
		QuestListPanel.Show(false);

		MenuButtonsPanel.Show(false);
		ButtonsPanel.Show(true);
		QuestDetailsPanel.Show(true);

		m_SelectedQuest = quest;
		m_QuestMenuController.QuestTitle = quest.GetTitle();
		m_QuestMenuController.NotifyPropertyChanged("QuestTitle");

		string description;
		string objectiveText;
		if (!m_QuestLogMode)
		{
			switch (questState)
			{
				case ExpansionQuestState.NONE:
				{
					description = quest.GetDescriptions()[0];
					objectiveText = quest.GetObjectiveText();
					ObjectivePanel.Show(true);
					Accept.Show(true);
					Complete.Show(false);
					Cancel.Show(false);
					Share.Show(false);
				}
				break;
				case ExpansionQuestState.STARTED:
				{
					description = quest.GetDescriptions()[1];
					objectiveText = quest.GetObjectiveText();
					ObjectivePanel.Show(true);
					Cancel.Show(true);
					Accept.Show(false);
					Complete.Show(false);
					Share.Show(false);
				}
				break;
				case ExpansionQuestState.CAN_TURNIN:
				{
					description = quest.GetDescriptions()[2];
					ObjectivePanel.Show(false);
					Complete.Show(true);
					Accept.Show(false);
					Cancel.Show(true);
					Share.Show(false);
				}
				break;
			}
		}
		else
		{
			switch (questState)
			{
				case ExpansionQuestState.NONE:
				{
					description = quest.GetDescriptions()[0];
					objectiveText = quest.GetObjectiveText();
					ObjectivePanel.Show(true);
					Accept.Show(true);
					Complete.Show(false);
					Cancel.Show(false);
					Share.Show(false);
				}
				break;
				case ExpansionQuestState.STARTED:
				{
					description = quest.GetDescriptions()[0];
					objectiveText = quest.GetObjectiveText();
					ObjectivePanel.Show(true);
					Accept.Show(false);
					Complete.Show(false);
					Cancel.Show(true);
					if (!quest.IsGroupQuest() && DisplayShareButton())
						Share.Show(true);
					else
						Share.Show(false);
				}
				break;
				case ExpansionQuestState.CAN_TURNIN:
				{
					ObjectivePanel.Show(false);
					if (quest.GetQuestTurnInIDs().Count() == 0 || quest.IsAutocomplete())
					{
						description = quest.GetDescriptions()[2];
						Complete.Show(true);
					}
					else
					{
						description = quest.GetDescriptions()[0];
						Complete.Show(false);
					}
		
					Accept.Show(false);
					Cancel.Show(true);
					if (!quest.IsGroupQuest() && DisplayShareButton())
						Share.Show(true);
					else
						Share.Show(false);
				}
				break;
			}
		}

		StringLocaliser descriptiontext = new StringLocaliser(description, g_Game.GetPlayer().GetIdentity().GetName());
		m_QuestMenuController.QuestDescription = descriptiontext.Format();
		m_QuestMenuController.NotifyPropertyChanged("QuestDescription");
		
		if (objectiveText != m_QuestMenuController.QuestObjective)
		{
			m_QuestMenuController.QuestObjective = objectiveText;
			m_QuestMenuController.NotifyPropertyChanged("QuestObjective");
		}

		SetObjectiveItems(quest, questState);

		SetQuestItems(quest);
		
		SetRewardItems(quest);

	#ifdef EXPANSIONMODHARDLINE
		if (quest.GetReputationReward() && GetExpansionSettings().GetHardline().UseReputation)
		{
			RewardPanel.Show(true);
			Reputation.Show(true);
			m_QuestMenuController.ReputationVal = quest.GetReputationReward().ToString();
			m_QuestMenuController.NotifyPropertyChanged("ReputationVal");
		}
		else
		{
			Reputation.Show(false);
		}

	#ifdef EXPANSIONMODAI
		m_QuestMenuController.FactionRewardEntries.Clear();
		
		map<string, int> factionRewards = quest.GetFactionReputationRewards();
		if (factionRewards.Count() > 0)
		{
			RewardPanel.Show(true);
			FactionReputation.Show(true);
			foreach (string factionName, int reputation: factionRewards)
			{
				ExpansionQuestMenuFactionReward factionReward = new ExpansionQuestMenuFactionReward(factionName, reputation);
				m_QuestMenuController.FactionRewardEntries.Insert(factionReward);
			}
		}
		else
		{
			FactionReputation.Show(false);
		}
	#endif
	#endif

		m_InDetailView = true;

		QuestDebug(ToString() + "::SetQuest - End");
	}
	
	protected void SetObjectiveItems(ExpansionQuestConfig quest, ExpansionQuestState questState)
	{
		string className;
		int amount;
		
		m_QuestMenuController.ObjectiveItems.Clear();
		
		array<ref ExpansionQuestObjectiveConfigBase> questObjectives = quest.GetObjectives();
		if (questObjectives && questObjectives.Count() > 0)
		{
			for (int i = 0; i < questObjectives.Count(); i++)
			{
				ExpansionQuestObjectiveConfigBase objective = quest.GetObjectives()[i];
				ExpansionQuestObjectiveData playerObjectiveData;
				int objectiveType = objective.GetObjectiveType();
				int j;

				QuestDebug(ToString() + "::SetObjectiveItems - Objective type: " + objectiveType);
				
				switch (objectiveType)
				{
					case ExpansionQuestObjectiveType.COLLECT:
					{
						QuestDebug(ToString() + "::SetObjectiveItems - Check for items to collect..");
						ExpansionQuestObjectiveCollectionConfig collectionDelivery = ExpansionQuestObjectiveCollectionConfig.Cast(objective);
						if (collectionDelivery.NeedAnyCollection() && !m_NeedToSelectObjItem)
						{
							ObjectivePanel.Show(true);
							m_NeedToSelectObjItem = true;
						}
						
						bool setDefaultEntry = false;
						for (j = 0; j < collectionDelivery.GetCollections().Count(); j++)
						{
							ExpansionQuestObjectiveDelivery collection = collectionDelivery.GetCollections()[j];
							int currentCollectionCount = 0;
							className = collection.GetClassName();
							amount = collection.GetAmount();
							ExpansionQuestMenuItemEntry collectionObjectiveEntry = new ExpansionQuestMenuItemEntry(className, amount);
							
							if (collectionDelivery.NeedAnyCollection())
							{
								if (m_CurrentPlayerQuestData)
									playerObjectiveData = m_CurrentPlayerQuestData.GetObjectiveByIndex(i);
								
								if (playerObjectiveData)
								{
									currentCollectionCount = playerObjectiveData.GetDeliveryCountByIndex(j);
									if (currentCollectionCount >= amount)
									{
										collectionObjectiveEntry.SetQuestMenu(this);
										collectionObjectiveEntry.SetIsObjectiveItemEntry(true);
										collectionObjectiveEntry.SetObjectiveItemIndex(j);
									}
								}
							}
							
							if (questState == ExpansionQuestState.CAN_TURNIN && collectionDelivery.NeedAnyCollection() && currentCollectionCount < amount)
								continue;

							m_QuestMenuController.ObjectiveItems.Insert(collectionObjectiveEntry);
							
							if (m_NeedToSelectObjItem && !setDefaultEntry && currentCollectionCount >= amount)
							{
								collectionObjectiveEntry.OnItemButtonClick();
								setDefaultEntry = true;
							}
							
							QuestDebug(ToString() + "::SetObjectiveItems - Add objective item entry for item: " + className + " - COLLECT");
						}
					}
					break;
					case ExpansionQuestObjectiveType.DELIVERY:
					{
						QuestDebug(ToString() + "::SetObjectiveItems - Check for items to deliver..");
						ExpansionQuestObjectiveDeliveryConfig objectiveDelivery = ExpansionQuestObjectiveDeliveryConfig.Cast(objective);
						array<ref ExpansionQuestObjectiveDelivery> deliveries = objectiveDelivery.GetCollections();
						for (j = 0; j < deliveries.Count(); j++)
						{
							ExpansionQuestObjectiveDelivery delivery = deliveries[j];
							className = delivery.GetClassName();
							amount = delivery.GetAmount();
							ExpansionQuestMenuItemEntry deliverObjectiveEntry = new ExpansionQuestMenuItemEntry(className, amount);
							m_QuestMenuController.ObjectiveItems.Insert(deliverObjectiveEntry);
							QuestDebug(ToString() + "::SetObjectiveItems - Add objective item entry for item: " + className + " - DELIVERY");
						}
					}
					break;
					case ExpansionQuestObjectiveType.CRAFTING:
					{
						QuestDebug(ToString() + "::SetObjectiveItems - Check for items to craft..");
						ExpansionQuestObjectiveCraftingConfig objectiveCrafting = ExpansionQuestObjectiveCraftingConfig.Cast(objective);
						for (j = 0; j < objectiveCrafting.GetItemNames().Count(); j++)
						{
							className = objectiveCrafting.GetItemNames()[j];
							amount = 1;
							ExpansionQuestMenuItemEntry craftingObjectiveEntry = new ExpansionQuestMenuItemEntry(className, amount);
							m_QuestMenuController.ObjectiveItems.Insert(craftingObjectiveEntry);
							QuestDebug(ToString() + "::SetObjectiveItems - Add objective item entry for item: " + className + " - CRAFTING");
						}
					}
					break;
				}
			}
		}
	}
	
	protected void SetQuestItems(ExpansionQuestConfig quest)
	{
		QuestItemsPanel.Show(false);
		m_QuestMenuController.QuestItemEntries.Clear();
		
		array<ref ExpansionQuestItemConfig> questItems = quest.GetQuestItems();
		if (questItems && questItems.Count() > 0)
		{
			QuestItemsPanel.Show(true);
			foreach (ExpansionQuestItemConfig questItem: questItems)
			{
				string className = questItem.GetClassName();
				int amount = questItem.GetAmount();
				ExpansionQuestMenuItemEntry questItemEntry = new ExpansionQuestMenuItemEntry(className, amount);
				m_QuestMenuController.QuestItemEntries.Insert(questItemEntry);
				QuestDebug(ToString() + "::SetQuestItems - Add quest item entry for item: " + className);
			}
		}
	}
	
	protected void SetRewardItems(ExpansionQuestConfig quest)
	{
		m_QuestMenuController.RewardEntries.Clear();
		int rewardsCount = quest.GetRewards().Count();
		RewardPanel.Show(false);
		if (rewardsCount > 0)
		{
			RewardPanel.Show(true);
			if (quest.NeedToSelectReward() && quest.GetRewards().Count() > 1)
			{
				Reward.SetText("#STR_EXPANSION_QUEST_MENU_REWARD_LABEL");
			}
			else if (quest.RandomReward() && quest.GetRewards().Count() > 1)
			{
				if (quest.GetRewardBehavior() == ExpansionQuestRewardBehavior.RANDOMIZED_ON_COMPLETION)
				{
					StringLocaliser loc = new StringLocaliser("STR_EXPANSION_QUEST_MENU_RANDOMREWARD_LABEL", quest.GetRandomRewardAmount().ToString());
					Reward.SetText(loc.Format());
				}
				else if (quest.GetRewardBehavior() == ExpansionQuestRewardBehavior.RANDOMIZED_ON_START)
				{
					Reward.SetText("#STR_EXPANSION_QUEST_MENU_REWARDS_LABEL");
				}
			}
			else
			{
				Reward.SetText("#STR_EXPANSION_QUEST_MENU_REWARDS_LABEL");
			}
			
			RewardSelection.Show(false);
			if (rewardsCount > 0)
			{
				RewardSelection.Show(true);
				for (int i = 0; i < quest.GetRewards().Count(); i++)
				{
					ExpansionQuestRewardConfig reward = quest.GetRewards()[i];
					ExpansionQuestMenuItemEntry rewardEntry = new ExpansionQuestMenuItemEntry(reward.GetClassName(), reward.GetAmount(), reward.GetAttachments());
					rewardEntry.SetQuestRewardConfig(reward);
					rewardEntry.SetQuestMenu(this);
					rewardEntry.SetIsRewardEntry(true);
					m_QuestMenuController.RewardEntries.Insert(rewardEntry);
				}
			}
		}
	}

	void SetSelectedReward(ExpansionQuestRewardConfig reward)
	{
		m_SelectedReward = reward;
	}
	
	void SetSelectedObjectiveItem(int itemIndex)
	{
		m_SelectedObjectiveItemIndex = itemIndex;
		QuestDebug(ToString() + "::SetSelectedObjectiveItem - Set selected objective item index: " + m_SelectedObjectiveItemIndex);
	}

	void ResetRewardElements()
	{
		for (int i = 0; i < m_QuestMenuController.RewardEntries.Count(); i++)
		{
			ExpansionQuestMenuItemEntry entry = m_QuestMenuController.RewardEntries[i];
			entry.Reset();
		}
	}
	
	void OnAcceptButtonClick()
	{
		if (!m_SelectedQuest)
			return;

		if ( HasTooManyQuests() )
		{
			ExpansionNotification(new StringLocaliser("STR_EXPANSION_QUEST_MAXREACHED_TITLE"), new StringLocaliser("STR_EXPANSION_QUEST_MAXREACHED_DESC"), EXPANSION_NOTIFICATION_ICON_INFO, COLOR_EXPANSION_NOTIFICATION_ERROR).Create();
		}
		else
		{
			ExpansionQuestModule.GetModuleInstance().RequestCreateQuestInstance(m_SelectedQuest.GetID());
		}

		OnCloseButtonClick();
	}

	bool HasTooManyQuests()
	{
		ExpansionQuestPersistentData playerData = ExpansionQuestModule.GetModuleInstance().GetClientQuestData();
		if (!playerData)
		{
			EXTrace.Print(EXTrace.QUESTS, this, "no client quest data - skipping");
			return false;
		}
		int maxAllowed = GetExpansionSettings().GetQuest().MaxActiveQuests;
		if ( maxAllowed == -1 )
			return false;

		int questCount;
		foreach (ExpansionQuestPersistentQuestData data: playerData.QuestData)
		{
			if ( questCount >= maxAllowed )
				return true;

			int questID = data.QuestID;
			ExpansionQuestState state = data.State;

			if (state == ExpansionQuestState.NONE || state == ExpansionQuestState.COMPLETED)
				continue;

			ExpansionQuestConfig questConfig = ExpansionQuestModule.GetModuleInstance().GetQuestConfigByID(questID);
			if (!questConfig)
			{
				continue;
			}

			if (questConfig.IsAchievement())
				continue;
			
			questCount++;
		}

		return false;
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
		
		StringLocaliser title;
		StringLocaliser text;

		if (m_NeedToSelectObjItem && m_SelectedObjectiveItemIndex == -1)
		{
			title = new StringLocaliser("STR_EXPANSION_QUEST_TITLE", m_SelectedQuest.GetTitle());
			text = new StringLocaliser("STR_EXPANSION_QUEST_MENU_ERROR_OBJECTIVEITEM");
			ExpansionNotification(title, text, EXPANSION_NOTIFICATION_ICON_INFO, COLOR_EXPANSION_NOTIFICATION_ERROR).Create();
			return;
		}
		
		if (m_SelectedQuest.NeedToSelectReward())
		{
			if (m_SelectedReward)
			{
				ExpansionQuestModule.GetModuleInstance().RequestTurnInQuestClient(m_SelectedQuest.GetID(), true, m_SelectedReward, m_SelectedObjectiveItemIndex);
			}
			else
			{
				title = new StringLocaliser("STR_EXPANSION_QUEST_TITLE", m_SelectedQuest.GetTitle());
				text = new StringLocaliser("STR_EXPANSION_QUEST_MENU_ERROR_REWARD");
				ExpansionNotification(title, text, EXPANSION_NOTIFICATION_ICON_INFO, COLOR_EXPANSION_NOTIFICATION_ERROR).Create();
				return;
			}
		}
		else
		{
			ExpansionQuestModule.GetModuleInstance().RequestTurnInQuestClient(m_SelectedQuest.GetID(), false, null, m_SelectedObjectiveItemIndex);
		}

		ButtonsPanel.Show(false);
		CloseMenu();
	}

	void OnCloseButtonClick()
	{
		if (!m_InDetailView)
		{
			CloseMenu();
		}
		else if (m_Quests.Count() == 0)
		{
			CloseMenu();
		}
		else
		{
			ButtonsPanel.Show(false);
			QuestDetailsPanel.Show(false);
			MenuButtonsPanel.Show(true);
			QuestListPanel.Show(true);
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
	
	void OnHideHudButtonClick()
	{
		MissionGameplay mission = MissionGameplay.Cast(GetDayZGame().GetMission());
		if (mission)
		{
			mission.ToggleQuestHUD();
			if (!mission.QuestHudState())
			{
				m_QuestMenuController.HideHudLable = "#STR_EXPANSION_QUEST_MENU_HIDE_HUD";
			}
			else
			{
				m_QuestMenuController.HideHudLable = "#STR_EXPANSION_QUEST_MENU_SHOW_HUD";
			}

			m_QuestMenuController.NotifyPropertyChanged("HideHudLable");
		}
	}
	
	void OnShareButtonClick()
	{
		if (!m_SelectedQuest)
			return;

		ExpansionQuestModule.GetModuleInstance().RequestShareQuest(m_SelectedQuest.GetID());
	}

	bool DisplayShareButton()
	{
		bool canShow;
	#ifdef EXPANSIONMODGROUPS
		canShow = PlayerBase.Cast(g_Game.GetPlayer()).Expansion_GetPartyID() > -1;
	#endif

		return canShow;
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
			return true;
		}
		else if (w == Complete)
		{
			CompleteBackground.SetColor(ARGB(200, 220, 220, 220));
			CompleteLable.SetColor(ARGB(255, 0, 0, 0));
			return true;
		}
		else if (w == Cancel)
		{
			CancelBackground.SetColor(ARGB(200, 220, 220, 220));
			CancelLable.SetColor(ARGB(255, 0, 0, 0));
			return true;
		}
		else if (w == Close)
		{
			CloseBackground.SetColor(ARGB(200, 220, 220, 220));
			CloseLable.SetColor(ARGB(255, 0, 0, 0));
			return true;
		}
		else if (w == CloseMenu)
		{
			CloseMenuCharacter.SetColor(ARGB(255, 0, 0, 0));
			CloseMenuImage.SetColor(ARGB(255, 0, 0, 0));
			return true;
		}
		else if (w == Back)
		{
			BackBackground.SetColor(ARGB(200, 220, 220, 220));
			BackImage.SetColor(ARGB(255, 0, 0, 0));
			return true;
		}
		else if (w == HideHud)
		{
			HideHudBackground.SetColor(ARGB(200, 220, 220, 220));
			HideHudLable.SetColor(ARGB(255, 0, 0, 0));
			return true;
		}
		else if (w == Share)
		{
			ShareBackground.SetColor(ARGB(200, 220, 220, 220));
			ShareLable.SetColor(ARGB(255, 0, 0, 0));
			return true;
		}

		return false;
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (w == Accept)
		{
			AcceptBackground.SetColor(ARGB(200, 0, 0, 0));
			AcceptLable.SetColor(ARGB(255, 220, 220, 220));
			return true;
		}
		else if (w == Complete)
		{
			CompleteBackground.SetColor(ARGB(200, 0, 0, 0));
			CompleteLable.SetColor(ARGB(255, 220, 220, 220));
			return true;
		}
		else if (w == Cancel)
		{
			CancelBackground.SetColor(ARGB(200, 0, 0, 0));
			CancelLable.SetColor(ARGB(255, 220, 220, 220));
			return true;
		}
		else if (w == Close)
		{
			CloseBackground.SetColor(ARGB(200, 0, 0, 0));
			CloseLable.SetColor(ARGB(255, 220, 220, 220));
			return true;
		}
		else if (w == CloseMenu)
		{
			CloseMenuCharacter.SetColor(ARGB(255, 220, 220, 220));
			CloseMenuImage.SetColor(ARGB(255, 220, 220, 220));
			return true;
		}
		else if (w == Back)
		{
			BackBackground.SetColor(ARGB(200, 0, 0, 0));
			BackImage.SetColor(ARGB(255, 220, 220, 220));
			return true;
		}
		else if (w == HideHud)
		{
			HideHudBackground.SetColor(ARGB(200, 0, 0, 0));
			HideHudLable.SetColor(ARGB(255, 220, 220, 220));
			return true;
		}
		else if (w == Share)
		{
			ShareBackground.SetColor(ARGB(200, 0, 0, 0));
			ShareLable.SetColor(ARGB(255, 220, 220, 220));
			return true;
		}

		return false;
	}

	override void OnShow()
	{
		super.OnShow();
		
		SetFocus(GetLayoutRoot());
		
		GetUApi().GetInputByName("UAExpansionQuestLogToggle").ForceDisable(false);		
		GetUApi().UpdateControls();
		
		m_ClientQuestData = ExpansionQuestModule.GetModuleInstance().GetClientQuestData();
		m_ClientQuestConfigs = ExpansionQuestModule.GetModuleInstance().GetQuestConfigs();
	}

	override void OnHide()
	{
		if (m_CurrentNPCID > -1 && !m_QuestLogMode)
			ExpansionQuestModule.GetModuleInstance().ExitQuestMenu(m_CurrentNPCID);

		if (m_QuestLogMode)
			m_QuestLogMode = false;
		
		super.OnHide();
	}

	override bool CanClose()
	{
		if (m_CancelQuestDialog)
			return !m_CancelQuestDialog.IsVisible();

		return true;
	}
	
	void SetLogMode(bool state)
	{
		QuestDebug(ToString() + "::SetLogMode - State: " + state);
		m_QuestLogMode = state;
	}

	void QuestDebug(string text)
	{
	#ifdef EXPANSIONMODQUESTSUIDEBUG
		EXTrace.Print(EXTrace.QUESTS, this, text);
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
	string HideHudLable;
	ref ObservableCollection<ref ExpansionQuestMenuListEntry> Quests = new ObservableCollection<ref ExpansionQuestMenuListEntry>(this);
	ref ObservableCollection<ref ExpansionQuestMenuLogEntry> QuestLogs = new ObservableCollection<ref ExpansionQuestMenuLogEntry>(this);
	ref ObservableCollection<ref ExpansionQuestMenuItemEntry> RewardEntries = new ObservableCollection<ref ExpansionQuestMenuItemEntry>(this);
	ref ObservableCollection<ref ExpansionQuestMenuItemEntry> ObjectiveItems = new ObservableCollection<ref ExpansionQuestMenuItemEntry>(this);
	ref ObservableCollection<ref ExpansionQuestMenuItemEntry> QuestItemEntries = new ObservableCollection<ref ExpansionQuestMenuItemEntry>(this);
	ref ObservableCollection<ref ExpansionQuestMenuFactionReward> FactionRewardEntries = new ObservableCollection<ref ExpansionQuestMenuFactionReward>(this);
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

class ExpansionQuestMenuFactionReward: ExpansionScriptView
{
	protected ref ExpansionQuestMenuFactionRewardController m_QuestMenuFactionRewardController;
	string m_FactionName;
	int m_Reputation;
	
	void ExpansionQuestMenuFactionReward(string factionName, int reputation)
	{
		m_FactionName = factionName;
		m_Reputation = reputation;

		Class.CastTo(m_QuestMenuFactionRewardController, GetController());

		SetEntry();
	}
	
	void SetEntry()
	{
		m_QuestMenuFactionRewardController.FactionName = m_FactionName;
		m_QuestMenuFactionRewardController.Reputation = m_Reputation.ToString();
		
		m_QuestMenuFactionRewardController.NotifyPropertiesChanged({"FactionName", "Reputation"});
	}
	
	override string GetLayoutFile()
	{
		return "DayZExpansion/Quests/GUI/layouts/quests/expansion_quest_menu_faction_reward.layout";
	}

	override typename GetControllerType()
	{
		return ExpansionQuestMenuFactionRewardController;
	}
};

class ExpansionQuestMenuFactionRewardController: ExpansionViewController
{
	string FactionName;
	string Reputation;
};