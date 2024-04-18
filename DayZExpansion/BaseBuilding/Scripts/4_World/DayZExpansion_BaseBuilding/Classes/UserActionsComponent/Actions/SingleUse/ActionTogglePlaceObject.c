/**
 * ActionTogglePlaceObject.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

//! @note remember, this is a local (client/SP only) action!
modded class ActionTogglePlaceObject
{
	override void Start(ActionData action_data)
	{
		//! Always enable toggle off, only enable toggle on if deploy is allowed
		if (action_data.m_Player.IsPlacingLocal() || ActionDeployObject.Expansion_CheckDeploy(action_data.m_Player, action_data.m_Target, action_data.m_MainItem, true))
			super.Start(action_data);
	}
}
