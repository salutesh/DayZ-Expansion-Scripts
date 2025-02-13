/**
 * ExpansionHumanST.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionHumanST
{
	int m_CMD_Fall;
	int m_CMD_FallCancel;
	int m_CMD_Jump;
	int m_CMD_Land;
	int m_CMD_Swim;

	int m_CMD_Turn;
	int m_CMD_StopTurn;

	int m_CMD_WeaponFire;

	//int m_CMD_Expansion_Parachute_Pull;
	//int m_CMD_Expansion_Parachute_Cut;

	int m_CMD_Vehicle_GetIn;
	int m_CMD_Vehicle_SwitchSeat;
	int m_CMD_Vehicle_GetOut;
	int m_CMD_Vehicle_ClimbOut;
	int m_CMD_Vehicle_CrawlOut;
	int m_CMD_Vehicle_JumpOut;

	//int m_CMD_Guitar;
	//int m_CMD_Guitar_Stop;

	int m_VAR_MovementSpeed;
	int m_VAR_MovementDirection;

	int m_VAR_TurnAmount;

	int m_VAR_WaterLevel;

	int m_EVT_LandEarlyExit;

	int m_VAR_VehicleType;
	int m_VAR_VehicleSteering;
	int m_VAR_VehicleThrottle;
	int m_VAR_VehicleClutch;
	int m_VAR_VehicleBrake;
	int m_VAR_VehicleAccelerationFB;
	int m_VAR_VehicleAccelerationLR;

	int m_EVT_LeaveVehicle;
	int m_EVT_LandVehicle;

	int m_VAR_Look;
	int m_VAR_LookDirX;
	int m_VAR_LookDirY;

	//int m_VAR_GuitarChord;
	//int m_VAR_GuitarFret;

	int m_VAR_AimX;
	int m_VAR_AimY;

	int m_VAR_Raised;
	int m_VAR_ADS;
	int m_VAR_Stance;

	int m_VAR_Lean;

	int m_TAG_WeaponFire;

	void ExpansionHumanST( Human human )
	{
		HumanAnimInterface hai = human.GetAnimInterface();

		m_CMD_Fall = hai.BindCommand( "CMD_Fall" );
		m_CMD_FallCancel = hai.BindCommand( "CMD_FallCancel" );
		m_CMD_Jump = hai.BindCommand( "CMD_Jump" );
		m_CMD_Land = hai.BindCommand( "CMD_Land" );
		m_CMD_Swim = hai.BindCommand( "CMD_Swim" );

	#ifdef EXPANSIONMODAI
		m_CMD_Turn = hai.BindCommand( "CMD_eAI_Turn" );
		m_CMD_StopTurn = hai.BindCommand( "CMD_eAI_StopTurn" );
	#endif
		
		m_CMD_WeaponFire = hai.BindCommand( "CMD_WeaponFire" );

		//m_CMD_Expansion_Parachute_Pull = hai.BindCommand( "CMD_Expansion_Parachute_Pull" );
		//m_CMD_Expansion_Parachute_Cut = hai.BindCommand( "CMD_Expansion_Parachute_Cut" );

		m_CMD_Vehicle_GetIn = hai.BindCommand( "CMD_Vehicle_GetIn" );
		m_CMD_Vehicle_SwitchSeat = hai.BindCommand( "CMD_Vehicle_SwitchSeat" );
		m_CMD_Vehicle_GetOut = hai.BindCommand( "CMD_Vehicle_GetOut" );
		m_CMD_Vehicle_ClimbOut = hai.BindCommand( "CMD_Vehicle_ClimbOut" );
		m_CMD_Vehicle_CrawlOut = hai.BindCommand( "CMD_Vehicle_CrawlOut" );
		m_CMD_Vehicle_JumpOut = hai.BindCommand( "CMD_Vehicle_JumpOut" );

		//m_CMD_Guitar = hai.BindCommand( "CMD_Guitar" );
		//m_CMD_Guitar_Stop = hai.BindCommand( "CMD_Guitar_Stop" );

		m_VAR_MovementSpeed = hai.BindVariableFloat( "MovementSpeed" );
		m_VAR_MovementDirection = hai.BindVariableFloat( "MovementDirection" );

	#ifdef EXPANSIONMODAI
		m_VAR_TurnAmount = hai.BindVariableFloat( "eAI_TurnAmount" );
	#endif

		m_VAR_WaterLevel = hai.BindVariableFloat( "WaterLevel" );

		m_EVT_LandEarlyExit = hai.BindEvent( "LandEarlyExit" );

		m_VAR_VehicleType = hai.BindVariableInt( "VehicleType" );
		m_VAR_VehicleSteering = hai.BindVariableFloat( "VehicleSteering" );
		m_VAR_VehicleThrottle = hai.BindVariableFloat( "VehicleThrottle" );
		m_VAR_VehicleClutch = hai.BindVariableFloat( "VehicleClutch" );
		m_VAR_VehicleBrake = hai.BindVariableBool( "VehicleBrake" );
		m_VAR_VehicleAccelerationFB = hai.BindVariableFloat( "VehicleAccelerationFB" );
		m_VAR_VehicleAccelerationLR = hai.BindVariableFloat( "VehicleAccelerationLR" );

		m_EVT_LeaveVehicle = hai.BindEvent( "LeaveVehicle" );
		m_EVT_LandVehicle = hai.BindEvent( "LandVehicle" );

		m_VAR_Look = hai.BindVariableBool( "Look" );
	#ifdef EXPANSIONMODAI
		m_VAR_LookDirX = hai.BindVariableFloat( "eAI_LookDirX" );
		m_VAR_LookDirY = hai.BindVariableFloat( "eAI_LookDirY" );
	#endif

		//m_VAR_GuitarChord = hai.BindVariableInt( "GuitarChord" );
		//m_VAR_GuitarFret = hai.BindVariableFloat( "GuitarFret" );

	#ifdef EXPANSIONMODAI
		m_VAR_AimX = hai.BindVariableFloat( "eAI_AimX" );
		m_VAR_AimY = hai.BindVariableFloat( "eAI_AimY" );

		m_VAR_Raised = hai.BindVariableBool( "eAI_Raised" );
	#endif

		m_VAR_ADS = hai.BindVariableBool( "ADS" );
		m_VAR_Stance = hai.BindVariableInt( "Stance" );

	#ifdef EXPANSIONMODAI
		m_VAR_Lean = hai.BindVariableFloat( "eAI_Lean" );
	#endif

		m_TAG_WeaponFire = hai.BindTag("TagWeaponFire");
	}

	void CallFall( HumanCommandScript script, int pParamInt = 0, float pParamFloat = 0 )
	{
		script.PreAnim_CallCommand( m_CMD_Fall, pParamInt, pParamFloat );
	}

	void CallFall( DayZPlayer player, int pParamInt = 0, float pParamFloat = 0 )
	{
		player.AnimCallCommand( m_CMD_Fall, pParamInt, pParamFloat );
	}

	void CallFallCancel( HumanCommandScript script, int pParamInt = 0, float pParamFloat = 0 )
	{
		script.PreAnim_CallCommand( m_CMD_FallCancel, pParamInt, pParamFloat );
	}

	void CallFallCancel( DayZPlayer player, int pParamInt = 0, float pParamFloat = 0 )
	{
		player.AnimCallCommand( m_CMD_FallCancel, pParamInt, pParamFloat );
	}

	void CallJump( HumanCommandScript script, int pParamInt = 0, float pParamFloat = 0 )
	{
		script.PreAnim_CallCommand( m_CMD_Jump, pParamInt, pParamFloat );
	}

	void CallJump( DayZPlayer player, int pParamInt = 0, float pParamFloat = 0 )
	{
		player.AnimCallCommand( m_CMD_Jump, pParamInt, pParamFloat );
	}

	void CallLand( HumanCommandScript script, int pParamInt = 0, float pParamFloat = 0 )
	{
		script.PreAnim_CallCommand( m_CMD_Land, pParamInt, pParamFloat );
	}

	void CallLand( DayZPlayer player, int pParamInt = 0, float pParamFloat = 0 )
	{
		player.AnimCallCommand( m_CMD_Land, pParamInt, pParamFloat );
	}

	void CallSwim( HumanCommandScript script, int pParamInt = 0, float pParamFloat = 0 )
	{
		script.PreAnim_CallCommand( m_CMD_Swim, pParamInt, pParamFloat );
	}

	void CallSwim( DayZPlayer player, int pParamInt = 0, float pParamFloat = 0 )
	{
		player.AnimCallCommand( m_CMD_Swim, pParamInt, pParamFloat );
	}

	void CallTurn( HumanCommandScript script, int pParamInt = 0, float pParamFloat = 0 )
	{
		script.PreAnim_CallCommand( m_CMD_Turn, pParamInt, pParamFloat );
	}

	void CallTurn( DayZPlayer player, int pParamInt = 0, float pParamFloat = 0 )
	{
		player.AnimCallCommand( m_CMD_Turn, pParamInt, pParamFloat );
	}

	void CallStopTurn( HumanCommandScript script, int pParamInt = 0, float pParamFloat = 0 )
	{
		script.PreAnim_CallCommand( m_CMD_StopTurn, pParamInt, pParamFloat );
	}

	void CallStopTurn( DayZPlayer player, int pParamInt = 0, float pParamFloat = 0 )
	{
		player.AnimCallCommand( m_CMD_StopTurn, pParamInt, pParamFloat );
	}

	void CallWeaponFire( HumanCommandScript script, int pParamInt = 0, float pParamFloat = 0 )
	{
		script.PreAnim_CallCommand( m_CMD_WeaponFire, pParamInt, pParamFloat );
	}

	void CallWeaponFire( DayZPlayer player, int pParamInt = 0, float pParamFloat = 0 )
	{
		player.AnimCallCommand( m_CMD_WeaponFire, pParamInt, pParamFloat );
	}

	//void CallParachutePull( HumanCommandScript script, int pParamInt = 0, float pParamFloat = 0 )
	//{
	//	script.PreAnim_CallCommand( m_CMD_Expansion_Parachute_Pull, pParamInt, pParamFloat );
	//}

	//void CallParachuteCut( HumanCommandScript script, int pParamInt = 0, float pParamFloat = 0 )
	//{
	//	script.PreAnim_CallCommand( m_CMD_Expansion_Parachute_Cut, pParamInt, pParamFloat );
	//}

	void CallVehicleGetIn( HumanCommandScript script, int pParamInt = 0, float pParamFloat = 0 )
	{
		script.PreAnim_CallCommand( m_CMD_Vehicle_GetIn, pParamInt, pParamFloat );
	}

	void CallVehicleGetIn( DayZPlayer player, int pParamInt = 0, float pParamFloat = 0 )
	{
		player.AnimCallCommand( m_CMD_Vehicle_GetIn, pParamInt, pParamFloat );
	}

	void CallVehicleSwitchSeat( HumanCommandScript script, int pParamInt = 0, float pParamFloat = 0 )
	{
		script.PreAnim_CallCommand( m_CMD_Vehicle_SwitchSeat, pParamInt, pParamFloat );
	}

	void CallVehicleSwitchSeat( DayZPlayer player, int pParamInt = 0, float pParamFloat = 0 )
	{
		player.AnimCallCommand( m_CMD_Vehicle_SwitchSeat, pParamInt, pParamFloat );
	}

	void CallVehicleGetOut( HumanCommandScript script, int pParamInt = 0, float pParamFloat = 0 )
	{
		script.PreAnim_CallCommand( m_CMD_Vehicle_GetOut, pParamInt, pParamFloat );
	}

	void CallVehicleGetOut( DayZPlayer player, int pParamInt = 0, float pParamFloat = 0 )
	{
		player.AnimCallCommand( m_CMD_Vehicle_GetOut, pParamInt, pParamFloat );
	}

	void CallVehicleClimbOut( HumanCommandScript script, int pParamInt = 0, float pParamFloat = 0 )
	{
		script.PreAnim_CallCommand( m_CMD_Vehicle_ClimbOut, pParamInt, pParamFloat );
	}

	void CallVehicleClimbOut( DayZPlayer player, int pParamInt = 0, float pParamFloat = 0 )
	{
		player.AnimCallCommand( m_CMD_Vehicle_ClimbOut, pParamInt, pParamFloat );
	}

	void CallVehicleCrawlOut( HumanCommandScript script, int pParamInt = 0, float pParamFloat = 0 )
	{
		script.PreAnim_CallCommand( m_CMD_Vehicle_CrawlOut, pParamInt, pParamFloat );
	}

	void CallVehicleCrawlOut( DayZPlayer player, int pParamInt = 0, float pParamFloat = 0 )
	{
		player.AnimCallCommand( m_CMD_Vehicle_CrawlOut, pParamInt, pParamFloat );
	}

	void CallVehicleJumpOut( HumanCommandScript script, int pParamInt = 0, float pParamFloat = 0 )
	{
		script.PreAnim_CallCommand( m_CMD_Vehicle_JumpOut, pParamInt, pParamFloat );
	}

	void CallVehicleJumpOut( DayZPlayer player, int pParamInt = 0, float pParamFloat = 0 )
	{
		player.AnimCallCommand( m_CMD_Vehicle_JumpOut, pParamInt, pParamFloat );
	}

	//void CallGuitar( HumanCommandScript script, int pParamInt = 0, float pParamFloat = 0 )
	//{
	//	script.PreAnim_CallCommand( m_CMD_Guitar, pParamInt, pParamFloat );
	//}

	//void CallGuitarStop( HumanCommandScript script, int pParamInt = 0, float pParamFloat = 0 )
	//{
	//	script.PreAnim_CallCommand( m_CMD_Guitar_Stop, pParamInt, pParamFloat );
	//}

	void SetMovementSpeed( HumanCommandScript script, float param )
	{
		script.PreAnim_SetFloat( m_VAR_MovementSpeed, param );
	}

	void SetMovementSpeed( DayZPlayer player, float param )
	{
		player.AnimSetFloat( m_VAR_MovementSpeed, param );
	}

	void SetMovementDirection( HumanCommandScript script, float param )
	{
		script.PreAnim_SetFloat( m_VAR_MovementDirection, param );
	}

	void SetMovementDirection( DayZPlayer player, float param )
	{
		player.AnimSetFloat( m_VAR_MovementDirection, param );
	}

	void SetTurnAmount( HumanCommandScript script, float param )
	{
		script.PreAnim_SetFloat( m_VAR_TurnAmount, param );
	}

	void SetTurnAmount( DayZPlayer player, float param )
	{
		player.AnimSetFloat( m_VAR_TurnAmount, param );
	}

	void SetWaterLevel( HumanCommandScript script, float param )
	{
		script.PreAnim_SetFloat( m_VAR_WaterLevel, param );
	}

	void SetWaterLevel( DayZPlayer player, float param )
	{
		player.AnimSetFloat( m_VAR_WaterLevel, param );
	}

	bool IsLandEarlyExit( HumanCommandScript script )
	{
		return script.PrePhys_IsEvent( m_EVT_LandEarlyExit );
	}

	void SetVehicleType( HumanCommandScript script, int param )
	{
		script.PreAnim_SetInt( m_VAR_VehicleType, param );
	}

	void SetVehicleType( DayZPlayer player, int param )
	{
		player.AnimSetInt( m_VAR_VehicleType, param );
	}

	void SetVehicleSteering( HumanCommandScript script, float param )
	{
		script.PreAnim_SetFloat( m_VAR_VehicleSteering, param );
	}

	void SetVehicleSteering( DayZPlayer player, float param )
	{
		player.AnimSetFloat( m_VAR_VehicleSteering, param );
	}

	void SetVehicleThrottle( HumanCommandScript script, float param )
	{
		script.PreAnim_SetFloat( m_VAR_VehicleThrottle, param );
	}

	void SetVehicleThrottle( DayZPlayer player, float param )
	{
		player.AnimSetFloat( m_VAR_VehicleThrottle, param );
	}
	
	void SetVehicleClutch( HumanCommandScript script, float param )
	{
		script.PreAnim_SetFloat( m_VAR_VehicleClutch, param );
	}
	
	void SetVehicleClutch( DayZPlayer player, float param )
	{
		player.AnimSetFloat( m_VAR_VehicleClutch, param );
	}
	
	void SetVehicleBrake( HumanCommandScript script, bool param )
	{
		script.PreAnim_SetBool( m_VAR_VehicleBrake, param );
	}
	
	void SetVehicleBrake( DayZPlayer player, bool param )
	{
		player.AnimSetBool( m_VAR_VehicleBrake, param );
	}
	
	void SetVehicleAccelerationFB( HumanCommandScript script, float param )
	{
		script.PreAnim_SetFloat( m_VAR_VehicleAccelerationFB, param );
	}
	
	void SetVehicleAccelerationFB( DayZPlayer player, float param )
	{
		player.AnimSetFloat( m_VAR_VehicleAccelerationFB, param );
	}
	
	void SetVehicleAccelerationLR( HumanCommandScript script, float param )
	{
		script.PreAnim_SetFloat( m_VAR_VehicleAccelerationLR, param );
	}
	
	void SetVehicleAccelerationLR( DayZPlayer player, float param )
	{
		player.AnimSetFloat( m_VAR_VehicleAccelerationLR, param );
	}

	bool IsLeaveVehicle( HumanCommandScript script )
	{
		return script.PrePhys_IsEvent( m_EVT_LeaveVehicle );
	}

	bool IsLandVehicle( HumanCommandScript script )
	{
		return script.PrePhys_IsEvent( m_EVT_LandVehicle );
	}

	void SetLook( HumanCommandScript script, bool param )
	{
		script.PreAnim_SetBool( m_VAR_Look, param );
	}

	void SetLook( DayZPlayer player, bool param )
	{
		player.AnimSetBool( m_VAR_Look, param );
	}

	void SetLookDirX( HumanCommandScript script, float param )
	{
		script.PreAnim_SetFloat( m_VAR_LookDirX, param );
	}

	void SetLookDirX( DayZPlayer player, float param )
	{
		player.AnimSetFloat( m_VAR_LookDirX, param );
	}

	void SetLookDirY( HumanCommandScript script, float param )
	{
		script.PreAnim_SetFloat( m_VAR_LookDirY, param );
	}

	void SetLookDirY( DayZPlayer player, float param )
	{
		player.AnimSetFloat( m_VAR_LookDirY, param );
	}

	//void SetGuitarChord( HumanCommandScript script, int param )
	//{
	//	script.PreAnim_SetInt( m_VAR_GuitarChord, param );
	//}

	//void SetGuitarFret( HumanCommandScript script, float param )
	//{
	//	script.PreAnim_SetFloat( m_VAR_GuitarFret, param );
	//}

	void SetAimX( HumanCommandScript script, float param )
	{
		script.PreAnim_SetFloat( m_VAR_AimX, param );
	}

	void SetAimX( DayZPlayer player, float param )
	{
		player.AnimSetFloat( m_VAR_AimX, param );
	}

	void SetAimY( HumanCommandScript script, float param )
	{
		script.PreAnim_SetFloat( m_VAR_AimY, param );
	}

	void SetAimY( DayZPlayer player, float param )
	{
		player.AnimSetFloat( m_VAR_AimY, param );
	}

	void SetRaised( HumanCommandScript script, bool param )
	{
		script.PreAnim_SetBool( m_VAR_Raised, param );
	}

	void SetRaised( DayZPlayer player, bool param )
	{
		player.AnimSetBool( m_VAR_Raised, param );
	}

	void SetADS( HumanCommandScript script, bool param )
	{
		script.PreAnim_SetBool( m_VAR_ADS, param );
	}

	void SetADS( DayZPlayer player, bool param )
	{
		player.AnimSetBool( m_VAR_ADS, param );
	}

	void SetStance( HumanCommandScript script, int param )
	{
		script.PreAnim_SetInt( m_VAR_Stance, param );
	}

	void SetStance( DayZPlayer player, int param )
	{
		player.AnimSetInt( m_VAR_Stance, param );
	}

	void SetLean( HumanCommandScript script, float param )
	{
		script.PreAnim_SetFloat( m_VAR_Lean, param );
	}

	void SetLean( DayZPlayer player, float param )
	{
		player.AnimSetFloat( m_VAR_Lean, param );
	}
};
