/**
 * ExpansionWorldObjectsModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionRemovedObject: OLinkT
{
	string Type;
	vector Position;
	int Flags;
	int EventMask;

	void ExpansionRemovedObject(Object init)
	{
		Type = init.GetType();
		Position = init.GetPosition();
		Flags = init.GetFlags();
		EventMask = init.GetEventMask();
	}
}

[CF_RegisterModule(ExpansionWorldObjectsModule)]
class ExpansionWorldObjectsModule: CF_ModuleWorld
{
	static const string MISSION_OBJECT_FILES_FOLDER = "$mission:expansion\\objects\\";
	static const string MISSION_TRADER_FILES_FOLDER = "$mission:expansion\\traders\\";

	static ref array<Object> s_FirePlacesToDelete = new array<Object>;

	static ref map<Object, ref ExpansionRemovedObject> s_RemovedObjects = new map<Object, ref ExpansionRemovedObject>;
	static bool s_RemovedObjectsReceived;

	override void OnInit()
	{
		auto trace = EXTrace.Start(ExpansionTracing.MAPPING, this);

		super.OnInit();
		
		EnableMissionStart();
		EnableMissionFinish();
		EnableInvokeConnect();
		Expansion_EnableRPCManager();

		Expansion_RegisterClientRPC("RPC_RemoveObjects");
	}

	override void OnMissionStart(Class sender, CF_EventArgs args)
 	{
		auto trace = EXTrace.Start(ExpansionTracing.MAPPING, this);

		super.OnMissionStart(sender, args);

		if (!GetGame().IsServer())
			return;

		bool loadTraderNPCs;

		#ifdef EXPANSIONMODMARKET
		loadTraderNPCs = GetExpansionSettings().GetMarket().MarketSystemEnabled;
		#endif

		FindMissionFiles(true, loadTraderNPCs);
	}

	override void OnMissionFinish(Class sender, CF_EventArgs args)
 	{
		auto trace = EXTrace.Start(ExpansionTracing.MAPPING, this);

		super.OnMissionFinish(sender, args);

		RestoreRemovedObjects();
		DeleteFireplaces();
	}

	static void RestoreRemovedObjects()
	{
		auto trace = EXTrace.Start(ExpansionTracing.MAPPING, ExpansionWorldObjectsModule);

		foreach (Object obj, ExpansionRemovedObject removedObj: s_RemovedObjects)
		{
			if (!obj || !removedObj)
				continue;

			EXTrace.Print(EXTrace.MAPPING, null, "Restoring object " + obj + " at " + obj.GetPosition());
			obj.SetFlags(removedObj.Flags, true);
			obj.SetEventMask(removedObj.EventMask);
			obj.SetScale(1.0);
		}

		s_RemovedObjects.Clear();
	}

	override void OnInvokeConnect(Class sender, CF_EventArgs args)
	{
		auto trace = EXTrace.Start(ExpansionTracing.MAPPING, this);

		super.OnInvokeConnect(sender, args);

//! If Expansion Main is loaded, removed objects are sent as part of general settings RPC to avoid race conditions (building interiors checks if object was removed)
#ifndef EXPANSIONMOD
		if (!GetGame().IsServer())
			return;

		auto cArgs = CF_EventPlayerArgs.Cast(args);

		if (!cArgs.Identity)
			return;

		//! If this is a respawn, need to do nothing of the below
		if (SyncEvents.s_Expansion_RespawningUIDs[cArgs.Identity.GetId()])
			return;

		EXTrace.Add(trace, cArgs.Identity.GetId());

		auto rpc = Expansion_CreateRPC("RPC_RemoveObjects");
		WriteRemovedObjects(rpc);
		rpc.Expansion_Send(true, cArgs.Identity);
#endif
	}

	static void WriteRemovedObjects(ParamsWriteContext ctx)
	{
		auto trace = EXTrace.Start(ExpansionTracing.MAPPING, ExpansionWorldObjectsModule);

		ctx.Write(s_RemovedObjects.Count());
		foreach (Object obj, ExpansionRemovedObject removedObj: s_RemovedObjects)
		{
			ctx.Write(removedObj.Type);
			ctx.Write(removedObj.Position);
		}
	}

	//! Client only!
	static void RPC_RemoveObjects(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
		auto trace = EXTrace.Start(ExpansionTracing.MAPPING, ExpansionWorldObjectsModule);

		int count;
		ctx.Read(count);
		while (count)
		{
			string type;
			if (!ctx.Read(type))
				break;
			vector position;
			if (!ctx.Read(position))
				break;
			RemoveObjects(type, position);
			count--;
		}
	}

	static void DeleteFireplaces()
	{
		Print("Clear up static fireplaces: " + s_FirePlacesToDelete.Count());
		foreach (Object fireplace: s_FirePlacesToDelete)
		{
			GetGame().ObjectDelete(fireplace);
		}
		s_FirePlacesToDelete.Clear();
	}
	
	//! @note No-op, only kept for backwards compat with old init files
	static void FindMissionFiles(string missionFolder, bool loadObjects, bool loadTraders)
	{
		Print("DEPRECATED - calling FindMissionFiles from init.c is no longer needed");
	}

	static void FindMissionFiles(bool loadObjects, bool loadTraders)
	{
		array<string> objectFiles;
		array<string> traderFiles;

		if ( loadObjects && FileExist( MISSION_OBJECT_FILES_FOLDER ) )
		{
			objectFiles = ExpansionStatic.FindFilesInLocation(MISSION_OBJECT_FILES_FOLDER, EXPANSION_MAPPING_EXT);
			if (objectFiles.Count() >= 0)
			{
				LoadMissionObjects(objectFiles);
			}
		}

	#ifdef EXPANSIONMODMARKET
		if ( loadTraders && FileExist( MISSION_TRADER_FILES_FOLDER ) )
		{
			traderFiles = ExpansionStatic.FindFilesInLocation(MISSION_TRADER_FILES_FOLDER, EXPANSION_MAPPING_EXT);
			if (traderFiles.Count() >= 0)
			{

				LoadMissionTraders(traderFiles);
			}
		}
	#endif
	}

	// ------------------------------------------------------------
	// Expansion LoadMissionObjects
	// ------------------------------------------------------------
	static void LoadMissionObjects( array<string> files )
	{
		for ( int i = 0; i < files.Count(); i++ )
		{
			LoadMissionObjectsFile( files[i] );
		}
	}

	// ------------------------------------------------------------
	// Expansion LoadMissionObjectsFile
	// ------------------------------------------------------------
	static void LoadMissionObjectsFile( string name )
	{
		string filePath = MISSION_OBJECT_FILES_FOLDER + name;
		LoadObjectsFile(filePath);

		CF_Log.Debug( "Created all objects from mission object file: " + filePath );
	}

	static void LoadObjects(array<string> files, bool addExtension = false)
	{
		foreach (string filePath: files)
		{
			if (addExtension && !ExpansionString.EndsWithIgnoreCase(filePath, EXPANSION_MAPPING_EXT))
				filePath += EXPANSION_MAPPING_EXT;
			LoadObjectsFile(filePath);
		}
	}

	static void LoadObjectsFile(string filePath, array<Object> createdObjects = NULL)
	{
		auto trace = EXTrace.Start(ExpansionTracing.MAPPING, ExpansionWorldObjectsModule, filePath);

		FileHandle file = OpenFile( filePath, FileMode.READ );
		if ( !file )
			return;

		Object obj;
		string className;
		vector position;
		vector rotation;
		bool special;
		bool takeable;
		bool shouldRemove;
		float radius;
		string attachments;
		
		while ( GetObjectFromFile( file, className, position, rotation, special, takeable, shouldRemove, radius, attachments ) )
		{
			if (shouldRemove)
			{
				CF_Log.Debug( "Attempt to remove object " + className + " at " + position + " from file:" + filePath + ".");

				RemoveObjects(className, position, radius);
			}
			else
			{
				CF_Log.Debug( "Attempt to create object " + className + " at " + position + " from file:" + filePath + ".");

				obj = SpawnObject(className, position, rotation, special, takeable);
				
				if (attachments)
				{
					EntityAI itemWithAttachment = EntityAI.Cast(obj);
					if (itemWithAttachment)
						ProcessGear(itemWithAttachment, attachments);
				}

				if (obj && createdObjects)
					createdObjects.Insert(obj);
			}
		}

		CloseFile( file );

		GetGame().GetWorld().ProcessMarkedObjectsForPathgraphUpdate();
	}

	static Object SpawnObject(string className, vector position, vector rotation, bool special = false, bool takeable = true)
	{
		int flags = ECE_CREATEPHYSICS | ECE_NOLIFETIME | ECE_KEEPHEIGHT;
		#ifndef DAYZ_1_20
		flags |= ECE_DYNAMIC_PERSISTENCY;
		#endif

		Object obj = GetGame().CreateObjectEx( className, position, flags );
		if ( !obj )
			return NULL;

		obj.SetFlags(EntityFlags.STATIC, false);

		obj.SetOrientation( rotation );
		obj.SetAffectPathgraph( true, false );
		obj.Update();

		if ( obj.CanAffectPathgraph() )
		{
			GetGame().GetWorld().MarkObjectForPathgraphUpdate(obj);
		}

		if (!takeable)
		{
			ItemBase item = ItemBase.Cast( obj );
			if (item)
				item.Expansion_SetLootable(false);
		}

		if (special)
			ProcessObject( obj );

		return obj;
	}

	static array<Object> FindObjects(string className, vector position, float radius = 0.1)
	{
		array<Object> objects = {};
		GetGame().GetObjectsAtPosition3D(position, radius, objects, null);

		if (className == "*")
			return objects;

		int doPartialMatch;
		string classNameStart;
		string classNameEnd;

		int index = className.IndexOf("*");

		if (index > -1)
		{
			if (index == 0)
			{
				//! Ends with
				doPartialMatch = 1;
				className = className.Substring(1, className.Length() - 1);
			}
			else if (index == className.Length() - 1)
			{
				//! Starts with
				doPartialMatch = 2;
				className = className.Substring(0, index);
			}
			else
			{
				//! Starts/ends with
				doPartialMatch = 3;
				classNameStart = className.Substring(0, index);
				classNameEnd = className.Substring(index + 1, className.Length() - 1 - index);
			}
		}

		array<Object> filteredOjects = {};
		foreach (Object obj: objects)
		{
			string type;
			g_Game.ObjectGetType(obj, type);
			auto exType = new ExpansionString(type);
			bool match = false;
			switch (doPartialMatch)
			{
				case 0:
					if (type == className)
						match = true;
					break;
				case 1:
					if (exType.EndsWith(className))
						match = true;
					break;
				case 2:
					if (exType.StartsWith(className))
						match = true;
					break;
				case 3:
					if (exType.StartsWith(classNameStart) && exType.EndsWith(classNameEnd))
						match = true;
					break;
			}
			if (match)
				filteredOjects.Insert(obj);
		}

		return filteredOjects;
	}

	static void RemoveObjects(string className, vector position, float radius = 0.1)
	{
		array<Object> objects = FindObjects(className, position, radius);
		if (!objects.Count())
		{
			EXPrint("[ExpansionWorldObjectsModule::RemoveObject] Warning: Object " + className + " not found at " + position);
			return;
		}

		foreach (Object obj: objects)
		{
			RemoveObject(obj);
		}
	}

	static void RemoveObject(Object obj)
	{
		if (!s_RemovedObjects[obj])
		{
			s_RemovedObjects[obj] = new ExpansionRemovedObject(obj);
		}

		EXTrace.Print(EXTrace.MAPPING, null, "Removing object " + obj + " at " + obj.GetPosition());

		EntityFlags flags = obj.GetFlags();
		EntityEvent events = obj.GetEventMask();
		obj.ClearFlags(flags, true);
		obj.ClearEventMask(events);
		obj.SetEventMask(EntityEvent.NOTVISIBLE);
		obj.SetScale(0.0);
		dBodyDestroy(obj); //! Remove collision
	}

	static void ProcessObject(Object obj)
	{
#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.MAPPING, ExpansionWorldObjectsModule);
#endif

		CF_Log.Debug("Try to process mapping object: " + obj.ClassName());

		if (obj.IsInherited(ExpansionPointLight))
		{
			ExpansionPointLight light = ExpansionPointLight.Cast(obj);
			if (light)
			{
				light.SetDiffuseColor(1,0,0);
			}
			
			CF_Log.Debug("Processed mapping object: " + obj.ClassName() + "!");
		}
		else if (obj.IsInherited(FireplaceBase))
		{
			ProcessFireplace(obj);

			CF_Log.Debug("Processed mapping object: " + obj.ClassName() + "!");
		}
		else if (obj.IsInherited(Roadflare))
		{
			Roadflare flare = Roadflare.Cast( obj );
			if (flare) 
			{
				flare.GetCompEM().SetEnergy(999999);
				flare.GetCompEM().SwitchOn();
				flare.SwitchLight(false); //! Flickering
			}

			CF_Log.Debug( "Processed mapping object: " + obj.ClassName() + "!" );
		}
		#ifdef EXPANSIONMODMAPASSETS
		else if (obj.IsInherited(BuildingWithFireplace))
		{
			BuildingWithFireplace buildingWithFireplace;
			bldr_land_misc_barel_fire_1 barel_1;
			bldr_land_misc_barel_fire_2 barel_2;
			bldr_land_misc_barel_fire_3 barel_3;
			bldr_land_misc_barel_fire_4 barel_4;

			if (Class.CastTo(barel_1, obj))
				buildingWithFireplace = barel_1;
			else if (Class.CastTo(barel_2, obj))
				buildingWithFireplace = barel_2;
			else if (Class.CastTo(barel_3, obj))
				buildingWithFireplace = barel_3;
			else if (Class.CastTo(barel_4, obj))
				buildingWithFireplace = barel_4;

			if (buildingWithFireplace) 
			{
				int fire_point_index = 1;
				vector fire_place_pos = buildingWithFireplace.GetSelectionPositionMS(FireplaceIndoor.FIREPOINT_FIRE_POSITION + fire_point_index.ToString());
				vector fire_place_pos_world = buildingWithFireplace.ModelToWorld(fire_place_pos);	
				vector smoke_point_pos = buildingWithFireplace.GetSelectionPositionMS(FireplaceIndoor.FIREPOINT_SMOKE_POSITION + fire_point_index.ToString());
				vector smoke_point_pos_world = buildingWithFireplace.ModelToWorld(smoke_point_pos);		
				vector smokePos = smoke_point_pos_world;

				int flags = ECE_PLACE_ON_SURFACE | ECE_NOLIFETIME;
				#ifndef DAYZ_1_20
				flags |= ECE_NOPERSISTENCY_WORLD;
				#endif
				Object obj_fireplace = GetGame().CreateObjectEx("bldr_fireplace", fire_place_pos_world, flags);
				s_FirePlacesToDelete.Insert(obj_fireplace);
				
				ProcessFireplace(obj_fireplace, false);
			}

			CF_Log.Debug("Processed mapping object: " + obj.ClassName() + "!");
		}
		#endif
		#ifdef NAMALSK_SURVIVAL
		vfx_the_thing theThing;
		if (Class.CastTo(theThing, obj))
		{
			theThing.SetStable(true);
		}
		#endif
	}

	static void ProcessFireplace(Object obj, bool addStones = true)
	{
		FireplaceBase fireplace = FireplaceBase.Cast(obj);
		if (fireplace)
		{
			ItemBase item;

			//! Need to open barrel first, otherwise can't add items
			if (fireplace.IsInherited(BarrelHoles_ColorBase))
				fireplace.Open();
			
			if (fireplace.GetType() == "bldr_fireplace" && addStones)
			{
				//! Add stones
				item = ItemBase.Cast(fireplace.GetInventory().CreateAttachment("Stone"));
				item.SetQuantity(16);
				fireplace.SetStoneCircleState(true);
			}

			//! Add bark
			item = ItemBase.Cast(fireplace.GetInventory().CreateAttachment("Bark_Oak"));
			item.SetQuantity(8);
			//! Add firewood
			item = ItemBase.Cast(fireplace.GetInventory().CreateAttachment("Firewood"));
			item.SetQuantity(6);  //! Can only increase stack over 1 AFTER it has been attached because stack max depends on slot!
			//! Add sticks
			item = ItemBase.Cast(fireplace.GetInventory().CreateAttachment("WoodenStick"));
			item.SetQuantity(10);  //! Can only increase stack over 5 AFTER it has been attached because stack max depends on slot!

			fireplace.StartFire();
			fireplace.RefreshFireplaceVisuals();
		}
	}

	static void ProcessGear(EntityAI entity, string gear)
	{
		TStringArray tokens();
		gear.Split(",", tokens);

		foreach (string token: tokens)
		{
			DayZPlayerImplement ai;
			BuildingBase building;
			ZombieBase zombie;

#ifdef EXPANSIONMODAI
			if (token.IndexOf("faction:") == 0)
			{
				if (Class.CastTo(ai, entity))
				{
					string factionName = token.Substring(8, token.Length() - 8);
					eAIFaction faction = eAIFaction.Create(factionName);
					if (faction)
					{
						EXTrace.Print(EXTrace.AI, ai, "Setting faction " + faction.ToString());
						eAIGroup group = eAIGroup.GetGroupByLeader(ai, true, faction);
						if (group.GetFaction().Type() != faction.Type())
							group.SetFaction(faction);
						ai.eAI_SetPassive(false);
					}
				}
				continue;
			}
#endif

			if (token == "canbelooted:false" || token == "canbelooted:0")
			{
				if (Class.CastTo(ai, entity))
				{
					ai.Expansion_SetCanBeLooted(false);
				}
				continue;
			}

			if (token.IndexOf("loadout:") == 0)
			{
				string loadout = token.Substring(8, token.Length() - 8);
				ExpansionHumanLoadout.Apply(entity, loadout);
				continue;
			}

			if (token.IndexOf("name:") == 0)
			{
				string name = token.Substring(5, token.Length() - 5);
				if (Class.CastTo(ai, entity) && ai.m_Expansion_NetsyncData)
				{
					ai.m_Expansion_NetsyncData.Set(0, name);
				}
				else if (Class.CastTo(building, entity) && building.m_Expansion_NetsyncData)
				{
					building.m_Expansion_NetsyncData.Set(0, name);
				}
				else if (Class.CastTo(zombie, entity) && zombie.m_Expansion_NetsyncData)
				{
					zombie.m_Expansion_NetsyncData.Set(0, name);
				}
				continue;
			}

			array<string> items = new array<string>;
			token.Split("+", items);
			EntityAI itemEnt = NULL;
			//! Spawn weapon in hands
			if ( entity.IsInherited( Man ) && GetGame().ConfigIsExisting( "CfgVehicles " + items[0] + " suicideAnim" ) || GetGame().IsKindOf( items[0], "Rifle_Base" ) || GetGame().IsKindOf( items[0], "Pistol_Base" ) || GetGame().IsKindOf( items[0], "Archery_Base" ) || GetGame().IsKindOf( items[0], "Launcher_Base" ) )
				itemEnt = Man.Cast( entity ).GetHumanInventory().CreateInHands( items[0] );
			//! Spawn everything else in inventory
			if ( !itemEnt )
				itemEnt = ExpansionItemSpawnHelper.CreateInInventoryEx(entity, items[0] );
			ItemBase itemBase = ItemBase.Cast( itemEnt );
			if ( itemEnt )
			{
				for ( int j = 1; j < items.Count(); j++ )
				{
					if ( itemBase )
						itemBase.ExpansionCreateInInventory( items[j] );
					else
						itemEnt.GetInventory().CreateInInventory( items[j] );
				}
			}
		}
	}

	// ------------------------------------------------------------
	// Expansion GetObjectFromFile
	// ------------------------------------------------------------
	static bool GetObjectFromFile( FileHandle file, out string name, out vector position, out vector rotation, out bool special = false, out bool takeable = true, out bool shouldRemove = false, out float radius = 0.1, out string attachments = "" )
	{
		string line;
		int lineSize = FGets( file, line );

		if ( lineSize < 1 )
			return false;
		
		TStringArray tokens = new TStringArray;
		line.Split( "|", tokens );

		name = tokens.Get( 0 );
		position = tokens.Get( 1 ).ToVector();
		shouldRemove = name[0] == "-";
		if (shouldRemove)
		{
			name = name.Substring(1, name.Length() - 1);
			radius = tokens.Get( 2 ).ToFloat();
			if (!radius)
				radius = 0.1;
		}
		else
		{
			rotation = tokens.Get( 2 ).ToVector();
			special = tokens.Get( 3 ) == "true";
			string token = tokens[4];
			takeable = token != "false";
			if (token == "false" || token == "true")
				token = token[5];
			if (token)
				attachments = token;
			else
				attachments = "";
		}
		
		return true;
	}

	#ifdef EXPANSIONMODMARKET
	// ------------------------------------------------------------
	// Expansion LoadMissionTraders
	// ------------------------------------------------------------
	static void LoadMissionTraders( array<string> files )
	{
		for ( int i = 0; i < files.Count(); i++ )
		{
			LoadMissionTradersFile( files[i] );
		}
	}

	// ------------------------------------------------------------
	// Expansion LoadMissionTradersFile
	// ------------------------------------------------------------
	static void LoadMissionTradersFile( string name )
	{
#ifdef EXPANSIONTRACE
		auto trace = EXTrace.Start(ExpansionTracing.MAPPING, ExpansionWorldObjectsModule);
#endif

		CF_Log.Debug( "Attempting to load mission trader file: " + name );

		string filePath = MISSION_TRADER_FILES_FOLDER + name;
		LoadTradersFile(filePath);

		CF_Log.Debug( "Created all traders from mission trader file: " + filePath );
	}

	static void LoadTradersFile( string filePath )
	{
		auto trace = EXTrace.Start(ExpansionTracing.MAPPING, ExpansionWorldObjectsModule, filePath);

		FileHandle file = OpenFile( filePath, FileMode.READ );
		if ( !file )
			return;

		Object obj;
		EntityAI trader;
		string traderName;
		string className;
		string fileName;
		array<vector> positions;
		vector position;
		vector rotation;
		string gear;
		
		int i, j;

		while ( GetTraderFromMissionFile( file, traderName, positions, rotation, gear ) )
		{
			array<string> parts = new array<string>;
			traderName.Split(".", parts);
			className = parts[0];
			fileName = parts[1];
			position = positions[0];

			CF_Log.Debug( "Attempt to create mission trader " + className + " at " + position + " from file:" + filePath + ".");

			obj = GetGame().CreateObject( className, position, false, GetGame().IsKindOf(className, "DZ_LightAI"), true );
			if (!obj)
				continue;

			obj.SetOrientation( rotation );
			obj.SetAffectPathgraph( true, false );
			obj.Update();

			if ( obj.CanAffectPathgraph() )
			{
				GetGame().GetWorld().MarkObjectForPathgraphUpdate(obj);
			}

			trader = EntityAI.Cast( obj );
			if ( trader )
			{
				if ( gear )
				{
					ProcessGear(trader, gear);
				}

				ExpansionTraderNPCBase traderNPC;
				ExpansionTraderStaticBase traderStatic;
				ExpansionTraderZombieBase traderZombie;
				#ifdef ENFUSION_AI_PROJECT
				ExpansionTraderAIBase traderAI;
				#endif
				if ( Class.CastTo( traderNPC, obj ) )
					traderNPC.LoadTrader(fileName);
				else if ( Class.CastTo( traderStatic, obj ) )
					traderStatic.LoadTrader(fileName);
				else if ( Class.CastTo( traderZombie, obj ) )
					traderZombie.LoadTrader(fileName);
				#ifdef ENFUSION_AI_PROJECT
				else if ( Class.CastTo( traderAI, obj ) )
					traderAI.LoadTrader(fileName);
				#endif
				EXPrint("LoadMissionTradersFile trader " + trader + " fileName " + fileName);
			
				#ifdef EXPANSIONMODAI
				if ( traderAI )
				{
					eAIGroup ownerGrp = traderAI.GetGroup();
					for ( j = 0; j < positions.Count(); j++ )
					{
						EXPrint("Adding waypoint " + positions[j]);
						ownerGrp.AddWaypoint( positions[j] );
					}
			
					ownerGrp.SetWaypointBehaviour(eAIWaypointBehavior.ALTERNATE);
				}
				#endif

				CF_Log.Debug( "  Created" );
			}
		}

		CloseFile( file );

		GetGame().GetWorld().ProcessMarkedObjectsForPathgraphUpdate();
	}

	// ------------------------------------------------------------
	// Expansion GetTraderFromMissionFile
	// ------------------------------------------------------------
	static bool GetTraderFromMissionFile( FileHandle file, out string name, out array<vector> positions, out vector rotation, out string gear )
	{
		string line;
		int lineSize = FGets( file, line );

		if ( lineSize < 1 )
			return false;
		
		TStringArray tokens = new TStringArray;
		line.Split( "|", tokens );

		name = tokens.Get( 0 );
		TStringArray positionTokens = new TStringArray;
		tokens.Get( 1 ).Split( ",", positionTokens );
		positions = new array<vector>;
		vector coordinate;
		foreach (string positionToken : positionTokens)
		{
			TStringArray coordinateTokens = new TStringArray;
			positionToken.Split( " ", coordinateTokens );
			if ( coordinateTokens.Count() == 2 )
			{
				coordinate = Vector( 0, 0, 0 );
				coordinate[0] = coordinateTokens[0].ToFloat();
				coordinate[2] = coordinateTokens[1].ToFloat();
				coordinate[1] = GetGame().SurfaceY( coordinate[0], coordinate[2] );
			}
			else
			{
				coordinate = positionToken.ToVector();
			}
			positions.Insert( coordinate );
		}
		rotation = tokens.Get( 2 ).ToVector();

		gear = tokens.Get( 3 );
		
		return true;
	}
	#endif
}

typedef ExpansionWorldObjectsModule ExpansionObjectSpawnTools;
