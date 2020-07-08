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

void FindMissionFiles(string worldname, bool loadObjects, bool loadTraders)
{
	array<string> objectFiles;
	array<string> traderFiles;

	string objectFilesFolder = "$CurrentDir:\\missions\\ExpansionCOM." + worldname + "\\expansion\\objects\\";
	string traderFilesFolder = "$CurrentDir:\\missions\\ExpansionCOM." + worldname + "\\expansion\\traders\\";

	if ( loadObjects && FileExist( objectFilesFolder ) )
	{
		if (FindFilesInLocation(objectFilesFolder).Count() >= 0)
		{
			objectFiles = FindFilesInLocation(objectFilesFolder);
			
			LoadMissionObjects(objectFiles, worldname);
		}
	}

	if ( loadTraders && FileExist( traderFilesFolder ) )
	{
		if (FindFilesInLocation(traderFilesFolder).Count() >= 0)
		{
			traderFiles = FindFilesInLocation(traderFilesFolder);

			LoadMissionTraders(traderFiles, worldname);
		}
	}
}

// ------------------------------------------------------------
// Expansion LoadMissionObjects
// ------------------------------------------------------------
void LoadMissionObjects( array<string> files, string worldname )
{
	for ( int i = 0; i < files.Count(); i++ )
	{
		LoadMissionObjectsFile( files[i], worldname );
	}
}

// ------------------------------------------------------------
// Expansion LoadMissionObjectsFile
// ------------------------------------------------------------
void LoadMissionObjectsFile( string name, string worldname )
{
	Print( "Attempting to load mission object file: " + name );

	Object obj;
	string className;
	vector position;
	vector rotation;
	string special;

	string filePath = "$CurrentDir:\\missions\\ExpansionCOM." + worldname +"\\expansion\\objects\\" + name;
	FileHandle file = OpenFile( filePath, FileMode.READ );

	if ( !file )
		return;
	
	while ( GetObjectFromMissionFile( file, className, position, rotation, special ) )
	{
		Print( "Attempt to create mission object " + className + " at " + position + " from file:" + filePath + ".");
		obj = GetGame().CreateObject( className, position, false, false, true );

		if ( obj )
		{
			obj.SetPosition( position );
			obj.SetOrientation( rotation );
			vector roll = obj.GetOrientation();
			roll [ 2 ] = roll [ 2 ] - 1;
			obj.SetOrientation( roll );
			roll [ 2 ] = roll [ 2 ] + 1;
			obj.SetOrientation( roll );

			if ( obj.CanAffectPathgraph() )
			{
				obj.SetAffectPathgraph( true, false );
				GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( GetGame().UpdatePathgraphRegionByObject, 100, false, obj );
			}
			
			if ( special == "true")
				ProcessMissionObject( obj );

			Print( "  Created" );
		}
	}

	CloseFile( file );

	Print( "Created all objects from mission object file: " + filePath );
}

// ------------------------------------------------------------
// Expansion ProcessMissionObject
// ------------------------------------------------------------
void ProcessMissionObject(Object obj)
{
	Print( "Try to process mapping object: " + obj.ClassName() );
	if ( obj.IsInherited(ExpansionPointLight) )
	{
		ExpansionPointLight light = ExpansionPointLight.Cast( obj );
		if ( light )
			light.SetDiffuseColor(1,0,0);
		
		Print( "Processed mapping object: " + obj.ClassName() + "!" );
	}
	else if ( obj.IsKindOf("Fireplace") )
	{
		Fireplace fireplace = Fireplace.Cast( obj );
		if ( fireplace )
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(fireplace.GetInventory().CreateAttachment, 60 * 1000, true, "Bark_Oak");
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(fireplace.GetInventory().CreateAttachment, 60 * 1000, true, "Firewood");
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(fireplace.GetInventory().CreateAttachment, 60 * 1000, true, "WoodenStick");
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(fireplace.StartFire, 60 * 1000, true);
		}
		Print( "Processed mapping object: " + obj.ClassName() + "!" );
	}
	else if ( obj.IsInherited(BarrelHoles_ColorBase) )
	{
		BarrelHoles_Red barrel = BarrelHoles_Red.Cast( obj );
		if ( barrel ) 
		{
			barrel.Open();
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(barrel.GetInventory().CreateAttachment, 60 * 1000, true, "Bark_Oak");
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(barrel.GetInventory().CreateAttachment, 60 * 1000, true, "Firewood");
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(barrel.GetInventory().CreateAttachment, 60 * 1000, true, "WoodenStick");
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(barrel.StartFire, 60 * 1000, true);
		}
		Print( "Processed mapping object: " + obj.ClassName() + "!" );
	}
	else if ( obj.IsKindOf("Roadflare") )
	{
		Roadflare flare = Roadflare.Cast( obj );
		if ( flare ) 
		{
			flare.GetCompEM().SetEnergy(999999);
			flare.GetCompEM().SwitchOn();
			flare.SwitchLight(false); //! Flickering
		}
		Print( "Processed mapping object: " + obj.ClassName() + "!" );
	}
}

// ------------------------------------------------------------
// Expansion GetObjectFromMissionFile
// ------------------------------------------------------------
bool GetObjectFromMissionFile( FileHandle file, out string name, out vector position, out vector rotation, out string special = "false" )
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

// ------------------------------------------------------------
// Expansion LoadMissionTraders
// ------------------------------------------------------------
void LoadMissionTraders( array<string> files, string worldname )
{
	for ( int i = 0; i < files.Count(); i++ )
	{
		LoadMissionTradersFile( files[i], worldname );
	}
}

// ------------------------------------------------------------
// Expansion LoadMissionTradersFile
// ------------------------------------------------------------
void LoadMissionTradersFile( string name, string worldname )
{
	Print( "Attempting to load mission trader file: " + name );

	Object obj;
	ExpansionTraderBase trader;
	string className;
	vector position;
	vector rotation;
	TStringArray gear = new TStringArray;

	string filePath = "$CurrentDir:\\missions\\ExpansionCOM." + worldname + "\\expansion\\traders\\" + name;
	FileHandle file = OpenFile( filePath, FileMode.READ );

	if ( !file )
		return;
	
	while ( GetTraderFromMissionFile( file, className, position, rotation, gear ) )
	{
		Print( "Attempt to create mission trader " + className + " at " + position + " from file:" + filePath + ".");
		obj = GetGame().CreateObject( className, position, false, false, true );
		trader = ExpansionTraderBase.Cast( obj );
		
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

			Print( "  Created" );
		}
	}

	CloseFile( file );

	Print( "Created all traders from mission trader file: " + filePath );
}

// ------------------------------------------------------------
// Expansion GetTraderFromMissionFile
// ------------------------------------------------------------
bool GetTraderFromMissionFile( FileHandle file, out string name, out vector position, out vector rotation, out TStringArray gear )
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