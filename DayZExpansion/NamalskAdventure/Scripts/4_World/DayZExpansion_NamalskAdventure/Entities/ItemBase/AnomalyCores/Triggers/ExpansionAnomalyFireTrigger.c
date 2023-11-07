/**
 * ExpansionAnomalyFireTrigger.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionAnomalyFireTrigger: ExpansionAnomalyTriggerBase
{
	override void OnEnterAnomalyServer(IEntity other)
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	#endif
		ExDebugPrint("::OnEnterAnomalyServer - Entity: " + other.ToString());

		super.OnEnterAnomalyServer(other);

		ProcessEntityEvents(other);

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DeferredTriggerCheck, TRIGGER_CHECK_DELAY);
	}

	protected void ProcessEntityEvents(IEntity other)
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	#endif
		ExDebugPrint("::ProcessEntityEvents - Entity: " + other.ToString());

		if (other)
		{
			EntityAI objectEntity = EntityAI.Cast(other);
		}
	}
};