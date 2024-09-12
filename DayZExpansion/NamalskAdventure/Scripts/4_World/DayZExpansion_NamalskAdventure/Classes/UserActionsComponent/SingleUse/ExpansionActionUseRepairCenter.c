/**
 * ExpansionActionUseRepairCenter.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef NAMALSK_SURVIVAL
class ExpansionActionUseRepairCenter: ActionInteractBase
{
	void ExpansionActionUseRepairCenter()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif
		
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_HUDCursorIcon = CursorIcons.CloseDoors;
	}

	override string GetText()
	{
		return "Use 3D Printer";
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTCursor;
	}

	override typename GetInputType()
	{
		return InteractActionInput;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{		
		Expansion_3DPrinter printer;
		if (!Class.CastTo(printer, target.GetObject()))
		{
			return false;
		}
					
		bool is_in_range = vector.Distance( printer.WorldToModel( player.GetPosition() ), printer.GetMemoryPointPos( "printer_control" ) ) < 1.5;
		return (is_in_range && !printer.GetActivation());
	}
	
	override void OnExecuteClient(ActionData action_data)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif
		
		super.OnExecuteClient(action_data);
		GetDayZGame().GetExpansionGame().GetExpansionUIManager().CreateSVMenu("NA_WorkbenchMenu");
	}
	
	override void OnExecuteServer(ActionData action_data)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif
		
		PlayerBase player;
		if (!Class.CastTo(player, action_data.m_Player))
			return;
		
		Expansion_3DPrinter printer;
		if (Class.CastTo(printer, action_data.m_Target.GetObject()))
			ExpansionNamalskModule.GetModuleInstance().SendWorkbenchData(printer, player.GetIdentity());
		
		printer.SetCanViewCargo(false);
	}
};
#endif