/**
 * ExpansionTempQuestHolder.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionTempQuestHolder
{
	int ID = -1;
	string ClassName;
	ref array<ref ExpansionTempQuestHolderPosition> Positions = new array<ref ExpansionTempQuestHolderPosition>;
	string Name;
	string DefaultText;
	string LoadoutName;
#ifdef EXPANSIONMODAI
	int NPCEmoteID = EmoteConstants.ID_EMOTE_WATCHING;
#endif

	void ExpansionTempQuestHolder(int id, string className, string name = "Unknown", string text = "There is nothing to do here for you...")
	{
		ID = id;
		ClassName = className;
		Name = name;
		DefaultText = text;
	}

	void AddPosition(ExpansionTempQuestHolderPosition pos)
	{
		Positions.Insert(pos);
	}

	void SetLoadoutName(string loadout)
	{
		LoadoutName = loadout;
	}
		
#ifdef EXPANSIONMODAI
	void SetNPCEmoteID(int id)
	{
		NPCEmoteID = id;
	}
#endif
	
	void OnSend(ParamsWriteContext ctx)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif
		
		ctx.Write(ID);
		ctx.Write(ClassName);
		ctx.Write(Positions.Count());
		
		for (int i = 0; i < Positions.Count(); i++)
		{
			ExpansionTempQuestHolderPosition pos = Positions[i];
			pos.OnSend(ctx);
		}
		
		ctx.Write(Name);
		ctx.Write(DefaultText);
		ctx.Write(LoadoutName);
	#ifdef EXPANSIONMODAI
		ctx.Write(NPCEmoteID);
	#endif
	}
	
	bool OnRecieve(ParamsReadContext ctx)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif
		
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
		
		int posCount;
		if (!ctx.Read(posCount))
		{
			Error(ToString() + "::OnRecieve - posCount");
			return false;
		}
		
		for (int i = 0; i < posCount; i++)
		{
			ExpansionTempQuestHolderPosition pos = new ExpansionTempQuestHolderPosition(vector.Zero, vector.Zero);
			pos.OnRecieve(ctx);
			AddPosition(pos);
		}
		
		if (!ctx.Read(Name))
		{
			Error(ToString() + "::OnRecieve - Name");
			return false;
		}
		
		if (!ctx.Read(DefaultText))
		{
			Error(ToString() + "::OnRecieve - DefaultText");
			return false;
		}
		
		if (!ctx.Read(LoadoutName))
		{
			Error(ToString() + "::OnRecieve - LoadoutName");
			return false;
		}
		
	#ifdef EXPANSIONMODAI
		if (!ctx.Read(NPCEmoteID))
		{
			Error(ToString() + "::OnRecieve - NPCEmoteID");
			return false;
		}
	#endif
		
		return true;
	}
};

class ExpansionTempQuestHolderPosition
{
	vector Position;
	vector Orientation;

	void ExpansionTempQuestHolderPosition(vector pos, vector ori)
	{
		Position = pos;
		Orientation = ori;
	}
	
	void OnSend(ParamsWriteContext ctx)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif
		
		ctx.Write(Position);
		ctx.Write(Orientation);
	}
	
	bool OnRecieve(ParamsReadContext ctx)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.QUESTS, this);
#endif
		
		if (!ctx.Read(Position))
		{
			Error(ToString() + "::OnRecieve - Position");
			return false;
		}
		
		if (!ctx.Read(Orientation))
		{
			Error(ToString() + "::OnRecieve - Orientation");
			return false;
		}
		
		return true;
	}
};