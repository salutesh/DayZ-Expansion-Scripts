/**
 * ExpansionQuestObjectiveCollectionConfig.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestObjectiveCollectionConfig: ExpansionQuestObjectiveConfig
{
	protected ref ExpansionQuestObjectiveCollection Collection = new ExpansionQuestObjectiveCollection();

	void AddCollection(ExpansionQuestObjectiveCollection collection)
	{
		Collection = collection;
	}

	override ExpansionQuestObjectiveCollection GetCollection()
	{
		return Collection;
	}

	override void Save(string fileName)
	{
		JsonFileLoader<ExpansionQuestObjectiveCollectionConfig>.JsonSaveFile(EXPANSION_QUESTS_OBJECTIVES_COLLECTION_FOLDER + fileName + ".json", this);
	}

	override void OnSend(ParamsWriteContext ctx)
	{
		super.OnSend(ctx);

		Collection.OnSend(ctx);
	}

	override bool OnRecieve(ParamsReadContext ctx)
	{
		if (!super.OnRecieve(ctx))
			return false;

		if (!Collection)
			Collection = new ExpansionQuestObjectiveCollection();

		if (!Collection.OnRecieve(ctx))
			return false;

		return true;
	}

	override void QuestDebug()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		if (Collection)
			Collection.QuestDebug();
	#endif
	}
};