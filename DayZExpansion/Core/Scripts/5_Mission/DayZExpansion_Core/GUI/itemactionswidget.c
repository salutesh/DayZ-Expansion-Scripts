modded class ItemActionsWidget
{
#ifdef DIAG_DEVELOPER
	override protected string GetActionDesc(ActionBase action)
	{
		string desc = super.GetActionDesc(action);

		if (action && DiagMenu.IsInitialized() && DiagMenu.GetBool(PluginDiagMenu.EXPANSION_SHOW_ACTION_CLASSNAME))
			return string.Format("%1 <%2>", desc, action.ClassName());

		return desc;
	}
#endif
}
