/**
 * ExpansionObjectSpawnTools.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionObjectSpawnTools
{
	static ref array<EntityAI> firePlacesToDelete = new array<EntityAI>;
	static string objectFilesFolder;
	static string traderFilesFolder;
		
	void ~ExpansionObjectSpawnTools()
	{
		Print("Clear up static fireplaces: " + firePlacesToDelete.Count());
		foreach (Entity fireplace: firePlacesToDelete)
		{
			GetGame().ObjectDelete(fireplace);
		}
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

		objectFilesFolder = "$mission:expansion\\objects\\";
		traderFilesFolder = "$mission:expansion\\traders\\";

		if ( loadObjects && FileExist( objectFilesFolder ) )
		{
			objectFiles = ExpansionStatic.FindFilesInLocation(objectFilesFolder);
			if (objectFiles.Count() >= 0)
			{
				LoadMissionObjects(objectFiles);
			}
		}

	#ifdef EXPANSIONMODMARKET
		if ( loadTraders && FileExist( traderFilesFolder ) )
		{
			traderFiles = ExpansionStatic.FindFilesInLocation(traderFilesFolder);
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
	// Expansion FixObjectCollision
	// ------------------------------------------------------------
	static void FixObjectCollision( Object obj )
	{
		vector roll = obj.GetOrientation();
		roll[2] = roll[2] - 1;
		obj.SetOrientation( roll );
		roll[2] = roll[2] + 1;
		obj.SetOrientation( roll );
	}

	// ------------------------------------------------------------
	// Expansion LoadMissionObjectsFile
	// ------------------------------------------------------------
	static void LoadMissionObjectsFile( string name )
	{
		auto trace = EXTrace.Start(ExpansionTracing.MAPPING, null, objectFilesFolder + name);

		Object obj;
		string className;
		vector position;
		vector rotation;
		string special;

		string filePath = objectFilesFolder + name;
		FileHandle file = OpenFile( filePath, FileMode.READ );

		if ( !file )
			return;
		
		while ( GetObjectFromMissionFile( file, className, position, rotation, special ) )
		{
			CF_Log.Debug( "Attempt to create mission object " + className + " at " + position + " from file:" + filePath + ".");

			int flags = ECE_CREATEPHYSICS;

			obj = GetGame().CreateObjectEx( className, position, flags );
				if ( !obj )
					continue;

			obj.SetFlags(EntityFlags.STATIC, false);

			obj.SetPosition( position );
			obj.SetOrientation( rotation );

			FixObjectCollision( obj );

			if ( obj.CanAffectPathgraph() )
			{
				obj.SetAffectPathgraph( true, false );
				GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( GetGame().UpdatePathgraphRegionByObject, 100, false, obj );
			}

			EntityAI entityAI = EntityAI.Cast( obj );
			if ( entityAI )
			{
				if (IsMissionHost()) entityAI.SetLifetime(1.0);
			}

			if ( special == "true")
				ProcessMissionObject( obj );
		}

		CloseFile( file );

		CF_Log.Debug( "Created all objects from mission object file: " + filePath );
	}

	// ------------------------------------------------------------
	// Expansion ProcessMissionObject
	// ------------------------------------------------------------
	static void ProcessMissionObject(Object obj)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.MAPPING, "ExpansionObjectSpawnTools", "ProcessMissionObject");
#endif

		CF_Log.Debug("Try to process mapping object: " + obj.ClassName());

		ItemBase item;

		if (obj.IsInherited(ExpansionPointLight))
		{
			ExpansionPointLight light = ExpansionPointLight.Cast(obj);
			if (light)
			{
				light.SetDiffuseColor(1,0,0);
			}
			
			CF_Log.Debug("Processed mapping object: " + obj.ClassName() + "!");
		}
		else if (obj.IsKindOf("Fireplace"))
		{
			Fireplace fireplace = Fireplace.Cast(obj);
			if (fireplace)
			{
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
			}

			CF_Log.Debug("Processed mapping object: " + obj.ClassName() + "!");
		}
		else if (obj.IsInherited(BarrelHoles_ColorBase))
		{
			BarrelHoles_Red barrel = BarrelHoles_Red.Cast(obj);
			if (barrel) 
			{
				//! Need to open barrel first, otherwise can't add items
				barrel.Open();

				//! Add bark
				item = ItemBase.Cast(barrel.GetInventory().CreateAttachment("Bark_Oak"));
				item.SetQuantity(8);
				//! Add firewood
				item = ItemBase.Cast(barrel.GetInventory().CreateAttachment("Firewood"));
				item.SetQuantity(6);  //! Can only increase stack over 1 AFTER it has been attached because stack max depends on slot!
				//! Add sticks
				item = ItemBase.Cast(barrel.GetInventory().CreateAttachment("WoodenStick"));
				item.SetQuantity(10);  //! Can only increase stack over 5 AFTER it has been attached because stack max depends on slot!

				barrel.StartFire();
			}

			CF_Log.Debug("Processed mapping object: " + obj.ClassName() + "!");
		}
		else if (obj.IsKindOf("Roadflare"))
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
				
				Object obj_fireplace = GetGame().CreateObjectEx("FireplaceIndoor", fire_place_pos_world, ECE_PLACE_ON_SURFACE|ECE_NOLIFETIME);
				firePlacesToDelete.Insert(EntityAI.Cast(obj_fireplace));
				
				FireplaceIndoor fp_indoor = FireplaceIndoor.Cast(obj_fireplace);
				if (fp_indoor)
				{
					fp_indoor.SetFirePointIndex(fire_point_index);
					fp_indoor.SetSmokePointPosition(smokePos);
					fp_indoor.SetOrientation("0 0 0");

					//! Add bark
					item = ItemBase.Cast(fp_indoor.GetInventory().CreateAttachment("Bark_Oak"));
					item.SetQuantity(8);
					//! Add firewood
					item = ItemBase.Cast(fp_indoor.GetInventory().CreateAttachment("Firewood"));
					item.SetQuantity(6);  //! Can only increase stack over 1 AFTER it has been attached because stack max depends on slot!
					//! Add sticks
					item = ItemBase.Cast(fp_indoor.GetInventory().CreateAttachment("WoodenStick"));
					item.SetQuantity(10);  //! Can only increase stack over 5 AFTER it has been attached because stack max depends on slot!
					
					fp_indoor.StartFire();
				}
			}

			CF_Log.Debug("Processed mapping object: " + obj.ClassName() + "!");
		}
		#endif
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

#ifdef ENFUSION_AI_PROJECT
			if (token.IndexOf("faction:") == 0)
			{
				string factionName = token.Substring(8, token.Length() - 8);
				eAIFaction faction = eAIFaction.Create(factionName);
				if (faction)
				{
					if (Class.CastTo(ai, entity))
					{
						EXTrace.Print(EXTrace.AI, ai, "Setting faction " + faction.ToString());
						eAIGroup group = eAIGroup.GetGroupByLeader(ai);
						group.SetFaction(faction);
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
				itemEnt = entity.GetInventory().CreateInInventory( items[0] );
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
	// Expansion GetObjectFromMissionFile
	// ------------------------------------------------------------
	static bool GetObjectFromMissionFile( FileHandle file, out string name, out vector position, out vector rotation, out string special = "false" )
	{
		string line;
		int lineSize = FGets( file, line );

		if ( lineSize < 1 )
			return false;
		
		TStringArray tokens = new TStringArray;
		line.Split( "|", tokens );

		name = tokens.Get( 0 );
		position = tokens.Get( 1 ).ToVector();
		rotation = tokens.Get( 2 ).ToVector();
		special = tokens.Get( 3 );
		
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
		auto trace = CF_Trace_0(ExpansionTracing.MAPPING, "ExpansionObjectSpawnTools", "LoadMissionTradersFile");
#endif

		CF_Log.Debug( "Attempting to load mission trader file: " + name );

		Object obj;
		EntityAI trader;
		string traderName;
		string className;
		string fileName;
		array<vector> positions;
		vector position;
		vector rotation;
		string gear;

		string filePath = traderFilesFolder + name;
		FileHandle file = OpenFile( filePath, FileMode.READ );

		if ( !file )
			return;
		
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
			trader = EntityAI.Cast( obj );
			
			if ( trader )
			{
				trader.SetPosition( position );
				trader.SetOrientation( rotation );
				vector roll = trader.GetOrientation();
				roll [ 2 ] = roll [ 2 ] - 1;
				trader.SetOrientation( roll );
				roll [ 2 ] = roll [ 2 ] + 1;
				trader.SetOrientation( roll );

				if ( trader.CanAffectPathgraph() )
				{
					trader.SetAffectPathgraph( true, false );
					GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( GetGame().UpdatePathgraphRegionByObject, 100, false, trader );
				}
				
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
			
				#ifdef ENFUSION_AI_PROJECT
				//! See eAIGame::SpawnAI_Patrol
				if ( traderAI )
				{
					#ifdef EXPANSIONMODAI
					eAIGroup ownerGrp = traderAI.GetGroup();
					#else
					if ( eAIGlobal_HeadlessClient )
						GetRPCManager().SendRPC( "eAI", "HCLinkObject", new Param1< PlayerBase >( traderAI ), false, eAIGlobal_HeadlessClient );

					eAIGame game = MissionServer.Cast( GetGame().GetMission() ).GetEAIGame();
					eAIGroup ownerGrp = game.GetGroupByLeader( traderAI );
					#endif
					ownerGrp.SetFaction( new eAIFactionCivilian() );
					for ( j = 0; j < positions.Count(); j++ )
					{
						EXPrint("Adding waypoint " + positions[j]);
						ownerGrp.AddWaypoint( positions[j] );
					}
			
					#ifdef EXPANSIONMODAI
					ownerGrp.SetWaypointBehaviour(eAIWaypointBehavior.ALTERNATE);
					#else
					GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater( traderAI.RequestTransition, 10000, false, "Rejoin" );
					
					traderAI.SetAI( ownerGrp );
					#endif
				}
				#endif

				CF_Log.Debug( "  Created" );
			}
		}

		CloseFile( file );

		CF_Log.Debug( "Created all traders from mission trader file: " + filePath );
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
