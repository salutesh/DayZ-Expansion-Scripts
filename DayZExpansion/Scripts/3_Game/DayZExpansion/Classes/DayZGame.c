/**
 * ExpansionDayZGame.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

enum ExpansionProfilesOptions
{
	LICENCE
}

modded class DayZGame
{	
	protected float m_WaterDensity;
	protected float m_WorldGravity;
	protected string m_ExpansionClientVersion;
	protected string m_ExpansionLastestVersion;

	protected ref ExpansionGame m_ExpansionGame;

	// ------------------------------------------------------------
	// DayZGame Constructor
	// ------------------------------------------------------------
	void DayZGame()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZGame::DayZGame - Start");
		#endif

		Expansion_SetupDebugger();
		
		m_WaterDensity = 0.5;
		m_WorldGravity = 9.81;

		if ( !FileExist( EXPANSION_FOLDER ) )
		{
			MakeDirectory( EXPANSION_FOLDER );
		}
		
		if ( FileExist( EXPANSION_TEMP_INTERIORS ) )
		{	
			DeleteFile( EXPANSION_TEMP_INTERIORS );
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZGame::DayZGame - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// DayZGame Deconstructor
	// -------------------------------------------------------------
	void ~DayZGame()
	{
		if ( FileExist( EXPANSION_TEMP_INTERIORS ) )
		{	
			DeleteFile( EXPANSION_TEMP_INTERIORS );
		}
	}
	
	// ------------------------------------------------------------
	// Expansion SetExpansionGame
	// ------------------------------------------------------------
	void SetExpansionGame( ref ExpansionGame game )
	{
		m_ExpansionGame = game;
	}
	
	// ------------------------------------------------------------
	// Expansion GetExpansionGame
	// ------------------------------------------------------------
	ref ExpansionGame GetExpansionGame()
	{
		return m_ExpansionGame;
	}
	
	// ------------------------------------------------------------
	// Expansion Float GetWaterDensity
	// ------------------------------------------------------------	
	float GetWaterDensity()
	{
		return m_WaterDensity;
	}
	
	// ------------------------------------------------------------
	// Expansion Float GetWorldGravity
	// ------------------------------------------------------------	
	float GetWorldGravity()
	{
		return m_WorldGravity;
	}
	
	// ------------------------------------------------------------
	// Expansion Float CalculateBuoyancyAtPosition
	// ------------------------------------------------------------
	float CalculateBuoyancyAtPosition( vector position, float offset, float objectMass, float objectDensity, vector velocity, out bool isAboveWater )
	{
		float resultForce = 0;
		float downForce = 0.5;

		float depth = GetWaterDepth( position ) + offset;

		float forceFactor = 1.0 - ( depth / objectDensity );
		forceFactor = -forceFactor;
		if ( forceFactor > 0 )
		{
			resultForce = m_WorldGravity * objectMass * ( forceFactor - velocity[1] * m_WaterDensity );
			resultForce += -downForce * objectMass;
			
			isAboveWater = false;
		} else
		{
			isAboveWater = true;
		}

		return resultForce;
	}
	
	// ------------------------------------------------------------
	// Override String CreateDefaultPlayer
	// ------------------------------------------------------------
	override string CreateDefaultPlayer()
	{						
		string path = "cfgVehicles";
		string child_name = ""; 
		int count = GetGame().ConfigGetChildrenCount( path );
		
		for ( int p = 0; p < count; p++ )
		{
			GetGame().ConfigGetChildName( path, p, child_name );
			
			if ( GetGame().ConfigGetInt(path + " " + child_name + " scope") == 2 && GetGame().IsKindOf( child_name, "SurvivorBase" ) )
			{
				if ( child_name.Contains( "Expansion" ) || child_name.Contains( "Trader" ) )
					continue;
					
				return child_name;
			}
		}
		
		return "";
	}
	
	// ------------------------------------------------------------
	// Override String CreateRandomPlayer
	// ------------------------------------------------------------
	override string CreateRandomPlayer()
	{		
		string path = "cfgVehicles";
		string child_name = "";
		int count = GetGame().ConfigGetChildrenCount( path );
		array<string> char_class_names = new array<string>;
		
		for ( int p = 0; p < count; p++ )
		{
			GetGame().ConfigGetChildName( path, p, child_name );
			
			if ( GetGame().ConfigGetInt(path + " " + child_name + " scope") == 2 && GetGame().IsKindOf( child_name, "SurvivorBase" ) )
			{
				if ( child_name.Contains( "Expansion" ) || child_name.Contains( "Trader" ) )
					continue;

				char_class_names.Insert(child_name);
			}
		}

		return char_class_names.GetRandomElement();
	}
	
	// ------------------------------------------------------------
	// Override TStringArray ListAvailableCharacters
	// ------------------------------------------------------------
	override TStringArray ListAvailableCharacters()
	{		
		string path = "cfgVehicles";
		string child_name = "";
		int count = GetGame().ConfigGetChildrenCount ( path );
		array<string> char_class_names = new array<string>;
		
		for (int p = 0; p < count; p++)
		{
			GetGame().ConfigGetChildName ( path, p, child_name );
			
			if (GetGame().ConfigGetInt(path + " " + child_name + " scope") == 2 && GetGame().IsKindOf(child_name,"SurvivorBase"))
			{
				if ( child_name.Contains( "Expansion" ) || child_name.Contains( "Trader" ) )
					continue;

				char_class_names.Insert(child_name);
			}
		}
		
		return char_class_names;
	}
	
	// ------------------------------------------------------------
	// Expansion SetExpansionClientVersion
	// ------------------------------------------------------------
	void SetExpansionClientVersion(string version)
	{
		m_ExpansionClientVersion = version;
	}
	
	// ------------------------------------------------------------
	// Expansion SetExpansionLatestVersion
	// ------------------------------------------------------------
	void SetExpansionLatestVersion(string version)
	{
		m_ExpansionLastestVersion = version;
	}
	
	// ------------------------------------------------------------
	// Expansion GetExpansionClientVersion
	// ------------------------------------------------------------
	string GetExpansionClientVersion()
	{
		return m_ExpansionClientVersion;
	}
	
	// ------------------------------------------------------------
	// Expansion GetExpansionLatestVersion
	// ------------------------------------------------------------
	string GetExpansionLatestVersion()
	{
		return m_ExpansionLastestVersion;
	}
	
	// ------------------------------------------------------------
	// Override FirearmEffects
	// ------------------------------------------------------------
	override void FirearmEffects( Object source, Object directHit, int componentIndex, string surface, vector pos, vector surfNormal, vector exitPos, vector inSpeed, vector outSpeed, bool isWater, bool deflected, string ammoType ) 
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "DayZGame::FirearmEffects - Start");
		#endif
		
		super.FirearmEffects( source, directHit, componentIndex, surface, pos, surfNormal, exitPos, inSpeed, outSpeed, isWater, deflected, ammoType );

		if (m_ExpansionGame != NULL)
			m_ExpansionGame.FirearmEffects( source, directHit, componentIndex, surface, pos, surfNormal, exitPos, inSpeed, outSpeed, isWater, deflected, ammoType );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint( "DayZGame::FirearmEffects - End");
		#endif
	}

	override void OnUpdate( bool doSim, float timeslice ) 
	{
		super.OnUpdate( doSim, timeslice );

		if (m_ExpansionGame != NULL)
			m_ExpansionGame.OnUpdate( doSim, timeslice );
	}

	override void OnRPC( PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx )
	{
		super.OnRPC( sender, target, rpc_type, ctx );
				
		//! Move below if there becomes some problems
		//! Steve moved below on 30.06.2020 to fix a NULL pointer
		if (m_ExpansionGame != NULL)
			if ( m_ExpansionGame.OnRPC( sender, target, rpc_type, ctx ) )
				return;
	}
}