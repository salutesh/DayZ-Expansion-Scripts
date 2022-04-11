modded class ActionBase
{
#ifdef CF_DebugUI
	override bool CF_OnDebugUpdate(CF_Debug instance, CF_DebugUI_Type type)
	{
		return false;
	}
#endif
};
