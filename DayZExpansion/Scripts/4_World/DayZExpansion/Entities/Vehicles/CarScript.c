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
	// Skinning
	protected ExpansionSkinModule m_SkinModule;
	protected ExpansionSkin m_CurrentSkin;

	protected string m_CurrentSkinName;
	protected int m_CurrentSkinIndex;
	protected int m_CurrentSkinSynchRemote;

	protected bool m_CanBeSkinned;
	protected autoptr array< ExpansionSkin > m_Skins;

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

		RegisterNetSyncVariableInt( "m_CurrentSkinSynchRemote" );
		
		Class.CastTo( m_SkinModule, GetModuleManager().GetModule( ExpansionSkinModule ) );

		ExpansionSetupSkins();

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

		if ( m_CanBeSkinned && m_CurrentSkinSynchRemote != m_CurrentSkinIndex )
		{
			m_CurrentSkinIndex = m_CurrentSkinSynchRemote;
			if ( m_CurrentSkinIndex >= 0 && m_CurrentSkinIndex < m_Skins.Count() )
			{
				m_CurrentSkinName = m_SkinModule.GetSkinName( GetType(), m_CurrentSkinIndex );
				m_CurrentSkin = m_Skins[ m_CurrentSkinIndex ];
			} else
			{
				m_CurrentSkinName = "";
				m_CurrentSkin = NULL;	
			}

			ExpansionOnSkinUpdate();
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::OnVariablesSynchronized - End");
		#endif
	}

	protected void ExpansionSetupSkins()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::ExpansionSetupSkins - Start");
		#endif

		m_Skins = new array< ExpansionSkin >;

		if ( Class.CastTo( m_SkinModule, GetModuleManager().GetModule( ExpansionSkinModule ) ) )
		{
			m_SkinModule.RetrieveSkins( GetType(), m_Skins, m_CurrentSkinName );
		}

		m_CanBeSkinned = m_Skins.Count() != 0;

		if ( m_CanBeSkinned )
		{
			if ( m_CurrentSkinName != "" )
			{
				m_CurrentSkinIndex = m_SkinModule.GetSkinIndex( GetType(), m_CurrentSkinName );
			} else
			{
				m_CurrentSkinIndex = 0;
				
				m_CurrentSkinName = m_SkinModule.GetSkinName( GetType(), m_CurrentSkinIndex );
			}
			
			m_CurrentSkinSynchRemote = m_CurrentSkinIndex;
			m_CurrentSkin = m_Skins[ m_CurrentSkinIndex ];

			ExpansionOnSkinUpdate();
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::ExpansionSetupSkins - End");
		#endif
	}	

	override void EEHealthLevelChanged( int oldLevel, int newLevel, string zone )
	{
		super.EEHealthLevelChanged( oldLevel, newLevel, zone );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::EEHealthLevelChanged - Start");
		#endif

		if ( m_CanBeSkinned )
		{
			string sZone = zone;
			sZone.ToLower();

			if ( m_CurrentSkin )
			{
				if ( m_CurrentSkin.DamageZones )
				{
					if ( m_CurrentSkin.DamageZones.Count() > 0 )
					{
						for ( int i = 0; i < m_CurrentSkin.DamageZones.Count(); i++ )
						{
							string cZone = m_CurrentSkin.DamageZones[i].Zone;
							cZone.ToLower();

							if ( cZone == sZone )
							{
								ExpansionOnSkinDamageZoneUpdate( m_CurrentSkin.DamageZones[i], newLevel );
							}
						}
					}
				}
			}
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::EEHealthLevelChanged - End");
		#endif
	}

	void ExpansionSetSkin( int skinIndex )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::ExpansionSetSkin - Start");
		#endif
		
		#ifdef EXPANSION_SKIN_LOGGING
		Print( m_CanBeSkinned );
		Print( skinIndex );
		#endif

		if ( !m_CanBeSkinned )
		{
			m_CurrentSkinName = "";
			return;
		}

		m_CurrentSkinIndex = skinIndex;

		if ( m_CurrentSkinIndex < 0 )
		{
			m_CurrentSkinIndex = 0;
		}

		if ( m_CurrentSkinIndex >= m_Skins.Count() )
		{
			m_CurrentSkinIndex = 0;
		}

		#ifdef EXPANSION_SKIN_LOGGING
		Print( m_CurrentSkinIndex );
		#endif

		m_CurrentSkinName = m_SkinModule.GetSkinName( GetType(), m_CurrentSkinIndex );
		m_CurrentSkinSynchRemote = m_CurrentSkinIndex;
		m_CurrentSkin = m_Skins[ m_CurrentSkinIndex ];

		#ifdef EXPANSION_SKIN_LOGGING
		Print( m_CurrentSkinName );
		Print( m_CurrentSkinSynchRemote );
		Print( m_CurrentSkin );
		#endif

		ExpansionOnSkinUpdate();

		SetSynchDirty();

		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::ExpansionSetSkin - End");
		#endif
	}

	void ExpansionOnSkinDamageZoneUpdate( ExpansionSkinDamageZone zone, int level )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::ExpansionOnSkinDamageZoneUpdate - Start");
		#endif
		
		#ifdef EXPANSION_SKIN_LOGGING
		Print( zone );
		Print( level );
		#endif

		for ( int i = 0; i < zone.HiddenSelections.Count(); i++ )
		{
			int selectionIndex = GetHiddenSelectionIndex( zone.HiddenSelections[i] );

			#ifdef EXPANSION_SKIN_LOGGING
			Print( "HiddenSelection: " + zone.HiddenSelections[i] );
			Print( "SelectionIndex: " + selectionIndex );
			#endif

			if ( level >= 0 && level < zone.HealthLevels.Count() )
			{
				ExpansionSkinHealthLevel healthLevel = zone.HealthLevels[level];

				#ifdef EXPANSION_SKIN_LOGGING
				Print( "RVTexture: " + healthLevel.RVTexture );
				Print( "RVMaterial: " + healthLevel.RVMaterial );
				#endif

				SetObjectTexture( selectionIndex, healthLevel.RVTexture );
				SetObjectMaterial( selectionIndex, healthLevel.RVMaterial );
			}
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::ExpansionOnSkinDamageZoneUpdate - End");
		#endif
	}

	void ExpansionOnSkinUpdate()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::ExpansionOnSkinUpdate - Start");
		#endif

		if ( !m_CurrentSkin )
		{
			#ifdef EXPANSIONEXPRINT
			EXPrint( "ItemBase::ExpansionOnSkinUpdate called but m_CurrentSkin is NULL!" );
			#endif

			return;
		}

		if ( m_CurrentSkin.HornEXT != "" )
		{
			m_HornSoundSetEXT = m_CurrentSkin.HornEXT;
		} else
		{
			m_HornSoundSetEXT = "Expansion_Horn_Ext_SoundSet";
		}
		
		if ( m_CurrentSkin.HornINT != "" )
		{
			m_HornSoundSetINT = m_CurrentSkin.HornINT;
		} else
		{		
			m_HornSoundSetINT = "Expansion_Horn_Int_SoundSet";
		}

		for ( int i = 0; i < m_CurrentSkin.HiddenSelections.Count(); i++ )
		{
			ExpansionSkinHiddenSelection selection = m_CurrentSkin.HiddenSelections[ i ];

			int selectionIndex = GetHiddenSelectionIndex(  selection.HiddenSelection );

			#ifdef EXPANSION_SKIN_LOGGING
			Print( "HiddenSelection: " + selection.HiddenSelection );
			Print( "SelectionIndex: " + selectionIndex );
			Print( "RVTexture: " + selection.RVTexture );
			Print( "RVMaterial: " + selection.RVMaterial );
			#endif

			SetObjectTexture( selectionIndex, selection.RVTexture );
			SetObjectMaterial( selectionIndex, selection.RVMaterial );
		}
		
		for ( i = 0; i < m_CurrentSkin.DamageZones.Count(); i++ )
		{
			ExpansionOnSkinDamageZoneUpdate( m_CurrentSkin.DamageZones[i], GetHealthLevel( m_CurrentSkin.DamageZones[i].Zone ) );
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("CarScript::ExpansionOnSkinUpdate - End");
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
		#ifdef CF_MODULE_MODSTORAGE
		if ( CF_ModStorage.VERSION > 1 )
		{
			super.OnStoreSave( ctx );
			return;
		}
		#endif

		super.OnStoreSave( ctx );

		ctx.Write( m_CurrentSkinName );
	}

	// ------------------------------------------------------------
	override bool OnStoreLoad( ParamsReadContext ctx, int version )
	{
		#ifdef EXPANSION_CARSCRIPT_LOGGING
		EXLogPrint("CarScript::OnStoreLoad - Start");
		#endif

		#ifdef CF_MODULE_MODSTORAGE
		if ( CF_ModStorage.VERSION > 1 )
			return super.OnStoreLoad( ctx, version );
		#endif

		if ( !super.OnStoreLoad( ctx, version ) )
			return false;

		if (GetExpansionSaveVersion() < 16)
			return true;

		if ( Expansion_Assert_False( ctx.Read( m_CurrentSkinName ), "[" + this + "] Failed reading m_CurrentSkinName" ) )
			return false;

		return true;
	}

	#ifdef CF_MODULE_MODSTORAGE
	override void CF_OnStoreSave( CF_ModStorage storage, string modName )
	{
		super.CF_OnStoreSave( storage, modName );

		if ( modName != "DZ_Expansion" )
			return;

		storage.Write( m_CurrentSkinName );
	}

	override bool CF_OnStoreLoad( CF_ModStorage storage, string modName )
	{
		if ( !super.CF_OnStoreLoad( storage, modName ) )
			return false;

		if ( modName != "DZ_Expansion" )
			return true;

		storage.Read( m_CurrentSkinName );
		
		return true;
	}
	#endif

	// ------------------------------------------------------------
	override void EEOnAfterLoad()
	{
		super.EEOnAfterLoad();

		if ( m_CanBeSkinned )
		{
			m_CurrentSkinIndex = m_SkinModule.GetSkinIndex( GetType(), m_CurrentSkinName );
			m_CurrentSkinSynchRemote = m_CurrentSkinIndex;
			m_CurrentSkin = ExpansionSkin.Cast( m_Skins[ m_CurrentSkinIndex ] );

			ExpansionOnSkinUpdate();
		}
	}
};