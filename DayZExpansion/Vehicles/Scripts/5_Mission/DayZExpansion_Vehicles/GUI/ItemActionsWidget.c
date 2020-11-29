modded class ItemActionsWidget
{
	protected override void SetActionWidget(ActionBase action, string descText, string actionWidget, string descWidget)
	{
		Widget widget;
		
		widget = m_Root.FindAnyWidget(actionWidget);
		
#ifdef PLATFORM_XBOX
		ShowXboxHidePCIcons( actionWidget, !GetGame().GetInput().IsEnabledMouseAndKeyboardEvenOnServer() );
#else
#ifdef PLATFORM_PS4
		ShowXboxHidePCIcons( actionWidget, !GetGame().GetInput().IsEnabledMouseAndKeyboardEvenOnServer() );
#else
		ShowXboxHidePCIcons( actionWidget, false );
#endif
#endif
		// TODO: TMP: Car AM rework needed (checking of player's command)
		if(action && (!action.HasTarget() || m_Player.GetCommand_Vehicle() || m_Player.GetCommand_ExpansionVehicle()))
		{
			TextWidget actionName;
			Class.CastTo(actionName, widget.FindAnyWidget(descWidget));
			

			if(action.GetInput().GetInputType() == ActionInputType.AIT_CONTINUOUS)
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