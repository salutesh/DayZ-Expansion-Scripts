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

	int m_ClientSimulationTime;

	vector m_Position;
	vector m_Orientation;
	vector m_LinearVelocity;
	vector m_AngularVelocity;

	vector m_InitialTransform[4];
	vector m_PredictedTransform[4];

	float m_LinearVelocityX;
	float m_LinearVelocityY;
	float m_LinearVelocityZ;

	float m_AngularVelocityX;
	float m_AngularVelocityY;
	float m_AngularVelocityZ;

	void ExpansionVehicleSyncState( ExpansionVehicleScript vehicle )
	{
		m_Vehicle = vehicle;
		
		m_Position = m_Vehicle.GetPosition();
		m_Orientation = m_Vehicle.GetOrientation();
		m_LinearVelocity = vector.Zero;
		m_AngularVelocity = vector.Zero;
	}
	
	void RegisterNetVariables()
	{
		m_Vehicle.RegisterNetSyncVariableInt( "m_SyncState.m_ClientSimulationTime" );

		m_Vehicle.RegisterNetSyncVariableFloat( "m_SyncState.m_LinearVelocityX", 0, 0, 4 );
		m_Vehicle.RegisterNetSyncVariableFloat( "m_SyncState.m_LinearVelocityY", 0, 0, 4 );
		m_Vehicle.RegisterNetSyncVariableFloat( "m_SyncState.m_LinearVelocityZ", 0, 0, 4 );

		m_Vehicle.RegisterNetSyncVariableFloat( "m_SyncState.m_AngularVelocityX", 0, 0, 4 );
		m_Vehicle.RegisterNetSyncVariableFloat( "m_SyncState.m_AngularVelocityY", 0, 0, 4 );
		m_Vehicle.RegisterNetSyncVariableFloat( "m_SyncState.m_AngularVelocityZ", 0, 0, 4 );
	}
	
	void ~ExpansionVehicleSyncState()
	{
	}

	void OnVariablesSynchronized()
	{
		m_LinearVelocity = Vector( m_LinearVelocityX, m_LinearVelocityY, m_LinearVelocityZ );
		m_AngularVelocity = Vector( m_AngularVelocityX, m_AngularVelocityY, m_AngularVelocityZ );
	}

	void ClientSync()
	{
		ScriptRPC rpc = new ScriptRPC();			
		rpc.Write( m_Vehicle.GetTimeForSync() );
		rpc.Write( m_Position );
		rpc.Write( m_Orientation );
		rpc.Write( m_LinearVelocity );
		rpc.Write( m_AngularVelocity );
		rpc.Send( m_Vehicle, ExpansionVehicleRPC.ClientSync, false, NULL );
	}
};