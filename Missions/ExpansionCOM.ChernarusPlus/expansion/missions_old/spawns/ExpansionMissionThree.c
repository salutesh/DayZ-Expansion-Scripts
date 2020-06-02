/**
 * ExpansionMissionThree.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

static void ExpansionMissionThree()
{
	//! Notifications
	GetNotificationSystem().CreateNotification_Old("Mission", "A civilian mission appeared in Guglovo region", ARGB(255, 221, 38, 38), 7);

	//! Loot
	ref array< ref ExpansionMissionEventSoldierLoot > Loot = new array< ref ExpansionMissionEventSoldierLoot >;

	Loot.Insert( new ExpansionMissionEventSoldierLoot("SodaCan_Pipsi") );
	Loot.Insert( new ExpansionMissionEventSoldierLoot("SodaCan_Cola") );
	Loot.Insert( new ExpansionMissionEventSoldierLoot("SodaCan_Spite") );
	Loot.Insert( new ExpansionMissionEventSoldierLoot("SodaCan_Kvass") );
	Loot.Insert( new ExpansionMissionEventSoldierLoot("WaterBottle") );
	Loot.Insert( new ExpansionMissionEventSoldierLoot("Canteen") );
	Loot.Insert( new ExpansionMissionEventSoldierLoot("AmmoBox_308WinTracer_20Rnd") );
	Loot.Insert( new ExpansionMissionEventSoldierLoot("AmmoBox_762x54Tracer_20Rnd") );
	Loot.Insert( new ExpansionMissionEventSoldierLoot("AmmoBox_762x39Tracer_20Rnd") );
	Loot.Insert( new ExpansionMissionEventSoldierLoot("Ammo_308WinTracer") );
	Loot.Insert( new ExpansionMissionEventSoldierLoot("Ammo_762x54Tracer") );
	Loot.Insert( new ExpansionMissionEventSoldierLoot("Ammo_762x39Tracer") );
	Loot.Insert( new ExpansionMissionEventSoldierLoot("VSS") );

	ItemBase Chest1 = ItemBase.Cast(GetDayZExpansion().SpawnObject("SeaChest", "9083.189453 295.262146 7105.940430", "65.000000 0.000000 0.000000", 60));
	ItemBase Chest2 = ItemBase.Cast(GetDayZExpansion().SpawnObject("SeaChest", "9074.731445 295.213837 7101.162109", "-58.999989 0.000000 -0.000000", 60));

	GetDynamicMissions().SpawnMissionLoot(Loot, Chest1, 10);
	GetDynamicMissions().SpawnMissionLoot(Loot, Chest2, 10);

	//! Zombies
	GetGame().CreateObject("ZmbM_JournalistSkinny", "9094.033203 293.797638 7109.916992", false, true);
	GetGame().CreateObject("ZmbM_FishermanOld_Grey", "9092.425781 292.572968 7098.217773", false, true);
	GetGame().CreateObject("ZmbM_FirefighterNormal", "9090.531250 291.169525 7078.356445", false, true);
	GetGame().CreateObject("ZmbF_CitizenBSkinny", "9067.636719 294.881653 7099.928711", false, true);
	GetGame().CreateObject("ZmbF_JournalistNormal_White", "9094.838867 291.246765 7085.076660", false, true);
}