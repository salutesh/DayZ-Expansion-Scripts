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

class ExpansionActionUseSatelliteControl: ActionSingleUseBase
{
	void ExpansionActionUseSatelliteControl()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		m_Text = "#use";
	}

	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTCursor;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_ATTACHITEM;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
	}

	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (item.IsDamageDestroyed())
			return false;

		Expansion_Satellite_Control trgt = Expansion_Satellite_Control.Cast(target.GetObject());
		if (!trgt) 
			return false;
		
		return trgt.CanActivate();
	}

	override void OnStartServer(ActionData action_data)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		Expansion_Satellite_Control satControl = Expansion_Satellite_Control.Cast(action_data.m_Target.GetObject());
		if (satControl)
			satControl.StartSatellite();
		
		Expansion_KeyCard_NA_Antenna card = Expansion_KeyCard_NA_Antenna.Cast(action_data.m_MainItem);
		if (card)
			card.OnCardUsed();
	}
	
	protected void ExDebugPrint(string text)
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		EXTrace.Print(EXTrace.NAMALSKADVENTURE, this, text);
	#endif
	}
};