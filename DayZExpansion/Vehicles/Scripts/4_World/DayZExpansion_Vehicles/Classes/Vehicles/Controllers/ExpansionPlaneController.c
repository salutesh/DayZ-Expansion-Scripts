/**
 * ExpansionPlaneController.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionPlaneController
 * @brief		This class handle player controls to move planes
 **/
class ExpansionPlaneController: ExpansionVehicleController
{
	private float m_Engine;

	private float m_Flaps;

	private float m_AileronLeft;
	private float m_AileronRight;

	private float m_ElevatorUp;
	private float m_ElevatorDown;

	private float m_RudderLeft;
	private float m_RudderRight;

	private ExpansionVehiclePlaneBase m_Plane;

	void SetEngine( float value )
	{
		m_Engine = value;
	}

	float GetEngine()
	{
		return m_Engine;
	}

	void SetAileron( float left, float right, float value )
	{
		m_AileronLeft = left;
		m_AileronRight = right;
	}

	float GetAileronLeft()
	{
		return m_AileronLeft;
	}

	float GetAileronRight()
	{
		return m_AileronRight;
	}

	void SetElevator( float up, float down, float value )
	{
		m_ElevatorUp = up;
		m_ElevatorDown = down;
	}

	float GetElevatorUp()
	{
		return m_ElevatorUp;
	}

	float GetElevatorDown()
	{
		return m_ElevatorDown;
	}

	void SetRudder( float left, float right, float value )
	{
		m_RudderLeft = left;
		m_RudderRight = right;
	}

	float GetRudderLeft()
	{
		return m_RudderLeft;
	}

	float GetRudderRight()
	{
		return m_RudderRight;
	}

	void SetFlaps( float flaps )
	{
		m_Flaps = flaps;
	}

	float GetFlaps()
	{
		return m_Flaps;
	}

	protected override void OnUpdate()
	{
		super.OnUpdate();

		bool value = false;

		value = false;
		GetInputPress( "UAExpansionPlaneFlapsUp", value );
		if ( value )
			m_Flaps += 0.5;

		value = false;
		GetInputPress( "UAExpansionPlaneFlapsDown", value );
		if ( value )
			m_Flaps -= 0.5;

		m_Flaps = Math.Clamp( m_Flaps, 0.0, 1.0 );
			
		GetInputValue( "UAExpansionPlaneEngine", m_Engine );
		GetInputValue( "UAExpansionPlaneAileronLeft", m_AileronLeft );
		GetInputValue( "UAExpansionPlaneAileronRight", m_AileronRight );
		GetInputValue( "UAExpansionPlaneElevatorUp", m_ElevatorUp );
		GetInputValue( "UAExpansionPlaneElevatorDown", m_ElevatorDown );
		GetInputValue( "UAExpansionPlaneRudderLeft", m_RudderLeft );
		GetInputValue( "UAExpansionPlaneRudderRight", m_RudderRight );
	}

	protected override void OnReset()
	{
		super.OnReset();
		
		m_Engine = 0;
		m_AileronLeft = 0;
		m_AileronRight = 0;
		m_ElevatorUp = 0;
		m_ElevatorDown = 0;
		m_RudderLeft = 0;
		m_RudderRight = 0;

		// don't reset flaps
	}

	protected override void OnNetworkSend(  ParamsWriteContext ctx )
	{
		ctx.Write( m_Engine );

		ctx.Write( m_Flaps );

		ctx.Write( m_AileronLeft );
		ctx.Write( m_AileronRight );

		ctx.Write( m_ElevatorUp );
		ctx.Write( m_ElevatorDown );

		ctx.Write( m_RudderLeft );
		ctx.Write( m_RudderRight );
	}

	protected override bool OnNetworkRecieve( ParamsReadContext ctx )
	{
		ctx.Read( m_Engine );

		ctx.Read( m_Flaps );

		ctx.Read( m_AileronLeft );
		ctx.Read( m_AileronRight );

		ctx.Read( m_ElevatorUp );
		ctx.Read( m_ElevatorDown );

		ctx.Read( m_RudderLeft );
		ctx.Read( m_RudderRight );

		return true;
	}
}