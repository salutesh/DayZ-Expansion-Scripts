/**
 * ExpansionRespawnHandlerModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionLastPlayerSpawnLocation
{
	int Index;
	bool IsTerritory;
	
	void ExpansionLastPlayerSpawnLocation(int index, bool isTerritory)
	{
		Index = index;
		IsTerritory = isTerritory;
	}
}

[CF_RegisterModule(ExpansionRespawnHandlerModule)]
class ExpansionRespawnHandlerModule: CF_ModuleWorld
{
	protected string s_FileName;
	ref map<string, ref ExpansionPlayerState> m_PlayerStartStates;
	bool m_SpawnSelected;
	ref array<ref ExpansionRespawnDelayTimer> m_PlayerRespawnDelays;
	ref map<string, ref ExpansionLastPlayerSpawnLocation> m_PlayerLastIndex;

	// ------------------------------------------------------------
	// ExpansionRespawnHandlerModule Constructor
	// ------------------------------------------------------------	
	void ExpansionRespawnHandlerModule()
	{
		m_PlayerStartStates = new map<string, ref ExpansionPlayerState>;
		m_PlayerRespawnDelays = new array<ref ExpansionRespawnDelayTimer>;
		m_PlayerLastIndex = new map<string, ref ExpansionLastPlayerSpawnLocation>;
	}
	
	override void OnInit()
	{
		auto trace = EXTrace.Start(ExpansionTracing.RESPAWN);

		super.OnInit();
		
		EnableInvokeConnect();
		EnableMissionFinish();
		EnableMissionStart();
		EnableRPC();
	}

	// ------------------------------------------------------------
	// ExpansionRespawnHandlerModule GetRPCMin
	// ------------------------------------------------------------	
	override int GetRPCMin()
	{
		return ExpansionRespawnHandlerModuleRPC.INVALID;
	}
	
	// ------------------------------------------------------------
	// ExpansionRespawnHandlerModule GetRPCMax
	// ------------------------------------------------------------	
	override int GetRPCMax()
	{
		return ExpansionRespawnHandlerModuleRPC.COUNT;
	}
	
	// ------------------------------------------------------------
	// ExpansionRespawnHandlerModule OnRPC
	// ------------------------------------------------------------
	override void OnRPC(Class sender, CF_EventArgs args)
	{
		auto trace = EXTrace.Start(ExpansionTracing.RESPAWN);

		super.OnRPC(sender, args);

		auto rpc = CF_EventRPCArgs.Cast(args);

		switch (rpc.ID)
		{
			case ExpansionRespawnHandlerModuleRPC.ShowSpawnMenu:
			{
				RPC_ShowSpawnMenu(rpc.Sender, rpc.Context);
				break;
			}
			case ExpansionRespawnHandlerModuleRPC.SelectSpawn:
			{
				RPC_SelectSpawn(rpc.Sender, rpc.Context);
				break;
			}
			case ExpansionRespawnHandlerModuleRPC.CloseSpawnMenu:
			{
				RPC_CloseSpawnMenu(rpc.Sender, rpc.Context);
				break;
			}
			case ExpansionRespawnHandlerModuleRPC.RequestPlacePlayerAtTempSafePosition:
			{
				RPC_RequestPlacePlayerAtTempSafePosition(rpc.Sender, rpc.Context);
				break;
			}
			case ExpansionRespawnHandlerModuleRPC.CheckPlayerCooldowns:
			{
				RPC_CheckPlayerCooldowns(rpc.Sender, rpc.Context);
				break;
			}
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionRespawnHandlerModule StartSpawnSelection
	// Called on server
	// ------------------------------------------------------------
	void StartSpawnSelection(PlayerBase player, PlayerIdentity identity)
	{
		auto trace = EXTrace.Start(ExpansionTracing.RESPAWN);

		if (!IsMissionHost())
			return;
		
		if (!player)
		{
			Error(ToString() + "::StartSpawnSelection - player is NULL!");
			return;
		}
		
		if (!identity)
		{
			Error(ToString() + "::StartSpawnSelection - player identity is NULL!");
			return;
		}

		string uid = identity.GetId();
			
		array<ref ExpansionSpawnLocation> territoryspawnlist = new array<ref ExpansionSpawnLocation>;
		if (GetExpansionSettings().GetSpawn().SpawnOnTerritory)
			territoryspawnlist = GetTerritoryList(identity.GetId());

		if (!GetExpansionSettings().GetSpawn().SpawnLocations.Count() && (!territoryspawnlist || !territoryspawnlist.Count()))
			return;

		player.m_Expansion_SpawnSelect = true;

		//! Store state (to be restored once spawn selection ends)
		if (!m_PlayerStartStates.Contains(uid))
		{
			m_PlayerStartStates.Insert(uid, new ExpansionPlayerState(player));
			Save();
		}

		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(GetExpansionSettings().GetSpawn().SpawnLocations);
		rpc.Write(territoryspawnlist);
		rpc.Send(null, ExpansionRespawnHandlerModuleRPC.ShowSpawnMenu, true, identity);
	}
	
	// ------------------------------------------------------------
	// ExpansionRespawnHandlerModule CheckResumeSpawnSelection
	// ------------------------------------------------------------
	void CheckResumeSpawnSelection(PlayerBase player, PlayerIdentity identity)
	{
		auto trace = EXTrace.Start(ExpansionTracing.RESPAWN);

		if (!player)
			return;
		
		string uid = identity.GetId();

		//! If we have a player state, it means an earlier spawn select wasn't completed by the player
		ExpansionPlayerState state = m_PlayerStartStates.Get(uid);
		if (state)
		{
			StartSpawnSelection(player, identity);
		}
		else
		{
			//! Related: 0_Preload\3_Game\Analytics\AnalyticsManagerServer.c
			player.StatRegister(AnalyticsManagerServer.STAT_DISTANCE);
			player.StatRegister(AnalyticsManagerServer.STAT_PLAYTIME);
			player.StatSyncToClient();
		}
	}

	// ------------------------------------------------------------
	// ExpansionRespawnHandlerModule RPC_ShowSpawnMenu
	// Called on client
	// ------------------------------------------------------------
	private void RPC_ShowSpawnMenu(PlayerIdentity sender, ParamsReadContext ctx)
	{
		auto trace = EXTrace.Start(ExpansionTracing.RESPAWN);

		if (!IsMissionClient())
			return;

		array<ref ExpansionSpawnLocation> spawnlist;
		if (!ctx.Read(spawnlist))
		{
			Error(ToString() + "::RPC_ShowSpawnMenu - Could not read spawnlist");
		}

		array<ref ExpansionSpawnLocation> territoryspawnlist;
		if (!ctx.Read(territoryspawnlist))
		{
			Error(ToString() + "::RPC_ShowSpawnMenu - Could not read territoryspawnlist");
		}
		
		//! Abort if both lists not set/empty
		if ((!spawnlist || !spawnlist.Count()) && (!territoryspawnlist || !territoryspawnlist.Count()))
		{
			Error(ToString() + "::RPC_ShowSpawnMenu - No spawn locations!");
			return;
		}
		
		//! Call this periodically via call queue (will check if game is ready to show menu, then clean itself up)
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(Exec_ShowSpawnMenu, 1, true, spawnlist, territoryspawnlist);
	}
	
	// ------------------------------------------------------------
	// ExpansionRespawnHandlerModule Exec_ShowSpawnMenu
	// Called on client
	// ------------------------------------------------------------
	private void Exec_ShowSpawnMenu(array<ref ExpansionSpawnLocation> spawnlist, array<ref ExpansionSpawnLocation> territoryspawnlist)
	{
		//! Return if game is not yet ready to show spawn menu

		if (GetDayZGame().IsLoading())
			return;

		if (GetGame().GetUIManager().GetMenu() != NULL)
			return;

		if (GetDayZExpansion().GetExpansionUIManager().GetMenu() != NULL)
			return;

		if (GetDayZGame().GetMissionState() != DayZGame.MISSION_STATE_GAME )
			return;

		if (!GetGame().GetMission().GetHud())
			return;

		//! Game is ready to show menu

		auto trace = EXTrace.Start(ExpansionTracing.RESPAWN);

		ExpansionSpawnSelectionMenu spawnSelectionMenu = ExpansionSpawnSelectionMenu.Cast(GetDayZExpansion().GetExpansionUIManager().CreateSVMenu(EXPANSION_MENU_SPAWNSELECTION));
		//! In case spawn select menu could not be created, player will stay at original position and spawn select won't show
		if (!spawnSelectionMenu || !spawnSelectionMenu.IsVisible())
		{
			Error(ToString() + "::Exec_ShowSpawnMenu - Spawn selection menu could not be created or shown!");
			return;
		}
		
		if (spawnlist)
			spawnSelectionMenu.FillList(spawnlist, 0);

		if (territoryspawnlist)
			spawnSelectionMenu.FillList(territoryspawnlist, 1);
		
		RequestPlacePlayerAtTempSafePosition();
		
	#ifdef EXPANSIONMONITORMODULE
		if (GetExpansionSettings().GetSpawn().CreateDeathMarker)
		{
			ExpansionMonitorModule monitorModule = ExpansionMonitorModule.Cast(CF_ModuleCoreManager.Get(ExpansionMonitorModule));
			if (monitorModule)
				spawnSelectionMenu.CreateDeathMarker(monitorModule.GetLastDeathPosClient());
		}
	#endif
	}

	// ------------------------------------------------------------
	// ExpansionRespawnHandlerModule GetTerritoryList
	// Called on server
	// ------------------------------------------------------------	
	array<ref ExpansionSpawnLocation> GetTerritoryList(string playerUID)
	{
		auto trace = EXTrace.Start(ExpansionTracing.RESPAWN);

		array<ref ExpansionSpawnLocation> SpawnLocations = new array<ref ExpansionSpawnLocation>;

		if (!IsMissionHost())
			return SpawnLocations;
			
#ifdef EXPANSIONMODBASEBUILDING
		ExpansionTerritoryModule territories_module = ExpansionTerritoryModule.Cast(CF_ModuleCoreManager.Get(ExpansionTerritoryModule));
		if (!territories_module)
			return SpawnLocations;

		array<vector> positions = new array<vector>;
		ExpansionSpawnLocation location;
	
		map<int, TerritoryFlag> territoryFlags = territories_module.GetAllTerritoryFlags();
		foreach (int id, TerritoryFlag currentFlag: territoryFlags)
		{
			ExpansionTerritory territory = currentFlag.GetTerritory();
			if (!territory)
				continue;

			if (!territory.IsMember(playerUID))
				continue;
			
			vector pos = territory.GetPosition();
			// Offset player slighly horizontally and vertically so we don't spawn them on top of the flag pole
			pos = Vector(pos[0] + 0.5, pos[1], pos[2] + 0.5);

			positions.Insert(pos);
			location = new ExpansionSpawnLocation(territory.GetTerritoryName(), positions, true);
			if (GetExpansionSettings().GetSpawn().EnableRespawnCooldowns)
				location.SetUseCooldown(true);
			
			SpawnLocations.Insert(location);
			positions.Clear();
		}
		
#endif
		
		return SpawnLocations;
	}
	
	// ------------------------------------------------------------
	// ExpansionRespawnHandlerModule RequestPlacePlayerAtTempSafePosition
	// Called from Client
	// ------------------------------------------------------------	
	void RequestPlacePlayerAtTempSafePosition()
	{
		auto trace = EXTrace.Start(ExpansionTracing.RESPAWN);

		//! FIXME (or not): Moving the player causes desync. Just skip it for now.
		return;

		ScriptRPC rpc = new ScriptRPC();
		rpc.Send(null, ExpansionRespawnHandlerModuleRPC.RequestPlacePlayerAtTempSafePosition, true);
	}
	
	// ------------------------------------------------------------
	// ExpansionRespawnHandlerModule RequestPlacePlayerAtTempSafePosition
	// Called on Server
	// ------------------------------------------------------------	
	void RPC_RequestPlacePlayerAtTempSafePosition(PlayerIdentity sender, ParamsReadContext ctx)
	{
		auto trace = EXTrace.Start(ExpansionTracing.RESPAWN);

		//! FIXME (or not): Moving the player causes desync. Just skip it for now.
		return;

		if (!IsMissionHost())
			return;
		
		if (!sender)
			return;

		PlayerBase player = PlayerBase.GetPlayerByUID(sender.GetId());
		if (!player)
		{
			Error(ToString() + "::RPC_RequestPlacePlayerAtTempSafePosition - could not get player with ID '" + sender.GetId() + "'!");
			return;
		}

		//! Make it less likely to freeze or starve to death while in the cold deep (stats are returned to saved values once spawn select ends)
		player.SetHealth(100);
		player.GetStatEnergy().Set(player.GetStatEnergy().GetMax());
		player.GetStatWater().Set(player.GetStatWater().GetMax());
		player.GetStatHeatComfort().Set(1);

		//! Move player out of harm's way
		vector pos = player.GetPosition();
		pos[1] = -500;
		EXPrint(ToString() + "::RPC_RequestPlacePlayerAtTempSafePosition - player " + player.GetIdentity().GetName() + " (id=" + player.GetIdentity().GetId() + ") spawned at " + player.GetPosition() + ", moving to " + pos);
		player.SetPosition(pos);
	}
	
	// ------------------------------------------------------------
	// ExpansionRespawnHandlerModule SelectSpawn
	// Called from client
	// ------------------------------------------------------------
	void SelectSpawn(int index, int spawnPointIndex, bool isTerritory, bool useCooldown)
	{
		auto trace = EXTrace.Start(ExpansionTracing.RESPAWN);

		m_SpawnSelected = true;
		
		if (ProcessCooldown(GetGame().GetPlayer().GetIdentity(), index, isTerritory, useCooldown))
			return;

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(Exec_ShowSpawnMenu);

		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(index);
		rpc.Write(spawnPointIndex);
		rpc.Send(null, ExpansionRespawnHandlerModuleRPC.SelectSpawn, true);
	}
	
	//! Check existing cooldowns and add if none present.
	//! Return true if point index already had a cooldown (that hasn't expired), false otherwise.
	bool ProcessCooldown(PlayerIdentity sender, int index, bool isTerritory, bool useCooldown)
	{
		auto trace = EXTrace.Start(ExpansionTracing.RESPAWN);

		bool hasCooldown = false;

		if (GetExpansionSettings().GetSpawn().EnableRespawnCooldowns && useCooldown)
		{
			int respawnCooldown = GetExpansionSettings().GetSpawn().GetCooldown(isTerritory);
			string playerUID = sender.GetId();
			foreach (ExpansionRespawnDelayTimer timer: m_PlayerRespawnDelays)
			{
				if (timer.PlayerUID == playerUID && timer.Index == index)
				{
					int cooldownTime = timer.GetTimeDiff();
					int cooldown = respawnCooldown;
					if (GetExpansionSettings().GetSpawn().PunishMultispawn)
					{
						cooldown += timer.GetPunishment();
					}
					if (cooldownTime < cooldown)
					{
						hasCooldown = true;
					}
					break;
				}
			}

			if (!hasCooldown)
			{
				AddCooldown(playerUID, index, isTerritory);
			}
			else
			{
				ExpansionNotification(new StringLocaliser("STR_EXPANSION_SPAWNSELECTION_POINT_LOCKED"), new StringLocaliser("STR_EXPANSION_SPAWNSELECTION_POINT_LOCKED_DESC"), EXPANSION_NOTIFICATION_ICON_INFO, COLOR_EXPANSION_NOTIFICATION_ERROR).Create(sender);
			}
		}

		return hasCooldown;
	}

	// ------------------------------------------------------------
	// ExpansionRespawnHandlerModule RPC_SelectSpawn
	// Called on server
	// ------------------------------------------------------------
	private void RPC_SelectSpawn(PlayerIdentity sender, ParamsReadContext ctx)
	{
		auto trace = EXTrace.Start(ExpansionTracing.RESPAWN);

		if (!IsMissionHost())
			return;
		
		if (!sender)
			return;
		
		int pointIndex;
		if (!ctx.Read(pointIndex))
			Error(ToString() + "::RPC_SelectSpawn - ERROR: Could not read spawn point index!");
		
		int spawnPointIndex;
		if (!ctx.Read(spawnPointIndex)) //! @note failing to read spawn point should still call exec so proper cleanup is done
			Error(ToString() + "::RPC_SelectSpawn - ERROR: Could not read spawn point position!");

		Exec_SelectSpawn(sender, pointIndex, spawnPointIndex);
	}
	
	// ------------------------------------------------------------
	// ExpansionRespawnHandlerModule Exec_SelectSpawn
	// Called on server
	// ------------------------------------------------------------
	private void Exec_SelectSpawn(PlayerIdentity sender, int pointIndex, int spawnPointIndex)
	{
		auto trace = EXTrace.Start(ExpansionTracing.RESPAWN);

		string playerUID = sender.GetId();

		PlayerBase player = PlayerBase.GetPlayerByUID(playerUID);
		if (!player)
			return;
		
		ExpansionPlayerState state = m_PlayerStartStates.Get(playerUID); //! This seems to fail and seems to return NULL on respawns?
		if (!state)
		{
			Error(ToString() + "::Exec_SelectSpawn - Player start state not found for player " + player.GetIdentity().GetName() + " (id=" + player.GetIdentity().GetId() + ")!");
			return;
		}

		array<ref ExpansionSpawnLocation> locations = GetExpansionSettings().GetSpawn().SpawnLocations;
		array<ref ExpansionSpawnLocation> territories = GetTerritoryList(playerUID);

		if (pointIndex < 0 || pointIndex >= locations.Count() + territories.Count())
			return;  //! Invalid location index

		ExpansionSpawnLocation loc;
		
		if (pointIndex < locations.Count())
			loc = locations[pointIndex];
		else
			loc = territories[pointIndex - locations.Count()];
		
		if (ProcessCooldown(sender, pointIndex, loc.IsTerritory, loc.UseCooldown))
			return;

		if (spawnPointIndex < 0 || spawnPointIndex >= loc.Positions.Count())
			return;  //! Invalid position index

		vector spawnPoint = loc.Positions[spawnPointIndex];

		if (spawnPoint == vector.Zero)
		{
			//! If spawn point is still zero vector, use original spawn position
			//! (this is not necessarily an error, it is valid to include the zero vector among spawn positions in the configuration)
			spawnPoint = state.m_Position;
		}

		if (spawnPoint[1] == 0)  //! If Y is zero, use surface Y instead
			spawnPoint[1] = GetGame().SurfaceY(spawnPoint[0], spawnPoint[2]);
		spawnPoint[1] = spawnPoint[1] + 1;  //! Hack fix for inability to open inv after spawn: Let them fall a bit, that'll unlock it
		player.SetPosition(spawnPoint);
					
		EndSpawnSelection(player, state);

		if (GetExpansionSettings().GetLog().SpawnSelection)
			GetExpansionSettings().GetLog().PrintLog("[SpawnSelection] Player \"" + sender.GetName() + "\" (id=" + playerUID + ")" + " spawned at " + spawnPoint);
	}
	
	// ------------------------------------------------------------
	// ExpansionRespawnHandlerModule EndSpawnSelection
	// Called on server
	// ------------------------------------------------------------
	void EndSpawnSelection(PlayerBase player, ExpansionPlayerState state)
	{
		auto trace = EXTrace.Start(ExpansionTracing.RESPAWN);

		if ( !IsMissionHost() )
			return;

		PlayerIdentity identity = player.GetIdentity();
		string uid = identity.GetId();

		//! Make sure clothes and contained items are dry after using spawn select in case we were in the water
		ResetItemWetness(player);

		//! Restore state
		state.ApplyTo(player);
		m_PlayerStartStates.Remove(uid);

		Save();

		//! Remove any sickness player may have gained while in the cold deep
		player.RemoveAllAgents();

		player.m_PlayerOldPos = player.GetPosition();
		player.m_Expansion_SpawnSelect = false;

		//! Related: 0_Preload\3_Game\Analytics\AnalyticsManagerServer.c
		player.StatRegister(AnalyticsManagerServer.STAT_DISTANCE);
		player.StatRegister(AnalyticsManagerServer.STAT_PLAYTIME);
		player.StatSyncToClient();
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Send(null, ExpansionRespawnHandlerModuleRPC.CloseSpawnMenu, true, identity);
	}
	
	void ResetItemWetness(EntityAI parent)
	{
		auto trace = EXTrace.Start(ExpansionTracing.RESPAWN);

		array<EntityAI> items = new array<EntityAI>;
		parent.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items);
		ItemBase item;
		foreach (EntityAI entity: items)
		{
			if (Class.CastTo(item, entity))
				item.SetWet(0);
		}
	}

	// ------------------------------------------------------------
	// ExpansionRespawnHandlerModule RPC_CloseSpawnMenu
	// Called on client
	// ------------------------------------------------------------
	private void RPC_CloseSpawnMenu(PlayerIdentity sender, ParamsReadContext ctx)
	{
		auto trace = EXTrace.Start(ExpansionTracing.RESPAWN);
		
		if ( !IsMissionClient() )
			return;
		
		//! Use CallLater to make menu disappear *after* player position has updated on client
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(Exec_CloseSpawnMenu, 1000, false);
	}
	
	// ------------------------------------------------------------
	// ExpansionRespawnHandlerModule Exec_CloseSpawnMenu
	// Called on client
	// ------------------------------------------------------------
	private void Exec_CloseSpawnMenu()
	{
		auto trace = EXTrace.Start(ExpansionTracing.RESPAWN);

		if ( !IsMissionClient() )
			return;
		
		ExpansionSpawnSelectionMenu spawnSelectionMenu = ExpansionSpawnSelectionMenu.Cast(GetDayZExpansion().GetExpansionUIManager().GetMenu());
		if (spawnSelectionMenu)
		{
			GetDayZExpansion().GetExpansionUIManager().CloseMenu();
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionRespawnHandlerModule SetExpansionStartingGear
	// ------------------------------------------------------------
	void SetExpansionStartingGear(PlayerBase player)
	{
		auto trace = EXTrace.Start(ExpansionTracing.RESPAWN);

		if (!IsMissionHost())
			return;
				
		if (GetExpansionSettings().GetSpawn().StartingGear.EnableStartingGear)
		{
			EntityAI parent = player;
			ExpansionStartingGear gear;
			
			if (Class.CastTo(gear, GetExpansionSettings().GetSpawn().StartingGear))
			{
				//! Add items to clothing (if present) or player
				//! If items don't fit into inventory, they'll be placed in an ExpansionTemporaryOwnedContainer 

				//! Add items to top/shirt/jacket
				if (gear.UseUpperGear)
					AddGear(player, gear.UpperGear, parent, "Body");
				
				//! Add items to pants
				if (gear.UsePantsGear)
					AddGear(player, gear.PantsGear, parent, "Legs");
				
				//! Add items to backpack
				if (gear.UseBackpackGear)
					AddGear(player, gear.BackpackGear, parent, "Back");
				
				//! Add items to vest
				if (gear.UseVestGear)
					AddGear(player, gear.VestGear, parent, "Vest");
				
				//! Add primary weapon and its attachments
				if (gear.UsePrimaryWeapon)
					AddItem(player, gear.PrimaryWeapon, parent);
				
				//! Add primary weapon and its attachments
				if (gear.UseSecondaryWeapon)
					AddItem(player, gear.SecondaryWeapon, parent);
			}
		}
	}

	// ------------------------------------------------------------
	// ExpansionRespawnHandlerModule AddGear
	// ------------------------------------------------------------	
	private void AddGear(PlayerBase player, array< ref ExpansionStartingGearItem > gearItems, inout EntityAI parent, string slotName)
	{
		EntityAI item = player.FindAttachmentBySlotName(slotName);
		foreach (ExpansionStartingGearItem gearItem: gearItems)
		{
			if (item)
			{
				AddItem(player, gearItem, item);
				//! When gear doesn't fit into item, a ExpansionTemporaryOwnedContainer will be created
				//! and item reference will be changed in-place to this container. We need to update `out parent` in this case,
				//! So that other gear that may be added will be placed into that container as well if they don't fit into their
				//! respective player attachment slot item.
				if (item.IsInherited(ExpansionTemporaryOwnedContainer))
					parent = item;
				}
			else
			{
				AddItem(player, gearItem, parent);
			}
		}
	}

	// ------------------------------------------------------------
	// ExpansionRespawnHandlerModule AddItem
	// ------------------------------------------------------------
	private void AddItem(PlayerBase player, ExpansionStartingGearItem gearItem, inout EntityAI parent)
	{
		if (!gearItem.ClassName)
			return;

		int remainingAmount = gearItem.Quantity;  //! SpawnOnParent will deduct spawned amount!
		EntityAI item = EntityAI.Cast(ExpansionItemSpawnHelper.SpawnOnParent(gearItem.ClassName, player, parent, remainingAmount, gearItem.Quantity, gearItem.Attachments, -1, true));

		if (!item)
			return;

		if (GetExpansionSettings().GetSpawn().StartingGear.SetRandomHealth)
			SetRandomHealth(item);

		if (!GetExpansionSettings().GetSpawn().StartingGear.ApplyEnergySources)
			return;

		bool hasEnergySourceSlot;
		//hasEnergySourceSlot = item.GetInventory().HasInventorySlot(InventorySlots.GetSlotIdFromString("BatteryD"));  //! WHY THE FUCK does this not work?!?

		TStringArray attachmentSlotNames = new TStringArray;
		if (GetGame().ConfigIsExisting("CfgVehicles " + gearItem.ClassName + " attachments"))
			GetGame().ConfigGetTextArray("CfgVehicles " + gearItem.ClassName + " attachments", attachmentSlotNames);
		else if (GetGame().ConfigIsExisting("CfgWeapons " + gearItem.ClassName + " attachments"))
			GetGame().ConfigGetTextArray("CfgWeapons " + gearItem.ClassName + " attachments", attachmentSlotNames);

		foreach (string slotName: attachmentSlotNames)
		{
			slotName.ToLower();
			if (slotName == "batteryd")
			{
				hasEnergySourceSlot = true;
				break;
			}
		}

		if (hasEnergySourceSlot)
			item.GetInventory().CreateAttachment("Battery9V");  //! TODO: Should probably be able to deal with arbitrary power sources?
	}

	// ------------------------------------------------------------
	// ExpansionRespawnHandlerModule ExpansionEquipCharacter
	// ------------------------------------------------------------
	void ExpansionEquipCharacter(PlayerBase player)
	{
		auto trace = EXTrace.Start(ExpansionTracing.RESPAWN);

		if ( !IsMissionHost() )
			return;
		
		ExpansionStartingClothing startingClothing;
		if (Class.CastTo(startingClothing, GetExpansionSettings().GetSpawn().StartingClothing))
		{
			AddClothing(player, startingClothing.Headgear, InventorySlots.HEADGEAR);
			AddClothing(player, startingClothing.Glasses, InventorySlots.EYEWEAR);
			AddClothing(player, startingClothing.Masks, InventorySlots.MASK);
			AddClothing(player, startingClothing.Tops, InventorySlots.BODY);
			AddClothing(player, startingClothing.Vests, InventorySlots.VEST);
			AddClothing(player, startingClothing.Gloves, InventorySlots.GLOVES);
			AddClothing(player, startingClothing.Pants, InventorySlots.LEGS);
			AddClothing(player, startingClothing.Belts, InventorySlots.HIPS);
			AddClothing(player, startingClothing.Shoes, InventorySlots.FEET);
			AddClothing(player, startingClothing.Armbands, InventorySlots.ARMBAND);
			AddClothing(player, startingClothing.Backpacks, InventorySlots.BACK);
			
			SetExpansionStartingGear(player);
		}
	}
	
	private void AddClothing(PlayerBase player, TStringArray clothingItems, int slotId)
	{
		EntityAI clothing = player.GetInventory().CreateAttachmentEx(clothingItems.GetRandomElement(), slotId);
		if (clothing && GetExpansionSettings().GetSpawn().StartingClothing.SetRandomHealth)
			SetRandomHealth(clothing);
	}

	// ------------------------------------------------------------
	// ExpansionRespawnHandlerModule SetRandomHealth
	// ------------------------------------------------------------
	void SetRandomHealth(EntityAI itemEnt)
	{
		if (itemEnt)
		{
			int rndHlt = Math.RandomInt(55,100);
			itemEnt.SetHealth("","",rndHlt);
		}
	}

	// ------------------------------------------------------------
	// ExpansionRespawnHandlerModule OnMissionStart
	// ------------------------------------------------------------
	override void OnMissionStart(Class sender, CF_EventArgs args)
	{
		auto trace = EXTrace.Start(ExpansionTracing.RESPAWN);

		super.OnMissionStart(sender, args);

		if (!GetGame().IsDedicatedServer())
			return;

		string filename_old = EXPANSION_FOLDER + "playerstates.bin";

		int instance_id = GetGame().ServerConfigGetInt( "instanceId" );
		string folder = "$mission:storage_" + instance_id + "\\expansion";
		s_FileName = folder + "\\spawnselect.bin";

		if (!FileExist(folder))
			MakeDirectory(folder);

		if (!FileExist(s_FileName) && FileExist(filename_old))
		{
			if (CopyFile(filename_old, s_FileName))
				DeleteFile(filename_old);
		}

		//! Load all states of players that haven't finished spawn select
		Load();
	}

	// ------------------------------------------------------------
	// ExpansionRespawnHandlerModule OnMissionFinish
	// ------------------------------------------------------------
	override void OnMissionFinish(Class sender, CF_EventArgs args)
	{
		auto trace = EXTrace.Start(ExpansionTracing.RESPAWN);

		super.OnMissionFinish(sender, args);

		if (!GetGame().IsDedicatedServer())
			return;

		//! Save all states of players that haven't finished spawn select
		Save();
	}

	// ------------------------------------------------------------
	// ExpansionRespawnHandlerModule Save
	// ------------------------------------------------------------
	void Save()
	{
		auto trace = EXTrace.Start(ExpansionTracing.RESPAWN);

		FileSerializer file = new FileSerializer;
		if (file.Open(s_FileName, FileMode.WRITE))
		{
			//! Can't do it the sane, elegant way, causes server crash when accessing m_PlayerStartStates. Have to do it the awkward, BS way
			//file.Write(m_PlayerStartStates);

			file.Write(m_PlayerStartStates.Count());

			foreach (string uid, ExpansionPlayerState state: m_PlayerStartStates)
			{
				file.Write(uid);
				file.Write(state);
			}

			file.Close();
		}
	}

	// ------------------------------------------------------------
	// ExpansionRespawnHandlerModule Load
	// ------------------------------------------------------------
	void Load()
	{
		auto trace = EXTrace.Start(ExpansionTracing.RESPAWN);

		if (!FileExist(s_FileName))
			return;

		FileSerializer file = new FileSerializer;
		if (file.Open(s_FileName, FileMode.READ))
		{
			//! Can't do it the sane, elegant way, causes server crash when accessing m_PlayerStartStates. Have to do it the awkward, BS way
			//file.Read(m_PlayerStartStates);

			int count;
			file.Read(count);

			string uid;
			ExpansionPlayerState state;
			for (int i = 0; i < count; i++)
			{
				file.Read(uid);
				file.Read(state);
				if (uid && state)
					m_PlayerStartStates.Insert(uid, state);
			}

			file.Close();
		}
	}

	// ------------------------------------------------------------
	// ExpansionRespawnHandlerModule AddCooldown
	// ------------------------------------------------------------	
	void AddCooldown(string playerUID, int index, bool isTerritory)
	{
		auto trace = EXTrace.Start(ExpansionTracing.RESPAWN);

		if (!m_PlayerRespawnDelays)
			m_PlayerRespawnDelays = new array<ref ExpansionRespawnDelayTimer>;
		
		bool hasCooldownEntry = false;
		//! Check if player has a exiting cooldown entry for this spawn point index
		foreach (ExpansionRespawnDelayTimer timer: m_PlayerRespawnDelays)
		{
			if (timer.PlayerUID == playerUID && timer.Index == index)
			{
				if (GetExpansionSettings().GetSpawn().PunishMultispawn)
				{
					//! Check if this is the same spawnpoint the player used before
					ExpansionLastPlayerSpawnLocation lastLocation = m_PlayerLastIndex.Get(playerUID);
					if (lastLocation && lastLocation.Index == index)
					{
						//! Check if the player used this spawn point in the timeframe set in PunishTimeframe spawn settings
						if (timer.GetTimeDiff() < GetExpansionSettings().GetSpawn().PunishTimeframe)
						{
							timer.AddPunishment(GetExpansionSettings().GetSpawn().PunishCooldown);
						}
						else
						{
							if (timer.GetPunishment() > 0)
								timer.SetPunishment(0);
						}
					}
				}	
				
				hasCooldownEntry = true;
				timer.SetTime(); //! Update timestamp for this hasCooldownEntry
				break;
			}
		}
		
		//! Add new cooldown entry
		if (!hasCooldownEntry)
		{
			m_PlayerRespawnDelays.Insert(new ExpansionRespawnDelayTimer(playerUID, index, isTerritory));
		}
		
		//! Save last used spawnpoint index
		ExpansionLastPlayerSpawnLocation last;
		if (m_PlayerLastIndex.Find(playerUID, last))
		{	
			last.Index = index;
			last.IsTerritory = isTerritory;
		}
		else
		{
			last = new ExpansionLastPlayerSpawnLocation(index, isTerritory);
			m_PlayerLastIndex.Insert(playerUID, last);
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionRespawnHandlerModule RespawnCountdownCheck
	// Called on server
	// ------------------------------------------------------------
	void RespawnCountdownCheck(string playerUID)
	{
		auto trace = EXTrace.Start(ExpansionTracing.RESPAWN);

		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
			return;
		
		if (!m_PlayerRespawnDelays || m_PlayerRespawnDelays.Count() == 0)
			return;
		
		PlayerBase player = PlayerBase.GetPlayerByUID(playerUID);
		if (!player)
			return;
		
		array<ref ExpansionRespawnDelayTimer> playerCooldowns = new array<ref ExpansionRespawnDelayTimer>;
		foreach (ExpansionRespawnDelayTimer timer: m_PlayerRespawnDelays)
		{
			if (timer.PlayerUID == playerUID)
			{
				timer.Now = CF_Date.Now(true).GetTimestamp();
				playerCooldowns.Insert(timer);
			}
		}
		
		map<string, ref ExpansionLastPlayerSpawnLocation> lastIndex = new map<string, ref ExpansionLastPlayerSpawnLocation>;
		foreach (string playerID, ExpansionLastPlayerSpawnLocation last: m_PlayerLastIndex)
		{
			if (playerID == playerUID)
				lastIndex.Insert(playerUID, last);
		}
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(playerCooldowns);
		rpc.Write(lastIndex);
		rpc.Send(NULL, ExpansionRespawnHandlerModuleRPC.CheckPlayerCooldowns, true, player.GetIdentity());
	}
	
	// ------------------------------------------------------------
	// ExpansionRespawnHandlerModule RPC_CheckPlayerCooldowns
	// Called on client
	// ------------------------------------------------------------
	private void RPC_CheckPlayerCooldowns(PlayerIdentity sender, ParamsReadContext ctx)
	{
		auto trace = EXTrace.Start(ExpansionTracing.RESPAWN);

		if (!IsMissionClient())
			return;

		array<ref ExpansionRespawnDelayTimer> playerCooldowns = new array<ref ExpansionRespawnDelayTimer>;
		if (!ctx.Read(playerCooldowns))
		{
			Error(ToString() + "::RPC_CheckPlayerCooldowns - Could not read player cooldowns list");
		}
		
		map<string, ref ExpansionLastPlayerSpawnLocation> lastIndex = new map<string, ref ExpansionLastPlayerSpawnLocation>;
		if (!ctx.Read(lastIndex))
		{
			Error(ToString() + "::RPC_CheckPlayerCooldowns - Could not read player last index");
		}
		
		if (!m_PlayerRespawnDelays)
			m_PlayerRespawnDelays = new array<ref ExpansionRespawnDelayTimer>;
			
		int now = CF_Date.Now(true).GetTimestamp();

		m_PlayerRespawnDelays.Clear();
		foreach (auto playerCooldown: playerCooldowns)
		{
			//! Correct for difference between client and server clock
			playerCooldown.Timestamp += now - playerCooldown.Now;

			m_PlayerRespawnDelays.Insert(playerCooldown);
		}
		
		m_PlayerLastIndex.Insert(lastIndex.GetKey(0), lastIndex.GetElement(0));
	}
	
	// ------------------------------------------------------------
	// ExpansionRespawnHandlerModule OnInvokeConnect
	// ------------------------------------------------------------	
	override void OnInvokeConnect(Class sender, CF_EventArgs args)
	{
		auto trace = EXTrace.Start(ExpansionTracing.RESPAWN);

		super.OnInvokeConnect(sender, args);

		auto cArgs = CF_EventPlayerArgs.Cast(args);

		if (!cArgs.Player)
			return;
		
		if (!cArgs.Identity)
			return;
		
		if (GetExpansionSettings().GetSpawn().EnableRespawnCooldowns)
			RespawnCountdownCheck(cArgs.Identity.GetId());
	}
}