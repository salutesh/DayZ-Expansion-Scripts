/**
 * ExpansionQuestHUD.c
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
class ExpansionQuestHUD: ExpansionScriptView
{
	protected ref ExpansionQuestHUDController m_QuestHUDController;
	protected ref array<int> m_HiddenIDs = new array<int>;
	protected WrapSpacerWidget QuestEntriesWraper;
	protected ref map<int, ref ExpansionQuestHUDEntry> m_QuestEntries = new map<int, ref ExpansionQuestHUDEntry>;
	
	protected ref map<int, int> m_NpcIDsShow = new map<int, int>;
	protected ref array<int> m_NpcIDsHide = new array<int>;
	
	void ExpansionQuestHUD()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
		
		m_QuestHUDController = ExpansionQuestHUDController.Cast(GetController());
		
		ExpansionQuestModule.GetModuleInstance().GetQuestHUDCallbackSI().Insert(UpdateView);
	}

	void UpdateView()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
		
		if (!IsVisible())
		{
			EXTrace.Print(EXTrace.QUESTS, this, "not visible - skipping");
			return;
		}

		ExpansionQuestPersistentData playerData = ExpansionQuestModule.GetModuleInstance().GetClientQuestData();
		if (!playerData)
		{
			EXTrace.Print(EXTrace.QUESTS, this, "no client quest data - skipping");
			return;
		}

		array<ref ExpansionQuestPersistentQuestData> questDatas = playerData.GetQuestDatas();
		array<int> updatedIDs = new array<int>;
		foreach (ExpansionQuestPersistentQuestData data: questDatas)
		{
			int questID = data.QuestID;
			ExpansionQuestState state = data.State;

			if (state == ExpansionQuestState.NONE || state == ExpansionQuestState.COMPLETED)
				continue;

			ExpansionQuestConfig questConfig = ExpansionQuestModule.GetModuleInstance().GetQuestConfigClientByID(questID);
			if (!questConfig)
			{
				continue;
			}

			if (questConfig.IsAchievement())
				continue;

			ExpansionQuestHUDEntry entry;
			if (m_QuestEntries.Find(questConfig.GetID(), entry))
			{
				entry.UpdateQuestData(data);
			}
			else
			{
				entry = new ExpansionQuestHUDEntry(questConfig, data);
				QuestEntriesWraper.AddChild(entry.GetLayoutRoot());
				m_QuestEntries.Insert(questConfig.GetID(), entry);
				entry.SetEntry();
	
				int findeIndexHidden = -1;
				findeIndexHidden = m_HiddenIDs.Find(questID);
				if (findeIndexHidden == -1)
				{
					entry.Show();
				}
				else
				{
					entry.Hide();
				}
			}
			
			updatedIDs.Insert(questID);
		}
		
		foreach (int entryQuestID, ExpansionQuestHUDEntry hudEntry: m_QuestEntries)
		{
			if (updatedIDs.Find(entryQuestID) == -1)
			{
				hudEntry.Destroy();
				m_QuestEntries.Remove(entryQuestID);
			}
		}
	}

	void ShowHud(bool state)
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
		
		if (state)
		{
			Show();
		}
		else
		{
			Hide();
		}
	}
	
	override void OnShow()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
		
		super.OnShow();
		
		UpdateView();
	}
	
	override void OnHide()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
		
		super.OnHide();
	}

	override string GetLayoutFile()
	{
		return "DayZExpansion/Quests/GUI/layouts/quests/expansion_quest_hud.layout";
	}

	override typename GetControllerType()
	{
		return ExpansionQuestHUDController;
	}

	void ToggleQuestEntryVisibilityByID(int questID)
	{
		ExpansionQuestHUDEntry entry;
		int findIndex = -1;
		if (!IsEntryHidden(questID, entry, findIndex))
		{
			m_HiddenIDs.Insert(questID);
			entry.Hide();
		}
		else
		{
			m_HiddenIDs.Remove(findIndex);
			entry.Show();
		}
	}

	bool IsEntryHidden(int questID, out ExpansionQuestHUDEntry entry, out int findIndex)
	{
		findIndex = m_HiddenIDs.Find(questID);
		foreach (ExpansionQuestHUDEntry hudEntry: m_QuestEntries)
		{
			if (hudEntry.GetEntryQuestID() == questID)
			{
				entry = hudEntry;
				break;
			}
		}

		if (findIndex == -1)
			return false;
		return true;
	}
	
	void CheckQuestNPCs()
	{
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
		
		ExpansionQuestPersistentData playerQuestData = ExpansionQuestModule.GetModuleInstance().GetClientQuestData();
		if (!playerQuestData)
			return;
		
		array<ref ExpansionQuestConfig> questConfigs = ExpansionQuestModule.GetModuleInstance().GetQuestConfigsClient();
		if (!questConfigs)
			return;
		
		m_NpcIDsShow.Clear();
		m_NpcIDsHide.Clear();
		
		foreach (int questID, ExpansionQuestConfig config: questConfigs)
		{
			int questState = playerQuestData.GetQuestStateByQuestID(config.GetID());
			bool completed = false;			
			if (!config.IsRepeatable() && (ExpansionQuestModule.GetModuleInstance().HasCompletedQuest(questID, GetGame().GetPlayer().GetIdentity().GetId()) || questState == ExpansionQuestState.COMPLETED))
			{
				ExDebugPrint("::CheckQuestNPCs - Quest is already completed! Skip..");
				completed = true;
				
			}
		
			int timestamp;
			bool hasCooldown = false;
			if (playerQuestData)
				hasCooldown = playerQuestData.HasCooldownOnQuest(questID, timestamp);

			if (config.IsRepeatable() && hasCooldown)
			{
				ExDebugPrint("::CheckQuestNPCs - Quest is repeatable but on cooldown! Skip..");
				continue;
			}
	
			//! Check if quest is a achievement quest.
			if (config.IsAchievement())
			{
				ExDebugPrint("::CheckQuestNPCs - Quest is achivement quest! Skip..");
				continue;
			}
	
			//! Check if all pre-quests are completed.
			array<int> incompletedQuestIDs = new array<int>;
			if (config.GetPreQuestIDs().Count() > 0 && playerQuestData)
			{
				array<int> preQuestIDs = config.GetPreQuestIDs();
				int completedPreQuestCount;
				int completionCount;
	
				ExpansionQuestPersistentQuestData questPlayerData = playerQuestData.GetQuestDataByQuestID(questID);
				if (questPlayerData)
					completionCount = questPlayerData.CompletionCount;
	
				for (int i = 0; i < preQuestIDs.Count(); i++)
				{
					int preQuestID = preQuestIDs[i];
					ExpansionQuestPersistentQuestData preQuestPlayerData = playerQuestData.GetQuestDataByQuestID(preQuestID);
					if (preQuestPlayerData)
					{
						ExpansionQuestState preQuestState = preQuestPlayerData.State;
						int prequestCompletionCount = preQuestPlayerData.CompletionCount;
						if (preQuestState == ExpansionQuestState.COMPLETED || preQuestState == ExpansionQuestState.NONE && prequestCompletionCount > completionCount)
						{
							string preQuestStateText = typename.EnumToString(ExpansionQuestState, preQuestState);
							completedPreQuestCount++;
						}
						else
						{
							incompletedQuestIDs.Insert(preQuestID);
						}
					}
				}
			}
	
			if (config.GetPreQuestIDs().Count() > 0 && completedPreQuestCount < config.GetPreQuestIDs().Count() || incompletedQuestIDs.Count() > 0)
			{
				ExDebugPrint("::CheckQuestNPCs - Not all pre-quests completed! Skip..");
				continue;
			}
	
		#ifdef EXPANSIONMODHARDLINE
			//! Check if for reputation requirement if config has a value but system is disabled.
			if (config.GetReputationRequirement() > 0 && !GetExpansionSettings().GetHardline().UseReputation)
			{
				ExDebugPrint("::CheckQuestNPCs - Reputation system is disabled but quest has reputation requirement! Skip..");
				continue;
			}
	
			//! Check if client meats the reputation requirement.
			if (config.GetReputationRequirement() > 0)
			{
				int reputation = PlayerBase.Cast(GetGame().GetPlayer()).Expansion_GetReputation();
				if (reputation < config.GetReputationRequirement())
				{
					ExDebugPrint("::CheckQuestNPCs - Player has not required reputation! Skip..");
					continue;
				}
			}
		#endif
	
		#ifdef EXPANSIONMODAI
			//! Check if player has required faction.
			if (config.GetRequiredFaction() != string.Empty)
			{
				eAIGroup group = PlayerBase.Cast(GetGame().GetPlayer()).GetGroup();
				if (!group)
				{
					ExDebugPrint("::CheckQuestNPCs - Could not get player group! Skip..");
					continue;
				}
	
				eAIFaction playerFaction = group.GetFaction();
				if (!playerFaction)
				{
					ExDebugPrint("::CheckQuestNPCs - Could not get player faction! Skip..");
					continue;
				}
	
				string factionName = playerFaction.GetName();
				if (factionName != config.GetRequiredFaction())
				{
					ExDebugPrint("::CheckQuestNPCs - Player is not in required faction! Skip..");
					continue;
				}
			}
	
			//! Skip quests that apply the same faction as reward the player has already.
			if (playerFaction && config.GetFactionReward() != string.Empty && playerFaction.GetName() == config.GetFactionReward())
			{
				ExDebugPrint("::CheckQuestNPCs - Quest would apply same faction as the player has already! Skip..");
				continue;
			}
		#endif
						
			array<int> questGiverIDs = config.GetQuestGiverIDs();
			array<int> questTurnInIDs = config.GetQuestTurnInIDs();
						
			int state;
			if (questState < ExpansionQuestState.STARTED && !completed)
			{
				foreach (int giverID: questGiverIDs)
				{
					if (!m_NpcIDsShow.Find(giverID, state))
					{
						ExDebugPrint("::CheckQuestNPCs - SHOW MARKER - Quest ID:" + questID + " | Quest state: " + typename.EnumToString(ExpansionQuestState, questState) + " | NPC ID: " + giverID + " | State: " + state.ToString());
						m_NpcIDsShow.Insert(giverID, 0);
					}
				}
				
				foreach (int turnInID: questTurnInIDs)
				{					
					if (!m_NpcIDsShow.Find(turnInID, state) && m_NpcIDsHide.Find(turnInID) == -1)
					{
						ExDebugPrint("::CheckQuestNPCs - HIDE MARKER - Quest ID:" + questID + " | Quest state: " + typename.EnumToString(ExpansionQuestState, questState) + " | NPC ID: " + turnInID + " | State: " + state.ToString());
						m_NpcIDsHide.Insert(turnInID);
					}
				}
			}
			else if ((questState == ExpansionQuestState.STARTED || questState == ExpansionQuestState.CAN_TURNIN) && !completed)
			{
				foreach (int turnInID2: questTurnInIDs)
				{
					if (m_NpcIDsShow.Find(turnInID2, state))
					{
						ExDebugPrint("::CheckQuestNPCs - SHOW MARKER - Quest ID:" + questID + " | Quest state: " + typename.EnumToString(ExpansionQuestState, questState) + " | NPC ID: " + turnInID2 + " | State: " + state.ToString());
						if (questState == ExpansionQuestState.STARTED)
						{
							m_NpcIDsShow.Insert(turnInID2, 0);
						}
						else if (questState == ExpansionQuestState.CAN_TURNIN)
						{
							m_NpcIDsShow.Set(turnInID2, 1);
						}
					}
					else
					{
						if (questState == ExpansionQuestState.CAN_TURNIN && state != 1)
						{
							m_NpcIDsShow.Set(turnInID2, 1);
						}
					}
				}

				foreach (int giverID2: questGiverIDs)
				{
					if (!m_NpcIDsShow.Find(giverID2, state) && m_NpcIDsHide.Find(giverID2) == -1)
					{
						ExDebugPrint("::CheckQuestNPCs - HIDE MARKER - Quest ID:" + questID + " | Quest state: " + typename.EnumToString(ExpansionQuestState, questState) + " | NPC ID: " + giverID2 + " | State: " + state.ToString());
						m_NpcIDsHide.Insert(giverID2);
					}
				}
			}
		}
		
		ExpansionQuestNPCBase questNPC;
	#ifdef EXPANSIONMODAI
		ExpansionQuestNPCAIBase questAINPC;
	#endif
		ExpansionQuestStaticObject questObj;
		
		foreach (int npcIDShow, int showState: m_NpcIDsShow)
		{
			if (m_NpcIDsHide.Find(npcIDShow) > -1)
				continue;
			
			questNPC = ExpansionQuestModule.GetQuestNPCByID(npcIDShow);
		#ifdef EXPANSIONMODAI
			questAINPC = ExpansionQuestModule.GetQuestNPCAIByID(npcIDShow);
		#endif
			questObj = ExpansionQuestModule.GetQuestObjectByID(npcIDShow);
			
			if (questNPC)
			{
				questNPC.UpdateQuestMarker(true, showState);
			}
		#ifdef EXPANSIONMODAI
			else if (questAINPC)
			{
				questAINPC.UpdateQuestMarker(true, showState);
			}
		#endif
			else if (questObj)
			{
				questObj.UpdateQuestMarker(true, showState);
			}
		}
		
		foreach (int npcIDHide: m_NpcIDsHide)
		{
			if (m_NpcIDsShow.Find(npcIDHide, state))
				continue;
			
			questNPC = ExpansionQuestModule.GetQuestNPCByID(npcIDHide);
		#ifdef EXPANSIONMODAI
			questAINPC = ExpansionQuestModule.GetQuestNPCAIByID(npcIDHide);
		#endif
			questObj = ExpansionQuestModule.GetQuestObjectByID(npcIDHide);
			
			if (questNPC)
			{
				questNPC.UpdateQuestMarker(false);
			}
		#ifdef EXPANSIONMODAI
			else if (questAINPC)
			{
				questAINPC.UpdateQuestMarker(false);
			}
		#endif
			else if (questObj)
			{
				questObj.UpdateQuestMarker(false);
			}
		}
	}
	
	override void Update()
	{
		if (GetExpansionSettings().GetQuest().UseQuestNPCIndicators && IsVisible())
			CheckQuestNPCs();
	}
	
	override float GetUpdateTickRate()
	{
		return 3.0;
	}
	
	void ExDebugPrint(string text)
	{
		EXTrace.Start(EXTrace.QUESTS, this, text);
	}
};

class ExpansionQuestHUDController: ExpansionViewController {};
#endif