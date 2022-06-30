/**
 * ExpansionQuestNamalsk001.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

//! Quest instance that handles all the quest events
/*class ExpansionQuestNamalsk001: ExpansionQuest
{
	//! Called in constructor
	override void OnInit(ExpansionQuestModule module)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "OnInit").Add(sender).Add(ctx);
	#endif
	}

	override void SetQuestConfig(ExpansionQuestConfig config)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "SetQuestConfig").Add(sender).Add(ctx);
	#endif

   	 	SetConfig(config);
		GetQuestConfig().QuestDebug();

		QuestPrint(ToString() + "::SetQuestConfig - End");
	}

	//! Event called when a quest is stated
	override void OnQuestStart()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "OnQuestStart").Add(sender).Add(ctx);
	#endif

		QuestPrint(ToString() + "::OnQuestStart - Start");

		if (!ObjectivesCreated())
		{
			Error(ToString() + "::OnQuestStart - Quest objectives not created!");
			return;
		}

		if (GetPlayer())
		{
			if (!GetQuestConfig())
			{
				Error(ToString() + "::OnQuestStart - Could not get quest config!");
				return;
			}

			//! Start all quest objectives
			for (int i = 0; i < GetObjectives().Count(); i++)
			{
				ExpansionQuestObjectiveEventBase objective = GetObjectives()[i];
				objective.OnStart();
			}

			//! Add all quest items to the players inventory
			CreateQuestItems();

			SetInitialized(true);

			SetQuestState(ExpansionQuestState.STARTED);

			if (!GetQuestConfig().IsAchivement())
				SendNotification(new StringLocaliser("Quest Accepted"), new StringLocaliser("The quest %1 has been accepted!", GetQuestConfig().GetTitle()), ExpansionIcons.GetPath("Questionmark 2"), COLOR_EXPANSION_NOTIFICATION_INFO);
		}

		QuestDebug();

		QuestPrint(ToString() + "::OnQuestStart - End");
	}

	//! Event called when all quest objectives are completed
	override void OnQuestObjectivesComplete()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "OnQuestObjectivesComplete").Add(sender).Add(ctx);
	#endif

		QuestPrint(ToString() + "::OnQuestObjectivesComplete - Start");

		if (GetQuestState() >= ExpansionQuestState.CAN_TURNIN)
		{
			QuestPrint(ToString() + "::OnQuestObjectivesComplete - QUEST STATE IS ALREADY TURNIN!");
			return;
		}

		if (!GetQuestModule())
		{
			Error(ToString() + "::OnQuestObjectivesComplete - Can't get quest module!");
			return;
		}

		if (GetPlayer())
		{
			if (!GetQuestConfig())
			{
				Error(ToString() + "::OnQuestObjectivesComplete - Could not get quest config!");
				return;
			}

			GetQuestModule().OnQuestObjectivesComplete(GetQuestConfig().GetID(), GetPlayer());
			SetQuestState(ExpansionQuestState.CAN_TURNIN);

		#ifdef EXPANSIONMODNAVIGATION
			//! Create a marker on quest npc location for the player
			ExpansionQuestNPCData questNPC = GetQuestModule().GetQuestNPCDataByID(GetQuestConfig().GetQuestTurnInID());
			vector npcPos = questNPC.GetPosition();
			CreateClientMarker(npcPos, "Turn In Quest");
		#endif
		}

		QuestDebug();

		QuestPrint(ToString() + "::OnQuestObjectivesComplete - End");
	}

	//! Event called when a quest objective state has changed to incomplete after it was completed once
	override void OnQuestObjectivesIncomplete()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "OnQuestObjectivesIncomplete").Add(sender).Add(ctx);
	#endif

		QuestPrint(ToString() + "::OnQuestObjectivesIncomplete - Start");

		if (!GetQuestModule())
		{
			Error(ToString() + "::OnQuestObjectivesIncomplete - Can't get quest module!");
			return;
		}

		if (GetPlayer())
		{
			if (!GetQuestConfig() && GetQuestConfig().GetID() == -1)
			{
				Error(ToString() + "::OnQuestObjectivesIncomplete - Could not get quest config!");
				return;
			}

			GetQuestModule().OnQuestObjectivesIncomplete(GetQuestConfig().GetID(), GetPlayer());
			SetQuestState(ExpansionQuestState.STARTED);

			SendNotification(new StringLocaliser("Quest Incomplete"), new StringLocaliser("A objective state for the quest %1 has changed.", GetQuestConfig().GetTitle()), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_ORANGE);
		}

		QuestDebug();

		QuestPrint(ToString() + "::OnQuestObjectivesIncomplete - End");
	}

	//! Event called when ever a quest is completed and turned-in
	override void OnQuestTurnIn(ExpansionQuestRewardConfig reward = null)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "OnQuestTurnIn").Add(sender).Add(ctx);
	#endif

		QuestPrint(ToString() + "::OnQuestTurnIn - Start");

		if (GetQuestState() > ExpansionQuestState.CAN_TURNIN)
		{
			QuestPrint(ToString() + "::OnQuestTurnIn - QUEST STATE INVALID FOR TURN-IN!");
			return;
		}

		if (!GetQuestModule())
		{
			Error(ToString() + "::OnQuestTurnIn - Can't get quest module!");
			return;
		}

		if (GetPlayer())
		{
			if (!GetQuestConfig() && GetQuestConfig().GetID() == -1)
			{
				Error(ToString() + "::OnQuestTurnIn - Could not get quest config!");
				return;
			}

			SetQuestState(ExpansionQuestState.COMPLETED);

			int i;

			//! Call end event on all quest objectives
			for (i = 0; i < GetObjectives().Count(); i++)
			{
				ExpansionQuestObjectiveEventBase objective = GetObjectives()[i];
				objective.OnTurnIn();
			}

			//! Add all quest rewards to the players inventory
			SpawnQuestRewards(reward);

			SendNotification(new StringLocaliser("Quest Turn-In"), new StringLocaliser("The quest %1 has been completed!", GetQuestConfig().GetTitle()), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_SUCCSESS);

			SetIsCompleted(true);

			if (GetQuestConfig().GetFollowUpQuestID() > -1)
			{
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(GetQuestModule().RequestOpenQuestMenu, 1000, false, GetQuestConfig().GetQuestTurnInID(), GetPlayer().GetIdentity());
			}
		}

		QuestDebug();

		QuestPrint(ToString() + "::OnQuestTurnIn - End");
	}

	//! Event called when a quest is manualy canceled by the player
	override void OnQuestCancel()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "OnQuestCancel").Add(sender).Add(ctx);
	#endif

		QuestPrint(ToString() + "::OnQuestCancel - Start");

		if (GetPlayer())
		{
			SetQuestState(ExpansionQuestState.NONE);

			//! Cancel all active quest objectives
			for (int i = 0; i < GetObjectives().Count(); i++)
			{
				ExpansionQuestObjectiveEventBase objective = GetObjectives()[i];
				objective.OnCancel();
			}

			//! Remove all quest items from player/world
			CleanupQuestItems();

			SendNotification(new StringLocaliser("Quest Canceled"), new StringLocaliser("The quest %1 has been canceled!", GetQuestConfig().GetTitle()), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_EXPANSION);
		}

		//! Cleanup all spawned static quest objects from the object set
		m_QuestModule.CheckAndDeleteObjectSet(Config.GetID());

		QuestDebug();

		QuestPrint(ToString() + "::OnQuestCancel - End");
	}

	//! Event called when a quest gets recreated (reloaded because the player reloged/disconnected)
	override void OnQuestContinue()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "OnQuestContinue").Add(sender).Add(ctx);
	#endif

		QuestPrint(ToString() + "::OnQuestContinue - Start");

		if (!ObjectivesCreated())
		{
			Error(ToString() + "::OnQuestContinue - Quest objectives not created!");
			return;
		}

		if (GetPlayer())
		{
			if (!GetQuestConfig())
			{
				Error(ToString() + "::OnQuestContinue - Could not get quest config!");
				return;
			}

			//! Continue active quest objectives
			for (int i = 0; i < GetObjectives().Count(); i++)
			{
				ExpansionQuestObjectiveEventBase objective = GetObjectives()[i];
				objective.OnContinue();
			}

			//! Recreate quest items
			CreateQuestItems();

			SetInitialized(true);
		}

		QuestDebug();

		QuestPrint(ToString() + "::OnQuestContinue - End");
	}

	//! Event called for group quests only when a group member joins/rejoins the quest group
	override void OnGroupMemberJoined(string playerUID)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "OnGroupMemberJoined").Add(sender).Add(ctx);
	#endif
	}

	//! Event called for group quests only when a group member leaves the quest group
	override void OnGroupMemberLeave(string playerUID)
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "OnGroupMemberLeave").Add(sender).Add(ctx);
	#endif
	}

	//! Event called when quest instance is destroyed/cleaned-up
	override void OnQuestCleanup()
	{
	#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.QUESTS, this, "OnQuestCleanup").Add(sender).Add(ctx);
	#endif

		QuestPrint(ToString() + "::OnQuestCleanup - Start");

		//! Call cleanup event on all active quest objectives
		for (int i = 0; i < GetObjectives().Count(); i++)
		{
			ExpansionQuestObjectiveEventBase objective = GetObjectives()[i];
			objective.OnCleanup();
		}

		//! Cleanup all spawned quest item objects if quest gets cleaned up while not completed
		//! The items get readded to the player if he continues the quest in his next session.
		CleanupQuestItems();
		//! Cleanup all spawned static quest objects from the object set
		CleanupSetObjects();

		SetInitialized(false);

		QuestDebug();

		QuestPrint(ToString() + "::OnQuestCleanup - End");
	}

	override void CompletionCheck()
	{
		int complededObjectives = 0;
		for (int i = 0; i < GetObjectives().Count(); ++i)
		{
			ExpansionQuestObjectiveEventBase objective = GetObjectives().Get(i);
			if (objective.IsCompleted())
				complededObjectives++;
		}

		if (complededObjectives == GetObjectives().Count()) OnQuestObjectivesComplete();
		else if (complededObjectives < GetObjectives().Count() && GetQuestState() != ExpansionQuestState.STARTED) OnQuestObjectivesIncomplete();
	}

	override bool CanCompeteQuest()
	{
		int complededObjectives = 0;
		for (int i = 0; i < GetObjectives().Count(); ++i)
		{
			ExpansionQuestObjectiveEventBase objective = GetObjectives().Get(i);
			if (objective.CompletionCheck())
				complededObjectives++;
		}

		if (complededObjectives == GetObjectives().Count())
			return true;

		return false;
	}

	override void OnUpdate(float timeslice)
	{
		super.OnUpdate(timeslice);
	}

	override void QuestDebug()
	{
	#ifdef EXPANSIONMODQUESTSINSTANCEDEBUG
		QuestPrint("------------------------------------------------------------");
		QuestPrint("------------------------------------------------------------");
	#endif
	}
};*/