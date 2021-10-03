/**
 * ExpansionPlayerBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class PlayerBase
{
	private int m_ExpansionSaveVersion;

	Object m_PlayerHeadingDir;
	
	protected int m_TerritoryIdInside;

	protected ExpansionTerritoryModule m_TerritoryModule;
	
	protected string m_PlayerUID;
	protected string m_PlayerSteam;
	protected string m_PlayerName;

	protected int m_CountMap;
	protected int m_CountGPS;
	protected int m_CountPen;
	protected int m_CountCompass;

	private bool m_HasCalledKillFeed;
	
	//Only server side
	protected int m_QuickMarkerColor;
	
	ExpansionKillFeedModule m_KillfeedModule;
	
	// ------------------------------------------------------------
	// PlayerBase Constructor
	// ------------------------------------------------------------
	void PlayerBase()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::PlayerBase - Start");
		#endif

		if ( IsMissionClient() && GetGame() && GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ) ) 
			GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( this.DeferredClientInit, 100, false );
		
		m_TerritoryIdInside = -1;

		Class.CastTo( m_TerritoryModule, GetModuleManager().GetModule( ExpansionTerritoryModule ) );

		m_HasCalledKillFeed = false;
	
		m_CountMap = 0;
		m_CountGPS = 0;
		m_CountPen = 0;
		m_CountCompass = 0;
		
		SetRandomQuickMarkerColor();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::PlayerBase - End");
		#endif
	}

	// ------------------------------------------------------------
	// PlayerBase Destructor
	// ------------------------------------------------------------
	void ~PlayerBase()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::~PlayerBase - Start");
		#endif

		if ( IsMissionClient() && GetGame() && GetGame().GetPlayer() ) 
		{
			// need this for expansion (test 3) debugging
			// need to confirm if an issue is fixed and if 
			// not, need logs sent to me
			// 	- jacob
			#ifdef EXPANSIONEXLOGPRINT
			EXLogPrint( "Player Has Left Network Bubble from " + GetPosition() + " while we are at " + GetGame().GetPlayer().GetPosition() );
			#endif
		}

		//! Making sure we remove tha call for CreateGraveCross when ever the player base entity gets destroyed
		if ( GetGame() && GetExpansionSettings().GetGeneral().EnableGravecross )
		{
		#ifdef ENFUSION_AI_PROJECT
			if (!IsInherited(eAIBase))
		#endif
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Remove(CreateGraveCross);
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::~PlayerBase - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Override EEKilled
	// ------------------------------------------------------------
	override void EEKilled( Object killer )
	{
		if ( GetExpansionSettings().GetGeneral().EnableGravecross )
		{
		#ifdef ENFUSION_AI_PROJECT
			if (!IsInherited(eAIBase))
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
		if ( GetExpansionSettings().GetNotification().EnableKillFeed )
		{
			m_KillfeedModule = ExpansionKillFeedModule.Cast( GetModuleManager().GetModule( ExpansionKillFeedModule ) );
			if ( m_KillfeedModule && !IPADACK() )
			{
				UpdateIPADACK( !IsAlive() );
				m_KillfeedModule.OnPlayerHitBy( damageType, this, source, ammo );
			}
		}

		if (ammo == "Bullet_Expansion_Taser")
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(WakePlayer, 9000, false);

		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
	}
	
	// ------------------------------------------------------------
	// PlayerBase DeferredClientInit
	// ------------------------------------------------------------
	override void DeferredClientInit()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint(ToString() + "::DeferredClientInit - Start");
		#endif
		
		super.DeferredClientInit();
		
		#ifdef EXPANSIONEXLOGPRINT
		if ( GetGame() && GetGame().GetPlayer() )
		{
			EXLogPrint( ToString() + " '" + GetIdentityName() + "' (id=" + GetIdentityUID() + ") has entered network bubble at " + GetPosition() + " while we are at " + GetGame().GetPlayer().GetPosition() );
		}
		#endif

		//! Only run this on this client's player, not other playerbase entities (it will be handled in their client if they are players)
		if ( GetGame() && IsMissionClient() && GetGame().GetPlayer() == this && GetModuleManager() )
		{
			ExpansionMarkerModule module;
			if ( Class.CastTo( module, GetModuleManager().GetModule( ExpansionMarkerModule ) ) )
				module.Refresh();
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::DeferredClientInit - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion SetActions
	// ------------------------------------------------------------
	override void SetActions( out TInputActionMap InputActionMap )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::SetActions start");
		#endif

		super.SetActions( InputActionMap );
				
		//AddAction( ExpansionActionOpenParachute, InputActionMap );
		//AddAction( ExpansionActionCutParachute, InputActionMap );
		AddAction( ExpansionActionSelectNextPlacement, InputActionMap );
		AddAction( ExpansionActionCrackSafe );
		AddAction( ExpansionActionDestroyBarbedWire );
		AddAction( ExpansionActionDestroyLock );
		//AddAction( ExpansionActionStartPlayingGuitar, InputActionMap );
		//AddAction( ExpansionActionStopPlayingGuitar, InputActionMap );

		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::SetActions end");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion OnUnconsciousStart
	// ------------------------------------------------------------
	override void OnUnconsciousStart()
	{
		if ( IsMissionClient() && GetGame().GetUIManager().GetMenu() && GetGame().GetUIManager().GetMenu().IsVisible() )
			GetGame().GetUIManager().CloseAll();

		super.OnUnconsciousStart();
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
	// Expansion SetRandomQuickMarkerColor
	// ------------------------------------------------------------
	void SetRandomQuickMarkerColor()
	{
		if ( IsMissionHost() )
		{
			array<int> colors = new array<int>;
			
			int geen_sea = ARGB(255, 22, 160, 133);
			colors.Insert(geen_sea);
			
			int nephritis = ARGB(255, 39, 174, 96);
			colors.Insert(nephritis);
			
			int belize_hole = ARGB(255, 41, 128, 185);
			colors.Insert(belize_hole);
			
			int wisteria = ARGB(255, 142, 68, 173);
			colors.Insert(wisteria);
			
			int orange = ARGB(255, 243, 156, 18);
			colors.Insert(orange);
			
			int pumpkin = ARGB(255, 211, 84, 0);
			colors.Insert(pumpkin);
			
			int pomegranate = ARGB(255, 192, 57, 43);
			colors.Insert(pomegranate);
			
			int greenland_green = ARGB(255, 34, 166, 179);
			colors.Insert(greenland_green);
			
			int steel_pink = ARGB(255, 190, 46, 221);
			colors.Insert(steel_pink);
	
			m_QuickMarkerColor = colors.GetRandomElement();
		}
	}
	
	// ------------------------------------------------------------
	// PlayerBase GetQuickMarkerColor
	// ------------------------------------------------------------
	int GetQuickMarkerColor()
	{
		return m_QuickMarkerColor;
	}
	
	// ------------------------------------------------------------
	// PlayerBase GetBuildNoBuildZone
	// ------------------------------------------------------------
	ExpansionBuildNoBuildZone GetBuildNoBuildZone()
	{
		foreach ( ExpansionBuildNoBuildZone zone : GetExpansionSettings().GetBaseBuilding().Zones )
		{
			vector pos = GetPosition();
			vector center = zone.Center;

			if ( center[1] == 0 )
				pos[1] == 0;
	
			if ( vector.Distance( pos, zone.Center ) <= zone.Radius )
				return zone;
		}

		return NULL;
	}
	
	// ------------------------------------------------------------
	// PlayerBase TerritoryModuleExists
	// ------------------------------------------------------------
	private bool TerritoryModuleExists()
	{
		if ( !m_TerritoryModule )
		{
			#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
			EXPrint("PlayerBase::TerritoryModuleExists - [ERROR] Territory module is NULL!");
			#endif
			return false;
		}

		return true;
	}
	
	// ------------------------------------------------------------
	// PlayerBase IsInsideOwnPerimeter
	// Check if player is in own territory's perimeter (but not in territory itself)
	// ------------------------------------------------------------
	bool IsInsideOwnPerimeter( float territorySize = -1, float perimeterSize = -1 )
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXPrint("PlayerBase::IsInsideOwnPerimeter - Start");
		#endif

		return !IsInsideOwnTerritory( territorySize ) && IsInsideOwnTerritoryOrPerimeter( territorySize, perimeterSize );
	}
		
	// ------------------------------------------------------------
	// PlayerBase IsInPerimeter
	// Check if player is in a territory's perimeter (but not in territory itself)
	// ------------------------------------------------------------
	bool IsInPerimeter( float territorySize = -1, float perimeterSize = -1 )
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXPrint("PlayerBase::IsInPerimeter - Start");
		#endif

		return !IsInTerritory( territorySize ) && IsInTerritoryOrPerimeter( territorySize, perimeterSize );
	}
		
	// ------------------------------------------------------------
	// PlayerBase IsInTerritory
	// Check if player is in a territory
	// ------------------------------------------------------------
	bool IsInTerritory(float territorySize = -1)
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXPrint("PlayerBase::IsInTerritory - Start");
		#endif

		if ( !TerritoryModuleExists() )
			return false;
			
		return m_TerritoryModule.IsInTerritory( GetPosition(), territorySize );
	}

	// ------------------------------------------------------------
	// PlayerBase IsInTerritoryOrPerimeter
	// Check if player is in a territory or in its perimeter
	// ------------------------------------------------------------
	bool IsInTerritoryOrPerimeter( float territorySize = -1, float perimeterSize = -1 )
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXPrint("PlayerBase::IsInTerritoryOrPerimeter - Start");
		#endif

		if ( !TerritoryModuleExists() )
			return false;
			
		return m_TerritoryModule.IsInTerritoryOrPerimeter( GetPosition(), territorySize, perimeterSize );
	}
	
	// ------------------------------------------------------------
	// PlayerBase IsInsideOwnTerritory
	// Check if player is in own territory
	// ------------------------------------------------------------
	bool IsInsideOwnTerritory( float territorySize = -1 )
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXPrint("PlayerBase::IsInsideOwnTerritory - Start");
		#endif

		if ( !TerritoryModuleExists() )
			return false;

		string playerUID;
		if ( IsMissionHost() )
			playerUID = m_PlayerUID;

		return m_TerritoryModule.IsInsideOwnTerritory( GetPosition(), territorySize, playerUID );
	}

	// ------------------------------------------------------------
	// PlayerBase IsInsideOwnTerritoryOrPerimeter
	// Check if player is in own territory or in its perimeter
	// ------------------------------------------------------------
	bool IsInsideOwnTerritoryOrPerimeter( float territorySize = -1, float perimeterSize = -1 )
	{
		#ifdef EXPANSION_TERRITORY_MODULE_DEBUG
		EXPrint("PlayerBase::IsInsideOwnTerritoryOrPerimeter - Start");
		#endif

		if ( !TerritoryModuleExists() )
			return false;

		string playerUID;
		if ( IsMissionHost() )
			playerUID = m_PlayerUID;
			
		return m_TerritoryModule.IsInsideOwnTerritoryOrPerimeter( GetPosition(), territorySize, perimeterSize, playerUID );
	}
	
	// ------------------------------------------------------------
	// PlayerBase SetTerritoryIDInside, only client side
	// ------------------------------------------------------------
	void SetTerritoryIDInside(int territoryID)
	{
		m_TerritoryIdInside = territoryID;
	}
	
	// ------------------------------------------------------------
	// PlayerBase GetTerritoryIDInside, only client side
	// ------------------------------------------------------------
	int GetTerritoryIDInside()
	{
		return m_TerritoryIdInside;
	}
	
	// ------------------------------------------------------------
	// PlayerBase FindNearestFlag
	// Returns nearest TerritoryFlag object at given position
	// or NULL if no TerritoryFlag object is found.
	// ------------------------------------------------------------
	TerritoryFlag FindNearestFlag()
	{
		if ( !IsMissionClient() )
			return NULL;
		
		vector pos = GetPosition();
		array<Object> objects = new array<Object>;
		array<CargoBase> proxyCargos = new array<CargoBase> ;
		float radius = 10;
		GetGame().GetObjectsAtPosition3D( pos, radius, objects, proxyCargos );
		float distance;
		TerritoryFlag nearestFlag;
		
		if ( objects && objects.Count() > 0 )
		{
			for ( int i = 0; i < objects.Count(); ++i )
			{
				TerritoryFlag flag;
				if ( Class.CastTo( flag, objects.Get( i ) ) )
				{
					distance = vector.Distance( pos, flag.GetPosition() );
					if ( distance < radius )
					{
						radius = distance;
						nearestFlag = flag;
					}
				}
			}
		}

		return nearestFlag;
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
		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::Debug_PlayerForward - Start");
		#endif

		float speed;
		vector direction;
		GetInputController().GetMovement( speed, direction );

		Transform trans = Transform.GetObject( this );
		direction = direction.Multiply3( trans.GetBasis().data );

		vector pos = GetPosition() + direction;

		m_PlayerHeadingDir.SetPosition( pos );
		m_PlayerHeadingDir.SetDirection( direction );

		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::Debug_PlayerForward - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion ModCommandHandlerInside
	// ------------------------------------------------------------
	override bool ModCommandHandlerInside( float pDt, int pCurrentCommandID, bool pCurrentCommandFinished )	
	{
		ItemBase item = GetItemInHands();
		if ( item )
		{
			item.UpdateLaser();
		}

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
	bool HasItem( string name, out EntityAI item )
	{
		if ( !GetInventory() )
			return false;

		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::HasItem - Start");
		#endif
		
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
	
		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::HasItem - End");
		#endif
		
		return false;
	}
	
	// ------------------------------------------------------------
	// Expansion CanJump
	// ------------------------------------------------------------
	override bool CanJump()
	{
		if ( GetCommand_ExpansionFall() )
			return false;

		return super.CanJump();
	}
	
	// ------------------------------------------------------------
	// Expansion IsTargetInActiveRefresherRange
	// ------------------------------------------------------------	
	override bool IsTargetInActiveRefresherRange(EntityAI target)
	{
		//! Duplicate of vanilla code except using TerritorySize instead of constant
		array<vector> temp = new array<vector>;
		temp = GetGame().GetMission().GetActiveRefresherLocations();
		int count = temp.Count();
		if (count > 0)
		{
			float territorySize = GetExpansionSettings().GetTerritory().TerritorySize;

			vector pos = target.GetPosition();
			for (int i = 0; i < count; i++)
			{
				if ( vector.Distance(pos,temp.Get(i)) < territorySize )
					return true;
			}
			
			return false;
		}
		else
		{
			return false;
		}
	}

	/*
	// ------------------------------------------------------------
	// Expansion StartCommand_ExpansionGuitar
	// ------------------------------------------------------------
	override ExpansionHumanCommandGuitar StartCommand_ExpansionGuitar( Expansion_Guitar guitar )
	{
		if ( m_ExpansionST == NULL )
			m_ExpansionST = new ExpansionHumanST( this );
	
		ExpansionHumanCommandGuitar cmd = new ExpansionHumanCommandGuitar( this, guitar, m_ExpansionST );
		StartCommand_Script( cmd );
		return cmd;
	}
	*/
	
	// ------------------------------------------------------------
	// PlayerBase SetHasItemMap
	// ------------------------------------------------------------
	void SetHasItemMap(bool state)
	{
		if ( state )
		{
			m_CountMap += 1;
		}
		else if (m_CountMap > 0)
		{
			m_CountMap -= 1;
		}
		
		Print("m_CountMap: " + m_CountMap);
	}
	
	// ------------------------------------------------------------
	// PlayerBase HasItemMap
	// ------------------------------------------------------------
	bool HasItemMap()
	{
		return m_CountMap > 0;
	}
	
	// ------------------------------------------------------------
	// PlayerBase SetHasItemGPS
	// ------------------------------------------------------------
	void SetHasItemGPS(bool state)
	{
		if ( state )
		{
			m_CountGPS += 1;
		} 
		else if (m_CountGPS > 0)
		{
			m_CountGPS -= 1;
		}
		
		Print("m_CountGPS: " + m_CountGPS);
	}
	
	// ------------------------------------------------------------
	// PlayerBase HasItemGPS
	// ------------------------------------------------------------
	bool HasItemGPS()
	{
		return m_CountGPS > 0;
	}
	
	// ------------------------------------------------------------
	// PlayerBase SetHasItemPen
	// ------------------------------------------------------------
	void SetHasItemPen(bool state)
	{
		if ( state )
		{
			m_CountPen += 1;
		} 
		else if (m_CountPen > 0)
		{
			m_CountPen -= 1;
		}
	}
	
	// ------------------------------------------------------------
	// PlayerBase HasItemPen
	// ------------------------------------------------------------
	bool HasItemPen()
	{
		return m_CountPen > 0;
	}
	
	// ------------------------------------------------------------
	// PlayerBase SetHasItemCompass
	// ------------------------------------------------------------
	void SetHasItemCompass(bool state)
	{
		if (state)
		{
			m_CountCompass += 1;
		} 
		else if (m_CountCompass > 0)
		{
			m_CountCompass -= 1;
		}
		
		Print("m_CountCompass: " + m_CountCompass);
	}
	
	// ------------------------------------------------------------
	// PlayerBase HasItemCompass
	// ------------------------------------------------------------
	bool HasItemCompass()
	{
		return m_CountCompass > 0;
	}
	
	// ------------------------------------------------------------
	// PlayerBase WakePlayer
	// ------------------------------------------------------------
	void WakePlayer()
	{
		this.AddHealth("","Shock", 100);
	}
};