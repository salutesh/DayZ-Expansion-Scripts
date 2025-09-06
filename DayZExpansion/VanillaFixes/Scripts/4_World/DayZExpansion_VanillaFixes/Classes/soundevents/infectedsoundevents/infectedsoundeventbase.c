//! Fix vanilla not removing PosUpdate from call queue if InfectedSoundEventBase::Stop was never called
modded class InfectedSoundEventBase
{
	void ~InfectedSoundEventBase()
	{
		if (!g_Game)
			return;

		ScriptCallQueue callQueue = g_Game.GetCallQueue(CALL_CATEGORY_GAMEPLAY);
		int remaining = callQueue.GetRemainingTimeByName(this, "PosUpdate");
		if (remaining > -1)
		{
		#ifdef DIAG_DEVELOPER
			EXTrace.Print(EXTrace.AI, this, "~InfectedSoundEventBase PosUpdate remaining time " + remaining);
		#endif

			callQueue.RemoveByName(this, "PosUpdate");
		}
	}
}
