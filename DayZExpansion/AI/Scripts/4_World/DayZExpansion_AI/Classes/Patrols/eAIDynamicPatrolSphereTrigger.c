/**
 * eAIDynamicPatrolSphereTrigger.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class eAIDynamicPatrolSphereTrigger: Trigger
{
	static const float EAI_UPDATEINSIDERS_TICK = 1.0;

	protected float m_eAI_UpdateInsiders_Time;
	protected eAIDynamicPatrol m_eAI_Patrol;
	protected float m_eAI_InnerRadiusSq;
	protected bool m_eAI_PatrolCanSpawn;
	protected int m_eAI_InsidersInRingCount;  //! Number of insiders in ring defined by inner radius and outer radius that may trigger patrol spawn
	protected bool m_eAI_PatrolSpawnCheck;
	protected bool m_eAI_UpdatingInsiders;

	void eAI_SetParams(eAIDynamicPatrol patrol, float outerRadius, float innerRadius = 0.0)
	{
	#ifdef EXTRACE_DIAG
		auto trace = EXTrace.Start(EXTrace.AI, this, "" + GetPosition());
	#endif

		m_eAI_Patrol = patrol;
		m_eAI_InnerRadiusSq = innerRadius * innerRadius;
		SetCollisionSphere(outerRadius);
	}

	override void EOnFrame(IEntity other, float timeSlice)
	{
		m_eAI_UpdateInsiders_Time += timeSlice;
	}

	override protected bool CanAddObjectAsInsider(Object object)
	{
		PlayerBase player;
		if (Class.CastTo(player, object) && m_eAI_Patrol.CanBeTriggeredBy(player))
			return true;

		return false;
	}

	override protected bool ShouldRemoveInsider(TriggerInsider insider)
	{
		//! Remove insiders that no longer satisfy the CanAddObjectAsInsider condition (e.g. dead or faction changed to one that shouldn't trigger)
		if (!CanAddObjectAsInsider(insider.GetObject()))
			return true;

		return false;
	}

	override protected void OnEnterServerEvent(TriggerInsider insider)
	{
		//! If the inner radius is 0, then there can never be any insiders that will prevent spawn, so we can do spawn check right away
		if (m_eAI_InnerRadiusSq <= 0)
		{
			if (m_eAI_Patrol.CanSpawn())
				eAI_PatrolSpawn();
		}
		else
		{
			eAI_UpdateInsidersInRingCount(insider);

			//! Deferred patrol spawn check in case trigger was created when potential insiders were already in the area
			//! (so that OnEnterServerEvent can fire for all insiders first, thus updating insiders in ring count which is then later checked in PatrolSpawnCheck)
			if (!m_eAI_PatrolSpawnCheck)
			{
				m_eAI_PatrolSpawnCheck = true;
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(eAI_PatrolSpawnCheck, 34, false, true);
			}
		}
	}

	override protected void OnStayServerEvent(TriggerInsider insider, float deltaTime)
	{
		if (m_eAI_PatrolCanSpawn)
			eAI_UpdateInsidersInRingCount(insider);
	}

	override protected void OnLeaveServerEvent(TriggerInsider insider)
	{
		if (!m_eAI_UpdatingInsiders && m_eAI_InnerRadiusSq > 0)
		{
			//! Check if insider that left the trigger was in ring.
			//! If it was, there's no need to do a patrol spawn check (number of insiders that may block patrol spawn because they're inside trigger but outside ring i.e. too close to trigger center didn't change).
			//! If it wasn't, do a patrol spawn check (number of insiders that may block patrol spawn because they're inside trigger but outside the ring changed).
			if (!eAI_UpdateInsidersInRingCount(insider, -1))
				eAI_PatrolSpawnCheck(true);
		}
	}

	protected bool eAI_UpdateInsidersInRingCount(TriggerInsider insider, int delta = 1)
	{
		if (vector.DistanceSq(insider.GetObject().GetPosition(), GetPosition()) >= m_eAI_InnerRadiusSq)
		{
			m_eAI_InsidersInRingCount += delta;
			return true;
		}

		return false;
	}

	override protected void UpdateInsiders(int timeout)
	{
		if (m_eAI_UpdateInsiders_Time < EAI_UPDATEINSIDERS_TICK)
			return;

		m_eAI_UpdateInsiders_Time = 0.0;

		if (m_eAI_InnerRadiusSq > 0)
		{
			m_eAI_PatrolCanSpawn = m_eAI_Patrol.CanSpawn();
			m_eAI_InsidersInRingCount = 0;
		}

		m_eAI_UpdatingInsiders = true;

		super.UpdateInsiders(timeout);

		m_eAI_UpdatingInsiders = false;

		if (m_eAI_InnerRadiusSq > 0)
			eAI_PatrolSpawnCheck(false);
	}

	protected void eAI_PatrolSpawnCheck(bool checkCanSpawn = true)
	{
		if (checkCanSpawn)
			m_eAI_PatrolCanSpawn = m_eAI_Patrol.CanSpawn();

		if (m_eAI_PatrolCanSpawn && m_eAI_InsidersInRingCount > 0 && m_eAI_InsidersInRingCount == GetInsiders().Count())
			eAI_PatrolSpawn();

		m_eAI_PatrolSpawnCheck = false;
	}

	protected void eAI_PatrolSpawn()
	{
		m_eAI_Patrol.Spawn();
		Delete();  //! Deletes trigger in next frame
	}
};
