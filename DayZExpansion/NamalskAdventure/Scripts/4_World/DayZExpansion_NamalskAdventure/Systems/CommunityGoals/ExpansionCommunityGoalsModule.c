/**
 * ExpansionCommunityGoalsModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

[CF_RegisterModule(ExpansionCommunityGoalsModule)]
class ExpansionCommunityGoalsModule: CF_ModuleWorld
{
	protected static ExpansionCommunityGoalsModule s_ModuleInstance;
	static string s_dataFolderPath = "$mission:expansion\\community_goals\\";

	protected ref ExpansionCommunityGoalsData m_ServerData; //! Server
	protected ref map<int, ref ExpansionCommunityGoal> m_CommunityGoals; //! Server

	protected ref ScriptInvoker m_CommunityGoalsMenuInvoker; //! Client
	protected ref ScriptInvoker m_CommunityGoalsMenuDetailsInvoker; //! Client

	protected ref map<string, ref ExpansionCommunityGoalPlayerData> m_CommunityGoalsPlayerData; //! Server

	void ExpansionCommunityGoalsModule()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		s_ModuleInstance = this;
	}

	override void OnInit()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		super.OnInit();

		EnableMissionStart();
		EnableMissionLoaded();
		EnableRPC();
	}

	protected void CreateDirectoryStructure()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (!FileExist(s_dataFolderPath))
		{
			ExpansionStatic.MakeDirectoryRecursive(s_dataFolderPath);
			ExpansionStatic.MakeDirectoryRecursive(s_dataFolderPath + "contributors\\");
		}
	}

	override void OnMissionStart(Class sender, CF_EventArgs args)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		super.OnMissionLoaded(sender, args);

		//! Server only
		#ifdef SERVER
		CreateDirectoryStructure(); //! Create directoy structure if not existing.
		LoadCommunityGoalsServerData(); //! Load server data.
		#endif
	}

	override void OnMissionLoaded(Class sender, CF_EventArgs args)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		#ifdef SERVER
		ServerModuleInit();
		#endif

		#ifndef SERVER
		ClientModuleInit();
		#endif
	}

	protected void ServerModuleInit()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		//! Server only
		SpawnCommunityGoalBoards(); //! @note: Spawn community goal boards
	}

	protected void ClientModuleInit()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		m_CommunityGoalsMenuInvoker = new ScriptInvoker();
		m_CommunityGoalsMenuDetailsInvoker = new ScriptInvoker();
	}

	protected void LoadCommunityGoalsServerData()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (FileExist(s_dataFolderPath + "CommunityGoals.json"))
		{
			GetCommunityGoalsData(s_dataFolderPath + "CommunityGoals.json");
		}
		else
		{
			m_ServerData = new ExpansionCommunityGoalsData();
			m_ServerData.LoadDefaults();
			m_ServerData.Save();

			LoadCommunityGoals();
		}
	}

	protected void GetCommunityGoalsData(string file)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		m_ServerData = ExpansionCommunityGoalsData.Load(file);
		if (!m_ServerData)
		{
			Error(ToString() + "::GetServerData - Could not get namalsk adventure server data!");
			return;
		}

		LoadCommunityGoals();
	}

	protected void LoadCommunityGoals()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		m_CommunityGoals = new map<int, ref ExpansionCommunityGoal>;
		if (m_ServerData.GetCommunityGoalsData() && m_ServerData.GetCommunityGoalsData().Count() > 0)
		{
			array<ref ExpansionCommunityGoal> goals = m_ServerData.GetCommunityGoalsData();
			foreach (ExpansionCommunityGoal goalData: goals)
			{
				if (m_CommunityGoals.Find(goalData.GetID(), null))
					continue;

			    m_CommunityGoals.Insert(goalData.GetID(), goalData);

				//! Fire event logic on load if flagged as finished
				if (goalData.IsFinished() || goalData.GetProgress() >= goalData.GetGoal())
				{
					OnCommunityGoalReached(goalData.GetID(), goalData);
				}
			}
		}

		m_CommunityGoalsPlayerData = new map<string, ref ExpansionCommunityGoalPlayerData>;
		if (FileExist(s_dataFolderPath + "contributors\\"))
		{
			GetCommunityGoalsPlayerData(s_dataFolderPath + "contributors\\");
		}
	}

	protected void GetCommunityGoalsPlayerData(string path)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		array<string> files = new array<string>;
		files = ExpansionStatic.FindFilesInLocation(path, ".json");

		foreach (string fileName: files)
		{
			ExpansionCommunityGoalPlayerData communityGoalPlayerData = ExpansionCommunityGoalPlayerData.Load(path + fileName);
			if (!communityGoalPlayerData)
			{
				Error(ToString() + "::GetCommunityGoalsPlayerData - Could not get community goal player data from file: " + path + fileName + "!");
				continue;
			}

			ModuleDebugPrint("::GetCommunityGoalsPlayerData - Add community goal data for player with UID: " + communityGoalPlayerData.UID);

			m_CommunityGoalsPlayerData.Insert(communityGoalPlayerData.UID, communityGoalPlayerData);
		}
	}

	void UpdateCommunityGoalPlayerData(string uid, string name, int goalID, int points)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		ExpansionCommunityGoalPlayerData communityGoalPlayerData;
		if (m_CommunityGoalsPlayerData.Find(uid, communityGoalPlayerData))
		{
			if (communityGoalPlayerData.GetName() != name)
				communityGoalPlayerData.SetName(name);
			communityGoalPlayerData.UpdateContribution(goalID, points);
			communityGoalPlayerData.Save();

			m_CommunityGoalsPlayerData.Set(uid, communityGoalPlayerData);
		}
		else
		{
			communityGoalPlayerData = new ExpansionCommunityGoalPlayerData();
			communityGoalPlayerData.SetUID(uid);
			communityGoalPlayerData.SetName(name);
			communityGoalPlayerData.UpdateContribution(goalID, points);
			communityGoalPlayerData.Save();

			m_CommunityGoalsPlayerData.Insert(uid, communityGoalPlayerData);
		}
	}

	array<ref ExpansionCommunityGoalPlayerData> GetCommunityGoalContributors(int goalID)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		ModuleDebugPrint("::GetCommunityGoalContributors - Search for valid contributors for community goal ID: " + goalID);

		array<ref ExpansionCommunityGoalPlayerData> validContributers = new array<ref ExpansionCommunityGoalPlayerData>;
		foreach (string uid, ExpansionCommunityGoalPlayerData playerData: m_CommunityGoalsPlayerData)
		{
			ModuleDebugPrint("::GetCommunityGoalContributors - Check player data for player with UID: " + playerData.UID);
			ExpansionCommunityGoalContribution goalContribution;
			if (!playerData.Contributions.Find(goalID, goalContribution))
				continue;

			if (goalContribution.Points > 0)
			{
				ModuleDebugPrint("::GetCommunityGoalContributors - Player with UID " + playerData.UID + " is a valid contributor!");
				validContributers.Insert(playerData);
			}
		}

		return validContributers;
	}

	void UpdateCommunityGoal(PlayerBase player, int goalID, int progress)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		ExpansionCommunityGoal communityGoal;
		if (!m_CommunityGoals.Find(goalID, communityGoal))
			return;

		communityGoal.AddProgress(progress);

		//! Update persistent community goal data.
		m_ServerData.SetCommunityGoal(communityGoal);

		//! Update module community goal data.
		m_CommunityGoals.Set(goalID, communityGoal);

		//! Events when community goal is reached.
		if (communityGoal.GetProgress() >= communityGoal.GetGoal())
		{
			OnCommunityGoalReached(goalID, communityGoal);
		}

		//! Update players community goal data
		if (player)
			UpdateCommunityGoalPlayerData(player.GetIdentity().GetId(), player.GetIdentity().GetName(), goalID, progress);
	}

	bool GetCommunityGoalState(int goalID)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		ExpansionCommunityGoal communityGoal;
		if (!m_CommunityGoals.Find(goalID, communityGoal))
			return false;

		return communityGoal.IsFinished();
	}

	protected void OnCommunityGoalReached(int id, inout ExpansionCommunityGoal communityGoal)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		MissionBaseWorld.Cast(GetGame().GetMission()).Expansion_OnCommunityGoalReached(id, communityGoal);

		if (!communityGoal.IsFinished())
		{
			//! Set goal to finished
			communityGoal.SetFinished();

			//! Update persistent community goal data.
			m_ServerData.SetCommunityGoal(communityGoal);

			//! Update module community goal data.
			m_CommunityGoals.Set(id, communityGoal);
		}
	}

	protected void SpawnCommunityGoalBoards()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		//! Community boards
		array<ref ExpansionCommunityGoalsBoard> communityGoalsBoards = m_ServerData.GetCommunityGoalsBoards();
		foreach (ExpansionCommunityGoalsBoard communityGoalsBoard: communityGoalsBoards)
		{
			Object obj = GetGame().CreateObjectEx(communityGoalsBoard.ClassName, communityGoalsBoard.Position, ECE_UPDATEPATHGRAPH | ECE_CREATEPHYSICS | ECE_NOLIFETIME);
			Expansion_CommunityGoals_Board goalsBoardObj = Expansion_CommunityGoals_Board.Cast(obj);
			if (!goalsBoardObj)
			{
				Error(ToString() + "::SpawnCommunityGoalBoards - Could not spawn community goal board object!");
				GetGame().ObjectDelete(obj);
				return;
			}

			goalsBoardObj.SetPosition(communityGoalsBoard.Position);
			goalsBoardObj.SetOrientation(communityGoalsBoard.Orientation);
			goalsBoardObj.SetFactionID(communityGoalsBoard.FactionID);
			goalsBoardObj.Update();
		}
	}

	override int GetRPCMin()
	{
		return ExpansionCommunityGoalsModuleRPC.INVALID;
	}

	override int GetRPCMax()
	{
		return ExpansionCommunityGoalsModuleRPC.COUNT;
	}

	override void OnRPC(Class sender, CF_EventArgs args)
	{
		auto trace = EXTrace.Start(EXTrace.P2PMARKET, this);

		super.OnRPC(sender, args);
		auto rpc = CF_EventRPCArgs.Cast(args);

		switch (rpc.ID)
		{
			case ExpansionCommunityGoalsModuleRPC.SendCommunityGoalData:
			{
				RPC_SendCommunityGoalData(rpc.Context, rpc.Sender, rpc.Target);
				break;
			}
			case ExpansionCommunityGoalsModuleRPC.RequestCommunityGoalDetails:
			{
				RPC_RequestCommunityGoalDetails(rpc.Context, rpc.Sender, rpc.Target);
				break;
			}
			case ExpansionCommunityGoalsModuleRPC.SendCommunityGoalDetails:
			{
				RPC_SendCommunityGoalDetails(rpc.Context, rpc.Sender, rpc.Target);
				break;
			}
		}
	}

	void SendCommunityGoalData(Object target, PlayerIdentity identity)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
		{
			Error(ToString() + "::SendCommunityGoalData - Tried to call SendWorkbenchData on Client!");
			return;
		}

		Expansion_CommunityGoals_Board community_goal_board;
		if (!Class.CastTo(community_goal_board, target))
		{
			Error(ToString() + "::SendCommunityGoalData - Invalid target for community goal board object!");
			return;
		}

		int factionID = community_goal_board.GetFactionID();
		if (factionID == 0)
		{
			Error(ToString() + "::SendCommunityGoalData - Invalid faction ID");
			return;
		}

		array<ExpansionCommunityGoal> goalsToSend = new array<ExpansionCommunityGoal>;
		int goalsCount;
		foreach (int id, ExpansionCommunityGoal goal: m_CommunityGoals)
		{
			if (goal.GetFactionID() == factionID)
			{
				goalsToSend.Insert(goal);
				goalsCount++;
			}
		}

		auto rpc = ExpansionScriptRPC.Create();
		rpc.Write(goalsCount);

		foreach (ExpansionCommunityGoal goalToSend: goalsToSend)
		{
			goalToSend.OnSend(rpc);
		}

		rpc.Send(null, ExpansionCommunityGoalsModuleRPC.SendCommunityGoalData, true, identity);
	}

	protected void RPC_SendCommunityGoalData(ParamsReadContext ctx, PlayerIdentity senderRPC, Object target)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (!ExpansionScriptRPC.CheckMagicNumber(ctx))
			return;

		if (!GetGame().IsClient())
		{
			Error(ToString() + "::RPC_SendCommunityGoalData - Tried to call RPC_SendCommunityGoalData on Server!");
			return;
		}

		int goalsCount;
		if (!ctx.Read(goalsCount))
		{
			Error(ToString() + "::RPC_SendWorkbenchData - Couldn't read goalsCount");
			return;
		}

		array<ref ExpansionCommunityGoal> goalsToSend = new array<ref ExpansionCommunityGoal>;
		for (int i = 0; i < goalsCount; i++)
		{
			ExpansionCommunityGoal recivedGoal = new ExpansionCommunityGoal();
			if (!recivedGoal.OnRecieve(ctx))
			{
				Error(ToString() + "::RPC_SendWorkbenchData - Couldn't read community goal!");
				return;
			}

			goalsToSend.Insert(recivedGoal);
		}

		m_CommunityGoalsMenuInvoker.Invoke(goalsToSend);
	}

	void RequestCommunityGoalDetails(int goalID)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		ModuleDebugPrint("::RequestCommunityGoalDetails - Request community goal details for goal with ID: " + goalID);

		if (!GetGame().IsClient())
		{
			Error(ToString() + "::RequestCommunityGoalDetails - Tried to call RequestCommunityGoalDetails on Server!");
			return;
		}

		auto rpc = ExpansionScriptRPC.Create();
		rpc.Write(goalID);
		rpc.Send(null, ExpansionCommunityGoalsModuleRPC.RequestCommunityGoalDetails, true, null);
	}

	protected void RPC_RequestCommunityGoalDetails(ParamsReadContext ctx, PlayerIdentity senderRPC, Object target)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (!ExpansionScriptRPC.CheckMagicNumber(ctx))
			return;

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
		{
			Error(ToString() + "::RPC_RequestCommunityGoalDetails - Tried to call RPC_RequestCommunityGoalDetails on Client!");
			return;
		}

		int goalID;
		if (!ctx.Read(goalID))
		{
			Error(ToString() + "::RPC_RequestCommunityGoalDetails - Couldn't read goalID");
			return;
		}

		array<ref ExpansionCommunityGoalPlayerData> validContributors = GetCommunityGoalContributors(goalID);

		ModuleDebugPrint("::RPC_RequestCommunityGoalDetails - Valid contributors: " + validContributors.Count() + " | Goal ID: " + goalID);

		auto rpc = ExpansionScriptRPC.Create();
		rpc.Write(validContributors.Count());

		foreach (ExpansionCommunityGoalPlayerData contributor: validContributors)
		{
			contributor.OnSend(rpc);
		}

		rpc.Send(null, ExpansionCommunityGoalsModuleRPC.SendCommunityGoalDetails, true, senderRPC);
	}

	protected void RPC_SendCommunityGoalDetails(ParamsReadContext ctx, PlayerIdentity senderRPC, Object target)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (!ExpansionScriptRPC.CheckMagicNumber(ctx))
			return;

		if (!GetGame().IsClient())
		{
			Error(ToString() + "::RPC_SendCommunityGoalDetails - Tried to call RPC_SendCommunityGoalDetails on Server!");
			return;
		}

		int contributorCount;
		if (!ctx.Read(contributorCount))
		{
			Error(ToString() + "::RPC_SendCommunityGoalDetails - Couldn't read contributorCount");
			return;
		}

		array<ref ExpansionCommunityGoalPlayerData> contributors = new array<ref ExpansionCommunityGoalPlayerData>;
		for (int i = 0; i < contributorCount; i++)
		{
			ExpansionCommunityGoalPlayerData contributer = new ExpansionCommunityGoalPlayerData();
			if (!contributer.OnRecieve(ctx))
			{
				Error(ToString() + "::RPC_SendCommunityGoalDetails - Couldn't read community goal player data!");
				return;
			}

			contributors.Insert(contributer);
		}

		m_CommunityGoalsMenuDetailsInvoker.Invoke(contributors);
	}

	ScriptInvoker GetCommunityGoalsMenuSI()
	{
		return m_CommunityGoalsMenuInvoker;
	}

	ScriptInvoker GetCommunityGoalsMenuDetailsSI()
	{
		return m_CommunityGoalsMenuDetailsInvoker;
	}

	static ExpansionCommunityGoalsModule GetModuleInstance()
	{
		return s_ModuleInstance;
	}

	void ModuleDebugPrint(string text)
	{
		#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		EXTrace.Print(EXTrace.NAMALSKADVENTURE, this, text);
		#endif
	}
};