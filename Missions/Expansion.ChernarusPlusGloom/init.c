/**
 * init.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/
#include "$CurrentDir:\\mpmissions\\Expansion.ChernarusPlusGloom\\expansion\\ExpansionObjectSpawnTools.c"
#include "$CurrentDir:\\mpmissions\\Expansion.ChernarusPlusGloom\\expansion\\missions\\MissionConstructor.c"

void main()
{
	bool loadTraderObjects = false;
	bool loadTraderNPCs = false;

	string MissionWorldName = "empty";
	GetGame().GetWorldName(MissionWorldName);

	switch(MissionWorldName)
	{
		case "ChernarusPlusGloom":
		{
			MissionWorldName = "ChernarusPlus";
			break;
		}
		case "enochGloom":
		{
			MissionWorldName = "enoch";
			break;
		}
	}

	if (MissionWorldName != "empty")
	{
		//! Spawn mission objects and traders
		FindMissionFiles(MissionWorldName, loadTraderObjects, loadTraderNPCs);
	}

	//! Returning weather class reference
	Weather weather = g_Game.GetWeather();

	//! Disable weather controller from mission
	weather.MissionWeather(true); 
	
	//! Setup weather propperties
	weather.GetRain().SetLimits( 0.0 , 0.0 );
    weather.GetOvercast().SetLimits( 0.0 , 0.0 );
    weather.GetFog().SetLimits( 0.0 , 0.01 );

    weather.GetOvercast().SetForecastChangeLimits( 0.0, 0.1 );
    weather.GetRain().SetForecastChangeLimits( 0.0, 0.0 );
    weather.GetFog().SetForecastChangeLimits( 0.0, 0.0 );

    weather.GetOvercast().SetForecastTimeLimits( 1800 , 1800 );
    weather.GetRain().SetForecastTimeLimits( 600 , 600 );
    weather.GetFog().SetForecastTimeLimits( 1800 , 1800 );
    
    weather.GetOvercast().Set( Math.RandomFloatInclusive(0.0, 0.3), 0, 0);
    weather.GetRain().Set( Math.RandomFloatInclusive(0.0, 0.0), 0, 0);
    weather.GetFog().Set( Math.RandomFloatInclusive(0.0, 0.1), 0, 0);
    
    weather.SetWindMaximumSpeed(15);
    weather.SetWindFunctionParams(0.1, 0.3, 50);    

	//! Init server central economy
	Hive ce = CreateHive();
	if ( ce )
		ce.InitOffline();
	
	//! Setup time and date
	int year, month, day, hour, minute;
	int reset_month = 9, reset_day = 20;
	GetGame().GetWorld().GetDate(year, month, day, hour, minute);

	if ((month == reset_month) && (day < reset_day))
	{
		GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
	}
	else
	{
		if ((month == reset_month + 1) && (day > reset_day))
		{
			GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
		}
		else
		{
			if ((month < reset_month) || (month > reset_month + 1))
			{
				GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
			}
		}
	}
}

/**@class		CustomExpansionMission
 * @brief		This class handle expansion serverside mission
 **/
class CustomMission: MissionServer
{
	// ------------------------------------------------------------
	// SetRandomHealth
	// ------------------------------------------------------------
	void SetRandomHealth(EntityAI itemEnt)
	{
		if ( itemEnt )
		{
			int rndHlt = Math.RandomInt(55,100);
			itemEnt.SetHealth("","",rndHlt);
		}
	}
	
	override void OnInit()
	{
		ExpansionMissionModule missionModule;
		if ( Class.CastTo( missionModule, GetModuleManager().GetModule( ExpansionMissionModule ) ) )
		{
			missionModule.SetMissionConstructor( COMMissionConstructor );
		}

		super.OnInit();
	}
	
	// ------------------------------------------------------------
	// Override PlayerBase CreateCharacter
	// ------------------------------------------------------------
	override PlayerBase CreateCharacter(PlayerIdentity identity, vector pos, ParamsReadContext ctx, string characterName)
	{		
		Entity playerEnt;
		playerEnt = GetGame().CreatePlayer(identity, characterName, pos, 0, "NONE");
		Class.CastTo(m_player, playerEnt);
		
		GetGame().SelectPlayer(identity, m_player);
		
		return m_player;
	}

	// ------------------------------------------------------------
	// Override StartingEquipSetup
	// ------------------------------------------------------------
	override void StartingEquipSetup(PlayerBase player, bool clothesChosen)
	{
		if ( GetExpansionSettings() && GetExpansionSettings().GetSpawn() && GetExpansionSettings().GetSpawn().StartingGear.UseStartingGear )
		{
			SetStartingGear(player);
		}
		else
		{
			EntityAI itemTop;
			EntityAI itemEnt;
			ItemBase itemBs;
			float rand;
			
			itemTop = player.FindAttachmentBySlotName("Body");
			
			if ( itemTop )
			{
				itemEnt = itemTop.GetInventory().CreateInInventory("Rag");
				if ( Class.CastTo(itemBs, itemEnt ) )
					itemBs.SetQuantity(4);

				SetRandomHealth(itemEnt);
				
				itemEnt = itemTop.GetInventory().CreateInInventory("RoadFlare");
				SetRandomHealth(itemEnt);
				
				rand = Math.RandomFloatInclusive(0.0, 1.0);
				if ( rand < 0.35 )
					itemEnt = player.GetInventory().CreateInInventory("Apple");
				else if ( rand > 0.65 )
					itemEnt = player.GetInventory().CreateInInventory("Pear");
				else
					itemEnt = player.GetInventory().CreateInInventory("Plum");
			
				SetRandomHealth(itemEnt);
			}
		}
	}
}

Mission CreateCustomMission(string path)
{
	return new CustomMission();
}