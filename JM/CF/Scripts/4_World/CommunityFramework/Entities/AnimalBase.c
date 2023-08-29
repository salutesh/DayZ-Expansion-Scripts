modded class AnimalBase
{
#ifdef EXPANSION_MODSTORAGE
	autoptr CF_ModStorageBase m_CF_ModStorage = new CF_ModStorageObject<AnimalBase>(this);

	override void OnStoreSave(ParamsWriteContext ctx)
	{
		super.OnStoreSave(ctx);

		m_CF_ModStorage.OnStoreSave(ctx);
	}

	override bool OnStoreLoad(ParamsReadContext ctx, int version)
	{
		if (!super.OnStoreLoad(ctx, version))
		{
			return false;
		}

		return m_CF_ModStorage.OnStoreLoad(ctx, version);
	}
#endif

	void CF_OnStoreSave(CF_ModStorageMap storage)
	{
	}

	bool CF_OnStoreLoad(CF_ModStorageMap storage)
	{
		return true;
	}
};
