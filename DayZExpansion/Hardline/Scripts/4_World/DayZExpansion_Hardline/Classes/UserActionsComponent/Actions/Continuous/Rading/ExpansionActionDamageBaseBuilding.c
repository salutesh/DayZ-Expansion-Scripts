/**
 * ExpansionActionDamageBaseBuilding.c
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
modded class ExpansionActionDamageBaseBuilding
{
	override void OnFinishProgressServer(ActionData action_data)
	{
		super.OnFinishProgressServer(action_data);

		if (GetExpansionSettings().GetHardline().UseHumanity && GetExpansionSettings().GetHardline().HumanityDestroyBaseBuilding > 0)
		{
			PlayerBase ntarget;
			Class.CastTo(ntarget, action_data.m_Target.GetObject());
			if (action_data.m_Player != ntarget)
			{
				BaseBuildingBase base_building = BaseBuildingBase.Cast(action_data.m_Target.GetObject());				
				if (base_building && (base_building.IsRuined() || base_building.IsAlive()))
					m_HardlineModule.OnPlayerAction(action_data.m_Player, GetExpansionSettings().GetHardline().HumanityDestroyBaseBuilding);
			}
		}
	}
};
#endif