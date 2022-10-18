/**
 * ExpansionActionCrackSafe.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONMODBASEBUILDING
modded class ExpansionActionCrackSafe
{
	override void OnFinishProgressServer(ActionData action_data)
	{
		super.OnFinishProgressServer(action_data);

		if (GetExpansionSettings().GetHardline().UseHumanity && GetExpansionSettings().GetHardline().HumanityDestroySafe > 0)
		{
			PlayerBase ntarget;
			Class.CastTo(ntarget, action_data.m_Target.GetObject());
			if (action_data.m_Player != ntarget)
			{
				m_HardlineModule.OnPlayerAction(action_data.m_Player, GetExpansionSettings().GetHardline().HumanityDestroySafe);
			}
		}
	}
};
#endif