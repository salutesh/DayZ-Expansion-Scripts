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
	protected Expansion_Anomaly_Base m_Anomaly;

	protected ref TStringArray m_Items = {"ItemBase"};
	protected ref TStringArray m_Players = {"SurvivorBase"};
	protected ref TStringArray m_Animals = {"AnimalBase"};
	protected ref TStringArray m_Vehicles = {"CarScript"};
	protected ref TStringArray m_Infected = {"ZombieBase"};

	protected const int TRIGGER_CHECK_DELAY = 5000;

	protected bool m_IsActive;

	void ExpansionAnomalyTriggerBase()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		SetEventMask(EntityEvent.ENTER | EntityEvent.LEAVE);

		RegisterNetSyncVariableBool("m_IsActive");
	}

	void SetAnomaly(Expansion_Anomaly_Base anomaly)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		DebugTrace("::SetAnomaly - Anomaly: " + anomaly.ToString());

		m_Anomaly = anomaly;
	}

	void SetActive(bool state)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		DebugTrace("::SetActive - State: " + state);

		m_IsActive = state;
	}

	void SetTriggerRadius(int radius)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		SetCollisionSphere(radius);
	}

	//! Condition checks on given entity.
	protected bool EntityConditions(IEntity other)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		DebugTrace("::EntityConditions - Entity: " + other.ToString());
		
		Object entityObj; 
		Object.CastTo(entityObj, other);
		
		if (ExpansionStatic.IsAnyOf(entityObj, m_Items, true) || ExpansionStatic.IsAnyOf(entityObj, m_Players, true) || ExpansionStatic.IsAnyOf(entityObj, m_Animals, true) || ExpansionStatic.IsAnyOf(entityObj, m_Vehicles, true) || ExpansionStatic.IsAnyOf(entityObj, m_Infected, true))
		{
			DebugTrace("::EntityConditions - Return TRUE");
			return true;
		}

		DebugTrace("::EntityConditions - Return FALSE");
		return false;
	}

	override protected void AddInsider(Object obj)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		//! Do nothing..
	}

	override protected void RemoveInsiderByObject(Object object)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		//! Do nothing..
	}

	override void EOnEnter(IEntity other, int extra)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		DebugTrace("::EOnEnter - Entity: " + other.ToString());

		if (!EntityConditions(other))
		{
			DebugTrace("::EOnEnter - Entity conditions FALSE");
			return;
		}

		if (!m_IsActive)
		{
			DebugTrace("::EOnEnter - Trigger inactive");
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
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		if (!m_Anomaly)
			return;
		
		DebugTrace("::OnEnterAnomalyServer - Entity: " + other.ToString() + " | Anomaly: " + m_Anomaly.ToString() + " | Anomaly state: " + typename.EnumToString(ExpansionAnomalyState, m_Anomaly.GetAnomalyState()) + "| Previous anomaly state: " + typename.EnumToString(ExpansionAnomalyState, m_Anomaly.GetAnomalyState()));

		PlayerBase player = PlayerBase.Cast(other);
		if (player && HasActiveLEHSSuit(player))
			return;
					
		if (m_Anomaly.GetAnomalyCore())
		{
			m_Anomaly.DropAnormalyItems();
			//! note: Expansion_Anomaly_Base::AnomalyCoreRemoved should be called automaticly here after the core item has been detached by the Expansion_Anomaly_Base::EEItemDetached method.
			//! witch sets the anomaly state to ExpansionAnomalyState.NOCORE.
		}
		else
		{
			//! Inform anomaly about trigger activation so anomaly activation particle VFX is created and played.
			m_Anomaly.OnAnomalyZoneEnter();
		}
		
		//! note: When overriding this method you will need to call ExpansionAnomalyTriggerBase::DeferredTriggerCheck after all event calls
		//! to make sure the anomaly state is switched back to the correct state after it has been triggered (reset).
		//! --> GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DeferredTriggerCheck, TRIGGER_CHECK_DELAY);
	}

	//! Reset the trigger so it can call the trigger event again
	protected void DeferredTriggerCheck()
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);

		m_IsActive = true;

		if (m_Anomaly.GetAnomalyCore())
		{
			m_Anomaly.SetAnomalyState(ExpansionAnomalyState.IDLE);
		}
		else
		{
			m_Anomaly.SetAnomalyState(ExpansionAnomalyState.NOCORE);
		}
	}

	void OnEnterAnomalyClient(IEntity other)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		DebugTrace("::OnEnterAnomalyClient - Entity: " + other.ToString());
	}

	/*override void EOnLeave(IEntity other, int extra)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		DebugTrace("::EOnLeave - Entity: " + other.ToString());

		if (!GetGame().IsDedicatedServer())
			return;

		if (!EntityConditions(other))
			return;
	}*/
	
	protected bool HasActiveLEHSSuit(PlayerBase player)
	{
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
		
		int slot_id_lehsSuit = InventorySlots.GetSlotIdFromString("LEHS");
		dzn_lehs lehsSuit = dzn_lehs.Cast(player.GetInventory().FindAttachment(slot_id_lehsSuit));
		if (!lehsSuit || lehsSuit && lehsSuit.IsRuined())
		{
			DebugTrace("::OnEnterAnomalyServer - Return FALSE");
			return false;
		}
		
		DebugTrace("::OnEnterAnomalyServer - Player has LEHS suit: " + lehsSuit.ToString());
		
		int slot_id_Headgear = InventorySlots.GetSlotIdFromString("Headgear");
		dzn_lehs_helmet lehsHelmet = dzn_lehs_helmet.Cast(player.GetInventory().FindAttachment(slot_id_Headgear));
		if (!lehsHelmet || lehsHelmet && lehsHelmet.IsRuined())
		{
			DebugTrace("::OnEnterAnomalyServer - Return FALSE");
			return false;
		}
		
		DebugTrace("::OnEnterAnomalyServer - Player has LEHS helmet: " + lehsHelmet.ToString());
		DebugTrace("::OnEnterAnomalyServer - LEHS status: Visor up: " + lehsHelmet.IsVisorUp() + " | Has power: " + lehsHelmet.HasPower() + " | Pressurized: " + lehsHelmet.IsPressurized());
		
		if (lehsHelmet.IsVisorUp() || !lehsHelmet.HasPower() || !lehsHelmet.IsPressurized() || !lehsHelmet.HasCircuitBoard())
		{
			DebugTrace("::OnEnterAnomalyServer - Return FALSE");
			return false;
		}
		
		
		DebugTrace("::OnEnterAnomalyServer - Return TRUE");
		return true;
	}

	protected void DebugTrace(string text)
	{
		EXTrace.Start(EXTrace.NAMALSKADVENTURE, this, text);
	}
};