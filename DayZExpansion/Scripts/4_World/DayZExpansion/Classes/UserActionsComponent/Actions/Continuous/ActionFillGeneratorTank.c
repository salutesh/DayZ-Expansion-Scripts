/**
 * ActionFillGeneratorTank.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionFillGeneratorTank
{
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		ExpansionGenerator_Base pg;
		if ( Class.CastTo(pg,target.GetObject()) )
		{
			string selection = pg.GetActionComponentName( target.GetComponentIndex() );
			if ( pg.IsFuelTank( selection ) )
			{
				return pg.CanAddFuel( item );
			}
			return false;
		}
		
		return super.ActionCondition(player,target,item);
	}
};
