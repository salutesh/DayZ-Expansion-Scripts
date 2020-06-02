/**
 * ItemBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ItemBase
{
	private int m_ExpansionSaveVersion;

	protected ExpansionSkinModule m_SkinModule;
	protected ExpansionSkin m_CurrentSkin;

	protected string m_CurrentSkinName;
	protected int m_CurrentSkinIndex;
	protected int m_CurrentSkinSynchRemote;

	protected bool m_CanBeSkinned;
	protected autoptr array< ref ExpansionSkin > m_Skins;
	
	protected EntityAI m_WorldAttachment;
	protected vector m_AttachmentTransform[4];
	protected bool m_IsAttached;
	protected int m_AttachIDA;
	protected int m_AttachIDB;
	protected int m_AttachIDC;
	protected int m_AttachIDD;

	protected dBlock m_Block;

	void ItemBase()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ItemBase::ItemBase Start");
		#endif

		ExpansionSetupSkins();
		
		RegisterNetSyncVariableInt( "m_CurrentSkinSynchRemote", 0, m_Skins.Count() );
		RegisterNetSyncVariableBool( "m_IsAttached" );

		#ifdef EXPANSIONEXPRINT
		EXPrint("ItemBase::ItemBase End");
		#endif
	}

	/**
	\brief Returning if can be built
		\param 	
	*/
	bool IsBasebuilding()
	{
		return false;
	}

	/**
	\brief Returning if can be opened
		\param 	
	*/
	bool IsOpenable()
	{
		return false;
	}
	
	/**
	\brief Returning if wall is open
		\param 	
	*/
	bool IsOpened()
	{
		return false;
	}

	/**
	\brief Returning if player can open gate from selection
		\param 
	*/
	bool CanOpen( PlayerBase player, string selection )
	{
		return false;
	}
	
	/**
	\brief Returning if player can close gate from selection
		\param 	
	*/
	bool CanClose( string selection )
	{
		return false;
	}

	/**
	\brief Opening gate on defined selection
		\param 	
	*/
	void Open( string selection ) 
	{
	}
	
	/**
	\brief Closing gate on defined selection
		\param 	
	*/
	void Close( string selection ) 
	{
	}
	
	/**
	\brief Set code of wall
		\param 	
	*/
	void SetCode( string code )
	{
	}
	
	/**
	\brief Returning code of wall
		\param 	
	*/
	string GetCode()
	{
		return "";
	}

	/**
	\brief Returning if the wall has a code
		\param 	
	*/
	bool HasCode()
	{
		return false;
	}

	/**
	\brief Returning if the wall has a code
		\param "selection" selection for codelock
	*/
	bool HasCodeLock( string selection )
	{
		return false;
	}

	/**
	\brief Returning if wall is locked
		\param 	
	*/
	bool IsLocked()
	{
		return false;
	}

	/**
	\brief Locking base build
		\param 	
	*/
	void Lock()
	{
	}
	
	/**
	\brief Unlocking base build
		\param 	
	*/
	void Unlock()
	{
	}
	
	private void SendServerLockReply(bool reply, bool injuring, PlayerIdentity sender)
	{
		ScriptRPC rpc = new ScriptRPC;
		rpc.Write( reply );
		rpc.Write( injuring );
		rpc.Send( this, ExpansionLockRPC.SERVERREPLY, true, sender );
	}
	
	override void OnRPC( PlayerIdentity sender, int rpc_type, ParamsReadContext ctx )
	{
		super.OnRPC(sender, rpc_type, ctx);
		
		//! Due to some weird dayz bug sender may sometimes not be null even when it could be so this check isn't really needed
		if ( GetGame().IsServer() && GetGame().IsMultiplayer() && !sender )
			return;
		
		string code = "";
		string selection = "";
		switch (rpc_type)
		{	
			case ExpansionLockRPC.LOCK:
			{
				if ( !IsMissionHost() )
					return;
				
				if ( !ctx.Read( selection ) )
				{
					Error("ItemBase::OnRPC ExpansionLockRPC.LOCK can't read selection");
					SendServerLockReply( false, false, sender );
					return;
				}
				
				if ( !HasCode() || IsLocked() )
				{
					SendServerLockReply( false, false, sender );
					return;
				}

				Lock();
				
				SendServerLockReply( true, false, sender );
				
				return;
			}
			
			case ExpansionLockRPC.UNLOCK:
			{
				if ( !IsMissionHost() )
					return;
				
				if ( !ctx.Read( code ) || code.Length() != 4 )
				{
					Error("ItemBase::OnRPC ExpansionLockRPC.UNLOCK can't read code");
					SendServerLockReply( false, false, sender );
					//TODO: notification here
					return;
				}

				if ( !ctx.Read( selection ) )
				{
					Error("ItemBase::OnRPC ExpansionLockRPC.UNLOCK can't read selection");
					SendServerLockReply( false, false, sender );
					//TODO: notification here
					return;
				}
				
				if ( !HasCode() || !IsLocked() )
				{
					Error("ItemBase::OnRPC ExpansionLockRPC.UNLOCK !HasCode() || !IsLocked()");
					SendServerLockReply( false, false, sender );
					return;
				}

				if ( GetCode() != code )
				{
					#ifdef EXPANSIONEXLOGPRINT
					EXLogPrint("ItemBase::OnRPC ExpansionLockRPC.UNLOCK GetCode() != code");
					#endif

					SendServerLockReply( false, true, sender );
					PlayerBase player = PlayerBase.GetPlayerByUID( sender.GetId() );
					if ( player )
					{
						player.DecreaseHealth( "", "Health", -10 );
					}

					return;
				}

				Unlock();
				SendServerLockReply( true, false, sender );
				return;
			}
			
			case ExpansionLockRPC.SET:
			{
				if ( !IsMissionHost() )
					return;
				
				if ( !ctx.Read( code ) || code.Length() != 4 )
				{
					Error("ItemBase::OnRPC ExpansionLockRPC.SET can't read code");
					SendServerLockReply( false, false, sender );
					//TODO: notification here
					return;
				}

				if ( !ctx.Read( selection ) )
				{
					Error("ItemBase::OnRPC ExpansionLockRPC.SET can't read selection");
					SendServerLockReply( false, false, sender );
					//TODO: notification here
					return;
				}
				
				if ( HasCode() )
				{
					// Base already has code so don't try setting it to another.
					SendServerLockReply( false, false, sender );
					return;
				}

				SetCode( code );

				SendServerLockReply( true, false, sender );
				return;
			}
			
			case ExpansionLockRPC.SERVERREPLY:
			{
				if ( !IsMissionClient() )
					return;
				
				bool reply;
				if ( !ctx.Read( reply ) )
				{
					Error("ItemBase::OnRPC ExpansionLockRPC.SERVERREPLY can't read reply");
					return;
				}

				bool injuring;
				if ( !ctx.Read( injuring ) )
				{
					Error("ItemBase::OnRPC ExpansionLockRPC.SERVERREPLY can't read injuring");
					return;
				}

				ExpansionLockUIBase menu;
				if ( !Class.CastTo( menu, GetGame().GetUIManager().FindMenu( MENU_EXPANSION_CODELOCK_MENU ) ) && !Class.CastTo( menu, GetGame().GetUIManager().FindMenu( MENU_EXPANSION_NUMPAD_MENU ) ) )
					return;
					
				menu.OnServerResponse( reply, injuring );
			}
		}
	}

	#ifdef EXPANSION_ITEM_ATTACHING
	override void EEItemLocationChanged( notnull InventoryLocation oldLoc, notnull InventoryLocation newLoc )
	{
		#ifdef EXPANSIONEXPRINT
		Print("ItemBase::EEItemLocationChanged - Start");
		#endif

		ExpansionAIBase new_player = null;
		ExpansionAIBase old_player = null;

		if( newLoc.GetParent() )
			new_player = ExpansionAIBase.Cast( newLoc.GetParent().GetHierarchyRootPlayer() );
		
		if( oldLoc.GetParent() )
			old_player = ExpansionAIBase.Cast( oldLoc.GetParent().GetHierarchyRootPlayer() );
		
		if ( !new_player && !old_player )
		{	
			super.EEItemLocationChanged( oldLoc, newLoc );

			//! Print("ItemBase::EEItemLocationChanged - End - Not AI");

			return;
		}

		EntityAI old_owner = oldLoc.GetParent();
		EntityAI new_owner = newLoc.GetParent();
		OnItemLocationChanged( old_owner, new_owner );

		if ( oldLoc.GetType() == InventoryLocationType.ATTACHMENT && newLoc.GetType() == InventoryLocationType.ATTACHMENT )
		{
			OnItemAttachmentSlotChanged(oldLoc,newLoc);
		}
		
		if ( oldLoc.GetType() == InventoryLocationType.ATTACHMENT )
		{
			if ( old_owner )
				OnWasDetached( old_owner, oldLoc.GetSlot() );
			else
				Error("EntityAI::EEItemLocationChanged - detached, but old_owner is null");
		}
		
		if ( newLoc.GetType() == InventoryLocationType.ATTACHMENT )
		{
			if ( new_owner )
				OnWasAttached( newLoc.GetParent(), newLoc.GetSlot() );
			else
				Error("EntityAI::EEItemLocationChanged - attached, but new_owner is null");
		}
		
		if ( newLoc.GetType() == InventoryLocationType.HANDS )
		{
			if ( new_player == old_player )
			{
			} else
			{
				if ( m_OldLocation )
				{
					m_OldLocation.Reset();
				}
			}
		}
		
		#ifdef EXPANSIONEXPRINT
		Print("ItemBase::EEItemLocationChanged - End");
		#endif
	}

	override void OnItemLocationChanged( EntityAI old_owner, EntityAI new_owner )
	{
		#ifdef EXPANSIONEXPRINT
		Print("ItemBase::OnItemLocationChanged - Start");
		#endif

		DayZPlayerImplement old_owner_dpi = NULL;
		DayZPlayerImplement new_owner_dpi = NULL;

		bool shouldSuper = true;
		
		if ( old_owner )
		{
			if ( old_owner.IsMan() )
			{
				old_owner_dpi = DayZPlayerImplement.Cast( old_owner );
			} else
			{
				old_owner_dpi = DayZPlayerImplement.Cast( old_owner.GetHierarchyRootPlayer() );
			}
		}

		if ( new_owner )
		{
			if ( new_owner.IsMan() )
			{
				new_owner_dpi = DayZPlayerImplement.Cast( new_owner );
			} else
			{
				new_owner_dpi = DayZPlayerImplement.Cast( new_owner.GetHierarchyRootPlayer() );
			}
		}

		//! super OnItemLocationChanged wants PlayerBase class, AI is Man so this is to prevent the super method from being called.
		if ( old_owner && old_owner.IsMan() && !PlayerBase.Cast( old_owner ) )
		{
			shouldSuper = false;
		} else if ( new_owner && new_owner.IsMan() && !PlayerBase.Cast( new_owner ) )
		{
			shouldSuper = false;
		}

		//! Print( shouldSuper );
		//! Print( old_owner );
		//! Print( new_owner );
		//! Print( old_owner_dpi );
		//! Print( new_owner_dpi );
		
		//! if ( new_owner )
		//!	Print( "new_owner IsMan: " + new_owner.IsMan() );

		//! if ( old_owner )
		//!	Print( "old_owner IsMan: " + old_owner.IsMan() );
		
		if ( shouldSuper )
		{
			//! Print( "ItemBase::OnItemLocationChanged - Start - Calling Super" );

			super.OnItemLocationChanged( old_owner, new_owner );

			//! Print( "ItemBase::OnItemLocationChanged - End - Calling Super" );
		}

		if ( !GetGame().IsServer() )
		{
			//! Print( "ItemBase::OnItemLocationChanged - End - Not Server" );

			return;
		}

		EntityAI parent = NULL;

		//! Attaching or detaching the items to the vehicle
		if ( old_owner_dpi && !new_owner ) // on drop
		{
			if ( dBodyIsDynamic( this ) && dBodyIsActive( this ) )
			{
				//! Print("ItemBase::OnItemLocationChanged - End - Is Dynamic");
				return;
			}

			if ( !Class.CastTo( parent, old_owner_dpi.GetParent() ) )
			{
				//! Print("ItemBase::OnItemLocationChanged - End - No Parent");
				return;
			}

			CarScript car;
			ExpansionVehicleScript veh;
			if ( !Class.CastTo( car, parent ) && !Class.CastTo( veh, parent ) )
			{
				//! Print("ItemBase::OnItemLocationChanged - End - Not Valid Parent");
				return;
			}

			//! Print( car );
			//! Print( veh );

			bool carAttach = car && car.CanObjectAttach( this );
			bool vehAttach = veh && veh.CanObjectAttach( this );

			//! Print( carAttach );
			//! Print( vehAttach );

			if ( carAttach || vehAttach )
			{
				vector tmItem[4];
				vector tmTarget[4];
				vector tmLocal[4];

				GetTransform( tmItem );
				parent.GetTransform( tmTarget );
				Math3D.MatrixInvMultiply4( tmTarget, tmItem, tmLocal );

				LinkToLocalSpaceOf( parent, tmLocal );
			}
		} else if ( new_owner_dpi && !old_owner ) //! On pickup
		{
			UnlinkFromLocalSpace();
		}
		
		#ifdef EXPANSIONEXPRINT
		Print("ItemBase::OnItemLocationChanged - End");
		#endif
	}
	#endif

	override bool IsInventoryVisible()
	{
		return ( m_IsAttached || super.IsInventoryVisible() );
	}

	void LinkToLocalSpaceOf( notnull EntityAI pParent, vector pLocalSpaceMatrix[4] )
	{
		Print( "ItemBase::LinkToLocalSpaceOf - Start - Target=" + pParent );

		if ( !GetGame().IsServer() )
		{
			Print( "ItemBase::LinkToLocalSpaceOf - End - Not Server" );

			return;
		}

		m_IsAttached = true;
		m_WorldAttachment = pParent;

		SetTransform( pLocalSpaceMatrix );

		m_WorldAttachment.AddChild( this, -1 );
		m_WorldAttachment.Update();

		// m_Block = dBodyCollisionBlock( m_WorldAttachment, this );

		Print( "ItemBase::LinkToLocalSpaceOf - End - Target=" + m_WorldAttachment );
	}

	void UnlinkFromLocalSpace()
	{
		Print( "ItemBase::UnlinkFromLocalSpace - Start" );

		if ( !GetGame().IsServer() )
		{
			Print( "ItemBase::UnlinkFromLocalSpace - End - Not Server" );

			return;
		}

		if ( !m_WorldAttachment )
		{
			Print( "ItemBase::UnlinkFromLocalSpace - End - No World Attachment" );

			return;
		}

		m_IsAttached = false;

		vector tmGlobal[4];

		GetTransform( tmGlobal );

		m_WorldAttachment.RemoveChild( this );

		SetTransform( tmGlobal );

		m_WorldAttachment.Update();
		m_WorldAttachment = NULL;

		Update();

		// dBodyRemoveBlock( this, m_Block );

		Print( "ItemBase::UnlinkFromLocalSpace - End" );
	}

	int GetExpansionSaveVersion()
	{
		return m_ExpansionSaveVersion;
	}
	
	override void OnStoreSave( ParamsWriteContext ctx )
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ItemBase::OnStoreSave - Start");
		#endif

		m_ExpansionSaveVersion = EXPANSION_VERSION_CURRENT_SAVE;
		ctx.Write( m_ExpansionSaveVersion );

		super.OnStoreSave( ctx );

		ctx.Write( m_CurrentSkinName );

		if ( m_WorldAttachment && m_IsAttached )
		{
			ctx.Write( m_IsAttached );

			m_WorldAttachment.GetPersistentID( m_AttachIDA, m_AttachIDB, m_AttachIDC, m_AttachIDD );

			ctx.Write( m_AttachIDA );
			ctx.Write( m_AttachIDB );
			ctx.Write( m_AttachIDC );
			ctx.Write( m_AttachIDD );

			vector tmItem[4];
			vector tmParent[4];
			GetTransform( tmItem );
			m_WorldAttachment.GetTransform( tmParent );
			Math3D.MatrixInvMultiply4( tmParent, tmItem, m_AttachmentTransform );

			ctx.Write( m_AttachmentTransform[0] );
			ctx.Write( m_AttachmentTransform[1] );
			ctx.Write( m_AttachmentTransform[2] );
			ctx.Write( m_AttachmentTransform[3] );
		} else
		{
			ctx.Write( false );
		}

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ItemBase::OnStoreSave - End");
		#endif
	}
	
	override bool OnStoreLoad( ParamsReadContext ctx, int version )
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ItemBase::OnStoreLoad - Start");
		#endif

		//! Use GetExpansionSaveVersion(), making sure this is read before everything else
		if ( !ctx.Read( m_ExpansionSaveVersion ) )
			return false;

		if ( !super.OnStoreLoad( ctx, version ) )
			return false;

		if ( !ctx.Read( m_CurrentSkinName ) )
			return false;

		if ( !ctx.Read( m_IsAttached ) )
			return false;

		if ( m_IsAttached )
		{
			if ( !ctx.Read( m_AttachIDA ) )
				return false;
				
			if ( !ctx.Read( m_AttachIDB ) )
				return false;
				
			if ( !ctx.Read( m_AttachIDC ) )
				return false;

			if ( !ctx.Read( m_AttachIDD ) )
				return false;
				
			vector transSide;
			vector transUp;
			vector transForward;
			vector transPos;
			if ( !ctx.Read( transSide ) || !ctx.Read( transUp ) || !ctx.Read( transForward ) || !ctx.Read( transPos ) )
				return false;

			m_AttachmentTransform[0] = transSide;
			m_AttachmentTransform[1] = transUp;
			m_AttachmentTransform[2] = transForward;
			m_AttachmentTransform[3] = transPos;
		}

		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ItemBase::OnStoreLoad - End");
		#endif

		return true;
	}

	override void EEOnAfterLoad()
	{
		#ifdef EXPANSIONEXPRINT
		Print("ItemBase::EEOnAfterLoad - Start");
		#endif

		super.EEOnAfterLoad();

		if ( m_CanBeSkinned )
		{
			m_CurrentSkinIndex = m_SkinModule.GetSkinIndex( GetType(), m_CurrentSkinName );
			m_CurrentSkinSynchRemote = m_CurrentSkinIndex;
			m_CurrentSkin = m_Skins[ m_CurrentSkinIndex ];

			ExpansionOnSkinUpdate();
		}

		if ( m_IsAttached )
		{
			m_WorldAttachment = GetGame().GetEntityByPersitentID( m_AttachIDA, m_AttachIDB, m_AttachIDC, m_AttachIDD );
		
			//! Same functionality as vanilla, delete if the storage owner is gone to prevent basebuilding appear randomly in the air
			if ( !m_WorldAttachment )
			{
				GetGame().ObjectDelete( this );
			} else
			{
				LinkToLocalSpaceOf( m_WorldAttachment, m_AttachmentTransform );
			}
		}
		
		#ifdef EXPANSIONEXPRINT
		Print("ItemBase::EEOnAfterLoad - End");
		#endif
	}

	override void OnCreatePhysics()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "ItemBase::OnCreatePhysics - Start" );
		#endif

		super.OnCreatePhysics();

		if ( !GetGame().IsServer() )
		{
			#ifdef EXPANSIONEXPRINT
			EXPrint( "ItemBase::OnCreatePhysics - End - Not Server" );
			#endif

			return;
		}

		//! GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).Call( CheckForAttachmentRaycast );

		#ifdef EXPANSIONEXPRINT
		EXPrint( "ItemBase::OnCreatePhysics - End" );
		#endif
	}

	void CheckForAttachmentRaycast()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "ItemBase::CheckForAttachmentRaycast - Start" );
		#endif

		if ( m_IsAttached && m_WorldAttachment )
		{
			#ifdef EXPANSIONEXPRINT
			EXPrint( "ItemBase::CheckForAttachmentRaycast - End - Already Attached" );
			#endif

			return;
		}
			
		vector boundingBox[2];
		float radius = ClippingInfo( boundingBox );

		vector start = GetPosition() + Vector( 0, boundingBox[1][1], 0 );
		vector end = GetPosition() - Vector( 0, boundingBox[0][1], 0 );

		RaycastRVParams params = new RaycastRVParams( start, end, this, radius );
		params.sorted = true;
		params.type = ObjIntersectGeom;
		params.flags = CollisionFlags.ALLOBJECTS;

		//! Initiate the raycast
		array< ref RaycastRVResult > results = new array< ref RaycastRVResult >();
		if ( DayZPhysics.RaycastRVProxy( params, results ) )
		{
			Object target = ExpansionAttachmentHelper.FindBestAttach( this, results );

			if ( target )
			{
				vector tmItem[4];
				vector tmTarget[4];
				vector tmLocal[4];

				GetTransform( tmItem );
				target.GetTransform( tmTarget );
				Math3D.MatrixInvMultiply4( tmTarget, tmItem, tmLocal );

				LinkToLocalSpaceOf( target, tmLocal );
			}
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint( "ItemBase::CheckForAttachmentRaycast - End" );
		#endif
	}
	
	override void EEDelete( EntityAI parent )
	{
		super.EEDelete( parent );
	}

	protected void SkinMessage( string message )
	{
		if ( IsMissionClient() )
		{
			Message( GetPlayer(), message );
		} else
		{
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint( message );
			#endif
		}	
	}

	override int GetHiddenSelectionIndex( string selection )
	{
		array<string> config_selections	= new array<string>;
		ConfigGetTextArray( "hiddenSelections", config_selections );
		
		for ( int i = 0; i < config_selections.Count(); ++i )
		{
			if ( config_selections.Get( i ) == selection )
			{
				return i;
			}
		}
		
		return -1;
	}

	override void OnVariablesSynchronized()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ItemBase::OnVariablesSynchronized - Start");
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
		EXPrint("ItemBase::OnVariablesSynchronized - End");
		#endif
	}

	protected void ExpansionSetupSkins()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ItemBase::ExpansionSetupSkins Start");
		#endif

		if ( Class.CastTo( m_SkinModule, GetModuleManager().GetModule( ExpansionSkinModule ) ) )
		{
			m_Skins = m_SkinModule.RetrieveSkins( GetType() );

			if ( m_Skins && m_Skins.Count() != 0 )
			{
				m_CanBeSkinned = true;

				#ifdef EXPANSIONEXPRINT
				EXPrint("ItemBase::ExpansionSetupSkins End");
				#endif

				return;
			}
		}

		m_Skins = new array< ref ExpansionSkin >;
		m_CanBeSkinned = false;

		#ifdef EXPANSIONEXPRINT
		EXPrint("ItemBase::ExpansionSetupSkins End");
		#endif
	}

	override void EEHealthLevelChanged( int oldLevel, int newLevel, string zone )
	{
		super.EEHealthLevelChanged( oldLevel, newLevel, zone );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ItemBase::EEHealthLevelChanged Start");
		#endif

		if ( m_CanBeSkinned )
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

	void ExpansionSetSkin( int skinIndex )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ItemBase::ExpansionSetSkin Start");
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

		m_CurrentSkinName = m_SkinModule.GetSkinName( GetType(), m_CurrentSkinIndex );
		m_CurrentSkinSynchRemote = m_CurrentSkinIndex;
		m_CurrentSkin = m_Skins[ m_CurrentSkinIndex ];

		ExpansionOnSkinUpdate();

		SetSynchDirty();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ItemBase::ExpansionSetSkin End");
		#endif
	}

	void ExpansionOnSkinDamageZoneUpdate( ExpansionSkinDamageZone zone, int level )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ItemBase::ExpansionOnSkinDamageZoneUpdate Start");
		#endif

		for ( int i = 0; i < zone.HiddenSelections.Count(); i++ )
		{
			int selectionIndex = GetHiddenSelectionIndex( zone.HiddenSelections[i] );
					
			if ( level >= 0 && level < zone.HealthLevels.Count() )
			{
				SetObjectTexture( selectionIndex, zone.HealthLevels[level].RVTexture );
				SetObjectMaterial( selectionIndex, zone.HealthLevels[level].RVMaterial );
			}
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ItemBase::ExpansionOnSkinDamageZoneUpdate End");
		#endif
	}

	void ExpansionOnSkinUpdate()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ItemBase::ExpansionOnSkinUpdate Start");
		#endif
		
		for ( int i = 0; i < m_CurrentSkin.HiddenSelections.Count(); i++ )
		{
			ExpansionSkinHiddenSelection selection = m_CurrentSkin.HiddenSelections[ i ];

			int selectionIndex = GetHiddenSelectionIndex( selection.HiddenSelection );

			//! SkinMessage( "HiddenSelection: " + selection.HiddenSelection );
			//! SkinMessage( "SelectionIndex: " + selectionIndex );
			//! SkinMessage( "RVTexture: " + selection.RVTexture );
			//! SkinMessage( "RVMaterial: " + selection.RVMaterial );

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
	
	bool IsQuestItem()
	{
		return false;
	}

	override void SetActions()
	{
		// AddAction(ExpansionActionDamageBaseBuilding);

		super.SetActions();
	}
}