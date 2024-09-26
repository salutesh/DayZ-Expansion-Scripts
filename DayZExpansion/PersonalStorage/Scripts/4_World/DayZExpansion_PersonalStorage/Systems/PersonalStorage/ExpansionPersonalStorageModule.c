/**
 * ExpansionPersonalStorageModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionPersonalStoragePlayerInventory
{
	PlayerBase m_Player;
	ref array<EntityAI> m_Inventory;

	void ExpansionPersonalStoragePlayerInventory(PlayerBase player)
	{
		m_Player = player;
		m_Inventory = new array<EntityAI>;
		Enumerate();
	}

	void Enumerate()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(ExpansionTracing.MARKET, this);
#endif 

		array<EntityAI> items = new array<EntityAI>;
		m_Player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);
		AddPlayerItems(items);
	}

	private void AddPlayerItems(array<EntityAI> items, array<EntityAI> existing = NULL)
	{
		foreach (EntityAI item: items)
		{
			if (!item)
				continue;

			if (existing && existing.Find(item) > -1)
				continue;

			m_Inventory.Insert( item );
		}
	}
};

enum ExpansionPersonalStorageModuleCallback
{
	ItemStored = 1,
	ItemRetrieved = 2,
	Error = 4
};

[CF_RegisterModule(ExpansionPersonalStorageModule)]
class ExpansionPersonalStorageModule: CF_ModuleWorld
{
	protected static ExpansionPersonalStorageModule s_Instance;
	static string s_PersonalStorageConfigFolderPath = "$mission:expansion\\personalstorage\\";

	protected ref map<string, ref array<ref ExpansionPersonalStorageItem>> m_ItemsData; //! Server
	protected ref map<int, ref ExpansionPersonalStorageConfig> m_PersonalStorageConfig; //! Server

	protected ref ExpansionPersonalStoragePlayerInventory m_LocalEntityInventory; //! Client
	protected ref ScriptInvoker m_PersonalStorageMenuCallbackInvoker; //! Client
	protected ref ScriptInvoker m_PersonalStorageMenuInvoker; //! Client
	protected ref ExpansionPersonalStorageConfig m_PersonalStorageClientConfig; //! Client

	static ref TStringArray m_HardcodedExcludes = {"AugOptic", "Magnum_Cylinder", "Magnum_Ejector", "M97DummyOptics"};

	protected bool m_Initialized;

	void ExpansionPersonalStorageModule()
	{
		s_Instance = this;
	}

	override void OnInit()
	{
		EnableMissionStart();
		EnableMissionLoaded();
		EnableInvokeConnect();
		EnableClientDisconnect();
		Expansion_EnableRPCManager();
		
		Expansion_RegisterClientRPC("RPC_SendItemData");
		Expansion_RegisterClientRPC("RPC_Callback");
		Expansion_RegisterServerRPC("RPC_RequestRetrieveItem");
		Expansion_RegisterServerRPC("RPC_RequestDepositItem");
	}

	protected void CreateDirectoryStructure()
	{
		if (!FileExist(s_PersonalStorageConfigFolderPath))
			ExpansionStatic.MakeDirectoryRecursive(s_PersonalStorageConfigFolderPath);

		if (!FileExist(GetPersonalStorageDataDirectory()))
			ExpansionStatic.MakeDirectoryRecursive(GetPersonalStorageDataDirectory());
	}

	override void OnMissionLoaded(Class sender, CF_EventArgs args)
	{
		if (GetGame().IsClient())
			ClientModuleInit();
	}

	protected void ClientModuleInit()
	{
		if (GetGame().IsClient())
		{
			m_PersonalStorageMenuCallbackInvoker = new ScriptInvoker();
			m_PersonalStorageMenuInvoker = new ScriptInvoker();
		}
	}

	override void OnMissionStart(Class sender, CF_EventArgs args)
	{
		if (GetGame().IsServer() && GetGame().IsMultiplayer())
		{
			m_ItemsData = new map<string, ref array<ref ExpansionPersonalStorageItem>>;
			m_PersonalStorageConfig = new map<int, ref ExpansionPersonalStorageConfig>;
			if (GetExpansionSettings().GetPersonalStorage().Enabled)
			{
				CreateDirectoryStructure();
				LoadPersonalStorageServerConfig();
			}
			
			array<string> personalStorageDirs = ExpansionStatic.FindDirectoriesInLocation(GetPersonalStorageDataDirectory());
			foreach (string directoryName: personalStorageDirs)
			{
				if (!m_ItemsData.Contains(directoryName))
				{
#ifdef EXTRACE
					EXTrace.Start(EXTrace.PERSONALSTORAGE, this, "::OnMissionStart - Add personal storage data for player with UID: " + directoryName);
#endif 
					LoadPersonalStorageItemData(directoryName);
				}
				else
				{
#ifdef EXTRACE
					EXTrace.Start(EXTrace.PERSONALSTORAGE, this, "::OnMissionStart - Personal storage data for player with UID: " + directoryName + " already loaded! Skip..");
#endif 
				}
			}
		}

		m_Initialized = true;
	}

	protected void LoadPersonalStorageServerConfig()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.PERSONALSTORAGE, this);
#endif 

		//! Move existing configs (if any) from old to new location
		string dataDir = GetPersonalStorageDataDirectory();
		array<string> personalStorageFilesExisting = ExpansionStatic.FindFilesInLocation(dataDir, ".json");
		foreach (string existingFile: personalStorageFilesExisting)
		{
			ExpansionStatic.CopyFileOrDirectoryTree(dataDir + existingFile, s_PersonalStorageConfigFolderPath + existingFile, "", true);
		}
		
		array<string> personalStorageFiles = ExpansionStatic.FindFilesInLocation(s_PersonalStorageConfigFolderPath, ".json");
		if (personalStorageFiles.Count() > 0)
		{
			foreach (string fileName: personalStorageFiles)
			{
				GetPersonalStorageConfig(fileName, s_PersonalStorageConfigFolderPath);
			}
		}
		else
		{
			CreateDefaultPersonalStorageConfig();
		}
	}

	protected void CreateDefaultPersonalStorageConfig()
	{
		string worldname;
		GetGame().GetWorldName(worldname);
		worldname.ToLower();
		
		vector mapPos = GetDayZGame().GetWorldCenterPosition();
		ExpansionPersonalStorageConfig personalStorage;
		if (worldname.IndexOf("namalsk") > -1)
		{
			personalStorage = new ExpansionPersonalStorageConfig();
			personalStorage.SetStorageID(1);
			personalStorage.SetClassName("ExpansionPersonalStorageChest");
			personalStorage.SetDisplayName("Jalovisco - Personal Storage");
			personalStorage.SetDisplayIcon("Backpack");
			personalStorage.SetPosition(Vector(8617.609375, 14.767379, 10521.810547));
			personalStorage.SetOrientation(Vector(-35.0, 0, 0));
		#ifdef EXPANSIONMODQUESTS
			personalStorage.SetQuestID(500);
		#endif
		#ifdef EXPANSIONMODHARDLINE
			personalStorage.SetReputation(1000);
		#endif
		#ifdef EXPANSIONMODAI
			personalStorage.SetFaction("Survivors");
		#endif
			personalStorage.Save();
	
			m_PersonalStorageConfig.Insert(1, personalStorage);
	
			personalStorage.Spawn();
		}
		else
		{
			personalStorage = new ExpansionPersonalStorageConfig();
			personalStorage.SetStorageID(1);
			personalStorage.SetClassName("ExpansionPersonalStorageChest");
			personalStorage.SetDisplayName("Personal Storage");
			personalStorage.SetDisplayIcon("Backpack");
			personalStorage.SetPosition(mapPos);
			personalStorage.SetOrientation(Vector(0, 0, 0));
			personalStorage.Save();
		}
	}

	protected void GetPersonalStorageConfig(string fileName, string path)
	{
		ExpansionPersonalStorageConfig personalStorageConfig = ExpansionPersonalStorageConfig.Load(path + fileName);
		if (!personalStorageConfig)
			return;

		m_PersonalStorageConfig.Insert(personalStorageConfig.GetStorageID(), personalStorageConfig);
		personalStorageConfig.Spawn(); //! Spawn the personal storage.
	}

	protected void LoadPersonalStorageItemData(string directoyName)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.PERSONALSTORAGE, this);
#endif 
		
		string storagePath = ExpansionPersonalStorageModule.GetPersonalStorageDataDirectory() + directoyName + "\\";
		if (FileExist(storagePath))
		{
			array<string> personalStorageFiles = ExpansionStatic.FindFilesInLocation(storagePath, ".json");
			for (int i = personalStorageFiles.Count() - 1; i >= 0; i--)
			{
				GetPersonalStorageItemData(personalStorageFiles[i], storagePath);
			}
		}
	}

	protected void GetPersonalStorageItemData(string fileName, string path)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.PERSONALSTORAGE, this);
#endif 
		
		ExpansionPersonalStorageItem itemData = ExpansionPersonalStorageItem.Load(path + fileName);
		if (!itemData)
			return;

		//! Check if the entity storage file still exists otherwise we delete the personal storage item file and dont add it to the system.
		if (!FileExist(itemData.GetEntityStorageFileName()))
		{
			PrintFormat("%1::GetPersonalStorageItemData - Delete personal storage file %2 as the associated entity storage file does not exist!", ToString(), path + fileName);
			DeleteFile(path + fileName); //! Delete the personal storage item JSON file.
			itemData = null;
			return;
		}
		
		//! Check if the item still exists!		
		if (!ExpansionStatic.ItemExists(itemData.GetClassName()))
		{
			PrintFormat("%1::GetPersonalStorageItemData - Delete personal storage file %2 and entity storage file %3 as the associated item %4 does not exist!", ToString(), path + fileName, itemData.GetEntityStorageFileName(), itemData.GetClassName());
			DeleteFile(path + fileName); //! Delete the personal storage item JSON file.
			ExpansionEntityStorageModule.DeleteFiles(itemData.GetEntityStorageBaseName()); //! Delete the entity storage .bin file.
			itemData = null;
			return;
		}
		
		//! Check if any of the container items
		bool save;
		itemData.CheckContainerItems(path + fileName, save);
		if (save)
			ExpansionPersonalStorageItem.Save(itemData);

		string playerUID = itemData.GetOwnerUID();
		array<ref ExpansionPersonalStorageItem> items;
		if (m_ItemsData.Find(playerUID, items))
		{
			items.Insert(itemData);
			m_ItemsData.Set(playerUID, items);
		}
		else
		{
			items = new array<ref ExpansionPersonalStorageItem>;
			items.Insert(itemData);
			m_ItemsData.Insert(playerUID, items);
		}
	}
	
	//! Server
	void SendItemData(PlayerIdentity identity, int storageID = -1, string displayName = string.Empty, string displayIcon = string.Empty, ExpansionPersonalStorageModuleCallback callback = 0)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.PERSONALSTORAGE, this);
#endif
		
		string playerUID = identity.GetId();
		ExpansionPersonalStorageConfig storageConfig = GetPersonalStorageConfigByID(storageID);
		if (!storageConfig)
		{
			Error(ToString() + "::SendItemData - Could not get personal stroage data for ID " + storageID);
			return;
		}

		auto rpc = Expansion_CreateRPC("RPC_SendItemData");
		storageConfig.OnSend(rpc);
		rpc.Write(storageID);
		rpc.Write(displayName);
		rpc.Write(displayIcon);

		array<ref ExpansionPersonalStorageItem> items = m_ItemsData[playerUID];
		array<ExpansionPersonalStorageItem> itemsToSend = new array<ExpansionPersonalStorageItem>;

		if (items && items.Count() > 0)
		{
			foreach (ExpansionPersonalStorageItem item: items)
			{
				if (storageConfig.IsGlobalStorage() || item.GetStorageID() == storageID)
				{
					if (itemsToSend.Find(item) == -1)
						itemsToSend.Insert(item);
				}
			}
		}

		rpc.Write(itemsToSend.Count());

		foreach (ExpansionPersonalStorageItem storedItem: itemsToSend)
		{
			storedItem.OnSend(rpc);
		}

		rpc.Write(callback);
		rpc.Expansion_Send(true, identity);
	}
	
	//! Client
	protected void RPC_SendItemData(PlayerIdentity senderRPC, Object target, ParamsReadContext ctx)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.PERSONALSTORAGE, this);
#endif 
		
		ExpansionPersonalStorageConfig storageConfig = new ExpansionPersonalStorageConfig();
		if (!storageConfig.OnRecieve(ctx))
			return;

		m_PersonalStorageClientConfig = storageConfig;

		int storageID;
		if (!ctx.Read(storageID))
			return;

		string displayName;
		if (!ctx.Read(displayName))
			return;

		string displayIcon;
		if (!ctx.Read(displayIcon))
			return;

		int itemsCount;
		if (!ctx.Read(itemsCount))
			return;

		array<ref ExpansionPersonalStorageItem> playerItems = new array<ref ExpansionPersonalStorageItem>;
		for (int i = 0; i < itemsCount; ++i)
		{
			ExpansionPersonalStorageItem item = new ExpansionPersonalStorageItem();
			if (!item.OnRecieve(ctx))
				return;

			item.SetIsStored(true);
			playerItems.Insert(item);
		}
	
		if (!GetDayZGame().GetExpansionGame().GetExpansionUIManager().GetMenu())
			GetDayZGame().GetExpansionGame().GetExpansionUIManager().CreateSVMenu("ExpansionPersonalStorageMenu");

		if (playerItems && playerItems.Count() > 0)
		{
			m_PersonalStorageMenuInvoker.Invoke(storageID, playerItems, displayName, displayIcon);
		}
		else
		{
			m_PersonalStorageMenuInvoker.Invoke(storageID, null, displayName, displayIcon);
		}

		int callback;
		if (!ctx.Read(callback))
			return;

		m_PersonalStorageMenuCallbackInvoker.Invoke(callback);
		
		playerItems.Clear();
		playerItems = null;
	}
	
	//! Client
	protected void RPC_Callback(PlayerIdentity senderRPC, Object target, ParamsReadContext ctx)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.PERSONALSTORAGE, this);
#endif 

		int callback;
		if (!ctx.Read(callback))
			return;

		m_PersonalStorageMenuCallbackInvoker.Invoke(callback);
	}
	
	//! Client
	void RequestRetrieveItem(ExpansionPersonalStorageItem item, int storageID)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.PERSONALSTORAGE, this);
#endif 

		TIntArray globalID = item.GetGlobalID();
		if (!globalID)
			return;
		
		auto rpc = Expansion_CreateRPC("RPC_RequestRetrieveItem");
		rpc.Write(storageID);
		rpc.Write(globalID);
		rpc.Expansion_Send(true);
	}
	
	//! Server
	protected void RPC_RequestRetrieveItem(PlayerIdentity senderRPC, Object target, ParamsReadContext ctx)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.PERSONALSTORAGE, this);
#endif 

		int storageID;
		if (!ctx.Read(storageID))
		{
			Error(ToString() + "::RPC_RequestRetrieveItem - Could not read storageID.");
			return;
		}

		TIntArray globalID;
		if (!ctx.Read(globalID))
		{
			Error(ToString() + "::RPC_RequestRetrieveItem - Could not read globalID.");
			return;
		}

		string playerUID = senderRPC.GetId();
		ExpansionPersonalStorageItem item = GetPersonalItemByGlobalID(playerUID, globalID);
		if (!item)
		{
			Error(ToString() + "::RPC_RequestRetrieveItem - Could not get deposited item!");
			return;
		}

		PlayerBase player = PlayerBase.Cast(senderRPC.GetPlayer());
		if (!player)
		{
			Error(ToString() + "::RPC_RequestRetrieveItem - Could not get player!");
			return;
		}

		EntityAI loadedEntity;
		if (!LoadItem(item, player, loadedEntity))
		{
			Error(ToString() + "::RPC_RequestRetrieveItem - Could not restore stored item for personal storage data: " + item.GetGlobalID());
			return;
		}

		string displayName = loadedEntity.GetDisplayName();
		RemoveItemByGlobalID(playerUID, globalID);
		SendItemData(senderRPC, storageID, "", "", ExpansionPersonalStorageModuleCallback.ItemRetrieved);

		ExpansionNotification(new StringLocaliser("Item Retrieved!"), new StringLocaliser("You have retrieved a %1.", displayName), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_SUCCESS, 7, ExpansionNotificationType.TOAST).Create(senderRPC);
	}
	
	//! Client
	void RequestDepositItem(int storageID, Entity item)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.PERSONALSTORAGE, this);
#endif 

		auto rpc = Expansion_CreateRPC("RPC_RequestDepositItem");
		rpc.Write(storageID);
		rpc.Expansion_Send(item, true);
	}
	
	//! Server
	protected void RPC_RequestDepositItem(PlayerIdentity senderRPC, Object target, ParamsReadContext ctx)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.PERSONALSTORAGE, this);
#endif 

		int storageID;
		if (!ctx.Read(storageID))
		{
			Error(ToString() + "::RPC_RequestDepositItem - Could not read storageID.");
			return;
		}

		EntityAI objEntity;
		if (!Class.CastTo(objEntity, target))
		{
			Error(ToString() + "::RPC_RequestDepositItem - Could not get target object!");
			return;
		}

		ExpansionPersonalStorageConfig storageConfig = GetPersonalStorageConfigByID(storageID);
		if (!storageConfig)
		{
			Error(ToString() + "::RPC_RequestDepositItem - Could not get personal stroage data for ID " + storageID);			
			return;
		}

		if (!ConditonCheck(senderRPC, storageID, objEntity, storageConfig.IsGlobalStorage()))
		{
			CallbackError(senderRPC);
			return;
		}

		string displayName = objEntity.GetDisplayName();
		string playerUID = senderRPC.GetId();
		PlayerBase player = PlayerBase.ExpansionGetPlayerByIdentity(senderRPC);
		if (!player)
		{
			Error(ToString() + "::RPC_RequestDepositItem - Could not get player for UID " + playerUID);
			return;
		}

		ExpansionPersonalStorageItem newItem = new ExpansionPersonalStorageItem();
		newItem.SetFromItem(objEntity, playerUID);
		if (!newItem.IsGlobalIDValid())
		{
			Error(ToString() + "::RPC_RequestDepositItem - Global ID for item is invalid! Global ID: " + newItem.GetGlobalID());
			return;
		}

		newItem.SetIsStored(true);
		newItem.SetStorageID(storageID);

		if (!StoreItem(newItem, objEntity))
		{
			Error(ToString() + "::RPC_RequestDepositItem - Could not store item!");
			return;
		}
		
		newItem.SetStoreTime();
		AddStoredItem(playerUID, newItem);
		newItem.Save();
		SendItemData(senderRPC, storageID, "", "", ExpansionPersonalStorageModuleCallback.ItemStored);
		ExpansionNotification(new StringLocaliser("Item Deposited!"), new StringLocaliser("You have deposited the item %1.", displayName), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_SUCCESS, 7, ExpansionNotificationType.TOAST).Create(senderRPC);
	}

	void StorePersonalStorageCase(PlayerBase player)
	{
		string playerUID = player.GetIdentity().GetId();

		ExpansionPersonalProtectiveCaseBase personalStorageCaseItem;
		if (!ExpansionPersonalProtectiveCaseBase.CastTo(personalStorageCaseItem, player.FindAttachmentBySlotName("ExpansionPersonalContainer")))
			return;

		if (!personalStorageCaseItem.ExpansionIsContainerOwner(player))
			return;

		ExpansionPersonalStorageItem personalStorageCase = new ExpansionPersonalStorageItem();
		personalStorageCase.SetFromItem(personalStorageCaseItem, playerUID);
		if (!personalStorageCase.IsGlobalIDValid())
		{
			Error(ToString() + "::StorePersonalStorageCase - Global ID for personal storage case is invalid! Global ID: " + personalStorageCase.GetGlobalID());
			personalStorageCase = null;
			return;
		}

		personalStorageCase.SetIsStored(true);
		personalStorageCase.SetStorageID(-1);

		if (!StoreItem(personalStorageCase, personalStorageCaseItem))
		{
			Error(ToString() + "::StorePersonalStorageCase - Could not store item!");
			personalStorageCase = null;
			return;
		}

		AddStoredItem(playerUID, personalStorageCase);
		personalStorageCase.Save();
	}

	void RestorePersonalStorageCase(PlayerBase player)
	{
		string playerUID = player.GetIdentity().GetId();
		array<ref ExpansionPersonalStorageItem> items;
		if (m_ItemsData.Find(playerUID, items))
		{
			foreach (ExpansionPersonalStorageItem item: items)
			{
				typename itemType = item.GetClassName().ToType();
				if (itemType && itemType.IsInherited(ExpansionPersonalProtectiveCaseBase))
				{
					EntityAI loadedEntity;
					if (!LoadItem(item, player, loadedEntity))
					{
						Error(ToString() + "::RestorePersonalStorageCase - Could not restore stored personal storage case with global ID: " + item.GetGlobalID());
						return;
					}

					return;
				}
			}
		}
	}

	void SpawnPesonalStorageCase(PlayerBase player)
	{
		//! Make sure he has not a case already.
		if (player.FindAttachmentBySlotName("ExpansionPersonalContainer"))
			return;

		Object obj = GetGame().CreateObjectEx("ExpansionSmallPersonalProtectorCase", player.GetPosition(), ECE_SETUP | ECE_UPDATEPATHGRAPH | ECE_CREATEPHYSICS | ECE_NOLIFETIME);
		ExpansionPersonalProtectiveCaseBase personalStorageCase;
		if (!ExpansionPersonalProtectiveCaseBase.CastTo(personalStorageCase, obj))
		{
			if (obj)
				GetGame().ObjectDelete(obj);

			Error(ToString() + "::SpawnPesonalStorageCase - Could not spawn Personal Storage case!");
			return;
		}

		personalStorageCase.ExpansionSetContainerOwner(player);
	}

	//! Boilerplaite for better modding purposes.
	bool ConditonCheck(PlayerIdentity identity, int storageID, EntityAI item, bool isGlobal = false)
	{
		auto settings = GetExpansionSettings().GetPersonalStorage();
		int playerItemsCount = GetPlayerItemsCount(identity.GetId(), storageID, isGlobal);

	#ifdef EXPANSIONMODHARDLINE
		ExpansionPersonalStorageConfig storageConfig = GetPersonalStorageConfigByID(storageID);
		if (storageConfig)
		{
			int reputationToUnlock = storageConfig.GetReputation();
			PlayerBase player = PlayerBase.ExpansionGetPlayerByIdentity(identity);
			if (player)
			{
				int reputation = player.Expansion_GetReputation();
				int limit = GetStorageLimitByReputation(reputation, reputationToUnlock);
				if (playerItemsCount >= limit)
				{
					ExpansionNotification(new StringLocaliser("Max items to deposit reached!"), new StringLocaliser("You already have %1 items in total in your storage. Limit is %2.", playerItemsCount.ToString(), limit.ToString()), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_ERROR, 7, ExpansionNotificationType.TOAST).Create(identity);
					return false;
				}
			}
		}
	#else
		if (settings.MaxItemsPerStorage != -1 && playerItemsCount >= settings.MaxItemsPerStorage)
		{
			ExpansionNotification(new StringLocaliser("Max items to deposit reached!"), new StringLocaliser("You already have %1 items in total in your storage. Limit is %2.", playerItemsCount.ToString(), settings.MaxItemsPerStorage.ToString()), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_ERROR, 7, ExpansionNotificationType.TOAST).Create(identity);			
			return false;
		}
	#endif

		return true;
	}

#ifdef EXPANSIONMODHARDLINE
	int GetStorageLimitByReputation(int reputation, int reputationToUnlock)
	{
		auto hardlineSettings = GetExpansionSettings().GetHardline();
		auto personalStorageSettings = GetExpansionSettings().GetPersonalStorage();
		int minStorage = personalStorageSettings.MaxItemsPerStorage;
		int storageLimit = minStorage;
		storageLimit = hardlineSettings.GetLimitByReputation(reputation, reputationToUnlock, minStorage, 1);

		return storageLimit;
	}
#endif
	
	//! Server
	void CallbackError(PlayerIdentity identity)
	{
		auto rpc = Expansion_CreateRPC("RPC_Callback");
		rpc.Write(ExpansionPersonalStorageModuleCallback.Error);
		rpc.Expansion_Send(true, identity);
	}

	void AddStoredItem(string playerUID, ExpansionPersonalStorageItem item)
	{
		array<ref ExpansionPersonalStorageItem> currentItems;
		if (m_ItemsData.Find(playerUID, currentItems))
		{
			currentItems.Insert(item);
			m_ItemsData.Set(playerUID, currentItems);
		}
		else
		{
			currentItems = new array<ref ExpansionPersonalStorageItem>;
			currentItems.Insert(item);
			m_ItemsData.Insert(playerUID, currentItems);
		}
	}

	array<EntityAI> GetSlotItems(EntityAI entity, inout bool hasEngineBeltSlot)
	{
		array<EntityAI> slotItems = new array<EntityAI>;
		for (int i = 0; i < entity.GetInventory().GetAttachmentSlotsCount(); i++)
		{
			int slotID = entity.GetInventory().GetAttachmentSlotId(i);
			string slotName = InventorySlots.GetSlotName(slotID);
			if (slotName == "EngineBelt")
				hasEngineBeltSlot = true;

			EntityAI slotItem = entity.GetInventory().FindAttachment(slotID);
			if (slotItem)
				slotItems.Insert(slotItem);
		}

		return slotItems;
	}

	protected int GetPlayerItemsCount(string playerUID, int storageID = -1, bool isGlobal = false)
	{
		int count;
		foreach (string uid, array<ref ExpansionPersonalStorageItem> items: m_ItemsData)
		{
			if (uid != playerUID)
				continue;

			foreach (ExpansionPersonalStorageItem item: items)
			{
				if (item.GetOwnerUID() == playerUID)
				{
					if (!isGlobal && item.GetStorageID() != storageID)
						continue;

					count++;
				}
			}
		}

		return count;
	}

	protected bool StoreItem(ExpansionPersonalStorageItem item, EntityAI itemEntity)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.PERSONALSTORAGE, this);
#endif 

		if (!itemEntity)
		{
			Error(ToString() + "::StoreItem - Could not get item entity!");
			return false;
		}

		bool success = ExpansionEntityStorageModule.SaveToFile(itemEntity, item.GetEntityStorageFileName());
		if (!success)
		{
			Error(ToString() + "::StoreVehicle - Could not store item entity!");
			return false;
		}

		ExpansionEntityStorageModule.DeleteSavedEntity(itemEntity);

		return true;
	}

	protected bool LoadItem(ExpansionPersonalStorageItem item, PlayerBase player, out EntityAI loadedEntity = null)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.PERSONALSTORAGE, this);
#endif 

		if (!ExpansionEntityStorageModule.RestoreFromFile(item.GetEntityStorageFileName(), loadedEntity, null, player))
		{
			Error(ToString() + "::LoadItem - Could not restore item " + item.GetClassName() + " from file " + item.GetEntityStorageFileName());
			return false;
		}

		return true;
	}

	protected void RemoveItemByGlobalID(string playerUID, TIntArray globalID)
	{
		array<ref ExpansionPersonalStorageItem> items;
		if (!m_ItemsData.Find(playerUID, items))
			return;

		for (int i = items.Count() - 1; i >= 0; i--)
		{
			ExpansionPersonalStorageItem item = items[i];
			if (item.IsGlobalIDValid() && item.IsGlobalIDEqual(globalID))
			{
				ExpansionEntityStorageModule.DeleteFiles(item.GetEntityStorageBaseName());
				items.RemoveOrdered(i);
			}
		}

		m_ItemsData.Insert(playerUID, items);

		string baseName = ExpansionStatic.IntToHex(globalID);
		string filePath = GetPersonalStorageDataDirectory() + playerUID + "\\" + baseName + ".json";
		if (FileExist(filePath))
			DeleteFile(filePath);
	}

	protected ExpansionPersonalStorageItem GetPersonalItemByGlobalID(string playerUID, TIntArray globalID)
	{
		array<ref ExpansionPersonalStorageItem> items;
		if (m_ItemsData.Find(playerUID, items))
		{
			for (int i = 0; i < items.Count(); i++)
			{
				ExpansionPersonalStorageItem item = items[i];
				if (item && item.IsGlobalIDValid() && item.IsGlobalIDEqual(globalID))
				{
					return item;
				}
			}
		}

		return null;
	}

	static bool ItemCheckEx(EntityAI item)
	{
		if (ExpansionStatic.IsAnyOf(item, GetExpansionSettings().GetPersonalStorage().ExcludedClassNames))
			return false;

		if (item.IsRuined())
			return false;

		//! Don`t add rotten food items
		Edible_Base foodItem;
		if (Class.CastTo(foodItem, item) && foodItem.HasFoodStage())
		{
			FoodStage foodStage = foodItem.GetFoodStage();
			FoodStageType foodStageType = foodStage.GetFoodStageType();
			if (foodStageType == FoodStageType.ROTTEN || foodStageType == FoodStageType.BURNED)
				return false;
		}

	#ifdef WRDG_DOGTAGS
		//! Don`t add players own dogtag
		if (item.IsInherited(Dogtag_Base))
		{
			if (item.GetHierarchyRootPlayer())
				return false;
		}
	#endif

	#ifdef EXPANSIONMODQUESTS
		//! Don`t add quest items
		ItemBase itemIB;
		if (Class.CastTo(itemIB, item))
		{
			if (itemIB.Expansion_IsQuestItem() || itemIB.Expansion_IsQuestGiver())
				return false;
		}
	#endif
		
		if (!item.CanPutInCargo(null))
			return false;
		
		return true;
	}

	array<EntityAI> LocalGetEntityInventory()
	{
		return m_LocalEntityInventory.m_Inventory;
	}

	void EnumeratePlayerInventory(PlayerBase player)
	{
		m_LocalEntityInventory = new ExpansionPersonalStoragePlayerInventory(player);
	}

	ScriptInvoker GetPersonalStorageMenuCallbackSI()
	{
		return m_PersonalStorageMenuCallbackInvoker;
	}

	ScriptInvoker GetPersonalStorageMenuSI()
	{
		return m_PersonalStorageMenuInvoker;
	}

	ExpansionPersonalStorageConfig GetPersonalStorageConfigByID(int storageID)
	{
		return m_PersonalStorageConfig[storageID];
	}

	ExpansionPersonalStorageConfig GetPersonalStorageClientConfig()
	{
		return m_PersonalStorageClientConfig;
	}

	static ExpansionPersonalStorageModule GetModuleInstance()
	{
		return s_Instance;
	}
	
	static string GetPersonalStorageDataDirectory()
	{
		int instance_id = GetGame().ServerConfigGetInt("instanceId");
		return "$mission:storage_" + instance_id + "\\expansion\\personalstorage\\";
	}
};
