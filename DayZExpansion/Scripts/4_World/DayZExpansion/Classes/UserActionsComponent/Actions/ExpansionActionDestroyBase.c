/**
 * ExpansionActionDestroyBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/*! @brief Provides base classes for other "destroy"-type actions (e.g. for raiding).
    The item to be destroyed is damaged over time in cycles until m_MinHealth01 is reached,
    and the tool used for destroying is damaged or depleted in the process.
    Tools that have an energy manager and quantity will not be damaged by performing the action,
    instead they need to be turned on and should loose quantity as they consume energy on their own.
    Other tools will receive the configured amount of tool damage.
    Each succesfully completed cycle is logged to the admin log.

	NOTE you should not override the following methods in derived actions:

	`CreateAndSetupActionCallback` (override `Setup` instead)
	`ActionCondition` (override `DestroyCondition` and/or `CanBeDestroyed` instead)
	`OnFinishProgressServer`

	See `ExpansionActionDestroyBarbedWire` and `ExpansionActionCrackSafe` for examples.
*/

class ExpansionActionDestroyBase : ExpansionActionToolBase
{
	void ExpansionActionDestroyBase()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_DISASSEMBLE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_SpecialtyWeight = UASoftSkillsWeight.ROUGH_HIGH;
		m_FullBody = true;
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}

	override string GetText()
	{
		return "#destroy" + " " + m_TargetName;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( !super.ActionCondition( player, target, item ) )
			return false;

		if ( !DestroyCondition( player, target, item, false ) )
			return false;

		return true;
	}

	bool DestroyCondition( PlayerBase player, ActionTarget target, ItemBase item, bool camera_check )
	{
		return CanBeDestroyed( target.GetParentOrObject() );
	}

	bool CanBeDestroyed( Object targetObject )
	{
		return true;
	}

	override void OnFinishProgressServer( ActionData action_data )
	{
		if ( !CanBeDestroyed( action_data.m_Target.GetParentOrObject() ) )
			return;

		ItemBase targetItem = GetTargetItem( action_data.m_Target.GetParentOrObject() );

		if ( targetItem )
		{
			float curHealth = targetItem.GetHealth();
			float maxHealth = targetItem.GetMaxHealth( "", "Health" );
			float minHealth = Math.Floor( maxHealth * m_MinHealth01 );
			float dmg = Math.Ceil( ( maxHealth - minHealth ) / m_Cycles );

			if ( curHealth > minHealth )
			{
				if ( curHealth - dmg < minHealth )
					dmg = curHealth - minHealth;

				targetItem.DecreaseHealth( dmg );

				super.OnFinishProgressServer( action_data );
			} else
			{
				dmg = 0;
			}

			targetItem.RaidLog( action_data.m_MainItem, "", curHealth, dmg, 1.0 );

			if ( targetItem.GetHealth() <= minHealth )
			{
				if ( !targetItem.IsDamageDestroyed() )
					targetItem.ExpansionOnDestroyed( action_data.m_Player );
			}
		}
	}
}
