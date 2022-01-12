/*
 * CarScript.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
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
	
	// ------------------------------------------------------------
	// Constructor
	// ------------------------------------------------------------
	void CarScript()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::CarScript - Start");
		#endif

		if ( IsMissionHost() )
		{
			m_MarkerModule = ExpansionMarkerModule.Cast( GetModuleManager().GetModule( ExpansionMarkerModule ) );
			m_ServerMarker = "";
		}

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
};
