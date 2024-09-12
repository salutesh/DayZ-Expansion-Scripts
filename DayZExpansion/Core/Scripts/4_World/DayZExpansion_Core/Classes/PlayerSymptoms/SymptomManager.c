modded class SymptomManager
{
#ifdef EXPANSION_MODSTORAGE
	void CF_OnStoreSave(CF_ModStorage ctx)
	{
		TIntArray symptoms = {};

		foreach (auto primarySymptom: m_SymptomQueuePrimary)
		{
			if (primarySymptom.IsPersistent())
				symptoms.Insert(primarySymptom.GetType());
		}

		foreach (auto secondarySymptom: m_SymptomQueueSecondary)
		{
			if (secondarySymptom.IsPersistent())
				symptoms.Insert(secondarySymptom.GetType());
		}

		//! Unlike vanilla, we write count so we'll always be able to successfully read back what was actually saved
		ctx.Write(symptoms.Count());

		foreach (int id: symptoms)
		{
			ctx.Write(id);
		}
	}

	bool CF_OnStoreLoad(CF_ModStorage ctx, int version)
	{
		int count;
		if (!ctx.Read(count))
			return false;

		while (count--)
		{
			int id;
			if (!ctx.Read(id))
				return false;

			if (IsSymptomPrimary(id))
				QueueUpPrimarySymptom(id);
			else
				QueueUpSecondarySymptom(id);
		}

		return true;
	}
#endif
}
