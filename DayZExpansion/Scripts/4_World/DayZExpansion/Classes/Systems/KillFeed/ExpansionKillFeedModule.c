/**
 * ExpansionKillFeedModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

enum ExpansionDamageSource
{
	NONE,
	EXPLOSION_WEAPON,
	EXPLOSION_VEHICLE,
	FALLING,
	PLAYER_WEAPON,
	PLAYER_MELEWEAPON,
	INFECTED,
	ANIMAL,
	AI_WEAPON,
	VEHICLE_HIT,
	VEHICLE_CRASH,
	HELICOPTER_HIT,
	WIRE,
	FIRE,
	FIST,
	COUNT,
}

class ExpansionKillFeedData
{
	int m_DmgType;
	EntityAI m_Source;
	string m_Ammo;
	
	void ExpansionKillFeedData(int damageType, EntityAI source, string ammo)
	{
		m_DmgType = damageType;
		m_Source = source;
		m_Ammo = ammo;
	}
}
	
class ExpansionKillFeedModule: JMModuleBase
{
	protected EntityAI m_HitSource;
	protected EntityAI m_HitSourceParent;
	protected EntityAI m_VehicleDriver;
	protected int m_LastDamageSource;
	
	protected CarScript m_Vehicle;
	
	protected string m_PlayerName;
	protected PlayerBase m_KillerPlayer;
	protected ExpansionAIBase m_KillerAI;
	protected EntityAI m_KillerEntity;
	protected string m_KillerName;
	protected ItemBase m_KillerWeapon;
	protected ref StringLocaliser m_Killfeed;
	protected ItemBase m_PlayerItemInHands;
	
	// ------------------------------------------------------------
	// ExpansionKillFeedModule Constructor
	// Gets called on server and client
	// ------------------------------------------------------------
	void ExpansionKillFeedModule()
    {
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionKillFeedModule::ExpansionKillFeedModule - Start");
		#endif
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionKillFeedModule::ExpansionKillFeedModule - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionKillFeedModule Destructor
	// Gets called on server and client
	// ------------------------------------------------------------
	void ~ExpansionKillFeedModule()
    {
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionKillFeedModule::~ExpansionKillFeedModule - Start");
		#endif
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionKillFeedModule::~ExpansionKillFeedModule - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionKillFeedModule IsEnabled
	// ------------------------------------------------------------
	override bool IsEnabled()
	{
		return m_Enabled && GetExpansionSettings().GetGeneral().EnableKillFeed;
	}
	
	// ------------------------------------------------------------
	// ExpansionKillFeedModule OnPlayerHitEvent
	// ------------------------------------------------------------
	void OnPlayerHitEvent(int damageType, EntityAI source, string ammo)
	{
		m_HitSource = NULL;
		m_HitSourceParent = NULL;
		m_LastDamageSource = ExpansionDamageSource.NONE;
		Man driver;
		Man player;
		
		CarScript car;
		HelicopterScript heli;
		
		switch ( damageType )
		{
			case DT_CLOSE_COMBAT:
			{
				#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionKillFeedModule::OnPlayerHitEvent - DT_CLOSE_COMBAT");
		#endif

				if ( source.IsZombie() )
				{
					#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionKillFeedModule::OnPlayerHitEvent - INFECTED");
		#endif

					m_LastDamageSource = ExpansionDamageSource.INFECTED;
					m_HitSource = source;
				}			
				else if ( source.IsAnimal() )
				{
					#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionKillFeedModule::OnPlayerHitEvent - ANIMAL");
		#endif

					m_LastDamageSource = ExpansionDamageSource.ANIMAL;
					m_HitSource = source;
				}
				else if ( source.IsPlayer() )
				{
					#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionKillFeedModule::OnPlayerHitEvent - FIST");
		#endif	

					m_LastDamageSource = ExpansionDamageSource.FIST;	
					m_HitSource = source;	
				}
				else if ( source.IsMeleeWeapon() )
				{
					#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionKillFeedModule::OnPlayerHitEvent - MELEWEAPON");
		#endif

					m_LastDamageSource = ExpansionDamageSource.PLAYER_MELEWEAPON;
					if ( source.GetHierarchyParent().IsInherited( SurvivorBase ) )
					{
						m_HitSource = source;
						if ( source.GetHierarchyParent() != NULL )
							m_HitSourceParent = EntityAI.Cast( source.GetHierarchyParent() );
					}
				}
				break;
			}
			case DT_FIRE_ARM:
			{
				#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionKillFeedModule::OnPlayerHitEvent - DT_FIRE_ARM");
		#endif

				if ( source.IsWeapon() )
				{
					m_LastDamageSource = ExpansionDamageSource.PLAYER_WEAPON;
					if ( source.GetHierarchyParent().IsInherited( SurvivorBase ) )
					{
						m_HitSource = source;
						if ( source.GetHierarchyParent() != NULL )
							m_HitSourceParent = EntityAI.Cast( source.GetHierarchyParent() );
					}
				}
				break;
			}
			case DT_EXPLOSION:
			{
				#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionKillFeedModule::OnPlayerHitEvent - DT_EXPLOSION");
		#endif

				//! Explosion source was a weapon (Grenade/Launcher/Explosive)
				if ( source.IsWeapon() || source.IsInherited( ItemBase ) || source.IsInherited( Ammunition_Base ) )
				{
					#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionKillFeedModule::OnPlayerHitEvent - Explosion came from weapon!");
		#endif

					m_LastDamageSource = ExpansionDamageSource.EXPLOSION_WEAPON;
					m_HitSource = source;
					if ( source.GetHierarchyParent() != NULL )
						m_HitSourceParent = EntityAI.Cast( source.GetHierarchyParent() );
				}
				//! Explosion source was a vehicle (Car/Heli)
				else if ( source.GetHierarchyParent().IsInherited( CarScript ) || source.GetHierarchyParent().IsInherited( ExpansionHelicopterScript ) )
				{
					#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionKillFeedModule::OnPlayerHitEvent - Explosion came from vehicle!");
		#endif

					m_LastDamageSource = ExpansionDamageSource.EXPLOSION_VEHICLE;
					m_HitSource = source.GetHierarchyParent();
				}
				break;
			}					
			case DT_CUSTOM:
			{
				#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionKillFeedModule::OnPlayerHitEvent - DT_CUSTOM");
		#endif
				if ( ammo == "FallDamage" )
				{
					m_LastDamageSource = ExpansionDamageSource.FALLING;

					#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionKillFeedModule::OnPlayerHitEvent - FALLING");
		#endif
				}
				//! Player got rolled over/hit by a vehicle
				else if ( source.GetType() == "AreaDamageBase" )  
				{
					// Check if hit came from a car
					if ( source.GetHierarchyParent().IsKindOf( "CarScript" ) )
					{
						#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionKillFeedModule::OnPlayerHitEvent - VEHICLE HIT");
		#endif

						m_LastDamageSource = ExpansionDamageSource.VEHICLE_HIT;
						m_HitSource = source;
						car = CarScript.Cast( source );
						if ( car.CrewSize() > 0 )
						{
							player = car.CrewMember( 0 );
							if ( player )
								m_HitSourceParent = player;
						}
					}
					// Check if hit came from a helicopter
					else if ( source.GetHierarchyParent().IsKindOf( "HelicopterScript" ) )
					{
						#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionKillFeedModule::OnPlayerHitEvent - HELICOPTER HIT");
		#endif

						m_LastDamageSource = ExpansionDamageSource.HELICOPTER_HIT;
						m_HitSource = source;
						heli = HelicopterScript.Cast( source );
						if ( heli.CrewSize() > 0 )
						{
							player = heli.CrewMember( 0 );
							if ( player )
								m_HitSourceParent = player;
						}
					}
				}
				//! Player crashed with his vehicle
				else if ( ammo == "TransportHit" )
				{
					#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionKillFeedModule::OnPlayerHitEvent - TRANSPORT HIT");
		#endif
					
					#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "ExpansionKillFeedModule:: Source: " + source.GetType() );
		#endif

					m_LastDamageSource = ExpansionDamageSource.VEHICLE_CRASH;
					m_HitSource = source;
					
					if ( source.IsKindOf( "CarScript" ) )
					{
						car = CarScript.Cast( source );
						m_Vehicle = car;
						if ( car.IsCar() )
						{
							#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "ExpansionKillFeedModule::OnPlayerHitEvent - Vehicle was a car!" );
		#endif
														
							if ( car.CrewSize() > 0 )
							{
								driver = car.CrewMember( 0 );
								if ( driver )
									m_VehicleDriver = driver;
							}
						}
						else if ( car.IsHelicopter() )
						{
							#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "ExpansionKillFeedModule::OnPlayerHitEvent - Vehicle was a helicopter!" );
		#endif		

							if ( car.CrewSize() > 0 )
							{
								driver = car.CrewMember( 0 );
								if ( driver )
									m_VehicleDriver = driver;
							}
						}
					}
				}
				else if ( ammo == "FireDamage" )
				{
					#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionKillFeedModule::OnPlayerHitEvent - FIRE HIT");
		#endif

					m_LastDamageSource = ExpansionDamageSource.FIRE;
					m_HitSource = source;
				}
				break;
			}
			
			GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( this.ResetLastHitSource, 3000, false );
		}
	}
	
	void ResetLastHitSource()
	{
		m_LastDamageSource = ExpansionDamageSource.NONE;
	}
	
	// ------------------------------------------------------------
	// ExpansionKillFeedModule OnPlayerKilledEventNew
	// ------------------------------------------------------------
	void OnPlayerKilledEventNew( PlayerBase player, Object killer )
	{
		bool msg = false;
		bool dehydration = false;
		bool starvation = false;
		bool bloodlost = false;
		
		m_KillerPlayer 			= NULL;
		m_KillerAI 				= NULL;
		m_KillerEntity			= NULL;
		m_KillerWeapon 			= NULL;
		m_Killfeed 				= NULL;
		m_PlayerItemInHands 	= NULL;
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint( "ExpansionKillFeedModule::OnPlayerKilledEventNew - killer: " + killer.ClassName() );
		#endif
		
		if ( !player )
			return;

		m_PlayerName = player.GetIdentityName();
		
		float statWater = player.GetStatWater().Get();
		float statEnergy = player.GetStatEnergy().Get();
		float statBlood = player.GetHealth("", "Blood");
			
		// Check if player died from dehydration, starvation or bloodloss before continue to the other checks
		if ( statWater == 0 || statEnergy == 0 || statBlood == 0 )
		{
			if ( statWater == 0 )
			{
				#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionKillFeedModule::OnPlayerKilledEvent - Player died from dehydration!");
		#endif

				// Player died from dehydration
				m_Killfeed = new StringLocaliser( "STR_EXPANSION_KILLFEED_PLAYER_DIED_DEHYDRATION", m_PlayerName );
				msg = true;
				dehydration = true;
			}
			else if ( statEnergy == 0 )
			{
				#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionKillFeedModule::OnPlayerKilledEvent - Player died from starvation!");
		#endif

				// Player died from starvation
				m_Killfeed = new StringLocaliser( "STR_EXPANSION_KILLFEED_PLAYER_DIED_STARVATION", m_PlayerName );
				msg = true;
				starvation = true;
			}
			else if ( statBlood == 0 ) 
			{
				#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionKillFeedModule::OnPlayerKilledEvent - Player died from bloodloss!");
		#endif

				// Player died from bloodloss
				m_Killfeed = new StringLocaliser( "STR_EXPANSION_KILLFEED_PLAYER_DIED_BLEEDING", m_PlayerName );
				msg = true;
				bloodlost = true;
			}
			
			// Dont continue to other checks when player died from dehydration/starvation/bloodlost
			if ( dehydration || starvation || bloodlost )
			{
				if ( msg && m_Killfeed != NULL )
				{
					if ( GetExpansionSettings() && GetExpansionSettings().GetGeneral().KillFeedMessageType == ExpansionAnnouncementType.CHAT )
					{
						player.SendChatMessage( m_Killfeed.GetText() );
						GetExpansionLogger().KillFeedLog( m_Killfeed.Format() );
					}
					else if ( GetExpansionSettings() && GetExpansionSettings().GetGeneral().KillFeedMessageType == ExpansionAnnouncementType.NOTIFICATION )
					{
						GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_KILLFEED_TITLE" ), m_Killfeed, EXPANSION_NOTIFICATION_ICON_BANDIT, COLOR_EXPANSION_NOTIFICATION_EXPANSION, 7 );
						GetExpansionLogger().KillFeedLog( m_Killfeed.Format() );
					}
				}
				
				return;
			}
		}
		
		ref ExpansionKillFeedData hitData = player.GetCurrentHitData();
		if ( hitData && hitData != null )
		{
			OnPlayerHitEvent( hitData.m_DmgType, hitData.m_Source, hitData.m_Ammo );
		}
		else if ( !hitData )
		{
			m_LastDamageSource = ExpansionDamageSource.NONE;
		}
		
		switch ( m_LastDamageSource )
		{
			case ExpansionDamageSource.EXPLOSION_WEAPON:
			{
				if ( m_HitSource /*&& ( m_HitSource.IsInherited( Grenade_Base ) || m_HitSource.IsInherited( Ammunition_Base ) )*/)
				{
					if ( m_HitSourceParent != NULL )
					{
						if ( m_HitSourceParent.IsInherited(SurvivorBase) )
						{
							m_KillerPlayer = PlayerBase.Cast( m_HitSourceParent );
							m_KillerName = m_KillerPlayer.GetIdentityName();
							m_Killfeed = new StringLocaliser( "STR_EXPANSION_KILLFEED_PLAYER_DIED_EXPLOSION_WEAPON", m_PlayerName, m_HitSource.GetDisplayName(), m_KillerName );
							msg = true;
						}
					}
					else
					{
						m_Killfeed = new StringLocaliser( "STR_EXPANSION_KILLFEED_PLAYER_DIED_EXPLOSION", m_PlayerName, m_HitSource.GetDisplayName() );
						msg = true;
					}
				}
				break;
			}
			//! Todo: Does not work yet
			case ExpansionDamageSource.EXPLOSION_VEHICLE:
			{
				if ( m_HitSource )
				{
					m_Killfeed = new StringLocaliser( "STR_EXPANSION_KILLFEED_PLAYER_DIED_EXPLOSION_VEHICLE", m_PlayerName, m_HitSource.GetDisplayName() );
					msg = true;
				}
				break;
			}
			case ExpansionDamageSource.FALLING:
			{
				#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionKillFeedModule::OnPlayerKilledEvent - Player died from falling!");
		#endif

				m_Killfeed = new StringLocaliser( "STR_EXPANSION_KILLFEED_PLAYER_DIED_FALLING", m_PlayerName );
				msg = true;
				break;
			}
			case ExpansionDamageSource.PLAYER_WEAPON:
			{
				if ( m_HitSource && m_HitSource.IsWeapon() )
				{
					if ( m_HitSourceParent != NULL )
					{
						if ( m_HitSourceParent.IsInherited(SurvivorBase) )
						{
							m_KillerPlayer = PlayerBase.Cast( m_HitSourceParent );
							m_KillerName = m_KillerPlayer.GetIdentityName();
							m_KillerWeapon = m_KillerPlayer.GetItemInHands();
							float shootDistance = vector.Distance( player.GetPosition(), m_KillerWeapon.GetPosition() );
							m_Killfeed = new StringLocaliser( "STR_EXPANSION_KILLFEED_PLAYER_KILLED_PLAYER_WEAPON", m_PlayerName, m_KillerName, m_KillerWeapon.GetDisplayName(), Math.Round(shootDistance).ToString() );
							msg = true;
						}
					}
				}
				break;
			}
			case ExpansionDamageSource.PLAYER_MELEWEAPON:
			{
				if ( m_HitSource && m_HitSource.IsWeapon())
				{
					if ( m_HitSourceParent != NULL )
					{
						if ( m_HitSourceParent.IsInherited(SurvivorBase) )
						{
							m_KillerPlayer = PlayerBase.Cast( m_HitSourceParent );
							m_KillerName = m_KillerPlayer.GetIdentityName();
							m_KillerWeapon = m_KillerPlayer.GetItemInHands();
							m_Killfeed = new StringLocaliser( "STR_EXPANSION_KILLFEED_PLAYER_KILLED_PLAYER_MELE", m_PlayerName, m_KillerName, m_KillerWeapon.GetDisplayName() );
							msg = true;
						}
					}
				}
				break;
			}
			case ExpansionDamageSource.INFECTED:
			{
				if ( m_HitSource && m_HitSource.IsZombie() )
				{
					m_Killfeed = new StringLocaliser( "STR_EXPANSION_KILLFEED_PLAYER_KILLED_ZOMBIE", m_PlayerName );
					msg = true;
				}
				break;
			}
			case ExpansionDamageSource.ANIMAL:
			{
				if ( m_HitSource && m_HitSource.IsAnimal() )
				{
					m_Killfeed = new StringLocaliser( "STR_EXPANSION_KILLFEED_PLAYER_KILLED_ANIMAL", m_PlayerName, m_HitSource.GetDisplayName() );
					msg = true;
				}
				break;
			}
			case ExpansionDamageSource.VEHICLE_HIT:
			{
				if ( m_HitSource && ( m_HitSource.IsInherited( CarScript ) ) )
				{
					if ( m_HitSourceParent != NULL )
					{
						if ( m_HitSourceParent.IsInherited( SurvivorBase ) )
						{
							m_KillerPlayer = PlayerBase.Cast( m_HitSourceParent );
							m_KillerName = m_KillerPlayer.GetIdentityName();
							m_Killfeed = new StringLocaliser( "STR_EXPANSION_KILLFEED_PLAYER_DIED_VEHICLE", m_PlayerName, m_KillerName, m_HitSource.GetDisplayName() );
							msg = true;
						}
					}
				}
				break;
			}
			case ExpansionDamageSource.HELICOPTER_HIT:
			{
				if ( m_HitSource && ( m_HitSource.IsKindOf( "HelicopterScript" ) ) )
				{
					if ( m_HitSourceParent != NULL )
					{
						if ( m_HitSourceParent.IsInherited( SurvivorBase ) )
						{
							m_KillerPlayer = PlayerBase.Cast( m_HitSourceParent );
							m_KillerName = m_KillerPlayer.GetIdentityName();
							m_Killfeed = new StringLocaliser( "STR_EXPANSION_KILLFEED_PLAYER_DIED_VEHICLE", m_PlayerName, m_KillerName, m_HitSource.GetDisplayName() );
							msg = true;
						}
					}
				}
				break;
			}
			case ExpansionDamageSource.VEHICLE_CRASH:
			{
				if ( m_HitSource )
				{
					//! Vehicle had active driver as it crashed
					if ( m_VehicleDriver )
					{						
						//! Vehicle was a helicopter
						if ( m_Vehicle.IsHelicopter() )
						{
							//! Get heli driver as he as the one who crashed the vehicle
							m_KillerPlayer = PlayerBase.Cast( m_VehicleDriver );
							m_KillerName = m_KillerPlayer.GetIdentityName();

							if ( m_KillerPlayer )
							{
								//! If the killed player is not the driver of heli, he was killed by driver
								if ( m_KillerPlayer != player )
								{
									m_Killfeed = new StringLocaliser( "STR_EXPANSION_KILLFEED_PLAYER_KILLED_CRASH_HELI", m_PlayerName, m_HitSource.GetDisplayName(), m_KillerName );
									msg = true;
								}
								//! Killed player was the heli driver
								else
								{
									m_Killfeed = new StringLocaliser( "STR_EXPANSION_KILLFEED_PLAYER_CRASHED_HELI", m_PlayerName, m_HitSource.GetDisplayName() );
									msg = true;
								}
							}
						}
						//! Vehicle was a car
						else if ( m_Vehicle.IsCar() )
						{
							//! Get vehicle driver as he as the one who crashed the vehicle
							m_KillerPlayer = PlayerBase.Cast( m_VehicleDriver );
							m_KillerName = m_KillerPlayer.GetIdentityName();

							if ( m_KillerPlayer )
							{
								//! If the killed player is not the driver he was killed by driver
								if ( m_KillerPlayer != player )
								{
									m_Killfeed = new StringLocaliser( "STR_EXPANSION_KILLFEED_PLAYER_KILLED_CRASH_CAR", m_PlayerName, m_HitSource.GetDisplayName(), m_KillerName );
									msg = true;
								}
								//! Killed player was the driver
								else
								{
									m_Killfeed = new StringLocaliser( "STR_EXPANSION_KILLFEED_PLAYER_CRASHED_CAR", m_PlayerName, m_HitSource.GetDisplayName() );
									msg = true;
								}
							}
						}
					}
					//! Vehicle had no active driver
					else
					{
						//! Vehicle was a helicopter
						if ( m_Vehicle.IsHelicopter() )
						{
							m_Killfeed = new StringLocaliser( "STR_EXPANSION_KILLFEED_PLAYER_KILLED_CRASH_HELI_NODRIVER", m_PlayerName, m_HitSource.GetDisplayName() );
							msg = true;
						}
						//! Vehicle was a car
						else if ( m_Vehicle.IsCar() )
						{
							m_Killfeed = new StringLocaliser( "STR_EXPANSION_KILLFEED_PLAYER_KILLED_CRASH_CAR_NODRIVER", m_PlayerName, m_HitSource.GetDisplayName() );
							msg = true;
						}
					}
				}
				
				break;
			}
			case ExpansionDamageSource.WIRE:
			{
				//! Wire dmg is not used yet?!
				break;
			}
			case ExpansionDamageSource.FIRE:
			{
				if ( m_HitSource && m_HitSource.IsFireplace() )
				{
					m_Killfeed = new StringLocaliser( "STR_EXPANSION_KILLFEED_PLAYER_DIED_FIRE", m_PlayerName );
					msg = true;
				}
				break;
			}
			case ExpansionDamageSource.FIST:
			{
				if ( m_HitSource )
				{
					if ( m_HitSource.IsPlayer() )
					{
						m_KillerPlayer = PlayerBase.Cast( m_HitSource );
						m_KillerName = m_KillerPlayer.GetIdentityName();
						m_Killfeed = new StringLocaliser( "STR_EXPANSION_KILLFEED_PLAYER_KILLED_PLAYE_BAREHANDS", m_PlayerName, m_KillerName );
						msg = true;
					}
				}
				break;
			}
			case ExpansionDamageSource.NONE:
			{
				#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionKillFeedModule::OnPlayerKilledEvent - NONE");
		#endif

				//! Check if we have a player and killer object
				if ( player && killer )
				{	
					//! Check if killer object was a player		
					if ( killer.IsInherited( SurvivorBase ) )
					{
						m_KillerEntity = EntityAI.Cast( killer );
						m_KillerPlayer = PlayerBase.Cast( m_KillerEntity.GetHierarchyRootPlayer() );

						if ( m_KillerPlayer )
						{
							m_KillerName = m_KillerPlayer.GetIdentityName();

							if ( m_KillerPlayer == player )
							{
								#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionKillFeedModule::OnPlayerKilledEvent - Player commited suicide!");
		#endif

								m_PlayerItemInHands = player.GetItemInHands();
								if ( m_PlayerItemInHands )
								{
									m_Killfeed = new StringLocaliser( "STR_EXPANSION_KILLFEED_PLAYER_DIED_SUICIDE", m_PlayerName, m_PlayerItemInHands.GetDisplayName() );
									msg = true;
								}
							}
						}
					}
				}		
				break;
			}
		}
		
		if ( msg && m_Killfeed != NULL )
		{
			if ( GetExpansionSettings() && GetExpansionSettings().GetGeneral().KillFeedMessageType == ExpansionAnnouncementType.CHAT )
			{
				player.SendChatMessage( m_Killfeed.GetText() );
				GetExpansionLogger().KillFeedLog( m_Killfeed.Format() );
			}
			else if ( GetExpansionSettings() && GetExpansionSettings().GetGeneral().KillFeedMessageType == ExpansionAnnouncementType.NOTIFICATION )
			{
				GetNotificationSystem().CreateNotification( new StringLocaliser( "STR_EXPANSION_KILLFEED_TITLE" ), m_Killfeed, EXPANSION_NOTIFICATION_ICON_BANDIT, COLOR_EXPANSION_NOTIFICATION_EXPANSION, 7 );
				GetExpansionLogger().KillFeedLog( m_Killfeed.Format() );
			}
		}
	}
}