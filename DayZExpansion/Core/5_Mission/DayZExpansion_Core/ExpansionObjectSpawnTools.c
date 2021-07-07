/**
 * ExpansionObjectSpawnTools.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionObjectSpawnTools
{
	static string objectFilesFolder;
	static string traderFilesFolder;

	static void FindMissionFiles(string missionFolder, bool loadObjects, bool loadTraders)
	{
		array<string> objectFiles;
		array<string> traderFiles;

		objectFilesFolder = missionFolder + "\\expansion\\objects\\";
		traderFilesFolder = missionFolder + "\\expansion\\traders\\";

		if ( loadObjects && FileExist( objectFilesFolder ) )
		{
			objectFiles = FindFilesInLocation(objectFilesFolder);
			if (objectFiles.Count() >= 0)
			{
				
				LoadMissionObjects(objectFiles);
			}
		}

	#ifdef EXPANSIONMODMARKET
		if ( loadTraders && FileExist( traderFilesFolder ) )
		{
			traderFiles = FindFilesInLocation(traderFilesFolder);
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
		#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint( "Attempting to load mission object file: " + name );
			#endif

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
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint( "Attempt to create mission object " + className + " at " + position + " from file:" + filePath + ".");
			#endif

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

		#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint( "Created all objects from mission object file: " + filePath );
			#endif
	}

	// ------------------------------------------------------------
	// Expansion ProcessMissionObject
	// ------------------------------------------------------------
	static void ProcessMissionObject(Object obj)
	{
		#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint( "Try to process mapping object: " + obj.ClassName() );
			#endif

		if ( obj.IsInherited(ExpansionPointLight) )
		{
			ExpansionPointLight light = ExpansionPointLight.Cast( obj );
			if ( light )
			{
				light.SetDiffuseColor(1,0,0);
				light.SetLifetime(3600);
			}
			
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint( "Processed mapping object: " + obj.ClassName() + "!" );
			#endif
		}
		else if ( obj.IsKindOf("Fireplace") )
		{
			Fireplace fireplace = Fireplace.Cast( obj );
			if ( fireplace )
			{
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(fireplace.GetInventory().CreateAttachment, 60 * 1000, false, "Bark_Oak");
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(fireplace.GetInventory().CreateAttachment, 60 * 1000, false, "Firewood");
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(fireplace.GetInventory().CreateAttachment, 60 * 1000, false, "WoodenStick");
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(fireplace.StartFire, 60 * 1000, false);
				fireplace.SetLifetime(3600);
			}

			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint( "Processed mapping object: " + obj.ClassName() + "!" );
			#endif
		}
		else if ( obj.IsInherited(BarrelHoles_ColorBase) )
		{
			BarrelHoles_Red barrel = BarrelHoles_Red.Cast( obj );
			if ( barrel ) 
			{
				barrel.Open();
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(barrel.GetInventory().CreateAttachment, 60 * 1000, false, "Bark_Oak");
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(barrel.GetInventory().CreateAttachment, 60 * 1000, false, "Firewood");
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(barrel.GetInventory().CreateAttachment, 60 * 1000, false, "WoodenStick");
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(barrel.StartFire, 60 * 1000, false);
				barrel.SetLifetime(3600);
			}

			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint( "Processed mapping object: " + obj.ClassName() + "!" );
			#endif
		}
		else if ( obj.IsKindOf("Roadflare") )
		{
			Roadflare flare = Roadflare.Cast( obj );
			if ( flare ) 
			{
				flare.GetCompEM().SetEnergy(999999);
				flare.GetCompEM().SwitchOn();
				flare.SwitchLight(false); //! Flickering
				flare.SetLifetime(0);
			}

			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint( "Processed mapping object: " + obj.ClassName() + "!" );
			#endif
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
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "Attempting to load mission trader file: " + name );
		#endif

		Object obj;
		EntityAI trader;
		string traderName;
		string className;
		string fileName;
		vector position;
		vector rotation;
		TStringArray gear = new TStringArray;

		string filePath = traderFilesFolder + name;
		FileHandle file = OpenFile( filePath, FileMode.READ );

		if ( !file )
			return;
		
		while ( GetTraderFromMissionFile( file, traderName, position, rotation, gear ) )
		{
			array<string> parts = new array<string>;
			traderName.Split(".", parts);
			className = parts[0];
			fileName = parts[1];

			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint( "Attempt to create mission trader " + className + " at " + position + " from file:" + filePath + ".");
			#endif

			obj = GetGame().CreateObject( className, position, false, false, true );
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
					for( int i = 0; i < gear.Count(); i++ )
					{
						trader.GetInventory().CreateAttachment( gear[i] );
					}
				}

				ExpansionTraderStaticBase traderStatic;
				ExpansionTraderNPCBase traderNPC;
				if ( Class.CastTo( traderStatic, trader ) )
					traderStatic.LoadTrader(fileName);
				else if ( Class.CastTo( traderNPC, trader ) )
					traderNPC.LoadTrader(fileName);
				EXPrint("LoadMissionTradersFile traderNPC " + traderNPC + " traderStatic " + traderStatic + " fileName " + fileName);

				#ifdef EXPANSIONEXLOGPRINT
				EXLogPrint( "  Created" );
				#endif
			}
		}

		CloseFile( file );

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "Created all traders from mission trader file: " + filePath );
		#endif
	}

	// ------------------------------------------------------------
	// Expansion GetTraderFromMissionFile
	// ------------------------------------------------------------
	static bool GetTraderFromMissionFile( FileHandle file, out string name, out vector position, out vector rotation, out TStringArray gear )
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

		string gears_array = tokens.Get( 3 );
		TStringArray geartokens = new TStringArray;
		gears_array.Split( ",", geartokens );
		gear = geartokens;
		
		return true;
	}
	#endif
}
