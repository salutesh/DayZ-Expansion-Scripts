/**
 * ExpansionActionUseTeleporter.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSIONMODTELEPORTER
class ExpansionActionUseTeleporter: ActionInteractBase
{
	void ExpansionActionUseTeleporter()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
		m_HUDCursorIcon = CursorIcons.CloseDoors;
	}
	
	override string GetText()
	{
		return "Activate Teleporter";
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
		Expansion_Teleporter_Big teleporter = Expansion_Teleporter_Big.Cast(target.GetObject());
		if (!teleporter) 
			return false;
		
		Expansion_KeyCard_Teleporter keyCard = Expansion_KeyCard_Teleporter.Cast(player.GetHumanInventory().GetEntityInHands());
		if (!keyCard || keyCard && keyCard.IsRuined())
			return false;
		
		string selection = teleporter.GetActionComponentName(target.GetComponentIndex());
		if (selection != "panel")
			return false;
		
		return !teleporter.IsActive();
	}

	override void OnStartServer(ActionData action_data)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		Expansion_Teleporter_Big teleporter = Expansion_Teleporter_Big.Cast(action_data.m_Target.GetObject());
		Expansion_KeyCard_Teleporter keyCard = Expansion_KeyCard_Teleporter.Cast(action_data.m_Player.GetHumanInventory().GetEntityInHands());
		
		int teleporterID = teleporter.GetTeleporterID();
		string playerUID = action_data.m_Player.GetIdentity().GetId();
		
		if (teleporter && keyCard)
		{
			if (!ExpansionTeleporterModule.GetModuleInstance().CanUseTeleporter(teleporterID, playerUID))
				ExpansionTeleporterModule.GetModuleInstance().OnTeleporterKeyCardUsed(teleporter, keyCard);
		}
	}
	
	protected void ExDebugPrint(string text)
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		EXTrace.Print(EXTrace.NAMALSKADVENTURE, this, text);
	#endif
	}
};
#endif
