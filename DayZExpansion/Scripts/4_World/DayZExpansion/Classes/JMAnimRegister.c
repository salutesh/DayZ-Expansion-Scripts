modded class ModItemRegisterCallbacks
{
	override void RegisterTwoHanded( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior )
	{
		super.RegisterTwoHanded( pType, pBehavior );
		
		pType.AddItemInHandsProfileIK("ExpansionBoxSetBase", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/hatchback_02/Hatchback_02_Wheel.anm");
	}

	override void RegisterOneHanded( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior )
	{
		super.RegisterOneHanded( pType, pBehavior );
		
		//! Consumables
		pType.AddItemInHandsProfileIK("ExpansionBread1", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_pipe.asi", pBehavior, "dz/anims/anm/player/ik/gear/crowbar.anm");
		pType.AddItemInHandsProfileIK("ExpansionBread2", "dz/anims/workspaces/player/player_main/weapons/player_main_1h_pipe.asi", pBehavior, "dz/anims/anm/player/ik/gear/crowbar.anm");
		pType.AddItemInHandsProfileIK("ExpansionBread3", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/rice.anm");
		pType.AddItemInHandsProfileIK("ExpansionCheese1", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/tuna.anm");
		pType.AddItemInHandsProfileIK("ExpansionCheese2", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/crowbar.anm");
		pType.AddItemInHandsProfileIK("ExpansionCheese3", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/tuna.anm");
		pType.AddItemInHandsProfileIK("ExpansionCheese4", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/tuna.anm");
		
		//! Props
		pType.AddItemInHandsProfileIK("Expansion_Guitar", "DayZExpansion/Animations/Player/Props/player_main_guitar.asi", pBehavior, "dz/anims/anm/player/ik/gear/LongWoodenStick.anm");
	}
}
