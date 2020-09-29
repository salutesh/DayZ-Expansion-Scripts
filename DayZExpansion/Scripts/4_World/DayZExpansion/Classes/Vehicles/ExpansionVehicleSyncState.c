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

	vector m_InitialTransform[4];
	vector m_PredictedTransform[4];

	void ExpansionVehicleSyncState( ExpansionVehicleScript vehicle )
	{
		m_Vehicle = vehicle;
		
		m_Position = m_Vehicle.GetPosition();
		m_Orientation = m_Vehicle.GetOrientation();
		m_LinearVelocity = vector.Zero;
		m_AngularVelocity = vector.Zero;
	}
	
	void ~ExpansionVehicleSyncState()
	{
	}

	void OnVariablesSynchronized()
	{
	}

	void SynchronizeVariables( ExpansionVehicleNetworkMode mode )
	{
		switch ( mode )
		{
		case ExpansionVehicleNetworkMode.SERVER_ONLY:
			return;
		case ExpansionVehicleNetworkMode.PREDICTION:
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