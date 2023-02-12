/**
 * JMPlayerForm.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef JM_COT
modded class JMPlayerForm
{
	private UIActionEditableText m_ExpansionReputation;
	private bool m_ExpansionReputationUpdated;

	override void OnClientPermissionsUpdated()
	{
		super.OnClientPermissionsUpdated();

		// idk unify naming for the permission? rest of player permissions are parented here
		UpdatePermission(m_ExpansionReputation, "Admin.Player.Set.ExpansionReputation");
	}

	override private Widget InitActionWidgetsStats(Widget actionsParent)
	{
		Widget parent = super.InitActionWidgetsStats(actionsParent);

		// add in our edit box - awkward since we cannot get the containing grid spacer directly but have to walk the hierarchy until we find it
		// TODO localize
		Widget actions = m_Stamina.GetLayoutRoot().GetParent();
		if (actions)
		{
			m_ExpansionReputation = UIActionManager.CreateEditableText(actions, "Reputation", this, "Click_SetExpansionReputation", "", "");
			Widget spacer = UIActionManager.CreatePanel(actions, 0, 1);
			int sort = spacer.GetSort();
			m_ApplyStats.GetLayoutRoot().SetSort(sort + 1);
			m_RefreshStats.GetLayoutRoot().SetSort(sort + 2);
		}

		return parent;
	}

	override void RefreshStats()
	{
		super.RefreshStats();

		if (m_ExpansionReputation)
			m_ExpansionReputation.SetText(m_SelectedInstance.PlayerObject.Expansion_GetReputation().ToString());
	}

	override void Click_ApplyStats(UIEvent eid, ref UIActionBase action)
	{
		super.Click_ApplyStats(eid, action);

		if (eid != UIEvent.CLICK)
			return;

		if (m_ExpansionReputationUpdated)
		{
			m_ExpansionReputationUpdated = false;

			if (m_ExpansionReputation)
				m_Module.SetExpansionReputation(ToFloat(m_ExpansionReputation.GetText()), JM_GetSelected().GetPlayers());
		}
	}

	void Click_SetExpansionReputation(UIEvent eid, ref UIActionBase action)
	{
		if (eid != UIEvent.CHANGE)
			return;

		m_ExpansionReputationUpdated = true;
	}
}
#endif
