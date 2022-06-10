/**
 * ExpansionQuestObjectiveAICamp.c
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
class ExpansionQuestObjectiveAICamp
{
	private ref array<vector> Positions = new array<vector>;
	private string NPCSpeed;
	private string NPCMode;
	private string NPCFaction;
	private string NPCLoadoutFile;
	private autoptr array<string> ClassNames = new array<string>;
	private bool SpecialWeapon = false;
	private autoptr array<string> AllowedWeapons = new array<string>;

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

	void AddPosition(vector pos)
	{
		Positions.Insert(pos);
	}

	array<vector> GetPositions()
	{
		return Positions;
	}
	
	void AddClassName(string name)
	{
		ClassNames.Insert(name);
	}

	array<string> GetClassNames()
	{
		return ClassNames;
	}

	void SetNeedSpecialWeapon(bool state)
	{
		SpecialWeapon = state;
	}

	bool NeedSpecialWeapon()
	{
		return SpecialWeapon;
	}

	void AddAllowedWeapon(string name)
	{
		AllowedWeapons.Insert(name);
	}

	array<string> GetAllowedWeapons()
	{
		return AllowedWeapons;
	}

	void OnSend(ParamsWriteContext ctx)
	{
		int positionsCount = Positions.Count();
		ctx.Write(positionsCount);
		
		int i;
		for (i = 0; i < Positions.Count(); i++)
		{
			ctx.Write(Positions[i]);
		}

		ctx.Write(NPCSpeed);
		ctx.Write(NPCMode);
		ctx.Write(NPCFaction);
		ctx.Write(NPCLoadoutFile);
	}

	bool OnRecieve(ParamsReadContext ctx)
	{
		if (!Positions)
			Positions = new array<vector>;
		else
			Positions.Clear();

		int positionsCount;
		if (!ctx.Read(positionsCount))
			return false;

		for (int i = 0; i < positionsCount; i++)
		{
			vector pos;
			if (!ctx.Read(pos))
				return false;

			Positions.Insert(pos);
		}

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
		for (int i = 0; i < Positions.Count(); ++i)
		{
			Print(ToString() + "::QuestDebug - Position" + i + " :" + Positions[i]);
		}
		Print(ToString() + "::QuestDebug - NPCSpeed: " + NPCSpeed);
		Print(ToString() + "::QuestDebug - NPCSpeed: " + NPCMode);
		Print(ToString() + "::QuestDebug - NPCFaction: " + NPCFaction);
		Print(ToString() + "::QuestDebug - NPCLoadoutFile: " + NPCLoadoutFile);
		Print("------------------------------------------------------------");
	#endif
	}
};
#endif