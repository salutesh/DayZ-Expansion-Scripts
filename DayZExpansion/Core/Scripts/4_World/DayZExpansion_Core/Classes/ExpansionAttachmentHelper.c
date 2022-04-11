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
		if (!target)
			return false;

		CarScript car = NULL;
		if (Class.CastTo(car, target))
		{
			if (!car.m_Expansion_AcceptingAttachment)
				return false;

			return car.Expansion_CanObjectAttach(child);
		}

		ItemBase item = NULL;
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
		Object target = obj;

		if (CanAttachTo(child, target))
		{
			return target;
		}

		while (target != NULL && Class.CastTo(target, target.GetParent()))
		{
			if (CanAttachTo(child, target))
			{
				return target;
			}
		}

		if (CanAttachTo(child, target))
		{
			return target;
		}

		return NULL;
	}

	static Object FindBestAttach(Object child, array<ref RaycastRVResult> results)
	{
		Object best = NULL;

		Object obj;
		for (int i = 0; i < results.Count(); i++)
		{
			if (!CanAttachTo(child, results[i].parent))
			{
				if (!CanAttachTo(child, results[i].obj))
					continue;

				obj = results[i].obj;
			}

			if (!obj)
			{
				obj = results[i].parent;

				if (!obj)
					continue;
			}

			if (obj == child.GetParent())
				return obj;

			best = obj;
		}

		return best;
	}

	static Object FindBestAttach(Object child, set<Object> objects)
	{
		Object best = NULL;

		Object obj;
		for (int i = 0; i < objects.Count(); i++)
		{
			obj = FindRootAttach(child, objects[i]);
			if (!obj)
				continue;

			if (obj == child.GetParent())
				return obj;

			best = obj;
		}

		return best;
	}
};
