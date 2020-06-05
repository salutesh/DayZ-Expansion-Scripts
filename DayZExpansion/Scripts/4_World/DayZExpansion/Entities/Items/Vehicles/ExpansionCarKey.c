/*
 * ExpansionCarKey.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

enum ExpansionCarKeyRPC
{
	NONE,
	RequestItemData = 20602,
	SendItemData,
	COUNT
}

class ExpansionCarKey extends ItemBase
{
	protected static autoptr array<ExpansionCarKey> m_AllKeys = new array<ExpansionCarKey>;
	
	static ExpansionCarKey GetKeyByVehicle( CarScript vehicle )
	{
		if (!vehicle)
			return null;
		
		for ( int i = 0; i < m_AllKeys.Count(); ++i )
		{
			if (!m_AllKeys[i] || !m_AllKeys[i].IsPaired())
				continue;
			
			if ( m_AllKeys[i].IsPairedTo( vehicle ) )
				return m_AllKeys[i];
		}
		
		return null;
	}
	
	protected int m_VehicleIDA = 0;
	protected int m_VehicleIDB = 0;
	protected int m_VehicleIDC = 0;
	protected int m_VehicleIDD = 0;

	protected string m_VehicleDisplayName;
	
	// ------------------------------------------------------------
	// ExpansionCarKey Constructor
	// ------------------------------------------------------------
	void ExpansionCarKey()
	{
		RegisterNetSyncVariableInt( "m_VehicleIDA" );
		RegisterNetSyncVariableInt( "m_VehicleIDB" );
		RegisterNetSyncVariableInt( "m_VehicleIDC" );
		RegisterNetSyncVariableInt( "m_VehicleIDD" );

		m_VehicleDisplayName = ConfigGetString( "displayName" );
		
		m_AllKeys.Insert(this);
	}
	
	// ------------------------------------------------------------
	// ExpansionCarKey DeConstructor
	// ------------------------------------------------------------
	void ~ExpansionCarKey()
	{
		m_AllKeys.RemoveItem(this);
	}

	// ------------------------------------------------------------
	// Expansion EEInit
	// ------------------------------------------------------------
	override void EEInit()
	{		
		super.EEInit();
		
		if ( IsMissionClient() )
		{
			RequestItemData();
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionCarKey GetDisplayName
	// ------------------------------------------------------------
	override string GetDisplayName()
	{
		return m_VehicleDisplayName + " key";
	}
	
	// ------------------------------------------------------------
	// ExpansionCarKey PairToVehicle
	// ------------------------------------------------------------
	void PairToVehicle( CarScript vehicle )
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionCarKey::PairToVehicle - Start vehicle : " + vehicle);
		#endif
		
		m_VehicleIDA = vehicle.GetPersistentIDA();
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionCarKey::PairToVehicle - m_VehicleIDA : " + m_VehicleIDA);
		#endif
		
		m_VehicleIDB = vehicle.GetPersistentIDB();
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionCarKey::PairToVehicle - m_VehicleIDB : " + m_VehicleIDB);
		#endif
		
		m_VehicleIDC = vehicle.GetPersistentIDC();
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionCarKey::PairToVehicle - m_VehicleIDC : " + m_VehicleIDC);
		#endif
		
		m_VehicleIDD = vehicle.GetPersistentIDD();
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionCarKey::PairToVehicle - m_VehicleIDD : " + m_VehicleIDD);
		#endif

		m_VehicleDisplayName = vehicle.ConfigGetString( "displayName" );
		
		PlayerBase ownerPlayer = GetHierarchyRootPlayer();
		if ( ownerPlayer && ownerPlayer.GetIdentity() )
			RPC_RequestItemData( ownerPlayer.GetIdentity() );

		SetSynchDirty();
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionCarKey::PairToVehicle - End vehicle : " + vehicle);
		#endif
	}
	
	void UnPair()
	{
		m_VehicleIDA = 0;
		m_VehicleIDB = 0;
		m_VehicleIDC = 0;
		m_VehicleIDD = 0;
		
		m_VehicleDisplayName = ConfigGetString( "displayName" );
		
		PlayerBase ownerPlayer = GetHierarchyRootPlayer();
		if ( ownerPlayer && ownerPlayer.GetIdentity() )
			RPC_RequestItemData( ownerPlayer.GetIdentity() );
		
		SetSynchDirty();
	}
	
	// ------------------------------------------------------------
	// ExpansionCarKey PairToVehicle
	// ------------------------------------------------------------
	void PairToVehicle( ExpansionVehicleScript vehicle )
	{
		// TODO
	}
	
	protected void KeyMessage( string message )
	{
		if ( IsMissionClient() )
		{
			Message( GetPlayer(), message );
		}
		else
		{
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint( message );
			#endif
		}
	}

	// ------------------------------------------------------------
	// ExpansionCarKey IsPaired
	// ------------------------------------------------------------
	bool IsPaired()
	{
		if ( m_VehicleIDA != 0 && m_VehicleIDB != 0 && m_VehicleIDC != 0 && m_VehicleIDD != 0 )
		{
			//KeyMessage("ExpansionCarKey::IsPaired true");
			return true;
		}
		
		//KeyMessage("ExpansionCarKey::IsPaired false");

		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionCarKey IsPairedTo
	// ------------------------------------------------------------
	bool IsPairedTo( CarScript vehicle )
	{
		if ( vehicle.GetPersistentIDA() != m_VehicleIDA )
		{
			return false;
		}

		if ( vehicle.GetPersistentIDB() != m_VehicleIDB )
		{
			return false;
		}

		if ( vehicle.GetPersistentIDC() != m_VehicleIDC )
		{
			return false;
		}

		if ( vehicle.GetPersistentIDD() != m_VehicleIDD )
		{
			return false;
		}

		return true;
	}
	
	// ------------------------------------------------------------
	// ExpansionCarKey IsPairedTo
	// ------------------------------------------------------------
	bool IsPairedTo( ExpansionVehicleScript vehicle )
	{
		// TODO
		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionCarKey OnStoreSave
	// ------------------------------------------------------------
	override void OnStoreSave(ParamsWriteContext ctx)
	{
		super.OnStoreSave(ctx);
		
		ctx.Write( m_VehicleIDA );
		ctx.Write( m_VehicleIDB );
		ctx.Write( m_VehicleIDC );
		ctx.Write( m_VehicleIDD );
		ctx.Write( m_VehicleDisplayName );
	}
	
	// ------------------------------------------------------------
	// ExpansionCarKey OnStoreLoad
	// ------------------------------------------------------------	
	override bool OnStoreLoad(ParamsReadContext ctx, int version)
	{
		if ( !super.OnStoreLoad( ctx, version ) )
			return false;

		if ( !ctx.Read( m_VehicleIDA ) )
			return false;

		if ( !ctx.Read( m_VehicleIDB ) )
			return false;

		if ( !ctx.Read( m_VehicleIDC ) )
			return false;

		if ( !ctx.Read( m_VehicleIDD ) )
			return false;

		if ( !ctx.Read( m_VehicleDisplayName ) )
			return false;

		return true;
	}
	
	// ------------------------------------------------------------
	// ExpansionCarKey OnItemLocationChanged
	// ------------------------------------------------------------
	override void OnItemLocationChanged( EntityAI old_owner, EntityAI new_owner ) 
	{
		super.OnItemLocationChanged( old_owner, new_owner );
		
		if ( new_owner && new_owner.IsInherited( PlayerBase ) )
		{
			if ( IsMissionClient() )
			{
				RequestItemData();
			}
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionCarKey OnRPC
	// ------------------------------------------------------------		
	override void OnRPC( PlayerIdentity sender, int rpc_type, ParamsReadContext ctx )
	{
		super.OnRPC(sender, rpc_type, ctx);
		
		switch (rpc_type)
		{
			case ExpansionCarKeyRPC.RequestItemData:
			{
				RPC_RequestItemData( sender );
				break;
			}
			case ExpansionCarKeyRPC.SendItemData:
			{
				RPC_SendItemData( ctx, sender );
				break;
			}
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionCarKey RPC_SendItemData
	// Called on client
	// ------------------------------------------------------------
	private void RPC_SendItemData( ref ParamsReadContext ctx, PlayerIdentity senderRPC )
	{
		if ( !ctx.Read( m_VehicleDisplayName ) )
			return;
	}
	
	// ------------------------------------------------------------
	// ExpansionCarKey RPC_RequestItemData
	// Called on server
	// ------------------------------------------------------------
	private void RPC_RequestItemData( PlayerIdentity senderRPC )
	{
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write( m_VehicleDisplayName );
		rpc.Send( this, ExpansionCarKeyRPC.SendItemData, true, senderRPC );
	}
	
	// ------------------------------------------------------------
	// ExpansionCarKey SendItemData
	// Called on client
	// ------------------------------------------------------------
	void RequestItemData()
	{
		if ( IsMissionClient() )
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Send( this, ExpansionCarKeyRPC.RequestItemData, true, NULL );
		}
	}
}
