/**
 * ExpansionAnomalyTriggerBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionAnomalyTriggerBase: Trigger
{
	static ref TStringArray s_Items = {"ItemBase"};
	static ref TStringArray s_Players = {"SurvivorBase"};
	//static ref TStringArray s_Animals = {"AnimalBase"};
	static ref TStringArray s_Vehicles = {"Transport"};
	//static ref TStringArray s_Infected = {"ZombieBase"};

	protected const float MAX_CARGODMG_INFLICTED = -5.0; //! Max. damage infliced on players gear when triggering the anomaly.
	protected const float MIN_CARGODMG_INFLICTED = -1.0; //! Min. damage infliced on players gear when triggering the anomaly.
	protected const float MAX_SHOCK_INFLICTED = -25.0; //! Max. shock damage infliced on players.
	protected const float MIN_SHOCK_INFLICTED = -20.0; //! Min. shock damage infliced on players.
	protected const float MAX_DMG_INFLICTED = -10.0; //! Max. damage infliced on players.
	protected const float MIN_DMG_INFLICTED = -5.0; //! Min. damage infliced on players.

	protected Expansion_Anomaly_Base m_Anomaly;
	protected const int TRIGGER_CHECK_DELAY = 5000;
	protected bool m_IsActive;

	void ExpansionAnomalyTriggerBase()
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	#endif

		SetEventMask(EntityEvent.ENTER | EntityEvent.LEAVE | EntityEvent.CONTACT | EntityEvent.TOUCH);

		RegisterNetSyncVariableBool("m_IsActive");
	}

	void SetAnomaly(Expansion_Anomaly_Base anomaly)
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	#endif
		ExDebugPrint("::SetAnomaly - Anomaly: " + anomaly.ToString());

		m_Anomaly = anomaly;
	}

	void SetActive(bool state)
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	#endif
		ExDebugPrint("::SetActive - State: " + state);

		m_IsActive = state;
	}

	void SetTriggerRadius(int radius)
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	#endif
		SetCollisionSphere(radius);
	}

	//! Condition checks on given entity.
	protected bool EntityConditions(IEntity other)
	{	
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	#endif
		ExDebugPrint("::EntityConditions - Entity: " + other.ToString());

		EntityAI entityObj = EntityAI.Cast(other);
		if (entityObj)
		{
			if (ExpansionStatic.IsAnyOf(entityObj, s_Players, true) || ExpansionStatic.IsAnyOf(entityObj, s_Vehicles, true) || ExpansionStatic.IsAnyOf(entityObj, s_Items, true))
			{
			#ifdef NAMALSK_SURVIVAL
				PlayerBase player = PlayerBase.Cast(other);
				if (player && ExpansionAnomaliesModule.GetModuleInstance().HasActiveLEHSSuit(player))
				{
					ExDebugPrint("::EntityConditions - Return FALSE. Entity is player and has LEHS suit!");
					return false;
				}
			#endif
	
				ExDebugPrint("::EntityConditions - Return TRUE");
				return true;
			}
		}
		else
		{
			return true;
		}
		
		ExDebugPrint("::EntityConditions - Return FALSE");
		return false;
	}

	override protected void AddInsider(Object obj)
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	#endif
		//! Do nothing..
	}

	override protected void RemoveInsiderByObject(Object object)
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	#endif
		//! Do nothing..
	}

	/*override void EOnContact(IEntity other, Contact extra)
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	#endif
		ExDebugPrint("::EOnContact - Entity: " + other.ToString());
		
		if (!EntityConditions(other))
		{
			ExDebugPrint("::EOnEnter - Entity conditions FALSE");
			return;
		}

		if (!m_IsActive)
		{
			ExDebugPrint("::EOnEnter - Trigger inactive");
			return;
		}

		if (!GetGame().IsDedicatedServer())
		{
			OnEnterAnomalyClient(other);
		}

		if (GetGame().IsServer())
		{
			OnEnterAnomalyServer(other);
		}
	}
	
	override void EOnTouch(IEntity other, int extra)
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	#endif
		ExDebugPrint("::EOnTouch - Entity: " + other.ToString());
		
		if (!EntityConditions(other))
		{
			ExDebugPrint("::EOnEnter - Entity conditions FALSE");
			return;
		}

		if (!m_IsActive)
		{
			ExDebugPrint("::EOnEnter - Trigger inactive");
			return;
		}

		if (!GetGame().IsDedicatedServer())
		{
			OnEnterAnomalyClient(other);
		}

		if (GetGame().IsServer())
		{
			OnEnterAnomalyServer(other);
		}
	}*/

	override void EOnEnter(IEntity other, int extra)
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	#endif
		ExDebugPrint("::EOnEnter - Entity: " + other.ToString());

		if (!EntityConditions(other))
		{
			ExDebugPrint("::EOnEnter - Entity conditions FALSE");
			return;
		}

		if (!m_IsActive)
		{
			ExDebugPrint("::EOnEnter - Trigger inactive");
			return;
		}

		if (!GetGame().IsDedicatedServer())
		{
			OnEnterAnomalyClient(other);
		}

		if (GetGame().IsServer())
		{
			OnEnterAnomalyServer(other);
		}
	}

	void OnEnterAnomalyServer(IEntity other)
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	#endif

		if (!m_Anomaly)
			return;

		ExDebugPrint("::OnEnterAnomalyServer - Entity: " + other.ToString() + " | Anomaly: " + m_Anomaly.ToString() + " | Position: " + m_Anomaly.GetPosition() + " | Anomaly state: " + typename.EnumToString(ExpansionAnomalyState, m_Anomaly.GetAnomalyState()) + "| Previous anomaly state: " + typename.EnumToString(ExpansionAnomalyState, m_Anomaly.GetAnomalyState()));

		//! Inform anomaly about trigger activation so anomaly activation particle VFX is created and played.
		m_Anomaly.OnAnomalyZoneEnter();

		//! @note: MiscGameplayFunctions.Expansion_HasAnyCargo does not return true when the anomaly has a attached core in its core slot so we check for the core entity also here.
		if (MiscGameplayFunctions.Expansion_HasAnyCargo(m_Anomaly) || m_Anomaly.HasAnomalyCore())
		{
			m_Anomaly.DropAnormalyItems();
			//! @note: Expansion_Anomaly_Base::AnomalyCoreRemoved should be called automaticly here after the core item has been detached by the Expansion_Anomaly_Base::EEItemDetached method.
			//! witch sets the anomaly state to ExpansionAnomalyState.NOCORE.
		}

		//! @note: When overriding this method you will need to call ExpansionAnomalyTriggerBase::DeferredTriggerCheck after all event calls
		//! to make sure the anomaly state is switched back to the correct state after it has been triggered (reset).
		//! --> GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DeferredTriggerCheck, TRIGGER_CHECK_DELAY);
	}

	//! Reset the trigger so it can call the trigger event again
	protected void DeferredTriggerCheck()
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	#endif

		m_IsActive = true;
		bool evrStromActive = ExpansionNamalskModule.GetModuleInstance().IsEVRStormActive();
		if (!evrStromActive)
		{
			if (m_Anomaly.HasAnomalyCore())
			{
				m_Anomaly.SetAnomalyState(ExpansionAnomalyState.IDLE);
			}
			else
			{
				m_Anomaly.SetAnomalyState(ExpansionAnomalyState.NOCORE);
			}
		}
		else
		{
			if (m_Anomaly.HasAnomalyCore())
			{
				m_Anomaly.SetAnomalyState(ExpansionAnomalyState.UNSTABLE);
			}
			else
			{
				m_Anomaly.SetAnomalyState(ExpansionAnomalyState.UNSTABLENOCORE);
			}
		}
	}

	void OnEnterAnomalyClient(IEntity other)
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
	#endif
		ExDebugPrint("::OnEnterAnomalyClient - Entity: " + other.ToString());
	}

	protected void ExDebugPrint(string text)
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		EXTrace.Print(EXTrace.NAMALSKADVENTURE, this, text);
	#endif
	}
};