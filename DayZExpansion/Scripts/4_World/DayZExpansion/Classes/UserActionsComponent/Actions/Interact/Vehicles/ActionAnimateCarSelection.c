/**
 * ActionAnimateCarSelection.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionAnimateCarSelection
{
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		CarScript car;
		
		if ( !target || !target.GetObject() )
			return false;

		Object targetObject = target.GetObject();
		array<string> selections = new array<string>();
		targetObject.GetActionComponentNameList(target.GetComponentIndex(), selections);

		Entity targetEnt = Entity.Cast(targetObject);
		if ( Class.CastTo(car, targetEnt) )
		{
			for (int i = 0; i < selections.Count(); i++)
			{
				m_AnimSource = car.GetAnimSourceFromSelection( selections[i]);
				if ( m_AnimSource != "" )
				{
					if ( car.GetAnimationPhase( m_AnimSource ) <= 0.5 )
					{
						if ( car.IsLocked() )
							return false;
					}
				}
			}
		}

		return super.ActionCondition(player, target, item);
	}

	override bool CanBeUsedInVehicle()
	{
		return true;
	}
}