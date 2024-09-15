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
	protected EntityAI m_Expansion_WorldAttachment;
	protected vector m_Expansion_AttachmentTransform[4];
	protected bool m_Expansion_IsAttached;
	protected int m_Expansion_AttachIDA;
	protected int m_Expansion_AttachIDB;
	protected int m_Expansion_AttachIDC;
	protected int m_Expansion_AttachIDD;
	protected dBlock m_Expansion_Block;
	
	void ItemBase()
	{
		RegisterNetSyncVariableBool( "m_Expansion_IsAttached" );

		GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( LongDeferredInit, 1000 );

		//SetEventMask(EntityEvent.CONTACT);
	}

	void LongDeferredInit()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.VEHICLES, this, "LongDeferredInit");
#endif

	}

	#ifdef EXPANSION_MODSTORAGE
	override void CF_OnStoreSave(CF_ModStorageMap storage)
	{
		super.CF_OnStoreSave(storage);

		auto ctx = storage[DZ_Expansion_Vehicles];
		if (!ctx) return;

		if ( m_Expansion_WorldAttachment && m_Expansion_IsAttached )
		{
			ctx.Write( m_Expansion_IsAttached );

			m_Expansion_WorldAttachment.GetPersistentID( m_Expansion_AttachIDA, m_Expansion_AttachIDB, m_Expansion_AttachIDC, m_Expansion_AttachIDD );

			ctx.Write( m_Expansion_AttachIDA );
			ctx.Write( m_Expansion_AttachIDB );
			ctx.Write( m_Expansion_AttachIDC );
			ctx.Write( m_Expansion_AttachIDD );

			vector tmItem[4];
			vector tmParent[4];
			GetTransform( tmItem );
			m_Expansion_WorldAttachment.GetTransform( tmParent );
			Math3D.MatrixInvMultiply4( tmParent, tmItem, m_Expansion_AttachmentTransform );

			ctx.Write( m_Expansion_AttachmentTransform[0] );
			ctx.Write( m_Expansion_AttachmentTransform[1] );
			ctx.Write( m_Expansion_AttachmentTransform[2] );
			ctx.Write( m_Expansion_AttachmentTransform[3] );
		} else
		{
			ctx.Write( false );
		}
	}
	
	override bool CF_OnStoreLoad(CF_ModStorageMap storage)
	{
		if (!super.CF_OnStoreLoad(storage))
			return false;

		auto ctx = storage[DZ_Expansion_Vehicles];
		if (!ctx) return true;

		if (!ctx.Read(m_Expansion_IsAttached))
			return false;

		m_ExpansionSaveVersion = ctx.GetVersion();

		if ( m_Expansion_IsAttached )
		{
			if (!ctx.Read(m_Expansion_AttachIDA))
				return false;
			if (!ctx.Read(m_Expansion_AttachIDB))
				return false;
			if (!ctx.Read(m_Expansion_AttachIDC))
				return false;
			if (!ctx.Read(m_Expansion_AttachIDD))
				return false;
				
			vector transSide;
			vector transUp;
			vector transForward;
			vector transPos;

			if (!ctx.Read(transSide))
				return false;
			if (!ctx.Read(transUp))
				return false;
			if (!ctx.Read(transForward))
				return false;
			if (!ctx.Read(transPos))
				return false;

			m_Expansion_AttachmentTransform[0] = transSide;
			m_Expansion_AttachmentTransform[1] = transUp;
			m_Expansion_AttachmentTransform[2] = transForward;
			m_Expansion_AttachmentTransform[3] = transPos;
		}

		return true;
	}
	#endif
	
	override void EEOnAfterLoad()
	{
		super.EEOnAfterLoad();
		
		if ( m_Expansion_IsAttached )
		{
			m_Expansion_WorldAttachment = GetGame().GetEntityByPersitentID( m_Expansion_AttachIDA, m_Expansion_AttachIDB, m_Expansion_AttachIDC, m_Expansion_AttachIDD );
		
			//! Same functionality as vanilla, delete if the storage owner is gone to prevent basebuilding appear randomly in the air
			if ( !m_Expansion_WorldAttachment )
			{
				GetGame().ObjectDelete( this );
			} else
			{
				LinkToLocalSpaceOf( m_Expansion_WorldAttachment, m_Expansion_AttachmentTransform );
			}
		}
	}
	
	#ifndef EXPANSION_ITEM_ATTACHING_DISABLE
	void CheckForAttachmentRaycast()
	{
		if ( m_Expansion_IsAttached && m_Expansion_WorldAttachment )
		{
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
	}
	#else
	void CheckForAttachmentRaycast()
	{
	}
	#endif

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
	// OnItemLocationChanged
	//============================================	
	#ifndef EXPANSION_ITEM_ATTACHING_DISABLE
	override void OnItemLocationChanged( EntityAI old_owner, EntityAI new_owner )
	{
		#ifdef EXPANSION_ITEM_ATTACHING_LOGGING
		Print("ItemBase::OnItemLocationChanged - Start");
		#endif

		super.OnItemLocationChanged( old_owner, new_owner );

		if ( !GetGame().IsServer() )
		{
			#ifdef EXPANSION_ITEM_ATTACHING_LOGGING
			Print( "ItemBase::OnItemLocationChanged - End - Not Server" );
			#endif

			return;
		}

		DayZPlayerImplement old_owner_dpi = NULL;
		DayZPlayerImplement new_owner_dpi = NULL;
		
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

			bool carAttach;
			if (car && car.Expansion_CanObjectAttach( this ))
				carAttach = true;
			bool vehAttach;
			if (veh && veh.Expansion_CanObjectAttach( this ))
				vehAttach = true;

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
		return ( m_Expansion_IsAttached || super.IsInventoryVisible() );
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

		m_Expansion_IsAttached = true;
		m_Expansion_WorldAttachment = pParent;

		vector tmParent[4];
		m_Expansion_WorldAttachment.GetTransform( tmParent );
		Math3D.MatrixMultiply4( tmParent, pLocalSpaceMatrix, pLocalSpaceMatrix );

		vector pos = pLocalSpaceMatrix[3];
		SetTransform( pLocalSpaceMatrix );

		m_Expansion_WorldAttachment.AddChild( this, -1 );
		m_Expansion_WorldAttachment.Update();

		CreateDynamicPhysics(PhxInteractionLayers.DYNAMICITEM);
		m_Expansion_Block = dBodyCollisionBlock(m_Expansion_WorldAttachment, this);

		#ifdef EXPANSION_ITEM_ATTACHING_LOGGING
		Print( "ItemBase::LinkToLocalSpaceOf - End - Target=" + m_Expansion_WorldAttachment );
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

		if ( !m_Expansion_WorldAttachment )
		{
			#ifdef EXPANSION_ITEM_ATTACHING_LOGGING
			Print( "ItemBase::UnlinkFromLocalSpace - End - No World Attachment" );
			#endif

			return;
		}

		//dBodyRemoveBlock(this, m_Expansion_Block);

		m_Expansion_IsAttached = false;

		vector tmGlobal[4];

		GetTransform( tmGlobal );

		m_Expansion_WorldAttachment.RemoveChild( this );

		SetTransform( tmGlobal );

		m_Expansion_WorldAttachment.Update();
		m_Expansion_WorldAttachment = NULL;

		Update();

		#ifdef EXPANSION_ITEM_ATTACHING_LOGGING
		Print( "ItemBase::UnlinkFromLocalSpace - End" );
		#endif
	}

	bool Expansion_ParentDrops(EntityAI parent)
	{
		if (Transport.Cast(parent))
			return true;

		if (ExpansionVehicleBase.Cast(parent))
			return true;

		return false;
	}

	void Expansion_PhysicsDrop()
	{
		if (!GetGame().IsServer())
			return;

		EntityAI parent = GetHierarchyParent();
		if (!Expansion_ParentDrops(parent))
			return;

		vector transform[4];
		GetTransform(transform);

		vector velocity = dBodyGetVelocityAt(parent, transform[3]);
		float mass = dBodyGetMass(this);
		if (!mass)
			mass = GetWeight() / 1000;
		vector force = velocity * mass;

		ExpansionVehicle vehicle;

		//! If parent is exploded vehicle, make parts fly off violently
		if (ExpansionVehicle.Get(vehicle, parent) && vehicle.IsExploded())
		{
			if (!force.Length())
				force = GetDirection(); //! If vehicle is standing still, use direction vector instead of velocity
			force = force.Normalized() * 500;
		}

		InventoryLocation src = new InventoryLocation;
		GetInventory().GetCurrentInventoryLocation(src);
		if (parent.GetInventory().GetSlotLock(src.GetSlot()))
			parent.GetInventory().SetSlotLock(src.GetSlot(), false);
		InventoryLocation dst = new InventoryLocation;
		dst.SetGround(this, transform);

		InventoryMode invMode = InventoryMode.SERVER;
		if (!GetGame().IsMultiplayer())
			invMode = InventoryMode.LOCAL;

		GetInventory().TakeToDst(invMode, src, dst);
		ThrowPhysically(null, force);
	}
};
