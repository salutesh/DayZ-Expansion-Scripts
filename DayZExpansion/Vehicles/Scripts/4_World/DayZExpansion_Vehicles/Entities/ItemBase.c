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
	protected bool m_IsAttached;
	protected EntityAI m_WorldAttachment;
	
	//============================================
	// ItemBase Constructor
	//============================================
	void ItemBase()
	{
		GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).Call( DeferredInit );
		GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( LongDeferredInit, 1000 );
	}

	//============================================
	// LongDeferredInit
	//============================================	
	void LongDeferredInit()
	{
	}
	
	//============================================
	// DeferredInit
	//============================================	
#ifdef DAYZ_1_12
	void DeferredInit()
#else
	override void DeferredInit()
#endif
	{
		#ifndef DAYZ_1_12
		super.DeferredInit();
		#endif

		//CreateDynamicPhysics(PhxInteractionLayers.DYNAMICITEM);
		//SetDynamicPhysicsLifeTime(-1);
		//dBodyActive(this, ActiveState.INACTIVE);
	}

	#ifdef CF_MODULE_MODSTORAGE
	override void CF_OnStoreSave( CF_ModStorage storage, string modName )
	{
		#ifdef EXPANSION_STORAGE_DEBUG
		EXPrint("[VEHICLES] ItemBase::CF_OnStoreSave " + this + " " + modName);
		#endif

		super.CF_OnStoreSave( storage, modName );

		if ( modName != "DZ_Expansion_Vehicles" )
			return;

		//! Write here
	}
	
	override bool CF_OnStoreLoad( CF_ModStorage storage, string modName )
	{
		#ifdef EXPANSION_STORAGE_DEBUG
		EXPrint("[VEHICLES] ItemBase::CF_OnStoreLoad " + this + " " + modName);
		#endif

		if ( !super.CF_OnStoreLoad( storage, modName ) )
			return false;

		if ( modName != "DZ_Expansion_Vehicles" )
			return true;

		//! Read here

		return true;
	}
	#endif
	
	//============================================
	// EEOnAfterLoad
	//============================================
	override void EEOnAfterLoad()
	{
		super.EEOnAfterLoad();
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
		super.OnVariablesSynchronized();
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
			ExpansionVehicleBase veh;
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
};