modded class ItemActionsWidget
{
#ifdef DIAG
	override protected string GetActionDesc(ActionBase action)
	{
		string desc = super.GetActionDesc(action);

		if (action && EXTrace.ACTIONS)
			return string.Format("%1 <%2>", desc, action.ClassName());

		return desc;
	}
#endif
}
