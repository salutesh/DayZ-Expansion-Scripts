class ExpansionInventoryTools
{
	protected static ref ExpansionInventoryTools s_Instance = new ExpansionInventoryTools();

	protected static EntityAI s_TidyInProgress;

	protected ref ExpansionRPCManager m_RPCManager = new ExpansionRPCManager(this);

	private void ExpansionInventoryTools()
	{
		m_RPCManager.RegisterClient("RPC_SyncLocations");
		m_RPCManager.RegisterBoth("RPC_Tidy");
	}

	static ExpansionInventoryTools GetInstance()
	{
		return s_Instance;
	}

	static bool IsTidyInProgress(EntityAI entity)
	{
		return s_TidyInProgress == entity;
	}

	/**
	 * @brief Tidy cargo to maximize free space and combine stacks
	 *
	 * @param owner
	 * @param recursive
	 * @param sync  sync to client
	 *
	 * @return true if successful
	 */
	static bool Tidy(notnull EntityAI owner, bool recursive = false, bool sync = true)
	{
		if (s_TidyInProgress == owner)
			return false;

		GameInventory inventory = owner.GetInventory();

		if (!inventory)
			return false;

		if (!s_TidyInProgress)
			s_TidyInProgress = owner;

		bool result = Tidy(inventory, recursive, sync);

		if (s_TidyInProgress == owner)
			s_TidyInProgress = null;

		return result;
	}

	/**
	 * @brief Tidy cargo to maximize free space and combine stacks
	 *
	 * @param inventory
	 * @param recursive
	 * @param sync  sync to client
	 *
	 * @return true if successful
	 */
	static bool Tidy(notnull GameInventory inventory, bool recursive = false, bool sync = true)
	{
		bool result;

		CargoBase cargo = inventory.GetCargo();

		if (cargo)
			result |= Tidy(cargo, recursive, sync);

		if (recursive)
		{
			for (int i = 0; i < inventory.AttachmentCount(); ++i)
			{
				EntityAI attachment = inventory.GetAttachmentFromIndex(i);
				result |= Tidy(attachment, true, sync);
			}
		}

		return result;
	}

	/**
	 * @brief Tidy cargo to maximize free space and combine stacks
	 *
	 * @param cargo
	 * @param recursive
	 * @param sync  sync to client
	 *
	 * @return true if successful
	 */
	static bool Tidy(notnull CargoBase cargo, bool recursive = false, bool sync = true)
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start0(EXTrace.GENERAL_ITEMS, ExpansionInventoryTools, string.Format("Tidy(%1, recursive = %2, sync = %3)", cargo, recursive.ToString(), sync.ToString()));
	#endif

		TStringArray types = {};
		map<string, ref array<ItemBase>> stacks = new map<string, ref array<ItemBase>>;
		array<ItemBase> items = {};
		bool isServer = g_Game.IsServer();

		for (int i = 0; i < cargo.GetItemCount(); ++i)
		{
			ItemBase item;
			if (Class.CastTo(item, cargo.GetItem(i)))
			{
				if (isServer && item.Expansion_GetStackAmount() < item.Expansion_GetStackMax())
				{
					string type = item.GetType();
					array<ItemBase> typeStacks;

					if (!stacks.Find(type, typeStacks))
					{
						types.Insert(type);
						typeStacks = {};
						stacks[type] = typeStacks;
					}

					typeStacks.Insert(item);
				}
				else
				{
					items.Insert(item);
				}
			}
		}

		if (isServer)
			CombineStacks(types, stacks, items);

		return Tidy(cargo.GetCargoOwner(), items, recursive, sync);
	}

	/**
	 * @brief Tidy cargo to maximize free space
	 *
	 * @param parent
	 * @param items
	 * @param recursive
	 * @param sync  sync to client
	 */
	static bool Tidy(EntityAI parent, array<ItemBase> items, bool recursive = false, bool sync = true)
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start0(EXTrace.GENERAL_ITEMS, ExpansionInventoryTools, string.Format("Tidy(%1, items(%2), recursive = %3, sync = %4)", parent, items.Count(), recursive.ToString(), sync.ToString()));
	#endif

		bool result;

		map<int, ref array<ref InventoryLocation>> largestDimensionToLocs = new map<int, ref array<ref InventoryLocation>>;
		TIntArray largestDimensions = {};
		int largestDimension;
		array<ref InventoryLocation> srcLocs;

		foreach (ItemBase item: items)
		{
			InventoryLocation currentLoc = new InventoryLocation();

			if (item.GetInventory().GetCurrentInventoryLocation(currentLoc))
			{
				if (GameInventory.LocationRemoveEntity(currentLoc))
				{
					vector itemSize = item.ConfigGetVector("itemSize");
					largestDimension = itemSize[0] * itemSize[1];

					if (!largestDimensionToLocs.Find(largestDimension, srcLocs))
					{
						srcLocs = {};
						largestDimensionToLocs[largestDimension] = srcLocs;
						largestDimensions.Insert(largestDimension);
					}

					srcLocs.Insert(currentLoc);
				}
				else
				{
					CF.FormatError("Couldn't remove %1", item.ToString());
				}
			}
			else
			{
				CF.FormatError("Couldn't get current inventory location %1", item.ToString());
			}
		}

		largestDimensions.Sort();

		GameInventory parentInventory = parent.GetInventory();
		TIntArray srcIndexes;
		array<ref InventoryLocation> locsToSync;

		if (g_Game.IsDedicatedServer() && sync)
		{
			//srcIndexes = {};
			locsToSync = {};
		}

		for (int i = largestDimensions.Count() - 1; i >= 0; --i)
		{
			largestDimension = largestDimensions[i];
			srcLocs = largestDimensionToLocs[largestDimension];

			foreach (InventoryLocation srcLoc: srcLocs)
			{
				EntityAI entity = srcLoc.GetItem();

				InventoryLocation newLoc = new InventoryLocation();
				bool foundFreeLoc = parentInventory.FindFreeLocationFor(entity, FindInventoryLocationType.CARGO, newLoc);
				bool moveEntity = foundFreeLoc && (!srcLoc.CompareLocationOnly(newLoc) || srcLoc.GetFlip() != newLoc.GetFlip());

				if (moveEntity && GameInventory.LocationAddEntity(newLoc))
				{
					if (locsToSync)
					{
						//srcIndexes.Insert(srcLoc.GetIdx());
						locsToSync.Insert(newLoc);
					}

					result |= true;
				}
				else if (!GameInventory.LocationAddEntity(srcLoc))
				{
					if (GameInventory.SetGroundPosByOwner(entity.GetHierarchyRoot(), entity, newLoc))
					{
						if (GameInventory.LocationAddEntity(newLoc))
						{
							if (locsToSync)
							{
								//srcIndexes.Insert(srcLoc.GetIdx());
								locsToSync.Insert(newLoc);
							}
						}
						else
						{
							CF.FormatError("Couldn't drop %1", entity.ToString());
						}
					}
					else
					{
						CF.FormatError("Couldn't set ground pos by owner for %1", entity.ToString());
					}
				}

				if (recursive)
					result |= Tidy(entity, true, sync);
			}
		}

		if (locsToSync && locsToSync.Count() > 0)
			SyncLocations(parent, srcIndexes, locsToSync);
		//else if (result && sync && g_Game.IsClient())
			//RequestTidy(parent, recursive);

		//if (result && sync && g_Game.IsMultiplayer())
			//RequestTidy(parent, recursive);

		return result;
	}

	/**
	 * @brief Combine stacks
	 *
	 * @param stacks  lookup table of item type to stackable items
	 * @param items  array to insert combined items into
	 */
	static void CombineStacks(TStringArray types, map<string, ref array<ItemBase>> stacks, array<ItemBase> items)
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start0(EXTrace.GENERAL_ITEMS, ExpansionInventoryTools, "CombineStacks(stacks, items)");
	#endif

		foreach (string type: types)
		{
			CombineStacks(stacks[type], items);
		}
	}

	/**
	 * @brief Combine stacks
	 *
	 * @param stacks  stackable items
	 * @param items  array to insert combined items into
	 */
	static void CombineStacks(array<ItemBase> stacks, array<ItemBase> items)
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start0(EXTrace.GENERAL_ITEMS, ExpansionInventoryTools, "CombineStacks(typeStacks, items)");
		int n;
	#endif

		for (int i = 0; i < stacks.Count(); ++i)
		{
			ItemBase item = stacks[i];

			for (int j = i + 1; j < stacks.Count(); ++j)
			{
				ItemBase other = stacks[j];

				other.Expansion_SuppressDelete(true);
				item.CombineItems(other);
				if (other)
					other.Expansion_SuppressDelete(false);

			#ifdef EXTRACE
				++n;
			#endif

				//! Ammo stack deletion when count reaches zero is handled by the game engine, other stacks we handle ourselves
				if (!other || other.ToDelete())
				{
					++i;
				}
				else if (other.Expansion_GetStackAmount() == other.Expansion_GetStackMin())
				{
					++i;

					if (other.ConfigGetBool("varQuantityDestroyOnMin"))
					{
						//! Instantly remove item from location to free up space for tidy
						g_Game.ObjectDelete(other);
					}
				}

				if (item.Expansion_GetStackAmount() == item.Expansion_GetStackMax())
					break;
			}

			items.Insert(item);
		}

	#ifdef EXTRACE
		EXTrace.Add(trace, n);
	#endif
	}

	protected static void SyncLocations(EntityAI parent, TIntArray srcIndexes, array<ref InventoryLocation> locs)
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start0(EXTrace.GENERAL_ITEMS, ExpansionInventoryTools, "SyncLocations");
	#endif

		auto rpc = s_Instance.m_RPCManager.CreateRPC("RPC_SyncLocations");

		rpc.WriteUInt(locs.Count(), 16);
		foreach (int i, InventoryLocation loc: locs)
		{
			//PrintFormat("srcIndex %1 %2", srcIndexes[i], loc.GetItem());
			//rpc.WriteUInt(srcIndexes[i], 16);
			int low, high;
			loc.GetItem().GetNetworkID(low, high);
			rpc.Write(low, 32);
			rpc.Write(high, 32);
			//PrintFormat("dstIndex %1 %2", loc.GetIdx(), loc.GetItem());
			//rpc.WriteUInt(loc.GetIdx(), 16);
			rpc.WriteUInt(loc.GetRow(), 16);
			rpc.WriteUInt(loc.GetCol(), 4);
			rpc.WriteBool(loc.GetFlip());
		}

		PlayerBase.Expansion_Send(rpc, parent, true);
	}

	protected void RPC_SyncLocations(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start0(EXTrace.GENERAL_ITEMS, ExpansionInventoryTools, "RPC_SyncLocations");
	#endif

		EntityAI parent;
		if (!Class.CastTo(parent, target))
			return;

		GameInventory inventory = parent.GetInventory();
		if (!inventory)
			return;

		CargoBase cargo = inventory.GetCargo();
		if (!cargo)
			return;

		ExpansionBitStreamReader reader = m_RPCManager.GetReader(ctx);

		int count;
		if (!reader.ReadUInt(count, 16) || count < 0)
			return;

		array<ref InventoryLocation> newLocs = {};

		while (count--)
		{
			//int srcIdx;
			//if (!reader.ReadUInt(srcIdx, 16))
				//return;

			int low, high;
			if (!reader.Read(low, 32))
				return;
			if (!reader.Read(high, 32))
				return;

			int idx;
			//if (!reader.ReadUInt(idx, 16))
				//return;

			int row;
			if (!reader.ReadUInt(row, 16))
				return;

			int col;
			if (!reader.ReadUInt(col, 4))
				return;

			bool flip;
			if (!reader.ReadBool(flip))
				return;

			//EntityAI item = cargo.GetItem(srcIdx);
			//if (!item)
				//continue;

			//PrintFormat("srcIndex %1 %2", srcIdx, item);

			EntityAI item;
			if (!Class.CastTo(item, g_Game.GetObjectByNetworkId(low, high)))
				continue;

			InventoryLocation currentLoc = new InventoryLocation();
			if (!item.GetInventory().GetCurrentInventoryLocation(currentLoc))
			{
				CF.FormatError("Couldn't get current inventory location %1", item.ToString());
				continue;
			}

			if (!GameInventory.LocationRemoveEntity(currentLoc))
			{
				CF.FormatError("Couldn't remove %1", item.ToString());
				continue;
			}

			InventoryLocation newLoc = new InventoryLocation();
			newLoc.SetCargo(parent, item, idx, row, col, flip);
			newLocs.Insert(newLoc);
		}

		foreach (InventoryLocation loc: newLocs)
		{
			if (!GameInventory.LocationAddEntity(loc))
			{
				CF.FormatError("Couldn't move %1", loc.GetItem().ToString());
				continue;
			}
		#ifdef DIAG_DEVELOPER
			else
			{
				EXTrace.Print(EXTrace.GENERAL_ITEMS, ExpansionInventoryTools, "Moved " + loc.GetItem());
			}
		#endif
		}
	}

	static void RequestTidy(EntityAI parent, bool recursive = false)
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start0(EXTrace.GENERAL_ITEMS, ExpansionInventoryTools, "RequestTidy", "" + parent);
	#endif

		auto rpc = s_Instance.m_RPCManager.CreateRPC("RPC_Tidy");
		rpc.Write(recursive);
		rpc.Expansion_Send(parent, true);
	}

	void RPC_Tidy(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start0(EXTrace.GENERAL_ITEMS, ExpansionInventoryTools, "RPC_Tidy", "" + target);
	#endif

		EntityAI parent;
		if (!Class.CastTo(parent, target))
			return;

		if (g_Game.IsDedicatedServer())
		{
			Man senderPlayer = sender.GetPlayer();
			Man rootPlayer = parent.GetHierarchyRootPlayer();

			if (senderPlayer != rootPlayer)
			{
				//! Do not allow tidying other player's inventory unless restrained or uncon
				DayZPlayerImplement rootPlayerImpl;
				if (Class.CastTo(rootPlayerImpl, rootPlayer))
				{
					if (!rootPlayerImpl.IsRestrained() && !rootPlayerImpl.IsUnconscious() && !rootPlayerImpl.Expansion_IsAI())
						return;
				}

				//! Do not allow tidying inventory if not in vicinity
				float maxDistSq = UAMaxDistances.DEFAULT * UAMaxDistances.DEFAULT;
				if (vector.DistanceSq(senderPlayer.GetPosition(), parent.GetPosition()) > maxDistSq)
					return;  
			}
		}

		bool recursive;
		if (!ctx.Read(recursive))
			return;

		Tidy(parent, recursive, g_Game.IsDedicatedServer());
	}
}
