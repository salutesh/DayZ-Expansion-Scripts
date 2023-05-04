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
		auto trace = EXTrace.Start(ExpansionTracing.MARKET);

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

	protected ref array<ref ExpansionPersonalStorageItem> m_PlayerItems //! Client
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
		EnableInvokeDisconnect();
		EnableRPC();
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
			m_PlayerItems = new array<ref ExpansionPersonalStorageItem>;
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
		}

		m_Initialized = true;
	}

	protected void LoadPersonalStorageServerConfig()
	{
		auto trace = EXTrace.Start(EXTrace.PERSONALSTORAGE, this);

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

	override void OnInvokeConnect(Class sender, CF_EventArgs args)
	{
		auto trace = EXTrace.Start(EXTrace.PERSONALSTORAGE, this);

		super.OnInvokeConnect(sender, args);

		auto cArgs = CF_EventPlayerArgs.Cast(args);
		if (GetGame().IsServer() && GetGame().IsMultiplayer() && GetExpansionSettings().GetPersonalStorage().Enabled)
		{
			string playerUID = cArgs.Identity.GetId();
			LoadPersonalStorageItemData(playerUID);

			if (GetExpansionSettings().GetPersonalStorage().UsePersonalStorageCase)
				RestorePersonalStorageCase(cArgs.Player);
		}
	}

	override void OnClientDisconnect(Class sender, CF_EventArgs args)
	{
		auto trace = EXTrace.Start(EXTrace.PERSONALSTORAGE, this);

		super.OnClientDisconnect(sender, args);

		auto cArgs = CF_EventPlayerDisconnectedArgs.Cast(args);

		if (GetGame().IsServer() && GetGame().IsMultiplayer())
		{
			string playerUID = cArgs.Identity.GetId();
			if (m_ItemsData.Contains(playerUID))
			{
				m_ItemsData.Remove(playerUID);
			}
		}
	}

	protected void LoadPersonalStorageItemData(string playerUID)
	{
		auto trace = EXTrace.Start(EXTrace.PERSONALSTORAGE, this);

		string storagePath = ExpansionPersonalStorageModule.GetPersonalStorageDataDirectory() + playerUID + "\\";
		if (FileExist(storagePath))
		{
			array<string> personalStorageFiles = ExpansionStatic.FindFilesInLocation(storagePath, ".json");
			foreach (string fileName: personalStorageFiles)
			{
				GetPersonalStorageItemData(fileName, storagePath);
			}
		}
	}

	protected void GetPersonalStorageItemData(string fileName, string path)
	{
		auto trace = EXTrace.Start(EXTrace.PERSONALSTORAGE, this);

		ExpansionPersonalStorageItem itemData = ExpansionPersonalStorageItem.Load(path + fileName);
		if (!itemData)
			return;

		//! Check if the entity storage file still exists otherwise we delete the personal storage item file and dont add it to the system.
		if (!FileExist(itemData.GetEntityStorageFileName()))
		{
			DeleteFile(path + fileName); //! Delete the personal storage item JSON file.
			itemData = null;
			return;
		}

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

	override int GetRPCMin()
	{
		return ExpansionPersonalStorageModuleRPC.INVALID;
	}

	override int GetRPCMax()
	{
		return ExpansionPersonalStorageModuleRPC.COUNT;
	}

	override void OnRPC(Class sender, CF_EventArgs args)
	{
		super.OnRPC(sender, args);
		auto rpc = CF_EventRPCArgs.Cast(args);

		switch (rpc.ID)
		{
			case ExpansionPersonalStorageModuleRPC.RequestDepositItem:
			{
				RPC_RequestDepositItem(rpc.Context, rpc.Sender, rpc.Target);
				break;
			}
			case ExpansionPersonalStorageModuleRPC.RequestRetrieveItem:
			{
				RPC_RequestRetrieveItem(rpc.Context, rpc.Sender, rpc.Target);
				break;
			}
			case ExpansionPersonalStorageModuleRPC.SendItemData:
			{
				RPC_SendItemData(rpc.Context, rpc.Sender, rpc.Target);
				break;
			}
			case ExpansionPersonalStorageModuleRPC.Callback:
			{
				RPC_Callback(rpc.Context, rpc.Sender, rpc.Target);
				break;
			}
		}
	}

	void SendItemData(PlayerIdentity identity, int storageID = -1, string displayName = string.Empty, string displayIcon = string.Empty, ExpansionPersonalStorageModuleCallback callback = 0)
	{
		auto trace = EXTrace.Start(EXTrace.PERSONALSTORAGE, this);
		
		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
		{
			Error(ToString() + "::SendItemData - Tryed to call SendItemData on Client!");
			return;
		}

		string playerUID = identity.GetId();

		ExpansionPersonalStorageConfig storageConfig = GetPersonalStorageConfigByID(storageID);
		if (!storageConfig)
		{
			Error(ToString() + "::SendItemData - Could not get personal stroage data for ID " + storageID);
			return;
		}

		auto rpc = ExpansionScriptRPC.Create();
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

		rpc.Send(NULL, ExpansionPersonalStorageModuleRPC.SendItemData, true, identity);
	}

	protected void RPC_SendItemData(ParamsReadContext ctx, PlayerIdentity senderRPC, Object target)
	{
		if (!ExpansionScriptRPC.CheckMagicNumber(ctx))
		{
			Error(ToString() + "::RPC_SendItemData - Magic number check failed!");
			return;
		}

		if (!GetGame().IsClient())
		{
			Error(ToString() + "::RPC_SendItemData - Tryed to call RPC_SendItemData on Server!");
			return;
		}

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

		if (!m_PlayerItems)
		{
			m_PlayerItems = new array<ref ExpansionPersonalStorageItem>;
		}
		else
		{
			m_PlayerItems.Clear();
		}

		for (int i = 0; i < itemsCount; ++i)
		{
			ExpansionPersonalStorageItem item = new ExpansionPersonalStorageItem();
			if (!item.OnRecieve(ctx))
				return;

			item.SetIsStored(true);
			m_PlayerItems.Insert(item);
		}

		GetDayZGame().GetExpansionGame().GetExpansionUIManager().CreateSVMenu("ExpansionPersonalStorageMenu");

		if (m_PlayerItems && m_PlayerItems.Count() > 0)
		{
			m_PersonalStorageMenuInvoker.Invoke(storageID, m_PlayerItems, displayName, displayIcon);
		}
		else
		{
			m_PersonalStorageMenuInvoker.Invoke(storageID, null, displayName, displayIcon);
		}

		int callback;
		if (!ctx.Read(callback))
			return;

		m_PersonalStorageMenuCallbackInvoker.Invoke(callback);
	}

	protected void RPC_Callback(ParamsReadContext ctx, PlayerIdentity senderRPC, Object target)
	{
		auto trace = EXTrace.Start(EXTrace.PERSONALSTORAGE, this);

		if (!ExpansionScriptRPC.CheckMagicNumber(ctx))
			return;

		if (!GetGame().IsClient())
		{
			Error(ToString() + "::RPC_Callback - Tryed to call RPC_Callback on Server!");
			return;
		}

		int callback;
		if (!ctx.Read(callback))
			return;

		m_PersonalStorageMenuCallbackInvoker.Invoke(callback);
	}

	void RequestRetrieveItem(ExpansionPersonalStorageItem item, int storageID)
	{
		auto trace = EXTrace.Start(EXTrace.PERSONALSTORAGE, this);
		
		if (!GetGame().IsClient())
		{
			Error(ToString() + "::RequestRetrieveItem - Tryed to call RequestRetrieveItem on Server!");
			return;
		}

		TIntArray globalID = item.GetGlobalID();

		auto rpc = ExpansionScriptRPC.Create();
		rpc.Write(storageID);
		rpc.Write(globalID);
		rpc.Send(NULL, ExpansionPersonalStorageModuleRPC.RequestRetrieveItem, true);
	}

	protected void RPC_RequestRetrieveItem(ParamsReadContext ctx, PlayerIdentity senderRPC, Object target)
	{
		auto trace = EXTrace.Start(EXTrace.PERSONALSTORAGE, this);

		if (!ExpansionScriptRPC.CheckMagicNumber(ctx))
		{
			Error(ToString() + "::RPC_RequestRetrieveItem - Magic number check failed!");
			return;
		}

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
		{
			Error(ToString() + "::RPC_RequestRetrieveItem - Tryed to call RPC_RequestRetrieveItem on Server!");
			return;
		}

		if (!senderRPC)
		{
			Error(ToString() + "::RPC_RequestRetrieveItem - Could not get player identity!");
			return;
		}

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

		ExpansionNotification(new StringLocaliser("Item Retrieved!"), new StringLocaliser("You have retrieved a %1.", displayName), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7, ExpansionNotificationType.TOAST).Create(senderRPC);
	}

	void RequestDepositItem(int storageID, Entity item)
	{
		auto trace = EXTrace.Start(EXTrace.PERSONALSTORAGE, this);

		if (!GetGame().IsClient())
		{
			Error(ToString() + "::RequestDepositItem - Tryed to call RequestDepositItem on Server!");
			return;
		}

		auto rpc = ExpansionScriptRPC.Create();
		rpc.Write(storageID);
		rpc.Send(item, ExpansionPersonalStorageModuleRPC.RequestDepositItem, true);
	}

	protected void RPC_RequestDepositItem(ParamsReadContext ctx, PlayerIdentity senderRPC, Object target)
	{
		auto trace = EXTrace.Start(EXTrace.PERSONALSTORAGE, this);

		if (!ExpansionScriptRPC.CheckMagicNumber(ctx))
		{
			Error(ToString() + "::RPC_RequestDepositItem - Magic number check failed!");
			return;
		}

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
		{
			Error(ToString() + "::RPC_RequestDepositItem - Tryed to call RPC_RequestDepositItem on Server!");
			return;
		}

		if (!senderRPC)
		{
			Error(ToString() + "::RPC_RequestDepositItem - Could not get player identity!");
			return;
		}

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

		ExpansionNotification(new StringLocaliser("Item Deposited!"), new StringLocaliser("You have deposited the item %1.", displayName), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7, ExpansionNotificationType.TOAST).Create(senderRPC);
	}

	void StorePersonalStorageCase(PlayerBase player)
	{
		auto trace = EXTrace.Start(EXTrace.PERSONALSTORAGE, this);

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
		auto trace = EXTrace.Start(EXTrace.PERSONALSTORAGE, this);

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
		if (settings.MaxItemsPerStorage != -1 && playerItemsCount >= settings.MaxItemsPerStorage)
		{
			ExpansionNotification(new StringLocaliser("Max items to deposit reached!"), new StringLocaliser("You already have %1 items in total in your storage. Limit is %2.", playerItemsCount.ToString(), settings.MaxItemsPerStorage.ToString()), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_ERROR, 7, ExpansionNotificationType.TOAST).Create(identity);			
			return false;
		}

	#ifdef EXPANSIONMODHARDLINE
		ExpansionPersonalStorageConfig storageConfig = GetPersonalStorageConfigByID(storageID);
		if (!storageConfig)
			return false;

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
	#endif

		return true;
	}

#ifdef EXPANSIONMODHARDLINE
	int GetStorageLimitByReputation(int reputation, int reputationToUnlock)
	{
		auto trace = EXTrace.Start(EXTrace.PERSONALSTORAGE, this);

		auto hardlineSettings = GetExpansionSettings().GetHardline();
		auto personalStorageSettings = GetExpansionSettings().GetPersonalStorage();
		int minStorage = personalStorageSettings.MaxItemsPerStorage;
		int storageLimit = minStorage;
		storageLimit = hardlineSettings.GetLimitByReputation(reputation, reputationToUnlock, minStorage, 1);

		return storageLimit;
	}
#endif

	void CallbackError(PlayerIdentity senderRPC)
	{
		auto rpc = ExpansionScriptRPC.Create();
		rpc.Write(ExpansionPersonalStorageModuleCallback.Error);
		rpc.Send(NULL, ExpansionPersonalStorageModuleRPC.Callback, true, senderRPC);
	}

	void AddStoredItem(string playerUID, ExpansionPersonalStorageItem item)
	{
		auto trace = EXTrace.Start(EXTrace.PERSONALSTORAGE, this);
		
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
		auto trace = EXTrace.Start(EXTrace.PERSONALSTORAGE, this);

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

		GetGame().ObjectDelete(itemEntity);

		return true;
	}

	protected bool LoadItem(ExpansionPersonalStorageItem item, PlayerBase player, out EntityAI loadedEntity = null)
	{
		auto trace = EXTrace.Start(EXTrace.PERSONALSTORAGE, this);

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
				DeleteFile(item.GetEntityStorageFileName());
				items.RemoveOrdered(i);
			}
		}

		m_ItemsData.Insert(playerUID, items);

		string fileName = ExpansionStatic.IntToHex(globalID);
		string filePath = GetPersonalStorageDataDirectory() + playerUID + "\\" + fileName + ".json";
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

	static bool ItemCheck(EntityAI item)
	{
		if (!ItemCheckEx(item))
			return false;

		return true;
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
			if (itemIB.IsQuestItem() || itemIB.IsQuestGiver())
				return false;
		}
	#endif

		//! Don`t add any active items.
		if (item.HasEnergyManager())
		{
			if (item.GetCompEM().IsWorking())
				return false;
		}

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
