#ifndef SERVER
//! Client only!

typedef CF_DoublyLinkedNodes_WeakRef<ScriptedLightBase> eAI_ScriptedLightNodes;
typedef CF_DoublyLinkedNode_WeakRef<ScriptedLightBase> eAI_ScriptedLightNode;

modded class ScriptedLightBase
{
	static ref eAI_ScriptedLightNodes s_eAI_LightNodes = new eAI_ScriptedLightNodes();

	ref eAI_ScriptedLightNode m_eAI_LightNode = s_eAI_LightNodes.Add(this);

	//! Players in radius of light
	ref array<PlayerBase> m_eAI_Players;

#ifdef DIAG_DEVELOPER
	void ScriptedLightBase()
	{
		g_Game.GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(eAI_DeferredInit, 34);
	}
#endif

	void ~ScriptedLightBase()
	{
	#ifndef DIAG_DEVELOPER
		if (!g_Game)
			return;
	#endif

		if (s_eAI_LightNodes)
			s_eAI_LightNodes.Remove(m_eAI_LightNode);

		if (m_eAI_Players)
		{
			for (int i = m_eAI_Players.Count() - 1; i >= 0; --i)
			{
				PlayerBase player = m_eAI_Players[i];

				if (player)
					player.eAI_RemoveLight(this, false);
			}
		}

	#ifdef DIAG_DEVELOPER
		EXTrace.Print(EXTrace.AI, null, string.Format("~%1 pos=%2 radius=%3 enabled=%4", this, GetWorldPosition().ToString(), GetRadius(), IsEnabled()));
	#endif
	}

	void eAI_DeferredInit()
	{
		EXTrace.Print(EXTrace.AI, this, string.Format("pos=%1 radius=%2 enabled=%3", GetWorldPosition().ToString(), GetRadius(), IsEnabled()));
	}

	bool eAI_IsVisible(bool isNight = false)
	{
		if (!IsEnabled())
			return false;

		if (!isNight && !IsVisibleDuringDaylight())
			return false;

		EntityAI parentEntity;
		if (Class.CastTo(parentEntity, GetParent()))
		{
			GameInventory inventory = parentEntity.GetInventory();
			if (inventory && inventory.IsInCargo())
				return false;
		}

		return true;
	}

	bool eAI_IsLightOnPlayer(PlayerBase player)
	{
		EntityAI parentEntity;
		if (Class.CastTo(parentEntity, GetParent()) && parentEntity.GetHierarchyRootPlayer() == player)
			return true;

		return false;
	}

	bool eAI_IsPlayerInLight(PlayerBase player)
	{
		if (eAI_IsPositionInLight(player.GetPosition()))
		{
			vector position = GetWorldPosition();
			vector endPos = player.GetBonePositionWS(player.GetBoneIndexByName("pelvis"));
			vector dir = endPos - position;
			vector begPos = position + dir.Normalized() * 0.1;
			vector contactPos;
			vector contactDir;
			int contactComponent;
			set<Object> results = new set<Object>;

			if (DayZPhysics.RaycastRV(begPos, endPos, contactPos, contactDir, contactComponent, results, null, null, false, false, ObjIntersectGeom) && results.Count() > 0)
			{
				EntityAI parent;
				if (!Class.CastTo(parent, player.GetParent()))
					parent = player;

				EntityAI entity;
				if (Class.CastTo(entity, results[0]) && entity.GetHierarchyRoot() == parent)
					return true;
			}
		}

		return false;
	}

	bool eAI_IsPositionInLight(vector position)
	{
		vector lightPosition = GetWorldPosition();
		float f = 0.95;  //! Account for light intensity fall-off
		float radius = GetRadius() * f;

		if (vector.DistanceSq(position, lightPosition) <= radius * radius)
		{
			if (GetLightType() != LightSourceType.SpotLight)  //! Don't do angle check if not spotlight
				return true;

			vector dir = vector.Direction(lightPosition, position).Normalized();
			float dot = vector.Dot(GetDirection(), dir);
			float angle = Math.Acos(dot) * Math.RAD2DEG;

			//! @note dot angle is half angle, so need to halve light angle as well for comparison
			if (angle <= GetSpotLightAngle() * 0.5)
				return true;
		}

		return false;
	}

	void eAI_AddPlayer(PlayerBase player)
	{
		if (!m_eAI_Players)
			m_eAI_Players = {};

		int index = m_eAI_Players.Insert(player);

	#ifdef DIAG_DEVELOPER
		EXTrace.Print(EXTrace.AI, this, string.Format("[Lights] added %1 at index %2", player, index));
	#endif
	}

	void eAI_RemovePlayer(PlayerBase player)
	{
		if (m_eAI_Players)
		{
			int index = m_eAI_Players.Find(player);

			if (index > -1)
				m_eAI_Players.Remove(index);

		#ifdef DIAG_DEVELOPER
			EXTrace.Print(EXTrace.AI, this, string.Format("[Lights] removed %1 at index %2", player, index));
		#endif
		}
	}
}
#endif
