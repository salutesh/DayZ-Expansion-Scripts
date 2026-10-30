/**
 * ActionCloseFence.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionCloseFence
{
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		bool ret = super.ActionCondition( player, target, item );

		Object targetObject = target.GetObject();

		if (!ret)
		{
			if ( targetObject && targetObject.CanUseConstruction() )
			{
				Fence fence = Fence.Cast( targetObject );
				
				if ( fence && fence.CanCloseFence() && fence.GetType() != "Fence" )
					ret = true;
			}
		}

		if (ret)
		{
			ItemBase tgtItem;
			if (Class.CastTo(tgtItem, targetObject))
			{
				if (ExpansionActionClose.Expansion_CloseAndLockCondition(tgtItem))
					m_Text = "#STR_EXPANSION_BB_CODE_CLOSE_LOCK";
				else
					m_Text = "#close";
			}
		}
		
		return ret;
	}

	override void OnEndServer(ActionData action_data)
	{
		super.OnEndServer(action_data);

		ItemBase tgtItem;
		if (Class.CastTo(tgtItem, action_data.m_Target.GetObject()) && ExpansionActionClose.Expansion_CloseAndLockCondition(tgtItem))
			tgtItem.ExpansionLock();
	}
}
