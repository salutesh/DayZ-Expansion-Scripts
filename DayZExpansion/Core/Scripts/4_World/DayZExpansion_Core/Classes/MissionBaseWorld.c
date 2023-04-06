/**
 * MissionBaseWorld.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class MissionBaseWorld
{
	//! This being here allows overriding in init.c
	bool Expansion_CheckActionSuccess(ActionBase action, ActionData action_data)
	{
		return action.Expansion_CheckSuccess(action_data);
	}
}
