/*
 * CarScript.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

//TODO
//TODO REMOVE WHAT IS DUPLICATED FROM Vehicles/Scripts/4_World
//TODO

/**@class		CarScript
 * @brief		
 **/
modded class CarScript
{
	// Vehicle markers
	protected ExpansionMarkerModule m_MarkerModule;
	protected string m_ServerMarker;
	protected static int m_ServerMarkerIndex = 0;
	
	// Safezone
	protected bool m_SafeZone;
	protected bool m_SafeZoneSynchRemote;
	
	// ------------------------------------------------------------
	// Constructor
	// ------------------------------------------------------------
	void CarScript()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::CarScript - Start");
		#endif

		m_MarkerModule = ExpansionMarkerModule.Cast( GetModuleManager().GetModule( ExpansionMarkerModule ) );
		m_ServerMarker = "";

		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::CarScript - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Destructor
	// ------------------------------------------------------------
	void ~CarScript()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::~CarScript - Start");
		#endif

		//RemoveServerMarker();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::~CarScript - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion CreateServerMarker
	// ------------------------------------------------------------
	void CreateServerMarker()
	{
		if ( IsMissionHost() )
		{
			if ( m_ServerMarker != "" )
				return;
			
			m_ServerMarkerIndex++;
			m_ServerMarker = "[DBG:" + m_ServerMarkerIndex + "]" + GetDisplayName();
			
			if ( IsCar() )
				m_MarkerModule.CreateServerMarker( m_ServerMarker, "Car", GetPosition(), ARGB(255, 255, 0, 0), true );
			else if ( IsHelicopter() )
				m_MarkerModule.CreateServerMarker( m_ServerMarker, "Helicopter", GetPosition(), ARGB(255, 0, 255, 0), true );
			else if ( IsBoat() )
				m_MarkerModule.CreateServerMarker( m_ServerMarker, "Boat", GetPosition(), ARGB(255, 0, 0, 255), true );
		}
	}
	
	// ------------------------------------------------------------
	// Expansion RemoveServerMarker
	// ------------------------------------------------------------
	void RemoveServerMarker()
	{
		if ( IsMissionHost() )
		{
			if ( m_ServerMarker != "" )
			{
				m_MarkerModule.RemoveServerMarker( m_ServerMarker );
				m_ServerMarker = "";
			}
		}
	}

	// ------------------------------------------------------------
	bool IsInSafeZone()
	{
		return m_SafeZone;
	}

	// ------------------------------------------------------------
	override bool CanBeDamaged()
	{
		if ( GetExpansionSettings().GetSafeZone().Enabled && !GetExpansionSettings().GetSafeZone().EnableVehicleinvincibleInsideSafeZone )
		{
			if ( IsInSafeZone() )
			{
				return false;
			}
		}

		return super.CanBeDamaged();
	}

	// ------------------------------------------------------------
	// Called only server side
	// ------------------------------------------------------------
	void OnEnterSafeZone()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::OnEnterSafeZone - Start");
		#endif

		m_SafeZone = true;

		/*if ( IsMissionHost() )
		{
			m_SafeZoneSynchRemote = true;

			SetAllowDamage( false );

			SetSynchDirty();
		}

		if ( IsMissionClient() )
		{
			
		}
		*/

		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::OnEnterSafeZone - End");
		#endif
	}

	// ------------------------------------------------------------
	// Called only server side
	// ------------------------------------------------------------
	void OnLeftSafeZone()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::OnLeftSafeZone - Start");
		#endif
		
		m_SafeZone = false;

		/*if ( IsMissionHost() )
		{
			m_SafeZoneSynchRemote = false;

			SetAllowDamage( true );
			
			SetSynchDirty();
		}

		if ( IsMissionClient() )
		{
			
		}*/

		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::OnLeftSafeZone - End");
		#endif
	}

	// ------------------------------------------------------------
	// OnVariablesSynchronized
	// ------------------------------------------------------------
	override void OnVariablesSynchronized()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::OnVariablesSynchronized - Start");
		#endif

		super.OnVariablesSynchronized();

		if ( m_SafeZoneSynchRemote && !m_SafeZone )
		{
			OnEnterSafeZone();
		} else if ( !m_SafeZoneSynchRemote && m_SafeZone )
		{
			OnLeftSafeZone();
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::OnVariablesSynchronized - End");
		#endif
	}
	
	// ------------------------------------------------------------
	//! Called when entity is being created as new by CE/ Debug
	override void EEOnCECreate()
	{
		#ifdef EXPANSIONEXPRINT
		Print("CarScript::EEOnCECreate - Start");
		#endif

		super.EEOnCECreate();
		
		//! There is no need for this now as no wild car spawns with a key rn?!
		/*array< EntityAI > items = new array< EntityAI >;
		GetInventory().EnumerateInventory( InventoryTraversalType.PREORDER, items );
		for ( int i = 0; i < items.Count(); i++ )
		{
			ExpansionCarKey key;
			if ( Class.CastTo( key, items[i] ) )
			{
				PairKeyTo( key );
			}
		}*/
		
		if ( GetExpansionSettings().GetDebug().ShowVehicleDebugMarkers )
		{
			CreateServerMarker();
		}
		
		#ifdef EXPANSIONEXPRINT
		Print("CarScript::EEOnCECreate - End");
		#endif
	}
	
	// ------------------------------------------------------------
	override void OnStoreSave(ParamsWriteContext ctx)
	{
		#ifdef EXPANSION_STORAGE_DEBUG
		EXPrint("CarScript::OnStoreSave " + this + " " + GetGame().SaveVersion());
		#endif

		#ifdef CF_MODULE_MODSTORAGE
		if ( GetGame().SaveVersion() >= EXPANSION_VERSION_GAME_MODSTORAGE_TARGET )
		{
			super.OnStoreSave( ctx );
			return;
		}
		#endif

		super.OnStoreSave( ctx );
	}

	// ------------------------------------------------------------
	override bool OnStoreLoad( ParamsReadContext ctx, int version )
	{
		#ifdef EXPANSION_CARSCRIPT_LOGGING
		EXLogPrint("CarScript::OnStoreLoad - Start");
		#endif

		#ifdef EXPANSION_STORAGE_DEBUG
		EXPrint("CarScript::OnStoreLoad " + this + " " + version);
		#endif

		if ( Expansion_Assert_False( super.OnStoreLoad( ctx, version ), "[" + this + "] Failed reading OnStoreLoad super" ) )
			return false;

		#ifdef CF_MODULE_MODSTORAGE
		if ( version > EXPANSION_VERSION_GAME_MODSTORAGE_TARGET || m_ExpansionSaveVersion > EXPANSION_VERSION_SAVE_MODSTORAGE_TARGET )
			return true;
		#endif

		if (GetExpansionSaveVersion() < 16)
			return true;

		if ( GetExpansionSaveVersion() >= 21 )
			return true;

		string currentSkinName = m_CurrentSkinName;

		if ( Expansion_Assert_False( ctx.Read( m_CurrentSkinName ), "[" + this + "] Failed reading m_CurrentSkinName" ) )
			return false;

		if ( m_CurrentSkinName == "" )
			m_CurrentSkinName = currentSkinName;

		return true;
	}

	#ifdef CF_MODULE_MODSTORAGE
	override void CF_OnStoreSave( CF_ModStorage storage, string modName )
	{
		#ifdef EXPANSION_STORAGE_DEBUG
		EXPrint("CarScript::CF_OnStoreSave " + this + " " + modName);
		#endif

		super.CF_OnStoreSave( storage, modName );

		if ( modName != "DZ_Expansion" )
			return;
	}

	override bool CF_OnStoreLoad( CF_ModStorage storage, string modName )
	{
		#ifdef EXPANSION_STORAGE_DEBUG
		EXPrint("CarScript::CF_OnStoreLoad " + this + " " + modName);
		#endif

		if ( !super.CF_OnStoreLoad( storage, modName ) )
			return false;

		if ( modName != "DZ_Expansion" )
			return true;

		if ( GetExpansionSaveVersion() >= 21 )
			return true;

		string currentSkinName = m_CurrentSkinName;

		storage.Read( m_CurrentSkinName );

		if ( m_CurrentSkinName == "" )
			m_CurrentSkinName = currentSkinName;
		
		return true;
	}
	#endif
};