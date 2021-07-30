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

	private float m_CyclicForward;
	private float m_CyclicBackward;

	private float m_CyclicLeft;
	private float m_CyclicRight;

	private bool m_AutoHover;

	private bool m_FreeLook = true;
	private bool m_WasFreeLookPressed;

	private bool m_IsFlare;
	
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
		m_CyclicForward = forward;
		m_CyclicBackward = backward;
	}

	float GetCyclicForward()
	{
		return m_CyclicForward;
	}

	float GetCyclicBackward()
	{
		return m_CyclicBackward;
	}

	void SetCyclicX( float left, float right, float value )
	{
		m_CyclicLeft = left;
		m_CyclicRight = right;
	}

	float GetCyclicLeft()
	{
		return m_CyclicLeft;
	}

	float GetCyclicRight()
	{
		return m_CyclicRight;
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

		GetInputValue( "UAExpansionHeliCyclicForward", m_CyclicForward );
		GetInputValue( "UAExpansionHeliCyclicBackward", m_CyclicBackward );
		GetInputValue( "UAExpansionHeliCyclicLeft", m_CyclicLeft );
		GetInputValue( "UAExpansionHeliCyclicRight", m_CyclicRight );

		if ( !m_FreeLook )
		{
			float cyclicForward;
			float cyclicBackward;
			float cyclicLeft;
			float cyclicRight;

			if ( GetExpansionClientSettings().UseInvertedMouseControl )
			{
				GetInputValue( "UAAimUp", cyclicForward );
				GetInputValue( "UAAimDown", cyclicBackward );
				GetInputValue( "UAAimLeft", cyclicLeft );
				GetInputValue( "UAAimRight", cyclicRight );
			} else 
			{
				GetInputValue( "UAAimDown", cyclicForward );
				GetInputValue( "UAAimUp", cyclicBackward );
				GetInputValue( "UAAimLeft", cyclicLeft );
				GetInputValue( "UAAimRight", cyclicRight );
			}

			m_CyclicForward += cyclicForward * GetExpansionClientSettings().HelicopterMouseVerticalSensitivity;
			m_CyclicBackward += cyclicBackward * GetExpansionClientSettings().HelicopterMouseVerticalSensitivity;
			m_CyclicLeft += cyclicLeft * GetExpansionClientSettings().HelicopterMouseHorizontalSensitivity;
			m_CyclicRight += cyclicRight * GetExpansionClientSettings().HelicopterMouseHorizontalSensitivity;
		}
	}

	protected override void OnReset()
	{
		super.OnReset();

		m_CollectiveUp = 0;
		m_CollectiveDown = 0;
		m_AntiTorqueLeft = 0;
		m_AntiTorqueRight = 0;
		m_CyclicLeft = 0;
		m_CyclicRight = 0;
		m_CyclicForward = 0;
		m_CyclicRight = 0;

		m_FreeLook = true;

		// don't reset freelook
	}

	protected override void OnNetworkSend(  ParamsWriteContext ctx )
	{
		ctx.Write( m_AutoHover );

		if ( !m_AutoHover )
		{
			ctx.Write( m_CollectiveUp );
			ctx.Write( m_CollectiveDown );

			ctx.Write( m_CyclicForward );
			ctx.Write( m_CyclicBackward );

			ctx.Write( m_CyclicLeft );
			ctx.Write( m_CyclicRight );
		}

		ctx.Write( m_AntiTorqueLeft );
		ctx.Write( m_AntiTorqueRight );
	}

	protected override bool OnNetworkRecieve( ParamsReadContext ctx )
	{
		ctx.Read( m_AutoHover );

		if ( !m_AutoHover )
		{
			ctx.Read( m_CollectiveUp );
			ctx.Read( m_CollectiveDown );

			ctx.Read( m_CyclicForward );
			ctx.Read( m_CyclicBackward );

			ctx.Read( m_CyclicLeft );
			ctx.Read( m_CyclicRight );
		}

		ctx.Read( m_AntiTorqueLeft );
		ctx.Read( m_AntiTorqueRight );

		return true;
	}
}