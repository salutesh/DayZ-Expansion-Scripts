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

/*class ExpansionHardlineActionData : ActionData
{
	int m_QuantityOnActionStart = 0;
	int m_LiquidType;
};

class ExpansionHardlineActionReciveData : ActionReciveData
{
	int m_QuantityOnActionStart;
	int m_LiquidType;
};

modded class ActionForceConsume
{
	override ActionData CreateActionData()
	{
		ActionData action_data = new ExpansionHardlineActionData;
		return action_data;
	}
	
	override void WriteToContext(ParamsWriteContext ctx, ActionData action_data)
	{
		super.WriteToContext(ctx, action_data);

		ExpansionHardlineActionData hardlineActionData = ExpansionHardlineActionData.Cast(action_data);

		ctx.Write(hardlineActionData.m_QuantityOnActionStart);
	}

	override bool ReadFromContext(ParamsReadContext ctx, out ActionReciveData action_recive_data)
	{
		if (!action_recive_data)
		{
			action_recive_data = new ExpansionHardlineActionReciveData;
		}

		super.ReadFromContext(ctx, action_recive_data);

		ExpansionHardlineActionReciveData action_data_hardline = ExpansionHardlineActionReciveData.Cast(action_recive_data);

		if (!ctx.Read(action_data_hardline.m_QuantityOnActionStart))
			return false;

		return true;
	}

	override void HandleReciveData(ActionReciveData action_recive_data, ActionData action_data)
	{
		super.HandleReciveData(action_recive_data, action_data);

		ExpansionHardlineActionReciveData recive_data_po = ExpansionHardlineActionReciveData.Cast(action_recive_data);
		ExpansionHardlineActionData action_data_po = ExpansionHardlineActionData.Cast(action_data);

		action_data_po.m_QuantityOnActionStart = recive_data_po.m_QuantityOnActionStart;
	}	
	
	override void OnStartAnimationLoop( ActionData action_data )
	{
		super.OnStartAnimationLoop( action_data );

		if ( action_data.m_MainItem )
		{
			ExpansionHardlineActionData action_data_po = ExpansionHardlineActionData.Cast(action_data);
			action_data_po.m_QuantityOnActionStart = action_data.m_MainItem.GetQuantity();
		}
	}

	override void OnEndServer( ActionData action_data )
	{
		super.OnEndServer(action_data);

		if (action_data.m_MainItem.IsLiquidContainer())
			return;
		
		ExpansionHardlineActionData action_data_po = ExpansionHardlineActionData.Cast(action_data);
		
		int quantityAfterAction = action_data.m_MainItem.GetQuantity();
		int quantityDiff = action_data_po.m_QuantityOnActionStart - quantityAfterAction;

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
};*/