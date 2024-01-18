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

	void eAI_SetParams(eAIDynamicPatrol patrol, float outerRadius, float innerRadius = 0.0)
	{
	#ifdef DIAG
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
		Man player;
		if (Class.CastTo(player, object) && player.GetIdentity())
			return true;

		return false;
	}

	override protected void OnEnterServerEvent(TriggerInsider insider)
	{
		eAI_PatrolSpawnCheck(insider);
	}

	override protected void OnStayServerEvent(TriggerInsider insider, float deltaTime)
	{
		eAI_PatrolSpawnCheck(insider);
	}

	override protected void UpdateInsiders(int timeout)
	{
		if (m_eAI_UpdateInsiders_Time < EAI_UPDATEINSIDERS_TICK)
			return;

		m_eAI_UpdateInsiders_Time = 0.0;

		super.UpdateInsiders(timeout);
	}

	protected void eAI_PatrolSpawnCheck(TriggerInsider insider)
	{
		if (m_eAI_Patrol.CanSpawn() && vector.DistanceSq(insider.GetObject().GetPosition(), GetPosition()) >= m_eAI_InnerRadiusSq)
		{
			m_eAI_Patrol.Spawn();
			GetGame().ObjectDelete(this);
		}
	}
};
