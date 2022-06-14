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
    private UIActionButton m_PositionPlayer;

    override private Widget InitActionWidgetsPosition( Widget actionsParent )
	{
        Widget parent = super.InitActionWidgetsPosition(actionsParent);
        Widget expansionActions = UIActionManager.CreateGridSpacer(parent, 1, 1);
        Widget expansionActionsBut = UIActionManager.CreateGridSpacer(expansionActions, 1, 2);
        m_PositionPlayer = UIActionManager.CreateButton(expansionActionsBut, "Copy Position", this, "Click_PlayerPostion");

		return parent;
	}

    void Click_PlayerPostion( UIEvent eid, ref UIActionBase action )
	{
		if (!m_SelectedInstance)
			return;

		vector position = m_SelectedInstance.GetPosition();
		m_PositionX.SetText(position[0].ToString());
		m_PositionY.SetText(position[1].ToString());
		m_PositionZ.SetText(position[2].ToString());

        GetGame().CopyToClipboard("<" + position[0].ToString() + ", " + position[1].ToString() + ", " + position[2].ToString() + ">");
        StringLocaliser title = new StringLocaliser("Community Online Tools");
		StringLocaliser text = new StringLocaliser("Copied position of selected player to clipboard. Position: (" + position[0].ToString() + ", " + position[1].ToString() + ", " + position[2].ToString() + ")");
		ExpansionNotification(title, text, EXPANSION_NOTIFICATION_ICON_INFO, COLOR_EXPANSION_NOTIFICATION_INFO).Create();
	}
}
#endif