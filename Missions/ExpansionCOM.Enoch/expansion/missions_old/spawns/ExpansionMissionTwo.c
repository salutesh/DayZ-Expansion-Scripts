/**
 * ExpansionMissionTwo.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

static void ExpansionMissionTwo()
{
	//! Notifications
	GetNotificationSystem().CreateNotification_Old("Mission", "A high tier bunker mission appeared in Zvir region", ARGB(255, 221, 38, 38), 7);

	//! Loot
	ref array< ref ExpansionMissionEventSoldierLoot > Loot = new array< ref ExpansionMissionEventSoldierLoot >;

	Loot.Insert( new ExpansionMissionEventSoldierLoot("AK74") );
	Loot.Insert( new ExpansionMissionEventSoldierLoot("AK101") );
	Loot.Insert( new ExpansionMissionEventSoldierLoot("SKS") );
	Loot.Insert( new ExpansionMissionEventSoldierLoot("M4A1") );
	Loot.Insert( new ExpansionMissionEventSoldierLoot("AKM") );
	Loot.Insert( new ExpansionMissionEventSoldierLoot("Mag_STANAG_30Rnd") );
	Loot.Insert( new ExpansionMissionEventSoldierLoot("Mag_AKM_30Rnd") );
	Loot.Insert( new ExpansionMissionEventSoldierLoot("Mag_SVD_10Rnd") );
	Loot.Insert( new ExpansionMissionEventSoldierLoot("Mag_VSS_10Rnd") );
	Loot.Insert( new ExpansionMissionEventSoldierLoot("PlateCarrierVest") );
	Loot.Insert( new ExpansionMissionEventSoldierLoot("PlaceCarrierHolster") );
	Loot.Insert( new ExpansionMissionEventSoldierLoot("PlaceCarrierPouches") );
	Loot.Insert( new ExpansionMissionEventSoldierLoot("Rangefinder") );

	ItemBase Box1 = ItemBase.Cast(GetDayZExpansion().SpawnObject("AmmoBox", "113.128952 157.733932 5002.329590", "0.000000 0.000000 0.000000", 60));
	ItemBase Box2 = ItemBase.Cast(GetDayZExpansion().SpawnObject("AmmoBox", "113.151741 157.733932 5002.201172", "0.000000 0.000000 0.000000", 60));
	ItemBase Box3 = ItemBase.Cast(GetDayZExpansion().SpawnObject("AmmoBox", "112.840454 157.733932 5002.329102", "0.000000 0.000000 0.000000", 60));
	ItemBase Box4 = ItemBase.Cast(GetDayZExpansion().SpawnObject("AmmoBox", "112.849998 157.733932 5002.200195", "0.000000 0.000000 0.000000", 60));
	ItemBase Box5 = ItemBase.Cast(GetDayZExpansion().SpawnObject("WoodenCrate", "113.098907 157.633926 5001.935547", "91.000031 0.000000 0.000000", 60));
	ItemBase Chest = ItemBase.Cast(GetDayZExpansion().SpawnObject("SeaChest", "112.968826 157.633926 5000.894531", "-120.999947 0.000000 -0.000000", 60));

	GetDynamicMissions().SpawnMissionLoot(Loot, Box1, 10);
	GetDynamicMissions().SpawnMissionLoot(Loot, Box2, 10);
	GetDynamicMissions().SpawnMissionLoot(Loot, Box3, 10);
	GetDynamicMissions().SpawnMissionLoot(Loot, Box4, 10);
	GetDynamicMissions().SpawnMissionLoot(Loot, Box5, 10);
	GetDynamicMissions().SpawnMissionLoot(Loot, Chest, 20);

	//! Soldiers
	ExpansionSoldierBase soldier1 = GetDayZExpansion().CreateExpansionSoldier("ExpansionSoldierLewis", "90.070755 157.228485 5020.066895", "0 0 0", {"TTSKOPants", "TTSKOBoots", "TTsKOJacket_Camo", "AssaultBag_Ttsko","GorkaHelmet","GorkaHelmetVisor","HighCapacityVest_Olive"} );
	ExpansionSoldierBase soldier2 = GetDayZExpansion().CreateExpansionSoldier("ExpansionSoldierLewis", "101.256798 156.836075 5003.950195", "0 0 0", {"TTSKOPants", "TTSKOBoots", "TTsKOJacket_Camo", "AssaultBag_Ttsko","GorkaHelmet","GorkaHelmetVisor","HighCapacityVest_Olive"} );

	soldier1.SetWeapon("AKM");
	soldier1.SetMagazine("Mag_AKM_30Rnd");
	soldier2.SetWeapon("AKM");
	soldier2.SetMagazine("Mag_AKM_30Rnd");
}