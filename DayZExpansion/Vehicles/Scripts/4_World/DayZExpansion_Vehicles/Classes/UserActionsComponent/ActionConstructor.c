/**
 * ActionConstructor.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ActionConstructor
 * @brief		
 **/
modded class ActionConstructor
{
	// ------------------------------------------------------------
	// RegisterActions
	// ------------------------------------------------------------
	override void RegisterActions( TTypenameArray actions )
	{
		super.RegisterActions( actions );

		//! Vehicles
		actions.Insert( ExpansionActionGetInExpansionVehicle );
		actions.Insert( ExpansionActionGetOutExpansionVehicle );
		
		#ifdef EXPANSION_VEHICLE_TOWING
		actions.Insert( ExpansionActionConnectTow );
		actions.Insert( ExpansionActionDisconnectTow );
		#endif
		
		actions.Insert( ExpansionActionSwitchGear );
		actions.Insert( ExpansionActionPairKey );
		actions.Insert( ExpansionActionAdminUnpairKey );
		actions.Insert( ExpansionActionLockVehicle );
		actions.Insert( ExpansionActionUnlockVehicle );
		actions.Insert( ExpansionActionCarHorn );
		actions.Insert( ExpansionActionOpenVehicleDoor );
		actions.Insert( ExpansionActionCloseVehicleDoor );

		actions.Insert( ExpansionActionStartMotor );
		actions.Insert( ExpansionActionStopMotor );
		
		actions.Insert( ExpansionVehicleActionStartMotor );
		actions.Insert( ExpansionVehicleActionStopMotor );
		
		actions.Insert( ExpansionVehicleActionStartEngine );
		actions.Insert( ExpansionVehicleActionStopEngine );

		actions.Insert( ExpansionActionSwitchLights );
		actions.Insert( ExpansionActionFillFuel );

		//! Boats
		actions.Insert( ExpansionActionSwitchBoatController );
		actions.Insert( ExpansionActionSwitchBoatControllerInput );		
		
		//! Helicopters
		actions.Insert( ExpansionActionRocket );
		actions.Insert( ExpansionActionHelicopterHoverRefill );
		actions.Insert( ExpansionActionSwitchAutoHover );
		actions.Insert( ExpansionActionSwitchAutoHoverInput );
	}
};