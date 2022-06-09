/**
 * ExpansionQuestPlayerInventory.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestsPlayerInventory
{
	PlayerBase m_Player;
	ref array<EntityAI> m_Inventory;

	void ExpansionQuestsPlayerInventory(PlayerBase player)
	{
		m_Player = player;
		m_Inventory = new array<EntityAI>;
		Enumerate();
	}

	void Enumerate()
	{
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS);

		if (!m_Player)
			return;

		m_Inventory.Clear();

		array<EntityAI> items = new array<EntityAI>;
		m_Player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);
		AddPlayerItems(items, m_Player);
	}

	private void AddPlayerItems(array<EntityAI> items, PlayerBase player)
	{
		for (int i = 0; i < items.Count(); i++)
		{
			EntityAI item = items[i];

			if (!item)
				continue;

		#ifdef WRDG_DOGTAGS
			Dogtag_Base dogTag;
			if (Class.CastTo(dogTag, item))
			{
			#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
				Print(ToString() + "::AddPlayerItems - Item is Dogtag");
			#endif
				int slotId = InventorySlots.GetSlotIdFromString("Dogtag");
				Dogtag_Base playerDogTag;
				EntityAI slotItem = player.GetInventory().FindAttachment(slotId);
				if (Class.CastTo(playerDogTag, slotItem))
				{
					if (dogTag == playerDogTag)
					{
					#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
						Print(ToString() + "::AddPlayerItems - Dogtag is players dogtag! Don't add!");
					#endif
						continue;
					}
				}
			}
		#endif

			m_Inventory.Insert(item);
		}
	}

	bool HasItem(string typeName, out array<EntityAI> items)
	{
		if (!items)
			items = new array<EntityAI>;

		foreach (EntityAI item: m_Inventory)
		{
			if (!item)
				continue;

			if (item.ClassName() == typeName)
			{
				items.Insert(item);
			}
		}

		if (items.Count() > 0)
			return true;

		return false;
	}
};