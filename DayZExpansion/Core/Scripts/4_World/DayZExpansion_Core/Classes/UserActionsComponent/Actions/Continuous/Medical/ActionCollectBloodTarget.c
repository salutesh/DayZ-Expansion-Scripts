/**
 * ActionCollectBloodTarget.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionCollectBloodTarget
{
	override bool ActionCondition ( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( player.IsInSafeZone() )
			return false;

		return super.ActionCondition(player, target, item);
	}
};

modded class ActionCollectBloodTargetLambda
{
	override void CopyOldPropertiesToNew (notnull EntityAI old_item, EntityAI new_item)
	{
		//! Make collecting blood from self in safezones actually decrease blood by temporarily disabling godmode
		if (m_Target.IsInSafeZone())
			m_Target.SetAllowDamage(true);

		super.CopyOldPropertiesToNew(old_item, new_item);

		if (m_Target.IsInSafeZone())
			m_Target.SetAllowDamage(false);
	}
};
