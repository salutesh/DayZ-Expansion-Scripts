/**
 * ActionForceConsume.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ActionForceConsume
{
	int m_QuantityOnActionStart = 0;

	override void OnStartAnimationLoop( ActionData action_data )
	{
		super.OnStartAnimationLoop( action_data );

		if ( action_data.m_MainItem )
		{
			m_QuantityOnActionStart = action_data.m_MainItem.GetQuantity();
		}
	}

	override void OnEndServer( ActionData action_data )
	{
		super.OnEndServer(action_data);

		if (action_data.m_MainItem.IsLiquidContainer())
			return;
		
		int quantityAfterAction = action_data.m_MainItem.GetQuantity();
		int quantityDiff = m_QuantityOnActionStart - quantityAfterAction;

		PlayerBase ntarget;
		Class.CastTo(ntarget, action_data.m_Target.GetObject());
		if (action_data.m_Player != ntarget && quantityDiff > 0)
		{
			if (action_data.m_MainItem.IsInherited(HumanSteakMeat))
			{
				m_HardlineModule.OnPlayerNegativeAction(action_data.m_Player, quantityDiff);
			}
			else
			{
				m_HardlineModule.OnPlayerPositiveAction(action_data.m_Player, quantityDiff);
			}
		}
	}
};