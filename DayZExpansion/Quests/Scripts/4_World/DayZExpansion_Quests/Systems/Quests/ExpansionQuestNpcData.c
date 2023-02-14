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
	bool NPCEmoteIsStatic;
#endif
	string NPCLoadoutFile;
	bool IsStatic;
}

class ExpansionQuestNPCData: ExpansionQuestNPCDataBase
{
	static const int CONFIGVERSION = 2;

	int NPCInteractionEmoteID = EmoteConstants.ID_EMOTE_GREETING;
	int NPCQuestCancelEmoteID = EmoteConstants.ID_EMOTE_SHRUG;
	int NPCQuestStartEmoteID = EmoteConstants.ID_EMOTE_NOD;
	int NPCQuestCompleteEmoteID = EmoteConstants.ID_EMOTE_CLAP;

	void ExpansionQuestNPCData()
	{
		ConfigVersion = CONFIGVERSION;
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

	void AddQuestID(int questID)
	{
		if (QuestIDs.Find(questID) == -1)
		{
			QuestIDs.Insert(questID);
		}
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
		Object obj = GetGame().CreateObjectEx(ClassName, Position, ECE_INITAI | ECE_CREATEPHYSICS | ECE_ROTATIONFLAGS | ECE_PLACE_ON_SURFACE);
 		ExpansionQuestNPCBase questNPC;
		if (!ExpansionQuestNPCBase.CastTo(questNPC, obj))
	    {
			Error("ExpansionQuestNPCDataBase::SpawnNPC - Used unsupported object " + ClassName + " as quest NPC in config. Only objects based on ExpansionQuestNPCBase class are allowed!");
			GetGame().ObjectDelete(obj);
	        return null;
	    }

		if (Position)
	        questNPC.SetPosition(Position);

	    if (Orientation)
	        questNPC.SetOrientation(Orientation);

		questNPC.Update();
		questNPC.m_Expansion_NetsyncData.Set(0, NPCName);
		ExpansionHumanLoadout.Apply(questNPC, GetLoadoutFile(), false);

		return questNPC;
	}

#ifdef EXPANSIONMODAI
	ExpansionQuestNPCAIBase SpawnNPCAI()
	{
		Object obj = GetGame().CreateObjectEx(ClassName, Position, ECE_INITAI | ECE_CREATEPHYSICS | ECE_ROTATIONFLAGS | ECE_PLACE_ON_SURFACE);
		ExpansionQuestNPCAIBase questNPC;
	    if (!ExpansionQuestNPCAIBase.CastTo(questNPC, obj))
	    {
			Error("ExpansionQuestNPCDataBase::SpawnNPC - Used unsupported object " + ClassName + " as AI quest NPC in config. Only objects based on ExpansionQuestNPCAIBase class are allowed!");
			GetGame().ObjectDelete(obj);
	        return null;
	    }

		if (Position)
	        questNPC.SetPosition(Position);

	    if (Orientation)
	        questNPC.SetOrientation(Orientation);

		questNPC.Update();
		questNPC.m_Expansion_NetsyncData.Set(0, NPCName);
		ExpansionHumanLoadout.Apply(questNPC, GetLoadoutFile(), false);

		eAIGroup ownerGrp = questNPC.GetGroup();
		foreach (vector point: Waypoints)
		{
			EXPrint("Adding waypoint " + point);
			ownerGrp.AddWaypoint(point);
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
	    Object obj = GetGame().CreateObjectEx(ClassName, Position, ECE_ROTATIONFLAGS | ECE_PLACE_ON_SURFACE);
	    ExpansionQuestStaticObject questObject;
	    if (!ExpansionQuestStaticObject.CastTo(questObject, obj))
	    {
			Error("ExpansionQuestNPCDataBase::SpawnNPC - Used unsupported object " + ClassName + " as AI quest NPC in config. Only objects based on ExpansionQuestStaticObject class are allowed!");
			GetGame().ObjectDelete(obj);
	        return null;
	    }

	    if (Position)
	        questObject.SetPosition(Position);

	    if (Orientation)
	        questObject.SetOrientation(Orientation);

	    questObject.m_Expansion_NetsyncData.Set(0, NPCName);

	    return questObject;
	}

	static ExpansionQuestNPCData Load(string fileName)
	{
		bool save;
		Print("[ExpansionQuestNPCData] Load existing configuration file:" + fileName);

		ExpansionQuestNPCData npcConfig;
		ExpansionQuestNPCDataBase npcConfigBase;

		if (!ExpansionJsonFileParser<ExpansionQuestNPCDataBase>.Load(EXPANSION_QUESTS_NPCS_FOLDER + fileName, npcConfigBase))
			return NULL;

		if (npcConfigBase.ConfigVersion < CONFIGVERSION)
		{
			Print("[ExpansionQuestNPCData] Convert existing configuration file:" + fileName + " to version " + CONFIGVERSION);
			npcConfig = new ExpansionQuestNPCData();
			
			//! Copy over old configuration that haven't changed
			npcConfig.CopyConfig(npcConfigBase);
			
			npcConfig.ConfigVersion = CONFIGVERSION;
			save = true;
		}
		else
		{
			if (!ExpansionJsonFileParser<ExpansionQuestNPCData>.Load(EXPANSION_QUESTS_NPCS_FOLDER + fileName, npcConfig))
				return NULL;
		}

		if (save)
			npcConfig.Save(fileName);

		return npcConfig;
	}

	void Save(string fileName)
	{
		if (!ExpansionString.EndsWithIgnoreCase(fileName, ".json"))
			fileName += ".json";

		ExpansionJsonFileParser<ExpansionQuestNPCData>.Save(EXPANSION_QUESTS_NPCS_FOLDER + fileName, this);
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
		NPCEmoteIsStatic = npcDataBase.NPCEmoteIsStatic;
	#endif
		NPCLoadoutFile = npcDataBase.NPCLoadoutFile;
		IsStatic = npcDataBase.IsStatic;
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