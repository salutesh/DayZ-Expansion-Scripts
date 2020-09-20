/**
 * ExpansionVehicleSyncState.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionVehicleSyncState
{
	ExpansionVehicleScript m_Vehicle;

	int m_Time;
	int m_LastRecievedTime;
	int m_TimeDelta;

	vector m_Position;
	vector m_Orientation;
	vector m_LinearVelocity;
	vector m_AngularVelocity;

	ref Transform m_InitialTransform;
	ref Transform m_PredictedTransform;

	private float m_LinVelX;
	private float m_LinVelY;
	private float m_LinVelZ;

	private float m_AngVelX;
	private float m_AngVelY;
	private float m_AngVelZ;

	void ExpansionVehicleSyncState( ExpansionVehicleScript vehicle )
	{
		m_Vehicle = vehicle;
		
		m_Position = m_Vehicle.GetPosition();
		m_Orientation = m_Vehicle.GetOrientation();
		m_LinearVelocity = vector.Zero;
		m_AngularVelocity = vector.Zero;

		m_InitialTransform = new Transform();
		m_PredictedTransform = new Transform();
	}
	
	void ~ExpansionVehicleSyncState()
	{
		delete m_InitialTransform;
		delete m_PredictedTransform;
	}

	void OnVariablesSynchronized()
	{
		m_LinearVelocity[0] = m_LinVelX;
		m_LinearVelocity[1] = m_LinVelY;
		m_LinearVelocity[2] = m_LinVelZ;

		m_AngularVelocity[0] = m_AngVelX;
		m_AngularVelocity[1] = m_AngVelY;
		m_AngularVelocity[2] = m_AngVelZ;
	}

	void SynchronizeVariables( ExpansionVehicleNetworkMode mode )
	{
		switch ( mode )
		{
		case ExpansionVehicleNetworkMode.SERVER_ONLY:
			return;
		case ExpansionVehicleNetworkMode.PREDICTION:
			m_LinVelX = m_LinearVelocity[0];
			m_LinVelY = m_LinearVelocity[1];
			m_LinVelZ = m_LinearVelocity[2];

			m_AngVelX = m_AngularVelocity[0];
			m_AngVelY = m_AngularVelocity[1];
			m_AngVelZ = m_AngularVelocity[2];
			return;
		case ExpansionVehicleNetworkMode.CLIENT:
			ScriptRPC rpc = new ScriptRPC();			
			rpc.Write( m_Vehicle.GetTimeForSync() );
			rpc.Write( m_Position );
			rpc.Write( m_Orientation );
			rpc.Write( m_LinearVelocity );
			rpc.Write( m_AngularVelocity );
			rpc.Send( m_Vehicle, ExpansionVehicleRPC.ClientSync, false, NULL );
			return;
		}
	}
};