modded class PlayerBase
{
#ifdef EXPANSION_MODSTORAGE
	protected int m_Expansion_StoreLoadedVersion;

	//! Safer implementation of vanilla PlayerBase::OnStoreSave/OnStoreLoad specifically for entity storage
	//! Vanilla plays fairly loose with the way some of PlayerBase data is written to storage which makes reading it back safely a challenge.
	//! To circumvent these problems for entity storage, vanilla OnStoreSave/Load is handled completely by CF_ModStorage
	//! (normal storage save/load will still use vanilla OnStoreSave/Load, as will any data added by 3rd party mods)
	override void OnStoreSave(ParamsWriteContext ctx)
	{
		if (ExpansionEntityStorageModule.IsCurrentRootEntity(this))
		{
			EXTrace.Print(EXTrace.PLAYER, this, "---- [EntityStorage] PlayerBase::OnStoreSave -> CF_ModStorageObject::OnStoreSave ----");
			m_CF_ModStorage.OnStoreSave(ctx);
		}
		else
		{
			super.OnStoreSave(ctx);
		}
	}

	override bool OnStoreLoad(ParamsReadContext ctx, int version)
	{
		m_Expansion_StoreLoadedVersion = version;

		if (ExpansionEntityStorageModule.IsCurrentRootEntity(this) && ExpansionEntityStorageModule.GetRestoredVersion() >= 13)
		{
			EXTrace.Print(EXTrace.PLAYER, this, "---- [EntityStorage] PlayerBase::OnStoreLoad -> CF_ModStorageObject::OnStoreLoad ----");
			return m_CF_ModStorage.OnStoreLoad(ctx, version);
		}

		EXTrace.Print(EXTrace.PLAYER, this, "---- PlayerBase::OnStoreLoad super  ----");
		return super.OnStoreLoad(ctx, version);
	}
#endif
}
