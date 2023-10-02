/**
 * JMAnimRegister.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ModItemRegisterCallbacks
{
	override void RegisterOneHanded(DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior)
	{
		super.RegisterOneHanded(pType, pBehavior);

		pType.AddItemInHandsProfileIK("Expansion_AnomalyCore_Ice", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/explosives/POX_grenade.anm");
		pType.AddItemInHandsProfileIK("Expansion_AnomalyCore_Warper", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/explosives/POX_grenade.anm");
		pType.AddItemInHandsProfileIK("Expansion_AnomalyCore_Fire", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/explosives/POX_grenade.anm");
		pType.AddItemInHandsProfileIK("ExpansionAnomalyCoreProtectiveCase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/protector_case.anm");
		pType.AddItemInHandsProfileIK("Expansion_KeyCard_NA_Antenna", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/PunchedCard.anm");
		pType.AddItemInHandsProfileIK("Expansion_KeyCard_Teleporter", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/PunchedCard.anm");
		pType.AddItemInHandsProfileIK("Expansion_KeyCard_A1_B1", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/PunchedCard.anm");
		pType.AddItemInHandsProfileIK("Expansion_SupplyCrate_Bunker_Key", "dz/anims/workspaces/player/player_main/props/player_main_1h_keys.asi", pBehavior, "dz/anims/anm/player/ik/gear/handcuff_keys.anm");
		pType.AddItemInHandsProfileIK("Expansion_SupplyCrate_WorldT1_Key", "dz/anims/workspaces/player/player_main/props/player_main_1h_keys.asi", pBehavior, "dz/anims/anm/player/ik/gear/handcuff_keys.anm");
		pType.AddItemInHandsProfileIK("Expansion_Bunker_Generator_Key", "dz/anims/workspaces/player/player_main/props/player_main_1h_keys.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/sparkplug.anm");
		pType.AddItemInHandsProfileIK("AmmoCleaningKit", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/cleaning_kit_wood.anm");
		pType.AddItemInHandsProfileIK("CommunityGoalTicket", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/PunchedCard.anm");
		pType.AddItemInHandsProfileIK("MedicalSupplies", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/first_aid_kit.anm");
		pType.AddItemInHandsProfileIK("AmmunitionSupplies", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/cleaning_kit_wood.anm");
		pType.AddItemInHandsProfileIK("NA_Dokuments_AthenaOneBunker1", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/paper.anm");
		pType.AddItemInHandsProfileIK("NA_Dokuments_AthenaOneBunker2", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/paper.anm");
		pType.AddItemInHandsProfileIK("NA_Dokuments_Antenna1", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/paper.anm");
		pType.AddItemInHandsProfileIK("NA_Dokuments_Antenna2", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/paper.anm");
		pType.AddItemInHandsProfileIK("NA_Dokuments_Antenna3", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/paper.anm");
	}
};