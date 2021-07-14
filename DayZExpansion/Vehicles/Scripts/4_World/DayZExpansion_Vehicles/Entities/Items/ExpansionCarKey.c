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

class ExpansionCarKey extends ItemBase
{
	protected static autoptr array<ExpansionCarKey> m_AllKeys = new array<ExpansionCarKey>;

	private bool m_IsMasterKey;
	private int MasterKeyUses;
	
	//! After pairing to a vehicle, it's the ID of the master key.
	//! This allows "changing locks" on vehicles so old paired keys no longer work
	private int m_PersistentIDA;
	private int m_PersistentIDB;
	private int m_PersistentIDC;
	private int m_PersistentIDD;

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
		
		RegisterNetSyncVariableBool( "m_IsMasterKey" );
		RegisterNetSyncVariableInt( "MasterKeyUses" );

		RegisterNetSyncVariableInt( "m_PersistentIDA" );
		RegisterNetSyncVariableInt( "m_PersistentIDB" );
		RegisterNetSyncVariableInt( "m_PersistentIDC" );
		RegisterNetSyncVariableInt( "m_PersistentIDD" );

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

		GetKeyObject();

		if ( !m_Vehicle && IsPaired() )
		{
			EXPrint(ToString() + "::EEOnAfterLoad - Unpairing \"" + GetDisplayName() + "\" " + GetPosition() + " because its vehicle no longer exists");
			Unpair( true );
		}
		
		#ifdef EXPANSION_CARKEY_LOGGING
		EXLogPrint("ExpansionCarKey::EEOnAfterLoad - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionCarKey GetMasterUses
	// ------------------------------------------------------------
	int GetMasterUses()
	{
		if ( m_IsMasterKey && MasterKeyUses == -1 )
		{
			//! Key from old storage. Initialize MasterKeyUses from Expansion settings.
			MasterKeyUses = GetExpansionSettings().GetVehicle().MasterKeyUses;
		}

		return MasterKeyUses;
	}
	
	// ------------------------------------------------------------
	// ExpansionCarKey GetMasterUses
	// ------------------------------------------------------------
	void SetMasterUses(int amount = 0, bool synch = true)
	{
		MasterKeyUses = amount;

		if ( synch )
			SetSynchDirty();
	}
	
	// ------------------------------------------------------------
	// ExpansionCarKey IsMaster
	// ------------------------------------------------------------
	bool IsMaster()
	{
		return m_IsMasterKey;
	}
	
	// ------------------------------------------------------------
	// ExpansionCarKey SetMaster
	// ------------------------------------------------------------
	private void SetMaster(bool state)
	{
		m_IsMasterKey = state;
	}
	
	// ------------------------------------------------------------
	// ExpansionCarKey GetDisplayName
	// ------------------------------------------------------------
	string GetKeyDisplayName()
	{
		return m_VehicleDisplayName;
	}
	
	// ------------------------------------------------------------
	// ExpansionCarKey GetObject
	// ------------------------------------------------------------
	Object GetKeyObject()
	{
		if ( !m_Vehicle )
		{
			EXPrint(ToString() + "::GetKeyObject - looking for vehicle");
			foreach ( CarScript car : CarScript.GetAll() )
			{
				if ( IsPairedTo( car ) )
				{
					EXPrint(ToString() + "::GetKeyObject - found " + car);
					m_Vehicle = car;
					break;
				}
			}
		}

		return m_Vehicle;
	}
	
	// ------------------------------------------------------------
	// ExpansionCarKey GetPersistentIDA
	// ------------------------------------------------------------
	int GetPersistentIDA()
	{		
		return m_PersistentIDA;
	}
	
	// ------------------------------------------------------------
	// ExpansionCarKey GetPersistentIDA
	// ------------------------------------------------------------
	int GetPersistentIDB()
	{		
		return m_PersistentIDB;
	}
	
	// ------------------------------------------------------------
	// ExpansionCarKey GetPersistentIDA
	// ------------------------------------------------------------
	int GetPersistentIDC()
	{		
		return m_PersistentIDC;
	}
	
	// ------------------------------------------------------------
	// ExpansionCarKey GetPersistentIDA
	// ------------------------------------------------------------
	int GetPersistentIDD()
	{		
		return m_PersistentIDD;
	}
	
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
		ExpansionVehicleBase veh;
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
		ExpansionVehicleBase veh;
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
	
	override bool NameOverride( out string output )
	{
		if ( IsPaired() )
		{
			if ( IsMaster() )
			{
				output = m_VehicleDisplayName + " #STR_EXPANSION_MASTER_KEYS";
			} else {
				output = m_VehicleDisplayName + " #STR_EXPANSION_KEYS";
			}
		} else {
			output = "#STR_EXPANSION_KEYS";
		}
		
		return true;
	}

	override bool DescriptionOverride(out string output)
	{
		if ( IsMaster() )
			output = "#STR_EXPANSION_CAR_MASTER_KEYS_DESC " + GetMasterUses().ToString();
		else
			output = "#STR_EXPANSION_CAR_KEYS_DESC";
		
		return true;
	}

	//override bool DescriptionOverride(out string output)
	//{
	//	output = "ID:"
	//	output = output + " A=" + m_PersistentIDA;
	//	output = output + " B=" + m_PersistentIDB;
	//	output = output + " C=" + m_PersistentIDC;
	//	output = output + " D=" + m_PersistentIDD;
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
		
		GetPersistentID( m_PersistentIDA, m_PersistentIDB, m_PersistentIDC, m_PersistentIDD );

		vehicle.SetPersistentIDA(m_PersistentIDA);
		vehicle.SetPersistentIDB(m_PersistentIDB);
		vehicle.SetPersistentIDC(m_PersistentIDC);
		vehicle.SetPersistentIDD(m_PersistentIDD);

		if ( GetExpansionSettings().GetVehicle().MasterKeyPairingMode != 0 )
		{
			SetMaster(true); //! If we can pair from the car, then it mean the car doesn't have any paired keys to it
			SetMasterUses(GetExpansionSettings().GetVehicle().MasterKeyUses, false);
		}

		#ifdef EXPANSION_CARKEY_LOGGING
		EXLogPrint("ExpansionCarKey::PairToVehicle 1 - m_PersistentIDA : " + m_PersistentIDA);
		EXLogPrint("ExpansionCarKey::PairToVehicle 1 - m_PersistentIDB : " + m_PersistentIDB);
		EXLogPrint("ExpansionCarKey::PairToVehicle 1 - m_PersistentIDC : " + m_PersistentIDC);
		EXLogPrint("ExpansionCarKey::PairToVehicle 1 - m_PersistentIDD : " + m_PersistentIDD);
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
	// ExpansionCarKey PairWithMasterKey
	// ------------------------------------------------------------
	void PairWithMasterKey( ExpansionCarKey key )
	{
		#ifdef EXPANSION_CARKEY_LOGGING
		EXLogPrint("ExpansionCarKey::PairWithMasterKey 1 - Start key : " + key);
		#endif

		if ( !key.IsMaster() || this.IsMaster() )
			return;

		if ( key.GetMasterUses() <= 1 )
		{
			if ( GetExpansionSettings().GetVehicle().MasterKeyPairingMode == 1 )
				key.SetMaster(false);
		}

		key.SetMasterUses( key.GetMasterUses() - 1 );

		m_PersistentIDA = key.GetPersistentIDA();
		m_PersistentIDB = key.GetPersistentIDB();
		m_PersistentIDC = key.GetPersistentIDC();
		m_PersistentIDD = key.GetPersistentIDD();

		#ifdef EXPANSION_CARKEY_LOGGING
		EXLogPrint("ExpansionCarKey::PairWithMasterKey 1 - m_PersistentIDA : " + m_PersistentIDA);
		EXLogPrint("ExpansionCarKey::PairWithMasterKey 1 - m_PersistentIDB : " + m_PersistentIDB);
		EXLogPrint("ExpansionCarKey::PairWithMasterKey 1 - m_PersistentIDC : " + m_PersistentIDC);
		EXLogPrint("ExpansionCarKey::PairWithMasterKey 1 - m_PersistentIDD : " + m_PersistentIDD);
		#endif

		m_VehicleDisplayName = key.GetKeyDisplayName();
		m_Vehicle = key.GetKeyObject();

		RPC_RequestItemData( NULL );
		SetSynchDirty();
		
		#ifdef EXPANSION_CARKEY_LOGGING
		EXLogPrint("ExpansionCarKey::PairWithMasterKey 1 - End key : " + key);
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionCarKey PairToVehicle
	// ------------------------------------------------------------
	void PairToVehicle( ExpansionVehicleBase vehicle )
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
				ExpansionVehicleBase exveh;
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

		m_PersistentIDA = 0;
		m_PersistentIDB = 0;
		m_PersistentIDC = 0;
		m_PersistentIDD = 0;
		m_IsMasterKey = false;

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
		if ( m_PersistentIDA != 0 || m_PersistentIDB != 0 || m_PersistentIDC != 0 || m_PersistentIDD != 0 )
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
		//msg = msg + " " + vehicle.GetPersistentIDA() + "=" + m_PersistentIDA;
		//msg = msg + " " + vehicle.GetPersistentIDB() + "=" + m_PersistentIDB;
		//msg = msg + " " + vehicle.GetPersistentIDC() + "=" + m_PersistentIDC;
		//msg = msg + " " + vehicle.GetPersistentIDD() + "=" + m_PersistentIDD;
		//
		//KeyMessage(msg);

		if ( m_PersistentIDA == 0 || vehicle.GetPersistentIDA() != m_PersistentIDA )
			return false;

		if ( m_PersistentIDB == 0 || vehicle.GetPersistentIDB() != m_PersistentIDB )
			return false;

		if ( m_PersistentIDC == 0 || vehicle.GetPersistentIDC() != m_PersistentIDC )
			return false;

		if ( m_PersistentIDD == 0 || vehicle.GetPersistentIDD() != m_PersistentIDD )
			return false;

		//KeyMessage("PAIRED");

		return true;
	}
	
	// ------------------------------------------------------------
	// ExpansionCarKey IsPairedTo
	// ------------------------------------------------------------
	bool IsPairedToMaster( ExpansionCarKey masterkey )
	{
		if ( m_PersistentIDA == 0 || masterkey.GetPersistentIDA() != m_PersistentIDA )
			return false;

		if ( m_PersistentIDB == 0 || masterkey.GetPersistentIDB() != m_PersistentIDB )
			return false;

		if ( m_PersistentIDC == 0 || masterkey.GetPersistentIDC() != m_PersistentIDC )
			return false;

		if ( m_PersistentIDD == 0 || masterkey.GetPersistentIDD() != m_PersistentIDD )
			return false;

		return true;
	}
	
	// ------------------------------------------------------------
	// ExpansionCarKey IsPairedTo
	// ------------------------------------------------------------
	bool IsPairedTo( ExpansionVehicleBase vehicle )
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
		#ifdef CF_MODULE_MODSTORAGE
		if ( GetGame().SaveVersion() >= EXPANSION_VERSION_GAME_MODSTORAGE_TARGET )
		{
			super.OnStoreSave( ctx );
			return;
		}
		#endif

		#ifdef EXPANSION_CARKEY_LOGGING
		EXLogPrint("ExpansionCarKey::OnStoreSave - Start");
		#endif
		
		super.OnStoreSave(ctx);
		
		ctx.Write( m_PersistentIDA );
		ctx.Write( m_PersistentIDB );
		ctx.Write( m_PersistentIDC );
		ctx.Write( m_PersistentIDD );
		ctx.Write( m_VehicleDisplayName );

		ctx.Write( MasterKeyUses );
		ctx.Write( m_IsMasterKey );
		
		#ifdef EXPANSION_CARKEY_LOGGING
		EXLogPrint("ExpansionCarKey::OnStoreSave - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionCarKey OnStoreLoad
	// ------------------------------------------------------------	
	override bool OnStoreLoad(ParamsReadContext ctx, int version)
	{
		if ( Expansion_Assert_False( super.OnStoreLoad( ctx, version ), "[" + this + "] Failed reading OnStoreLoad super" ) )
			return false;

		#ifdef CF_MODULE_MODSTORAGE
		if ( version > EXPANSION_VERSION_GAME_MODSTORAGE_TARGET || m_ExpansionSaveVersion > EXPANSION_VERSION_SAVE_MODSTORAGE_TARGET )
			return true;
		#endif

		if ( Expansion_Assert_False( ctx.Read( m_PersistentIDA ), "[" + this + "] Failed reading m_PersistentIDA" ) )
			return false;
		if ( Expansion_Assert_False( ctx.Read( m_PersistentIDB ), "[" + this + "] Failed reading m_PersistentIDB" ) )
			return false;
		if ( Expansion_Assert_False( ctx.Read( m_PersistentIDC ), "[" + this + "] Failed reading m_PersistentIDC" ) )
			return false;
		if ( Expansion_Assert_False( ctx.Read( m_PersistentIDD ), "[" + this + "] Failed reading m_PersistentIDD" ) )
			return false;
		if ( Expansion_Assert_False( ctx.Read( m_VehicleDisplayName ), "[" + this + "] Failed reading m_VehicleDisplayName" ) )
			return false;

		if ( m_ExpansionSaveVersion < 19 )
		{
			if ( IsPaired() )
			{
				m_IsMasterKey = true;  //! Keys from old storage should be master keys if already paired
				MasterKeyUses = -1;
			}
		} else {
			if ( Expansion_Assert_False( ctx.Read( MasterKeyUses ), "[" + this + "] Failed reading MasterKeyUses" ) )
				return false;

			if ( Expansion_Assert_False( ctx.Read( m_IsMasterKey ), "[" + this + "] Failed reading m_IsMasterKey" ) )
				return false;
		}

		SetSynchDirty();

		#ifdef EXPANSION_CARKEY_LOGGING
		EXLogPrint("ExpansionCarKey::OnStoreLoad - End and return TRUE");
		#endif
		
		return true;
	}

	#ifdef CF_MODULE_MODSTORAGE
	override void CF_OnStoreSave( CF_ModStorage storage, string modName )
	{
		super.CF_OnStoreSave( storage, modName );

		if ( modName != "DZ_Expansion_Vehicles" )
			return;

		storage.Write( m_PersistentIDA );
		storage.Write( m_PersistentIDB );
		storage.Write( m_PersistentIDC );
		storage.Write( m_PersistentIDD );
		storage.Write( m_VehicleDisplayName );
		
		storage.Write( MasterKeyUses );
		storage.Write( m_IsMasterKey );
	}
	
	override bool CF_OnStoreLoad( CF_ModStorage storage, string modName )
	{
		if ( !super.CF_OnStoreLoad( storage, modName ) )
			return false;

		if ( modName != "DZ_Expansion_Vehicles" )
			return true;

		if ( Expansion_Assert_False( storage.Read( m_PersistentIDA ), "[" + this + "] Failed reading m_PersistentIDA" ) )
			return false;
		if ( Expansion_Assert_False( storage.Read( m_PersistentIDB ), "[" + this + "] Failed reading m_PersistentIDB" ) )
			return false;
		if ( Expansion_Assert_False( storage.Read( m_PersistentIDC ), "[" + this + "] Failed reading m_PersistentIDC" ) )
			return false;
		if ( Expansion_Assert_False( storage.Read( m_PersistentIDD ), "[" + this + "] Failed reading m_PersistentIDD" ) )
			return false;
		if ( Expansion_Assert_False( storage.Read( m_VehicleDisplayName ), "[" + this + "] Failed reading m_VehicleDisplayName" ) )
			return false;

		if ( storage.GetVersion() < 19 )
		{
			if ( IsPaired() )
			{				
				SetMaster(true); //! Keys from old storage should be master keys if already paired
				SetMasterUses(-1, false);
			}
		} else {
			if ( Expansion_Assert_False( storage.Read( MasterKeyUses ), "[" + this + "] Failed reading MasterKeyUses" ) )
				return false;

			if ( Expansion_Assert_False( storage.Read( m_IsMasterKey ), "[" + this + "] Failed reading m_IsMasterKey" ) )
				return false;
		}

		return true;
	}
	#endif
	
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
