modded class ModItemRegisterCallbacks
{
	override void RegisterEmptyHanded( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior )
	{
		super.RegisterEmptyHanded( pType, pBehavior );
	}

	override void RegisterTwoHanded( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior )
	{
		super.RegisterTwoHanded( pType, pBehavior );
		
		pType.AddItemInHandsProfileIK("ExpansionHelicopterBattery", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_extinguisher.asi", pBehavior, "dz/anims/anm/player/ik/two_handed/hunting_g.anm");
		pType.AddItemInHandsProfileIK("ExpansionAircraftBattery", "dz/anims/workspaces/player/player_main/weapons/player_main_2h_extinguisher.asi", pBehavior, "dz/anims/anm/player/ik/two_handed/hunting_g.anm");
		
		pType.AddItemInHandsProfileIK("ExpansionMerlinFrontWheel", "dz/anims/workspaces/player/player_main/player_main_2h.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/batterytruck.anm");
		pType.AddItemInHandsProfileIK("ExpansionUAZWheel_Ruined", "dz/anims/workspaces/player/player_main/player_main_2h.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/batterytruck.anm");
	}

	override void RegisterOneHanded( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior )
	{
		super.RegisterOneHanded( pType, pBehavior );

		pType.AddItemInHandsProfileIK("ExpansionHydraulicHoses", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/gear/fence_kit_folded.anm");
		pType.AddItemInHandsProfileIK("ExpansionCarKey", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/sparkplug.anm");
		pType.AddItemInHandsProfileIK("ExpansionKeyChainBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/sparkplug.anm");
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

		pType.AddItemInHandsProfileIK("ExpansionVodnikWheel", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/hatchback_02/Hatchback_02_wheel.anm");
		
		pType.AddItemInHandsProfileIK("ExpansionBusWheel", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/v3s/truck_01_wheel.anm");
		pType.AddItemInHandsProfileIK("ExpansionBusWheelDouble", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/v3s/truck_01_wheeldouble.anm");
		pType.AddItemInHandsProfileIK("ExpansionBusWheel_Ruined", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/v3s/truck_01_wheel_ruined.anm");
		pType.AddItemInHandsProfileIK("ExpansionBusWheelDouble_Ruined", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/v3s/truck_01_wheeldouble_ruined.anm");
		
		pType.AddItemInHandsProfileIK("ExpansionTractorFrontWheel", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/v3s/truck_01_wheel.anm");
		pType.AddItemInHandsProfileIK("ExpansionTractorBackWheel", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/v3s/truck_01_wheeldouble.anm");
		pType.AddItemInHandsProfileIK("ExpansionTractorFrontWheel_Ruined", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/v3s/truck_01_wheel.anm");
		pType.AddItemInHandsProfileIK("ExpansionTractorBackWheel_Ruined", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/v3s/truck_01_wheeldouble.anm");
		
		pType.AddItemInHandsProfileIK("ExpansionVodnikWheel", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/v3s/truck_01_wheel.anm");
		
		pType.AddItemInHandsProfileIK("ExpansionUAZWheel", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/hatchback/Hatchback_wheel.anm");
        pType.AddItemInHandsProfileIK("ExpansionUAZDoorDriver", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/sedan_02/sedan_02_Trunk.anm");
		pType.AddItemInHandsProfileIK("ExpansionUAZDoorCoDriver", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/sedan_02/sedan_02_Trunk.anm");
		pType.AddItemInHandsProfileIK("ExpansionUAZDoorCargo1", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/sedan_02/sedan_02_Trunk.anm");
		pType.AddItemInHandsProfileIK("ExpansionUAZDoorCargo2", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/sedan_02/sedan_02_Trunk.anm");
		pType.AddItemInHandsProfileIK("ExpansionUAZDoorHood", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/hatchback/hatchback_hood.anm");

        pType.AddItemInHandsProfileIK("Expansion_Landrover_Wheel", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/hatchback/Hatchback_wheel.anm");
        pType.AddItemInHandsProfileIK("Expansion_Landrover_Wheel_Ruined", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/hatchback/Hatchback_wheel.anm");
		pType.AddItemInHandsProfileIK("Expansion_Landrover_Driverdoor", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/sedan_02/sedan_02_Trunk.anm");
		pType.AddItemInHandsProfileIK("Expansion_Landrover_Codriverdoor", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/sedan_02/sedan_02_Trunk.anm");
		pType.AddItemInHandsProfileIK("Expansion_Landrover_Left", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/sedan_02/sedan_02_Trunk.anm");
		pType.AddItemInHandsProfileIK("Expansion_Landrover_Right", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/sedan_02/sedan_02_Trunk.anm");
		pType.AddItemInHandsProfileIK("Expansion_Landrover_Trunk", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/hatchback/hatchback_trunk.anm");
		pType.AddItemInHandsProfileIK("Expansion_Landrover_Hood", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/hatchback/hatchback_hood.anm");

		pType.AddItemInHandsProfileIK("ExpansionAnWheel", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/hatchback_02/Hatchback_02_wheel.anm");
		pType.AddItemInHandsProfileIK("ExpansionAnWheelSmall", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/hatchback_02/Hatchback_02_wheel.anm");
		
		pType.AddItemInHandsProfileIK("ExpansionC130JWheel", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/hatchback_02/Hatchback_02_wheel.anm");
		
		pType.AddItemInHandsProfileIK("ExpansionTT650Wheel", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/hatchback_02/Hatchback_02_wheel.anm");
		
		pType.AddItemInHandsProfileIK("Expansion250NWheel", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/hatchback_02/Hatchback_02_wheel.anm");
		
		pType.AddItemInHandsProfileIK("ExpansionOldBikeWheel", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/hatchback_02/Hatchback_02_wheel.anm");
		
		pType.AddItemInHandsProfileIK("ExpansionMerlinBackWheel", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/hatchback_02/Hatchback_02_wheel.anm");

		pType.AddItemInHandsProfileIK("Vehicle_ExpansionVodnikWheel", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/hatchback_02/Hatchback_02_wheel.anm");
		
		pType.AddItemInHandsProfileIK("Vehicle_ExpansionBusWheel", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/v3s/v3s_wheel.anm");
		pType.AddItemInHandsProfileIK("Vehicle_ExpansionBusWheelDouble", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/v3s/v3s_wheeldouble.anm");
		
		pType.AddItemInHandsProfileIK("Vehicle_ExpansionTractorFrontWheel", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/v3s/v3s_wheel.anm");
		pType.AddItemInHandsProfileIK("Vehicle_ExpansionTractorBackWheel", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/v3s/v3s_wheeldouble.anm");
		
		pType.AddItemInHandsProfileIK("Vehicle_ExpansionVodnikWheel", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/hatchback_02/Hatchback_02_wheel.anm");
		
		pType.AddItemInHandsProfileIK("Vehicle_ExpansionUAZWheel", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/hatchback_02/Hatchback_02_wheel.anm");
		
		pType.AddItemInHandsProfileIK("Vehicle_ExpansionAnWheel", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/hatchback_02/Hatchback_02_wheel.anm");
		pType.AddItemInHandsProfileIK("Vehicle_ExpansionAnWheelSmall", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/hatchback_02/Hatchback_02_wheel.anm");
		
		pType.AddItemInHandsProfileIK("Vehicle_ExpansionC130JWheel", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/hatchback_02/Hatchback_02_wheel.anm");
		
		pType.AddItemInHandsProfileIK("Vehicle_ExpansionTT650Wheel", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/hatchback_02/Hatchback_02_wheel.anm");
		
		pType.AddItemInHandsProfileIK("Vehicle_Expansion250NWheel", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/hatchback_02/Hatchback_02_wheel.anm");
		
		pType.AddItemInHandsProfileIK("Vehicle_ExpansionOldBikeWheel", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/hatchback_02/Hatchback_02_wheel.anm");
		
		pType.AddItemInHandsProfileIK("Vehicle_ExpansionMerlinFrontWheel", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/hatchback_02/Hatchback_02_wheel.anm");
		pType.AddItemInHandsProfileIK("Vehicle_ExpansionMerlinBackWheel", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", pBehavior, "dz/anims/anm/player/ik/vehicles/hatchback_02/Hatchback_02_wheel.anm");
	}

	override void RegisterPistol( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior )
	{
		super.RegisterPistol( pType, pBehavior );
	}

	override void RegisterFireArms( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior )
	{
		super.RegisterFireArms( pType, pBehavior );
	}

	override void RegisterArcheryItem( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior )
	{
		super.RegisterArcheryItem( pType, pBehavior );
	}

	override void RegisterLauncher( DayZPlayerType pType, DayzPlayerItemBehaviorCfg pBehavior )
	{
		super.RegisterLauncher( pType, pBehavior );
	}

	override void CustomBoneRemapping( DayZPlayerType pType )
	{
		super.CustomBoneRemapping( pType );
	}
}