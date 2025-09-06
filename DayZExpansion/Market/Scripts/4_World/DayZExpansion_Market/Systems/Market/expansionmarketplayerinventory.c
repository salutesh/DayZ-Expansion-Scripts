class ExpansionMarketPlayerInventory
{
	PlayerBase m_Player;
	ref array<EntityAI> m_Inventory = {};
	ref array<EntityAI> m_Vehicles = {};
	int m_VehicleInventoryCount;

	void ExpansionMarketPlayerInventory(PlayerBase player)
	{
		m_Player = player;
		Enumerate();
	}

	void ~ExpansionMarketPlayerInventory()
	{
	#ifdef EXPANSIONMODMARKET_DEBUG
		EXPrint("~ExpansionMarketPlayerInventory");
	#endif
	}

	void Enumerate()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(ExpansionTracing.MARKET, this);
#endif

		array<EntityAI> playerAttachments = {};
		array<EntityAI> itemAttachments = {};
		array<EntityAI> cargoItems = {};
		array<EntityAI> cargoItemsWithAttachments = {};
		array<EntityAI> cargoItemsWithCargo = {};

		EntityAI handItem = m_Player.GetHumanInventory().GetEntityInHands();
		bool handItemIsEmpty;

		if (handItem)
		{
			handItem = SubstituteOwnedVehicle(handItem);
			handItemIsEmpty = handItem.IsEmpty();
		}

		//! 1) add item in hands if empty 1st
	#ifdef DIAG_DEVELOPER
		EXTrace.Print(EXTrace.MARKET, this, "PRIO 1 - item in hands w/o attachments and cargo:");
	#endif
		if (handItem && handItemIsEmpty)
			AddPlayerItem(handItem);

		//! 2) add items w/o attachments in cargo of hand item 2nd
	#ifdef DIAG_DEVELOPER
		EXTrace.Print(EXTrace.MARKET, this, "PRIO 2 - items w/o attachments and cargo in cargo of item in hands:");
	#endif
		if (handItem)
		{
			AddDirectCargoItems(handItem, cargoItemsWithAttachments, cargoItemsWithCargo);
			AddAttachmentsCargoItems(handItem, itemAttachments, cargoItemsWithAttachments, cargoItemsWithCargo, true);
		}

		//! 3) add items w/o attachments in cargo of player attachments 3rd
	#ifdef DIAG_DEVELOPER
		EXTrace.Print(EXTrace.MARKET, this, "PRIO 3 - items w/o attachments and cargo in cargo of player attachments:");
	#endif
		AddAttachmentsCargoItems(m_Player, playerAttachments, cargoItemsWithAttachments, cargoItemsWithCargo);

		//! 4) add items w/o attachments in cargo of attachments of player attachments 4th
	#ifdef DIAG_DEVELOPER
		EXTrace.Print(EXTrace.MARKET, this, "PRIO 4 - items w/o attachments and cargo in cargo of attachments of player attachments:");
	#endif
		foreach (EntityAI attachment: playerAttachments)
		{
			AddAttachmentsCargoItems(attachment, itemAttachments, cargoItemsWithAttachments, cargoItemsWithCargo, true);
		}

		//! enumerate attachments of cargo items w/ attachments recursively
		foreach (EntityAI cargoItem: cargoItemsWithAttachments)
		{
			AddAttachmentsAndCargo(cargoItem, itemAttachments, cargoItems, true);
		}

		//! enumerate items in cargo of other items w/o attachments recursively
		foreach (EntityAI cargoItemWithCargo: cargoItemsWithCargo)
		{
			AddAttachmentsAndCargo(cargoItemWithCargo, itemAttachments, cargoItems, true);
		}

		//! enumerate driven vehicles
		array<EntityAI> driven = GetNearbyDrivenVehicles();
		foreach (EntityAI vehicle: driven)
		{
			if (m_Vehicles.Find(vehicle) == -1)
				cargoItemsWithAttachments.Insert(vehicle);
		}

		//! 4b) add cargo items in cargo of other items w/o attachments
	#ifdef DIAG_DEVELOPER
		EXTrace.Print(EXTrace.MARKET, this, "PRIO 4b - cargo items in cargo of items:");
	#endif
		AddPlayerItems(cargoItems);

		//! 5) add attachments of attachments 5th
	#ifdef DIAG_DEVELOPER
		EXTrace.Print(EXTrace.MARKET, this, "PRIO 5 - attachments of items in cargo:");
	#endif
		AddPlayerItems(itemAttachments);

		//! 5b) add cargo items w/ cargo
	#ifdef DIAG_DEVELOPER
		EXTrace.Print(EXTrace.MARKET, this, "PRIO 5b - cargo items w/ cargo:");
	#endif
		AddPlayerItems(cargoItemsWithCargo);

		//! 6) add cargo items w/ attachments 6th
	#ifdef DIAG_DEVELOPER
		EXTrace.Print(EXTrace.MARKET, this, "PRIO 6 - cargo items w/ attachments:");
	#endif
		AddPlayerItems(cargoItemsWithAttachments);

		//! 7) add item in hands if NOT empty 7th
	#ifdef DIAG_DEVELOPER
		EXTrace.Print(EXTrace.MARKET, this, "PRIO 7 - item in hands w/ attachments or cargo:");
	#endif
		if (handItem && !handItemIsEmpty)
			AddPlayerItem(handItem);

		//! 8) add player attachments 8th
	#ifdef DIAG_DEVELOPER
		EXTrace.Print(EXTrace.MARKET, this, "PRIO 8 - player attachments:");
	#endif
		AddPlayerItems(playerAttachments);

	#ifdef DIAG_DEVELOPER
		int count = m_Player.GetInventory().CountInventory() - 1 + m_VehicleInventoryCount;
		EXTrace.Print(EXTrace.MARKET, this, "::Enumerate - " + m_Inventory.Count() + "/" + count);
	#endif
	}

	private void AddAttachmentsCargoItems(EntityAI entity, array<EntityAI> attachments, array<EntityAI> cargoItemsWithAttachments = null, array<EntityAI> cargoItemsWithCargo = null, bool recursive = false)
	{
		GameInventory inventory = entity.GetInventory();

		int attachmentCount = inventory.AttachmentCount();

		for (int i = 0; i < attachmentCount; ++i)
		{
			EntityAI attachment = inventory.GetAttachmentFromIndex(i);

			AddDirectCargoItems(attachment, cargoItemsWithAttachments, cargoItemsWithCargo);

			attachments.Insert(attachment);

			if (recursive)
				AddAttachmentsCargoItems(attachment, attachments, cargoItemsWithAttachments, cargoItemsWithCargo, true);
		}
	}

	private void AddAttachmentsAndCargo(EntityAI entity, array<EntityAI> attachments, array<EntityAI> cargoItems, bool recursive = false)
	{
		GameInventory inventory = entity.GetInventory();

		int attachmentCount = inventory.AttachmentCount();

		int i;

		for (i = 0; i < attachmentCount; ++i)
		{
			EntityAI attachment = inventory.GetAttachmentFromIndex(i);

			attachment = SubstituteOwnedVehicle(attachment);

			attachments.Insert(attachment);

			if (recursive)
				AddAttachmentsAndCargo(attachment, attachments, cargoItems, true);
		}

		CargoBase cargo = entity.GetInventory().GetCargo();

		if (cargo)
		{
			int cargoCount = cargo.GetItemCount();

			for (i = 0; i < cargoCount; ++i)
			{
				EntityAI cargoItem = cargo.GetItem(i);

				cargoItem = SubstituteOwnedVehicle(cargoItem);

				cargoItems.Insert(cargoItem);

				if (recursive)
					AddAttachmentsAndCargo(cargoItem, attachments, cargoItems, true);
			}
		}
	}

	private void AddDirectCargoItems(EntityAI entity, array<EntityAI> cargoItemsWithAttachments, array<EntityAI> cargoItemsWithCargo)
	{
		CargoBase cargo = entity.GetInventory().GetCargo();

		if (cargo)
		{
			int cargoCount = cargo.GetItemCount();

			for (int i = 0; i < cargoCount; i++)
			{
				EntityAI cargoItem = cargo.GetItem(i);

				cargoItem = SubstituteOwnedVehicle(cargoItem);

				if (cargoItem.GetInventory().AttachmentCount() == 0)
				{
					CargoBase cargoItemCargo = cargoItem.GetInventory().GetCargo();
					if (cargoItemCargo && cargoItemCargo.GetItemCount() > 0)
						cargoItemsWithCargo.Insert(cargoItem);
					else
						AddPlayerItem(cargoItem);
				}
				else
				{
					cargoItemsWithAttachments.Insert(cargoItem);
				}
			}
		}
	}

	private void AddPlayerItem(EntityAI item, array<EntityAI> existing = NULL)
	{
		if (!item || item.IsSetForDeletion())
			return;

		if (existing && existing.Find(item) > -1)
			return;

	#ifdef DIAG_DEVELOPER
		string hierarchy;
		EntityAI entity = item;

		while (entity && entity != m_Player)
		{
			if (entity != item)
				hierarchy += " <- ";

			hierarchy += entity.GetDebugName();

			if (entity.GetInventory().IsInCargo())
				hierarchy += " (in cargo)";

			entity = entity.GetHierarchyParent();
		}

		EXTrace.Print(EXTrace.MARKET, this, "  " + hierarchy);
	#endif

		m_Inventory.Insert(item);
	}

	private void AddPlayerItems(array<EntityAI> items, array<EntityAI> existing = NULL)
	{
		for (int i = items.Count() - 1; i >= 0; --i)
		{
			AddPlayerItem(items[i], existing);
		}
	}
	
	EntityAI SubstituteOwnedVehicle(EntityAI item)
	{
		#ifdef EXPANSIONMODVEHICLE
		//! If this is a master key of a vehicle, substitute the vehicle itself if it's close
		ExpansionCarKey key;
		if (Class.CastTo(key, item) && key.IsMaster())
		{
			EntityAI vehicle;
			if (Class.CastTo(vehicle, key.GetKeyObject()) && key.GetHierarchyRoot() != vehicle && IsVehicleNearby(vehicle))
			{
				item = vehicle;
				m_Vehicles.Insert(item);
				m_VehicleInventoryCount += item.GetInventory().CountInventory() - 1;
			}
		}
		#endif

		return item;
	}
	
	array<EntityAI> GetNearbyDrivenVehicles(string className = "", int amount = -1)
	{
		array<EntityAI> driven = new array<EntityAI>;

		string type;
		EntityAI vehicleEntity;

		auto node = ExpansionVehicle.s_All.m_Head;
		while (node)
		{
			auto vehicle = node.m_Value;
			node = node.m_Next;
			type = vehicle.GetType();
			type.ToLower();
			if ((!className || type == className) && vehicle.GetLastDriverUID() == m_Player.GetIdentityUID())
			{
				#ifdef EXPANSIONMODVEHICLE
				if (vehicle.IsLocked())
					continue;
				#endif
				
				if (vehicle.GetCrew().Count() > 0)
					continue;

				vehicleEntity = vehicle.GetEntity();
				if (IsVehicleNearby(vehicleEntity))
					driven.Insert(vehicleEntity);

				if (driven.Count() == amount)
					return driven;
			}
		}

		return driven;
	}
	
	bool IsVehicleNearby(Object vehicle)
	{
		float maxDistance = GetExpansionSettings().GetMarket().GetMaxVehicleDistanceToTrader(vehicle.GetType());

		if (vector.Distance(m_Player.GetPosition(), vehicle.GetPosition()) <= maxDistance)
			return true;

		return false;
	}
}
