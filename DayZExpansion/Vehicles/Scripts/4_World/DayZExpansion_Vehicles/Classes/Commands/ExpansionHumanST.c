/**
 * ExpansionHumanST.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionHumanST
{
	private int m_CMD_Fall;
	private int m_CMD_Jump;
	private int m_CMD_Land;

	private int m_CMD_Expansion_Parachute_Pull;
	private int m_CMD_Expansion_Parachute_Cut;

	private int m_CMD_Vehicle_GetIn;
	private int m_CMD_Vehicle_SwitchSeat;
	private int m_CMD_Vehicle_GetOut;
	private int m_CMD_Vehicle_ClimbOut;
	private int m_CMD_Vehicle_CrawlOut;
	private int m_CMD_Vehicle_JumpOut;

	private int m_CMD_Guitar;
	private int m_CMD_Guitar_Stop;

	private int m_VAR_MovementSpeed;
	private int m_VAR_MovementDirection;

	private int m_EVT_LandEarlyExit;

	private int m_VAR_VehicleType;
	private int m_VAR_VehicleSteering;
	private int m_VAR_VehicleThrottle;
	private int m_VAR_VehicleClutch;
	private int m_VAR_VehicleBrake;
	private int m_VAR_VehicleAccelerationFB;
	private int m_VAR_VehicleAccelerationLR;

	private int m_VAR_Look;
	private int m_VAR_LookDirX;
	private int m_VAR_LookDirY;

	private int m_VAR_GuitarChord;
	private int m_VAR_GuitarFret;

	void ExpansionHumanST( Human human )
	{
		HumanAnimInterface hai = human.GetAnimInterface();

		m_CMD_Fall = hai.BindCommand( "CMD_Fall" );
		m_CMD_Jump = hai.BindCommand( "CMD_Jump" );
		m_CMD_Land = hai.BindCommand( "CMD_Land" );
		
		m_CMD_Expansion_Parachute_Pull = hai.BindCommand( "CMD_Expansion_Parachute_Pull" );
		m_CMD_Expansion_Parachute_Cut = hai.BindCommand( "CMD_Expansion_Parachute_Cut" );

		m_CMD_Vehicle_GetIn = hai.BindCommand( "CMD_Vehicle_GetIn" );
		m_CMD_Vehicle_SwitchSeat = hai.BindCommand( "CMD_Vehicle_SwitchSeat" );
		m_CMD_Vehicle_GetOut = hai.BindCommand( "CMD_Vehicle_GetOut" );
		m_CMD_Vehicle_ClimbOut = hai.BindCommand( "CMD_Vehicle_ClimbOut" );
		m_CMD_Vehicle_CrawlOut = hai.BindCommand( "CMD_Vehicle_CrawlOut" );
		m_CMD_Vehicle_JumpOut = hai.BindCommand( "CMD_Vehicle_JumpOut" );

		m_CMD_Guitar = hai.BindCommand( "CMD_Guitar" );
		m_CMD_Guitar_Stop = hai.BindCommand( "CMD_Guitar_Stop" );

		m_VAR_MovementSpeed = hai.BindVariableFloat( "MovementSpeed" );
		m_VAR_MovementDirection = hai.BindVariableFloat( "MovementDirection" );

		m_EVT_LandEarlyExit = hai.BindEvent( "LandEarlyExit" );

		m_VAR_VehicleType = hai.BindVariableInt( "VehicleType" );
		m_VAR_VehicleSteering = hai.BindVariableFloat( "VehicleSteering" );
		m_VAR_VehicleThrottle = hai.BindVariableFloat( "VehicleThrottle" );
		m_VAR_VehicleClutch = hai.BindVariableFloat( "VehicleClutch" );
		m_VAR_VehicleBrake = hai.BindVariableBool( "VehicleBrake" );
		m_VAR_VehicleAccelerationFB = hai.BindVariableFloat( "VehicleAccelerationFB" );
		m_VAR_VehicleAccelerationLR = hai.BindVariableFloat( "VehicleAccelerationLR" );

		m_VAR_Look = hai.BindVariableBool( "Look" );
		m_VAR_LookDirX = hai.BindVariableFloat( "LookDirX" );
		m_VAR_LookDirY = hai.BindVariableFloat( "LookDirY" );

		m_VAR_GuitarChord = hai.BindVariableInt( "GuitarChord" );
		m_VAR_GuitarFret = hai.BindVariableFloat( "GuitarFret" );
	}

	void CallFall( HumanCommandScript script, int pParamInt = 0, float pParamFloat = 0 )
	{
		script.PreAnim_CallCommand( m_CMD_Fall, pParamInt, pParamFloat );
	}

	void CallJump( HumanCommandScript script, int pParamInt = 0, float pParamFloat = 0 )
	{
		script.PreAnim_CallCommand( m_CMD_Jump, pParamInt, pParamFloat );
	}

	void CallLand( HumanCommandScript script, int pParamInt = 0, float pParamFloat = 0 )
	{
		script.PreAnim_CallCommand( m_CMD_Land, pParamInt, pParamFloat );
	}

	void CallParachutePull( HumanCommandScript script, int pParamInt = 0, float pParamFloat = 0 )
	{
		script.PreAnim_CallCommand( m_CMD_Expansion_Parachute_Pull, pParamInt, pParamFloat );
	}

	void CallParachuteCut( HumanCommandScript script, int pParamInt = 0, float pParamFloat = 0 )
	{
		script.PreAnim_CallCommand( m_CMD_Expansion_Parachute_Cut, pParamInt, pParamFloat );
	}

	void CallVehicleGetIn( HumanCommandScript script, int pParamInt = 0, float pParamFloat = 0 )
	{
		script.PreAnim_CallCommand( m_CMD_Vehicle_GetIn, pParamInt, pParamFloat );
	}

	void CallVehicleSwitchSeat( HumanCommandScript script, int pParamInt = 0, float pParamFloat = 0 )
	{
		script.PreAnim_CallCommand( m_CMD_Vehicle_SwitchSeat, pParamInt, pParamFloat );
	}

	void CallVehicleGetOut( HumanCommandScript script, int pParamInt = 0, float pParamFloat = 0 )
	{
		script.PreAnim_CallCommand( m_CMD_Vehicle_GetOut, pParamInt, pParamFloat );
	}

	void CallVehicleClimbOut( HumanCommandScript script, int pParamInt = 0, float pParamFloat = 0 )
	{
		script.PreAnim_CallCommand( m_CMD_Vehicle_ClimbOut, pParamInt, pParamFloat );
	}

	void CallVehicleCrawlOut( HumanCommandScript script, int pParamInt = 0, float pParamFloat = 0 )
	{
		script.PreAnim_CallCommand( m_CMD_Vehicle_CrawlOut, pParamInt, pParamFloat );
	}

	void CallVehicleJumpOut( HumanCommandScript script, int pParamInt = 0, float pParamFloat = 0 )
	{
		script.PreAnim_CallCommand( m_CMD_Vehicle_JumpOut, pParamInt, pParamFloat );
	}

	void CallGuitar( HumanCommandScript script, int pParamInt = 0, float pParamFloat = 0 )
	{
		script.PreAnim_CallCommand( m_CMD_Guitar, pParamInt, pParamFloat );
	}

	void CallGuitarStop( HumanCommandScript script, int pParamInt = 0, float pParamFloat = 0 )
	{
		script.PreAnim_CallCommand( m_CMD_Guitar_Stop, pParamInt, pParamFloat );
	}

	void SetMovementSpeed( HumanCommandScript script, float param )
	{
		script.PreAnim_SetFloat( m_VAR_MovementSpeed, param );
	}

	void SetMovementDirection( HumanCommandScript script, float param )
	{
		script.PreAnim_SetFloat( m_VAR_MovementDirection, param );
	}

	bool IsLandEarlyExit( HumanCommandScript script )
	{
		return script.PrePhys_IsEvent( m_EVT_LandEarlyExit );
	}

	void SetVehicleType( HumanCommandScript script, int param )
	{
		script.PreAnim_SetInt( m_VAR_VehicleType, param );
	}

	void SetVehicleSteering( HumanCommandScript script, float param )
	{
		script.PreAnim_SetFloat( m_VAR_VehicleSteering, param );
	}

	void SetVehicleThrottle( HumanCommandScript script, float param )
	{
		script.PreAnim_SetFloat( m_VAR_VehicleThrottle, param );
	}
	
	void SetVehicleClutch( HumanCommandScript script, float param )
	{
		script.PreAnim_SetFloat( m_VAR_VehicleClutch, param );
	}
	
	void SetVehicleBrake( HumanCommandScript script, bool param )
	{
		script.PreAnim_SetBool( m_VAR_VehicleBrake, param );
	}
	
	void SetVehicleAccelerationFB( HumanCommandScript script, float param )
	{
		script.PreAnim_SetFloat( m_VAR_VehicleAccelerationFB, param );
	}
	
	void SetVehicleAccelerationLR( HumanCommandScript script, float param )
	{
		script.PreAnim_SetFloat( m_VAR_VehicleAccelerationLR, param );
	}

	void SetLook( HumanCommandScript script, bool param )
	{
		script.PreAnim_SetBool( m_VAR_Look, param );
	}

	void SetLookDirX( HumanCommandScript script, float param )
	{
		script.PreAnim_SetFloat( m_VAR_LookDirX, param );
	}

	void SetLookDirY( HumanCommandScript script, float param )
	{
		script.PreAnim_SetFloat( m_VAR_LookDirY, param );
	}

	void SetGuitarChord( HumanCommandScript script, int param )
	{
		script.PreAnim_SetInt( m_VAR_GuitarChord, param );
	}

	void SetGuitarFret( HumanCommandScript script, float param )
	{
		script.PreAnim_SetFloat( m_VAR_GuitarFret, param );
	}
}