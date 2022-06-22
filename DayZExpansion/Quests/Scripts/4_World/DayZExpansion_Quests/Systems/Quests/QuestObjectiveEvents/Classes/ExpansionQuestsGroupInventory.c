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
	ref array<PlayerBase> m_GroupPlayers;
	ref array<EntityAI> m_GroupInventory;

	void ExpansionQuestsGroupInventory(ExpansionPartyData group)
	{
		m_GroupPlayers = new array<PlayerBase>;
		m_GroupInventory = new array<EntityAI>;

		for (int i = 0; i < group.GetPlayers().Count(); i++)
		{
			ExpansionPartyPlayerData playerGroupData = group.GetPlayers()[i];
			PlayerBase groupPlayer = PlayerBase.GetPlayerByUID(playerGroupData.GetID());
			if (!groupPlayer)
				continue;

			m_GroupPlayers.Insert(groupPlayer);
		}

		EnumerateGroupInventory();
	}

	void EnumerateGroupInventory()
	{
		auto trace = EXTrace.Start(ExpansionTracing.QUESTS);

		m_GroupInventory.Clear();

		array<EntityAI> items = new array<EntityAI>;

		for (int i = 0; i < m_GroupPlayers.Count(); i++)
		{
			PlayerBase groupPlayer = m_GroupPlayers[i];
			groupPlayer.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);
			AddGroupPlayerItems(items, groupPlayer);
		}
	}

	private void AddGroupPlayerItems(array<EntityAI> items, PlayerBase player)
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
				Print(ToString() + "::AddGroupPlayerItems - Item is Dogtag");
			#endif
				if (dogTag)
				{
					int slotId = InventorySlots.GetSlotIdFromString("Dogtag");
					Dogtag_Base playerDogTag;
					EntityAI slotItem = player.GetInventory().FindAttachment(slotId);
					if (Class.CastTo(playerDogTag, slotItem))
					{
						if (dogTag == playerDogTag)
						{
						#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
							Print(ToString() + "::AddGroupPlayerItems - Dogtag is players dogtag! Don't add!");
						#endif
							continue;
						}
					}
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

		for (int i = 0; i < m_GroupInventory.Count(); i++)
		{
			EntityAI item = m_GroupInventory[i];
			if (!item)
				continue;
		
			if (item.ClassName() == typeName || item.GetType() == typeName)
			{
				items.Insert(item);
			}
		}

		if (items.Count() > 0)
			return true;

		return false;
	}
};