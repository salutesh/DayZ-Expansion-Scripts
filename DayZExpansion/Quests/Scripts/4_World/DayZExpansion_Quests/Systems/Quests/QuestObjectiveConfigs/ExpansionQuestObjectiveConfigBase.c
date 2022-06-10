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

class ExpansionQuestObjectiveConfigBase
{
	private int ID = -1; //! Unique objective ID
	private int ObjectiveType = ExpansionQuestObjectiveType.NONE; //! Quest obecjtive type.
	private string ObjectiveText = string.Empty;

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

	void Save(string fileName)
	{
		JsonFileLoader<ExpansionQuestObjectiveConfigBase>.JsonSaveFile(EXPANSION_QUESTS_OBJECTIVES_FOLDER + fileName + ".JSON", this);
	}

	void OnSend(ParamsWriteContext ctx)
	{
		ctx.Write(ID);
		ctx.Write(ObjectiveType);
		ctx.Write(ObjectiveText);
	}

	bool OnRecieve(ParamsReadContext ctx)
	{
		if (!ctx.Read(ID))
			return false;

		if (!ctx.Read(ObjectiveType))
			return false;
		
		if (!ctx.Read(ObjectiveText))
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
		Print("------------------------------------------------------------");
	#endif
	}
};