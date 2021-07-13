/**
 * ExpansionPlayerBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class PlayerBase
{
	private int m_ExpansionSaveVersion;

	Object m_PlayerHeadingDir;

	protected ref Timer m_SafezoneUpdateTimer;

	protected bool m_SafeZone;
	protected bool m_SafeZoneSynchRemote;
	protected bool m_LeavingSafeZone;

	protected int m_TerritoryIdInside;

	protected ExpansionTerritoryModule m_TerritoryModule;
	
	protected string m_PlayerUID;
	protected string m_PlayerSteam;
	protected string m_PlayerName;

	private static autoptr map< string, PlayerBase > m_AllPlayersUID = new map< string, PlayerBase >;
	private static autoptr array< PlayerBase > m_AllPlayers = new array< PlayerBase >;

	protected bool m_HasMap;
	protected bool m_HasGPS;
	protected bool m_HasPen;
	protected bool m_HasCompass;

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

		#ifndef EXPANSIONMODVEHICLE
		//! PlayerBase in Vehicles_Scripts calls this on its own
		if ( IsMissionClient() && GetGame() && GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ) ) 
			GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( this.DeferredClientInit, 100, false );
		#endif

		m_SafeZone = false;
		m_LeavingSafeZone = false;
		
		m_TerritoryIdInside = -1;

		Class.CastTo( m_TerritoryModule, GetModuleManager().GetModule( ExpansionTerritoryModule ) );

		m_HasMap = false;
		m_HasGPS = false;
		m_HasPen = false;
		m_HasCompass = false;
		m_HasCalledKillFeed = false;

		m_CountMap = 0;
		m_CountGPS = 0;
		m_CountPen = 0;
		m_CountCompass = 0;
		
		SetRandomQuickMarkerColor();
		
		m_AllPlayers.Insert( this );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::PlayerBase - End");
		#endif
	}

	// ------------------------------------------------------------
	// Override EEKilled
	// ------------------------------------------------------------
	override void EEKilled( Object killer )
	{
		if ( GetExpansionSettings().GetNotification().EnableKillFeed )
		{
			m_KillfeedModule = ExpansionKillFeedModule.Cast( GetModuleManager().GetModule( ExpansionKillFeedModule ) );
			if ( m_KillfeedModule )
			{
				m_KillfeedModule.PlayerKilled( this, killer );
			}
		}

		if ( GetExpansionSettings().GetGeneral().EnableGravecross )
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(CreateGraveCross, 5000, false, IsSwimming());
		}
		
		super.EEKilled(killer);
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
				m_KillfeedModule.PlayerHitBy( damageType, this, source, ammo );
			}
		}

		if (ammo == "Bullet_Expansion_Taser")
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(WakePlayer, 9000, false);

		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);
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

		if ( m_AllPlayersUID && m_AllPlayersUID.Get( m_PlayerUID ) == this )
		{
			m_AllPlayersUID.Remove( m_PlayerUID );
		}

		if ( m_AllPlayers )
		{
			int remove_index = m_AllPlayers.Find( this );
			if ( remove_index >= 0 )
			{
				m_AllPlayers.Remove( remove_index );
			}
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::~PlayerBase - End");
		#endif
	}
		
	// ------------------------------------------------------------
	// PlayerBase DeferredClientInit
	// ------------------------------------------------------------
	override void DeferredClientInit()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::DeferredClientInit - Start");
		#endif
		
		super.DeferredClientInit();
		
		#ifdef EXPANSIONEXLOGPRINT
		if ( GetGame() && GetGame().GetPlayer() )
		{
			EXLogPrint( "Player Has Entered Network Bubble at " + GetPosition() + " while we are at " + GetGame().GetPlayer().GetPosition() );
		}
		#endif

		if ( GetGame() && IsMissionClient() && GetModuleManager() )
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
	// Expansion OnVariablesSynchronized
	// ------------------------------------------------------------
	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::OnVariablesSynchronized - Start");
		#endif

		if ( m_SafeZoneSynchRemote && !m_SafeZone )
		{
			OnEnterSafeZone();
		} 
		else if ( !m_SafeZoneSynchRemote && m_SafeZone )
		{
			OnLeftSafeZone();
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::OnVariablesSynchronized - End");
		#endif
	}

	override void OnUnconsciousStart()
	{
		if ( IsMissionClient() && GetGame().GetUIManager().GetMenu() && GetGame().GetUIManager().GetMenu().IsVisible() )
			GetGame().GetUIManager().CloseAll();

		super.OnUnconsciousStart();
	}

	// ------------------------------------------------------------
	// Expansion Init
	// ------------------------------------------------------------
	override void Init()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::Init start");
		#endif
		
		super.Init();

		m_SafezoneUpdateTimer = new Timer();
		m_SafezoneUpdateTimer.Run(1, this, "SafezoneUpdate", null, true);

		RegisterNetSyncVariableBool( "m_SafeZoneSynchRemote" );

		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::Init end");
		#endif
	}
	
	// ------------------------------------------------------------
	// PlayerBase IsInSafeZone
	// ------------------------------------------------------------
	bool IsInSafeZone()
	{
		return m_SafeZone;
	}

	// ------------------------------------------------------------
	// PlayerBase OnEnterSafeZone, only server side
	// ------------------------------------------------------------
	void OnEnterSafeZone()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::OnEnterSafeZone start");
		#endif

		m_SafeZone = true;		
		m_LeavingSafeZone = false;

		if ( IsMissionHost() )
		{
			m_SafeZoneSynchRemote = true;

			SetAllowDamage( false );

			GetInputController().OverrideRaise( true, false );
			GetInputController().OverrideMeleeEvade( true, false );

			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_SAFEZONE_TITLE" ), new StringLocaliser( "STR_EXPANSION_SAFEZONE_ENTER" ), EXPANSION_NOTIFICATION_ICON_INFO, COLOR_EXPANSION_NOTIFICATION_SUCCSESS, 7, GetIdentity() );
		
			SetSynchDirty();
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::OnEnterSafeZone end");
		#endif
	}

	// ------------------------------------------------------------
	// PlayerBase OnLeftSafeZone, only server side
	// ------------------------------------------------------------
	void OnLeftSafeZone()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::OnLeftSafeZone - Start");
		#endif

		if ( m_SafeZone && !m_LeavingSafeZone )
		{
			m_SafeZone = false;
			m_LeavingSafeZone = true;

			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_SAFEZONE_TITLE" ), new StringLocaliser( "STR_EXPANSION_SAFEZONE_LEAVING" ), EXPANSION_NOTIFICATION_ICON_INFO, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, GetIdentity() );
			
			//! Wait 10 seconds
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(HasLeftSafeZone, 10000, false);
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::OnLeftSafeZone - End");
		#endif
	}

	// ------------------------------------------------------------
	// PlayerBase HasLeftSafeZone, only server side
	// ------------------------------------------------------------
	void HasLeftSafeZone()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::HasLeftSafeZone - Start");
		#endif

		//! If the player is still outside of the safezone
		if ( IsMissionHost() && !m_SafeZone )
		{
			m_SafeZoneSynchRemote = false;

			SetAllowDamage( true );

			GetInputController().OverrideRaise( false, false );
			GetInputController().OverrideMeleeEvade( false, false );

			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_SAFEZONE_TITLE" ), new StringLocaliser( "STR_EXPANSION_SAFEZONE_LEFT" ), EXPANSION_NOTIFICATION_ICON_INFO, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, GetIdentity() );
		
			SetSynchDirty();
		}
		
		m_LeavingSafeZone = false;

		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::HasLeftSafeZone - End");
		#endif
	}

	// ------------------------------------------------------------
	// PlayerBase SafezoneUpdate
	// ------------------------------------------------------------
	void SafezoneUpdate()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::SafezoneUpdate start");
		#endif
		
		static bool switched = false;
		if ( IsInSafeZone() ) 
		{
			switched = true;
			GetInputController().OverrideRaise( true, false );
			GetInputController().OverrideMeleeEvade( true, false );
		}
		else
		{
			if (switched)
			{
				GetInputController().OverrideRaise( false, false );
				GetInputController().OverrideMeleeEvade( false, false );
				switched = false;
			}	
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::SafezoneUpdate end");
		#endif
	}

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
	// PlayerBase SetTerritoryIDInside, only server side
	// ------------------------------------------------------------
	void SetTerritoryIDInside(int territoryID)
	{
		m_TerritoryIdInside = territoryID;
	}
	
	// ------------------------------------------------------------
	// PlayerBase GetTerritoryIDInside, only server side
	// ------------------------------------------------------------
	int GetTerritoryIDInside()
	{
		return m_TerritoryIdInside;
	}
	
	// ------------------------------------------------------------
	// PlayerBase FindNearestFlag
	// Returns nearest ExpansionFlagBase object at given position
	// or NULL if no ExpansionFlagBase object is found.
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
	void CreateGraveCross( bool wasSwimming )
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
		vector ground;

		//! The idea here is that the gravecross should spawn on top of the thing the player died on if it's a building or large item,
		//! and not below sea level if over water

		if ( wasSwimming )
		{
			//! Add a bit of vertical offset if the player was swimming so cross sits above water level
			ground = Vector(pos[0], pos[1] + 1.3, pos[2]);
		} else
		{
			ground = Vector(pos[0], GetGame().SurfaceY(pos[0], pos[2]), pos[2]);

			PhxInteractionLayers layerMask = PhxInteractionLayers.BUILDING|PhxInteractionLayers.ROADWAY|PhxInteractionLayers.TERRAIN|PhxInteractionLayers.WATERLAYER|PhxInteractionLayers.ITEM_LARGE;
			Object hitObject;
			vector hitPosition;
			vector hitNormal;
			float hitFraction;

			if ( DayZPhysics.RayCastBullet( pos, ground, layerMask, this, hitObject, hitPosition, hitNormal, hitFraction ) )
				ground[1] = hitPosition[1];
		}
		
		ground[1] = ground[1] + offsetY;

		grave = Expansion_GraveBase.Cast(GetGame().CreateObjectEx(graveobject, ground, ECE_CREATEPHYSICS|ECE_UPDATEPATHGRAPH));
		grave.SetPosition(ground);
		
		grave.MoveAttachmentsFromEntity(this);
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
			ExpansionHumanCommandVehicle ehcv = ExpansionHumanCommandVehicle.Cast( GetCommand_Script() );
			if ( ehcv != NULL )
			{
				if ( ehcv.WasGearChange() )
				{
					ExpansionGearChangeActionCallback cb = ExpansionGearChangeActionCallback.Cast( AddCommandModifier_Action( DayZPlayerConstants.CMD_ACTIONMOD_SHIFTGEAR, ExpansionGearChangeActionCallback ) );
					cb.SetVehicleCommand( ehcv );
				}
				return true;
			}

			//ExpansionHumanCommandGuitar ehcg = ExpansionHumanCommandGuitar.Cast( GetCommand_Script() );
			//if ( ehcg != NULL )
			//{
			//	return true;
			//}
		}

		return false;
	}
	
	// ------------------------------------------------------------
	// Expansion SendChatMessage 
	// ------------------------------------------------------------
	override void SendChatMessage( string message )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::SendChatMessage - Start");
		#endif

		if ( IsMissionClient() )
		{
			GetGame().GetMission().OnEvent( ChatMessageEventTypeID, new ChatMessageEventParams( ExpansionChatChannels.CCDirect, "", message, "" ) );
		}
		else
		{
			array<Man> players = new array<Man>;
			GetGame().GetPlayers( players );
			
			foreach( auto player : players  )
			{
				Param1<string> m_MessageParam = new Param1<string>(message);
				GetGame().RPCSingleParam( player, ERPCs.RPC_USER_ACTION_MESSAGE, m_MessageParam, true, player.GetIdentity() );
			}
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::SendChatMessage - End");
		#endif
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
			ref CargoBase cargo = attachment.GetInventory().GetCargo();
			
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
	// PlayerBase OnCommandExpansionVehicleStart
	// ------------------------------------------------------------
	override void OnCommandExpansionVehicleStart()
	{
		super.OnCommandExpansionVehicleStart();

		if ( GetInventory() )
			GetInventory().LockInventory( LOCK_FROM_SCRIPT );

		ItemBase itemInHand = GetItemInHands();
		EntityAI itemOnHead = FindAttachmentBySlotName( "Headgear" );

		if ( itemInHand )
		{
			if ( itemInHand.GetCompEM() )
			{
				itemInHand.GetCompEM().SwitchOff();
			}

			GetItemAccessor().HideItemInHands(true);
		}

		if ( itemOnHead )
		{
			if ( itemOnHead.GetCompEM() )
			{
				itemOnHead.GetCompEM().SwitchOff();
			}
		}
		
		ExpansionHumanCommandVehicle hcv = GetCommand_ExpansionVehicle();
		if ( hcv && hcv.GetVehicleSeat() == DayZPlayerConstants.VEHICLESEAT_DRIVER )
		{
			OnVehicleSeatDriverEnter();
		}
	}

	// ------------------------------------------------------------
	// Expansion HeadingModel
	// ------------------------------------------------------------
	override bool HeadingModel( float pDt, SDayZPlayerHeadingModel pModel )
	{
		if ( GetCommand_ExpansionVehicle() )
		{
			m_fLastHeadingDiff = 0;

			pModel.m_fOrientationAngle = 0;
			return true;
		}

		return super.HeadingModel( pDt, pModel );
	}
	
	// ------------------------------------------------------------
	// PlayerBase SetHasItemMap
	// ------------------------------------------------------------
	void SetHasItemMap(bool state)
	{
		if ( state )
		{
			m_CountMap = m_CountMap + 1;
		} else {
			m_CountMap = m_CountMap - 1;
		}

		if ( m_CountMap < 0 )
			m_CountMap = 0;

		m_HasMap = state;
	}
	
	// ------------------------------------------------------------
	// PlayerBase HasItemMap
	// ------------------------------------------------------------
	bool HasItemMap()
	{
		if ( m_CountMap > 0 )
			return true;

		return m_HasMap;
	}
	
	// ------------------------------------------------------------
	// PlayerBase SetHasItemGPS
	// ------------------------------------------------------------
	void SetHasItemGPS(bool state)
	{
		if ( state )
		{
			m_CountGPS = m_CountGPS + 1;
		} else {
			m_CountGPS = m_CountGPS - 1;
		}

		if ( m_CountGPS < 0 )
			m_CountGPS = 0;

		m_HasGPS = state;
	}
	
	// ------------------------------------------------------------
	// PlayerBase HasItemGPS
	// ------------------------------------------------------------
	bool HasItemGPS()
	{
		if ( m_CountGPS > 0 )
			return true;

		return m_HasGPS;
	}
	
	// ------------------------------------------------------------
	// PlayerBase SetHasItemPen
	// ------------------------------------------------------------
	void SetHasItemPen(bool state)
	{
		if ( state )
		{
			m_CountPen = m_CountPen + 1;
		} else {
			m_CountPen = m_CountPen - 1;
		}

		if ( m_CountPen < 0 )
			m_CountPen = 0;

		m_HasPen = state;
	}
	
	// ------------------------------------------------------------
	// PlayerBase HasItemPen
	// ------------------------------------------------------------
	bool HasItemPen()
	{
		if ( m_CountPen > 0 )
			return true;

		return m_HasPen;
	}
	
	// ------------------------------------------------------------
	// PlayerBase SetHasItemCompass
	// ------------------------------------------------------------
	void SetHasItemCompass(bool state)
	{
		if ( state )
		{
			m_CountCompass = m_CountCompass + 1;
		} else {
			m_CountCompass = m_CountCompass - 1;
		}

		if ( m_CountCompass < 0 )
			m_CountCompass = 0;

		m_HasCompass = state;
	}
	
	// ------------------------------------------------------------
	// PlayerBase HasItemCompass
	// ------------------------------------------------------------
	bool HasItemCompass()
	{
		if ( m_CountCompass > 0 )
			return true;

		return m_HasCompass;
	}

	void WakePlayer()
	{
		this.AddHealth("","Shock", 100);
	}
};