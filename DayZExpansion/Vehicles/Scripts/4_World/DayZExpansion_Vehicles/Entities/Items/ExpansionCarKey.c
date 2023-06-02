/**
 * ExpansionCarKey.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionCarKey: ItemBase
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

	ref ExpansionNetsyncData m_Expansion_NetsyncData;

	private Object m_Vehicle;

	void ExpansionCarKey()
	{
		RegisterNetSyncVariableBool( "m_IsMasterKey" );
		RegisterNetSyncVariableInt( "MasterKeyUses" );

		RegisterNetSyncVariableInt( "m_PersistentIDA" );
		RegisterNetSyncVariableInt( "m_PersistentIDB" );
		RegisterNetSyncVariableInt( "m_PersistentIDC" );
		RegisterNetSyncVariableInt( "m_PersistentIDD" );

		m_Expansion_NetsyncData = new ExpansionNetsyncData(this);
		
		m_AllKeys.Insert(this);
	}

	void ~ExpansionCarKey()
	{		
		if (!GetGame())
			return;

		m_AllKeys.RemoveItem(this);
	}

	override void EEOnAfterLoad()
	{
		auto trace = EXTrace.Start(EXTrace.VEHICLES, this);
		
		super.EEOnAfterLoad();

		GetKeyObject();

		//if ( !m_Vehicle && IsPaired() )
		//{
			//EXPrint(ToString() + "::EEOnAfterLoad - Unpairing \"" + GetDisplayName() + "\" " + GetPosition() + " because its vehicle no longer exists");
			//Unpair( true );
		//}
	}

	override void OnVariablesSynchronized()
	{
		auto trace = EXTrace.Start(EXTrace.VEHICLES, this);
		
		super.OnVariablesSynchronized();

		TIntArray id = {m_PersistentIDA, m_PersistentIDB, m_PersistentIDC, m_PersistentIDD};
		EXTrace.Print(EXTrace.VEHICLES, this, "Key ID: " + ExpansionStatic.IntToHex(id));
	}

	override void OnInventoryEnter(Man player)
	{
		super.OnInventoryEnter(player);

		PlayerBase pb;
		if (Class.CastTo(pb, player))
		{
			pb.SetHasCarKey(this, true);
		}
	}
	
	override void OnInventoryExit(Man player)
	{
		super.OnInventoryExit(player);

		PlayerBase pb;
		if (Class.CastTo(pb, player))
		{
			pb.SetHasCarKey(this, false);
		}
	}

	int GetMasterUses()
	{
		if ( m_IsMasterKey && MasterKeyUses == -1 )
		{
			//! Key from old storage. Initialize MasterKeyUses from Expansion settings.
			MasterKeyUses = GetExpansionSettings().GetVehicle().MasterKeyUses;
		}

		return MasterKeyUses;
	}

	void SetMasterUses(int amount = 0, bool synch = true)
	{
		MasterKeyUses = amount;

		if ( synch )
			SetSynchDirty();
	}

	bool IsMaster()
	{
		return m_IsMasterKey;
	}

	void SetMaster(bool state)
	{
		m_IsMasterKey = state;
	}

	string Expansion_GetPairedVehicleType()
	{
		string vehicleType;
		m_Expansion_NetsyncData.Get(0, vehicleType);
		return vehicleType;
	}

	Object GetKeyObject()
	{
		if ( !m_Vehicle )
		{
			CF_Log.Debug(ToString() + "::GetKeyObject - looking for vehicle");

			auto node = CarScript.s_Expansion_AllVehicles.m_Head;
			while (node)
			{
				CarScript car = node.m_Value;
				if ( IsPairedTo( car ) )
				{
					EXPrint(ToString() + "::GetKeyObject - found " + car);
					m_Vehicle = car;
					break;
				}
				node = node.m_Next;
			}
		}

		return m_Vehicle;
	}

	int GetPersistentIDA()
	{		
		return m_PersistentIDA;
	}

	int GetPersistentIDB()
	{		
		return m_PersistentIDB;
	}

	int GetPersistentIDC()
	{		
		return m_PersistentIDC;
	}

	int GetPersistentIDD()
	{		
		return m_PersistentIDD;
	}

	/**
	 * @note 	Does not get keys which may be stored in a 
	 * 			player inventory while they are not logged in
	 */
	static ExpansionCarKey GetFirstKeyForVehicle( Object obj )
	{
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
		
		return NULL;
	}

	/**
	 * @note 	Does not get keys which may be stored in a 
	 * 			player inventory while they are not logged in
	 */
	static void GetKeysForVehicle( Object obj, inout array< ExpansionCarKey > keys )
	{
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
	}
	
	override bool NameOverride( out string output )
	{
		if ( IsPaired() )
		{
			string vehicleType;
			string name;
			if (m_Expansion_NetsyncData.Get(0, vehicleType) && vehicleType != string.Empty)
				name = ExpansionStatic.GetItemDisplayNameWithType(vehicleType);
			else
				name = ConfigGetString("displayName");

			if ( IsMaster() )
			{
				output = name + " #STR_EXPANSION_MASTER_KEYS";
			} else {
				output = name + " #STR_EXPANSION_KEYS";
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

	void PairToVehicle( CarScript vehicle )
	{
		auto trace = EXTrace.Start(EXTrace.VEHICLES, this);

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

		TIntArray id = {m_PersistentIDA, m_PersistentIDB, m_PersistentIDC, m_PersistentIDD};
		EXTrace.Print(EXTrace.VEHICLES, this, "Key ID: " + ExpansionStatic.IntToHex(id));

		m_Expansion_NetsyncData.Set(0, vehicle.GetType());
		m_Vehicle = vehicle;

		m_Expansion_NetsyncData.Send(null);
		SetSynchDirty();
	}

	void PairWithMasterKey( ExpansionCarKey key )
	{
		auto trace = EXTrace.Start(EXTrace.VEHICLES, this);

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

		TIntArray id = {m_PersistentIDA, m_PersistentIDB, m_PersistentIDC, m_PersistentIDD};
		EXTrace.Print(EXTrace.VEHICLES, this, "Key ID: " + ExpansionStatic.IntToHex(id));

		m_Expansion_NetsyncData.Set(0, key.Expansion_GetPairedVehicleType());
		m_Vehicle = key.GetKeyObject();

		m_Expansion_NetsyncData.Send(null);
		SetSynchDirty();
	}

	void PairToVehicle( ExpansionVehicleBase vehicle )
	{
		m_Vehicle = vehicle;
		
		// TODO
	}

	void Unpair( bool ignore_check = false )
	{
		auto trace = EXTrace.Start(EXTrace.VEHICLES, this);

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
		m_Expansion_NetsyncData.Set(0, "");
		
		m_Expansion_NetsyncData.Send(null);
		SetSynchDirty();
	}

	protected void KeyMessage( string message )
	{
		#ifdef EXPANSION_CARKEY_LOGGING	
		#ifdef JM_COT
		if ( IsMissionClient() )
		{
			Message( GetPlayer(), message );
		}
		#endif
		Print( message );
		#endif
	}

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

	bool IsPairedTo( ExpansionVehicleBase vehicle )
	{
		// TODO
		
		return false;
	}

	#ifdef EXPANSION_MODSTORAGE
	override void CF_OnStoreSave(CF_ModStorageMap storage)
	{
		super.CF_OnStoreSave(storage);

		auto ctx = storage[DZ_Expansion_Vehicles];
		if (!ctx) return;

		ctx.Write(m_PersistentIDA);
		ctx.Write(m_PersistentIDB);
		ctx.Write(m_PersistentIDC);
		ctx.Write(m_PersistentIDD);

		string vehicleType;
		m_Expansion_NetsyncData.Get(0, vehicleType);
		ctx.Write(vehicleType);

		ctx.Write(MasterKeyUses);
		ctx.Write(m_IsMasterKey);
	}
	
	override bool CF_OnStoreLoad(CF_ModStorageMap storage)
	{
		if (!super.CF_OnStoreLoad(storage))
			return false;

		auto ctx = storage[DZ_Expansion_Vehicles];
		if (!ctx) return true;

		if (!ctx.Read(m_PersistentIDA))
			return false;

		if (!ctx.Read(m_PersistentIDB))
			return false;

		if (!ctx.Read(m_PersistentIDC))
			return false;

		if (!ctx.Read(m_PersistentIDD))
			return false;

		string vehicleType;
		if (!ctx.Read(vehicleType))
			return false;

		m_Expansion_NetsyncData.Set(0, vehicleType);

		if (!ctx.Read(MasterKeyUses))
			return false;

		if (!ctx.Read(m_IsMasterKey))
			return false;

		return true;
	}
	#endif
	
	override void OnRPC( PlayerIdentity sender, int rpc_type, ParamsReadContext ctx )
	{
		super.OnRPC(sender, rpc_type, ctx);
		
		m_Expansion_NetsyncData.OnRPC(sender, rpc_type, ctx);
	}
};
