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
	override void RegisterHeavy( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior )
	{
		super.RegisterHeavy( pType, pBehavior );

		pType.AddItemInHandsProfileIK("ExpansionBriefcase", "dz/anims/workspaces/player/player_main/player_main_2h.asi", pBehavior, "dz/anims/anm/player/ik/two_handed/jerrycan.anm");
		pType.AddItemInHandsProfileIK("ExpansionTemporaryOwnedContainer", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/heavy/wooden_crate.anm");
	}
	
	override void RegisterOneHanded(DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior)
	{
		super.RegisterOneHanded(pType, pBehavior);

		pType.AddItemInHandsProfileIK("ExpansionCircuitBoard_MK1", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/tuna.anm");
		pType.AddItemInHandsProfileIK("ExpansionCircuitBoard_MK2", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/tuna.anm");
		pType.AddItemInHandsProfileIK("ExpansionCircuitBoard_MK3", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/tuna.anm");
	}
	
	override void RegisterTwoHanded(DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior)
	{
		super.RegisterTwoHanded(pType, pBehavior);
		
		pType.AddItemInHandsProfileIK("ExpansionDebugGoat", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_extinguisher.asi", pBehavior, "dz/anims/anm/player/ik/two_handed/alice_g.anm");
	}
}
