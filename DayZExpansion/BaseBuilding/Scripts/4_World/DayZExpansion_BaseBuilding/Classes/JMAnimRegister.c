modded class ModItemRegisterCallbacks
{
	override void RegisterEmptyHanded( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior )
	{
		super.RegisterEmptyHanded( pType, pBehavior );
	}
	
	override void RegisterTwoHanded( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior )
	{
		super.RegisterTwoHanded( pType, pBehavior );
		
		//! Small Placables
		pType.AddItemInHandsProfileIK("ExpansionCone", "dz/anims/workspaces/player/player_main/player_main_2h.asi", pBehavior, "dz/anims/anm/player/ik/two_handed/spotlight.anm");
		pType.AddItemInHandsProfileIK("ExpansionSignDanger2", "dz/anims/workspaces/player/player_main/player_main_2h.asi", pBehavior, "dz/anims/anm/player/ik/two_handed/shovel.anm");
		pType.AddItemInHandsProfileIK("ExpansionSignDanger3", "dz/anims/workspaces/player/player_main/player_main_2h.asi", pBehavior, "dz/anims/anm/player/ik/two_handed/spotlight.anm");
		
		//! Basebuilding Kits
		pType.AddItemInHandsProfileIK("ExpansionCamoTentKit", "dz/anims/workspaces/player/player_main/player_main_2h.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/BatteryTruck.anm");
		pType.AddItemInHandsProfileIK("ExpansionCamoBoxKit", "dz/anims/workspaces/player/player_main/player_main_2h.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/BatteryTruck.anm");
		
		//! Basebuilding Components
		pType.AddItemInHandsProfileIK("ExpansionLumber3", "dz/anims/workspaces/player/player_main/player_main_2h.asi", pBehavior, "dz/anims/anm/player/ik/two_handed/wooden_plank.anm");
		pType.AddItemInHandsProfileIK("ExpansionLumber1_5", "dz/anims/workspaces/player/player_main/player_main_2h.asi", pBehavior, "dz/anims/anm/player/ik/two_handed/wooden_plank.anm");
		pType.AddItemInHandsProfileIK("ExpansionLumber1", "dz/anims/workspaces/player/player_main/player_main_2h.asi", pBehavior, "dz/anims/anm/player/ik/two_handed/wooden_plank.anm");
		pType.AddItemInHandsProfileIK("ExpansionLumber0_5", "dz/anims/workspaces/player/player_main/player_main_2h.asi", pBehavior, "dz/anims/anm/player/ik/two_handed/wooden_plank.anm");
	
		pType.AddItemInHandsProfileIK("ExpansionSafeMini", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_extinguisher.asi", pBehavior, "dz/anims/anm/player/ik/two_handed/coyote_g.anm");
		
		pType.AddItemInHandsProfileIK("ExpansionBoltCutters", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_pipewrench.asi", pBehavior, "dz/anims/anm/player/ik/two_handed/pipewrench.anm");
	}

	override void RegisterOneHanded( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior )
	{
		super.RegisterOneHanded( pType, pBehavior );

		//! Basebuilding Kits
		pType.AddItemInHandsProfileIK("ExpansionBarbedWireKit", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/barbed_wire.anm");
		
		pType.AddItemInHandsProfileIK("ExpansionCodeLock", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/paper.anm");
		
		//! Misc Items
		pType.AddItemInHandsProfileIK("ExpansionPowerSwitch", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/fence_kit_folded.anm");
		pType.AddItemInHandsProfileIK("ExpansionPropaneTorch", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/ButanCanister.anm");
	}

	override void RegisterRestrained( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior )
	{
		super.RegisterRestrained( pType, pBehavior );
	}

	override void RegisterSurrender( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior )
	{
		super.RegisterSurrender( pType, pBehavior );
	}

	override void RegisterHeavy( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior )
	{
		super.RegisterHeavy( pType, pBehavior );

		//! Misc Placables
		pType.AddItemInHandsProfileIK("ExpansionSignDanger", "dz/anims/workspaces/player/player_main/props/player_main_heavy_generator.asi", pBehavior, "dz/anims/anm/player/ik/heavy/hesco_box.anm");
		pType.AddItemInHandsProfileIK("ExpansionSignRoadBarrier", "dz/anims/workspaces/player/player_main/props/player_main_heavy_generator.asi", pBehavior, "dz/anims/anm/player/ik/heavy/hesco_box.anm");
		pType.AddItemInHandsProfileIK("ExpansionToolBox", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/hatchback_02/Hatchback_02_Wheel.anm");
	
		//! Basebuilding Placables
		pType.AddItemInHandsProfileIK("ExpansionHescoKit", "dz/anims/workspaces/player/player_main/props/player_main_heavy_generator.asi", pBehavior, "dz/anims/anm/player/ik/heavy/hesco_box.anm");
		pType.AddItemInHandsProfileIK("ExpansionBarrierGateKit", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/heavy/wooden_crate.anm");
		pType.AddItemInHandsProfileIK("ExpansionSafeLarge", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/heavy/55galDrum.anm");
		pType.AddItemInHandsProfileIK("ExpansionSafeMedium", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/heavy/55galDrum.anm");
		pType.AddItemInHandsProfileIK("ExpansionStove", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/heavy/55galDrum.anm");
		pType.AddItemInHandsProfileIK("ExpansionGunrack", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/heavy/sea_chest.anm");
		pType.AddItemInHandsProfileIK("ExpansionParkingMeterKit", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/heavy/car_tent_packed.anm");
	}
}
