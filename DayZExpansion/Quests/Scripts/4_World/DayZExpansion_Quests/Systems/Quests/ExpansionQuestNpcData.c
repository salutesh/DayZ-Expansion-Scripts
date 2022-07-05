/**
 * ExpansionQuestNPCData.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestNPCDataBase
{
	int ConfigVersion;
	int ID;
	string ClassName;
	bool IsAI;
	vector Position;
	vector Orientation;
	ref array<int> QuestIDs = new array<int>;
	string NPCName = "Unknown";
	string DefaultNPCText = "What do you want? Leave me alone!";

#ifdef EXPANSIONMODAI
	ref array<vector> Waypoints = new array<vector>;
	int NPCEmoteID = EmoteConstants.ID_EMOTE_WATCHING;
#endif
}

class ExpansionQuestNPCData: ExpansionQuestNPCDataBase
{
	static const int CONFIGVERSION = 1;
	
#ifdef EXPANSIONMODAI
	bool NPCEmoteIsStatic;
#endif
	string NPCLoadoutFile;
	bool IsStatic;
	
	void ExpansionQuestNPCData()
	{
		ConfigVersion = 1;
	}
	
	void SetID(int id)
	{
		ID = id;
	}

	int GetID()
	{
		return ID;
	}

	void SetClassName(string className)
	{
		ClassName = className;
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

	void SetLoadoutFile(string fileName)
	{
		NPCLoadoutFile = fileName;
	}
	
	string GetLoadoutFile()
	{
		return NPCLoadoutFile;
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
	void SetIsEmoteStatic(bool state)
	{
		NPCEmoteIsStatic = state;
	}
	
	bool IsEmoteStatic()
	{
		return NPCEmoteIsStatic;
	}
	
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
	
	void SetIsStatic(bool state)
	{
		IsStatic = state;
	}
	
	bool IsStatic()
	{
		return IsStatic;
	}

	ExpansionQuestNPCBase SpawnNPC()
	{
		Object obj = GetGame().CreateObjectEx(ClassName, GetPosition(), ECE_INITAI | ECE_CREATEPHYSICS | ECE_ROTATIONFLAGS | ECE_PLACE_ON_SURFACE);

		if (Position)
			obj.SetPosition(Position);

		if (Orientation)
			obj.SetOrientation(Orientation);

		EntityAI enity = EntityAI.Cast(obj);
		ExpansionQuestNPCBase questNPC = ExpansionQuestNPCBase.Cast(enity);
		questNPC.m_Expansion_NetsyncData.Set(0, NPCName);
		ExpansionHumanLoadout.Apply(questNPC, GetLoadoutFile(), false);

		if (Position)
			questNPC.SetPosition(Position);

		if (Orientation)
			questNPC.SetOrientation(Orientation);

		return questNPC;
	}

#ifdef EXPANSIONMODAI
	ExpansionQuestNPCAIBase SpawnNPCAI()
	{
		Object obj = GetGame().CreateObjectEx(ClassName, GetPosition(), ECE_INITAI | ECE_CREATEPHYSICS | ECE_ROTATIONFLAGS | ECE_PLACE_ON_SURFACE);

		if (Position)
			obj.SetPosition(Position);

		if (Orientation)
			obj.SetOrientation(Orientation);

		EntityAI enity = EntityAI.Cast(obj);
		ExpansionQuestNPCAIBase questNPC = ExpansionQuestNPCAIBase.Cast(enity);
		questNPC.m_Expansion_NetsyncData.Set(0, NPCName);
		ExpansionHumanLoadout.Apply(questNPC, GetLoadoutFile(), false);

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

		eAIGroup ownerGrp = questNPC.GetGroup();
		for (int j = 0; j < Waypoints.Count(); j++)
		{
			EXPrint("Adding waypoint " + Waypoints[j]);
			ownerGrp.AddWaypoint(Waypoints[j]);
		}

		if (Waypoints.Count() > 1)
			ownerGrp.SetWaypointBehaviour(eAIWaypointBehavior.ALTERNATE);
		else if (Waypoints.Count() <= 1)
			ownerGrp.SetWaypointBehaviour(eAIWaypointBehavior.HALT);

		return questNPC;
	}
#endif

	ExpansionQuestStaticObject SpawnObject()
	{
		Object obj = GetGame().CreateObjectEx(ClassName, GetPosition(), ECE_ROTATIONFLAGS | ECE_PLACE_ON_SURFACE);
		
		if (Position)
			obj.SetPosition(Position);

		if (Orientation)
			obj.SetOrientation(Orientation);
		
		EntityAI enity = EntityAI.Cast(obj);
		ExpansionQuestStaticObject questObject = ExpansionQuestStaticObject.Cast(enity);
		questObject.SetPosition(Position);
		questObject.SetOrientation(Orientation);
		questObject.m_Expansion_NetsyncData.Set(0, NPCName);
		
		return questObject;
	}
	
	static ExpansionQuestNPCData Load(string fileName)
	{
		bool save;
		CF_Log.Info("[ExpansionQuestNPCData] Load existing configuration file:" + fileName);

		ExpansionQuestNPCData npcConfig;
		ExpansionQuestNPCDataBase npcConfigBase;
		JsonFileLoader<ExpansionQuestNPCDataBase>.JsonLoadFile(EXPANSION_QUESTS_NPCS_FOLDER + fileName, npcConfigBase);
		
		if (npcConfigBase.ConfigVersion < CONFIGVERSION)
		{
			CF_Log.Info("[ExpansionQuestNPCData] Convert existing configuration file:" + fileName + " to version " + CONFIGVERSION);
			npcConfig = new ExpansionQuestNPCData();			
			//! Copy over old configuration that haven't changed
			npcConfig.CopyConfig(npcConfigBase);
			npcConfig.ConfigVersion = CONFIGVERSION;
			save = true;
		}
		else
		{
			JsonFileLoader<ExpansionQuestNPCData>.JsonLoadFile(EXPANSION_QUESTS_NPCS_FOLDER + fileName, npcConfig);
		}
		
		if (save)
		{
			JsonFileLoader<ExpansionQuestNPCData>.JsonSaveFile(EXPANSION_QUESTS_NPCS_FOLDER + fileName, npcConfig);
		}
		
		return npcConfig;
	}

	void Save(string fileName)
	{
		JsonFileLoader<ExpansionQuestNPCData>.JsonSaveFile(EXPANSION_QUESTS_NPCS_FOLDER + fileName + ".JSON", this);
	}
	
	void CopyConfig(ExpansionQuestNPCDataBase npcDataBase)
	{
		ID = npcDataBase.ID;
		ClassName = npcDataBase.ClassName;
		IsAI = npcDataBase.IsAI;
		Position = npcDataBase.Position;
		Orientation = npcDataBase.Orientation;
		QuestIDs = npcDataBase.QuestIDs;
		NPCName = npcDataBase.NPCName;
		DefaultNPCText = npcDataBase.DefaultNPCText; 
	
	#ifdef EXPANSIONMODAI
		Waypoints = npcDataBase.Waypoints;
		NPCEmoteID = npcDataBase.NPCEmoteID;
	#endif
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