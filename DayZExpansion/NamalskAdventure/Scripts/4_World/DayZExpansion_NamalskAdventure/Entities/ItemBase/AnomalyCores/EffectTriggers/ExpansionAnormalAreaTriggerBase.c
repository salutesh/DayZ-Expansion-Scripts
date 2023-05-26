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

//! @note: Base class of the area effect trigger
class ExpansionAnormalAreaTriggerBase extends EffectTrigger
{
	protected ref TStringArray m_Items = {"ItemBase"};
	protected ref TStringArray m_Players = {"SurvivorBase"};
	protected ref TStringArray m_Animals = {"AnimalBase"};
	protected ref TStringArray m_Vehicles = {"Transport"};
	protected ref TStringArray m_Infected = {"ZombieBase"};
	
	protected const float MAX_CARGODMG_INFLICTED = -5.0; //! Max. damage infliced on players gear when triggering the anomaly.
	protected const float MIN_CARGODMG_INFLICTED = -1.0; //! Min. damage infliced on players gear when triggering the anomaly.
	protected const float MAX_SHOCK_INFLICTED = -25.0; //! Max. shock damage infliced on players.
	protected const float MIN_SHOCK_INFLICTED = -20.0; //! Min. shock damage infliced on players.
	protected const float MAX_DMG_INFLICTED = -10.0; //! Max. damage infliced on players.
	protected const float MIN_DMG_INFLICTED = -5.0; //! Min. damage infliced on players.
	
	const float DAMAGE_TICK_RATE = 10; //deal damage every n-th second
	const int DAMAGE_PER_TICK = 1;

	override void OnEnterServerEvent(TriggerInsider insider)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		ExDebugPrint("::OnEnterServerEvent - Insider: " + insider.GetObject().ToString());

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
		ExDebugPrint("::OnLeaveServerEvent - Insider: " + insider.GetObject().ToString());

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
		ExDebugPrint("::OnEnterClientEvent - Insider: " + insider.GetObject().ToString());

		super.OnEnterClientEvent(insider);
	}

	override void OnLeaveClientEvent(TriggerInsider insider)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		ExDebugPrint("::OnLeaveClientEvent - Insider: " + insider.GetObject().ToString());

		super.OnLeaveClientEvent(insider);
	}

	override void OnStayStartServerEvent(int nrOfInsiders)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		m_TimeAccuStay += m_DeltaTime;
		if (m_TimeAccuStay > DAMAGE_TICK_RATE)
		{
			m_DealDamageFlag = true;
			//! @note: This is where we would normally reset the m_TimeAccuStay, but we need the value as deltaT when dealing damage to the insiders, so we reset it only after the insider update in OnStayFinishServerEvent
		}
	}

	override void OnStayFinishServerEvent()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		if (m_DealDamageFlag) //! @note: The flag was previously set to true, the insiders have been updated at this point, reset the flag and the timer as well
		{
			m_TimeAccuStay = 0;
			m_DealDamageFlag = false;
		}
	}

	override void OnStayServerEvent(TriggerInsider insider, float deltaTime)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		ExDebugPrint("::OnStayServerEvent - Insider: " + insider.GetObject().ToString());

		if (m_DealDamageFlag)
		{
			DayZCreatureAI creature = DayZCreatureAI.Cast(insider.GetObject());
			if (creature && creature.m_EffectTriggerCount != 0)
			{
				//! @note: We devide by m_EffectTriggerCount for multiple trigger presence(overlapping triggers)
				creature.DecreaseHealth("", "", DAMAGE_PER_TICK * m_TimeAccuStay / creature.m_EffectTriggerCount);
			}
		}
	}

	override string GetAmbientSoundsetName()
	{
		return "Expansion_AnomalyArea1_Soundset";
	}

	protected void ExDebugPrint(string text)
	{
		EXTrace.Print(EXTrace.NAMALSKADVENTURE, this, text);
	}
};