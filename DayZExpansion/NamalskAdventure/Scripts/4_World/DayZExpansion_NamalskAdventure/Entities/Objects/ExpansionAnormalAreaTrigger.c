/**
 * ExpansionAnormalAreaTrigger.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

// In this file you will find both Static and Dynamic contaminated area triggers
class ExpansionAnormalAreaTrigger extends EffectTrigger
{
	const float DAMAGE_TICK_RATE = 10; //deal damage every n-th second
	const int DAMAGE_PER_TICK = 1;

	// ----------------------------------------------
	// 				TRIGGER EVENTS
	// ----------------------------------------------

	override void OnEnterServerEvent(TriggerInsider insider)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		DebugTrace("::OnEnterServerEvent - Insider: " + insider.GetObject().ToString());

		super.OnEnterServerEvent(insider);
		
		if (insider)
		{
			PlayerBase playerInsider = PlayerBase.Cast(insider.GetObject());
			if (playerInsider)
			{
				playerInsider.GetModifiersManager().ActivateModifier(eModifiers.MDF_AREAEXPOSURE);
			}
			else
			{
				DayZCreatureAI creature = DayZCreatureAI.Cast(insider.GetObject());
				if (creature)
					creature.IncreaseEffectTriggerCount();
			}
		}

	}

	override void OnLeaveServerEvent(TriggerInsider insider)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		DebugTrace("::OnLeaveServerEvent - Insider: " + insider.GetObject().ToString());

		super.OnLeaveServerEvent(insider);

		if (insider)
		{
			PlayerBase playerInsider = PlayerBase.Cast(insider.GetObject());

			if (playerInsider && !m_Manager.IsPlayerInTriggerType(playerInsider, this))
			{
				playerInsider.GetModifiersManager().DeactivateModifier(eModifiers.MDF_AREAEXPOSURE);
			}
			else
			{
				DayZCreatureAI creature = DayZCreatureAI.Cast(insider.GetObject());
				if (creature)
					creature.DecreaseEffectTriggerCount();
			}
		}
	}

	override void OnEnterClientEvent(TriggerInsider insider)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		DebugTrace("::OnEnterClientEvent - Insider: " + insider.GetObject().ToString());
		
		super.OnEnterClientEvent(insider);
	}

	override void OnLeaveClientEvent(TriggerInsider insider)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		DebugTrace("::OnLeaveClientEvent - Insider: " + insider.GetObject().ToString());

		super.OnLeaveClientEvent(insider);
	}

	override void OnStayStartServerEvent(int nrOfInsiders)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		m_TimeAccuStay += m_DeltaTime;
		if (m_TimeAccuStay > DAMAGE_TICK_RATE)
		{
			m_DealDamageFlag = true;
			//! This is where we would normally reset the m_TimeAccuStay, but we need the value as deltaT when dealing damage to the insiders, so we reset it only after the insider update in OnStayFinishServerEvent
		}
	}

	override void OnStayFinishServerEvent()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (m_DealDamageFlag) //! The flag was previously set to true, the insiders have been updated at this point, reset the flag and the timer as well
		{
			m_TimeAccuStay = 0;
			m_DealDamageFlag = false;
		}
	}

	override void OnStayServerEvent(TriggerInsider insider, float deltaTime)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		DebugTrace("::OnStayServerEvent - Insider: " + insider.GetObject().ToString());

		if (m_DealDamageFlag)
		{
			DayZCreatureAI creature = DayZCreatureAI.Cast(insider.GetObject());
			if (creature && creature.m_EffectTriggerCount != 0)
			{
				creature.DecreaseHealth("", "", DAMAGE_PER_TICK * m_TimeAccuStay / creature.m_EffectTriggerCount);// we devide by m_EffectTriggerCount for multiple trigger presence(overlapping triggers)
			}
			
			PlayerBase player = PlayerBase.Cast(insider.GetObject());
			if (player)
			{
				creature = DayZCreatureAI.Cast(insider.GetObject());
				if (creature && creature.m_EffectTriggerCount != 0)
				{
					float heatComfort = player.GetStatHeatComfort().Get();
					heatComfort = heatComfort - 1000 * m_TimeAccuStay / creature.m_EffectTriggerCount;
					player.GetStatHeatComfort().Set(heatComfort); //! Let the character feel the cold.
				}
			}
		}
	}

	override string GetAmbientSoundsetName()
	{
		return "Expansion_AnomalyArea1_Soundset";
	}
	
	protected void DebugTrace(string text)
	{
		EXTrace.Start(EXTrace.NAMALSKADVENTURE, this, text);
	}
};