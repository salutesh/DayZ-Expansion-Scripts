/**
 * ExpansionBoatController.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionBoatController
 * @brief		This class handle player controls to move boats
 **/
class ExpansionBoatController: ExpansionVehicleController
{
	private float m_Forward;
	private float m_Backward;
	private float m_Turbo;

	private float m_TurnLeft;
	private float m_TurnRight;

	void SetForward( float forward, float backward, float turbo = 0 )
	{
		m_Forward = forward;
		m_Backward = backward;
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

	protected override void OnUpdate()
	{
		super.OnUpdate();
		
		GetInputValue( "UAExpansionBoatMoveForward", m_Forward );
		GetInputValue( "UAExpansionBoatMoveBackward", m_Backward );
		GetInputValue( "UAExpansionBoatRotateLeft", m_TurnLeft );
		GetInputValue( "UAExpansionBoatRotateRight", m_TurnRight );
		GetInputValue( "UAExpansionBoatTurbo", m_Turbo );
	}

	protected override void OnReset()
	{
		super.OnReset();

		m_Forward = 0;
		m_Backward = 0;
		m_TurnLeft = 0;
		m_TurnRight = 0;
		m_Turbo = 0;
	}

	protected override void OnNetworkSend(  ParamsWriteContext ctx )
	{
		ctx.Write( m_Forward );
		ctx.Write( m_Backward );

		ctx.Write( m_Turbo );

		ctx.Write( m_TurnLeft );
		ctx.Write( m_TurnRight );
	}

	protected override bool OnNetworkRecieve( ParamsReadContext ctx )
	{
		ctx.Read( m_Forward );
		ctx.Read( m_Backward );
		
		ctx.Read( m_Turbo );

		ctx.Read( m_TurnLeft );
		ctx.Read( m_TurnRight );

		return true;
	}
}