/**
 * ExpansionQuestObjectiveTargetConfig.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestObjectiveTargetConfig: ExpansionQuestObjectiveConfigBase
{
	private vector Position = vector.Zero;
	private float MaxDistance = -1;
	private autoptr ExpansionQuestObjectiveTarget Target;

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

	void SetTarget(ExpansionQuestObjectiveTarget target)
	{
		Target = target;
	}

	override ExpansionQuestObjectiveTarget GetTarget()
	{
		return Target;
	}

	override void Save(string fileName)
	{
		JsonFileLoader<ExpansionQuestObjectiveTargetConfig>.JsonSaveFile(EXPANSION_QUESTS_OBJECTIVES_TARGET_FOLDER + fileName + ".JSON", this);
	}

	override void OnSend(ParamsWriteContext ctx)
	{
		super.OnSend(ctx);

		Target.OnSend(ctx);
	}

	override bool OnRecieve(ParamsReadContext ctx)
	{
		if (!super.OnRecieve(ctx))
			return false;

		if (!Target)
			Target = new ExpansionQuestObjectiveTarget();

		if (!Target.OnRecieve(ctx))
			return false;

		return true;
	}

	override void QuestDebug()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		if (Target)
			Target.QuestDebug();
	#endif
	}
};