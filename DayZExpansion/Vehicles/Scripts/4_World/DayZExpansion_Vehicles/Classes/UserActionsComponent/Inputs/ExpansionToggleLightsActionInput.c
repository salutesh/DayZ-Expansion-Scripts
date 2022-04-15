/**
 * ExpansionToggleLightsActionInput.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionToggleLightsActionInput : DefaultActionInput
{
	ref ActionTarget target_new;
	
	void ToggleLightsActionInput(PlayerBase player)
	{
		SetInput("UAToggleHeadlight");
		m_InputType = ActionInputType.AIT_SINGLE;
	}
	
	override void UpdatePossibleActions(PlayerBase player, ActionTarget target, ItemBase item, int action_condition_mask)
	{
		if( ForceActionCheck(player) )
		{
			m_SelectAction = m_ForcedActionData.m_Action;
			return;
		}
		//ForceActionTarget(player.m_PlayerLightManager.
		
		m_SelectAction = NULL;
		array<ActionBase_Basic> possible_actions;
		ActionBase action;
		int i;

		m_MainItem = NULL;
		if ( player && player.IsInVehicle() )
		{
			ExpansionHumanCommandVehicle vehCommand = player.GetCommand_ExpansionVehicle();
			if( vehCommand )
			{
				ExpansionVehicleBase trans = vehCommand.GetExpansionVehicle();
				if( trans )
				{
					target_new = new ActionTarget(trans, null, -1, vector.Zero, -1);
					ForceActionTarget(target_new);
				}
			}
			
			if ( !target_new )
				ClearForcedTarget();
		}
		
		target = m_ForcedTarget;
		m_Target = m_ForcedTarget;
		
		if(target && target.GetObject())
		{
			target.GetObject().GetActions(this.Type(), possible_actions);
			if(possible_actions)
			{
				for (i = 0; i < possible_actions.Count(); i++)
				{
					action = ActionBase.Cast(possible_actions.Get(i));
					if ( action.Can(player, target, m_MainItem, action_condition_mask) )
					{
						m_SelectAction = action;
						return;
					}
				}
			}
		}
	}
};