modded class DigitalCodeLock
{
	override void SetLockState(bool state)
	{
		if (state && g_Game.IsServer())
			Expansion_StoreOrRestoreParentContents(true);

		super.SetLockState(state);

		if (!state && g_Game.IsServer())
			g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).Call(Expansion_StoreOrRestoreParentContents, false);
	}
}
