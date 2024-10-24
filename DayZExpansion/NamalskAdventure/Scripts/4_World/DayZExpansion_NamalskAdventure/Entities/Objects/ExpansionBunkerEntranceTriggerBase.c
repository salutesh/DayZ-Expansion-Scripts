/**
 * ExpansionBunkerEntranceTriggerBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionBunkerEntranceTriggerBase: Trigger
{
	protected ref array<PlayerBase> m_TeleportingPlayers;

	protected const vector m_A3_Bunker_StartPos = "1922.738037 241.701569 1225.019409";
	protected const vector m_A3_Bunker_StartOri = "290.000000 -0.000000 -0.000000";

	void ExpansionBunkerEntranceTriggerBase()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif
		SetEventMask(EntityEvent.ENTER | EntityEvent.LEAVE);

		m_TeleportingPlayers = new array<PlayerBase>;
	}

	override protected void AddInsider(Object obj)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif
		//! Do nothing..
	}

	override protected void RemoveInsiderByObject(Object object)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif
		//! Do nothing..
	}

	override void EOnEnter(IEntity other, int extra)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		if (!EntityConditions(other))
			return;

		PlayerBase player = PlayerBase.Cast(other);
		if (player)
		{
			DayZPlayerSyncJunctures.ExpansionTeleport(player, m_A3_Bunker_StartPos, m_A3_Bunker_StartOri);
		}
	}

	override void EOnLeave(IEntity other, int extra)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif

		PlayerBase player = PlayerBase.Cast(other);
		int findIndex = m_TeleportingPlayers.Find(player);
		if (player && findIndex > -1)
			m_TeleportingPlayers.RemoveOrdered(findIndex);
	}

	//! Condition checks on given entity.
	protected bool EntityConditions(IEntity other)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif
		ExDebugPrint("::EntityConditions - Entity: " + other.ToString());

		PlayerBase player = PlayerBase.Cast(other);
		if (player && m_TeleportingPlayers.Find(player) == -1)
		{
			return true;
		}

		ExDebugPrint("::EntityConditions - Return FALSE");
		return false;
	}

	void SetTriggerRadius(int radius)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.NAMALSKADVENTURE, this);
#endif
		SetCollisionSphere(radius);
	}

	protected void ExDebugPrint(string text)
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE_DEBUG
		EXTrace.Print(EXTrace.NAMALSKADVENTURE, this, text);
	#endif
	}
};