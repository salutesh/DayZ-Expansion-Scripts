/**
 * ExpansionQuestNpcData.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestNpcData
{
	private int ID;
	private string ClassName;
	private bool IsAI = false;
	private vector Position;
	private vector Orientation;
	private ref TStringArray Gear = new TStringArray;
	private ref array<int> QuestIDs = new array<int>;
	private string NPCName = "Unknown";
	private string DefaultNPCText = "What do you want? Leave me alown..!";

#ifdef EXPANSIONMODAI
	private ref array<vector> Waypoints = new array<vector>;
	private int NPCEmoteID = EmoteConstants.ID_EMOTE_WATCHING;
#endif

	void SetID(int id)
	{
		ID = id;
	}

	int GetID()
	{
		return ID;
	}

	void SetName(string className)
	{
		ClassName = className;
	}

	string GetName()
	{
		return ClassName;
	}

	void SetIsAI(bool state)
	{
		IsAI = state;
	}

	bool IsAI()
	{
		return IsAI;
	}

	void SetQuestIDs(array<int> questIDs)
	{
		QuestIDs.InsertAll(questIDs);
	}

	array<int> GetQuestIDs()
	{
		return QuestIDs;
	}

	void SetPosition(vector pos)
	{
		Position = pos;
	}

	vector GetPosition()
	{
		return Position;
	}

	void SetOrientation(vector orientation)
	{
		Orientation = orientation;
	}

	vector GetOrientation()
	{
		return Orientation;
	}

	void SetGear(TStringArray gear)
	{
		Gear.InsertAll(gear);
	}

	TStringArray GetGear()
	{
		return Gear;
	}

	void SetNPCName(string name)
	{
		NPCName = name;
	}

	string GetNPCName()
	{
		return NPCName;
	}

	void SetDefaultNPCText(string text)
	{
		DefaultNPCText = text;
	}

	string GetDefaultNPCText()
	{
		return DefaultNPCText;
	}

#ifdef EXPANSIONMODAI
	void AddWaypoint(vector waypoint)
	{
		Waypoints.Insert(waypoint);
	}

	array<vector> GetWaypoints()
	{
		return Waypoints;
	}

	void SetEmoteID(int emoteID)
	{
		NPCEmoteID = emoteID;
	}

	int GetEmoteID()
	{
		return NPCEmoteID;
	}
#endif

	ExpansionQuestNpcBase SpawnNPC()
	{
		Object obj = GetGame().CreateObjectEx(GetName(), GetPosition(), ECE_INITAI | ECE_CREATEPHYSICS | ECE_ROTATIONFLAGS | ECE_PLACE_ON_SURFACE);

		if (Position)
			obj.SetPosition(Position);

		if (Orientation)
			obj.SetOrientation(Orientation);

		EntityAI enity = EntityAI.Cast(obj);
		ExpansionQuestNpcBase questNPC = ExpansionQuestNpcBase.Cast(enity);

		for (int i = 0; i < GetGear().Count(); i++)
		{
			array<string> items = new array<string>;
			GetGear()[i].Split("+", items);
			EntityAI itemEnt = NULL;
			//! Spawn weapon in hands
			if (questNPC.IsInherited(Man) && GetGame().ConfigIsExisting("CfgVehicles " + items[0] + " suicideAnim") || GetGame().IsKindOf(items[0], "Rifle_Base") || GetGame().IsKindOf(items[0], "Pistol_Base") || GetGame().IsKindOf(items[0], "Archery_Base") || GetGame().IsKindOf(items[0], "Launcher_Base"))
				itemEnt = questNPC.GetHumanInventory().CreateInHands(items[0]);
			//! Spawn everything else in inventory
			if (!itemEnt)
				itemEnt = questNPC.GetInventory().CreateInInventory(items[0]);
			ItemBase itemBase = ItemBase.Cast(itemEnt);
			if (itemEnt)
			{
				for (int j = 1; j < items.Count(); j++)
				{
					if (itemBase)
						itemBase.ExpansionCreateInInventory(items[j]);
					else
						itemEnt.GetInventory().CreateInInventory(items[j]);
				}
			}
		}

		if (Position)
			questNPC.SetPosition(Position);

		if (Orientation)
			questNPC.SetOrientation(Orientation);

		return questNPC;
	}

#ifdef EXPANSIONMODAI
	ExpansionQuestNpcAIBase SpawnNPCAI()
	{
		Object obj = GetGame().CreateObjectEx(GetName(), GetPosition(), ECE_INITAI | ECE_CREATEPHYSICS | ECE_ROTATIONFLAGS | ECE_PLACE_ON_SURFACE);

		if (Position)
			obj.SetPosition(Position);

		if (Orientation)
			obj.SetOrientation(Orientation);

		EntityAI enity = EntityAI.Cast(obj);
		ExpansionQuestNpcAIBase questNPC = ExpansionQuestNpcAIBase.Cast(enity);

		for (int i = 0; i < GetGear().Count(); i++)
		{
			array<string> items = new array<string>;
			GetGear()[i].Split("+", items);
			EntityAI itemEnt = NULL;
			//! Spawn weapon in hands
			if (questNPC.IsInherited(Man) && GetGame().ConfigIsExisting("CfgVehicles " + items[0] + " suicideAnim") || GetGame().IsKindOf(items[0], "Rifle_Base") || GetGame().IsKindOf(items[0], "Pistol_Base") || GetGame().IsKindOf(items[0], "Archery_Base") || GetGame().IsKindOf(items[0], "Launcher_Base"))
				itemEnt = questNPC.GetHumanInventory().CreateInHands(items[0]);
			//! Spawn everything else in inventory
			if (!itemEnt)
				itemEnt = questNPC.GetInventory().CreateInInventory(items[0]);
			ItemBase itemBase = ItemBase.Cast(itemEnt);
			if (itemEnt)
			{
				for (int j = 1; j < items.Count(); j++)
				{
					if (itemBase)
						itemBase.ExpansionCreateInInventory(items[j]);
					else
						itemEnt.GetInventory().CreateInInventory(items[j]);
				}
			}
		}
		
		questNPC.SetPosition(Position);
		questNPC.SetOrientation(Orientation);
		vector roll = questNPC.GetOrientation();
		roll [ 2 ] = roll [ 2 ] - 1;
		questNPC.SetOrientation(roll);
		roll [ 2 ] = roll [ 2 ] + 1;
		questNPC.SetOrientation(roll);

		if (questNPC.CanAffectPathgraph())
		{
			questNPC.SetAffectPathgraph(true, false);
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(GetGame().UpdatePathgraphRegionByObject, 100, false, questNPC);
		}
		
		#ifdef EXPANSIONMODAI
		eAIGroup ownerGrp = questNPC.GetGroup();
		#else
		if (eAIGlobal_HeadlessClient)
			GetRPCManager().SendRPC("eAI", "HCLinkObject", new Param1< PlayerBase >(questNPC), false, eAIGlobal_HeadlessClient);

		eAIGame game = MissionServer.Cast(GetGame().GetMission()).GetEAIGame();
		eAIGroup ownerGrp = game.GetGroupByLeader(questNPC);
		#endif
		ownerGrp.SetFaction(new eAIFactionCivilian());
		for (j = 0; j < Waypoints.Count(); j++)
		{
			EXPrint("Adding waypoint " + Waypoints[j]);
			ownerGrp.AddWaypoint(Waypoints[j]);
		}

		#ifdef EXPANSIONMODAI
		if (Waypoints.Count() > 1)
			ownerGrp.SetWaypointBehaviour(eAIWaypointBehavior.ALTERNATE);
		else if (Waypoints.Count() <= 1)
			ownerGrp.SetWaypointBehaviour(eAIWaypointBehavior.HALT);
		#else
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(questNPC.RequestTransition, 10000, false, "Rejoin");

		questNPC.SetAI(ownerGrp);
		#endif

		return questNPC;
	}
#endif

	void Save(string fileName)
	{
		JsonFileLoader<ExpansionQuestNpcData>.JsonSaveFile(EXPANSION_QUESTS_NPCS_FOLDER + fileName + ".JSON", this);
	}

	void OnSend(ParamsWriteContext ctx)
	{
		ctx.Write(ID);
		ctx.Write(ClassName);
		ctx.Write(NPCName);
		ctx.Write(DefaultNPCText);
	}

	bool OnRecieve(ParamsReadContext ctx)
	{
		if (!ctx.Read(ID))
		{
			Error(ToString() + "::OnRecieve - ID");
			return false;
		}

		if (!ctx.Read(ClassName))
		{
			Error(ToString() + "::OnRecieve - ClassName");
			return false;
		}

		if (!ctx.Read(NPCName))
		{
			Error(ToString() + "::OnRecieve - NPCName");
			return false;
		}

		if (!ctx.Read(DefaultNPCText))
		{
			Error(ToString() + "::OnRecieve - DefaultNPCText");
			return false;
		}

		return true;
	}

	void QuestDebug()
	{
	#ifdef EXPANSIONMODQUESTSMODULEDEBUG
		Print("------------------------------------------------------------");
		Print(ToString() + "::QuestDebug - ID: " + ID);
		Print(ToString() + "::QuestDebug - ClassName: " + ClassName);
		Print(ToString() + "::QuestDebug - NPCName: " + NPCName);
		Print(ToString() + "::QuestDebug - DefaultNPCText: " + DefaultNPCText);
		Print("------------------------------------------------------------");
	#endif
	}
};