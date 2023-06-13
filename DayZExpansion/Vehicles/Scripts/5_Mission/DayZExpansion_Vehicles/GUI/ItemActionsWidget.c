modded class ItemActionsWidget
{
	protected override void SetActionWidget(ActionBase action, string descText, string actionWidget, string descWidget)
	{
		Widget widget = m_Root.FindAnyWidget(actionWidget);

		// TODO: TMP: Car AM rework needed (checking of player's command)
		if (action && (!action.HasTarget() || (m_Player && (m_Player.GetCommand_Vehicle() || m_Player.GetCommand_ExpansionVehicle()))))
		{
			TextWidget actionName;
			Class.CastTo(actionName, widget.FindAnyWidget(descWidget));
			

			if (action.GetInput().GetInputType() == ActionInputType.AIT_CONTINUOUS)
			{
				descText = descText + " " + "#action_target_cursor_hold";
				actionName.SetText(descText);
			}
			else
			{
				actionName.SetText(descText);
			}

			widget.Show(true);
		}
		else
		{
			widget.Show(false);
		}
	}
};