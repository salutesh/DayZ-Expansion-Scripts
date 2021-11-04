/**
 * ItemBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ItemBase
{
	//! TODO: After we finally switch over to CF_ModStorage, we may still need a way to access Expansion storage save version outside of CF_OnStoreLoad/Save (e.g. AfterStoreLoad).
	//! For now, CF_OnStoreLoad will set it as well when used.
	protected int m_ExpansionSaveVersion;

	//! Skinning
	protected ExpansionSkinModule m_SkinModule;
	protected ExpansionSkin m_CurrentSkin;

	protected string m_CurrentSkinName;
	protected int m_CurrentSkinIndex;
	protected int m_CurrentSkinSynchRemote;

	protected bool m_CanBeSkinned;
	protected autoptr array< ExpansionSkin > m_Skins;
	protected bool m_Expansion_SZCleanup;
	protected bool m_Expansion_IsStoreLoaded;

	void ItemBase()
	{
		ExpansionSetupSkins();

		RegisterNetSyncVariableInt( "m_CurrentSkinSynchRemote", 0, m_Skins.Count() );
	}
	
	//============================================
	// GetExpansionSaveVersion
	// OBSOLETE
	//============================================
	int GetExpansionSaveVersion()
	{
		return m_ExpansionSaveVersion;
	}
	
	//============================================
	// OnStoreSave
	//============================================
	override void OnStoreSave( ParamsWriteContext ctx )
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ItemBase::OnStoreSave - Start");
		#endif

		#ifdef EXPANSION_STORAGE_DEBUG
		EXPrint("[CORE] ItemBase::OnStoreSave " + this + " " + GetGame().SaveVersion());
		#endif

		//! If we are saving after game version target for ModStorage support (1st stable)
		#ifdef CF_MODULE_MODSTORAGE
		if ( GetGame().SaveVersion() > EXPANSION_VERSION_GAME_MODSTORAGE_TARGET )
		{
			super.OnStoreSave( ctx );
			return;
		}
		#endif

		m_ExpansionSaveVersion = EXPANSION_VERSION_CURRENT_SAVE;
		ctx.Write( m_ExpansionSaveVersion );

		//! If we are saving game version target for ModStorage support (1st stable) or later
		#ifdef CF_MODULE_MODSTORAGE
		if ( GetGame().SaveVersion() >= EXPANSION_VERSION_GAME_MODSTORAGE_TARGET )
		{
			super.OnStoreSave( ctx );
			return;
		}
		#endif

		super.OnStoreSave( ctx );

		ctx.Write( m_CurrentSkinName );
        
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ItemBase::OnStoreSave - End");
		#endif
	}
		
	//============================================
	// OnStoreLoad
	//============================================
	override bool OnStoreLoad( ParamsReadContext ctx, int version )
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("[CORE] ItemBase::OnStoreLoad - Start");
		#endif

		#ifdef EXPANSION_STORAGE_DEBUG
		EXPrint("[CORE] ItemBase::OnStoreLoad " + this + " " + version);
		#endif

		#ifdef CF_MODULE_MODSTORAGE
		if ( version > EXPANSION_VERSION_GAME_MODSTORAGE_TARGET )
			return super.OnStoreLoad( ctx, version );
		#endif

		if ( Expansion_Assert_False( ctx.Read( m_ExpansionSaveVersion ), "[CORE] [" + this + "] Failed reading m_ExpansionSaveVersion" ) )
			return false;

		#ifdef CF_MODULE_MODSTORAGE
		if ( m_ExpansionSaveVersion > EXPANSION_VERSION_SAVE_MODSTORAGE_TARGET )
			return super.OnStoreLoad( ctx, version );
		#endif

		if ( Expansion_Assert_False( super.OnStoreLoad( ctx, version ), "[CORE] [" + this + "] Failed reading OnStoreLoad super" ) )
			return false;

		if ( GetExpansionSaveVersion() < 22 )
			return true;

		string currentSkinName = m_CurrentSkinName;

		if ( Expansion_Assert_False( ctx.Read( m_CurrentSkinName ), "[" + this + "] Failed reading m_CurrentSkinName" ) )
			return false;

		if ( m_CurrentSkinName == "" )
			m_CurrentSkinName = currentSkinName;

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("[CORE] ItemBase::OnStoreLoad - End");
		#endif

		return true;
	}

	#ifdef CF_MODULE_MODSTORAGE
	override void CF_OnStoreSave( CF_ModStorage storage, string modName )
	{
		#ifdef EXPANSION_STORAGE_DEBUG
		EXPrint("[CORE] ItemBase.CF_OnStoreSave " + this + " " + modName);
		#endif

		super.CF_OnStoreSave( storage, modName );

		if ( modName != "DZ_Expansion_Core" )
			return;

		storage.Write( m_CurrentSkinName );
	}
	
	override bool CF_OnStoreLoad( CF_ModStorage storage, string modName )
	{
		#ifdef EXPANSION_STORAGE_DEBUG
		EXPrint("[CORE] ItemBase::CF_OnStoreLoad " + this + " " + modName);
		#endif

		if ( !super.CF_OnStoreLoad( storage, modName ) )
			return false;

		if ( modName != "DZ_Expansion_Core" )
			return true;

		m_ExpansionSaveVersion = storage.GetVersion();

		if ( GetExpansionSaveVersion() < 22 )
			return true;

		string currentSkinName = m_CurrentSkinName;

		if ( Expansion_Assert_False( storage.Read( m_CurrentSkinName ), "[" + this + "] Failed reading m_CurrentSkinName" ) )
			return false;

		if ( m_CurrentSkinName == "" )
			m_CurrentSkinName = currentSkinName;

		return true;
	}
	#endif
	
	override void EEOnAfterLoad()
	{
		#ifdef EXPANSIONEXPRINT
		Print("[CORE] ItemBase::EEOnAfterLoad - Start");
		#endif

		super.EEOnAfterLoad();

		#ifdef EXPANSION_SKIN_LOGGING
		Print( m_CanBeSkinned );
		Print( m_CurrentSkinName );
		#endif

		if ( m_CanBeSkinned )
		{
			m_CurrentSkinIndex = m_SkinModule.GetSkinIndex( GetType(), m_CurrentSkinName );
			m_CurrentSkinSynchRemote = m_CurrentSkinIndex;
			m_CurrentSkin = m_Skins[ m_CurrentSkinIndex ];

			ExpansionOnSkinUpdate();
		}
		#ifdef EXPANSION_SKIN_LOGGING
		Print( m_CurrentSkinIndex );
		Print( m_CurrentSkin );
		#endif
		
		#ifdef EXPANSIONEXPRINT
		Print("[CORE] ItemBase::EEOnAfterLoad - End");
		#endif
	}

	bool IsLocked()
	{
		return false;
	}
	
	//! Return true if the item contains liquid
	bool ExpansionIsLiquidItem()
	{
		return IsLiquidContainer() || IsKindOf("SodaCan_ColorBase");
	}

	/*! ExpansionActionOn<...>

		Override these in inherited items and call them from within an action
		if you want the item to react to different phases of the action.
	*/

	void ExpansionActionOnStart( ActionData action_data )
	{
	}

	void ExpansionActionOnAnimationEvent( ActionData action_data )
	{
	}

	void ExpansionActionOnStartAnimationLoop( ActionData action_data )
	{
	}

	void ExpansionActionOnUpdate( ActionData action_data )
	{
	}

	void ExpansionActionOnFinishProgress( ActionData action_data )
	{
	}

	void ExpansionActionOnEndRequest( ActionData action_data )
	{
	}

	void ExpansionActionOnEndInput( ActionData action_data )
	{
	}

	void ExpansionActionOnEndAnimationLoop( ActionData action_data )
	{
	}

	void ExpansionActionOnEnd( ActionData action_data )
	{
	}
	
	void ExpansionActionOnStageStart( SequentialActionData action_data )
	{
	}

	void ExpansionActionOnStageEnd( SequentialActionData action_data )
	{
	}

	// This is just here so it can be overriden in derived classes, but should return a useful default.
	bool CanBeDamaged()
	{
		// TODO: Hmmm. Being able to destroy is not necessarily the same as being able to damage, 
		// although the former includes the ability to do the latter, just not vice versa.
		// For now this should suffice, as it's unlikely that an item that cannot be destroyed will be able to get damaged?
		return GetCanBeDestroyed();
	}

	protected string GetDestroySound()
	{
		return "";
	}

	override void OnVariablesSynchronized()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("[CORE] ItemBase::OnVariablesSynchronized - Start");
		#endif

		super.OnVariablesSynchronized();

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
		EXPrint("[CORE] ItemBase::OnVariablesSynchronized - End");
		#endif
	}

	//============================================
	// ExpansionSetupSkins
	//============================================
	protected void ExpansionSetupSkins()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ItemBase::ExpansionSetupSkins Start");
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
		EXPrint("ItemBase::ExpansionSetupSkins End");
		#endif
	}

	array< ExpansionSkin > ExpansionGetSkins()
	{
		return m_Skins;
	}

	bool ExpansionHasSkin( int skinIndex )
	{
		return skinIndex > -1 && skinIndex < m_Skins.Count();
	}

	//============================================
	// EEHealthLevelChanged
	//============================================
	override void EEHealthLevelChanged( int oldLevel, int newLevel, string zone )
	{
		super.EEHealthLevelChanged( oldLevel, newLevel, zone );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ItemBase::EEHealthLevelChanged Start");
		#endif

		if ( m_CanBeSkinned && m_CurrentSkin )
		{
			string sZone = zone;
			sZone.ToLower();

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
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ItemBase::EEHealthLevelChanged End");
		#endif
	}

	//============================================
	// ExpansionSetSkin
	//============================================
	void ExpansionSetSkin( int skinIndex )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ItemBase::ExpansionSetSkin Start");
		#endif

		#ifdef EXPANSION_SKIN_LOGGING
		Print( m_CanBeSkinned );
		Print( skinIndex );
		#endif

		if ( !m_CanBeSkinned )
		{
			m_CurrentSkinName = "";

			#ifdef EXPANSIONEXPRINT
			EXPrint("ItemBase::ExpansionSetSkin End");
			#endif

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
		EXPrint("ItemBase::ExpansionSetSkin End");
		#endif
	}

	//============================================
	// ExpansionOnSkinDamageZoneUpdate
	//============================================
	void ExpansionOnSkinDamageZoneUpdate( ExpansionSkinDamageZone zone, int level )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ItemBase::ExpansionOnSkinDamageZoneUpdate Start");
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
		EXPrint("ItemBase::ExpansionOnSkinDamageZoneUpdate End");
		#endif
	}

	//============================================
	// ExpansionOnSkinUpdate
	//============================================
	void ExpansionOnSkinUpdate()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ItemBase::ExpansionOnSkinUpdate Start");
		#endif

		if ( !m_CurrentSkin )
		{
			// Removed the public log, was spamming everyone
			#ifdef EXPANSIONEXPRINT
			EXPrint("ItemBase::ExpansionOnSkinUpdate called but m_CurrentSkin is NULL!");
			#endif

			return;
		}
		
		for ( int i = 0; i < m_CurrentSkin.HiddenSelections.Count(); i++ )
		{
			ExpansionSkinHiddenSelection selection = m_CurrentSkin.HiddenSelections[ i ];

			int selectionIndex = GetHiddenSelectionIndex( selection.HiddenSelection );

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
		EXPrint("ItemBase::ExpansionOnSkinUpdate End");
		#endif
	}

	// ------------------------------------------------------------
	// EEKilled
	// ------------------------------------------------------------
	override void EEKilled( Object killer )
	{
		super.EEKilled( killer );

		//TODO: store as global variable until new CF module system is added?
		ExpansionItemBaseModule module;
		if (Class.CastTo(module, GetModuleManager().GetModule(ExpansionItemBaseModule)))
		{
			module.PlayDestroySound(GetPosition(), GetDestroySound());
		}

		ExpansionOnDestroyed( killer );
	}

	void ExpansionOnDestroyed( Object killer )
	{
	}

	//! This deals with spawning magazines on weapons correctly and should be used as a replacement for vanilla CreateInInventory
	EntityAI ExpansionCreateInInventory(string className, bool attachOnly = false)
	{
		if (!GetInventory())
			return NULL;

		//! NOTE: Both actual magazines and ammon inherit from Magazine_Base, so we check destroyOnEmpty if it's actually a mag or not
		if (IsInherited(Weapon_Base) && GetGame().ConfigIsExisting("CfgMagazines " + className) && !GetGame().ConfigGetInt("CfgMagazines " + className + " destroyOnEmpty"))
		{
			//! It's an actual magazine
			Weapon_Base weapon = Weapon_Base.Cast(this);

			InventoryLocation il = new InventoryLocation;
			il.SetAttachment(weapon, NULL, InventorySlots.MAGAZINE);
			
			EntityAI magazine = SpawnEntity(className, il, ECE_IN_INVENTORY, RF_DEFAULT);
			if (!magazine)
				return NULL;

			if (GetGame().IsServer())
			{
				GetGame().RemoteObjectDelete(magazine);
				GetGame().RemoteObjectTreeDelete(weapon);
			}

			//! Important: Needs to be called BEFORE pushing bullet to chamber, otherwise save will occur in FSM transition
			int stateId = weapon.ExpansionGetMagAttachedFSMStateID();

			int muzzleIndex = weapon.GetCurrentMuzzle();
			if (!weapon.IsChamberFull(muzzleIndex) || weapon.IsChamberFiredOut(muzzleIndex))
			{
				EXPrint("ExpansionCreateInInventory - pushing to chamber " + weapon);
				pushToChamberFromAttachedMagazine(weapon, muzzleIndex);
			}

			if (stateId > -1)
			{
				ScriptReadWriteContext ctx = new ScriptReadWriteContext;
				ctx.GetWriteContext().Write(stateId);
				weapon.LoadCurrentFSMState(ctx.GetReadContext(), GetGame().SaveVersion());
			}

			if (GetGame().IsServer())
			{
				GetGame().RemoteObjectTreeCreate(weapon);
				GetGame().RemoteObjectCreate(magazine);
			}
			else
			{
				weapon.ShowMagazine();
			}

			//! We are done
			return magazine;
		}

		//! Any other item
		if (attachOnly)
			return GetInventory().CreateAttachment(className);
		else
			return GetInventory().CreateInInventory(className);
	}

	EntityAI ExpansionCreateAttachment(string className)
	{
		return ExpansionCreateInInventory(className, true);
	}

	void OnEnterZone(ExpansionZoneType type)
	{
	}

	void OnExitZone(ExpansionZoneType type)
	{
	}
	
	override void EEItemLocationChanged(notnull InventoryLocation oldLoc, notnull InventoryLocation newLoc)
	{
		super.EEItemLocationChanged(oldLoc, newLoc);

		if (!GetGame().IsServer())
			return;

		if (!GetExpansionSettings().GetSafeZone().Enabled)
			return;

		if (!GetExpansionSettings().GetSafeZone().EnableForceSZCleanup)
			return;

		//! Ignore unknown old location (item loaded from storage or newly created by CE or script)
		if (oldLoc.GetType() == InventoryLocationType.UNKNOWN)
			return;

		#ifdef EXPANSION_SAFEZONE_DEBUG
		EXPrint("[CORE][Expansion_SZCleanup] " + ToString() + " " + GetPosition() + " location changed " + typename.EnumToString(InventoryLocationType, oldLoc.GetType()) + " -> " + typename.EnumToString(InventoryLocationType, newLoc.GetType()));
		#endif

		if (newLoc.GetType() == InventoryLocationType.GROUND)
		{
			ExpansionCreateCleanup();
		}
		else if (oldLoc.GetType() == InventoryLocationType.GROUND)
		{
			float lifetime = GetEconomyProfile().GetLifetime();
			if (m_Expansion_SZCleanup || GetLifetimeMax() < lifetime)
			{
				m_Expansion_SZCleanup = false;

				//! Make sure to reset max lifetime to value from CE
				SetLifetimeMax(lifetime);

				#ifdef EXPANSION_SAFEZONE_DEBUG
				EXPrint("[CORE][Expansion_SZCleanup] " + ToString() + " " + GetPosition() + " unmarked for cleanup - lifetime " + GetLifetime());
				#endif
			}
		}
	}

	override void DeferredInit()
    {
		super.DeferredInit();

		if (!m_Expansion_IsStoreLoaded)
			return;

		if (!GetExpansionSettings().GetSafeZone().Enabled)
			return;

		if (!GetExpansionSettings().GetSafeZone().EnableForceSZCleanup)
			return;

		ExpansionCreateCleanup();
    }

	override void AfterStoreLoad()
	{
		super.AfterStoreLoad();
		
		m_Expansion_IsStoreLoaded = true;
	}

	override void OnCEUpdate()
	{
		super.OnCEUpdate();

		if (m_Expansion_SZCleanup && GetLifetime() < 0)
		{
			if (!GetHierarchyParent())
			{
				GetGame().ObjectDelete(this);

				#ifdef EXPANSION_SAFEZONE_DEBUG
				EXPrint("[CORE][Expansion_SZCleanup] " + ToString() + " " + GetPosition() + " deleted");
				#endif
			}
			else
			{
				//! Failsafe - this should not be able to happen
				m_Expansion_SZCleanup = false;

				EXPrint("[CORE][Expansion_SZCleanup] " + ToString() + " " + GetPosition() + " unmarked for cleanup - object has parent");
			}
		}
	}

	void ExpansionCreateCleanup()
	{
		if (IsInherited(ExpansionTemporaryOwnedContainer))
			return;

		if (m_Expansion_SZCleanup)
			return;

		if (GetHierarchyParent())
			return;

		if (!ExpansionZoneModule.IsInsideSafeZone(GetPosition()))
			return;

		foreach (string name: GetExpansionSettings().GetSafeZone().ForceSZCleanup_ExcludedItems)
		{
			if (IsKindOf(name))
				return;
		}

		m_Expansion_SZCleanup = true;

		//! Using SetLifetimeMax here prevents the lifetime resetting to the value from CE
		//! if this item can receive attachments/cargo and something is attached/put in cargo
		//! while the item is on ground
		SetLifetimeMax(GetExpansionSettings().GetSafeZone().ItemLifetimeInSafeZone);

		#ifdef EXPANSION_SAFEZONE_DEBUG
		EXPrint("[CORE][Expansion_SZCleanup] " + ToString() + " " + GetPosition() + " marked for cleanup - lifetime " + GetLifetime());
		#endif
	}
};
