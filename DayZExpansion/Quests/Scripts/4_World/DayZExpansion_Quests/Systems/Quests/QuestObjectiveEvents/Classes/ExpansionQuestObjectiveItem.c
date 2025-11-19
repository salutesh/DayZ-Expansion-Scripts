/**
 * ExpansionQuestObjectiveItem.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestObjectiveItem
{
	ItemBase m_ObjectiveItem;
	int m_RemainingAmount;
	string m_ClassName;

	void ExpansionQuestObjectiveItem(ItemBase item)
	{
		m_ObjectiveItem = item;
		m_ClassName = item.GetType();
	}

	void SetRemaining(int amount)
	{
		m_RemainingAmount = amount;
	}

	int GetRemaining()
	{
		return m_RemainingAmount;
	}

	string GetClassName()
	{
		return m_ClassName;
	}

	string GetClassNameLower()
	{
		string className = m_ClassName;
		className.ToLower();
		return className;
	}

	void DestroyItem()
	{
		if (m_RemainingAmount == 0)
		{
			#ifdef EXPANSIONMODVEHICLE
			ExpansionVehicle vehicle;
			if (ExpansionVehicle.Get(vehicle, m_ObjectiveItem))
			{
				//! Delete all keys for vehicle
				array< ExpansionCarKey > keys;
				ExpansionCarKey.GetKeysForVehicle(vehicle, keys);
				foreach (ExpansionCarKey key : keys)
				{
					g_Game.ObjectDelete(key);
				}
			}
			#endif

			g_Game.ObjectDelete(m_ObjectiveItem);
			return;
		}

		ItemBase itm;
		Magazine mag;
		if (Class.CastTo(mag, m_ObjectiveItem))
		{
			mag.ServerSetAmmoCount(m_RemainingAmount);
		}
		else if ( Class.CastTo(itm, m_ObjectiveItem))
		{
			itm.SetQuantity(m_RemainingAmount);
		}
	}

	ItemBase GetItem()
	{
		return m_ObjectiveItem;
	}
};