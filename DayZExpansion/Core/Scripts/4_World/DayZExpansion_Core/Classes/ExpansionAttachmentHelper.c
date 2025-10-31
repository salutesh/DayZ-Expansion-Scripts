/**
 * ExpansionAttachmentHelper.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
 */

class ExpansionAttachmentHelper
{
	static bool CanAttachTo(Object child, Object target)
	{
		ExpansionVehicle vehicle;
		if (ExpansionVehicle.Get(vehicle, target))
		{
			return vehicle.CanObjectAttach(child);
		}

		ItemBase item;
		if (Class.CastTo(item, target))
		{
			if (!item.m_Expansion_AcceptingAttachment)
				return false;

			return item.Expansion_CanObjectAttach(child);
		}

		return false;
	}

	static Object FindRootAttach(Object child, Object obj)
	{
		while (obj)
		{
			if (CanAttachTo(child, obj))
				return obj;

			if (!Class.CastTo(obj, obj.GetParent()))
				break;
		}

		return null;
	}

	static Object FindBestAttach(Object child, array<ref RaycastRVResult> results)
	{
		Object best;

		Object obj;
		IEntity parent;
		DayZPlayerImplement player;

		if (Class.CastTo(player, child))
			parent = player.Expansion_GetParent();
		else
			parent = child.GetParent();

		foreach (RaycastRVResult result: results)
		{
			if (!CanAttachTo(child, result.parent))
			{
				if (!CanAttachTo(child, result.obj))
					continue;

				obj = result.obj;
			}

			if (!obj)
			{
				obj = result.parent;

				if (!obj)
					continue;
			}

			if (obj == parent)
				return obj;

			best = obj;
		}

		return best;
	}

	static Object FindBestAttach(Object child, set<Object> objects)
	{
		Object best;

		Object root;
		IEntity parent;
		DayZPlayerImplement player;

		if (Class.CastTo(player, child))
			parent = player.Expansion_GetParent();
		else
			parent = child.GetParent();

		foreach (Object obj: objects)
		{
			root = FindRootAttach(child, obj);
			if (!root)
				continue;

			if (root == parent)
				return root;

			best = root;
		}

		return best;
	}

	static Object IsAttachment(Object owner, vector position, float radius)
	{
		vector boundingBox[2];

		vector start = position;
		vector end = position - Vector(0, radius, 0);

		RaycastRVParams params = new RaycastRVParams(start, end, owner, radius);
		params.sorted = true;
		params.type = ObjIntersectGeom;
		params.flags = CollisionFlags.ALLOBJECTS;

		//! Initiate the raycast
		array<ref RaycastRVResult> results = new array<ref RaycastRVResult>();
		if (DayZPhysics.RaycastRVProxy(params, results))
			return ExpansionAttachmentHelper.FindBestAttach(owner, results);

		return null;
	}
};
