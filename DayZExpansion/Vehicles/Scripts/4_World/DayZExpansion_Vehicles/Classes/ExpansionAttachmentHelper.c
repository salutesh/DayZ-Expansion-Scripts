/**
 * ExpansionAttachmentHelper.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
 */

class ExpansionAttachmentHelper
{
	static bool CanAttachTo(Object child, Object target)
	{
		if (!s_ExpansionPlayerAttachment)
			return false;

		if (!target)
			return false;

		CarScript car = NULL;
		if (Class.CastTo(car, target))
		{
			return car.CanObjectAttach(child);
		}

		ExpansionVehicleBase veh = NULL;
		if (Class.CastTo(veh, target))
		{
			return veh.CanObjectAttach(child);
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
		if (!s_ExpansionPlayerAttachment)
			return NULL;

		Object obj;

		for (int i = 0; i < results.Count(); i++)
		{
			obj = results[i].parent;
			if (obj)
			{
				if (CanAttachTo(child, obj))
					return obj;

				continue;
			}

			obj = results[i].obj;
			if (obj && CanAttachTo(child, obj))
				return obj;
		}

		return NULL;
	}

	static Object FindBestAttach(Object child, set<Object> objects)
	{
		if (!s_ExpansionPlayerAttachment)
			return NULL;

		for (int i = 0; i < objects.Count(); i++)
		{
			Object attch = FindRootAttach(child, objects[i]);
			if (attch)
			{
				return attch;
			}
		}

		return NULL;
	}
};