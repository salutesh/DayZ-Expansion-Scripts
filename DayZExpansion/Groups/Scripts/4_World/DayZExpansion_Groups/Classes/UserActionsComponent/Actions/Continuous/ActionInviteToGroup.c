/**
 * ActionInviteToGroup.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ActionInviteToGroup: ActionInteractBase
{
	void ActionInviteToGroup()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_INTERACTONCE;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
		m_Text = "#STR_EXPANSION_PARTY_INVITE_PLAYER";
	}

	override void CreateConditionComponents()  
	{
		m_ConditionTarget = new CCTMan(UAMaxDistances.DEFAULT);
		m_ConditionItem = new CCINone;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		PlayerBase targetPlayer = PlayerBase.Cast(target.GetParentOrObject());
		
		if (!targetPlayer || !targetPlayer.IsAlive())
			return false;

	#ifdef ENFUSION_AI_PROJECT
		if (targetPlayer.IsAI())
			return false;  //! TODO: Maybe integrate with AI group system?
	#endif
		
	#ifdef EXPANSIONMODMARKET
		if (targetPlayer.IsInherited(ExpansionTraderNPCBase))
			return false;
	#endif
		
		if (!targetPlayer.GetIdentity())
			return false;
		
		ExpansionPartyModule partyModule;
		if (GetGame().IsClient() && CF_Modules<ExpansionPartyModule>.Get(partyModule))
			return partyModule.GetParty() != null;

		return player.Expansion_GetParty() && !targetPlayer.Expansion_GetParty();
	}
	
	override void OnExecuteClient( ActionData action_data )
	{
		Object target = action_data.m_Target.GetParentOrObject();
		PlayerBase targetPlayer = PlayerBase.Cast(target);
		if (targetPlayer)
		{
			ExpansionPartyModule partyModule = ExpansionPartyModule.Cast(CF_ModuleCoreManager.Get(ExpansionPartyModule));
			string targetUID = targetPlayer.GetIdentity().GetId();
			partyModule.InvitePlayer(targetUID);
		}
	}
}