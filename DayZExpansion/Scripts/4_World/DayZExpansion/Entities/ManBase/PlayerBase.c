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
enum ExpansionPlayerBaseRPC
{
	INVALID = 20500,
	SyncQuests,
	COUNT
}

modded class PlayerBase
{
	private int m_ExpansionSaveVersion;

	Object m_PlayerHeadingDir;

	protected ref Timer m_SafezoneUpdateTimer;
	protected ref Timer m_RadiationZoneUpdateTimer;
	protected ref Timer m_HumanityUpdateTimer;
	protected ref Timer m_HumanityTimer;

	protected bool m_SafeZone;
	protected bool m_SafeZoneSynchRemote;

	protected bool m_IsHumanityHero;
	protected bool m_IsHumanityBandit;
	protected bool m_PlayerHasHeroSet;
	protected bool m_PlayerHasBanditSet;

	protected bool m_WasInVehicle;

	protected int m_CurrentHumanity;
	protected int m_TerritoryIdInside;

	protected ExpansionTerritoryModule m_TerritoryModule;
	protected ExpansionKillFeedModule m_KillfeedModule;
	protected ExpansionHumanityModule m_HumanityModule;

	protected autoptr array< ExpansionMoneyBase > m_Money;

	protected ref ExpansionMarketReserve m_MarketReserve;
	protected ref ExpansionMarketSell m_MarketSell;

	protected ref ExpansionHumanVehicleCommand_ST m_ExpansionVehicleCommandST;
	protected ref ExpansionHumanFallCommand_ST m_ExpansionFallCommandST;

	protected ref ExpansionKillFeedData m_KillFeedData;
	//! protected ref array<ref ExpansionQuest> m_Quests;
	
	protected string m_PlayerUID;
	protected string m_PlayerSteam;
	protected string m_PlayerName;

	private static ref map< string, PlayerBase > m_AllPlayersUID = new map< string, PlayerBase >;
	private static ref array< PlayerBase > m_AllPlayers = new array< PlayerBase >;

	protected bool m_HasMap;
	protected bool m_HasGPS;
	
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
			GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).Call( DeferredClientInit );
		}

		m_SafeZone = false;
		
		m_TerritoryIdInside = -1;

		Class.CastTo( m_TerritoryModule, GetModuleManager().GetModule( ExpansionTerritoryModule ) );
		Class.CastTo( m_KillfeedModule, GetModuleManager().GetModule( ExpansionHumanityModule ) );
		Class.CastTo( m_HumanityModule, GetModuleManager().GetModule( ExpansionKillFeedModule ) );

		//! m_KillFeedData = null;

		m_MarketReserve = new ExpansionMarketReserve; 
		m_MarketSell = new ExpansionMarketSell;
		
		m_Money = new array< ExpansionMoneyBase >;
		//! m_Quests = new array<ref ExpansionQuest >;

		m_HasMap = false;
		m_HasGPS = false;
		
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

		
		if ( m_HumanityTimer )
		{
			m_HumanityTimer.Stop();
			delete m_HumanityTimer;
		}

		if ( m_SafezoneUpdateTimer )
		{
			m_SafezoneUpdateTimer.Stop();
			delete m_SafezoneUpdateTimer;
		}
		
		if ( m_RadiationZoneUpdateTimer )
		{
			m_RadiationZoneUpdateTimer.Stop();
			delete m_RadiationZoneUpdateTimer;
		}

		if ( m_HumanityUpdateTimer )
		{
			m_HumanityUpdateTimer.Stop();
			delete m_HumanityUpdateTimer;
		}

		if ( m_KillFeedData )
			delete m_KillFeedData;

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

		if ( GetGame() && IsMissionClient() && !IsMissionHost() && GetModuleManager() )
		{
			Expansion3DMarkerModule module;
			if ( Class.CastTo( module, GetModuleManager().GetModule( Expansion3DMarkerModule ) ) )
			{
				module.RefreshMarkers();
			}
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

		m_AllPlayers.Insert( this );
		
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

		RemoveAction( ActionGetOutTransport );	

		AddAction( ExpansionActionGetOutTransport );
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

		// AddAction( ExpansionActionConnectWinch );
		// AddAction( ExpansionActionDisconnectWinch );
		
		// AddAction( ExpansionActionStartPlane );
		// AddAction( ExpansionActionStopPlane );

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

		/*
		m_RadiationZoneUpdateTimer = new Timer();
		m_RadiationZoneUpdateTimer.Run(1, this, "RadiationZoneUpdate", null, true);
		
		m_HumanityTimer = new Timer();
		m_HumanityTimer.Run(3600, this, "AddHumanity", new Param1<int>( 10 ), true);
		m_HumanityUpdateTimer = new Timer();
		m_HumanityUpdateTimer.Run(5, this, "HumanityUpdate", null, true);

		m_IsHumanityBandit = false;
		m_IsHumanityHero = false;
		m_CurrentHumanity = 0;
		m_PlayerHasHeroSet = false;
		m_PlayerHasBanditSet = false;
		
		RegisterNetSyncVariableBool( "m_IsHumanityBandit" );
		RegisterNetSyncVariableBool( "m_IsHumanityHero" );	
		RegisterNetSyncVariableInt( "m_CurrentHumanity" );
		*/

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
	// PlayerBase AddHumanity
	// ------------------------------------------------------------
	void AddHumanity(int value)
	{
		if ( !GetGame().IsServer() )
		{
			Error( "[EXPANSION] AddHumanity can only be called on the server!" );
			return;
		}

		m_CurrentHumanity += value;
	}

	// ------------------------------------------------------------
	// PlayerBase DecreaseHumanity
	// ------------------------------------------------------------
	void DecreaseHumanity(int value)
	{
		if ( !GetGame().IsServer() )
		{
			Error( "[EXPANSION] DecreaseHumanity can only be called on the server!" );
			return;
		}

		m_CurrentHumanity -= value;
	}
	
	// ------------------------------------------------------------
	// PlayerBase SetHumanity
	// ------------------------------------------------------------
	void SetHumanity(int value)
	{
		if ( !GetGame().IsServer() )
		{
			Error( "[EXPANSION] SetHumanity can only be called on the server!" );
			return;
		}

		m_CurrentHumanity = value;
	}

	// ------------------------------------------------------------
	// PlayerBase GetKarma
	// ------------------------------------------------------------
	int GetKarma()
	{
		return m_CurrentHumanity;
	}

	// ------------------------------------------------------------
	// PlayerBase IsBandit
	// ------------------------------------------------------------
	bool IsBandit()
	{
		if (m_CurrentHumanity < -1000)
			return true;

		return false;
	}

	// ------------------------------------------------------------
	// PlayerBase IsHero
	// ------------------------------------------------------------
	bool IsHero()
	{
		if (m_CurrentHumanity > 2000)
			return true;

		return false;
	}
	
	// ------------------------------------------------------------
	// PlayerBase HasHeroSet
	// ------------------------------------------------------------
	bool HasHeroSet()
	{
		return m_PlayerHasHeroSet;
	}
	
	// ------------------------------------------------------------
	// PlayerBase HasBanditSet
	// ------------------------------------------------------------
	bool HasBanditSet()
	{
		return m_PlayerHasBanditSet;
	}

	// ------------------------------------------------------------
	// PlayerBase KarmaUpdate
	// ------------------------------------------------------------
	void KarmaUpdate()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::KarmaUpdate start");
		#endif

		if ( IsMissionHost() )
		{
			if ( IsBandit() && !m_IsHumanityBandit ) 
			{
				if ( !HasBanditSet() )
				{
					ItemBase banditSet;
	
					if (banditSet)
						GetGame().ObjectDelete(banditSet);
	
					banditSet = ItemBase.Cast( GetInventory().CreateInInventory( "ExpansionBanditSet" ) );
					if ( !banditSet )
						banditSet = ItemBase.Cast( GetGame().CreateObject( "ExpansionBanditSet", GetPosition() ) );
				}
				
				GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_HUMANITY_TITLE" ), new StringLocaliser( "STR_EXPANSION_HUMANITY_BECOME_BANDIT" ), EXPANSION_NOTIFICATION_ICON_BANDIT, COLOR_EXPANSION_NOTIFICATION_AMETHYST, 5, GetIdentity() );
				m_IsHumanityBandit = true;
				m_IsHumanityHero = false;
				m_PlayerHasBanditSet = true;
			}

			if ( IsHero() && !m_IsHumanityHero ) 
			{
				if ( !HasHeroSet() )
				{
					ItemBase heroSet;
	
					if (heroSet)
						GetGame().ObjectDelete(heroSet);
	
					heroSet = ItemBase.Cast( GetInventory().CreateInInventory( "ExpansionHeroSet" ) );
					if ( !heroSet )
						heroSet = ItemBase.Cast( GetGame().CreateObject( "ExpansionHeroSet", GetPosition() ) );
				}

				GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_HUMANITY_TITLE" ), new StringLocaliser( "STR_EXPANSION_HUMANITY_BECOME_HERO" ), EXPANSION_NOTIFICATION_ICON_HERO, COLOR_EXPANSION_NOTIFICATION_AMETHYST, 5, GetIdentity() );
				m_IsHumanityHero = true;
				m_IsHumanityBandit = false;
				m_PlayerHasHeroSet = true;
			}
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::KarmaUpdate end");
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
	bool IsInTerritory()
	{
		if ( !m_TerritoryModule )
			return false;
			
		return m_TerritoryModule.IsInTerritory( GetPosition() );
	}
	
	// ------------------------------------------------------------
	// PlayerBase IsInsideOwnTerritory
	// Check if player is in own territory
	// ------------------------------------------------------------
	bool IsInsideOwnTerritory()
	{
		if ( !m_TerritoryModule )
			return false;

		if ( IsMissionHost() )
		{
			return m_TerritoryModule.IsInsideOwnTerritory( GetPosition(), -1, m_PlayerUID );
		}

		return m_TerritoryModule.IsInsideOwnTerritory( GetPosition() );
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
			EXPrint("PlayerBase::ModCommandHandlerInside - Cast ExpansionHumanVehicleCommand");
			#endif

			if ( ExpansionHumanVehicleCommand.Cast( hcScript ) != NULL )
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
			#ifdef EXPANSIONEXPRINT
			EXPrint("PlayerBase::ModCommandHandlerInside - Cast ExpansionHumanFallCommand");
			#endif

			ExpansionHumanFallCommand fall;

			if ( Class.CastTo( fall, hcScript ) )
			{
				int landType;
				if ( fall.HasLanded() )
				{
					#ifdef EXPANSIONEXPRINT
					EXPrint("PlayerBase::ModCommandHandlerInside - HasLanded");
					#endif

					DayZPlayerType type = GetDayZPlayerType();
					NoiseParams npar;

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
					
					fall.Land( landType );

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

			#ifdef EXPANSIONEXPRINT
			EXPrint("PlayerBase::ModCommandHandlerInside - NoCustomScript");
			#endif
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

	/*
	// ------------------------------------------------------------
	// Expansion CanReleaseAttachment 
	// ------------------------------------------------------------
	override bool CanReleaseAttachment ( EntityAI attachment )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint( "PlayerBase::CanReleaseAttachment start" );
		#endif

		if ( IsBandit() )
		{
			if ( FindAttachmentBySlotName( "Armband" ) )
			{
				return false;
			} 
		}

		#ifdef EXPANSIONEXPRINT
		EXPrint( "PlayerBase::CanReleaseAttachment end" );
		#endif

		return super.CanReleaseAttachment( attachment );
	}
	*/
	
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
			GetGame().GetMission().OnEvent( ChatMessageEventTypeID, new ChatMessageEventParams( CCDirect, "", message, "" ) );
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
	override ExpansionHumanVehicleCommand StartCommand_ExpansionVehicle( Object vehicle, int seatIdx, int seat_anim )
	{
		if ( m_ExpansionVehicleCommandST == NULL )
		{
			m_ExpansionVehicleCommandST = new ExpansionHumanVehicleCommand_ST( this );
		}
	
		ExpansionHumanVehicleCommand cmd = new ExpansionHumanVehicleCommand( this, vehicle, seatIdx, seat_anim, m_ExpansionVehicleCommandST );
		StartCommand_Script( cmd );
		return cmd;
	}

	// ------------------------------------------------------------
	// Expansion StartCommand_ExpansionFall
	// ------------------------------------------------------------
	override void StartCommand_ExpansionFall( float pYVelocity )
	{
		if ( m_ExpansionFallCommandST == NULL )
		{
			m_ExpansionFallCommandST = new ExpansionHumanFallCommand_ST( this );
		}
	
		StartCommand_Script( new ExpansionHumanFallCommand( this, pYVelocity, m_ExpansionFallCommandST ) );
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
		
		ExpansionHumanVehicleCommand hcv = GetCommand_ExpansionVehicle();
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
	// PlayerBase ClearKillFeedData
	// ------------------------------------------------------------
	void ClearKillFeedData()
	{
		m_KillFeedData = null;
	}

	// ------------------------------------------------------------
	// Expansion EEHitBy
	// ------------------------------------------------------------
	/*
	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::EEHitBy - Start");
		#endif

		super.EEHitBy( damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef );		

		//! Kill-Feed
		if ( GetExpansionSettings().GetGeneral().EnableKillFeed )
		{
			if ( m_KillFeedData )
				delete m_KillFeedData;
			
			m_KillFeedData = new ExpansionKillFeedData( damageType, source, ammo );
			
			GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( this.ClearKillFeedData, 3000, false );
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::EEHitBy - End");
		#endif
	}
	*/

	// ------------------------------------------------------------
	// Expansion EEKilled
	// ------------------------------------------------------------
	/*
	override void EEKilled( Object killer )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::EEKilled - Start");
		#endif

		if ( GetExpansionSettings().GetGeneral().EnableKillFeed && m_KillfeedModule )
		{
			m_KillfeedModule.OnPlayerKilledEventNew( this, killer );
		}

		if ( GetExpansionSettings().GetGeneral().EnableHumanity && m_HumanityModule )
		{
			m_HumanityModule.OnPlayerKilledEvent( this, killer );
		}

		super.EEKilled( killer );

		#ifdef EXPANSIONEXPRINT
		EXPrint("PlayerBase::EEKilled - End");
		#endif
	}
	*/
	
	// ------------------------------------------------------------
	// PlayerBase GetCurrentHitData
	// ------------------------------------------------------------
	ExpansionKillFeedData GetCurrentHitData()
	{
		return m_KillFeedData;
	}
	
	// ------------------------------------------------------------
	// PlayerBase AddQuest
	// ------------------------------------------------------------
	/*
	int AddQuest(ExpansionQuest quest, out ExpansionQuest playerquest)
	{
		return 0;

		playerquest = new ExpansionQuest(quest.QuestID, quest.QuestName, quest.QuestItemTitle, quest.QuestItemText, quest.QuestText, quest.QuestType, quest.QuestPosition, quest.QuestItemClassName, quest.QuestItemPosition, quest.QuestItemDirection );
		
		int index = m_Quests.Insert( playerquest );
		
		playerquest.SetQuestIndex(index);
		
		SyncQuests();
		
		return index;
	}
	*/
	
	// ------------------------------------------------------------
	// PlayerBase RemoveQuest
	// ------------------------------------------------------------
	void RemoveQuest(int index)
	{
	/*
		m_Quests.Remove( index );
	*/
	}
	
	// ------------------------------------------------------------
	// PlayerBase ClearQuests
	// ------------------------------------------------------------
	void ClearQuests()
	{
	/*
		m_Quests.Clear();
	*/
	}
	
	// ------------------------------------------------------------
	// PlayerBase GetQuests
	// ------------------------------------------------------------		
	/*
	array<ref ExpansionQuest> GetQuests()
	{
		return NULL;
		// return m_Quests;
	}
	*/
	
	// ------------------------------------------------------------
	// PlayerBase SetQuestState
	// ------------------------------------------------------------
	/*
	void SetQuestState(int index, ExpansionQuestStates state)
	{
		ExpansionQuest quest = m_Quests.Get( index );
		if ( quest )
		{
			quest.SetQuestState( state );
		}
		
		SyncQuests();
	}
	*/

	// ------------------------------------------------------------
	// Expansion OnRPC
	// ------------------------------------------------------------	
	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext ctx)
	{
		switch ( rpc_type )
		{
			case ExpansionPlayerBaseRPC.SyncQuests:
			{
				RPC_SyncQuests( ctx, sender );
				return;
			}
		}

		super.OnRPC(sender, rpc_type, ctx);
	}
	
	// ------------------------------------------------------------
	// PlayerBase SyncQuests
	// ------------------------------------------------------------
	void SyncQuests()
	{
	/*
		if ( IsMissionHost() )
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write( m_Quests );
			rpc.Send( this, ExpansionPlayerBaseRPC.SyncQuests, true, GetIdentity() );
		}
	*/
	}
	
	// ------------------------------------------------------------
	// PlayerBase RPC_SyncQuests
	// ------------------------------------------------------------
	private void RPC_SyncQuests(ref ParamsReadContext ctx, PlayerIdentity sender)
	{
	/*
		if ( IsMissionClient() )
		{
			if ( !ctx.Read( m_Quests ) )
				return;
		}
	*/
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
		ctx.Write( m_CurrentHumanity );

		// ctx.Write( m_Quests );
	}
	
	// ------------------------------------------------------------
	// Expansion OnStoreLoad
	// ------------------------------------------------------------
	override bool OnStoreLoad( ParamsReadContext ctx, int version )
	{
		//! Use GetExpansionSaveVersion()
		//! Making sure this is read before everything else.

		if ( !ctx.Read( m_ExpansionSaveVersion ) )
			return false;

		if ( !super.OnStoreLoad( ctx, version ) )
			return false;
		
		if ( !ctx.Read( m_WasInVehicle ) )
			return false;

		if ( GetExpansionSaveVersion() >= 2 )
		{
			if ( !ctx.Read( m_CurrentHumanity ) )
				return false;
		}

		/*
		if ( !ctx.Read( m_Quests ) )
		 	return false;
		else
			SyncQuests();
		*/
	
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
				vector ground = Vector( GetPosition()[0], GetGame().SurfaceY( GetPosition()[0], GetPosition()[2] ), GetPosition()[2] );
				SetPosition( ground );

				m_WasInVehicle = false;
			}
		}
	}

	// ------------------------------------------------------------
	string GetIdentityUID()
	{
		if ( IsMissionClient() )
			return GetIdentity().GetId();

		return m_PlayerUID;
	}

	// ------------------------------------------------------------
	string GetIdentitySteam()
	{
		if ( IsMissionClient() )
			return GetIdentity().GetPlainId();

		return m_PlayerSteam;
	}

	// ------------------------------------------------------------
	string GetIdentityName()
	{
		if ( IsMissionClient() )
			return GetIdentity().GetName();

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
}
