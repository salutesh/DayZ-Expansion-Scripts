modded class Clothing
{
	int m_eAI_CargoChangedCount;

	override void EECargoIn(EntityAI item)
	{
		super.EECargoIn(item);

		eAI_OnCargoChanged();
	}

	override void EECargoOut(EntityAI item)
	{
		super.EECargoOut(item);

		eAI_OnCargoChanged();
	}

	override void EECargoMove(EntityAI item)
	{
		super.EECargoMove(item);

		eAI_OnCargoChanged();
	}

	void eAI_OnCargoChanged()
	{
		if (!g_Game.IsServer() || !IsInitialized() || IsPendingDeletion() || ExpansionInventoryTools.IsTidyInProgress(this))
			return;

		eAIBase ai;
		if (Class.CastTo(ai, GetHierarchyRoot()) && !ai.IsDamageDestroyed() && !ai.IsPendingDeletion())
		{
			if (++m_eAI_CargoChangedCount == 3)
				ai.m_eAI_CargosToTidy.Insert(this);
		}
	}
}
