/**
 * ExpansionMissionOne.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

static void ExpansionMissionOne()
{
	//! Notifications
	GetNotificationSystem().CreateNotification_Old("Mission", "A high tier soldiers mission appeared near Zaprudino", ARGB(255, 221, 38, 38), 7);

	//! Loot
	ref array< ref ExpansionMissionEventSoldierLoot > Loot = new array< ref ExpansionMissionEventSoldierLoot >;

	Loot.Insert( new ExpansionMissionEventSoldierLoot("SVD") );
	Loot.Insert( new ExpansionMissionEventSoldierLoot("SVD") );
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

	Barrel_ColorBase Barrel = Barrel_ColorBase.Cast(GetDayZExpansion().SpawnObject("Barrel_Blue", "5593.746582 158.851974 12603.505859", "0.000000 0.000000 0.000000", 60));
	Barrel.Open();

	GetDynamicMissions().SpawnMissionLoot(Loot, Barrel, 10);

	//! Soldiers
	ExpansionSoldierBase soldier1 = GetDayZExpansion().CreateExpansionSoldier("ExpansionSoldierLewis", "5626.089844 158.704315 12602.038086", "0 0 0", {"ExpansionTacticalGlovesDesert", "USMCPants_Desert", "ExpansionDesertTortilla", "ExpansionMich2001Desert","NVGoggles","JungleBoots_Beige","USMCJacket_Desert","SportGlasses_Black","UKAssVest_Khaki"} );
	ExpansionSoldierBase soldier2 = GetDayZExpansion().CreateExpansionSoldier("ExpansionSoldierLewis", "5581.225586 158.695343 12600.797852", "0 0 0", {"ExpansionTacticalGlovesDesert", "USMCPants_Desert", "ExpansionDesertTortilla", "ExpansionMich2001Desert","NVGoggles","JungleBoots_Beige","USMCJacket_Desert","SportGlasses_Black","UKAssVest_Khaki"} );
	ExpansionSoldierBase soldier3 = GetDayZExpansion().CreateExpansionSoldier("ExpansionSoldierLewis", "5625.378418 158.708405 12629.527344", "0 0 0", {"ExpansionTacticalGlovesDesert", "USMCPants_Desert", "ExpansionDesertTortilla", "ExpansionMich2001Desert","NVGoggles","JungleBoots_Beige","USMCJacket_Desert","SportGlasses_Black","UKAssVest_Khaki"} );

	soldier1.SetWeapon("AKM");
	soldier1.SetMagazine("Mag_AKM_30Rnd");
	soldier2.SetWeapon("AKM");
	soldier2.SetMagazine("Mag_AKM_30Rnd");
	soldier3.SetWeapon("AKM");
	soldier3.SetMagazine("Mag_AKM_30Rnd");
}