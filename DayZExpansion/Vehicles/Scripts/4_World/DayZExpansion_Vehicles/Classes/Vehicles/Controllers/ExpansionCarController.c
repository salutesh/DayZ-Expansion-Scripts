/**
 * ExpansionCarController.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionCarController
 * @brief		This class handle player controls to move boats
 **/
class ExpansionCarController: ExpansionVehicleController
{
	private int m_Gear;

	private float m_Forward;
	private float m_Backward;
	
	private float m_Gentle;
	private float m_Turbo;

	private float m_TurnLeft;
	private float m_TurnRight;

	private ExpansionVehicleCarBase m_Car;

	void ExpansionCarController( EntityAI vehicle )
	{
		Class.CastTo( m_Car, vehicle );
	}
	
	void SetGear( int gear )
	{
		m_Gear = gear;
	}

	int GetGear()
	{
		return m_Gear;
	}

	void SetForward( float forward, float backward, float gentle = 0, float turbo = 0 )
	{
		m_Forward = forward;
		m_Backward = backward;
		m_Gentle = gentle;
		m_Turbo = turbo;
	}

	float GetForward()
	{
		return m_Forward;
	}

	float GetBackward()
	{
		return m_Backward;
	}

	float GetGentle()
	{
		return m_Gentle;
	}

	float GetTurbo()
	{
		return m_Turbo;
	}

	void SetTurn( float left, float right, float value )
	{
		m_TurnLeft = left;
		m_TurnRight = right;
	}

	float GetTurnLeft()
	{
		return m_TurnLeft;
	}

	float GetTurnRight()
	{
		return m_TurnRight;
	}

	// ------------------------------------------------------------
	//! OnUpdate
	// ------------------------------------------------------------
	protected override void OnUpdate()
	{
		bool gear_up_press;
		bool gear_down_press;
		
		int gear = m_Gear;
		
		GetInputPress( "UACarShiftGearUp", gear_up_press );
		GetInputPress( "UACarShiftGearDown", gear_down_press );
		
		if ( gear_up_press )
		{
			m_Gear += 1;
		}
		if ( gear_down_press )
		{
			m_Gear -= 1;
		}
		
		if ( m_Gear < 0 )
			m_Gear = 0;
		if ( m_Gear >= m_Car.GetGearsCount() )
			m_Gear = m_Car.GetGearsCount();
		
		if ( gear != m_Gear )
		{
			m_ECommand.SignalGearChange();
		}
		
		GetInputValue( "UACarForward", m_Forward );
		GetInputValue( "UACarBack", m_Backward );
		GetInputValue( "UACarLeft", m_TurnLeft );
		GetInputValue( "UACarRight", m_TurnRight );
		GetInputValue( "UAVehicleSlow", m_Gentle );
		GetInputValue( "UAVehicleTurbo", m_Turbo );
	}

	protected override void OnReset()
	{
		super.OnReset();

		m_Gear = 1;
		m_Forward = 0;
		m_Backward = 0;
		m_TurnLeft = 0;
		m_TurnRight = 0;
		m_Gentle = 0;
		m_Turbo = 0;
	}

	protected override void OnNetworkSend( ref ParamsWriteContext ctx )
	{
		ctx.Write( m_Gear );

		ctx.Write( m_Forward );
		ctx.Write( m_Backward );

		ctx.Write( m_Gentle );
		ctx.Write( m_Turbo );

		ctx.Write( m_TurnLeft );
		ctx.Write( m_TurnRight );
	}

	protected override bool OnNetworkRecieve( ref ParamsReadContext ctx )
	{
		ctx.Read( m_Gear );

		ctx.Read( m_Forward );
		ctx.Read( m_Backward );
		
		ctx.Read( m_Gentle );
		ctx.Read( m_Turbo );

		ctx.Read( m_TurnLeft );
		ctx.Read( m_TurnRight );

		return true;
	}
}