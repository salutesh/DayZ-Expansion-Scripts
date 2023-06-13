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
	//private UIActionDropdownList m_ExpansionFaction;  //! TODO: Should use dropdown but COT dropdowns are scuffed
	private UIActionEditableText m_ExpansionFaction;
	private bool m_ExpansionFactionUpdated;

	override void OnClientPermissionsUpdated()
	{
		super.OnClientPermissionsUpdated();

		// idk unify naming for the permission? rest of player permissions are parented here
		UpdatePermission(m_ExpansionFaction, "Admin.Player.Set.ExpansionFaction");
	}

	override private Widget InitActionWidgetsStats(Widget actionsParent)
	{
		Widget parent = super.InitActionWidgetsStats(actionsParent);

		Widget actions = m_Stamina.GetLayoutRoot().GetParent();
		if (actions)
		{
			/*
			TStringArray factions = {};

			foreach (typename factionType: eAIRegisterFaction.s_FactionTypes)
			{
				eAIFaction faction = eAIFaction.Cast(factionType.Spawn());
				if (faction)
					factions.Insert(faction.GetName());
			}

			m_ExpansionFaction = UIActionManager.CreateDropdownBox(actions, GetLayoutRoot(), "Faction:", factions, this, "Click_SetExpansionFaction");

			EditBoxWidget editBox = m_ExpansionFaction.GetEditBoxWidget();
			float w;
			float h;
			editBox.GetSize( w, h );
			editBox.SetSize( 0.5, h );
			editBox.Update();
			*/
			m_ExpansionFaction = UIActionManager.CreateEditableText(actions, "Faction:", this, "Click_SetExpansionFaction", "", "");
			#ifdef COT_UIACTIONS_SETWIDTH
			m_ExpansionFaction.SetEditBoxWidth(0.5);
			#endif
			Widget spacer = UIActionManager.CreatePanel(actions, 0, 1);
			int sort = spacer.GetSort();
			m_ApplyStats.GetLayoutRoot().SetSort(sort + 1);
			m_RefreshStats.GetLayoutRoot().SetSort(sort + 2);
		}

		return parent;
	}

	override void RefreshStats(bool force = false)
	{
		super.RefreshStats(force);

		if (force)
			m_ExpansionFactionUpdated = false;

		if (!m_SelectedInstance)
			return;

		if (m_ExpansionFaction && !m_ExpansionFactionUpdated)
		{
			string factionName;

			int factionTypeID = m_SelectedInstance.GetExpansionFactionTypeID();
			if (factionTypeID)
			{
				typename factionType = eAIFaction.GetTypeByID(factionTypeID);
				if (factionType)
				{
					eAIFaction faction = eAIFaction.Cast(factionType.Spawn());
					if (faction)
						factionName = faction.GetName();
				}
			}

			m_ExpansionFaction.SetText(factionName);
		}
	}

	override void Click_ApplyStats(UIEvent eid, UIActionBase action)
	{
		super.Click_ApplyStats(eid, action);

		if (eid != UIEvent.CLICK)
			return;

		if (m_ExpansionFactionUpdated)
		{
			m_ExpansionFactionUpdated = false;

			if (m_ExpansionFaction)
			{
				m_Module.SetExpansionFaction(m_ExpansionFaction.GetText(), JM_GetSelected().GetPlayers());
			}
		}
	}

	void Click_SetExpansionFaction(UIEvent eid, UIActionBase action)
	{
		if (eid != UIEvent.CHANGE)
			return;

		m_ExpansionFactionUpdated = true;
	}
}
#endif
