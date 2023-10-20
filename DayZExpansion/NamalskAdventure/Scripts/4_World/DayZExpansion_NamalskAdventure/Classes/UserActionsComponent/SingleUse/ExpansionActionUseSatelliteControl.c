/**
 * ExpansionActionUseSatelliteControl.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionUseSatelliteControl: ActionInteractBase
{
	void ExpansionActionUseSatelliteControl()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_HUDCursorIcon = CursorIcons.CloseDoors;
	}

	override string GetText()
	{
		return "Activate Antenna";
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

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		Expansion_Satellite_Control satControl = Expansion_Satellite_Control.Cast(target.GetObject());
		if (!satControl) 
			return false;
	
	#ifdef SERVER
		if (!satControl.CanActivate()) 
			return false;
	#endif
		
		bool is_in_range = vector.Distance(satControl.WorldToModel(player.GetPosition()), "0.0 0.7 0.4") < 2;
		return (is_in_range);
	}

	override void OnStartServer(ActionData action_data)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		Expansion_Satellite_Control satControl = Expansion_Satellite_Control.Cast(action_data.m_Target.GetObject());
		if (satControl)
			satControl.StartSatellite();
		
		Expansion_KeyCard_NA_Antenna keyCard = Expansion_KeyCard_NA_Antenna.Cast(satControl.FindAttachmentBySlotName("Att_ExpansionKeyCardAntenna"));
		if (keyCard)
			keyCard.OnCardUsed();
	}
	
	protected void ExDebugPrint(string text)
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		EXTrace.Print(EXTrace.NAMALSKADVENTURE, this, text);
	#endif
	}
};