/**
 * ExpansionPlayerBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class PlayerBase
{
	Object m_PlayerHeadingDir;

	private bool m_HasCalledKillFeed;
	
	ExpansionKillFeedModule m_KillfeedModule;
	
	bool m_Expansion_SpawnSelect;

	// ------------------------------------------------------------
	// PlayerBase Constructor
	// ------------------------------------------------------------
	void PlayerBase()
	{
		m_HasCalledKillFeed = false;
	}

	// ------------------------------------------------------------
	// PlayerBase Destructor
	// ------------------------------------------------------------
	void ~PlayerBase()
	{
		//! Making sure we remove tha call for CreateGraveCross when ever the player base entity gets destroyed
		if ( GetGame() && GetExpansionSettings().GetGeneral().EnableGravecross )
		{
		#ifdef ENFUSION_AI_PROJECT
			if (!IsAI())
		#endif
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(CreateGraveCross);
		}
	}
	
	/*override void Init()
	{
		super.Init();
		
		//! Vehicles mod will set this if loaded
		if (!m_ExpansionST)
			m_ExpansionST = new ExpansionHumanST(this);
	}*/

	// ------------------------------------------------------------
	// Override EEKilled
	// ------------------------------------------------------------
	override void EEKilled( Object killer )
	{
		if ( GetExpansionSettings().GetGeneral().EnableGravecross )
		{
		#ifdef ENFUSION_AI_PROJECT
			if (!IsAI())
		#endif
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(CreateGraveCross, 5000, false);
		}
		
		super.EEKilled(killer);
	}
	
	// ------------------------------------------------------------
	// Override EEHitBy
	// ------------------------------------------------------------
	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		if ( GetExpansionSettings().GetNotification().EnableKillFeed && GetIdentity() )
		{
			m_KillfeedModule = ExpansionKillFeedModule.Cast( CF_ModuleCoreManager.Get( ExpansionKillFeedModule ) );
			if ( m_KillfeedModule && !IPADACK() )
			{
				UpdateIPADACK( !IsAlive() );
				m_KillfeedModule.OnPlayerHitBy( damageType, this, source, ammo );
			}
		}

		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
	}
	
	// ------------------------------------------------------------
	// Expansion SetActions
	// ------------------------------------------------------------
	override void SetActions( out TInputActionMap InputActionMap )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.PLAYER, this, "SetActions");
#endif

		super.SetActions( InputActionMap );
				
		//AddAction( ExpansionActionStartPlayingGuitar, InputActionMap );
		//AddAction( ExpansionActionStopPlayingGuitar, InputActionMap );
	}

	// ------------------------------------------------------------
	// PlayerBase IsPlayerAlreadyDeadAndCalledKillFeed IPADACK
	// ------------------------------------------------------------
	bool IPADACK()
	{
		return m_HasCalledKillFeed;
	}

	// ------------------------------------------------------------
	// PlayerBase UpdateIsPlayerAlreadyDeadAndCalledKillFeed UpdateIPADACK
	// ------------------------------------------------------------
	void UpdateIPADACK(bool state = true)
	{
		m_HasCalledKillFeed = state;
	}

	// ------------------------------------------------------------
	// Expansion SpawnGraveCross
	// ------------------------------------------------------------
	void CreateGraveCross()
	{
		int lifetimeThreshhold = GetExpansionSettings().GetGeneral().GravecrossTimeThreshold;
		bool deleteBody = GetExpansionSettings().GetGeneral().GravecrossDeleteBody;

		string graveobject = "Expansion_Gravecross";

		//! Offset of 0.6 is to account for cross anchor point not being at the bottom of the cross,
		//! if we change cross object and not using ECE_TRACE this needs to be adjusted!
		float offsetY = 0.6;
		
		float playtime = StatGet("playtime");

		if (playtime <= lifetimeThreshhold)
		{
			graveobject = "Expansion_Gravecross_LowLifetime";

			//! Offset of 1.035 is to account for cross anchor point not being at the bottom of the cross,
			//! if we change cross object and not using ECE_TRACE this needs to be adjusted!
			offsetY = 1.035;
		}

		Expansion_GraveBase grave;

		vector pos = GetPosition();
		vector ground = Vector(pos[0], GetGame().SurfaceY(pos[0], pos[2]), pos[2]);

		//! The idea here is that the gravecross should spawn on top of the thing the player died on if it's a building or large item,
		//! and not below sea level if over water

		PhxInteractionLayers layerMask;

		layerMask |= PhxInteractionLayers.BUILDING;
		layerMask |= PhxInteractionLayers.ROADWAY;
		layerMask |= PhxInteractionLayers.TERRAIN;
		layerMask |= PhxInteractionLayers.WATERLAYER;  //! Doesn't seem to work?
		layerMask |= PhxInteractionLayers.ITEM_LARGE;

		Object hitObject;
		vector hitPosition;
		vector hitNormal;
		float hitFraction;

		if ( DayZPhysics.RayCastBullet( pos, ground, layerMask, this, hitObject, hitPosition, hitNormal, hitFraction ) )
			ground[1] = hitPosition[1];

		float water_depth = GetGame().GetWaterDepth(ground);

		if ( water_depth > 0 )
		{
			//! Add water depth so cross sits above water level (with bottom bit submerged slightly)
			ground[1] = ground[1] + water_depth - 0.5;
		}
		
		ground[1] = ground[1] + offsetY;

		grave = Expansion_GraveBase.Cast(GetGame().CreateObjectEx(graveobject, ground, ECE_CREATEPHYSICS|ECE_UPDATEPATHGRAPH));
		grave.SetPosition(ground);
		
		grave.MoveAttachmentsFromEntity(this, ground, GetOrientation());
		grave.SetOrientation(GetOrientation());
		
		if (deleteBody)
			Delete();
	}
	
	// ------------------------------------------------------------
	// PlayerBase Debug_PlayerForward
	// ------------------------------------------------------------	
	void Debug_PlayerForward()
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_0(ExpansionTracing.PLAYER, this, "Debug_PlayerForward");
#endif

		float speed;
		vector direction;
		GetInputController().GetMovement( speed, direction );

		Transform trans = Transform.GetObject( this );
		direction = direction.Multiply3( trans.GetBasis().data );

		vector pos = GetPosition() + direction;

		m_PlayerHeadingDir.SetPosition( pos );
		m_PlayerHeadingDir.SetDirection( direction );
	}
	
	// ------------------------------------------------------------
	// Expansion ModCommandHandlerInside
	// ------------------------------------------------------------
	override bool ModCommandHandlerInside( float pDt, int pCurrentCommandID, bool pCurrentCommandFinished )	
	{
		if ( super.ModCommandHandlerInside( pDt, pCurrentCommandID, pCurrentCommandFinished ) )
		{
			return true;
		}

		if ( pCurrentCommandID == DayZPlayerConstants.COMMANDID_SCRIPT )
		{
			//ExpansionHumanCommandGuitar ehcg = ExpansionHumanCommandGuitar.Cast( GetCommand_Script() );
			//if ( ehcg != NULL )
			//{
			//	return true;
			//}
		}

		return false;
	}
	
	// ------------------------------------------------------------
	// PlayerBase HasItem
	// ------------------------------------------------------------
	//! Not used anywhere?!
	/*bool HasItem( string name, out EntityAI item )
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_1(ExpansionTracing.PLAYER, this, "HasItem").Add(name);
#endif

		if ( !GetInventory() )
			return false;
		
		for ( int att_i = 0; att_i < GetInventory().AttachmentCount(); ++att_i )
		{
			EntityAI attachment = GetInventory().GetAttachmentFromIndex( att_i );
			CargoBase cargo = attachment.GetInventory().GetCargo();
			
			if ( !cargo )
				continue;

			for ( int cgo_i = 0; cgo_i < cargo.GetItemCount(); ++cgo_i )
			{
				EntityAI cargo_item = cargo.GetItem( cgo_i );
				if ( !cargo_item )
					continue;

				if ( cargo_item.GetType() == name )
					return Class.CastTo( item, cargo_item );
			}
		}
			
		return false;
	}*/

	/*
	// ------------------------------------------------------------
	// Expansion StartCommand_ExpansionGuitar
	// ------------------------------------------------------------
	override ExpansionHumanCommandGuitar StartCommand_ExpansionGuitar( Expansion_Guitar guitar )
	{
		ExpansionHumanCommandGuitar cmd = new ExpansionHumanCommandGuitar( this, guitar, m_ExpansionST );
		StartCommand_Script( cmd );
		return cmd;
	}
	*/

	override void UpdatePlayerMeasures()
	{
		if (!m_Expansion_SpawnSelect)
			super.UpdatePlayerMeasures();
	}
};
