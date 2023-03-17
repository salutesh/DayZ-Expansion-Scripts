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
	string NPCName = "Unknown";
	string DefaultNPCText = "What do you want? Leave me alone!";

#ifdef EXPANSIONMODAI
	ref array<vector> Waypoints = new array<vector>;
	int NPCEmoteID = EmoteConstants.ID_EMOTE_WATCHING;
	bool NPCEmoteIsStatic;
#endif
	string NPCLoadoutFile;
	bool IsStatic;
	int NPCInteractionEmoteID = EmoteConstants.ID_EMOTE_GREETING;
	int NPCQuestCancelEmoteID = EmoteConstants.ID_EMOTE_SHRUG;
	int NPCQuestStartEmoteID = EmoteConstants.ID_EMOTE_NOD;
	int NPCQuestCompleteEmoteID = EmoteConstants.ID_EMOTE_CLAP;
}

class ExpansionQuestNPCData: ExpansionQuestNPCDataBase
{
	static const int CONFIGVERSION = 4;

#ifdef EXPANSIONMODAI
	string NPCFaction = "InvincibleObservers";
#endif

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
		Position = ExpansionAIPatrol.GetPlacementPosition(Position);
		
		Object obj = GetGame().CreateObject(ClassName, Position);
		if (!obj)
			return NULL;

		ExpansionQuestNPCAIBase questNPC = ExpansionQuestNPCAIBase.Cast(obj);
		if (!questNPC)
		{
			GetGame().ObjectDelete(obj);
			return NULL;
		}

		questNPC.SetPosition(Position);
		questNPC.SetOrientation(Orientation);
		questNPC.Update();
		questNPC.m_Expansion_NetsyncData.Set(0, NPCName);
		ExpansionHumanLoadout.Apply(questNPC, NPCLoadoutFile, false);
		questNPC.Expansion_SetCanBeLooted(false);
		questNPC.eAI_SetUnlimitedReload(true);
		questNPC.eAI_SetAccuracy(1.0, 1.0);
		questNPC.eAI_SetThreatDistanceLimit(800);

		eAIGroup aiGroup = questNPC.GetGroup();

		if (NPCFaction != string.Empty)
		{
			eAIFaction faction = eAIFaction.Create(NPCFaction);
			if (faction && aiGroup.GetFaction().Type() != faction.Type())
				aiGroup.SetFaction(faction);
		}
		
		aiGroup.SetFormation(new eAIFormationColumn(aiGroup));
		aiGroup.SetWaypointBehaviour(eAIWaypointBehavior.ALTERNATE);

		for (int idx = 0; idx < Waypoints.Count(); idx++)
		{
			aiGroup.AddWaypoint(Waypoints[idx]);
			if (Waypoints[idx] == Position)
				aiGroup.m_CurrentWaypointIndex = idx;
		}
		
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
		NPCName = npcDataBase.NPCName;
		DefaultNPCText = npcDataBase.DefaultNPCText;

	#ifdef EXPANSIONMODAI
		Waypoints = npcDataBase.Waypoints;
		NPCEmoteID = npcDataBase.NPCEmoteID;
		NPCEmoteIsStatic = npcDataBase.NPCEmoteIsStatic;
	#endif
		NPCLoadoutFile = npcDataBase.NPCLoadoutFile;
		IsStatic = npcDataBase.IsStatic;
		
		NPCInteractionEmoteID = npcDataBase.NPCInteractionEmoteID;
		NPCQuestCancelEmoteID = npcDataBase.NPCQuestCancelEmoteID;
		NPCQuestStartEmoteID = npcDataBase.NPCQuestStartEmoteID;
		NPCQuestCompleteEmoteID = npcDataBase.NPCQuestCompleteEmoteID;
	}
	
	void OnSend(ParamsWriteContext ctx)
	{
		ctx.Write(ID);
		ctx.Write(NPCName);
	}
	
	bool OnRecieve(ParamsReadContext ctx)
	{
		if (!ctx.Read(ID))
			return false;
		
		if (!ctx.Read(NPCName))
			return false;
		
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