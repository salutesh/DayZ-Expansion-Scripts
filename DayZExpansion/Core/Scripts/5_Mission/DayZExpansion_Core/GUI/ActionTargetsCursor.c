/**
 * ActionTargetsCursor.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionTargetsCursor
{
#ifdef EXPANSIONUI
	override void Update()
	{
		//! Don't show floating widget if any script view menu is opened
		ExpansionScriptViewMenu viewMenu = ExpansionScriptViewMenu.Cast(GetDayZExpansion().GetExpansionUIManager().GetMenu());
		if (viewMenu && viewMenu.IsVisible())
		{
			HideWidget();
			return;
		}

		super.Update();
	}
#endif

#ifdef DIAG
	override protected string GetActionDesc(ActionBase action)
	{
		string desc = super.GetActionDesc(action);

		if (action && EXTrace.ACTIONS)
			return string.Format("%1 <%2>", desc, action.ClassName());

		return desc;
	}
#endif
};