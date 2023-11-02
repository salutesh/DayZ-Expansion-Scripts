/**
 * ExpansionGarageModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

enum ExpansionGarageModuleCallback
{
	VehicleStored = 1,
	VehicleRetrieved,
	NoVehicles,
	NoTerritory,
	Update
};

[CF_RegisterModule(ExpansionGarageModule)]
class ExpansionGarageModule: CF_ModuleWorld
{
	static string s_GarageFolderPath;

	static ExpansionGarageModule s_Instance;

#ifdef EXPANSIONMODBASEBUILDING
	protected ref ExpansionTerritoryModule m_TerritoryModule;
	protected ref ExpansionTerritory m_TerritoryTemp;
	protected ref map<int, ExpansionParkingMeter> m_TerritoryParkingMeters;
	protected ref array<ExpansionParkingMeter> m_ParkingMeters;
#endif
	protected ref ScriptInvoker m_GarageMenuInvoker; //! Client
	protected ref ScriptInvoker m_GarageMenuCallbackInvoker; //! Client
	protected ref array<ref ExpansionGarageData> m_GarageData;

#ifdef EXPANSIONMODGROUPS
	protected ref ExpansionPartyModule m_PartyModule;
	protected ref ExpansionPartyData m_PartyDataTemp;
#endif

#ifdef EXPANSIONMODMARKET
	protected ref ExpansionMarketModule m_MarketModule;
#endif

	void ExpansionGarageModule()
	{
		s_Instance = this;
	}

	override void OnInit()
	{
		super.OnInit();

		EnableMissionStart();
		Expansion_EnableRPCManager();

		Expansion_RegisterServerRPC("RPC_RequestPlayerVehicles");
		Expansion_RegisterClientRPC("RPC_SendPlayerVehicles");
		Expansion_RegisterServerRPC("RPC_DepositVehicleRequest");
		Expansion_RegisterServerRPC("RPC_RetrieveVehicleRequest");
		Expansion_RegisterClientRPC("RPC_Callback");
	}

	protected void CreateDirectoryStructure()
	{
		int instance_id = GetGame().ServerConfigGetInt("instanceId");
		s_GarageFolderPath = "$mission:storage_" + instance_id + "\\expansion\\garage\\";
		if (!FileExist(s_GarageFolderPath))
			ExpansionStatic.MakeDirectoryRecursive(s_GarageFolderPath);
	}

	override void OnMissionStart(Class sender, CF_EventArgs args)
	{
	#ifdef EXPANSIONMODBASEBUILDING
		Class.CastTo(m_TerritoryModule, CF_ModuleCoreManager.Get(ExpansionTerritoryModule));
	#endif
	#ifdef EXPANSIONMODGROUPS
		Class.CastTo(m_PartyModule, CF_ModuleCoreManager.Get(ExpansionPartyModule));
	#endif
	#ifdef EXPANSIONMODMARKET
		Class.CastTo(m_MarketModule, CF_ModuleCoreManager.Get(ExpansionMarketModule));
	#endif

		if (GetGame().IsServer() && GetGame().IsMultiplayer())
			ServerModuleInit();

		if (GetGame().IsClient())
			ClientModuleInit();
	}

	bool IsInGarage(CarScript vehicle)
	{
		if (vehicle.m_Expansion_GlobalID.IsZero())
			return false;

		foreach (ExpansionGarageData garageData: m_GarageData)
		{
			foreach (ExpansionGarageVehicleData vehicleData: garageData.m_Vehicles)
			{
				if (vehicle.m_Expansion_GlobalID.IsEqual(vehicleData.m_GlobalID))
				{
					EXPrint(ToString() + "::IsInGarage - " + vehicle.GetType() + " " + vehicle.GetPosition() + " has identical global ID to " + vehicleData.m_ClassName + " " + vehicleData.m_Position + " stored in garage " + garageData.m_OwnerUID);
					if (vehicle.GetType() == vehicleData.m_ClassName)
						return true;
				}
			}
		}

		return false;
	}

	protected void ServerModuleInit()
	{
		//! Server only
		if (GetGame().IsServer() && GetGame().IsMultiplayer())
		{
			m_GarageData = new array<ref ExpansionGarageData>;
		#ifdef EXPANSIONMODBASEBUILDING
			m_TerritoryParkingMeters = new map<int, ExpansionParkingMeter>;
			m_ParkingMeters = new array<ExpansionParkingMeter>;
		#endif

			CreateDirectoryStructure();
			LoadGarageData();
		}
	}

	protected void ClientModuleInit()
	{
		//! Client only
		if (GetGame().IsClient())
		{
			if (!m_GarageMenuInvoker)
				m_GarageMenuInvoker = new ScriptInvoker(); //! Client

			if (!m_GarageMenuCallbackInvoker)
				m_GarageMenuCallbackInvoker = new ScriptInvoker(); //! Client
		}
	}

	protected void LoadGarageData()
	{
		auto trace = EXTrace.Start(EXTrace.GARAGE, this);

		array<string> garageFiles = ExpansionStatic.FindFilesInLocation(s_GarageFolderPath, ".json");
		foreach (string fileName: garageFiles)
		{
			LoadGarageData(fileName, s_GarageFolderPath);
		}
	}

	protected void LoadGarageData(string fileName, string path)
	{
		ExpansionGarageData garageData = ExpansionGarageData.Load(path + fileName);
		if (garageData)
		{
			Print(ToString() + "::LoadGarageData - Add data from file:" + path + fileName);
			m_GarageData.Insert(garageData);
		}
	}

	array<ref ExpansionGarageData> GetGarageData()
	{
		return m_GarageData;
	}

	//! Client
	void RequestPlayerVehicles()
	{
		auto trace = EXTrace.Start(EXTrace.GARAGE, this);

		auto rpc = Expansion_CreateRPC("RPC_RequestPlayerVehicles");
		rpc.Expansion_Send(true);
	}

	//! Server
	protected void RPC_RequestPlayerVehicles(PlayerIdentity identity, Object target, ParamsReadContext ctx)
	{
		auto trace = EXTrace.Start(EXTrace.GARAGE, this);

		string playerUID = identity.GetId();
		PlayerBase player = PlayerBase.GetPlayerByUID(playerUID);
		if (!player)
		{
			Error(ToString() + "::RPC_RequestPlayerVehicles - Could not get player base!");
			return;
		}

		//! Get all nearby vehicles in the world in search radius.
		bool foundTerritoryForStoring;
		autoptr array<ref ExpansionGarageVehicleData> worldVehicles = GetWorldVehicles(player, foundTerritoryForStoring);

		//! Get all stored territory/personal vehicles dependig on current garage and retrieve mode.
		bool foundTerritoryForRetrieving;
		autoptr array<ref ExpansionGarageVehicleData> storedVehicles = GetStoredVehicles(player, foundTerritoryForRetrieving);

		//! Callback if we found no territory
	#ifdef EXPANSIONMODBASEBUILDING
		auto setting = GetExpansionSettings().GetGarage();
		if (setting.GarageMode == ExpansionGarageMode.Territory && (!foundTerritoryForStoring && !foundTerritoryForRetrieving))
		{
			auto rpcCallBackNoTerritory = Expansion_CreateRPC("RPC_Callback");
			rpcCallBackNoTerritory.Write(ExpansionGarageModuleCallback.NoTerritory);
			rpcCallBackNoTerritory.Expansion_Send(true, identity);
			return;
		}
	#endif

		//! Callback if we found no vehicles
		if ((!storedVehicles || storedVehicles.Count() == 0) && (!worldVehicles || worldVehicles.Count() == 0))
		{
			auto rpcCallBackNoVehicles = Expansion_CreateRPC("RPC_Callback");
			rpcCallBackNoVehicles.Write(ExpansionGarageModuleCallback.NoVehicles);
			rpcCallBackNoVehicles.Expansion_Send(true, identity);
			return;
		}

		auto rpc = Expansion_CreateRPC("RPC_SendPlayerVehicles");
		if (!worldVehicles)
		{
			rpc.Write(0);
		}
		else
		{
			rpc.Write(worldVehicles.Count());
			foreach (ExpansionGarageVehicleData worldData: worldVehicles)
			{
				worldData.OnSend(rpc);
			}
		}

		if (!storedVehicles)
		{
			rpc.Write(0);
		}
		else
		{
			rpc.Write(storedVehicles.Count());
			foreach (ExpansionGarageVehicleData storedData: storedVehicles)
			{
				storedData.OnSend(rpc);
			}
		}

		rpc.Expansion_Send(true, identity);
	}

	//! Client
	protected void RPC_SendPlayerVehicles(PlayerIdentity identity, Object target, ParamsReadContext ctx)
	{
		auto trace = EXTrace.Start(EXTrace.GARAGE, this);

		int i;
		int worldVehiclesCount;
		if (!ctx.Read(worldVehiclesCount))
		{
			Error(ToString() + "::RPC_SendPlayerVehicles - Could not read world vehicles count!");
			return;
		}

		array<ref ExpansionGarageVehicleData> worldVehicles = new array<ref ExpansionGarageVehicleData>;
		for (i = 0; i < worldVehiclesCount; i++)
		{
			ExpansionGarageVehicleData worldData = new ExpansionGarageVehicleData();
			if (!worldData.OnRecieve(ctx))
			{
				Error(ToString() + "::RPC_SendPlayerVehicles - Could not read world vehicle data!");
				return;
			}

			worldVehicles.Insert(worldData);
		}

		int storedVehiclesCount;
		if (!ctx.Read(storedVehiclesCount))
		{
			Error(ToString() + "::RPC_SendPlayerVehicles - Could not read stored vehicles count!");
			return;
		}

		array<ref ExpansionGarageVehicleData> storedVehicles = new array<ref ExpansionGarageVehicleData>;
		for (i = 0; i < storedVehiclesCount; i++)
		{
			ExpansionGarageVehicleData storedData = new ExpansionGarageVehicleData();
			if (!storedData.OnRecieve(ctx))
			{
				Error(ToString() + "::RPC_SendPlayerVehicles - Could not read stored vehicle data!");
				return;
			}

			storedVehicles.Insert(storedData);
		}

		m_GarageMenuInvoker.Invoke(worldVehicles, storedVehicles);
	}

	//! Client
	void DepositVehicleRequest(Object vehicleObj)
	{
		auto trace = EXTrace.Start(EXTrace.GARAGE, this);

		if (!vehicleObj)
			return;

		auto rpc = Expansion_CreateRPC("RPC_DepositVehicleRequest");
		rpc.Expansion_Send(vehicleObj, true);
	}

	//! Server
	protected void RPC_DepositVehicleRequest(PlayerIdentity identity, Object vehicleObj, ParamsReadContext ctx)
	{
		auto trace = EXTrace.Start(EXTrace.GARAGE, this);

		auto settings = GetExpansionSettings().GetGarage();
		if (!settings.Enabled)
			return;

		if (!vehicleObj)
		{
			Error(ToString() + "::RPC_DepositVehicleRequest - Could not get vehicle object!");
			return;
		}

		string playerUID = identity.GetId();
		PlayerBase player = PlayerBase.GetPlayerByUID(playerUID);
		if (!player)
		{
			Error(ToString() + "::RPC_DepositVehicleRequest - Could not get player base!");
			return;
		}

		CarScript vehicle;

		if (!Class.CastTo(vehicle, vehicleObj))
		{
			Error(ToString() + "::RPC_DepositVehicleRequest - Could not get vehicle!");
			return;
		}

		int garageMode = settings.GarageMode;
		int storeMax = settings.MaxStorableVehicles;

	#ifdef EXPANSIONMODGROUPS
		if (settings.EnableGroupFeatures && GetExpansionSettings().GetParty().EnableParties)
			m_PartyDataTemp = player.Expansion_GetParty();
	#endif

	#ifdef EXPANSIONMODBASEBUILDING
		int territoryID;
		string territoryOwnerUID;
		ExpansionParkingMeter parkingMeter;
		if (settings.GarageMode == ExpansionGarageMode.Territory)
		{
			m_TerritoryTemp = GetTerritory(player);
			if (!m_TerritoryTemp)
			{
				CleanupTemp();
				return;
			}

			territoryID = m_TerritoryTemp.GetTerritoryID();
			territoryOwnerUID = m_TerritoryTemp.GetOwnerID();

			if (player.IsInsideOwnTerritory() && m_TerritoryParkingMeters.Find(m_TerritoryTemp.GetTerritoryID(), parkingMeter))
			{
				storeMax = parkingMeter.GetStorageSizeByCircuitBoardType();
			}
		}
		else if (settings.GarageMode == ExpansionGarageMode.Personal)
		{
			parkingMeter = GetParkingMeter(player.GetPosition());
			if (parkingMeter)
			{
				storeMax = parkingMeter.GetStorageSizeByCircuitBoardType();
			}
		}
	#endif

		string ownerUID;
		bool canStore = CanStore(player, vehicle, ownerUID);

		CleanupTemp();

		if (!canStore)
			return;

		Object blockingObject;
		
		vector position = vehicle.GetPosition();
		vector checkPosition = Vector(position[0], position[1] - vehicle.GetModelZeroPointDistanceFromGround(), position[2]);
		if (!ExpansionItemSpawnHelper.IsSpawnPositionFree(checkPosition, vehicle.GetOrientation(), vehicle.GetType(), blockingObject) && blockingObject != vehicleObj)
		{
			string displayName;
			PlayerBase blockingPlayer;
			if (Class.CastTo(blockingPlayer, blockingObject) && blockingPlayer.GetIdentity())
				displayName = blockingPlayer.GetIdentityName();  //! So you can call 'em out in chat - unless it's yourself...
			else
				displayName = blockingObject.GetDisplayName();
			string positionStr = ExpansionStatic.VectorToString(checkPosition, ExpansionVectorToString.Labels);
			auto localiser = new StringLocaliser("STR_EXPANSION_GARAGE_ERROR_BLOCKED", vehicle.GetDisplayName(), displayName, positionStr);
			ExpansionNotification(new StringLocaliser("STR_EXPANSION_GARAGE_ERROR"), localiser, ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_ERROR, 7, ExpansionNotificationType.GARAGE).Create(identity);
			return;
		}

		ExpansionGarageData garageData;
		bool hasData = false;

		if (garageMode == ExpansionGarageMode.Personal)
		{
			garageData = GetGarageDataByOwnerUID(ownerUID);
		}
	#ifdef EXPANSIONMODBASEBUILDING
		else if (garageMode == ExpansionGarageMode.Territory)
		{
			garageData = GetGarageDataByTerritoryID(territoryID);
		}
	#endif
		else
		{
			Error(ToString() + "::RPC_DepositVehicleRequest - Unsupported garage mode " + settings.GarageMode);
			return;
		}

		if (!garageData)
		{
			garageData = new ExpansionGarageData();

			if (garageMode == ExpansionGarageMode.Personal)
			{
				garageData.SetOwnerUID(ownerUID);
			}
		#ifdef EXPANSIONMODBASEBUILDING
			else if (garageMode == ExpansionGarageMode.Territory)
			{
				garageData.SetTerritoryID(territoryID);
				garageData.SetOwnerUID(territoryOwnerUID);
			}
		#endif
		}
		else
		{
			hasData = true;
		}

		//! Check if vehicle is ruined
		if (vehicle.IsRuined())
		{
			ExpansionNotification(new StringLocaliser("STR_EXPANSION_GARAGE_ERROR"), new StringLocaliser("STR_EXPANSION_GARAGE_ERROR_DESTROYED", vehicle.GetDisplayName()), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_ERROR, 7, ExpansionNotificationType.GARAGE).Create(identity);
			return;
		}

		int vehicleCount = garageData.m_Vehicles.Count() + 1;
		if (vehicleCount > storeMax)
		{
			ExpansionNotification(new StringLocaliser("STR_EXPANSION_GARAGE_ERROR"), new StringLocaliser("STR_EXPANSION_GARAGE_ERROR_STORELIMIT", vehicle.GetDisplayName()), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_ERROR, 7, ExpansionNotificationType.GARAGE).Create(identity);
			return;
		}

		//! Check if the position would not be taken by another stored vehicle
		//! XXX: Disabled, as it doesn't really make much sense. Check when retrieving is enough.
		//ExpansionGarageVehicleData collidingVehicleData;
		//if (!IsPositionFree(vehicle, collidingVehicleData))
		//{
			//ExpansionNotification(new StringLocaliser("STR_EXPANSION_GARAGE_ERROR"), new StringLocaliser("STR_EXPANSION_GARAGE_ERROR_FREESPACE", vehicle.GetDisplayName(), ExpansionStatic.GetItemDisplayNameWithType(collidingVehicleData.m_ClassName)), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_ERROR, 7, ExpansionNotificationType.GARAGE).Create(identity);
			//return;
		//}

		//! Check if vehicles has any crew members.
		if (vehicle.Expansion_GetVehicleCrew().Count() > 0)
		{
			ExpansionNotification(new StringLocaliser("STR_EXPANSION_GARAGE_ERROR"), new StringLocaliser("STR_EXPANSION_GARAGE_ERROR_CREW", vehicle.GetDisplayName()), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_ERROR, 7, ExpansionNotificationType.GARAGE).Create(identity);
			return;
		}

		if (settings.NeedKeyToStore && !vehicle.HasKey())
		{
			ExpansionNotification(new StringLocaliser("STR_EXPANSION_GARAGE_ERROR"), new StringLocaliser("STR_EXPANSION_GARAGE_ERROR_KEY", vehicle.GetDisplayName()), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_ERROR, 7, ExpansionNotificationType.GARAGE).Create(identity);
			return;
		}

		//! Check if vehicle has any cargo items that are not attachments if the "CanStoreWithCargo" setting is enabled.
		if (!settings.CanStoreWithCargo)
		{
			if (MiscGameplayFunctions.Expansion_HasAnyCargo(vehicle))
			{
				ExpansionNotification(new StringLocaliser("STR_EXPANSION_GARAGE_ERROR"), new StringLocaliser("STR_EXPANSION_GARAGE_ERROR_CARGO", vehicle.GetDisplayName()), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_ERROR, 7, ExpansionNotificationType.GARAGE).Create(identity);
				return;
			}
		}

		ExpansionGarageVehicleData vehicleData = new ExpansionGarageVehicleData();
		vehicleData.SetDataFromVehicleObject(vehicle);

	#ifdef EXPANSIONMODMARKET
		if (settings.EnableMarketFeatures && GetExpansionSettings().GetMarket().MarketSystemEnabled)
		{
			int price = vehicleData.m_StorePrice;

			array<int> monies = new array<int>;
			int playerWorth = m_MarketModule.GetPlayerWorth(player, monies, NULL, true);
			if (playerWorth < price)
			{
				ExpansionNotification(new StringLocaliser("STR_EXPANSION_GARAGE_ERROR"), new StringLocaliser("STR_EXPANSION_GARAGE_ERROR_PRICE", vehicle.GetDisplayName(), price.ToString(), playerWorth.ToString()), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_ERROR, 7, ExpansionNotificationType.GARAGE).Create(identity);
				return;
			}

			if (!m_MarketModule.RemoveMoney(price, player))
			{
				Error(ToString() + "::RPC_DepositVehicleRequest - Could not remove money from player!");
				return;
			}
		}
	#endif

		//! Store the vehicle and delete it.
		if (!StoreVehicle(vehicleData, vehicle))
		{
			ExpansionNotification(new StringLocaliser("STR_EXPANSION_GARAGE_ERROR"), new StringLocaliser("STR_EXPANSION_GARAGE_ERROR_STORED", vehicle.GetDisplayName()), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_ERROR, 7, ExpansionNotificationType.GARAGE).Create(identity);
			if (GetExpansionSettings().GetLog().Garage)
				GetExpansionSettings().GetLog().PrintLog("[VirtualGarage]::ERROR:: Player \"%1\" (id=%2 pos=%3) tried to store a vehicle \"%4\" (GlobalID=%5 pos=%6) but it failed!", identity.GetName(), identity.GetId(), player.GetPosition().ToString(), vehicleData.m_ClassName, ExpansionStatic.IntToHex(vehicleData.m_GlobalID), vehicleData.m_Position.ToString());
			//! Force menu update on client to update menu listings
			auto callbackRPC = Expansion_CreateRPC("RPC_Callback");
			callbackRPC.Write(ExpansionGarageModuleCallback.Update);
			callbackRPC.Expansion_Send(true, identity);
			return;
		}

		//! Add new vehicle data to the players/territory garage data.
		garageData.AddVehicle(vehicleData);

		garageData.Save();

		if (!hasData)
			m_GarageData.Insert(garageData);

		ExpansionNotification(new StringLocaliser("STR_EXPANSION_GARAGE_INFO"), new StringLocaliser("STR_EXPANSION_GARAGE_SUCCESS_STORE", vehicle.GetDisplayName()), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_SUCCESS, 7, ExpansionNotificationType.GARAGE).Create(identity);
		if (GetExpansionSettings().GetLog().Garage)
			GetExpansionSettings().GetLog().PrintLog("[VirtualGarage] Player \"%1\" (id=%2 pos=%3) stored a vehicle \"%4\" (GlobalID=%5 pos=%6)", identity.GetName(), identity.GetId(), player.GetPosition().ToString(), vehicleData.m_ClassName, ExpansionStatic.IntToHex(vehicleData.m_GlobalID), vehicleData.m_Position.ToString());
		
		auto rpc = Expansion_CreateRPC("RPC_Callback");
		rpc.Write(ExpansionGarageModuleCallback.VehicleStored);
		rpc.Expansion_Send(true, identity);
	}

	bool CanStore(PlayerBase player, CarScript vehicle, out string ownerUID = string.Empty)
	{
		string playerUID = player.GetIdentity().GetId();
		vector playerPos = player.GetPosition();

	#ifdef DIAG
		EXTrace.Print(EXTrace.GARAGE, this, "::CanStore - checking world vehicle " + vehicle.GetType() + " " + vehicle.GetPosition() + " for player " + playerUID + " " + playerPos);
	#endif

		auto settings = GetExpansionSettings().GetGarage();

		if (!vehicle.m_Expansion_HasLifetime && !settings.AllowStoringDEVehicles)
			return false;

		bool isOwnedVehicle;

		if (settings.GarageMode == ExpansionGarageMode.Personal || settings.GarageStoreMode == ExpansionGarageStoreMode.Personal)
		{
			isOwnedVehicle = IsVehicleOwner(vehicle, playerUID);
		#ifdef DIAG
			EXTrace.Print(EXTrace.GARAGE, this, "Is owned vehicle (personal)? " + isOwnedVehicle);
		#endif

			if (isOwnedVehicle)
			{
				ownerUID = playerUID;
			}
		#ifdef EXPANSIONMODGROUPS
			else if (m_PartyDataTemp)
			{
				isOwnedVehicle = IsGroupMemberVehicle(vehicle, m_PartyDataTemp);
			#ifdef DIAG
				EXTrace.Print(EXTrace.GARAGE, this, "Is owned vehicle (group)? " + isOwnedVehicle);
			#endif
				if (isOwnedVehicle)
				{
					ownerUID = m_PartyDataTemp.GetOwnerUID();
					if (settings.GroupStoreMode == ExpansionGarageGroupStoreMode.RetrieveOnly)
					{
					#ifdef DIAG
						EXTrace.Print(EXTrace.GARAGE, this, "Cannot store vehicle: Is group member vehicle, but GroupStoreMode is RetrieveOnly");
					#endif
						return false;
					}
				}
			}
		#endif
		}

	#ifdef EXPANSIONMODBASEBUILDING
		if (settings.GarageMode == ExpansionGarageMode.Territory)
		{
			if (!m_TerritoryTemp)
				return false;

			switch (settings.GarageStoreMode)
			{
				case ExpansionGarageStoreMode.Personal:
					break;
				case ExpansionGarageStoreMode.TerritoryShared:
					isOwnedVehicle = IsTerritoryMemberVehicle(vehicle, m_TerritoryTemp);
				#ifdef DIAG
					EXTrace.Print(EXTrace.GARAGE, this, "Is territory member vehicle? " + isOwnedVehicle);
				#endif
					break;
				case ExpansionGarageStoreMode.TerritoryTyrannical:
					isOwnedVehicle = IsTerritoryMemberVehicle(vehicle, m_TerritoryTemp);
				#ifdef DIAG
					EXTrace.Print(EXTrace.GARAGE, this, "Is territory member vehicle? " + isOwnedVehicle);
				#endif
					if (isOwnedVehicle)
					{
						ExpansionTerritoryMember playerTerritoryData = m_TerritoryTemp.GetMember(playerUID);
						isOwnedVehicle = playerTerritoryData.GetRank() >= ExpansionTerritoryRank.MODERATOR;
					#ifdef DIAG
						EXTrace.Print(EXTrace.GARAGE, this, "Is tyrant? " + isOwnedVehicle);
					#endif
					}
					break;
			}
		}
	#endif

	#ifdef DIAG
		EXTrace.Print(EXTrace.GARAGE, this, typename.EnumToString(ExpansionGarageStoreMode, settings.GarageStoreMode) + " store mode - can store vehicle? " + isOwnedVehicle);
	#endif

		return isOwnedVehicle;
	}

	bool CanRetrieve(PlayerBase player, ExpansionGarageVehicleData vehicleData, bool enemyTerritory)
	{
		string playerUID = player.GetIdentity().GetId();
		vector playerPos = player.GetPosition();

	#ifdef DIAG
		EXTrace.Print(EXTrace.GARAGE, this, "::CanRetrieve - checking stored vehicle " + vehicleData.m_ClassName + " " + vehicleData.m_Position + " for player " + playerUID + " " + playerPos);
	#endif

		auto settings = GetExpansionSettings().GetGarage();
		int vehicleSearchRadius = settings.VehicleSearchRadius;

	#ifdef EXPANSIONMODBASEBUILDING
		ExpansionParkingMeter parkingMeter;
		if (settings.GarageMode == ExpansionGarageMode.Territory)
		{
			if (m_TerritoryTemp && player.IsInsideOwnTerritory() && m_TerritoryParkingMeters.Find(m_TerritoryTemp.GetTerritoryID(), parkingMeter))
			{
				vehicleSearchRadius = parkingMeter.GetRadiusByCircuitBoardType();
			}
		}
		else if (settings.GarageMode == ExpansionGarageMode.Personal)
		{
			parkingMeter = GetParkingMeter(player.GetPosition());
			if (parkingMeter)
			{
				vehicleSearchRadius = parkingMeter.GetRadiusByCircuitBoardType();
			}
		}
	#endif

		float distanceSq = Math.Round(vector.DistanceSq(playerPos, vehicleData.m_Position));
		float maxDist = Math.Max(vehicleSearchRadius, settings.MaxDistanceFromStoredPosition);
		if (distanceSq > maxDist * maxDist)
		{
		#ifdef DIAG
			EXTrace.Print(EXTrace.GARAGE, this, "Cannot retrieve vehicle: Out of range");
		#endif
			return false;
		}

		if (!FileExist(vehicleData.GetEntityStorageFileName()))
			return false;

		//! @note this may only be true if garage mode is set to Territory
		if (enemyTerritory)
			return true;

		bool isOwnedVehicle;

		if (settings.GarageMode == ExpansionGarageMode.Personal || settings.GarageRetrieveMode == ExpansionGarageRetrieveMode.Personal)
		{
			isOwnedVehicle = IsVehicleOwner(vehicleData, playerUID);
		#ifdef DIAG
			EXTrace.Print(EXTrace.GARAGE, this, "Is owned vehicle (personal)? " + isOwnedVehicle);
		#endif

		#ifdef EXPANSIONMODGROUPS
			if (!isOwnedVehicle && m_PartyDataTemp)
			{
				isOwnedVehicle = IsGroupMemberVehicle(vehicleData, m_PartyDataTemp);
			#ifdef DIAG
				EXTrace.Print(EXTrace.GARAGE, this, "Is owned vehicle (group)? " + isOwnedVehicle);
			#endif
				if (isOwnedVehicle && settings.GroupStoreMode == ExpansionGarageGroupStoreMode.StoreOnly)
				{
				#ifdef DIAG
					EXTrace.Print(EXTrace.GARAGE, this, "Cannot retrieve vehicle: Is group member vehicle, but GroupStoreMode is StoreOnly");
				#endif
					return false;
				}
			}
		#endif
		}

	#ifdef EXPANSIONMODBASEBUILDING
		if (settings.GarageMode == ExpansionGarageMode.Territory)
		{
			if (!m_TerritoryTemp)
				return false;

			switch (settings.GarageRetrieveMode)
			{
				case ExpansionGarageRetrieveMode.Personal:
					break;
				case ExpansionGarageRetrieveMode.TerritoryShared:
					isOwnedVehicle = IsTerritoryMemberVehicle(vehicleData, m_TerritoryTemp);
				#ifdef DIAG
					EXTrace.Print(EXTrace.GARAGE, this, "Is territory member vehicle? " + isOwnedVehicle);
				#endif
					break;
				case ExpansionGarageRetrieveMode.TerritoryTyrannical:
					isOwnedVehicle = IsTerritoryMemberVehicle(vehicleData, m_TerritoryTemp);
				#ifdef DIAG
					EXTrace.Print(EXTrace.GARAGE, this, "Is territory member vehicle? " + isOwnedVehicle);
				#endif
					if (isOwnedVehicle)
					{
						ExpansionTerritoryMember playerTerritoryData = m_TerritoryTemp.GetMember(playerUID);
						isOwnedVehicle = playerTerritoryData.GetRank() >= ExpansionTerritoryRank.MODERATOR;
					#ifdef DIAG
						EXTrace.Print(EXTrace.GARAGE, this, "Is tyrant? " + isOwnedVehicle);
					#endif
					}
					break;
			}
		}
	#endif

	#ifdef DIAG
		EXTrace.Print(EXTrace.GARAGE, this, typename.EnumToString(ExpansionGarageRetrieveMode, settings.GarageRetrieveMode) + " retrieve mode - can retrieve vehicle? " + isOwnedVehicle);
	#endif

		return isOwnedVehicle;
	}

	void CleanupTemp()
	{
	#ifdef EXPANSIONMODGROUPS
		m_PartyDataTemp = NULL;
	#endif

	#ifdef EXPANSIONMODBASEBUILDING
		m_TerritoryTemp = NULL;
	#endif
	}

	void RetrieveVehicleRequest(ExpansionGarageVehicleData vehicleData)
	{
		auto trace = EXTrace.Start(EXTrace.GARAGE, this);

		if (!GetGame().IsClient())
		{
			Error(ToString() + "::RetrieveVehicleRequest - Tryed to call RetrieveVehicleRequest on Server!");
			return;
		}

		auto rpc = Expansion_CreateRPC("RPC_RetrieveVehicleRequest");
		rpc.Write(vehicleData.m_GlobalID);
		rpc.Expansion_Send(true);
	}

	//! Server
	protected void RPC_RetrieveVehicleRequest(PlayerIdentity identity, Object target, ParamsReadContext ctx)
	{
		auto trace = EXTrace.Start(EXTrace.GARAGE, this);

		auto settings = GetExpansionSettings().GetGarage();
		if (!settings.Enabled)
			return;

		string playerUID = identity.GetId();
		PlayerBase player = PlayerBase.GetPlayerByUID(playerUID);
		if (!player)
		{
			Error(ToString() + "::RPC_RetrieveVehicleRequest - Could not get player base!");
			return;
		}

		TIntArray globalID;
		if (!ctx.Read(globalID))
		{
			Error(ToString() + "::RPC_RetrieveVehicleRequest - Could not read garage vehicle global ID!");
			return;
		}

		ExpansionGarageVehicleData vehicleData;
		ExpansionGarageData garageData = GetGarageDataByGlobalID(globalID, vehicleData);
		if (!garageData)
		{
			Error(ToString() + "::RPC_RetrieveVehicleRequest - Could not get garage data with vehicle global ID " + globalID + "!");
			return;
		}

	#ifdef EXPANSIONMODGROUPS
		if (settings.EnableGroupFeatures && GetExpansionSettings().GetParty().EnableParties)
			m_PartyDataTemp = player.Expansion_GetParty();
	#endif

		bool enemyTerritory;

	#ifdef EXPANSIONMODBASEBUILDING
		if (settings.GarageMode == ExpansionGarageMode.Territory)
		{
			m_TerritoryTemp = GetTerritory(player, garageData.m_TerritoryID, enemyTerritory);
			if (!m_TerritoryTemp)
			{
				CleanupTemp();
				return;
			}
		}
	#endif

		bool canRetrieve = CanRetrieve(player, vehicleData, enemyTerritory);

		CleanupTemp();

		if (!canRetrieve)
			return;

		Object blockingObject;
		if (!ExpansionItemSpawnHelper.IsSpawnPositionFree(vehicleData.m_Position, vehicleData.m_Orientation, vehicleData.m_ClassName, blockingObject))
		{
			string displayName;
			PlayerBase blockingPlayer;
			if (Class.CastTo(blockingPlayer, blockingObject) && blockingPlayer.GetIdentity())
				displayName = blockingPlayer.GetIdentityName();  //! So you can call 'em out in chat - unless it's yourself...
			else
				displayName = blockingObject.GetDisplayName();
			string positionStr = ExpansionStatic.VectorToString(vehicleData.m_Position, ExpansionVectorToString.Labels);
			auto localiser = new StringLocaliser("STR_EXPANSION_GARAGE_ERROR_BLOCKED", ExpansionStatic.GetItemDisplayNameWithType(vehicleData.m_ClassName), displayName, positionStr);
			ExpansionNotification(new StringLocaliser("STR_EXPANSION_GARAGE_ERROR"), localiser, ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_ERROR, 7, ExpansionNotificationType.GARAGE).Create(identity);
			return;
		}

		if (!LoadVehicle(vehicleData))
		{
			ExpansionNotification(new StringLocaliser("STR_EXPANSION_GARAGE_ERROR"), new StringLocaliser("STR_EXPANSION_GARAGE_ERROR_RETRIEVED", ExpansionStatic.GetItemDisplayNameWithType(vehicleData.m_ClassName)), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_ERROR, 7, ExpansionNotificationType.GARAGE).Create(identity);
			if (GetExpansionSettings().GetLog().Garage)
				GetExpansionSettings().GetLog().PrintLog("[VirtualGarage]::ERROR:: Player \"%1\" (id=%2 pos=%3) tried to retrieve a vehicle \"%4\" (GlobalID=%5 pos=%6) but it failed!", identity.GetName(), identity.GetId(), player.GetPosition().ToString(), vehicleData.m_ClassName, ExpansionStatic.IntToHex(vehicleData.m_GlobalID), vehicleData.m_Position.ToString());
			//! Force menu update on client to update menu listings
			auto callbackTerritoryRPC = Expansion_CreateRPC("RPC_Callback");
			callbackTerritoryRPC.Write(ExpansionGarageModuleCallback.Update);
			callbackTerritoryRPC.Expansion_Send(true, identity);
			return;
		}

		garageData.RemoveVehicle(vehicleData);
		garageData.Save();

		ExpansionNotification(new StringLocaliser("STR_EXPANSION_GARAGE_INFO"), new StringLocaliser("STR_EXPANSION_GARAGE_SUCCESS_RETRIEVE", ExpansionStatic.GetItemDisplayNameWithType(vehicleData.m_ClassName), "X: " + vehicleData.m_Position[0].ToString() + "/Y: " + vehicleData.m_Position[2].ToString()), ExpansionIcons.GetPath("Exclamationmark"), COLOR_EXPANSION_NOTIFICATION_SUCCESS, 7, ExpansionNotificationType.GARAGE).Create(identity);
		if (GetExpansionSettings().GetLog().Garage)
			GetExpansionSettings().GetLog().PrintLog("[VirtualGarage] Player \"%1\" (id=%2 pos=%3) retrieved a vehicle \"%4\" (GlobalID=%5 pos=%6)", identity.GetName(), identity.GetId(), player.GetPosition().ToString(), vehicleData.m_ClassName, ExpansionStatic.IntToHex(vehicleData.m_GlobalID), vehicleData.m_Position.ToString());
		auto rpc = Expansion_CreateRPC("RPC_Callback");
		rpc.Write(ExpansionGarageModuleCallback.VehicleRetrieved);
		rpc.Expansion_Send(true, identity);
	}

	//! Client
	protected void RPC_Callback(PlayerIdentity identity, Object target, ParamsReadContext ctx)
	{
		auto trace = EXTrace.Start(EXTrace.GARAGE, this);

		ExpansionGarageModuleCallback callback;
		if (!ctx.Read(callback))
			return;

		m_GarageMenuCallbackInvoker.Invoke(callback);
	}

	protected bool StoreVehicle(ExpansionGarageVehicleData vehicleData, EntityAI vehicleEntity)
	{
		auto trace = EXTrace.Start(EXTrace.GARAGE, this);

		if (!vehicleEntity)
		{
			Error(ToString() + "::StoreVehicle - Could not get vehicle!");
			return false;
		}

		bool success;

		if (GetExpansionSettings().GetGarage().UseVirtualStorageForCargo)
		{
			success = ExpansionEntityStorageModule.SaveToFile(vehicleEntity, vehicleData.GetEntityStorageFileName());
		}
		else
		{
			vector position = ExpansionStatic.GetSurfacePosition(vehicleEntity.GetPosition()) - "0 1 0";
			ExpansionEntityStoragePlaceholder placeholder;
			success = ExpansionEntityStoragePlaceholder.Expansion_StoreEntityAndReplace(vehicleEntity, "ExpansionGarageVehicle", position, ECE_KEEPHEIGHT, placeholder);
		}

		if (!success)
		{
			Error(ToString() + "::StoreVehicle - Could not store vehicle!");
			return false;
		}

		GetGame().ObjectDelete(vehicleEntity);

		return true;
	}

	protected bool LoadVehicle(ExpansionGarageVehicleData vehicleData, out EntityAI loadedEntity = null, bool setLastDriver = true)
	{
		auto trace = EXTrace.Start(EXTrace.GARAGE, this);

		ExpansionEntityStoragePlaceholder placeholder = ExpansionEntityStoragePlaceholder.GetByStoredEntityGlobalID(vehicleData.m_GlobalID);
		if (!placeholder && !GetExpansionSettings().GetGarage().UseVirtualStorageForCargo)
			EXPrint("WARNING: No placeholder found for vehicle data " + vehicleData.m_ClassName + " " + vehicleData.m_Position);

		if (!ExpansionEntityStorageModule.RestoreFromFile(vehicleData.GetEntityStorageFileName(), loadedEntity, placeholder))
		{
			Error(ToString() + "::LoadVehicle - Could not restore vehicle " + vehicleData.m_ClassName + " from file " + vehicleData.GetEntityStorageFileName());
			return false;
		}

		if (placeholder)
			placeholder.Delete();

		CarScript vehicle;

		if (setLastDriver && Class.CastTo(vehicle, loadedEntity) && vehicle.ExpansionGetLastDriverUID() == string.Empty && vehicle.GetType() == vehicleData.m_ClassName)
		{
			PlayerBase player = GetNearbyVehicleOwner(vehicle.GetPosition(), vehicleData.m_OwnerUIDs);
			if (player)
				vehicle.ExpansionSetLastDriverUID(player);
		}

		return true;
	}

	protected PlayerBase GetNearbyVehicleOwner(vector position, array<string> uids)
	{
		PlayerBase player;
		PlayerBase closestPlayer;
		float smallestDistanceSq = float.MAX;

		foreach (string uid: uids)
		{
			player = PlayerBase.GetPlayerByUID(uid);
			if (player)
			{
				vector pos = player.GetPosition();
				float dist = vector.DistanceSq(pos, position);
				if (dist < smallestDistanceSq)
				{
					smallestDistanceSq = dist;
					closestPlayer = player;
				}
			}
		}

		return closestPlayer;
	}

	protected array<ref ExpansionGarageVehicleData> GetWorldVehicles(PlayerBase player, out bool foundTerritoryForStoring)
	{
		auto trace = EXTrace.Start(EXTrace.GARAGE, this);

		array<ref ExpansionGarageVehicleData> worldVehicles = new array<ref ExpansionGarageVehicleData>;
		vector playerPos = player.GetPosition();

		auto settings = GetExpansionSettings().GetGarage();

		float vehicleSearchRadius = settings.VehicleSearchRadius;

	#ifdef EXPANSIONMODGROUPS
		if (settings.EnableGroupFeatures && GetExpansionSettings().GetParty().EnableParties)
			m_PartyDataTemp = player.Expansion_GetParty();
	#endif

	#ifdef EXPANSIONMODBASEBUILDING
		ExpansionParkingMeter parkingMeter;
		if (settings.GarageMode == ExpansionGarageMode.Territory)
		{
			m_TerritoryTemp = GetTerritory(player);
			if (!m_TerritoryTemp)
			{
				CleanupTemp();
				return NULL;
			}

			foundTerritoryForStoring = true;

			if (player.IsInsideOwnTerritory() && m_TerritoryParkingMeters.Find(m_TerritoryTemp.GetTerritoryID(), parkingMeter))
			{
				vehicleSearchRadius = parkingMeter.GetRadiusByCircuitBoardType();
			}
		}
		else if (settings.GarageMode == ExpansionGarageMode.Personal)
		{
			parkingMeter = GetParkingMeter(player.GetPosition());
			if (parkingMeter)
			{
				vehicleSearchRadius = parkingMeter.GetRadiusByCircuitBoardType();
			}
		}
	#endif

		float vehicleSearchRadiusSq = vehicleSearchRadius * vehicleSearchRadius;

		auto node = CarScript.s_Expansion_AllVehicles.m_Head;
		while (node)
		{
			auto vehicle = node.m_Value;

			node = node.m_Next;

			if (vector.DistanceSq(vehicle.GetPosition(), playerPos) > vehicleSearchRadiusSq)
				continue;

			if (!CanStore(player, vehicle))
				continue;

			ExpansionGarageVehicleData vehicleData = new ExpansionGarageVehicleData();
			vehicleData.SetDataFromVehicleObject(vehicle);

		#ifdef EXPANSIONMODBASEBUILDING
			if (settings.GarageMode == ExpansionGarageMode.Territory)
				vehicleData.SetTerritoryID(m_TerritoryTemp.GetTerritoryID());
		#endif

			worldVehicles.Insert(vehicleData);
		}

		CleanupTemp();

		EXTrace.Add(trace, "World vehicles: " + worldVehicles.Count());

		return worldVehicles;
	}

	//! @brief get territory for storage/retrieval of vehicles.
	//! If in a territory, return territory only if a member (or group has territory members if garage group features enabled)
	//! If not in a territory, return 1st territory that player (or their group, if garage group features are enabled) is a member of (if any)
#ifdef EXPANSIONMODBASEBUILDING
	ExpansionTerritory GetTerritory(PlayerBase player)
	{
		ExpansionTerritory territory;
		string playerUID = player.GetIdentity().GetId();

		//! Search for territory flag if there is one nearby

		TerritoryFlag nearbyFlag = m_TerritoryModule.GetFlagAtPosition3D(player.GetPosition());
		if (nearbyFlag)
		{
			EXTrace.Print(EXTrace.GARAGE, this, "::GetTerritory - player is inside a territory");
			territory = nearbyFlag.GetTerritory();

			if (IsTerritoryMember(player, territory))
				return territory;

		#ifdef DIAG
			EXTrace.Print(EXTrace.GARAGE, this, "::GetTerritory - neither player nor group are members in this territory");
		#endif
		}
		else
		{
			EXTrace.Print(EXTrace.GARAGE, this, "::GetTerritory - player is outside a territory. Trying to find territory with member UID " + playerUID);
			map<int, TerritoryFlag> flags = m_TerritoryModule.GetAllTerritoryFlags();
			foreach (int territoryID, TerritoryFlag flag: flags)
			{
				territory = flag.GetTerritory();

				if (IsTerritoryMember(player, territory))
					return territory;
			}

		#ifdef DIAG
			EXTrace.Print(EXTrace.GARAGE, this, "::GetTerritory - neither player nor group are members in any territory");
		#endif
		}

		return NULL;
	}

	ExpansionTerritory GetTerritory(PlayerBase player, int territoryID, out bool enemyTerritory)
	{
		TerritoryFlag territoryFlag = m_TerritoryModule.GetTerritoryFlag(territoryID);

		if (!territoryFlag || !territoryFlag.HasExpansionTerritoryInformation())
		{
		#ifdef DIAG
			EXTrace.Print(EXTrace.GARAGE, this, "::GetTerritory - territory with ID " + territoryID + " does not exist");
		#endif
			return NULL;
		}

		ExpansionTerritory territory = territoryFlag.GetTerritory();

		if (IsTerritoryMember(player, territory))
			return territory;

		//! If neither player nor group are members of this territory (enemy territory) but player is within territory radius,
		//! they can just retrieve anything in that territory
		if (m_TerritoryModule.IsInTerritory(player.GetPosition(), -1, territoryFlag))
		{
		#ifdef DIAG
			EXTrace.Print(EXTrace.GARAGE, this, "::GetTerritory - neither player nor group are members in this territory, but player position is within territory radius");
		#endif
			enemyTerritory = true;
			return territory;
		}

		return NULL;
	}

	bool IsTerritoryMember(PlayerBase player, ExpansionTerritory territory)
	{
		if (territory.IsMember(player.GetIdentity().GetId()))
		{
		#ifdef DIAG
			EXTrace.Print(EXTrace.GARAGE, this, "::GetTerritory - player is territory member");
		#endif
			return true;
		}

	#ifdef EXPANSIONMODGROUPS
		if (m_PartyDataTemp && IsAnyTerritoryMemberAGroupMember(territory, m_PartyDataTemp))
		{
		#ifdef DIAG
			EXTrace.Print(EXTrace.GARAGE, this, "::GetTerritory - player group has member in this territory");
		#endif
			return true;
		}
	#endif

		return false;
	}
#endif

	protected array<ref ExpansionGarageVehicleData> GetStoredVehicles(PlayerBase player, out bool foundTerritoryForRetrieving)
	{
		auto trace = EXTrace.Start(EXTrace.GARAGE, this);

		array<ref ExpansionGarageVehicleData> storedVehicles = new array<ref ExpansionGarageVehicleData>;

		auto settings = GetExpansionSettings().GetGarage();

	#ifdef EXPANSIONMODGROUPS
		if (settings.EnableGroupFeatures && GetExpansionSettings().GetParty().EnableParties)
			m_PartyDataTemp = player.Expansion_GetParty();
	#endif

		foreach (ExpansionGarageData garageData: m_GarageData)
		{
			bool enemyTerritory = false;

		#ifdef EXPANSIONMODBASEBUILDING
			if (settings.GarageMode == ExpansionGarageMode.Territory)
			{
				m_TerritoryTemp = GetTerritory(player, garageData.m_TerritoryID, enemyTerritory);
				if (!m_TerritoryTemp)
					continue;

				foundTerritoryForRetrieving = true;
			}
		#endif

			foreach (ExpansionGarageVehicleData vehicleData: garageData.m_Vehicles)
			{
				if (!CanRetrieve(player, vehicleData, enemyTerritory))
					continue;

				vehicleData.SetTerritoryID(garageData.m_TerritoryID);

				storedVehicles.Insert(vehicleData);
			}
		}

		CleanupTemp();

		EXTrace.Add(trace, "Stored vehicles: " + storedVehicles.Count());

		return storedVehicles;
	}

	protected ExpansionGarageData GetGarageDataByTerritoryID(int territoryID)
	{
		foreach (ExpansionGarageData current: m_GarageData)
		{
			if (current.m_TerritoryID == territoryID)
				return current;
		}

		return null;
	}

	protected ExpansionGarageData GetGarageDataByVehicleObject(Object vehicleObject)
	{
		foreach (ExpansionGarageData current: m_GarageData)
		{
			foreach (ExpansionGarageVehicleData vehicle: current.m_Vehicles)
			{
				if (vehicle.m_VehicleObject == vehicleObject)
					return current;
			}
		}

		return null;
	}

	protected ExpansionGarageData GetGarageDataByGlobalID(TIntArray globalID, out ExpansionGarageVehicleData vehicleData)
	{
		foreach (ExpansionGarageData current: m_GarageData)
		{
			foreach (ExpansionGarageVehicleData vehicle: current.m_Vehicles)
			{
				if (vehicle.IsGlobalIDValid() && vehicle.IsGlobalIDEqual(globalID))
				{
					vehicleData = vehicle;
					return current;
				}
			}
		}

		return null;
	}

	ScriptInvoker GetGarageMenuSI()
	{
		return m_GarageMenuInvoker;
	}

	ScriptInvoker GetGarageMenuCallbackSI()
	{
		return m_GarageMenuCallbackInvoker;
	}

	//! Check if object would be colliding with any of the stored vehicles
	protected bool IsPositionFree(Object obj, out ExpansionGarageVehicleData collidingVehicleData)
	{
		Object blockingObject;
		foreach (ExpansionGarageData data: m_GarageData)
		{
			foreach (ExpansionGarageVehicleData vehicleData: data.m_Vehicles)
			{
				if (!ExpansionItemSpawnHelper.IsSpawnPositionFree(vehicleData.m_Position, vehicleData.m_Orientation, vehicleData.m_ClassName, blockingObject))
				{
					if (blockingObject == obj)
					{
						collidingVehicleData = vehicleData;
						return false;
					}
				}
			}
		}

		return true;
	}

	array<string> GetVehicleOwnersFromKeys(Object vehicle)
	{
		array<string> owners = new array<string>;
		array<ExpansionCarKey> keys = new array<ExpansionCarKey>;
		ExpansionCarKey.GetKeysForVehicle(vehicle, keys);

		foreach (ExpansionCarKey key: keys)
		{
			PlayerBase player = PlayerBase.Cast(key.GetHierarchyRootPlayer());
			if (player)
			{
				string playerUID = player.GetIdentity().GetId();
				owners.Insert(playerUID);
			}
		}

		return owners;
	}

	bool IsVehicleOwner(CarScript vehicle, string playerUID)
	{
		if (vehicle.ExpansionGetLastDriverUID() == playerUID)
			return true;

		array<string> owners = GetVehicleOwnersFromKeys(vehicle);
		return (owners.Find(playerUID) > -1);
	}

	bool IsVehicleOwner(ExpansionGarageVehicleData vehicleData, string playerUID)
	{
		return (vehicleData.m_OwnerUIDs.Find(playerUID) > -1);
	}

	bool CanOpenMenu()
	{
		if (!GetGame().IsDedicatedServer())
		{
			if (GetGame().GetUIManager().GetMenu())
				return false;

			if (GetDayZGame().GetExpansionGame().GetExpansionUIManager().GetMenu())
				return false;
		}

		return true;
	}

#ifdef EXPANSIONMODBASEBUILDING
	void DropTerritoryVehicles(int territoryID, bool destroy = false)
	{
		auto trace = EXTrace.Start(EXTrace.GARAGE, this);

		for (int i = m_GarageData.Count() - 1; i >= 0; i--)
		{
			ExpansionGarageData currentGarageData = m_GarageData[i];
			if (currentGarageData.m_TerritoryID == territoryID)
			{
				DropVehicles(currentGarageData, destroy);
				m_GarageData.RemoveOrdered(i);
				DeleteFile(currentGarageData.GetFileName());
				break;
			}
		}

	}

	bool IsTerritoryMemberVehicle(CarScript vehicle, ExpansionTerritory territory)
	{
		array<string> owners = GetVehicleOwnersFromKeys(vehicle);
		string lastDriver = vehicle.ExpansionGetLastDriverUID();
		if (lastDriver != string.Empty && owners.Find(lastDriver) == -1)
			owners.Insert(lastDriver);

		foreach (string uid: owners)
		{
			if (territory.IsMember(uid))
				return true;
		}

		 return false;
	}

	bool IsTerritoryMemberVehicle(ExpansionGarageVehicleData vehicleData, ExpansionTerritory territory)
	{
		foreach (string uid: vehicleData.m_OwnerUIDs)
		{
			if (territory.IsMember(uid))
				return true;
		}

		return false;
	}
#endif

#ifdef EXPANSIONMODMARKET
	int GetMarketPrice(string typeName)
	{
		typeName.ToLower();
		ExpansionMarketItem marketItem = ExpansionMarketCategory.GetGlobalItem(typeName);
		if (marketItem)
			return marketItem.CalculatePrice(marketItem.MaxStockThreshold, GetExpansionSettings().GetGarage().StorePricePercent * 0.01);

		return 0;
	}
#endif

	protected ExpansionGarageData GetGarageDataByOwnerUID(string uid)
	{
		foreach (ExpansionGarageData current: m_GarageData)
		{
			if (current.m_OwnerUID == uid)
				return current;
		}

		return null;
	}

	protected void DropPlayerVehicles(string playerUID, bool destroy = false)
	{
		auto trace = EXTrace.Start(EXTrace.GARAGE, this);

		for (int i = m_GarageData.Count() - 1; i >= 0; i--)
		{
			ExpansionGarageData currentGarageData = m_GarageData[i];
			if (currentGarageData.m_OwnerUID == playerUID)
			{
				DropVehicles(currentGarageData, destroy);
				m_GarageData.RemoveOrdered(i);
				DeleteFile(currentGarageData.GetFileName());
				break;
			}
		}

	}
	
	void DropParkingMeterVehicles(vector pos, float maxDistance, bool destroy = false)
	{
		float maxDistanceSq = maxDistance * maxDistance;
		foreach (ExpansionGarageData garageData: m_GarageData)
		{
			bool changed;
			for (int i = garageData.m_Vehicles.Count() - 1; i >= 0; i--)
			{
				ExpansionGarageVehicleData storedData = garageData.m_Vehicles[i];
				float currentDistanceSq = vector.DistanceSq(storedData.m_Position, pos);
				if (storedData.m_Position != vector.Zero && currentDistanceSq <= maxDistanceSq)
				{
					EntityAI loadedEntity;
					if (LoadVehicle(storedData, loadedEntity, false) && destroy)
					{
						for (int j = 0; j < loadedEntity.GetInventory().AttachmentCount(); j++)
						{
							EntityAI attachment = loadedEntity.GetInventory().GetAttachmentFromIndex(j);
							if (attachment)
								attachment.SetHealth(0);
						}
		
						loadedEntity.SetHealth(0);
					}
					
					garageData.m_Vehicles.RemoveOrdered(i);
					changed = true;
				}
			}
			
			if (changed)
				garageData.Save();
		}
	}

	protected void DropVehicles(ExpansionGarageData garageData, bool destroy = false)
	{
		if (garageData)
		{
			foreach (ExpansionGarageVehicleData vehicleData: garageData.m_Vehicles)
			{
				EntityAI loadedEntity;
				if (LoadVehicle(vehicleData, loadedEntity, false) && destroy)
				{
					for (int i = 0; i < loadedEntity.GetInventory().AttachmentCount(); i++)
					{
						EntityAI attachment = loadedEntity.GetInventory().GetAttachmentFromIndex(i);
						if (attachment)
							attachment.SetHealth(0);
					}

					loadedEntity.SetHealth(0);
				}
			}
		}
	}

#ifdef EXPANSIONMODGROUPS
	bool IsGroupMemberVehicle(CarScript vehicle, ExpansionPartyData group)
	{
		array<string> owners = GetVehicleOwnersFromKeys(vehicle);
		string lastDriver = vehicle.ExpansionGetLastDriverUID();
		if (lastDriver != string.Empty && owners.Find(lastDriver) == -1)
			owners.Insert(lastDriver);

		foreach (string uid: owners)
		{
			if (group.IsMember(uid))
				return true;
		}

		return false;
	}

	bool IsGroupMemberVehicle(ExpansionGarageVehicleData vehicleData, ExpansionPartyData group)
	{
		foreach (string uid: vehicleData.m_OwnerUIDs)
		{
			if (group.IsMember(uid))
				return true;
		}

		return false;
	}

	//! Check if any of the of given uids is a group member.
#ifdef EXPANSIONMODBASEBUILDING
	bool IsAnyTerritoryMemberAGroupMember(ExpansionTerritory territory,  ExpansionPartyData group)
	{
		array<ref ExpansionTerritoryMember> members = territory.GetTerritoryMembers();
		foreach (ExpansionTerritoryMember member: members)
		{
			string memberUID = member.GetID();
			if (group.IsMember(memberUID))
				return true;
		}

		return false;
	}
#endif
#endif

#ifdef EXPANSIONMODBASEBUILDING
	void AddTerritoryParkingMeter(ExpansionParkingMeter parkingMeter, int territoryID)
	{
		if (!parkingMeter)
			return;

		m_TerritoryParkingMeters.Insert(territoryID, parkingMeter);
	}

	void RemoveTerritoryParkingMeter(int territoryID)
	{
		if (territoryID <= -1)
			return;

		m_TerritoryParkingMeters.Remove(territoryID);
	}

	bool HasTerritoryParkingMeter(int territoryID)
	{
		ExpansionParkingMeter territoryPM;
		if (m_TerritoryParkingMeters.Find(territoryID, territoryPM))
			return true;

		return false;
	}

	ExpansionParkingMeter GetTerritoryParkingMeter(int territoryID)
	{
		ExpansionParkingMeter territoryPM;
		m_TerritoryParkingMeters.Find(territoryID, territoryPM);
		return territoryPM;
	}
	
	void AddParkingMeter(ExpansionParkingMeter parkingMeter)
	{
		m_ParkingMeters.Insert(parkingMeter);
	}

	void RemoveParkingMeter(ExpansionParkingMeter pmObject)
	{
		foreach (ExpansionParkingMeter pm: m_ParkingMeters)
		{
			if (pm == pmObject)
				m_ParkingMeters.RemoveItem(pm);
		}
	}
	
	ExpansionParkingMeter GetParkingMeter(vector pos)
	{
		foreach (ExpansionParkingMeter pm: m_ParkingMeters)
		{
			float maxDistance = pm.GetRadiusByCircuitBoardType();
			float maxDistanceSq = maxDistance * maxDistance;
			float currentDistanceSq = vector.DistanceSq(pos, pm.GetPosition());
			if (currentDistanceSq <= maxDistanceSq)
				return pm;
		}
		
		return NULL;
	}
#endif
};