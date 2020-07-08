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
#include "$CurrentDir:\\mpmissions\\Expansion.EnochGloom\\expansion\\ExpansionObjectSpawnTools.c"
#include "$CurrentDir:\\mpmissions\\Expansion.EnochGloom\\expansion\\missions\\MissionConstructor.c"

void main()
{
	bool loadTraderObjects = false;
	bool loadTraderNPCs = false;

	string MissionWorldName = "empty";
	GetGame().GetWorldName(MissionWorldName);

	if (MissionWorldName != "empty")
	{
		//! Spawn mission objects and traders
		FindMissionFiles(MissionWorldName, loadTraderObjects, loadTraderNPCs);
	}

	//INIT WEATHER BEFORE ECONOMY INIT------------------------
	Weather weather = g_Game.GetWeather();

	weather.MissionWeather(false);	// false = use weather controller from Weather.c

	weather.GetOvercast().Set( Math.RandomFloatInclusive(0.02, 0.1), 1, 0);
	weather.GetRain().Set( 0, 1, 0);
	weather.GetFog().Set( 0, 1, 0);

	//INIT ECONOMY--------------------------------------
	Hive ce = CreateHive();
	if ( ce )
		ce.InitOffline();

	//DATE RESET AFTER ECONOMY INIT-------------------------
	int year, month, day, hour, minute;
	int reset_month = 8, reset_day = 10;
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
		playerEnt = GetGame().CreatePlayer(identity, characterName, pos, 0, "NONE");//Creates random player
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

				string chemlightArray[] = { "Chemlight_White", "Chemlight_Yellow", "Chemlight_Green", "Chemlight_Red" };
				int rndIndex = Math.RandomInt(0, 4);
				itemEnt = itemTop.GetInventory().CreateInInventory(chemlightArray[rndIndex]);
				SetRandomHealth(itemEnt);
			}
		}
	}
};

Mission CreateCustomMission(string path)
{
	return new CustomMission();
}