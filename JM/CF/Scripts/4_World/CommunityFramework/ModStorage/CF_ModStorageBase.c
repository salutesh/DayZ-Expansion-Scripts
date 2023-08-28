class CF_ModStorageBase
{
	void OnStoreSave(ParamsWriteContext ctx)
	{
	}

	bool OnStoreLoad(ParamsReadContext ctx, int version)
	{
		return true;
	}
};
