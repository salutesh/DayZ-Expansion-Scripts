/**
 * ExpansionRespawnHandlerModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionPlayerState
{
	float m_Health;
	float m_Energy;
	float m_Water;
	float m_HeatComfort;
	float m_Wetness;
	float m_Stamina;
	vector m_Position;

	void ExpansionPlayerState(PlayerBase player)
	{
		AcquireFrom(player);
	}

	void AcquireFrom(PlayerBase player)
	{
		m_Health = player.GetHealth();
		m_Energy = player.GetStatEnergy().Get();
		m_Water = player.GetStatWater().Get();
		m_HeatComfort = player.GetStatHeatComfort().Get();
		m_Wetness = player.GetStatWet().Get();
		m_Stamina = player.GetStatStamina().Get();
		m_Position = player.GetPosition();

		EXPrint(ToString() + "::AcquireFrom " + player.GetIdentity().GetName() + " (id=" + player.GetIdentity().GetId() + ") energy: " + m_Energy + " water: " + m_Water + " heat comfort: " + m_HeatComfort + " wetness: " + m_Wetness + " stamina: " + m_Stamina);
	}

	void ApplyTo(PlayerBase player)
	{
		player.SetHealth(m_Health);
		player.GetStatEnergy().Set(m_Energy);
		player.GetStatWater().Set(m_Water);
		player.GetStatHeatComfort().Set(m_HeatComfort);
		player.GetStatWet().Set(m_Wetness);
		player.GetStatStamina().Set(m_Stamina);

		EXPrint(ToString() + "::ApplyTo " + player.GetIdentity().GetName() + " (id=" + player.GetIdentity().GetId() + ") energy: " + m_Energy + " water: " + m_Water + " heat comfort: " + m_HeatComfort + " wetness: " + m_Wetness + " stamina: " + m_Stamina);
	}
}

class ExpansionRespawnHandlerModule: JMModuleBase
{
	protected string s_FileName;

	ref map<string, ref ExpansionPlayerState> m_PlayerStartStates;

	bool m_SpawnSelected;

	void ExpansionRespawnHandlerModule()
	{
		m_PlayerStartStates = new ref map<string, ref ExpansionPlayerState>;
	}

	// ------------------------------------------------------------
	// ExpansionRespawnHandlerModule IsServer
	// ------------------------------------------------------------	
	override bool IsServer()
	{
		return true;
	}
	
	// ------------------------------------------------------------
	// ExpansionRespawnHandlerModule IsClient
	// ------------------------------------------------------------
	override bool IsClient()
	{
		return true;
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
	#ifdef CF_BUGFIX_REF
	override void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx)
	#else
	override void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ref ParamsReadContext ctx)
	#endif
	{
		switch ( rpc_type )
		{
			case ExpansionRespawnHandlerModuleRPC.ShowSpawnMenu:
			{
				RPC_ShowSpawnMenu(sender, ctx);
				break;
			}
			case ExpansionRespawnHandlerModuleRPC.SelectSpawn:
			{
				RPC_SelectSpawn(sender, ctx);
				break;
			}
			case ExpansionRespawnHandlerModuleRPC.CloseSpawnMenu:
			{
				RPC_CloseSpawnMenu(sender, ctx);
				break;
			}
			case ExpansionRespawnHandlerModuleRPC.RequestPlacePlayerAtTempSafePosition:
			{
				RPC_RequestPlacePlayerAtTempSafePosition(sender, ctx);
				break;
			}
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionRespawnHandlerModule ShowSpawnSelection
	// Called on server
	// ------------------------------------------------------------
	void ShowSpawnSelection(PlayerIdentity sender)
	{
		Error(ToString() + "::ShowSpawnSelection is deprecated, use " + ToString() + "::StartSpawnSelection");
	}

	void StartSpawnSelection(PlayerBase player)
	{
		if ( !IsMissionHost() )
			return;

		PlayerIdentity identity = player.GetIdentity();
		string uid = identity.GetId();

		EXPrint(ToString() + "::StartSpawnSelection - Start -  player " + identity.GetName() + " (id=" + uid + ")");

		array<ref ExpansionSpawnLocation> territoryspawnlist;

		if (GetExpansionSettings().GetSpawn().SpawnOnTerritory)
			territoryspawnlist = GetTerritoryList(identity);

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

		Print("ExpansionRespawnHandlerModule::StartSpawnSelection - End");
	}
	
	void CheckResumeSpawnSelection(PlayerBase player)
	{
		EXPrint(ToString() + "::CheckResumeSpawnSelection - Start");

		PlayerIdentity identity = player.GetIdentity();
		string uid = identity.GetId();

		//! If we have a player state, it means an earlier spawn select wasn't completed by the player
		ExpansionPlayerState state = m_PlayerStartStates.Get(uid);
		if (state)
		{
			StartSpawnSelection(player);
		}
		else
		{
			player.StatRegister(AnalyticsManagerServer.STAT_DISTANCE);
			player.StatRegister(AnalyticsManagerServer.STAT_PLAYTIME);
			player.StatSyncToClient();
		}

		EXPrint(ToString() + "::CheckResumeSpawnSelection - End");
	}

	// ------------------------------------------------------------
	// ExpansionRespawnHandlerModule RPC_ShowSpawnMenu
	// Called on client
	// ------------------------------------------------------------
	private void RPC_ShowSpawnMenu(PlayerIdentity sender, ParamsReadContext ctx)
	{	
		EXPrint("ExpansionRespawnHandlerModule::RPC_ShowSpawnMenu - Start");
		
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
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionRespawnHandlerModule::RPC_ShowSpawnMenu - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionRespawnHandlerModule Exec_ShowSpawnMenu
	// Called on client
	// ------------------------------------------------------------
	private void Exec_ShowSpawnMenu(array<ref ExpansionSpawnLocation> spawnlist, array<ref ExpansionSpawnLocation> territoryspawnlist)
	{
		//! Return if game is not yet ready to show spawn menu

		if ( GetDayZGame().IsLoading() )
			return;

		if ( GetGame().GetUIManager().GetMenu() != NULL )
			return;

		if ( GetDayZGame().GetMissionState() != DayZGame.MISSION_STATE_GAME )
			return;

		if ( !GetGame().GetMission().GetHud() )
			return;

		//! Game is ready to show menu

		EXPrint("ExpansionRespawnHandlerModule::Exec_ShowSpawnMenu - Start");

		ExpansionSpawnSelectionMenu spawnSelectionMenu = ExpansionSpawnSelectionMenu.Cast(GetGame().GetUIManager().EnterScriptedMenu(MENU_EXPANSION_SPAWN_SELECTION_MENU, NULL));

		//! In case spawn select menu could not be created, player will stay at original position and spawn select won't show
		if (!spawnSelectionMenu || !spawnSelectionMenu.IsVisible())
		{
			Error(ToString() + "::Exec_ShowSpawnMenu - Spawn selection menu could not be created or shown!");
			return;
		}

		if (spawnlist)
			spawnSelectionMenu.FillList( spawnlist, 0 );

		if (territoryspawnlist)
			spawnSelectionMenu.FillList( territoryspawnlist, 1 );

		RequestPlacePlayerAtTempSafePosition();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionRespawnHandlerModule::Exec_ShowSpawnMenu - End");
		#endif
	}

	// ------------------------------------------------------------
	// ExpansionSpawnSelectionMenu GetTerritoryList
	// ------------------------------------------------------------	
	array< ref ExpansionSpawnLocation> GetTerritoryList(PlayerIdentity sender)
	{
		if ( !IsMissionHost() )
			return NULL;
			
		ExpansionTerritoryModule territories_module = ExpansionTerritoryModule.Cast(GetModuleManager().GetModule(ExpansionTerritoryModule));
		if (!territories_module)
			return NULL;

		array< ref ExpansionSpawnLocation> SpawnLocations = new array<ref ExpansionSpawnLocation>;
		array<vector> positions = new array<vector>;
		ExpansionSpawnLocation location;
		int TimesIsMember = 0;
	
		for ( int i = 0; i < territories_module.GetAllTerritoryFlags().Count(); ++i )
		{
			TerritoryFlag currentFlag = territories_module.GetAllTerritoryFlags().GetElement(i);
			ExpansionTerritory territory = currentFlag.GetTerritory();
			if (!territory)
				continue;

			if (!territory.IsMember(sender.GetId()))
				continue;

			TimesIsMember++;
			
			vector pos = territory.GetPosition();
			// Offset player slighly horizontally and vertically so we don't spawn them on top of the flag pole
			pos = Vector(pos[0] + 0.5, pos[1], pos[2] + 0.5);

			positions.Insert(pos);
			location = new ExpansionSpawnLocation(territory.GetTerritoryName(), positions);
			SpawnLocations.Insert(location);
			positions.Clear();
		}

		if (TimesIsMember > 0)
			return SpawnLocations;
		else
			return NULL;
	}
	
	// ------------------------------------------------------------
	// ExpansionRespawnHandlerModule SelectSpawn
	// Called from client
	// ------------------------------------------------------------
	void SelectSpawn(vector spawnPoint)
	{
		Print("ExpansionRespawnHandlerModule::SelectSpawn - Start");
		
		m_SpawnSelected = true;

		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(Exec_ShowSpawnMenu);

		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(spawnPoint);
		rpc.Send(null, ExpansionRespawnHandlerModuleRPC.SelectSpawn, true);
		
		Print("ExpansionRespawnHandlerModule::SelectSpawn - End");
	}
	
	void RequestPlacePlayerAtTempSafePosition()
	{
		ScriptRPC rpc = new ScriptRPC();
		rpc.Send(null, ExpansionRespawnHandlerModuleRPC.RequestPlacePlayerAtTempSafePosition, true);
	}

	void RPC_RequestPlacePlayerAtTempSafePosition(PlayerIdentity sender, ParamsReadContext ctx)
	{
		EXPrint(ToString() + "::RPC_RequestPlacePlayerAtTempSafePosition - Start");
	
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
		pos[1] = -1000;
		EXPrint(ToString() + "::RPC_RequestPlacePlayerAtTempSafePosition - player " + player.GetIdentity().GetName() + " (id=" + player.GetIdentity().GetId() + ") spawned at " + player.GetPosition() + ", moving to " + pos);
		player.SetPosition(pos);

		EXPrint(ToString() + "::RPC_RequestPlacePlayerAtTempSafePosition - End");
	}

	// ------------------------------------------------------------
	// ExpansionRespawnHandlerModule RPC_SelectSpawn
	// Called on server
	// ------------------------------------------------------------
	private void RPC_SelectSpawn(PlayerIdentity sender, ParamsReadContext ctx)
	{	
		Print("ExpansionRespawnHandlerModule::RPC_SelectSpawn - Start");
		
		if (!IsMissionHost())
			return;
		
		if (!sender)
			return;
		
		vector spawnPoint;
		if (!ctx.Read(spawnPoint)) //! @note failing to read spawn point should still call exec so proper cleanup is done
			Error(ToString() + "::RPC_SelectSpawn - ERROR: Could not read spawn point!");
		
		Exec_SelectSpawn(sender, spawnPoint);
		
		Print("ExpansionRespawnHandlerModule::RPC_SelectSpawn - End");
	}
	
	// ------------------------------------------------------------
	// ExpansionRespawnHandlerModule Exec_SelectSpawn
	// Called on server
	// ------------------------------------------------------------
	private void Exec_SelectSpawn(PlayerIdentity sender, vector spawnPoint)
	{	
		Print("ExpansionRespawnHandlerModule::Exec_SelectSpawn - Start");
		
		string uid = sender.GetId();
		PlayerBase player = PlayerBase.GetPlayerByUID(uid);
		if (player)
		{
			ExpansionPlayerState state = m_PlayerStartStates.Get(uid);
			if (!state)
			{
				Error(ToString() + "::Exec_SelectSpawn - Player start state not found for player " + player.GetIdentity().GetName() + " (id=" + player.GetIdentity().GetId() + ")!");
				return;
			}

			if (spawnPoint == vector.Zero)
			{
				//! Zero vector means select random spawn
				ExpansionSpawnLocation random_location = GetExpansionSettings().GetSpawn().SpawnLocations.GetRandomElement();
				if (random_location)
				{
					spawnPoint = random_location.Positions.GetRandomElement();
					EXPrint(ToString() + "::Exec_SelectSpawn - selected random spawn point from " + random_location.Name);
				}
				else
				{
					Error(ToString() + "::Exec_SelectSpawn - Could not get random spawn location - spawn list is empty!");
				}
			}

			if (spawnPoint == vector.Zero)
			{
				//! If spawn point is still zero vector, use original spawn position
				//! (this is not necessarily an error, it is valid to include the zero vector among spawn positions in the configuration)
				spawnPoint = state.m_Position;
				EXPrint(ToString() + "::Exec_SelectSpawn - selected original spawn point");
			}

			if (spawnPoint[1] == 0)  //! If Y is zero, use surface Y instead
				spawnPoint[1] = GetGame().SurfaceY(spawnPoint[0], spawnPoint[2]);
			spawnPoint[1] = spawnPoint[1] + 1;  //! Hack fix for inability to open inv after spawn: Let them fall a bit, that'll unlock it
			EXPrint(ToString() + "::Exec_SelectSpawn - moving player " + player.GetIdentity().GetName() + " (id=" + player.GetIdentity().GetId() + ") to " + spawnPoint);
			player.SetPosition(spawnPoint);
			
			EndSpawnSelection(player, state);

			if (GetExpansionSettings().GetLog().SpawnSelection)
				GetExpansionSettings().GetLog().PrintLog("[SpawnSelection] Player \"" + sender.GetName() + "\" (id=" + uid + ")" + " spawned at " + spawnPoint);
		}
		Print("ExpansionRespawnHandlerModule::Exec_SelectSpawn - End");
	}
	
	// ------------------------------------------------------------
	// ExpansionRespawnHandlerModule CloseSpawnMenu
	// Called on server
	// ------------------------------------------------------------
	void CloseSpawnMenu(PlayerIdentity sender)
	{
		Error(ToString() + "::CloseSpawnMenu is deprecated, use " + ToString() + "::EndSpawnSelection");
	}

	void EndSpawnSelection(PlayerBase player, ExpansionPlayerState state)
	{
		Print("ExpansionRespawnHandlerModule::EndSpawnSelection - Start");
		
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

		player.StatRegister(AnalyticsManagerServer.STAT_DISTANCE);
		player.StatRegister(AnalyticsManagerServer.STAT_PLAYTIME);
		player.StatSyncToClient();
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Send(null, ExpansionRespawnHandlerModuleRPC.CloseSpawnMenu, true, identity);
		
		Print("ExpansionRespawnHandlerModule::EndSpawnSelection - End");
	}
	
	void ResetItemWetness(EntityAI parent)
	{
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
		Print("ExpansionRespawnHandlerModule::RPC_CloseSpawnMenu - Start");
		
		if ( !IsMissionClient() )
			return;
		
		//! Use CallLater to make menu disappear *after* player position has updated on client
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(Exec_CloseSpawnMenu, 1000, false);
		
		Print("ExpansionRespawnHandlerModule::RPC_CloseSpawnMenu - End");
	}
	
	// ------------------------------------------------------------
	// ExpansionRespawnHandlerModule Exec_CloseSpawnMenu
	// Called on client
	// ------------------------------------------------------------
	private void Exec_CloseSpawnMenu()
	{
		if ( !IsMissionClient() )
			return;
		
		ExpansionSpawnSelectionMenu spawnSelectionMenu = ExpansionSpawnSelectionMenu.Cast(GetGame().GetUIManager().GetMenu());
		if (spawnSelectionMenu)
		{
			GetGame().GetUIManager().CloseMenu(MENU_EXPANSION_SPAWN_SELECTION_MENU);
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionRespawnHandlerModule SelectRandomSpawn
	// Called on client
	// ------------------------------------------------------------
	void SelectRandomSpawn()
	{
		if ( !IsMissionClient() )
			return;
		
		SelectSpawn(vector.Zero);
	}
	
	// ------------------------------------------------------------
	// ExpansionRespawnHandlerModule SetExpansionStartingGear
	// ------------------------------------------------------------
	void SetExpansionStartingGear(PlayerBase player)
	{
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

	override void OnMissionStart()
	{
		super.OnMissionStart();

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

	override void OnMissionFinish()
	{
		super.OnMissionFinish();

		if (!GetGame().IsDedicatedServer())
			return;

		//! Save all states of players that haven't finished spawn select
		Save();
	}

	void Save()
	{
		FileSerializer file = new FileSerializer;
		if (file.Open(s_FileName, FileMode.WRITE))
		{
			EXPrint(ToString() + "::Save - writing " + m_PlayerStartStates.Count() + " player states");

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

	void Load()
	{
		if (!FileExist(s_FileName))
			return;

		FileSerializer file = new FileSerializer;
		if (file.Open(s_FileName, FileMode.READ))
		{
			//! Can't do it the sane, elegant way, causes server crash when accessing m_PlayerStartStates. Have to do it the awkward, BS way
			//file.Read(m_PlayerStartStates);
			//EXPrint(ToString() + "::Load - read " + m_PlayerStartStates.Count() + " player states");

			int count;
			file.Read(count);

			EXPrint(ToString() + "::Load - reading " + count + " player states");

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
}