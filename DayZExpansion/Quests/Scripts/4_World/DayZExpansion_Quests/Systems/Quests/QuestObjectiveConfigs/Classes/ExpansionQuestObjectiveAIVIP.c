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

#ifdef EXPANSIONMODAI
class ExpansionQuestObjectiveAIVIP
{
	protected string NPCClassName;
	protected string NPCSpeed;
	protected string NPCMode;
	protected string NPCFaction;
	protected string NPCLoadoutFile;

	void SetNPCSpeed(string npcSpeed)
	{
		NPCSpeed = npcSpeed;
	}

	string GetNPCSpeed()
	{
		return NPCSpeed;
	}

	void SetNPCMode(string npcMode)
	{
		NPCMode = npcMode;
	}

	string GetNPCMode()
	{
		return NPCMode;
	}

	void SetNPCFaction(string npcFaction)
	{
		NPCFaction = npcFaction;
	}

	string GetNPCFaction()
	{
		return NPCFaction;
	}

	void SetNPCLoadoutFile(string loadoutFile)
	{
		NPCLoadoutFile = loadoutFile;
	}

	string GetNPCLoadoutFile()
	{
		return NPCLoadoutFile;
	}

	void SetClassName(string name)
	{
		NPCClassName = name;
	}

	string GetClassName()
	{
		return NPCClassName;
	}

	void OnSend(ParamsWriteContext ctx)
	{
		ctx.Write(NPCSpeed);
		ctx.Write(NPCMode);
		ctx.Write(NPCFaction);
		ctx.Write(NPCLoadoutFile);
	}

	bool OnRecieve(ParamsReadContext ctx)
	{
		string npcSpeed;
		if (!ctx.Read(npcSpeed))
			return false;

		NPCSpeed = npcSpeed;

		string npcMode;
		if (!ctx.Read(npcMode))
			return false;

		NPCMode = npcMode;

		string npcFaction;
		if (!ctx.Read(npcFaction))
			return false;

		NPCFaction = npcFaction;

		string loadoutFile;
		if (!ctx.Read(loadoutFile))
			return false;

		NPCLoadoutFile = loadoutFile;

		return true;
	}

	void QuestDebug()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		Print("------------------------------------------------------------");
		Print(ToString() + "::QuestDebug - NPCSpeed: " + NPCSpeed);
		Print(ToString() + "::QuestDebug - NPCSpeed: " + NPCMode);
		Print(ToString() + "::QuestDebug - NPCFaction: " + NPCFaction);
		Print(ToString() + "::QuestDebug - NPCLoadoutFile: " + NPCLoadoutFile);
		Print("------------------------------------------------------------");
	#endif
	}
};
#endif