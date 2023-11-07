/**
 * ExpansionQuestObjectiveAIVIP.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestObjectiveAIVIP
{
	protected string NPCLoadoutFile;
	protected string NPCClassName;

	void SetNPCLoadoutFile(string loadoutFile)
	{
		NPCLoadoutFile = loadoutFile;
	}

	string GetNPCLoadoutFile()
	{
		return NPCLoadoutFile;
	}
	
	void SetNPCClassName(string className)
	{
		NPCClassName = className;
	}

	string GetNPCClassName()
	{
		return NPCClassName;
	}

	void QuestDebug()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		Print("------------------------------------------------------------");
		Print(ToString() + "::QuestDebug - NPCLoadoutFile: " + NPCLoadoutFile);
		Print(ToString() + "::QuestDebug - NPCClassName: " + NPCClassName);
		Print("------------------------------------------------------------");
	#endif
	}
};
