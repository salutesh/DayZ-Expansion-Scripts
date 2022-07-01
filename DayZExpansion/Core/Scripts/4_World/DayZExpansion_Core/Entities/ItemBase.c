/**
 * ItemBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ItemBase
{
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

	bool m_Expansion_AcceptingAttachment;
	bool m_Expansion_CanPlayerAttach;
	bool m_Expansion_CanPlayerAttachSet;

	bool m_Expansion_IsAdminTool;
	bool m_Expansion_IsMeleeWeapon;

	void ItemBase()
	{
		m_Expansion_IsAdminTool = ConfigGetBool("expansionIsAdminTool");

		Expansion_SetIsMeleeWeapon();

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
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GENERAL_ITEMS, this, "OnStoreSave");
#endif

		CF_Log.Debug("[CORE] ItemBase::OnStoreSave " + this + " " + GetGame().SaveVersion());

		//! If we are saving after game version target for ModStorage support (1st stable)
		#ifdef EXPANSION_MODSTORAGE
		if ( GetGame().SaveVersion() > EXPANSION_VERSION_GAME_MODSTORAGE_TARGET )
		{
			super.OnStoreSave( ctx );
			return;
		}
		#endif

		m_ExpansionSaveVersion = EXPANSION_VERSION_CURRENT_SAVE;
		ctx.Write( m_ExpansionSaveVersion );

		//! If we are saving game version target for ModStorage support (1st stable) or later
		#ifdef EXPANSION_MODSTORAGE
		if ( GetGame().SaveVersion() >= EXPANSION_VERSION_GAME_MODSTORAGE_TARGET )
		{
			super.OnStoreSave( ctx );
			return;
		}
		#endif

		super.OnStoreSave( ctx );

		ctx.Write( m_CurrentSkinName );
	}
		
	//============================================
	// OnStoreLoad
	//============================================
	override bool OnStoreLoad( ParamsReadContext ctx, int version )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GENERAL_ITEMS, this, "OnStoreLoad");
#endif

		CF_Log.Debug("[CORE] ItemBase::OnStoreLoad " + this + " " + version);

		#ifdef EXPANSION_MODSTORAGE
		if ( version > EXPANSION_VERSION_GAME_MODSTORAGE_TARGET )
			return super.OnStoreLoad( ctx, version );
		#endif

		if ( Expansion_Assert_False( ctx.Read( m_ExpansionSaveVersion ), "[CORE] [" + this + "] Failed reading m_ExpansionSaveVersion" ) )
			return false;

		#ifdef EXPANSION_MODSTORAGE
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

		return true;
	}

	#ifdef EXPANSION_MODSTORAGE
	override void CF_OnStoreSave(CF_ModStorageMap storage)
	{
		super.CF_OnStoreSave(storage);

		auto ctx = storage[DZ_Expansion_Core];
		if (!ctx) return;

		ctx.Write(m_CurrentSkinName);
	}
	
	override bool CF_OnStoreLoad(CF_ModStorageMap storage)
	{
		if (!super.CF_OnStoreLoad(storage))
			return false;

		auto ctx = storage[DZ_Expansion_Core];
		if (!ctx) return true;

		//! @note needed so we can sucessfully load player inventory when they haven't connected in the transition period
		//! as well as menu character inventory (can cause client CTD otherwise)
		m_ExpansionSaveVersion = ctx.GetVersion();

		if (!ctx.Read(m_CurrentSkinName))
			return false;

		return true;
	}
	#endif

	bool ExpansionIsLocked()
	{
		return false;
	}

	bool IsLocked()
	{
		return false;
	}

	static void ExpansionPhaseObject(EntityAI other)
	{
#ifdef EXPANSIONMODVEHICLE
		ExpansionVehicleBase evb;
		CarScript car;

		if (Class.CastTo(car, other))
		{
			SetVelocity(other, car.m_State.m_LinearVelocity);
			dBodySetAngularVelocity(other, car.m_State.m_AngularVelocity);
		}
		else if (Class.CastTo(evb, other))
		{
			SetVelocity(other, evb.m_State.m_LinearVelocity);
			dBodySetAngularVelocity(other, evb.m_State.m_AngularVelocity);
		}
		else
#endif
		{
		}
	}

	void ExpansionCreateDynamicFromContact(EntityAI other, vector position, Contact data)
	{
		CreateDynamicPhysics(PhxInteractionLayers.DYNAMICITEM);
		EnableDynamicCCD(false);

		ExpansionHandleCollision(other, position);
	}

	void ExpansionHandleCollision(EntityAI other, vector position)
	{
		vector transform[4];
		GetTransform(transform);

		vector otherTransform[4];
		other.GetTransform(otherTransform);

		vector hitVelocity;
		float massA = dBodyGetMass(this);
		float massB = dBodyGetMass(other);
		float massNorm = 0.0;
		if (massA < massB)
			massNorm = (0.5 * massA / massB);
		else
			massNorm = 1.0 - (0.5 * massB / massA);

#ifdef EXPANSIONMODVEHICLE
		CarScript car;
		ExpansionVehicleBase evb;
		if (Class.CastTo(car, other))
		{
			hitVelocity = car.m_State.GetWorldVelocityAt(position.InvMultiply4(otherTransform)).Multiply3(transform);
		}
		else if (Class.CastTo(evb, other))
		{
			hitVelocity = evb.m_State.GetWorldVelocityAt(position.InvMultiply4(otherTransform)).Multiply3(transform);
		}
		else
#endif
		{
		}

		vector linearVelocity;
		vector angularVelocity;

		linearVelocity = -hitVelocity * (1.0 - massNorm);
		angularVelocity = position.InvMultiply4(otherTransform) * linearVelocity;

		SetVelocity(other, linearVelocity.InvMultiply3(otherTransform));
		dBodySetAngularVelocity(other, angularVelocity.InvMultiply3(otherTransform));

		linearVelocity = hitVelocity * massNorm;
		angularVelocity = position.InvMultiply4(transform) * linearVelocity;

		SetVelocity(this, linearVelocity.InvMultiply3(transform));
		dBodySetAngularVelocity(this, angularVelocity.InvMultiply3(transform));
	}
	
	//! Return true if the item contains liquid
	bool ExpansionIsLiquidItem()
	{
		return IsLiquidContainer() || IsKindOf("SodaCan_ColorBase");
	}

	void Expansion_SetIsMeleeWeapon()
	{
		if (IsWeapon())
			return;

		if (IsMeleeWeapon())
		{
			//! basically any item has isMeleeWeapon=1 in config, but we only want actual viable melee options
			//! (axes, knifes, sickles...)
			TStringArray inventorySlot();
			ConfigGetTextArray("inventorySlot", inventorySlot);
			if (inventorySlot.Find("Shoulder") == -1 && inventorySlot.Find("Melee") == -1 && inventorySlot.Find("Knife") == -1)
			{
				TStringArray itemInfo();
				ConfigGetTextArray("itemInfo", itemInfo);
				if (itemInfo.Find("Knife") == -1 && itemInfo.Find("Axe") == -1)
				{
					string suicideAnim = ConfigGetString("suicideAnim");
					if (!suicideAnim || suicideAnim != "woodaxe")
						return;
				}
			}
		}

		m_Expansion_IsMeleeWeapon = true;
	}

	bool Expansion_IsMeleeWeapon()
	{
		return m_Expansion_IsMeleeWeapon;
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
		return GetAllowDamage();
	}

	protected string GetDestroySound()
	{
		return "";
	}

	override void OnVariablesSynchronized()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GENERAL_ITEMS, this, "OnVariablesSynchronized");
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
	}

	//============================================
	// ExpansionSetupSkins
	//============================================
	protected void ExpansionSetupSkins()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SKIN, this, "ExpansionSetupSkins");
#endif

		m_Skins = new array< ExpansionSkin >;

		if ( CF_Modules<ExpansionSkinModule>.Get(m_SkinModule) )
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
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.GENERAL_ITEMS, this, "EEHealthLevelChanged");
#endif

		super.EEHealthLevelChanged( oldLevel, newLevel, zone );
	
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
	}

	//============================================
	// ExpansionSetSkin
	//============================================
	void ExpansionSetSkin( int skinIndex )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SKIN, this, "ExpansionSetSkin");
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
	}

	//============================================
	// ExpansionOnSkinDamageZoneUpdate
	//============================================
	void ExpansionOnSkinDamageZoneUpdate( ExpansionSkinDamageZone zone, int level )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SKIN, this, "ExpansionOnSkinDamageZoneUpdate");
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
	}

	//============================================
	// ExpansionOnSkinUpdate
	//============================================
	void ExpansionOnSkinUpdate()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.SKIN, this, "ExpansionOnSkinUpdate");
#endif

		if ( !m_CurrentSkin )
		{
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
	}

	// ------------------------------------------------------------
	// EEKilled
	// ------------------------------------------------------------
	override void EEKilled( Object killer )
	{
		super.EEKilled( killer );

		//TODO: store as global variable until new CF module system is added?
		ExpansionItemBaseModule module;
		if (CF_Modules<ExpansionItemBaseModule>.Get(module))
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
				#ifdef EXPANSIONEXPRINT
				EXPrint("ExpansionCreateInInventory - pushing to chamber " + weapon);
				#endif
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
		DayZPlayerImplement old_owner_dpi;
		DayZPlayerImplement new_owner_dpi;

		bool shouldSuper = true;
		
		if (oldLoc.GetParent())
			old_owner_dpi = DayZPlayerImplement.Cast(oldLoc.GetParent().GetHierarchyRootPlayer());
		
		if (newLoc.GetParent())
			new_owner_dpi = DayZPlayerImplement.Cast(newLoc.GetParent().GetHierarchyRootPlayer());

		//! super EEItemLocationChanged wants PlayerBase class, NPCs are DayZPlayerImplement so this is to prevent the super method from being called.
		if (old_owner_dpi && !PlayerBase.Cast(old_owner_dpi))
			shouldSuper = false;
		else if (new_owner_dpi && !PlayerBase.Cast(new_owner_dpi))
			shouldSuper = false;
		
		if (shouldSuper)
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
			float lifetime;
			if (GetEconomyProfile())
				lifetime = GetEconomyProfile().GetLifetime();
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

	override void OnInventoryEnter(Man player)
	{
		super.OnInventoryEnter(player);

		PlayerBase pb;
		if (Class.CastTo(pb, player))
			pb.Expansion_OnInventoryUpdate(this, true);
	}

	override void OnInventoryExit(Man player)
	{
		super.OnInventoryExit(player);

		PlayerBase pb;
		if (Class.CastTo(pb, player))
			pb.Expansion_OnInventoryUpdate(this, false);
	}

	override void OnWorkStart()
	{
		super.OnWorkStart();

		PlayerBase pb;
		if (Class.CastTo(pb, GetHierarchyRootPlayer()))
			pb.Expansion_OnInventoryUpdate(this, true, true);
	}

	override void OnWorkStop()
	{
		super.OnWorkStop();

		PlayerBase pb;
		if (Class.CastTo(pb, GetHierarchyRootPlayer()))
			pb.Expansion_OnInventoryUpdate(this, true, true);
	}

	typename Expansion_GetFamilyType()
	{
		if (IsInherited(ItemCompass))
		{
			return ItemCompass;
		}
		else if (IsInherited(ItemMap))
		{
			return ItemMap;
		}
		else
		{
			ExpansionString className = new ExpansionString(ClassName());
			if (className.EndsWith("_ColorBase"))
				return className.Get().ToType();
		}
		
		return Type();
	}

	override void DeferredInit()
    {
		super.DeferredInit();

		//! Skin

		if ( GetGame().IsServer() && m_CanBeSkinned )
		{
			m_CurrentSkinIndex = m_SkinModule.GetSkinIndex( GetType(), m_CurrentSkinName );
			m_CurrentSkinSynchRemote = m_CurrentSkinIndex;
			m_CurrentSkin = m_Skins[ m_CurrentSkinIndex ];

			ExpansionOnSkinUpdate();
		}

		//! Safezone item cleanup

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
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.CE, this, "AfterStoreLoad");
#endif

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

	bool Expansion_CanObjectAttach(Object obj)
	{
		return false;
	}

	bool Expansion_CarContactActivates()
	{
		if (IsInherited(CarWheel))
			return true;

		return false;
	}

	TStringArray Expansion_GetAttachmentSlots()
	{
		TStringArray attachments();

		string path;

		if (IsWeapon())
			path = CFG_WEAPONSPATH;
		else if (IsMagazine())
			path = CFG_MAGAZINESPATH;
		else
			path = CFG_VEHICLESPATH;

		path += " " + GetType();

		if (GetGame().ConfigIsExisting(path))
			GetGame().ConfigGetTextArray(path + " attachments", attachments);

		return attachments;
	}

	bool Expansion_HasAttachmentSlot(string slotName)
	{
		TStringArray attachments = Expansion_GetAttachmentSlots();

		return ExpansionStatic.StringArrayContainsIgnoreCase(attachments, slotName);
	}

	bool Expansion_IsAdminTool()
	{
		return m_Expansion_IsAdminTool;
	}
};
