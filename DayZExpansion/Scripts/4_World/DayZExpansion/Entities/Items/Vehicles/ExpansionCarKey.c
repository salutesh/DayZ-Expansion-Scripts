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
	INVALID = 20600,
	RequestItemData,
	SendItemData,
	COUNT
}

class ExpansionCarKey extends ItemBase
{
	protected static autoptr array<ExpansionCarKey> m_AllKeys = new array<ExpansionCarKey>;
	
	// ------------------------------------------------------------
	// ExpansionCarKey GetFirstKeyForVehicle
	// ------------------------------------------------------------
	/**
	 * @note 	Does not get keys which may be stored in a 
	 * 			player inventory while they are not logged in
	 */
	static ExpansionCarKey GetFirstKeyForVehicle( Object obj )
	{
		#ifdef EXPANSION_CARKEY_LOGGING
		EXLogPrint("ExpansionCarKey::GetFirstKeyForVehicle - Start");
		#endif
		
		if ( !obj )
			return NULL;

		CarScript car;
		ExpansionVehicleScript veh;
		if ( Class.CastTo( car, obj ) )
		{
			for ( int i = 0; i < m_AllKeys.Count(); ++i )
			{
				if (!m_AllKeys[i] || !m_AllKeys[i].IsPaired())
					continue;
				
				if ( m_AllKeys[i].IsPairedTo( car ) )
				{
					#ifdef EXPANSION_CARKEY_LOGGING
					EXLogPrint("ExpansionCarKey::GetFirstKeyForVehicle - End and return Key: " + m_AllKeys[i]);
					#endif
					return m_AllKeys[i];
				}
			}
		} else if ( Class.CastTo( veh, obj ) )
		{
			for ( i = 0; i < m_AllKeys.Count(); ++i )
			{
				if ( !m_AllKeys[i] || !m_AllKeys[i].IsPaired() )
					continue;
				
				if ( m_AllKeys[i].IsPairedTo( veh ) )
				{
					#ifdef EXPANSION_CARKEY_LOGGING
					EXLogPrint("ExpansionCarKey::GetFirstKeyForVehicle - End and return Key: " + m_AllKeys[i]);
					#endif
					return m_AllKeys[i];
				}
			}
		}
		
		#ifdef EXPANSION_CARKEY_LOGGING
		EXLogPrint("ExpansionCarKey::GetFirstKeyForVehicle - End and return NULL");
		#endif
		
		return NULL;
	}
		
	// ------------------------------------------------------------
	// ExpansionCarKey GetKeysForVehicle
	// ------------------------------------------------------------
	/**
	 * @note 	Does not get keys which may be stored in a 
	 * 			player inventory while they are not logged in
	 */
	static void GetKeysForVehicle( Object obj, inout array< ExpansionCarKey > keys )
	{
		#ifdef EXPANSION_CARKEY_LOGGING
		EXLogPrint("ExpansionCarKey::GetKeysForVehicle - Start");
		#endif
		
		if ( !keys )
			keys = new array< ExpansionCarKey >;

		if ( !obj )
			return;

		CarScript car;
		ExpansionVehicleScript veh;
		if ( Class.CastTo( car, obj ) )
		{
			for ( int i = 0; i < m_AllKeys.Count(); ++i )
			{
				if (!m_AllKeys[i] || !m_AllKeys[i].IsPaired())
					continue;
				
				if ( m_AllKeys[i].IsPairedTo( car ) )
					keys.Insert( m_AllKeys[i] );
			}
		} else if ( Class.CastTo( veh, obj ) )
		{
			for ( i = 0; i < m_AllKeys.Count(); ++i )
			{
				if ( !m_AllKeys[i] || !m_AllKeys[i].IsPaired() )
					continue;
				
				if ( m_AllKeys[i].IsPairedTo( veh ) )
					keys.Insert( m_AllKeys[i] );
			}
		}
		
		#ifdef EXPANSION_CARKEY_LOGGING
		EXLogPrint("ExpansionCarKey::GetKeysForVehicle - End");
		#endif
	}
	
	private int m_VehicleIDA;
	private int m_VehicleIDB;
	private int m_VehicleIDC;
	private int m_VehicleIDD;

	private string m_VehicleDisplayName;

	private Object m_Vehicle;
	
	// ------------------------------------------------------------
	// ExpansionCarKey Constructor
	// ------------------------------------------------------------
	void ExpansionCarKey()
	{
		#ifdef EXPANSION_CARKEY_LOGGING
		EXLogPrint("ExpansionCarKey::ExpansionCarKey - Start");
		#endif
		
		RegisterNetSyncVariableInt( "m_VehicleIDA" );
		RegisterNetSyncVariableInt( "m_VehicleIDB" );
		RegisterNetSyncVariableInt( "m_VehicleIDC" );
		RegisterNetSyncVariableInt( "m_VehicleIDD" );

		m_VehicleDisplayName = ConfigGetString( "displayName" );
		
		m_AllKeys.Insert(this);

		if ( GetGame().IsClient() )
			RequestItemData();
		
		#ifdef EXPANSION_CARKEY_LOGGING
		EXLogPrint("ExpansionCarKey::ExpansionCarKey - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionCarKey DeConstructor
	// ------------------------------------------------------------
	void ~ExpansionCarKey()
	{
		#ifdef EXPANSION_CARKEY_LOGGING
		EXLogPrint("ExpansionCarKey::~ExpansionCarKey - Start");
		#endif
		
		m_AllKeys.RemoveItem(this);
		
		#ifdef EXPANSION_CARKEY_LOGGING
		EXLogPrint("ExpansionCarKey::~ExpansionCarKey - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion EEInit
	// ------------------------------------------------------------
	override void EEInit()
	{		
		#ifdef EXPANSION_CARKEY_LOGGING
		EXLogPrint("ExpansionCarKey::ExpansionCarKey - Start");
		#endif
		
		super.EEInit();
		
		if ( IsMissionClient() )
		{
			RequestItemData();
		}
		
		#ifdef EXPANSION_CARKEY_LOGGING
		EXLogPrint("ExpansionCarKey::ExpansionCarKey - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionCarKey EEOnAfterLoad
	// ------------------------------------------------------------
	override void EEOnAfterLoad()
	{
		#ifdef EXPANSION_CARKEY_LOGGING
		EXLogPrint("ExpansionCarKey::EEOnAfterLoad - Start");
		#endif
		
		super.EEOnAfterLoad();

		m_Vehicle = GetGame().GetEntityByPersitentID( m_VehicleIDA, m_VehicleIDB, m_VehicleIDC, m_VehicleIDD );
		
		#ifdef EXPANSION_CARKEY_LOGGING
		EXLogPrint("ExpansionCarKey::EEOnAfterLoad - End");
		#endif
	}
	
	override bool NameOverride( out string output )
	{
		if ( IsPaired() )
			output = m_VehicleDisplayName + " #STR_EXPANSION_KEYS";
		else
			output = "#STR_EXPANSION_KEYS";
		
		return true;
	}

	//override bool DescriptionOverride(out string output)
	//{
	//	output = "ID:"
	//	output = output + " A=" + m_VehicleIDA;
	//	output = output + " B=" + m_VehicleIDB;
	//	output = output + " C=" + m_VehicleIDC;
	//	output = output + " D=" + m_VehicleIDD;
	//	return true;
	//}
	
	// ------------------------------------------------------------
	// ExpansionCarKey PairToVehicle
	// ------------------------------------------------------------
	void PairToVehicle( CarScript vehicle )
	{
		#ifdef EXPANSION_CARKEY_LOGGING
		EXLogPrint("ExpansionCarKey::PairToVehicle 1 - Start vehicle : " + vehicle);
		#endif
		
		m_VehicleIDA = vehicle.GetPersistentIDA();
		m_VehicleIDB = vehicle.GetPersistentIDB();
		m_VehicleIDC = vehicle.GetPersistentIDC();
		m_VehicleIDD = vehicle.GetPersistentIDD();

		#ifdef EXPANSION_CARKEY_LOGGING
		EXLogPrint("ExpansionCarKey::PairToVehicle 1 - m_VehicleIDA : " + m_VehicleIDA);
		EXLogPrint("ExpansionCarKey::PairToVehicle 1 - m_VehicleIDB : " + m_VehicleIDB);
		EXLogPrint("ExpansionCarKey::PairToVehicle 1 - m_VehicleIDC : " + m_VehicleIDC);
		EXLogPrint("ExpansionCarKey::PairToVehicle 1 - m_VehicleIDD : " + m_VehicleIDD);
		#endif

		m_VehicleDisplayName = vehicle.ConfigGetString( "displayName" );
		m_Vehicle = vehicle;

		RPC_RequestItemData( NULL );
		SetSynchDirty();
		
		#ifdef EXPANSION_CARKEY_LOGGING
		EXLogPrint("ExpansionCarKey::PairToVehicle 1 - End vehicle : " + vehicle);
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionCarKey PairToVehicle
	// ------------------------------------------------------------
	void PairToVehicle( ExpansionVehicleScript vehicle )
	{
		#ifdef EXPANSION_CARKEY_LOGGING
		EXLogPrint("ExpansionCarKey::PairToVehicle 2 - Start");
		#endif
		
		m_Vehicle = vehicle;
		
		// TODO
		
		#ifdef EXPANSION_CARKEY_LOGGING
		EXLogPrint("ExpansionCarKey::PairToVehicle 2 - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionCarKey Unpair
	// ------------------------------------------------------------
	void Unpair( bool ignore_check = false )
	{
		#ifdef EXPANSION_CARKEY_LOGGING
		EXLogPrint("ExpansionCarKey::Unpair - Start");
		#endif
		
		if ( !ignore_check )
		{
			array< ExpansionCarKey > keys = new array< ExpansionCarKey >;
			ExpansionCarKey.GetKeysForVehicle( m_Vehicle, keys );

			if ( keys.Count() <= 1 )
			{
				ExpansionVehicleScript exveh;
				if ( Class.CastTo( exveh, m_Vehicle ) )
				{
					//exveh.ResetKeyPairing();
				}

				CarScript vnveh;
				if ( Class.CastTo( vnveh, m_Vehicle ) )
				{
					vnveh.ResetKeyPairing();
				}
			}
		}

		m_VehicleIDA = 0;
		m_VehicleIDB = 0;
		m_VehicleIDC = 0;
		m_VehicleIDD = 0;

		m_Vehicle = NULL;
		m_VehicleDisplayName = ConfigGetString( "displayName" );
		
		RPC_RequestItemData( NULL );
		SetSynchDirty();
		
		#ifdef EXPANSION_CARKEY_LOGGING
		EXLogPrint("ExpansionCarKey::Unpair - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionCarKey KeyMessage
	// ------------------------------------------------------------
	protected void KeyMessage( string message )
	{
		#ifdef EXPANSION_CARKEY_LOGGING	
		if ( IsMissionClient() )
		{
			Message( GetPlayer(), message );

			Print( message );
		} else
		{
			Print( message );
		}
		#endif
	}

	// ------------------------------------------------------------
	// ExpansionCarKey IsPaired
	// ------------------------------------------------------------
	bool IsPaired()
	{
		if ( m_VehicleIDA != 0 || m_VehicleIDB != 0 || m_VehicleIDC != 0 || m_VehicleIDD != 0 )
		{
			KeyMessage("ExpansionCarKey::IsPaired - End and return TRUE");
			return true;
		}
		
		KeyMessage("ExpansionCarKey::IsPaired - End and return FALSE");
		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionCarKey IsPairedTo
	// ------------------------------------------------------------
	bool IsPairedTo( CarScript vehicle )
	{
		//string msg = "ExpansionCarKey::IsPairedTo:";
		//msg = msg + " " + vehicle.GetPersistentIDA() + "=" + m_VehicleIDA;
		//msg = msg + " " + vehicle.GetPersistentIDB() + "=" + m_VehicleIDB;
		//msg = msg + " " + vehicle.GetPersistentIDC() + "=" + m_VehicleIDC;
		//msg = msg + " " + vehicle.GetPersistentIDD() + "=" + m_VehicleIDD;
		//
		//KeyMessage(msg);

		if ( vehicle.GetPersistentIDA() != m_VehicleIDA )
			return false;

		if ( vehicle.GetPersistentIDB() != m_VehicleIDB )
			return false;

		if ( vehicle.GetPersistentIDC() != m_VehicleIDC )
			return false;

		if ( vehicle.GetPersistentIDD() != m_VehicleIDD )
			return false;

		//KeyMessage("PAIRED");

		return true;
	}
	
	// ------------------------------------------------------------
	// ExpansionCarKey IsPairedTo
	// ------------------------------------------------------------
	bool IsPairedTo( ExpansionVehicleScript vehicle )
	{
		#ifdef EXPANSION_CARKEY_LOGGING
		EXLogPrint("ExpansionCarKey::IsPairedTo 2 - Start");
		#endif
		
		// TODO
		#ifdef EXPANSION_CARKEY_LOGGING
		EXLogPrint("ExpansionCarKey::IsPairedTo 2 - End and return FALSE");
		#endif
		
		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionCarKey OnStoreSave
	// ------------------------------------------------------------
	override void OnStoreSave(ParamsWriteContext ctx)
	{
		#ifdef EXPANSION_CARKEY_LOGGING
		EXLogPrint("ExpansionCarKey::OnStoreSave - Start");
		#endif
		
		super.OnStoreSave(ctx);
		
		ctx.Write( m_VehicleIDA );
		ctx.Write( m_VehicleIDB );
		ctx.Write( m_VehicleIDC );
		ctx.Write( m_VehicleIDD );
		ctx.Write( m_VehicleDisplayName );
		
		#ifdef EXPANSION_CARKEY_LOGGING
		EXLogPrint("ExpansionCarKey::OnStoreSave - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionCarKey OnStoreLoad
	// ------------------------------------------------------------	
	override bool OnStoreLoad(ParamsReadContext ctx, int version)
	{
		#ifdef EXPANSION_CARKEY_LOGGING
		EXLogPrint("ExpansionCarKey::OnStoreLoad - Start");
		#endif
		
		if ( !super.OnStoreLoad( ctx, version ) )
			return false;

		if ( Expansion_Assert_False( ctx.Read( m_VehicleIDA ), "[" + this + "] Failed reading m_VehicleIDA" ) )
			return false;
		if ( Expansion_Assert_False( ctx.Read( m_VehicleIDB ), "[" + this + "] Failed reading m_VehicleIDB" ) )
			return false;
		if ( Expansion_Assert_False( ctx.Read( m_VehicleIDC ), "[" + this + "] Failed reading m_VehicleIDC" ) )
			return false;
		if ( Expansion_Assert_False( ctx.Read( m_VehicleIDD ), "[" + this + "] Failed reading m_VehicleIDD" ) )
			return false;
		if ( Expansion_Assert_False( ctx.Read( m_VehicleDisplayName ), "[" + this + "] Failed reading m_VehicleDisplayName" ) )
			return false;

		SetSynchDirty();

		#ifdef EXPANSION_CARKEY_LOGGING
		EXLogPrint("ExpansionCarKey::OnStoreLoad - End and return TRUE");
		#endif
		
		return true;
	}
	
	// ------------------------------------------------------------
	// ExpansionCarKey OnRPC
	// ------------------------------------------------------------		
	override void OnRPC( PlayerIdentity sender, int rpc_type, ParamsReadContext ctx )
	{
		#ifdef EXPANSION_CARKEY_LOGGING
		EXLogPrint("ExpansionCarKey::OnRPC - Start");
		#endif
		
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
		
		#ifdef EXPANSION_CARKEY_LOGGING
		EXLogPrint("ExpansionCarKey::OnRPC - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionCarKey RPC_SendItemData
	// Called on client
	// ------------------------------------------------------------
	private void RPC_SendItemData( ref ParamsReadContext ctx, PlayerIdentity senderRPC )
	{
		#ifdef EXPANSION_CARKEY_LOGGING
		EXLogPrint("ExpansionCarKey::RPC_SendItemData - Start");
		#endif
		
		if ( !ctx.Read( m_VehicleDisplayName ) )
			return;
		
		#ifdef EXPANSION_CARKEY_LOGGING
		EXLogPrint("ExpansionCarKey::RPC_SendItemData - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionCarKey RPC_RequestItemData
	// Called on server
	// ------------------------------------------------------------
	private void RPC_RequestItemData( PlayerIdentity senderRPC )
	{
		#ifdef EXPANSION_CARKEY_LOGGING
		EXLogPrint("ExpansionCarKey::RPC_SendItemData - Start");
		#endif
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write( m_VehicleDisplayName );
		rpc.Send( this, ExpansionCarKeyRPC.SendItemData, true, senderRPC );
		
		#ifdef EXPANSION_CARKEY_LOGGING
		EXLogPrint("ExpansionCarKey::RPC_SendItemData - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionCarKey SendItemData
	// Called on client
	// ------------------------------------------------------------
	void RequestItemData()
	{
		#ifdef EXPANSION_CARKEY_LOGGING
		EXLogPrint("ExpansionCarKey::RPC_SendItemData - Start");
		#endif
		
		if ( IsMissionClient() )
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Send( this, ExpansionCarKeyRPC.RequestItemData, true, NULL );
		}
		
		#ifdef EXPANSION_CARKEY_LOGGING
		EXLogPrint("ExpansionCarKey::RPC_SendItemData - End");
		#endif
	}
};
