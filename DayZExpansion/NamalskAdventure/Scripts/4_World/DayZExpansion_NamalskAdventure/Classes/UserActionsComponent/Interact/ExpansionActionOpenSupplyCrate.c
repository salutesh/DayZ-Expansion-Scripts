/**
 * ExpansionActionOpenSupplyCrate.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionActionOpenSupplyCrate: ActionInteractBase
{
	protected bool m_IsOpening = true;

	void ExpansionActionOpenSupplyCrate()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENLID;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ALL;
		m_LockTargetOnUse = false;
		m_IsOpening	= true;
		m_Text = "Open Crate";
	}

	override void CreateConditionComponents()
	{
		m_ConditionItem	= new CCINone;
		m_ConditionTarget = new CCTNone;
	}
	
	override bool ActionCondition(PlayerBase player, ActionTarget target, ItemBase item)
	{
		if (!IsInReach(player, target, UAMaxDistances.DEFAULT)) 
			return false;

		Expansion_SupplyCrate_Base crate = Expansion_SupplyCrate_Base.Cast(target.GetObject());
		if (crate)
		{
			m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW;
			float animationPhase = crate.GetAnimationPhase("Door1");			
			return (m_IsOpening && animationPhase <= 0.5 && crate.HasKey()) || (!m_IsOpening && animationPhase > 0.5 && crate.HasKey());
		}
		
		return false;
	}
	
	override void OnStartServer(ActionData action_data)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		float phase;

		if (m_IsOpening)
		{
			phase = 1.0;
		}
		else
		{
			phase = 0.0;
		}
		
		Expansion_SupplyCrate_Base crate = Expansion_SupplyCrate_Base.Cast(action_data.m_Target.GetObject());
		if (crate)
		{
			crate.OpenCrate();
		}
	}

	protected void DebugTrace(string text)
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		EXTrace.Start(EXTrace.NAMALSKADVENTURE, this, text);
	#endif
	}
};