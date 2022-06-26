/**
 * ExpansionQuestObjectiveConfigBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestObjectiveConfigBasement
{
	int ConfigVersion;
	int ID = -1; //! Unique objective ID
	int ObjectiveType = ExpansionQuestObjectiveType.NONE; //! Quest obecjtive type.
	string ObjectiveText = string.Empty;
	int TimeLimit = - 1;
};

class ExpansionQuestObjectiveConfigBase: ExpansionQuestObjectiveConfigBasement
{
	[NonSerialized()]
	static int CONFIGVERSION = 3;
	
	void ExpansionQuestObjectiveConfigBase()
	{
		ConfigVersion = 3;
	}
	
	void SetID(int id)
	{
		ID = id;
	}

	int GetID()
	{
		return ID;
	}

	void SetObjectiveType(int type)
	{
		ObjectiveType = type;
	}

	int GetObjectiveType()
	{
		return ObjectiveType;
	}

	void SetObjectiveText(string text)
	{
		ObjectiveText = text;
	}

	string GetObjectiveText()
	{
		return ObjectiveText;
	}

	void SetTimeLimit(int time)
	{
		TimeLimit = time;
	}
	
	int GetTimeLimit()
	{
		return TimeLimit;
	}
	
	array<ref ExpansionQuestObjectiveDelivery> GetDeliveries()
	{
		return NULL;
	}

	int GetAmount()
	{
		return -1;
	}

	array<string> GetClassNames()
	{
		return NULL;
	}

	ExpansionQuestObjectiveTarget GetTarget()
	{
		return NULL;
	}

	vector GetPosition()
	{
		return vector.Zero;
	}

	float GetMaxDistance()
	{
		return -1;
	}

	string GetMarkerName()
	{
		return string.Empty;
	}
	
	array<string> GetActionNames()
	{
		return NULL;
	}

	ExpansionQuestObjectiveCollection GetCollection()
	{
		return NULL;
	}

	ExpansionQuestObjectiveTreasureHunt GetTreasureHunt()
	{
		return NULL;
	}

#ifdef EXPANSIONMODAI
	ExpansionQuestObjectiveAIPatrol GetAIPatrol()
	{
		return NULL;
	}

	ExpansionQuestObjectiveAICamp GetAICamp()
	{
		return NULL;
	}

	ExpansionQuestObjectiveAIVIP GetAIVIP()
	{
		return NULL;
	}
#endif

	static ExpansionQuestObjectiveConfigBase Load(string fileName)
	{
		CF_Log.Info("[ExpansionQuestObjectiveConfigBase] Load existing configuration file:" + fileName);

		ExpansionQuestObjectiveConfigBase npcConfig;
		ExpansionQuestObjectiveConfigBasement npcConfigBase;
		JsonFileLoader<ExpansionQuestObjectiveConfigBasement>.JsonLoadFile(fileName, npcConfigBase);
		
		if (npcConfigBase.ConfigVersion < CONFIGVERSION)
		{
			CF_Log.Info("[ExpansionQuestObjectiveConfigBase] Convert existing configuration file:" + fileName + " to version " + CONFIGVERSION);
			npcConfig = new ExpansionQuestObjectiveConfigBase();			
			//! Copy over old configuration that haven't changed
			npcConfig.CopyConfig(npcConfigBase);
			npcConfig.ConfigVersion = CONFIGVERSION;
		}
		else
		{
			JsonFileLoader<ExpansionQuestObjectiveConfigBase>.JsonLoadFile(fileName, npcConfig);
		}
		
		return npcConfig;
	}
	
	void Save(string fileName)
	{
		JsonFileLoader<ExpansionQuestObjectiveConfigBase>.JsonSaveFile(fileName + ".JSON", this);
	}
	
	void CopyConfig(ExpansionQuestObjectiveConfigBasement configBase)
	{
		ID = configBase.ID;
		ObjectiveType = configBase.ObjectiveType;
		ObjectiveText = configBase.ObjectiveText;
		TimeLimit = configBase.TimeLimit;
	}

	void OnSend(ParamsWriteContext ctx)
	{
		ctx.Write(ID);
		ctx.Write(ObjectiveType);
		ctx.Write(ObjectiveText);
		ctx.Write(TimeLimit);
	}

	bool OnRecieve(ParamsReadContext ctx)
	{		
		if (!ctx.Read(ID))
			return false;

		if (!ctx.Read(ObjectiveType))
			return false;

		if (!ctx.Read(ObjectiveText))
			return false;
		
		if (!ctx.Read(TimeLimit))
			return false;

		return true;
	}

	void QuestDebug()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		Print("------------------------------------------------------------");
		Print(ToString() + "::QuestDebug - Objective ID: " + ID);
		Print(ToString() + "::QuestDebug - ObjectiveType: " + ObjectiveType);
		Print(ToString() + "::QuestDebug - ObjectiveText: " + ObjectiveText);
		Print(ToString() + "::QuestDebug - TimeLimit: " + TimeLimit);
		Print("------------------------------------------------------------");
	#endif
	}
};