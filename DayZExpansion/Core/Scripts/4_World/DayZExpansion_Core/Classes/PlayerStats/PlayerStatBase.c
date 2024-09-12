modded class PlayerStatBase
{
#ifdef EXPANSION_MODSTORAGE
	void CF_OnStoreSave(CF_ModStorage ctx)
	{   
		ctx.Write(Get());
	}

	bool CF_OnStoreLoad(CF_ModStorage ctx)
	{
		float value;
		if (!ctx.Read(value))
			return false;

		SetByFloat(value);

		return true;
	}
#endif
}
