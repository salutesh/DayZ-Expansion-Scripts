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
	protected int m_ExpansionSaveVersion;

	protected ExpansionSkinModule m_SkinModule;
	protected ExpansionSkin m_CurrentSkin;

	protected string m_CurrentSkinName;
	protected int m_CurrentSkinIndex;
	protected int m_CurrentSkinSynchRemote;

	protected bool m_CanBeSkinned;
	protected autoptr array< ExpansionSkin > m_Skins;
	
	protected EntityAI m_WorldAttachment;
	protected vector m_AttachmentTransform[4];
	protected bool m_IsAttached;
	protected int m_AttachIDA;
	protected int m_AttachIDB;
	protected int m_AttachIDC;
	protected int m_AttachIDD;

	protected ref ExpansionElectricityConnection m_ElectricitySource;
	protected ref array< ItemBase > m_ElectricityConnections;
	
	//============================================
	// ItemBase Constructor
	//============================================
	void ItemBase()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ItemBase::ItemBase - Start");
		EXPrint("ItemBase::ItemBase - Classname: " + ClassName());
		EXPrint("ItemBase::ItemBase - Type: " + GetType());
		#endif
		
		m_ElectricitySource = new ExpansionElectricityConnection( this );
		m_ElectricityConnections = new array< ItemBase >();
		if ( ExpansionCanRecievePower() )
		{
			//RegisterNetSyncVariableBool( "m_IsPairedSynch" );
			//RegisterNetSyncVariableBool( "m_IsOnSynch" );
			//RegisterNetSyncVariableInt( "m_SourceNetLow" );
			//RegisterNetSyncVariableInt( "m_SourceNetHigh" );
		}

		ExpansionSetupSkins();
		
		RegisterNetSyncVariableInt( "m_CurrentSkinSynchRemote", 0, m_Skins.Count() );
		RegisterNetSyncVariableBool( "m_IsAttached" );

		GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).Call( DeferredInit );
		GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( LongDeferredInit, 1000 );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ItemBase::ItemBase - End");
		#endif
	}
	
	//============================================
	// ItemBase Destructor
	//============================================
	void ~ItemBase()
	{
	}

	/**
	 * /note	This is a static boolean, do not change during runtime
	 * 
	 * /brief	Override this to determine if the item can be powered using the expansion power system
	 */
	bool ExpansionCanRecievePower()
	{
		return false;
	}

	/**
	 * /note	This is a static boolean, do not change during runtime
	 * 
	 * /brief	Override this to determine if the item powers other items
	 */
	bool ExpansionIsPowerSource()
	{
		return false;
	}

	bool ExpansionIsConnected( ItemBase source = NULL )
	{
		return m_ElectricitySource.IsConnected( source );
	}

	void ExpansionDisconnect()
	{
		m_ElectricitySource.Disconnect();

		SetSynchDirty();
	}

	void ExpansionAddConnection( ItemBase item )
	{
		Print( "[" + this + "] ExpansionAddConnection item=" + item );
		if ( !ExpansionIsPowerSource() || !item.ExpansionCanRecievePower() )
			return;

		item.m_ElectricitySource.Pair( this );

		SetSynchDirty();
		
		Print( "[" + this + "] ~ExpansionAddConnection" );
	}

	void _ExpansionAddConnection( ItemBase item )
	{
		int idx = m_ElectricityConnections.Find( item );
		if ( idx == -1 )
			m_ElectricityConnections.Insert( item );
	}

	void _ExpansionRemoveConnection( ItemBase item )
	{
		int idx = m_ElectricityConnections.Find( item );
		if ( idx != -1 )
			m_ElectricityConnections.Remove( idx );
	}

	override void OnWorkStart()
	{
		super.OnWorkStart();

		for ( int i = 0; i < m_ElectricityConnections.Count(); ++i )
		{
			m_ElectricityConnections[i].OnWorkStart();
		}
	}

	override void OnWorkStop()
	{
		super.OnWorkStop();
		
		for ( int i = 0; i < m_ElectricityConnections.Count(); ++i )
		{
			m_ElectricityConnections[i].OnWorkStop();
		}
	}
		
	//============================================
	// LongDeferredInit
	//============================================	
	void LongDeferredInit()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ItemBase::LongDeferredInit - Start");
		#endif

		if ( m_SkinModule )
			m_SkinModule.PerformCESkinSwap( this );

		#ifdef EXPANSIONEXPRINT
		EXPrint("ItemBase::LongDeferredInit - End");
		#endif
	}
	
	//============================================
	// DeferredInit
	//============================================	
	void DeferredInit()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ItemBase::DeferredInit - Start");
		#endif

		#ifdef EXPANSIONEXPRINT
		EXPrint("ItemBase::DeferredInit - End");
		#endif
	}
	
	//============================================
	// IsBasebuilding
	//============================================	
	/**
	\brief Returning if can be built
		\param 	
	*/
	bool IsBasebuilding()
	{
		return false;
	}
	
	//============================================
	// ExpansionIsOpenable
	//============================================	
	/**
	\brief Returning if can be opened
		\param 	
	*/
	bool ExpansionIsOpenable()
	{
		return false;
	}
	
	/**
	\brief Returning if wall/safe is open
		\param 	
	*/
	bool IsOpened()
	{
		return false;
	}

	/**
	\brief Returning if player can open gate/safe from selection
		\param 
	*/
	bool ExpansionCanOpen( PlayerBase player, string selection )
	{
		return false;
	}
	
	/**
	\brief Returning if player can close gate/safe from selection
		\param 	
	*/
	bool CanClose( string selection )
	{
		return false;
	}

	/**
	\brief Opening gate/safe on defined selection
		\param 	
	*/
	void Open( string selection ) 
	{
	}
	
	/**
	\brief Closing gate/safe on defined selection
		\param 	
	*/
	void Close( string selection ) 
	{
	}
	
	/**
	\brief Set code of wall/safe
		\param 	
	*/
	void SetCode( string code )
	{
	}
	
	/**
	\brief Returning code of wall/safe
		\param 	
	*/
	string GetCode()
	{
		return "";
	}

	/**
	\brief Returning if the wall/safe has a code
		\param 	
	*/
	bool HasCode()
	{
		return false;
	}

	/**
	\brief Returning if the wall/safe has a code
		\param "selection" selection for codelock
	*/
	bool ExpansionHasCodeLock( string selection )
	{
		return false;
	}

	/**
	\brief Returning codelock entity
		\param
	*/
	ExpansionCodeLock ExpansionGetCodeLock()
	{
		return null;
	}

	/**
	\brief Returning if wall/safe is locked
		\param 	
	*/
	bool IsLocked()
	{
		return false;
	}

	/**
	\brief Locking base build/safe
		\param 	
	*/
	void Lock()
	{
	}
	
	/**
	\brief Unlocking base build/safe
		\param 	
	*/
	void Unlock()
	{
	}
	
	/**
	\brief Failed attempt to unlock base build/safe
		\param 	
	*/
	void FailedUnlock()
	{
	}
	
	//============================================
	// SendServerLockReply
	//============================================	
	private void SendServerLockReply(bool reply, bool injuring, bool unlock, string code, PlayerIdentity sender)
	{
		ScriptRPC rpc = new ScriptRPC;
		rpc.Write( reply );
		rpc.Write( injuring );
		rpc.Write( unlock );
		rpc.Write( code );
		rpc.Send( this, ExpansionLockRPC.SERVERREPLY, true, sender );
	}
	
	//============================================
	// OnRPC
	//============================================	
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
					SendServerLockReply( false, false, false, "", sender );
					return;
				}
				
				if ( !HasCode() || IsLocked() )
				{
					SendServerLockReply( false, false, false, "", sender );
					return;
				}

				Lock();
				
				SendServerLockReply( true, false, true, "", sender );
				
				return;
			}
			
			case ExpansionLockRPC.UNLOCK:
			{
				if ( !IsMissionHost() || !GetExpansionSettings().GetBaseBuilding() )
					return;
				
				if ( !ctx.Read( code ) || code.Length() != GetExpansionSettings().GetBaseBuilding().CodeLockLength )
				{
					Error("ItemBase::OnRPC ExpansionLockRPC.UNLOCK can't read code");
					SendServerLockReply( false, false, false, "", sender );
					//TODO: notification here
					return;
				}

				if ( !ctx.Read( selection ) )
				{
					Error("ItemBase::OnRPC ExpansionLockRPC.UNLOCK can't read selection");
					SendServerLockReply( false, false, false, "", sender );
					//TODO: notification here
					return;
				}
				
				if ( !HasCode() || !IsLocked() )
				{
					#ifdef EXPANSIONEXLOGPRINT
					EXLogPrint("ItemBase::OnRPC ExpansionLockRPC.UNLOCK !HasCode() || !IsLocked()");
					#endif
					
					SendServerLockReply( false, false, false, "", sender );
					return;
				}

				if (HasCode() && ExpansionGetCodeLock() && ExpansionGetCodeLock().IsAuthorizedPlayer(sender.GetId())) 
				{
					Unlock();
					SendServerLockReply( true, false, true, GetCode(), sender );
					return;
				}
				
				if ( GetCode() != code )
				{
					#ifdef EXPANSIONEXLOGPRINT
					EXLogPrint("ItemBase::OnRPC ExpansionLockRPC.UNLOCK GetCode() != code");
					#endif
					
					
					bool InjuryPlayer = GetExpansionSettings().GetBaseBuilding().DoDamageWhenEnterWrongCodeLock;

					SendServerLockReply( false, InjuryPlayer, true, "", sender );
					
					if (InjuryPlayer)
					{
						PlayerBase player = PlayerBase.GetPlayerByUID( sender.GetId() );
						if ( player )
						{
							FailedUnlock();
							player.DecreaseHealth( "", "", GetExpansionSettings().GetBaseBuilding().DamageWhenEnterWrongCodeLock );
						}
					}

					return;
				}
				
				if (ExpansionGetCodeLock()) {
					ExpansionGetCodeLock().AddAuthorizedPlayer(sender.GetId());
				}
			
				Unlock();
				SendServerLockReply( true, false, true, GetCode(), sender );
				return;
			}
			
			case ExpansionLockRPC.SET:
			{
				if ( !IsMissionHost() )
					return;
				
				if ( !ctx.Read( code ) || code.Length() != GetExpansionSettings().GetBaseBuilding().CodeLockLength )
				{
					Error("ItemBase::OnRPC ExpansionLockRPC.SET can't read code");
					SendServerLockReply( false, false, false, "", sender );
					//TODO: notification here
					return;
				}

				if ( !ctx.Read( selection ) )
				{
					Error("ItemBase::OnRPC ExpansionLockRPC.SET can't read selection");
					SendServerLockReply( false, false, false, "", sender );
					//TODO: notification here
					return;
				}
				
				if ( HasCode() )
				{
					// Base already has code so don't try setting it to another.
					SendServerLockReply( false, false, false, "", sender );
					return;
				}

				SetCode( code );

				SendServerLockReply( true, false, false, "", sender );
				return;
			}

			case ExpansionLockRPC.CHANGE:
			{
				if ( !IsMissionHost() )
					return;
				
				if ( !ctx.Read( code ) || code.Length() != GetExpansionSettings().GetBaseBuilding().CodeLockLength )
				{
					Error("ItemBase::OnRPC ExpansionLockRPC.SET can't read code");
					SendServerLockReply( false, false, false, "", sender );
					//TODO: notification here
					return;
				}

				if ( !ctx.Read( selection ) )
				{
					Error("ItemBase::OnRPC ExpansionLockRPC.SET can't read selection");
					SendServerLockReply( false, false, false, "", sender );
					//TODO: notification here
					return;
				}
				
				SetCode( code );

				SendServerLockReply( true, false, false, "", sender );
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
				
				bool unlock;
				if ( !ctx.Read( unlock ) )
				{
					Error("ItemBase::OnRPC ExpansionLockRPC.SERVERREPLY can't read unlock");
					return;
				}
				
				if ( !ctx.Read( code ) )
				{
					Error("ItemBase::OnRPC ExpansionLockRPC.SERVERREPLY can't read code");
					return;
				}
				
				//Save the code
				if (reply && unlock && code != "")
				{
					ExpansionLockSaver.GetInstance().SaveCode( this, code );
				}
				//Code wrong, could be a saved code, so we remove it
				else if (!reply && unlock && code == "")
				{
					ExpansionLockSaver.GetInstance().RemoveCode( this );
				}

				ExpansionLockUIBase menu;
				if ( !Class.CastTo( menu, GetGame().GetUIManager().FindMenu( MENU_EXPANSION_CODELOCK_MENU ) ) && !Class.CastTo( menu, GetGame().GetUIManager().FindMenu( MENU_EXPANSION_NUMPAD_MENU ) ) )
					return;
					
				menu.OnServerResponse( reply, injuring );
			}
		}
	}
	
	//============================================
	// EEItemLocationChanged
	//============================================	
	#ifndef EXPANSION_ITEM_ATTACHING_DISABLE
	override void EEItemLocationChanged( notnull InventoryLocation oldLoc, notnull InventoryLocation newLoc )
	{
		#ifdef EXPANSIONEXPRINT
		Print("ItemBase::EEItemLocationChanged - Start");
		#endif

		ExpansionAIBase new_player = null;
		ExpansionAIBase old_player = null;
		
		if ( oldLoc.GetItem() )
			old_player = ExpansionAIBase.Cast( oldLoc.GetItem().GetHierarchyRootPlayer() );

		if ( newLoc.GetParent() )
			new_player = ExpansionAIBase.Cast( newLoc.GetParent().GetHierarchyRootPlayer() );
		
		if ( !new_player && !old_player )
		{	
			super.EEItemLocationChanged( oldLoc, newLoc );

			#ifdef EXPANSIONEXPRINT
			Print("ItemBase::EEItemLocationChanged - End - Not AI");
			#endif

			return;
		}

		EntityAI old_owner = oldLoc.GetItem();
		EntityAI new_owner = newLoc.GetItem();
		OnItemLocationChanged( old_owner, new_owner );

		if ( oldLoc.GetType() == InventoryLocationType.ATTACHMENT && newLoc.GetType() == InventoryLocationType.ATTACHMENT )
		{
			OnItemAttachmentSlotChanged( oldLoc, newLoc );
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
	
	//============================================
	// OnItemLocationChanged
	//============================================	
	override void OnItemLocationChanged( EntityAI old_owner, EntityAI new_owner )
	{
		#ifdef EXPANSION_ITEM_ATTACHING_LOGGING
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

		#ifdef EXPANSION_ITEM_ATTACHING_LOGGING
		Print( shouldSuper );
		Print( old_owner );
		Print( new_owner );
		Print( old_owner_dpi );
		Print( new_owner_dpi );
		
		if ( new_owner )
			Print( "new_owner IsMan: " + new_owner.IsMan() );

		if ( old_owner )
			Print( "old_owner IsMan: " + old_owner.IsMan() );
		#endif
		
		if ( shouldSuper )
		{
			#ifdef EXPANSION_ITEM_ATTACHING_LOGGING
			Print( "ItemBase::OnItemLocationChanged - Start - Calling Super" );
			#endif

			super.OnItemLocationChanged( old_owner, new_owner );

			#ifdef EXPANSION_ITEM_ATTACHING_LOGGING
			Print( "ItemBase::OnItemLocationChanged - End - Calling Super" );
			#endif
		}

		if ( !GetGame().IsServer() )
		{
			#ifdef EXPANSION_ITEM_ATTACHING_LOGGING
			Print( "ItemBase::OnItemLocationChanged - End - Not Server" );
			#endif

			return;
		}

		EntityAI parent = NULL;

		//Attaching or detaching the items to the vehicle
		if ( old_owner_dpi && !new_owner ) // on drop
		{
			if ( dBodyIsDynamic( this ) && dBodyIsActive( this ) )
			{
				#ifdef EXPANSION_ITEM_ATTACHING_LOGGING
				Print( "ItemBase::OnItemLocationChanged - End - Is Dynamic" );
				#endif

				return;
			}

			if ( !Class.CastTo( parent, old_owner_dpi.GetParent() ) )
			{
				#ifdef EXPANSION_ITEM_ATTACHING_LOGGING
				Print( "ItemBase::OnItemLocationChanged - End - No Parent" );
				#endif

				return;
			}

			CarScript car;
			ExpansionVehicleScript veh;
			if ( !Class.CastTo( car, parent ) && !Class.CastTo( veh, parent ) )
			{
				#ifdef EXPANSION_ITEM_ATTACHING_LOGGING
				Print( "ItemBase::OnItemLocationChanged - End - No Valid Parent" );
				#endif

				return;
			}

			#ifdef EXPANSION_ITEM_ATTACHING_LOGGING
			Print( car );
			Print( veh );
			#endif

			bool carAttach = car && car.CanObjectAttach( this );
			bool vehAttach = veh && veh.CanObjectAttach( this );

			#ifdef EXPANSION_ITEM_ATTACHING_LOGGING
			Print( carAttach );
			Print( vehAttach );
			#endif

			if ( carAttach || vehAttach )
			{
				vector tmItem[4];
				vector tmTarget[4];
				vector tmLocal[4];

				vector pPos = old_owner_dpi.GetPosition();
				vector pOri = old_owner_dpi.GetOrientation();

				old_owner_dpi.GetTransform( tmItem );
				PlaceOnSurfaceRotated( tmItem, pPos, 0, 0, 0, false );

				parent.GetTransform( tmTarget );
				Math3D.MatrixInvMultiply4( tmTarget, tmItem, tmLocal );

				LinkToLocalSpaceOf( parent, tmLocal );
			}
		} else if ( new_owner_dpi && !old_owner ) //! On pickup
		{
			UnlinkFromLocalSpace();
		}
		
		#ifdef EXPANSION_ITEM_ATTACHING_LOGGING
		Print("ItemBase::OnItemLocationChanged - End");
		#endif
	}
	#endif
	
	//============================================
	// IsInventoryVisible
	//============================================	
	override bool IsInventoryVisible()
	{
		return ( m_IsAttached || super.IsInventoryVisible() );
	}
	
	//============================================
	// LinkToLocalSpaceOf
	//============================================
	void LinkToLocalSpaceOf( notnull EntityAI pParent, vector pLocalSpaceMatrix[4] )
	{
		#ifdef EXPANSION_ITEM_ATTACHING_LOGGING
		Print( "ItemBase::LinkToLocalSpaceOf - Start - Target=" + pParent );
		#endif

		if ( !GetGame().IsServer() )
		{
			#ifdef EXPANSION_ITEM_ATTACHING_LOGGING
			Print( "ItemBase::LinkToLocalSpaceOf - End - Not Server" );
			#endif

			return;
		}
		/*
		InventoryLocation child_src = new InventoryLocation;
		GetInventory().GetCurrentInventoryLocation( child_src );
				
		InventoryLocation child_dst = new InventoryLocation;
		child_dst.SetGround( this, pLocalSpaceMatrix );
		child_dst.SetParent( pParent );

		if ( !GameInventory.LocationCanMoveEntity( child_src, child_dst ) )
		{
			Print( "ItemBase::LinkToLocalSpaceOf - End - LocationCanMoveEntity" );

			return;
		}

		if ( !GameInventory.LocationSyncMoveEntity( child_src, child_dst ) )
		{
			Print( "ItemBase::LinkToLocalSpaceOf - End - LocationSyncMoveEntity" );

			return;
		}
		*/

		#ifdef EXPANSION_ITEM_ATTACHING_LOGGING
		Print( pLocalSpaceMatrix[0] );
		Print( pLocalSpaceMatrix[1] );
		Print( pLocalSpaceMatrix[2] );
		Print( pLocalSpaceMatrix[3] );
		#endif

		m_IsAttached = true;
		m_WorldAttachment = pParent;

		SetTransform( pLocalSpaceMatrix );

		m_WorldAttachment.AddChild( this, -1 );
		m_WorldAttachment.Update();

		#ifdef EXPANSION_ITEM_ATTACHING_LOGGING
		Print( "ItemBase::LinkToLocalSpaceOf - End - Target=" + m_WorldAttachment );
		#endif
	}
	
	//============================================
	// UnlinkFromLocalSpace
	//============================================
	void UnlinkFromLocalSpace()
	{
		#ifdef EXPANSION_ITEM_ATTACHING_LOGGING
		Print( "ItemBase::UnlinkFromLocalSpace - Start" );
		#endif

		if ( !GetGame().IsServer() )
		{
			#ifdef EXPANSION_ITEM_ATTACHING_LOGGING
			Print( "ItemBase::UnlinkFromLocalSpace - End - Not Server" );
			#endif

			return;
		}

		if ( !m_WorldAttachment )
		{
			#ifdef EXPANSION_ITEM_ATTACHING_LOGGING
			Print( "ItemBase::UnlinkFromLocalSpace - End - No World Attachment" );
			#endif

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

		#ifdef EXPANSION_ITEM_ATTACHING_LOGGING
		Print( "ItemBase::UnlinkFromLocalSpace - End" );
		#endif
	}
	
	//============================================
	// GetExpansionSaveVersion
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

		m_ElectricitySource.OnStoreSave( ctx );

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
		EXLogPrint("ItemBase::OnStoreLoad - Start");
		#endif

		//! Use GetExpansionSaveVersion(), making sure this is read before everything else
		if ( Expansion_Assert_False( ctx.Read( m_ExpansionSaveVersion ), "[" + this + "] Failed reading m_ExpansionSaveVersion" ) )
			return false;

		if ( Expansion_Assert_False( super.OnStoreLoad( ctx, version ), "[" + this + "] Failed reading OnStoreLoad super" ) )
			return false;

		if ( Expansion_Assert_False( ctx.Read( m_CurrentSkinName ), "[" + this + "] Failed reading m_CurrentSkinName" ) )
			return false;

		if ( Expansion_Assert_False( ctx.Read( m_IsAttached ), "[" + this + "] Failed reading m_IsAttached" ) )
			return false;

		if ( m_IsAttached )
		{
			if ( Expansion_Assert_False( ctx.Read( m_AttachIDA ), "[" + this + "] Failed reading m_AttachIDA" ) )
				return false;
			if ( Expansion_Assert_False( ctx.Read( m_AttachIDB ), "[" + this + "] Failed reading m_AttachIDB" ) )
				return false;
			if ( Expansion_Assert_False( ctx.Read( m_AttachIDC ), "[" + this + "] Failed reading m_AttachIDC" ) )
				return false;
			if ( Expansion_Assert_False( ctx.Read( m_AttachIDD ), "[" + this + "] Failed reading m_AttachIDD" ) )
				return false;
				
			vector transSide;
			vector transUp;
			vector transForward;
			vector transPos;

			if ( Expansion_Assert_False( ctx.Read( transSide ), "[" + this + "] Failed reading transSide" ) )
				return false;
			if ( Expansion_Assert_False( ctx.Read( transUp ), "[" + this + "] Failed reading transUp" ) )
				return false;
			if ( Expansion_Assert_False( ctx.Read( transForward ), "[" + this + "] Failed reading transForward" ) )
				return false;
			if ( Expansion_Assert_False( ctx.Read( transPos ), "[" + this + "] Failed reading transPos" ) )
				return false;

			m_AttachmentTransform[0] = transSide;
			m_AttachmentTransform[1] = transUp;
			m_AttachmentTransform[2] = transForward;
			m_AttachmentTransform[3] = transPos;
		}

		if ( GetExpansionSaveVersion() < 8 )
		{
			m_ElectricitySource.Setup();
		} else
		{
			m_ElectricitySource.OnStoreLoad( ctx, version, GetExpansionSaveVersion() );
		}

		//SetSynchDirty();
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ItemBase::OnStoreLoad - End");
		#endif

		return true;
	}
	
	//============================================
	// EEOnAfterLoad
	//============================================
	override void EEOnAfterLoad()
	{
		#ifdef EXPANSIONEXPRINT
		Print("ItemBase::EEOnAfterLoad - Start");
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

		m_ElectricitySource.OnAfterLoad();
		
		#ifdef EXPANSIONEXPRINT
		Print("ItemBase::EEOnAfterLoad - End");
		#endif
	}
	
	//============================================
	// OnCreatePhysics
	//============================================
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

		GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).Call( CheckForAttachmentRaycast );

		#ifdef EXPANSIONEXPRINT
		EXPrint( "ItemBase::OnCreatePhysics - End" );
		#endif
	}
		
	//============================================
	// CheckForAttachmentRaycast
	//============================================
	#ifndef EXPANSION_ITEM_ATTACHING_DISABLE
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

				LinkToLocalSpaceOf( EntityAI.Cast( target ), tmLocal );
			}
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint( "ItemBase::CheckForAttachmentRaycast - End" );
		#endif
	}
	#else
	void CheckForAttachmentRaycast()
	{
	}
	#endif
	
	//============================================
	// SkinMessage
	//============================================
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

	//============================================
	// GetHiddenSelectionIndex
	//============================================
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

	//============================================
	// OnVariablesSynchronized
	//============================================
	override void OnVariablesSynchronized()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ItemBase::OnVariablesSynchronized - Start");
		#endif

		super.OnVariablesSynchronized();

		m_ElectricitySource.OnVariablesSynchronized();

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
	
	//============================================
	// Explode
	//============================================
 	override void Explode(int damageType, string ammoType = "")
	{
		float explosionDamageMultiplier = GetExpansionSettings().GetRaid().ExplosionDamageMultiplier;
		float blastDropoff = 1;
		float blastDistance;
		float blastRange = 5;
		float blastDropoffRange = 2.5;
		super.Explode(damageType, ammoType);
		//(point - min ) / (max - min ) 
		if (ammoType == "")
			ammoType = this.ConfigGetString("ammoType");

		string dmgPath = "CfgAmmo" + " " + ammoType + " " + "DamageApplied" + " " + "Health" + " " + "Damage";
		int explosionDamage = GetGame().ConfigGetInt(dmgPath);
		
		ref array<Object> nearest_objects = new array<Object>;
		ref array<CargoBase> proxy_cargos = new array<CargoBase>;
		GetGame().GetObjectsAtPosition3D( this.GetPosition(), blastRange, nearest_objects, proxy_cargos );
		for ( int i = 0; i < nearest_objects.Count(); i++ )
		{
			bool dealDamage = !GetExpansionSettings().GetRaid().EnableExplosiveWhitelist;
			Object nearest_object = nearest_objects.Get(i);

			if ( nearest_object.IsInherited( ExpansionBaseBuilding ) )
			{
				blastDistance = vector.Distance(nearest_object.GetPosition(), this.GetPosition());
				if (blastDistance > blastDropoffRange)
					blastDropoff = (1 - (blastDistance - blastDropoffRange) / (blastRange - blastDropoffRange));
				else 
					blastDropoff = 1;
				
				
				for (int x = 0; x < GetExpansionSettings().GetRaid().ExplosiveDamageWhitelist.Count(); ++x)
				{

					if (this.IsKindOf(GetExpansionSettings().GetRaid().ExplosiveDamageWhitelist[x]))
					{
						dealDamage = true;
					}
				}
				if (dealDamage)
					nearest_object.AddHealth( "GlobalHealth", "Health", ( explosionDamage * blastDropoff * explosionDamageMultiplier * -1) ); 
			}
		}
	}
	
	//============================================
	// SetActions
	//============================================	
	override void SetActions()
	{
		/*
		//! Legacy melee raiding
		if ( GetExpansionSettings().GetRaid() )
		{
			if ( GetExpansionSettings().GetRaid().AllowMeleeRaidingOnExpansion )
			{
				AddAction(ExpansionActionDamageBaseBuilding);
			}
		}
		*/

		super.SetActions();
	}

	void UpdateLaser()
	{
		
	}
}