/**
 * ExpansionTeleporterModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionTeleportPosition
{
	protected string m_DisplayName;
	protected ref array<ref ExpansionTeleportPositionEntry> m_Positions;
	protected string m_Faction;
	protected int m_QuestID;
	protected int m_Reputation;

	void ExpansionTeleportPosition()
	{
		m_Positions = new array<ref ExpansionTeleportPositionEntry>;
	}
	
	void SetData(string displayName, string faction = "", int questID = -1, int reputration = 0)
	{
		m_DisplayName = displayName;
		m_Faction = faction;
		m_QuestID = questID;
		m_Reputation = reputration;
	}

	string GetDisplayName()
	{
		return m_DisplayName;
	}

	void AddPosition(vector pos, vector ori)
	{
		ExpansionTeleportPositionEntry entry = new ExpansionTeleportPositionEntry();
		entry.SetPosition(pos, ori);
		m_Positions.Insert(entry);
	}

	array<ref ExpansionTeleportPositionEntry> GetPositions()
	{
		return m_Positions;
	}

	string GetFaction()
	{
		return m_Faction;
	}

	int GetQuestID()
	{
		return m_QuestID;
	}
	
	int GetReputation()
	{
		return m_Reputation;
	}

	void OnSend(ParamsWriteContext ctx)
	{
		auto trace = EXTrace.Start(EXTrace.TELEPORTER, this);

		ctx.Write(m_DisplayName);
		ctx.Write(m_Faction);
		ctx.Write(m_QuestID);
		ctx.Write(m_Reputation);

		int positionsCount = m_Positions.Count();
		ctx.Write(positionsCount);

		for (int i = 0; i < positionsCount; i++)
		{
			ExpansionTeleportPositionEntry entry = m_Positions[i];
			entry.OnSend(ctx);
		}
	}

	bool OnRecieve(ParamsReadContext ctx)
	{
		if (!ctx.Read(m_DisplayName))
			return false;

		if (!ctx.Read(m_Faction))
			return false;

		if (!ctx.Read(m_QuestID))
			return false;
		
		if (!ctx.Read(m_Reputation))
			return false;

		int positionsCount;
		if (!ctx.Read(positionsCount))
			return false;
		
		m_Positions = new array<ref ExpansionTeleportPositionEntry>;
		for (int i = 0; i < positionsCount; i++)
		{
			ExpansionTeleportPositionEntry entry = new ExpansionTeleportPositionEntry();
			if (!entry.OnRecieve(ctx))
				return false;

			m_Positions.Insert(entry);
		}

		return true;
	}
};