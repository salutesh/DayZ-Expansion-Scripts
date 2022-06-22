/**
 * ExpansionQuestsGroupInventory.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestsGroupInventory
{
	ref array<string> m_GroupPlayers = new array<string>;
	ref array<EntityAI> m_GroupInventory = new array<EntityAI>;

	void ExpansionQuestsGroupInventory(ExpansionPartyData group)
	{
		if (m_GroupPlayers.Count() > 0)
			m_GroupPlayers.Clear();
		
		for (int i = 0; i < group.GetPlayers().Count(); i++)
		{
			ExpansionPartyPlayerData playerGroupData = group.GetPlayers()[i];
			m_GroupPlayers.Insert(playerGroupData.GetID());
		}

		EnumerateGroupInventory();
	}

	void EnumerateGroupInventory()
	{
		Print(ToString() + ":: EnumerateGroupInventory - Start");
		
		m_GroupInventory.Clear();
		array<EntityAI> items = new array<EntityAI>;
		for (int i = 0; i < m_GroupPlayers.Count(); i++)
		{
			string groupPlayerUID = m_GroupPlayers[i];
			PlayerBase groupPlayer = PlayerBase.GetPlayerByUID(groupPlayerUID);
			if (groupPlayer)	
			{		
				groupPlayer.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);
				AddGroupPlayerItems(items, groupPlayer);
				items.Clear();
			}
		}
	}

	private void AddGroupPlayerItems(array<EntityAI> items, PlayerBase player)
	{
		Print(ToString() + "::AddGroupPlayerItems - Start");
		for (int i = 0; i < items.Count(); i++)
		{
			EntityAI item = items[i];
			if (!item)
				continue;

		#ifdef WRDG_DOGTAGS
			Dogtag_Base dogTag;
			if (Class.CastTo(dogTag, item))
			{
				int slotId = InventorySlots.GetSlotIdFromString("Dogtag");
				Dogtag_Base playerDogTag;
				//PlayerBase dogtagPlayer = PlayerBase.Cast(dogTag.GetHierarchyRootPlayer());
				EntityAI slotItem = player.GetInventory().FindAttachment(slotId);
				if (Class.CastTo(playerDogTag, slotItem))
				{
					if (dogTag == playerDogTag)
						continue;
				}
			}
		#endif

			m_GroupInventory.Insert(item);
		}
	}
	
	bool HasItem(string typeName, out array<EntityAI> items)
	{
		items = new array<EntityAI>;
		for (int i = 0; i < m_GroupInventory.Count(); i++)
		{
			EntityAI item = m_GroupInventory[i];
			if (!item)
				continue;
		
			if (item.Type().ToString() == typeName)
			{
				items.Insert(item);
			}
		}

		if (items.Count() > 0)
			return true;

		return false;
	}
};