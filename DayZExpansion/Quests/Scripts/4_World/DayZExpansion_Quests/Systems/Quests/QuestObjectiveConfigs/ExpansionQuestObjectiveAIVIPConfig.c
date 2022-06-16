/**
 * ExpansionQuestObjectiveAIVIPConfig.c
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
class ExpansionQuestObjectiveAIVIPConfig: ExpansionQuestObjectiveConfigBase
{
	private vector Position = vector.Zero;
	private float MaxDistance = 0;
	private ref ExpansionQuestObjectiveAIVIP AIVIP;
	private string MarkerName = string.Empty;

	void SetPosition(vector pos)
	{
		Position = pos;
	}

	override vector GetPosition()
	{
		return Position;
	}

	void SetMaxDistance(float max)
	{
		MaxDistance = max;
	}

	override float GetMaxDistance()
	{
		return MaxDistance;
	}

	void SetAIVIP(ExpansionQuestObjectiveAIVIP vip)
	{
		AIVIP = vip;
	}

	override ExpansionQuestObjectiveAIVIP GetAIVIP()
	{
		return AIVIP;
	}

	void SetMarkerName(string name)
	{
		MarkerName = name;
	}

	override string GetMarkerName()
	{
		return MarkerName;
	}

	override void Save(string fileName)
	{
		JsonFileLoader<ExpansionQuestObjectiveAIVIPConfig>.JsonSaveFile(EXPANSION_QUESTS_OBJECTIVES_AIVIP_FOLDER + fileName + ".JSON", this);
	}

	override void OnSend(ParamsWriteContext ctx)
	{
		super.OnSend(ctx);

		ctx.Write(Position);
		ctx.Write(MaxDistance);
		ctx.Write(MarkerName);
	}

	override bool OnRecieve(ParamsReadContext ctx)
	{
		if (!super.OnRecieve(ctx))
			return false;

		if (!ctx.Read(Position))
			return false;

		if (!ctx.Read(MaxDistance))
			return false;

		if (!ctx.Read(MarkerName))
			return false;

		return true;
	}

	override void QuestDebug()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		if (AIVIP)
			AIVIP.QuestDebug();
	#endif
	}
};
#endif