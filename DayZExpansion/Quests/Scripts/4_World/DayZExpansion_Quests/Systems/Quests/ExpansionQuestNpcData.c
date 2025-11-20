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

enum ExpansionQuestNPCType
{
	NORMAL,
	OBJECT,
	AI
};

class ExpansionQuestNPCData_V4
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
};

class ExpansionQuestNPCDataBase
{
	int ConfigVersion;
	int ID;
	string ClassName;
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
	int NPCInteractionEmoteID = EmoteConstants.ID_EMOTE_GREETING;
	int NPCQuestCancelEmoteID = EmoteConstants.ID_EMOTE_SHRUG;
	int NPCQuestStartEmoteID = EmoteConstants.ID_EMOTE_NOD;
	int NPCQuestCompleteEmoteID = EmoteConstants.ID_EMOTE_CLAP;
	
	#ifdef EXPANSIONMODAI
	string NPCFaction = "InvincibleObservers";
	#endif
	int NPCType = ExpansionQuestNPCType.NORMAL;
};

class ExpansionQuestNPCData: ExpansionQuestNPCDataBase
{
	[NonSerialized()]
	static const int CONFIGVERSION = 6;
	
	[NonSerialized()]
	static string FILENAME = "NONE";
	
	bool Active = true;
	
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
	
	string GetClassName()
	{
		return ClassName;
	}

	void SetNPCType(ExpansionQuestNPCType type)
	{
		NPCType = type;
	}

	ExpansionQuestNPCType GetNPCType()
	{
		return NPCType;
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
	
	void SetFaction(string faction)
	{
		NPCFaction = faction;
	}
	
	string GetFaction()
	{
		return NPCFaction;
	}
#endif
	
	bool IsActive()
	{
		return Active;
	}

	ExpansionQuestNPCBase SpawnNPC()
	{
		int flags = ECE_ROTATIONFLAGS | ECE_PLACE_ON_SURFACE;

		if (g_Game.IsKindOf(ClassName, "DZ_LightAI"))  //! Only true for animals and Zs
			flags |= ECE_INITAI;

		Object obj = ExpansionGame.CreateObjectExSafe(ClassName, Position, flags);
 		ExpansionQuestNPCBase questNPC;
		if (!ExpansionQuestNPCBase.CastTo(questNPC, obj))
	    {
			Error("ExpansionQuestNPCDataBase::SpawnNPC - Used unsupported object " + ClassName + " as quest NPC in config. Only objects based on ExpansionQuestNPCBase class are allowed!");
			g_Game.ObjectDelete(obj);
	        return null;
	    }

		if (Position)
	        questNPC.SetPosition(Position);

	    if (Orientation)
	        questNPC.SetOrientation(Orientation);

		questNPC.Update();
		if (!questNPC.m_Expansion_NetsyncData)
			questNPC.m_Expansion_NetsyncData = new ExpansionNetsyncData(questNPC);
	#ifdef DIAG_DEVELOPER
		NPCName = NPCName + " | ID: " + ID;
	#endif
		questNPC.m_Expansion_NetsyncData.Set(0, NPCName);
		ExpansionHumanLoadout.Apply(questNPC, GetLoadoutFile(), false);
		
	#ifdef EXPANSIONMODAI
		eAIGroup aiGroup = questNPC.GetGroup();
		if (NPCFaction != string.Empty)
		{
			eAIFaction faction = eAIFaction.Create(NPCFaction);
			if (faction && aiGroup.GetFaction().Type() != faction.Type())
				aiGroup.SetFaction(faction);
		}
	#endif

		return questNPC;
	}

#ifdef EXPANSIONMODAI
	ExpansionQuestNPCAIBase SpawnNPCAI()
	{
		Position = ExpansionAIPatrol.GetPlacementPosition(Position);
		
		Object obj = ExpansionGame.CreateObjectSafe(ClassName, Position);
		if (!obj)
			return NULL;

		ExpansionQuestNPCAIBase questNPC = ExpansionQuestNPCAIBase.Cast(obj);
		if (!questNPC)
		{
			g_Game.ObjectDelete(obj);
			return NULL;
		}

		questNPC.SetPosition(Position);
		questNPC.SetOrientation(Orientation);
		questNPC.Update();
	#ifdef DIAG_DEVELOPER
		NPCName = NPCName + " | ID: " + ID;
	#endif
		if (!questNPC.m_Expansion_NetsyncData)
			questNPC.m_Expansion_NetsyncData = new ExpansionNetsyncData(questNPC);
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
	
		if (!Waypoints.Count())
			Waypoints.Insert(Position);

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
	    Object obj = ExpansionGame.CreateObjectExSafe(ClassName, Position, ECE_ROTATIONFLAGS | ECE_PLACE_ON_SURFACE);
	    ExpansionQuestStaticObject questObject;
	    if (!ExpansionQuestStaticObject.CastTo(questObject, obj))
	    {
			Error("ExpansionQuestNPCDataBase::SpawnNPC - Used unsupported object " + ClassName + " as AI quest NPC in config. Only objects based on ExpansionQuestStaticObject class are allowed!");
			g_Game.ObjectDelete(obj);
	        return null;
	    }

	    if (Position)
	        questObject.SetPosition(Position);

	    if (Orientation)
	        questObject.SetOrientation(Orientation);

	#ifdef DIAG_DEVELOPER
		NPCName = NPCName + " | ID: " + ID;
	#endif
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
			
			if (npcConfigBase.ConfigVersion < 5)
			{
				ExpansionQuestNPCData_V4 npcConfigBaseV4;
				if (ExpansionJsonFileParser<ExpansionQuestNPCData_V4>.Load(EXPANSION_QUESTS_NPCS_FOLDER + fileName, npcConfigBaseV4))
				{
					npcConfig.NPCType = ExpansionQuestNPCType.NORMAL;
					if (npcConfigBaseV4.IsAI)
						npcConfig.NPCType = ExpansionQuestNPCType.AI;
					
					if (npcConfigBaseV4.IsStatic)
						npcConfig.NPCType = ExpansionQuestNPCType.OBJECT;
				}				
			}
			
			if (npcConfigBase.ConfigVersion < 6)
			{
				npcConfig.Active = true;
			}
			
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
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this, EXPANSION_QUESTS_NPCS_FOLDER + fileName);
#endif

		if (!ExpansionString.EndsWithIgnoreCase(fileName, ".json"))
			fileName += ".json";

		ExpansionJsonFileParser<ExpansionQuestNPCData>.Save(EXPANSION_QUESTS_NPCS_FOLDER + fileName, this);
	}

	void CopyConfig(ExpansionQuestNPCDataBase npcDataBase)
	{
		ID = npcDataBase.ID;
		ClassName = npcDataBase.ClassName;
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
		
		NPCInteractionEmoteID = npcDataBase.NPCInteractionEmoteID;
		NPCQuestCancelEmoteID = npcDataBase.NPCQuestCancelEmoteID;
		NPCQuestStartEmoteID = npcDataBase.NPCQuestStartEmoteID;
		NPCQuestCompleteEmoteID = npcDataBase.NPCQuestCompleteEmoteID;

	#ifdef EXPANSIONMODAI
		NPCFaction = npcDataBase.NPCFaction;
	#endif
		NPCType = npcDataBase.NPCType;
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