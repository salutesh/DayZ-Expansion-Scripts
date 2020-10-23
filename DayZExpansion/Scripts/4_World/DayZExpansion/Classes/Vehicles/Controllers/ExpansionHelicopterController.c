/**
 * ExpansionHelicopterController.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionHelicopterController
 * @brief		This class handle player controls to move helicopter
 **/
class ExpansionHelicopterController: ExpansionVehicleController
{
	private float m_CollectiveUp;
	private float m_CollectiveDown;

	private float m_AntiTorqueLeft;
	private float m_AntiTorqueRight;

	// M for Mouse
	private float m_CyclicForwardM;
	private float m_CyclicBackwardM;

	private float m_CyclicLeftM;
	private float m_CyclicRightM;

	// K for Keyboard
	private float m_CyclicForwardK;
	private float m_CyclicBackwardK;

	private float m_CyclicLeftK;
	private float m_CyclicRightK;

	private bool m_AutoHover;

	private bool m_FreeLook = true;
	private bool m_WasFreeLookPressed;

	private bool m_IsFlare;

	private ExpansionHelicopterScript m_Helicopter;
	
	void SetCollective( float up, float down, float value )
	{
		m_CollectiveUp = up;
		m_CollectiveDown = down;
	}

	float GetCollectiveUp()
	{
		return m_CollectiveUp;
	}

	float GetCollectiveDown()
	{
		return m_CollectiveDown;
	}

	void SetAntiTorque( float left, float right, float value )
	{
		m_AntiTorqueLeft = left;
		m_AntiTorqueRight = right;
	}

	float GetAntiTorqueLeft()
	{
		return m_AntiTorqueLeft;
	}

	float GetAntiTorqueRight()
	{
		return m_AntiTorqueRight;
	}

	void SetCyclicZ( float forward, float backward, float value )
	{
		m_CyclicForwardM = forward;
		m_CyclicForwardK = forward;
		m_CyclicBackwardM = backward;
		m_CyclicBackwardK = backward;
	}

	float GetCyclicForward()
	{
		return (m_CyclicForwardM * GetExpansionClientSettings().HelicopterMouseVerticalSensitivity) + m_CyclicForwardK;
	}

	float GetCyclicBackward()
	{
		return (m_CyclicBackwardM * GetExpansionClientSettings().HelicopterMouseVerticalSensitivity) + m_CyclicBackwardK;
	}

	void SetCyclicX( float left, float right, float value )
	{
		m_CyclicLeftM = left;
		m_CyclicLeftK = left;
		m_CyclicRightM = right;
		m_CyclicRightK = right;
	}

	float GetCyclicLeft()
	{
		return (m_CyclicLeftM * GetExpansionClientSettings().HelicopterMouseHorizontalSensitivity) + m_CyclicLeftK;
	}

	float GetCyclicRight()
	{
		return (m_CyclicRightM * GetExpansionClientSettings().HelicopterMouseHorizontalSensitivity) + m_CyclicRightK;
	}

	bool IsAutoHover()
	{
		return m_AutoHover;
	}

	bool IsFlare()
	{
		return m_IsFlare;
	}

	void SwitchAutoHover()
	{
		m_AutoHover = !m_AutoHover;
	}

	bool IsFreeLook()
	{
		return m_FreeLook;
	}

	protected override void OnUpdate()
	{
		super.OnUpdate();

		if ( IsMissionClient() )
		{
			if ( GetExpansionClientSettings().UseHelicopterMouseControl )
			{
				bool isFreelook = false;
				GetInputValue( "UAExpansionHeliFreeLook", isFreelook );

				if ( isFreelook && !m_WasFreeLookPressed )
				{
					m_WasFreeLookPressed = true;
					m_FreeLook = !m_FreeLook;
				} else if ( !isFreelook )
				{
					m_WasFreeLookPressed = false;
				}
			} else {
				m_FreeLook = true;
			}
		}

		GetInputValue( "UAExpansionHeliFlare", m_IsFlare ); // Not used

		GetInputValue( "UAExpansionHeliCollectiveUp", m_CollectiveUp );
		GetInputValue( "UAExpansionHeliCollectiveDown", m_CollectiveDown );

		GetInputValue( "UAExpansionHeliAntiTorqueLeft", m_AntiTorqueLeft );
		GetInputValue( "UAExpansionHeliAntiTorqueRight", m_AntiTorqueRight );

		GetInputValue( "UAExpansionHeliCyclicForward", m_CyclicForwardM );
		GetInputValue( "UAExpansionHeliCyclicBackward", m_CyclicBackwardM );
		GetInputValue( "UAExpansionHeliCyclicLeft", m_CyclicLeftM );
		GetInputValue( "UAExpansionHeliCyclicRight", m_CyclicRightM );

		if ( !m_FreeLook && GetExpansionClientSettings().UseHelicopterMouseControl )
		{
			if ( !GetExpansionClientSettings().UseInvertedMouseControl )
			{
				GetInputValue( "UAAimUp", m_CyclicForwardM );
				GetInputValue( "UAAimDown", m_CyclicBackwardM );
				GetInputValue( "UAAimLeft", m_CyclicLeftM );
				GetInputValue( "UAAimRight", m_CyclicRightM );
			} else 
			{
				GetInputValue( "UAAimDown", m_CyclicForwardM );
				GetInputValue( "UAAimUp", m_CyclicBackwardM );
				GetInputValue( "UAAimLeft", m_CyclicLeftM );
				GetInputValue( "UAAimRight", m_CyclicRightM );
			}
		}
		GetInputValue( "UAExpansionHeliCyclicForward", m_CyclicForwardK );
		GetInputValue( "UAExpansionHeliCyclicBackward", m_CyclicBackwardK );
		GetInputValue( "UAExpansionHeliCyclicLeft", m_CyclicLeftK );
		GetInputValue( "UAExpansionHeliCyclicRight", m_CyclicRightK );

		ExpansionDebugger.Display( EXPANSION_DEBUG_VEHICLE_CONTROLLER, "ExpansionHelicopterController::OnUpdate" );
	}

	protected override void OnReset()
	{
		super.OnReset();

		m_CollectiveUp = 0;
		m_CollectiveDown = 0;
		m_AntiTorqueLeft = 0;
		m_AntiTorqueRight = 0;
		m_CyclicForwardM = 0;
		m_CyclicBackwardM = 0;
		m_CyclicLeftM = 0;
		m_CyclicRightM = 0;
		m_CyclicForwardK = 0;
		m_CyclicBackwardK = 0;
		m_CyclicLeftK = 0;
		m_CyclicRightK = 0;
		
		m_FreeLook = true;

		// don't reset freelook

		ExpansionDebugger.Display( EXPANSION_DEBUG_VEHICLE_CONTROLLER, "ExpansionHelicopterController::OnReset" );
	}

	protected override void OnNetworkSend( ref ParamsWriteContext ctx )
	{
		ctx.Write( m_AutoHover );

		if ( !m_AutoHover )
		{
			ctx.Write( m_CollectiveUp );
			ctx.Write( m_CollectiveDown );

			ctx.Write( m_CyclicForwardM );
			ctx.Write( m_CyclicBackwardM );

			ctx.Write( m_CyclicForwardK );
			ctx.Write( m_CyclicBackwardK );

			ctx.Write( m_CyclicLeftM );
			ctx.Write( m_CyclicRightM );

			ctx.Write( m_CyclicLeftK );
			ctx.Write( m_CyclicRightK );
		}

		ctx.Write( m_AntiTorqueLeft );
		ctx.Write( m_AntiTorqueRight );
	}

	protected override bool OnNetworkRecieve( ref ParamsReadContext ctx )
	{
		ctx.Read( m_AutoHover );

		if ( !m_AutoHover )
		{
			ctx.Read( m_CollectiveUp );
			ctx.Read( m_CollectiveDown );

			ctx.Read( m_CyclicForwardM );
			ctx.Read( m_CyclicBackwardM );

			ctx.Write( m_CyclicForwardK );
			ctx.Write( m_CyclicBackwardK );

			ctx.Write( m_CyclicLeftM );
			ctx.Write( m_CyclicRightM );

			ctx.Write( m_CyclicLeftK );
			ctx.Write( m_CyclicRightK );
		}

		ctx.Read( m_AntiTorqueLeft );
		ctx.Read( m_AntiTorqueRight );

		return true;
	}
}