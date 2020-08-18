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

/**@class		Modified Expansion PlayerBase
 * @brief		
 **/

modded class PlayerBase
{
	private int m_ExpansionSaveVersion;

	Object m_PlayerHeadingDir;

	protected ref Timer m_SafezoneUpdateTimer;

	protected bool m_SafeZone;
	protected bool m_SafeZoneSynchRemote;

	protected bool m_WasInVehicle;

	protected int m_TerritoryIdInside;

	protected ExpansionTerritoryModule m_TerritoryModule;

	protected autoptr array< ExpansionMoneyBase > m_Money;

	protected ref ExpansionMarketReserve m_MarketReserve;
	protected ref ExpansionMarketSell m_MarketSell;

	protected ref ExpansionHumanCommandVehicle_ST m_ExpansionVehicleCommandST;
	protected ref ExpansionHumanCommandFall_ST m_ExpansionFallCommandST;
	
	protected string m_PlayerUID;
	protected string m_PlayerSteam;
	protected string m_PlayerName;

	private static autoptr map< string, PlayerBase > m_AllPlayersUID = new map< string, PlayerBase >;
	private static autoptr array< PlayerBase > m_AllPlayers = new array< PlayerBase >;

	protected bool m_HasMap;
	protected bool m_HasGPS;
	
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
		{
			GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( this.DeferredClientInit, 100, false );
		}

		m_SafeZone = false;
		
		m_TerritoryIdInside = -1;

		Class.CastTo( m_TerritoryModule, GetModuleManager().GetModule( ExpansionTerritoryModule ) );

		m_MarketReserve = new ExpansionMarketReserve; 
		m_MarketSell = new ExpansionMarketSell;
		
		m_Money = new array< ExpansionMoneyBase >;

		m_HasMap = false;
		m_HasGPS = false;
		
		SetRandomQuickMarkerColor();
		
		m_AllPlayers.Insert( this );
		
		// SetEventMask( EntityEvent.POSTFRAME );
		
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

		if (GetExpansionSettings().GetGeneral().EnableGravecross)
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(CreateGraveCross, 5000, false, true);
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
			if ( m_KillfeedModule )
			{
				m_KillfeedModule.PlayerHitBy( damageType, this, source, ammo );
			}
		}
		if (ammo == "Bullet_Expansion_Taser")
		{
			GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLater(WakePlayer, 9000, false);
		}
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
			
			//! Generate some random colors to select from
			/*for ( int y = 0; y < 5; y++ )
			{
				int random_color = ARGB(255, Math.RandomIntInclusive(0, 255), Math.RandomIntInclusive(0, 255), Math.RandomIntInclusive(0, 255));
				colors.Insert(random_color);
			}*/
	
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
	
/*
	void ScaleObject( Object obj, float scale )
	{
		vector currTrans[4];
		vector newTrans[4];

		obj.GetTransform( currTrans );

		vector scaleMatrix[4] = { 
			Vector( scale, 0, 0 ), 
			Vector( 0, scale, 0 ), 
			Vector( 0, 0, scale ), 
			"0 0 0 1" };

		Math3D.MatrixMultiply4( scaleMatrix, currTrans, newTrans );

		newTrans[3] = currTrans[3];

		obj.SetTransform( newTrans );
	}

	override void EOnPostFrame( IEntity other, int extra )
	{
		Print( "PlayerBase::EOnPostFrame" );

		super.EOnPostFrame( other, extra );

		ScaleObject( this, 3.0 );
	}
*/

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

		delete m_MarketReserve; 
		delete m_MarketSell;

		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::~PlayerBase - End");
		#endif
	}
		
	// ------------------------------------------------------------
	// PlayerBase DeferredClientInit
	// ------------------------------------------------------------
	void DeferredClientInit()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::DeferredClientInit - Start");
		#endif
		
		if ( GetGame() && GetGame().GetPlayer() )
		{
			//! Print( "Player Has Entered Network Bubble at " + GetPosition() + " while we are at " + GetGame().GetPlayer().GetPosition() );
		}

		if ( GetGame() && IsMissionClient() && GetModuleManager() )
		{
			ExpansionMarkerModule module;
			if ( Class.CastTo( module, GetModuleManager().GetModule( ExpansionMarkerModule ) ) )
				module.Refresh();
		}

		/*
		m_PlayerHeadingDir = GetGame().CreateObject( "ExpansionDebugBox", "0 0 0", true );

		if ( m_PlayerHeadingDir )
		{
			dBodyDestroy( m_PlayerHeadingDir );
		}
		*/
		
		if ( IsMissionOffline() )
		{
			ExpansionMarketModule mod;
			if ( Class.CastTo( mod, GetModuleManager().GetModule( ExpansionMarketModule ) ) )
			{
				mod.SpawnMoney( PlayerBase.Cast(GetGame().GetPlayer()), 14555.9 );
				mod.SpawnMoney( PlayerBase.Cast(GetGame().GetPlayer()), 14555.9 );
				mod.SpawnMoney( PlayerBase.Cast(GetGame().GetPlayer()), 14555.9 );
				mod.SpawnMoney( PlayerBase.Cast(GetGame().GetPlayer()), 14555.9 );
			}
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::DeferredClientInit - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion SetActions
	// ------------------------------------------------------------
	override void SetActions()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::SetActions start");
		#endif

		super.SetActions();

		//RemoveAction( ActionGetOutTransport );	

		//AddAction( ExpansionActionGetOutTransport );
		AddAction( ExpansionActionGetOutExpansionVehicle );
		
		AddAction( ExpansionActionCarHorn );
		AddAction( ExpansionActionHelicopterHoverRefill );
		
		AddAction( ExpansionActionOpenParachute );
		AddAction( ExpansionActionCutParachute );

		AddAction( ExpansionActionStartEngine );
		AddAction( ExpansionActionStopEngine );
		
		AddAction( ExpansionActionStartBoat );
		AddAction( ExpansionActionStopBoat );

		AddAction( ExpansionActionSwitchBoatController );
		AddAction( ExpansionActionSwitchBoatControllerInput );

		AddAction( ExpansionActionSelectNextPlacement );

		AddAction( ExpansionActionPaint );

		#ifdef EXPANSION_VEHICLE_TOWING
		AddAction( ExpansionActionConnectTow );
		AddAction( ExpansionActionDisconnectTow );
		#endif
		
		//AddAction( ExpansionActionStartPlane );
		//AddAction( ExpansionActionStopPlane );

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
		
		m_SafeZone = false;

		if ( IsMissionHost() )
		{
			m_SafeZoneSynchRemote = false;

			SetAllowDamage( true );

			GetInputController().OverrideRaise( false, false );
			GetInputController().OverrideMeleeEvade( false, false );

			GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_SAFEZONE_TITLE" ), new StringLocaliser( "STR_EXPANSION_SAFEZONE_LEFT" ), EXPANSION_NOTIFICATION_ICON_INFO, COLOR_EXPANSION_NOTIFICATION_ERROR, 7, GetIdentity() );
		
			SetSynchDirty();
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::OnLeftSafeZone - End");
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
		
	// ------------------------------------------------------------
	// PlayerBase IsInTerritory
	// Check if player is in a territory
	// ------------------------------------------------------------
	bool IsInTerritory(float territorySize = -1)
	{
		if ( !m_TerritoryModule )
			return false;
			
		return m_TerritoryModule.IsInTerritory( GetPosition(), territorySize );
	}
	
	// ------------------------------------------------------------
	// PlayerBase IsInsideOwnTerritory
	// Check if player is in own territory
	// ------------------------------------------------------------
	bool IsInsideOwnTerritory(float territorySize = -1)
	{
		if ( !m_TerritoryModule )
			return false;

		if ( IsMissionHost() )
		{
			return m_TerritoryModule.IsInsideOwnTerritory( GetPosition(), territorySize, m_PlayerUID );
		}

		return m_TerritoryModule.IsInsideOwnTerritory( GetPosition(), territorySize );
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
	ExpansionFlagBase FindNearestFlag()
	{
		if ( !IsMissionClient() )
			return NULL;
		
		vector pos = GetPosition();
		array<Object> objects = new array<Object>;
		array<CargoBase> proxyCargos = new array<CargoBase> ;
		GetGame().GetObjectsAtPosition3D( pos, 10, objects, proxyCargos );
		
		if ( objects && objects.Count() > 0 )
		{
			for ( int i = 0; i < objects.Count(); ++i )
			{
				Object obj = Object.Cast( objects.Get( i ) );
				if ( obj.IsKindOf( "ExpansionFlagBase" ) )
				{
					ExpansionFlagBase flag = ExpansionFlagBase.Cast( obj );

					return flag;
				}
			}
		}

		return NULL;
	}

	// ------------------------------------------------------------
	// Expansion SpawnGraveCross
	// ------------------------------------------------------------
	void CreateGraveCross()
	{
		vector crossOffset = GetPosition() - "0.3 -0.3 0.3";
			
		int lifetimeThreshhold = GetExpansionSettings().GetGeneral().GravecrossTimeThreshold;
		bool deleteBody = GetExpansionSettings().GetGeneral().GravecrossDeleteBody;
		float playtime = StatGet("playtime");

		Expansion_GraveBase graveCross;
		if (playtime <= lifetimeThreshhold) 
		{
			graveCross = Expansion_GraveBase.Cast(GetGame().CreateObjectEx("Expansion_Gravecross_LowLifetime", crossOffset, ECE_PLACE_ON_SURFACE));
		}
		else 
		{
			graveCross = Expansion_GraveBase.Cast(GetGame().CreateObjectEx("Expansion_Gravecross", crossOffset, ECE_PLACE_ON_SURFACE));
		}

		for (int i = 0; i < GetInventory().GetAttachmentSlotsCount(); i++)
		{
			int slot = GetInventory().GetAttachmentSlotId(i);
			EntityAI item = GetInventory().FindAttachment(slot);

			if (item && graveCross.GetInventory().CanAddAttachment(item))
			{
				if (GetGame().IsMultiplayer())
					ServerTakeEntityToTargetInventory(graveCross, FindInventoryLocationType.ATTACHMENT, item);
				else
					LocalTakeEntityToTargetInventory(graveCross, FindInventoryLocationType.ATTACHMENT, item);
			}
		}

		graveCross.SetOrientation(GetOrientation());
		graveCross.SetReceivedAttachments(true);
		
		if (deleteBody)
			Delete();
	}
	
	// ------------------------------------------------------------
	// Expansion EOnContact
	// ------------------------------------------------------------
	override private void EOnContact( IEntity other, Contact extra )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::EOnContact - Start");
		#endif

		Transport transport;
		if ( Class.CastTo( transport, other ) )
		{
			ExpansionRegisterTransportHit( transport );
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::EOnContact - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion ExpansionRegisterTransportHit
	// ------------------------------------------------------------
	override void RegisterTransportHit( Transport transport )
	{
		// Preventing vanilla (and other mods) code from running
	}
	
	// ------------------------------------------------------------
	// PlayerBase ExpansionRegisterTransportHit
	// ------------------------------------------------------------
	void ExpansionRegisterTransportHit( Transport transport )
	{
		bool hasParent = false;

		if ( GetParent() || GetCommand_Vehicle() )
			hasParent = true;

		if ( m_ExPlayerLinkType != ExpansionPlayerLink.NONE )
			hasParent = true;

		if ( m_TransportHitRegistered )
			return;

		m_TransportHitRegistered = hasParent;

		if ( !m_TransportHitRegistered )
		{
			m_TransportHitRegistered = true;
			m_TransportHitVelocity = GetVelocity( transport );

			if ( m_TransportHitVelocity.Length() > 2.5 )
			{
				float damage = m_TransportHitVelocity.Length();
				ProcessDirectDamage( DT_CUSTOM, transport, "", "TransportHit", "0 0 0", damage );
			} else
			{
				m_TransportHitRegistered = false;
			}

			if ( m_TransportHitVelocity.Length() > 2.5 )
			{
				// dBodyApplyImpulse( this, dBodyGetMass( this ) * m_TransportHitVelocity * 40.0 );
			}
		}
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
		direction = VectorHelper.Multiply( direction, trans.GetBasis() );

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

		//if ( !s_ExpansionPlayerAttachment )
		{
			return super.ModCommandHandlerInside( pDt, pCurrentCommandID, pCurrentCommandFinished );
		}

		IEntity parent = GetParent();
		HumanCommandMove hcm = GetCommand_Move();
		HumanCommandVehicle hcv = GetCommand_Vehicle();
		HumanCommandLadder hcl = GetCommand_Ladder();
		HumanCommandSwim hcs = GetCommand_Swim();
		HumanCommandClimb hcc = GetCommand_Climb();
		HumanCommandScript hcScript = GetCommand_Script();

		bool isPlayerAttached = false;
		
		if ( super.ModCommandHandlerInside( pDt, pCurrentCommandID, pCurrentCommandFinished ) )
		{
			return true;
		}

		if ( pCurrentCommandID == DayZPlayerConstants.COMMANDID_VEHICLE )
		{
			if ( hcv.WasGearChange() )
			{
				GearChangeActionCallback cb = GearChangeActionCallback.Cast( AddCommandModifier_Action( DayZPlayerConstants.CMD_ACTIONMOD_SHIFTGEAR, GearChangeActionCallback ) );
				cb.SetVehicleCommand( hcv );
			}

			return true;
		}
		
		if ( pCurrentCommandID == DayZPlayerConstants.COMMANDID_SCRIPT )
		{
			#ifdef EXPANSIONEXPRINT
			EXPrint("PlayerBase::ModCommandHandlerInside - Cast ExpansionHumanCommandVehicle");
			#endif

			if ( ExpansionHumanCommandVehicle.Cast( hcScript ) != NULL )
			{
				#ifdef EXPANSIONEXPRINT
				EXPrint("PlayerBase::ModCommandHandlerInside - End");
				#endif

				return true;
			}
		}

		m_Swimming.ResetCall();
		if ( m_Swimming.HandleSwimming( pCurrentCommandID, hcm, m_MovementState ) )
		{
			m_JumpClimb.CheckAndFinishJump();
			return true;
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::ModCommandHandlerInside - Handled Inside");
		#endif

		if ( pCurrentCommandID == DayZPlayerConstants.COMMANDID_SCRIPT )
		{
			ExpansionHumanCommandFall exFall;
			if ( Class.CastTo( exFall, hcScript ) )
			{
				DayZPlayerType type = GetDayZPlayerType();
				NoiseParams npar;
				int landType;

				if ( exFall.HasLanded() )
				{
					#ifdef EXPANSIONEXPRINT
					EXPrint("PlayerBase::ModCommandHandlerInside - HasLanded");
					#endif

					if ( parent )
					{
						landType = 0; 
						npar = type.GetNoiseParamsLandLight();
						m_FallYDiff = 0;
					} else
					{
						m_FallYDiff = m_FallYDiff - GetPosition()[1];
						if ( m_FallYDiff < 0.5 )
						{
							landType = 0; 
							npar = type.GetNoiseParamsLandLight();
						} else if ( m_FallYDiff < 1.0 )
						{
							landType = 0;
							npar = type.GetNoiseParamsLandLight();
						} else if ( m_FallYDiff < 2.0 )
						{
							landType = 1;
							npar = type.GetNoiseParamsLandHeavy();
						} else
						{
							landType = 2;
							npar = type.GetNoiseParamsLandHeavy();
						}
					}
					
					exFall.Land( landType );

					#ifdef EXPANSIONEXPRINT
					EXPrint("PlayerBase::ModCommandHandlerInside - Land Called");
					#endif

					AddNoise( npar );

					#ifdef EXPANSIONEXPRINT
					EXPrint("PlayerBase::ModCommandHandlerInside - Noise Added");
					#endif

					if ( m_FallYDiff >= DayZPlayerImplementFallDamage.FD_DMG_FROM_HEIGHT && GetInstanceType() == DayZPlayerInstanceType.INSTANCETYPE_CLIENT )
					{
						SpawnDamageDealtEffect();
					}
					
					#ifdef EXPANSIONEXPRINT
					EXPrint("PlayerBase::ModCommandHandlerInside - Effect");
					#endif

					m_FallDamage.HandleFallDamage( m_FallYDiff );
					m_JumpClimb.CheckAndFinishJump( landType );

					#ifdef EXPANSIONEXPRINT
					EXPrint("PlayerBase::ModCommandHandlerInside - Finished JumpClimb");
					#endif
				}

				#ifdef EXPANSIONEXPRINT
				EXPrint("PlayerBase::ModCommandHandlerInside - End");
				#endif

				return true;
			}
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::ModCommandHandlerInside - pCurrentCommandFinished End");
		#endif

		if ( PhysicsIsFalling( false ) )
		{
			#ifdef EXPANSIONEXPRINT
			EXPrint("PlayerBase::ModCommandHandlerInside - Called Fall");
			#endif

			StartCommand_ExpansionFall( 0 );
			m_FallYDiff = GetPosition()[1];

			#ifdef EXPANSIONEXPRINT
			EXPrint("PlayerBase::ModCommandHandlerInside - End");
			#endif

			return true;
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::ModCommandHandlerInside - End");
		#endif

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
		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::HasItem - Start");
		#endif

		if ( this.GetInventory() )
		{
			for ( int att_i = 0; att_i < this.GetInventory().AttachmentCount(); ++att_i )
			{
				EntityAI attachment = GetInventory().GetAttachmentFromIndex( att_i );
				ref CargoBase cargo = attachment.GetInventory().GetCargo();
				
				if ( cargo )
				{
					for ( int cgo_i = 0; cgo_i < cargo.GetItemCount(); ++cgo_i )
					{
						EntityAI cargo_item = cargo.GetItem( cgo_i );
						if ( cargo_item )
						{
							if ( cargo_item.GetType() == name )
							{
								#ifdef EXPANSIONEXPRINT
								EXPrint("PlayerBase::HasItem - End");
								#endif

								return Class.CastTo( item, cargo_item );
							}
						}
					}
				}
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
	// PlayerBase IsMarketItemReserved
	// ------------------------------------------------------------
	bool IsMarketItemReserved( string className )
	{
		return m_MarketReserve != NULL && m_MarketReserve.RootItem.ClassName == className && ( m_MarketReserve.Time >= GetGame().GetTime() - 30 );
	}

	// ------------------------------------------------------------
	// PlayerBase GetMarketReserve
	// ------------------------------------------------------------
	ref ExpansionMarketReserve GetMarketReserve()
	{
		return m_MarketReserve;
	}

	// ------------------------------------------------------------
	// PlayerBase GetMarketSell
	// ------------------------------------------------------------
	ref ExpansionMarketSell GetMarketSell()
	{
		return m_MarketSell;
	}

	// ------------------------------------------------------------
	// PlayerBase ClearMarketReserve
	// ------------------------------------------------------------
	void ClearMarketReserve()
	{
		m_MarketReserve.Valid = false;
		m_MarketReserve.Reserved.Clear();
	}

	// ------------------------------------------------------------
	// PlayerBase ClearMarketSell
	// ------------------------------------------------------------
	void ClearMarketSell()
	{
		m_MarketSell.Valid = false;
		m_MarketSell.Sell.Clear();
	}

	// ------------------------------------------------------------
	// PlayerBase SetMoney
	// ------------------------------------------------------------
	void SetMoney( array< ExpansionMoneyBase > monies )
	{
		m_Money = monies;
	}

	// ------------------------------------------------------------
	// PlayerBase GetMoney
	// ------------------------------------------------------------
	array< ExpansionMoneyBase > GetMoney()
	{
		return m_Money;
	}

	// ------------------------------------------------------------
	// Expansion StartCommand_ExpansionVehicle
	// ------------------------------------------------------------
	override ExpansionHumanCommandVehicle StartCommand_ExpansionVehicle( Object vehicle, int seatIdx, int seat_anim )
	{
		if ( m_ExpansionVehicleCommandST == NULL )
		{
			m_ExpansionVehicleCommandST = new ExpansionHumanCommandVehicle_ST( this );
		}
	
		ExpansionHumanCommandVehicle cmd = new ExpansionHumanCommandVehicle( this, vehicle, seatIdx, seat_anim, m_ExpansionVehicleCommandST );
		StartCommand_Script( cmd );
		return cmd;
	}

	// ------------------------------------------------------------
	// Expansion StartCommand_ExpansionFall
	// ------------------------------------------------------------
	override void StartCommand_ExpansionFall( float pYVelocity )
	{
		#ifndef EXPANSION_DISABLE_FALL
		if ( s_ExpansionPlayerAttachment )
		{
			if ( m_ExpansionFallCommandST == NULL )
			{
				m_ExpansionFallCommandST = new ExpansionHumanCommandFall_ST( this );
			}
		
			StartCommand_Script( new ExpansionHumanCommandFall( this, pYVelocity, m_ExpansionFallCommandST ) );
			return;
		}
		#endif
		
		StartCommand_Fall( pYVelocity );
	}

	// ------------------------------------------------------------
	// PlayerBase GetInVehicle
	// ------------------------------------------------------------
	bool GetInVehicle( )
	{
		return m_WasInVehicle;
	}

	// ------------------------------------------------------------
	// PlayerBase SetInVehicle
	// ------------------------------------------------------------
	void SetInVehicle( bool state )
	{
		m_WasInVehicle = state;
	}

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
	// PlayerBase OnCommandExpansionVehicleFinish
	// ------------------------------------------------------------
	override void OnCommandExpansionVehicleFinish()
	{
		super.OnCommandExpansionVehicleFinish();

		if ( GetInventory() )
			GetInventory().UnlockInventory(LOCK_FROM_SCRIPT);
			
		if ( GetItemInHands() )
		{
			GetItemAccessor().HideItemInHands(false);
		}
		
		if ( m_IsVehicleSeatDriver )
		{
			OnVehicleSeatDriverLeft();
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
			pModel.m_fHeadingAngle = 0;
			return true;
		}

		return super.HeadingModel( pDt, pModel );
	}

	// ------------------------------------------------------------
	// Expansion GetExpansionSaveVersion
	// ------------------------------------------------------------
	int GetExpansionSaveVersion()
	{
		return m_ExpansionSaveVersion;
	}

	// ------------------------------------------------------------
	// Expansion OnStoreSave
	// ------------------------------------------------------------
	override void OnStoreSave( ParamsWriteContext ctx )
	{
		m_ExpansionSaveVersion = EXPANSION_VERSION_CURRENT_SAVE;
		ctx.Write( m_ExpansionSaveVersion );

		super.OnStoreSave( ctx );
		
		ctx.Write( m_WasInVehicle );
	}
	
	// ------------------------------------------------------------
	// Expansion OnStoreLoad
	// ------------------------------------------------------------
	override bool OnStoreLoad( ParamsReadContext ctx, int version )
	{
		//! Use GetExpansionSaveVersion()
		//! Making sure this is read before everything else.

		if ( Expansion_Assert_False( ctx.Read( m_ExpansionSaveVersion ), "[" + this + "] Failed reading m_ExpansionSaveVersion" ) )
			return false;

		if ( !super.OnStoreLoad( ctx, version ) )
			return false;
		
		if ( Expansion_Assert_False( ctx.Read( m_WasInVehicle ), "[" + this + "] Failed reading m_WasInVehicle" ) )
			return false;
	
		return true;
	}
	
	// ------------------------------------------------------------
	// Expansion AfterStoreLoad
	// ------------------------------------------------------------
	override void AfterStoreLoad()
	{
		super.AfterStoreLoad();
		
		if ( GetGame().IsServer() )
		{
			if ( m_WasInVehicle )
			{
				vector rayStart = GetPosition();
				vector rayEnd = rayStart + "0 -6000 0";
				vector ground;
				
				RaycastRVParams params = new RaycastRVParams( rayStart, rayEnd, this, 0.2 );
				params.sorted = true;
				//params.type = ObjIntersectFire;
				params.flags = CollisionFlags.ALLOBJECTS;
				
				array< ref RaycastRVResult > results = new array< ref RaycastRVResult >();
				if ( DayZPhysics.RaycastRVProxy( params, results ) )
				{
					vector hitPos = vector.Zero;
					for (int i = 0; i < results.Count(); ++i)
					{
						ref RaycastRVResult currResult = results[i];
						if (!currResult)
							continue;
						
						Object obj;
						if (currResult.hierLevel > 0)
						{
							obj = currResult.parent;
						}	
						else
						{
							obj = currResult.obj;
						}
						
						if ( !obj || obj.IsTree() || obj.IsBush() || obj.IsScriptedLight() || obj.IsTransport() || obj.GetType() == string.Empty)
							continue;
						
						hitPos = currResult.pos;
						break;
					}
					
					if (hitPos != vector.Zero)
					{
						SetPosition( hitPos );
					}
					else
					{
						ground = Vector( GetPosition()[0], GetGame().SurfaceY( GetPosition()[0], GetPosition()[2] ), GetPosition()[2] );
						SetPosition( ground );
					}
				}
				else
				{
					ground = Vector( GetPosition()[0], GetGame().SurfaceY( GetPosition()[0], GetPosition()[2] ), GetPosition()[2] );
					SetPosition( ground );
				}

				m_WasInVehicle = false;
			}
		}
	}

	// ------------------------------------------------------------
	string GetIdentityUID()
	{
		if ( IsMissionClient() )
		{
			if ( GetIdentity() )
				return GetIdentity().GetId();
			else if ( IsMissionOffline() )
				return "OFFLINE";
		}

		return m_PlayerUID;
	}

	// ------------------------------------------------------------
	string GetIdentitySteam()
	{
		if ( IsMissionClient() )
		{
			if ( GetIdentity() )
				return GetIdentity().GetPlainId();
			else if ( IsMissionOffline() )
				return "OFFLINE";
		}

		return m_PlayerSteam;
	}

	// ------------------------------------------------------------
	string GetIdentityName()
	{
		if ( IsMissionClient() )
		{
			if ( GetIdentity() )
				return GetIdentity().GetName();
			else if ( IsMissionOffline() )
				return "OFFLINE";
		}

		return m_PlayerName;
	}

	// ------------------------------------------------------------
	// PlayerBase GetAll
	// ------------------------------------------------------------
	static array< PlayerBase > GetAll()
	{
		return m_AllPlayers;
	}
	
	// ------------------------------------------------------------
	// PlayerBase GetPlayerByUID
	// string id = PlayerIdentity::GetId()
	// Can be used on server/client side
	// ------------------------------------------------------------
	static PlayerBase GetPlayerByUID( string id )
	{
		if ( IsMissionHost() )
		{
			return m_AllPlayersUID.Get( id );
		} else
		{
			for ( int j = 0; j < ClientData.m_PlayerBaseList.Count(); ++j )
			{
				PlayerBase player;
				if ( !Class.CastTo( player, ClientData.m_PlayerBaseList[j] ) || !player.GetIdentity() )
					continue;
				
				if ( player.GetIdentity().GetId() == id )
					return PlayerBase.Cast( player );
			}
		}
		
		return NULL;
	}
	
	// ------------------------------------------------------------
	// PlayerBase AddPlayer
	// Only called server side, to get only alive players
	// ------------------------------------------------------------
	static void AddPlayer( PlayerBase player )
	{
		if ( !player )
			return;

		if ( player.GetIdentity() )
		{
			player.m_PlayerUID = player.GetIdentity().GetId();
			player.m_PlayerSteam = player.GetIdentity().GetPlainId();
			player.m_PlayerName = player.GetIdentity().GetName();
		}

		if ( player.m_PlayerUID != "" )
			m_AllPlayersUID.Set( player.m_PlayerUID, player );
	}
	
	// ------------------------------------------------------------
	// PlayerBase RemovePlayer
	// Only called server side, to get only alive players
	// ------------------------------------------------------------
	static void RemovePlayer( string id )
	{
		m_AllPlayersUID.Remove( id );
	}
	
	// ------------------------------------------------------------
	// PlayerBase SetHasItemMap
	// ------------------------------------------------------------
	void SetHasItemMap(bool state)
	{
		m_HasMap = state;
	}
	
	// ------------------------------------------------------------
	// PlayerBase HasItemMap
	// ------------------------------------------------------------
	bool HasItemMap()
	{
		return m_HasMap;
	}
	
	// ------------------------------------------------------------
	// PlayerBase SetHasItemGPS
	// ------------------------------------------------------------
	void SetHasItemGPS(bool state)
	{
		m_HasGPS = state;
	}
	
	// ------------------------------------------------------------
	// PlayerBase HasItemGPS
	// ------------------------------------------------------------
	bool HasItemGPS()
	{
		return m_HasGPS;
	}
	void WakePlayer()
	{
		this.AddHealth("","Shock", 100);
	}
}
