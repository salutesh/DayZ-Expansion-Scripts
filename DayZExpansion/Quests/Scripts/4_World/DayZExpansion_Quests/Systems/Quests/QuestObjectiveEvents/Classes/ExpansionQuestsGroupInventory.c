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

#ifdef EXPANSIONMODGROUPS
class ExpansionQuestsGroupInventory
{
	ref array<string> m_GroupPlayers = new array<string>;
	ref array<EntityAI> m_GroupInventory = new array<EntityAI>;

	void ExpansionQuestsGroupInventory(ExpansionPartyData group)
	{
		if (m_GroupPlayers.Count() > 0)
			m_GroupPlayers.Clear();

		array<ref ExpansionPartyPlayerData> groupPlayers = group.GetPlayers();
		foreach (ExpansionPartyPlayerData playerGroupData: groupPlayers)
		{
			m_GroupPlayers.Insert(playerGroupData.GetID());
		}

		EnumerateGroupInventory();
	}

	void EnumerateGroupInventory()
	{
		EXTrace.Start(EXTrace.QUESTS, this);

		m_GroupInventory.Clear();
		array<EntityAI> items = new array<EntityAI>;
		foreach (string groupPlayerUID: m_GroupPlayers)
		{
			PlayerBase groupPlayer = PlayerBase.GetPlayerByUID(groupPlayerUID);
			if (groupPlayer)
			{
				groupPlayer.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);
				AddGroupPlayerItems(items, groupPlayer);
				items.Clear();
			}
		}
	}

	protected void AddGroupPlayerItems(array<EntityAI> items, PlayerBase player)
	{
		EXTrace.Start(EXTrace.QUESTS, this);

		foreach (EntityAI item: items)
		{
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
		if (!items)
			items = new array<EntityAI>;

		foreach (EntityAI item: m_GroupInventory)
		{
			if (!item)
				continue;

			if (item.ClassName() == typeName || item.IsKindOf(typeName))
			{
				items.Insert(item);
			}
		}

		if (items.Count() > 0)
			return true;

		return false;
	}
	
	bool HasAnyOf(array<string> typeNames, out array<EntityAI> items)
	{
		if (!items)
			items = new array<EntityAI>;

		foreach (EntityAI item: m_GroupInventory)
		{
			if (!item)
				continue;

			if (ExpansionStatic.IsAnyOf(item, typeNames, true))
			{
				items.Insert(item);
			}
		}

		if (items.Count() > 0)
			return true;

		return false;
	}
};
#endif
