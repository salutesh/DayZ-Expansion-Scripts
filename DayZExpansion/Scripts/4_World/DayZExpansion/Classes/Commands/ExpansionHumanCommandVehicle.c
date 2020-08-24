/**
 * ExpansionHumanCommandVehicle.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionHumanCommandVehicle extends HumanCommandScript
{
	DayZPlayerImplement m_Player;
	ExpansionHumanST m_Table;
	HumanInputController m_Input;

	Object m_Vehicle;
	ExpansionVehicleScript m_VehicleEx;
	CarScript m_VehicleVn;

	int m_SeatIndex;
	int m_SeatAnim;
	int m_VehicleType;

	bool m_NeedFinish;

	bool m_IsGettingOut;
	bool m_IsGettingIn;
	bool m_IsIn;

	float m_Time;
	vector m_SeatTransform[4];
	vector m_SeatPosition;

	vector m_StartPosition;
	vector m_StartDirection;

	int m_PreviousInteractLayers;

	private float m_TimeGetIn;
	private float m_TimeGetOut;

	void ExpansionHumanCommandVehicle( DayZPlayerImplement player, Object vehicle, int seatIdx, int seat_anim, ExpansionHumanST table )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanCommandVehicle::ExpansionHumanCommandVehicle Start");
		#endif
		m_Player = player;
		m_Table = table;

		m_Vehicle = vehicle;

		Class.CastTo( m_VehicleEx, m_Vehicle );
		Class.CastTo( m_VehicleVn, m_Vehicle );

		m_SeatIndex = seatIdx;
		m_SeatAnim = seat_anim;

		m_Input = player.GetInputController();
		
		m_TimeGetIn = 1.0;
		m_TimeGetOut = 0.5;
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanCommandVehicle::ExpansionHumanCommandVehicle End");
		#endif
	}

	override void OnActivate()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanCommandVehicle::OnActivate Start");
		#endif
		m_IsGettingIn = true;

		m_StartPosition = m_Player.GetPosition();
		m_StartDirection = m_Player.GetDirection();

		vector entryPos;
		vector entryDir;
		vector tmPlayer[ 4 ];
		vector tmTarget[ 4 ];
		vector tmLocal[ 4 ];

		if ( m_VehicleEx )
		{
			m_VehicleEx.CrewTransform( m_SeatIndex, m_SeatTransform );
			m_SeatPosition = m_SeatTransform[3];

			m_Player.GetTransformWS( tmPlayer );
			m_Vehicle.GetTransform( tmTarget );
			Math3D.MatrixInvMultiply4( tmTarget, tmPlayer, tmLocal );

			m_Player.LinkToLocalSpaceOf( m_Vehicle, m_SeatTransform );

			m_VehicleEx.CrewGetIn( m_Player, m_SeatIndex );
		} else if ( m_VehicleVn )
		{
			m_VehicleVn.CrewTransform( m_SeatIndex, m_SeatTransform );
			m_SeatPosition = m_SeatTransform[3];

			m_Player.GetTransformWS( tmPlayer );
			m_Vehicle.GetTransform( tmTarget );
			Math3D.MatrixInvMultiply4( tmTarget, tmPlayer, tmLocal );

			m_Player.LinkToLocalSpaceOf( m_Vehicle, m_SeatTransform );

			m_VehicleVn.CrewGetIn( m_Player, m_SeatIndex );
		}

		m_Table.SetVehicleType( this, m_VehicleType );
		m_Table.CallVehicleGetIn( this, m_SeatAnim );

		dBodyEnableGravity( m_Player, false );

		dBodyActive( m_Player, ActiveState.INACTIVE );

		m_Player.OnCommandExpansionVehicleStart();

		m_Time = 0;
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanCommandVehicle::OnActivate End");
		#endif
	}

	override void OnDeactivate()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanCommandVehicle::OnDeactivate Start");
		#endif

		m_Table.SetVehicleType( this, -1 );
		
		if ( m_VehicleEx )
		{
			m_VehicleEx.CrewGetOut( m_SeatIndex );

			if ( !m_VehicleEx.CanObjectAttach( m_Player ) )
				m_Player.UnlinkFromLocalSpace();
		} else if ( m_VehicleVn )
		{
			m_VehicleVn.CrewGetOut( m_SeatIndex );

			if ( !m_VehicleVn.CanObjectAttach( m_Player ) )
				m_Player.UnlinkFromLocalSpace();
		}

		dBodyActive( m_Player, ActiveState.ALWAYS_ACTIVE );
		
		dBodyEnableGravity( m_Player, true );

		m_Player.OnCommandExpansionVehicleFinish();
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanCommandVehicle::OnDeactivate End");
		#endif
	}

	override void PreAnimUpdate( float pDt )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanCommandVehicle::PreAnimUpdate Start");
		#endif

		float heading = m_Input.GetHeadingAngle();
		PreAnim_SetFilteredHeading( heading, 0.3, 180 );

		HumanCommandWeapons hcw = m_Player.GetCommandModifier_Weapons();
		float lrAngle = hcw.GetBaseAimingAngleLR();

		float aimDiff = ( heading * Math.RAD2DEG ) - lrAngle;
		//m_Input.OverrideAimChangeX( true, aimDiff * Math.DEG2RAD );

		m_Table.SetLook( this, true );
		m_Table.SetLookDirX( this, heading * Math.RAD2DEG );

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanCommandVehicle::PreAnimUpdate End");
		#endif
	}

	override void PrePhysUpdate( float pDt )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanCommandVehicle::PrePhysUpdate Start");
		#endif

		if ( m_IsGettingIn )
		{
			if ( !m_IsIn )
			{
				m_IsIn = true;
				vector seatPosWS = m_Vehicle.GetGlobalPos( m_SeatPosition );
				vector playerPosWS = m_Player.GetPosition();

				vector dir = vector.Direction( playerPosWS, seatPosWS ).Normalized();
				float distance = vector.Distance( playerPosWS, seatPosWS );
				//if ( distance > 1 * pDt )
				//	distance = 1 * pDt;

				PrePhys_SetTranslation( dir * distance * pDt );
			}
		} else if ( !m_IsGettingOut )
		{
			PrePhys_SetTranslation( vector.Zero );
		}
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanCommandVehicle::PrePhysUpdate End");
		#endif
	}

	override bool PostPhysUpdate( float pDt )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanCommandVehicle::PostPhysUpdate Start");
		#endif

		float heading = m_Input.GetHeadingAngle();
		ExpansionDebugger.Display( EXPANSION_DEBUG_PLAYER_VEHICLE_COMMAND, "PostPhys Heading: " + heading );
		vector dir = vector.Zero;
		dir[0] = Math.Cos(heading + Math.PI_HALF);
		dir[2] = Math.Sin(heading + Math.PI_HALF);
		vector playerNPosWS = m_Vehicle.GetGlobalPos( m_SeatPosition );

		vector playerPosWS = vector.Zero;
		PostPhys_GetPosition( playerPosWS );

		Matrix3 basis;
		Quaternion quat;
		Matrix3.DirectionAndUp( dir, "0 1 0", basis );
		basis.ToQuaternion( quat );

		m_Time += pDt;
		if ( m_IsGettingIn )
		{
			m_IsGettingIn = false;

			m_Player.SetPosition( m_SeatPosition );

			if ( m_Time > m_TimeGetIn )
			{
				m_IsGettingIn = false;
			}

			//PostPhys_SetPosition( playerNPosWS );
		} else if ( m_IsGettingOut )
		{
			if ( m_Time > m_TimeGetOut )
			{
				m_NeedFinish = true;
			}

			vector exitDir;
			m_VehicleEx.CrewEntryWS( m_SeatIndex, playerNPosWS, exitDir );

			//PostPhys_SetPosition( vector.Lerp( playerPosWS, playerNPosWS, 5 * pDt ) );
		}

		m_Player.SetOrigin( m_SeatPosition );

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( heading );
		#endif
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "" + quat.data[0] + ", " + quat.data[1] + ", " + quat.data[2] + ", " + quat.data[3] );
		#endif
		
		//PostPhys_SetRotation( quat.data );

		// doesn't work
		//PostPhys_LockRotation();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanCommandVehicle::PostPhysUpdate End");
		#endif

		ExpansionDebugger.Push( EXPANSION_DEBUG_PLAYER_VEHICLE_COMMAND );
		
		return m_NeedFinish == false;
	}

	void GetOut()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanCommandVehicle::GetOut Start");
		#endif
		
		m_Time = 0;
		m_IsGettingIn = false;
		m_IsGettingOut = true;

		m_Table.CallVehicleGetOut( this );

		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanCommandVehicle::GetOut End");
		#endif
	}

	void SwitchSeat( int seatIdx, int seatAnimType )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanCommandVehicle::SwitchSeat Start");
		#endif
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionHumanCommandVehicle::SwitchSeat End");
		#endif
	}

	bool IsGettingOut()
	{
		return m_IsGettingOut;
	}

	bool IsGettingIn()
	{
		return m_IsGettingIn;
	}

	bool IsSwitchSeat()
	{
		return false;
	}

	ExpansionVehicleScript GetTransport()
	{
		return m_VehicleEx;
	}

	int GetVehicleSeat()
	{
		return m_SeatIndex;
	}

	void SetVehicleType( int vehicleType )
	{
		m_VehicleType = vehicleType;
	}
}
