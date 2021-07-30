/*
 * ExpansionVehicleSteering.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionVehicleSteering
{
	private ref array< float > m_IncreasingSpeed;
	private ref array< float > m_DecreasingSpeed;
	private ref array< float > m_CenteringSpeed;
	
	private string m_IncreasingSpeedFunc;
	private string m_DecreasingSpeedFunc;
	private string m_CenteringSpeedFunc;
	
	private float m_Speed;

	void ExpansionVehicleSteering( ExpansionVehicleBase vehicle )
	{
		if ( LoadSpeed( vehicle.GetType(), "increaseSpeed", m_IncreasingSpeed ) )
			m_IncreasingSpeedFunc = "GetSpeed";
		else
			m_IncreasingSpeedFunc = "GetSpeedSingle";
		
		if ( LoadSpeed( vehicle.GetType(), "decreaseSpeed", m_DecreasingSpeed ) )
			m_DecreasingSpeedFunc = "GetSpeed";
		else
			m_DecreasingSpeedFunc = "GetSpeedSingle";
		
		if ( LoadSpeed( vehicle.GetType(), "centeringSpeed", m_CenteringSpeed ) )
			m_CenteringSpeedFunc = "GetSpeed";
		else
			m_CenteringSpeedFunc = "GetSpeedSingle";
	}

	void ~ExpansionVehicleSteering()
	{
		delete m_IncreasingSpeed;
		delete m_DecreasingSpeed;
		delete m_CenteringSpeed;
	}

	float CalculateChange( float pDt, float pSpeed, float pCurrent, float pTarget )
	{
		float currentAbs = Math.AbsFloat( pCurrent );
		float targetAbs = Math.AbsFloat( pTarget );

		if ( pTarget == 0 )
			g_Script.CallFunctionParams( this, m_CenteringSpeedFunc, m_Speed, new Param2< float, array< float > >( pSpeed, m_CenteringSpeed ) );
		else if ( targetAbs > currentAbs )
			g_Script.CallFunctionParams( this, m_IncreasingSpeedFunc, m_Speed, new Param2< float, array< float > >( pSpeed, m_IncreasingSpeed ) );
		else if ( targetAbs < currentAbs )
			g_Script.CallFunctionParams( this, m_DecreasingSpeedFunc, m_Speed, new Param2< float, array< float > >( pSpeed, m_DecreasingSpeed ) );

		return ( pTarget - pCurrent ) * m_Speed * pDt;
	}

	private bool LoadSpeed( string pType, string pName, out array< float > pValues )
	{
		pValues = new array< float >();
		GetGame().ConfigGetFloatArray( "CfgVehicles " + pType + " SimulationModule Steering " + pName, pValues );
		
		if ( pValues.Count() == 0 )
		{
			pValues.Insert( 100 );
			Error( "[VEHICLE STEERING] Invalid count for " + pName );
			return false;
		}

		if ( pValues.Count() <= 2 )
			return false;

		for ( int i = 0; i < pValues.Count() / 2; ++i )
			pValues[i * 2] = pValues[i * 2] / 3.6;
		
		return true;
	}
	
	private float GetSpeedSingle( float pSpeed, array< float > pValues )
	{
		return pValues[0] / ( Math.AbsFloat( pSpeed ) + 1.0 );
	}

	private float GetSpeed( float pSpeed, array< float > pValues )
	{
		int count = pValues.Count() * 0.5;
		float speed;
				
		for (int i = 1; i < count; i++)
		{
			if (pValues[i * 2] > pSpeed)
			{				
				int c1 = (i - 1) * 2;
				int c2 = i * 2;
			
				float cs1 = pValues[ c1 ];
				float cs2 = pValues[ c2 ];
			
				float s1 = pValues[ c1 + 1 ];
				float s2 = pValues[ c2 + 1 ];
				
				float speedLerp = 1.0 - ( cs2 - pSpeed ) / ( cs2 - cs1 );
								
				speed = Math.Lerp( s1, s2, speedLerp ) * Math.DEG2RAD;
				
				return speed;
			}
		}
		
		speed = pValues[pValues.Count() - 1] * Math.DEG2RAD;
		
		return speed;

	}
};