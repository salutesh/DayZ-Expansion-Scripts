/**
 * ActionDetach.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionDetach
{
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		EntityAI tgt_parent = EntityAI.Cast( target.GetParent() );
		CarScript car;

		if ( Class.CastTo( car, target.GetParent() ) )
		{
			if ( car.IsLocked() )
			{
				return false;
			}
		}

		return super.ActionCondition( player, target, item );
	}

	override void OnExecuteServer( ActionData action_data )
	{
		super.OnExecuteServer(action_data);

		CarScript car;
		if (Class.CastTo(car, action_data.m_Target.GetParent()))
		{
			if (action_data.m_Player && action_data.m_Player.GetIdentity() && GetExpansionSettings().GetLog().VehicleAttachments)
			{
				GetExpansionSettings().GetLog().PrintLog("[VehicleAttachments] Player " + action_data.m_Player.GetIdentity().GetName() + " (uid=" + action_data.m_Player.GetIdentity().GetId() + ") detached " + action_data.m_Target.GetObject().GetDisplayName() + " (type=" + action_data.m_Target.GetObject().GetType() + ") from vehicle " + car.GetDisplayName() + "(pos=" + car.GetPosition() + " type=" + car.GetType() + ")");
			}
		}
	}
}