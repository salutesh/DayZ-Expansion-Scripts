/**
 * ExpansionKillFeedModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

[CF_RegisterModule(ExpansionKillFeedModule)]
class ExpansionKillFeedModule: CF_ModuleWorld
{
	private string							m_PlayerPrefix;
	private string							m_PlayerSteamWebhook;
	
	private string 							m_PlayerPrefix2;
	private string							m_PlayerSteamWebhook2;
	
	private string							m_DisplayName;
	private PlayerBase					m_Player;
	private PlayerBase					m_Source;
	private PlayerStat<float>		m_StatWater;
	private PlayerStat<float>		m_StatEnergy;
	private float							m_StatBlood;
	private EntityAI 						m_ItemEntity;
	
	private bool								m_HitCheckDone;

#ifdef JM_COT
	protected JMWebhookModule m_Webhook;
#endif
	
	// void EventOnPlayerKilled(ExpansionPlayerDeathType deathType, PlayerBase player, PlayerBase killer = null, EntityAI source = null)
	static ref ScriptInvoker				s_EventOnPlayerDeath = new ScriptInvoker();

	override void OnInit()
	{
		super.OnInit();

		EnableRPC();
		
#ifdef JM_COT
		CF_Modules<JMWebhookModule>.Get(m_Webhook);
#endif
	}
	
#ifdef JM_COT
	// ------------------------------------------------------------
	// ExpansionKillFeedModule GetWebhookTitle
	// ------------------------------------------------------------		
	string GetWebhookTitle()
	{
		return "Killfeed Module";
	}
	
	// ------------------------------------------------------------
	// ExpansionKillFeedModule GetWebhookTypes
	// ------------------------------------------------------------	
	void GetWebhookTypes( out array< string > types )
	{
		types.Insert( "Killfeed" );
	}

	string GetModuleName()
	{
		return ClassName();
	}

	void SendWebhook( string type, string message )
	{
		if ( !m_Webhook || IsMissionOffline() )
			return;

		auto msg = m_Webhook.CreateDiscordMessage();

		msg.GetEmbed().AddField( GetWebhookTitle(), message, false );

		m_Webhook.Post( GetModuleName() + type, msg );
	}

	void SendWebhook( string type, JMPlayerInstance player, string message )
	{
		#ifdef JM_COT_WEBHOOK_DEBUG
		Print( "+ExpansionKillFeedModule::SendWebhook() - Admin" );
		#endif
		if ( !m_Webhook || !player || IsMissionOffline() )
			return;

		auto msg = m_Webhook.CreateDiscordMessage( player, "Admin Account: " );

		msg.GetEmbed().AddField( GetWebhookTitle(), message, false );

		#ifdef JM_COT_WEBHOOK_DEBUG
		Print( "name=" + GetModuleName() + type );
		Print( "message=" + message );
		#endif

		m_Webhook.Post( GetModuleName() + type, msg );
		
		#ifdef JM_COT_WEBHOOK_DEBUG
		Print( "-ExpansionKillFeedModule::SendWebhook() - Admin" );
		#endif
	}

	void GetSubCommands(inout array<ref JMCommand> commands)
	{
	}

	void AddSubCommand(inout array<ref JMCommand> commands, string command, string function, string permission)
	{
		commands.Insert(new JMSubCommand(this, command, function, permission));
	}

	array<string> GetCommandNames()
	{
		return new array<string>();
	}
#endif

	// ------------------------------------------------------------
	// ExpansionKillFeedModule OnPlayerHitBy
	// Event executed from playerbase EEHitBy function
	// ------------------------------------------------------------	
	void OnPlayerHitBy(int damageType, PlayerBase player, EntityAI source, string ammo)
	{
		#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
		EXLogPrint( "ExpansionKillFeedModule::OnPlayerHitBy - Start" );
		#endif
		
		m_PlayerPrefix = GetPlayerPrefix( player.GetIdentity() );
		#ifdef JM_COT
		m_PlayerSteamWebhook = player.FormatSteamWebhook();
		#endif
		m_HitCheckDone = false;
		
		if (GetExpansionSettings().GetLog().Killfeed)
			GetExpansionSettings().GetLog().PrintLog("[Killfeed] - Player " + m_PlayerPrefix + " hit by " + source.GetDisplayName() + " [DmgType: " + damageType.ToString() + " | Ammo: " + ammo + "]" );
		
		switch ( damageType )
		{
			case DT_CUSTOM:
				#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
				EXLogPrint( "ExpansionKillFeedModule::OnPlayerHitBy - DT_CUSTOM" );
				#endif
				if( ammo == "FallDamage" )											//! Fall damage
				{
					OnFallDamage(player);
				} else if( ammo == "TransportHit" )									//! Vehicle Crash
				{
					OnTransportHit(player, source);
				} else if( ammo == "FireDamage" )									//! Fire damage (example: Fireplace)
				{
					OnFireDamage(player);
				} else if( source.GetType() == "AreaDamageBase" )					//! Area Hit
				{
					OnAreaDamage(player, source);
				} else if( ammo == "BarbedWireDamage" )								//! Barbed wire damage
				{
					OnBarbedWireDamage(player);
				}
			break;
			case DT_EXPLOSION:
				#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
				EXLogPrint( "ExpansionKillFeedModule::OnPlayerHitBy - DT_EXPLOSION" );
				#endif
				if( ammo == "ExpansionC4_Ammo" || (source && !source.IsWeapon()) )	//! Expansion Satchel or other non-weapon explosion
				{
					OnExplosionHit(player, source);
				}
			break;
		}
		
		#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
		EXLogPrint( "ExpansionKillFeedModule::OnPlayerHitBy - End" );
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionKillFeedModule OnFallDamage
	// ------------------------------------------------------------	
	private void OnFallDamage(PlayerBase player)
	{
		#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
		EXLogPrint( "ExpansionKillFeedModule::OnFallDamage - Start" );
		#endif
		
		if( player && !player.IsAlive() )
		{
			KillFeedMessage( ExpansionKillFeedMessageType.FALL, ExpansionIcons.GetPath("Skull 1"), m_PlayerPrefix );
			s_EventOnPlayerDeath.Invoke(ExpansionPlayerDeathType.FALL, player);
			DiscordMessage(ExpansionKillFeedMessageType.FALL, m_PlayerSteamWebhook);
			
			m_HitCheckDone = true;
		}
		
		#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
		EXLogPrint( "ExpansionKillFeedModule::OnFallDamage - End" );
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionKillFeedModule OnTransportHit
	// ------------------------------------------------------------	
	private void OnTransportHit(PlayerBase player, EntityAI source)
	{
		#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
		EXLogPrint( "ExpansionKillFeedModule::OnTransportHit - Start" );
		#endif
		
		if( player && !player.IsAlive() && source )
		{
			if( source.IsInherited( CarScript ) )	//! Vehicle Hit - Car
			{
				CarScript car = CarScript.Cast( source );
				m_Source = PlayerBase.Cast( EntityAI.Cast( source ).GetHierarchyParent() );
				m_PlayerPrefix2 = "";
				m_PlayerSteamWebhook2 = "";
				m_DisplayName = "";
				m_DisplayName = source.ClassName();
				
				if(m_Source && m_Source.IsInherited(PlayerBase))
				{
					m_PlayerPrefix2 = GetPlayerPrefix( m_Source.GetIdentity() );
					#ifdef JM_COT
					m_PlayerSteamWebhook2 = player.FormatSteamWebhook();
					#endif
				}
			
				if( car.IsCar() )
				{										
					if( m_Source && m_Source.IsInherited(PlayerBase))
					{
						KillFeedMessage( ExpansionKillFeedMessageType.CAR_HIT_DRIVER, ExpansionIcons.GetPath("Skull 1"), m_PlayerPrefix, m_DisplayName, m_PlayerPrefix2 );
						s_EventOnPlayerDeath.Invoke(ExpansionPlayerDeathType.VEHICLE, player, m_Source, source);
						DiscordMessage( ExpansionKillFeedMessageType.CAR_HIT_DRIVER, m_PlayerSteamWebhook, m_DisplayName, m_PlayerSteamWebhook2 );
						
						m_HitCheckDone = true;
					} else if( !m_Source )
					{
						KillFeedMessage( ExpansionKillFeedMessageType.CAR_HIT_NODRIVER, ExpansionIcons.GetPath("Skull 1"), m_PlayerPrefix, m_DisplayName );
						s_EventOnPlayerDeath.Invoke(ExpansionPlayerDeathType.VEHICLE, player, null, source);
						DiscordMessage( ExpansionKillFeedMessageType.CAR_HIT_NODRIVER, m_PlayerSteamWebhook, m_DisplayName );
						
						m_HitCheckDone = true;
					}
				}
			} else if( source.IsInherited( ExpansionHelicopterScript ) || source.IsInherited( ExpansionVehicleHelicopterBase ) )	//! Vehicle Hit - Helicopter
			{
				m_Source = PlayerBase.Cast( EntityAI.Cast( source ).GetHierarchyParent() );
				m_PlayerPrefix2 = "";
				m_PlayerSteamWebhook2 = "";
				m_DisplayName = "";
				m_DisplayName = source.ClassName();
				
				#ifdef JM_COT
				m_PlayerSteamWebhook2 = player.FormatSteamWebhook();
				#endif

				if( m_Source )
				{
					m_PlayerPrefix2 = GetPlayerPrefix( m_Source.GetIdentity() );
					KillFeedMessage( ExpansionKillFeedMessageType.HELI_HIT_DRIVER, ExpansionIcons.GetPath("Vehicle Crash"), m_PlayerPrefix, m_DisplayName, m_PlayerPrefix2 );
					s_EventOnPlayerDeath.Invoke(ExpansionPlayerDeathType.VEHICLE, player, m_Source, source);
					DiscordMessage( ExpansionKillFeedMessageType.HELI_HIT_DRIVER, m_PlayerSteamWebhook, m_DisplayName, m_PlayerSteamWebhook2 );
				
					m_HitCheckDone = true;
				} else if ( !m_Source )
				{
					KillFeedMessage( ExpansionKillFeedMessageType.HELI_HIT_NODRIVER, ExpansionIcons.GetPath("Vehicle Crash"), m_PlayerPrefix, m_DisplayName );
					s_EventOnPlayerDeath.Invoke(ExpansionPlayerDeathType.VEHICLE, player, null, source);
					DiscordMessage( ExpansionKillFeedMessageType.HELI_HIT_NODRIVER, m_PlayerSteamWebhook, m_DisplayName );
					
					m_HitCheckDone = true;
				}			
			}/* else if( source.IsInherited( ExpansionPlaneScript ) || source.IsInherited( ExpansionVehiclePlaneBase ) )	//! Vehicle Hit - Helicopter
			{
				m_Source = PlayerBase.Cast( EntityAI.Cast( source ).GetHierarchyParent() );
				m_PlayerPrefix2 = "";
				m_PlayerSteamWebhook2 = "";
				m_DisplayName = "";
				m_DisplayName = source.ClassName();
				²
				m_PlayerSteamWebhook2 = player.FormatSteamWebhook();

				if( m_Source )
				{
					m_PlayerPrefix2 = GetPlayerPrefix( m_Source.GetIdentity() );
					KillFeedMessage( ExpansionKillFeedMessageType.HELI_HIT_DRIVER, ExpansionIcons.GetPath("Vehicle Crash"), m_PlayerPrefix, m_DisplayName, m_PlayerPrefix2 );
					s_EventOnPlayerDeath.Invoke(ExpansionPlayerDeathType.VEHICLE, player, m_Source, source);
					DiscordMessage( ExpansionKillFeedMessageType.HELI_HIT_DRIVER, m_PlayerSteamWebhook, m_DisplayName, m_PlayerSteamWebhook2 );
				
					m_HitCheckDone = true;
				} else if ( !m_Source )
				{
					KillFeedMessage( ExpansionKillFeedMessageType.HELI_HIT_NODRIVER, ExpansionIcons.GetPath("Vehicle Crash"), m_PlayerPrefix, m_DisplayName );
					s_EventOnPlayerDeath.Invoke(ExpansionPlayerDeathType.VEHICLE, player, null, source);
					DiscordMessage( ExpansionKillFeedMessageType.HELI_HIT_NODRIVER, m_PlayerSteamWebhook, m_DisplayName );
					
					m_HitCheckDone = true;
				}
			} else if( source.IsInherited( ExpansionBoatScript ) || source.IsInherited( ExpansionVehicleBoatBase ) )	//! Vehicle Hit - Helicopter
			{
				m_Source = PlayerBase.Cast( EntityAI.Cast( source ).GetHierarchyParent() );
				m_PlayerPrefix2 = "";
				m_PlayerSteamWebhook2 = "";
				m_DisplayName = "";
				m_DisplayName = source.ClassName();
				²
				m_PlayerSteamWebhook2 = player.FormatSteamWebhook();

				if( m_Source )
				{
					m_PlayerPrefix2 = GetPlayerPrefix( m_Source.GetIdentity() );
					KillFeedMessage( ExpansionKillFeedMessageType.HELI_HIT_DRIVER, ExpansionIcons.GetPath("Vehicle Crash"), m_PlayerPrefix, m_DisplayName, m_PlayerPrefix2 );
					s_EventOnPlayerDeath.Invoke(ExpansionPlayerDeathType.VEHICLE, player, m_Source, source);
					DiscordMessage( ExpansionKillFeedMessageType.HELI_HIT_DRIVER, m_PlayerSteamWebhook, m_DisplayName, m_PlayerSteamWebhook2 );
				
					m_HitCheckDone = true;
				} else if ( !m_Source )
				{
					KillFeedMessage( ExpansionKillFeedMessageType.HELI_HIT_NODRIVER, ExpansionIcons.GetPath("Vehicle Crash"), m_PlayerPrefix, m_DisplayName );
					s_EventOnPlayerDeath.Invoke(ExpansionPlayerDeathType.VEHICLE, player, null, source);
					DiscordMessage( ExpansionKillFeedMessageType.HELI_HIT_NODRIVER, m_PlayerSteamWebhook, m_DisplayName );
					
					m_HitCheckDone = true;
				}
			} else if( source.IsInherited( ExpansionBikeScript ) || source.IsInherited( ExpansionVehicleBikeBase ) )	//! Vehicle Hit - Helicopter
			{
				m_Source = PlayerBase.Cast( EntityAI.Cast( source ).GetHierarchyParent() );
				m_PlayerPrefix2 = "";
				m_PlayerSteamWebhook2 = "";
				m_DisplayName = "";
				m_DisplayName = source.ClassName();
				²
				m_PlayerSteamWebhook2 = player.FormatSteamWebhook();

				if( m_Source )
				{
					m_PlayerPrefix2 = GetPlayerPrefix( m_Source.GetIdentity() );
					KillFeedMessage( ExpansionKillFeedMessageType.HELI_HIT_DRIVER, ExpansionIcons.GetPath("Vehicle Crash"), m_PlayerPrefix, m_DisplayName, m_PlayerPrefix2 );
					s_EventOnPlayerDeath.Invoke(ExpansionPlayerDeathType.VEHICLE, player, m_Source, source);
					DiscordMessage( ExpansionKillFeedMessageType.HELI_HIT_DRIVER, m_PlayerSteamWebhook, m_DisplayName, m_PlayerSteamWebhook2 );
				
					m_HitCheckDone = true;
				} else if ( !m_Source )
				{
					KillFeedMessage( ExpansionKillFeedMessageType.HELI_HIT_NODRIVER, ExpansionIcons.GetPath("Vehicle Crash"), m_PlayerPrefix, m_DisplayName );
					s_EventOnPlayerDeath.Invoke(ExpansionPlayerDeathType.VEHICLE, player, null, source);
					DiscordMessage( ExpansionKillFeedMessageType.HELI_HIT_NODRIVER, m_PlayerSteamWebhook, m_DisplayName );
					
					m_HitCheckDone = true;
				}
			}*/
		}
		
		#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
		EXLogPrint( "ExpansionKillFeedModule::OnTransportHit - End" );
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionKillFeedModule OnBarbedWireDamage
	// ------------------------------------------------------------	
	private void OnBarbedWireDamage(PlayerBase player)
	{
		#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
		EXLogPrint( "ExpansionKillFeedModule::OnBarbedWireDamage - Start" );
		#endif
		
		if( player && !player.IsAlive() )
		{
			KillFeedMessage( ExpansionKillFeedMessageType.BARBEDWIRE, ExpansionIcons.GetPath("Skull 1"), m_PlayerPrefix );
			s_EventOnPlayerDeath.Invoke(ExpansionPlayerDeathType.BARBEDWIRE, player);
			DiscordMessage( ExpansionKillFeedMessageType.BARBEDWIRE, m_PlayerSteamWebhook );
			
			m_HitCheckDone = true;
		}
		
		#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
		EXLogPrint( "ExpansionKillFeedModule::OnBarbedWireDamage - End" );
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionKillFeedModule OnFireDamage
	// ------------------------------------------------------------	
	private void OnFireDamage(PlayerBase player)
	{
		#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
		EXLogPrint( "ExpansionKillFeedModule::OnFireDamage - Start" );
		#endif
		
		if( player && !player.IsAlive() )
		{
			KillFeedMessage( ExpansionKillFeedMessageType.FIRE, ExpansionIcons.GetPath("Fireplace"), m_PlayerPrefix );
			s_EventOnPlayerDeath.Invoke(ExpansionPlayerDeathType.FIRE, player);
			DiscordMessage( ExpansionKillFeedMessageType.FIRE, m_PlayerSteamWebhook );
			
			m_HitCheckDone = true;
		}
		
		#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
		EXLogPrint( "ExpansionKillFeedModule::OnFireDamage - End" );
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionKillFeedModule OnAreaDamage
	// ------------------------------------------------------------	
	private void OnAreaDamage(PlayerBase player, EntityAI source)
	{
		#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
		EXLogPrint( "ExpansionKillFeedModule::OnAreaDamage - Start" );
		#endif
		
		if( source.GetHierarchyParent().IsKindOf( "CarScript" ) )							//! Check if hit came from a car
		{
			#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
			EXLogPrint("ExpansionKillFeedModule::OnAreaDamage - Car");
			#endif
		} else if( source.GetHierarchyParent().IsKindOf( "ExpansionHelicopterScript" ) )	//! Check if hit came from a helicopter
		{
			#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
			EXLogPrint("ExpansionKillFeedModule::OnAreaDamage - Heli");
			#endif
		} else if( source.GetHierarchyParent().IsKindOf( "ExpansionBoatScript" ) )			//! Check if hit came from a boat
		{
			#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
			EXLogPrint("ExpansionKillFeedModule::OnAreaDamage - Boat");
			#endif
		}
		
		#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
		EXLogPrint( "ExpansionKillFeedModule::OnAreaDamage - End" );
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionKillFeedModule OnExplosionHit
	// ------------------------------------------------------------	
	private void OnExplosionHit(PlayerBase player, EntityAI source)
	{
		#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
		EXLogPrint( "ExpansionKillFeedModule::OnExplosionHit - Start" );
		#endif
		
		m_DisplayName = source.GetType();
		
		//! Hardcoded condition for Expansion satchel item
		if (source.ClassName() == "Expansion_C4_Explosion")
		{
			m_DisplayName = "ExpansionSatchel";
			#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
			EXLogPrint("ExpansionKillFeedModule::OnExplosionHit m_DisplayName: " + m_DisplayName);
			#endif
		}	
		
		if ( player && !player.IsAlive() )
		{			
			if(source)
			{
				KillFeedMessage( ExpansionKillFeedMessageType.WEAPON_EXPLOSION, ExpansionIcons.GetPath("Skull 1"), m_PlayerPrefix, m_DisplayName );
				s_EventOnPlayerDeath.Invoke(ExpansionPlayerDeathType.EXPLOSION, player, null, source);
				DiscordMessage( ExpansionKillFeedMessageType.WEAPON_EXPLOSION, m_PlayerSteamWebhook, m_DisplayName );
				
				m_HitCheckDone = true;
			}	
		}
		
		#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
		EXLogPrint( "ExpansionKillFeedModule::OnExplosionHit - End" );
		#endif
	}
		
	bool WasHitCheckDone()
	{
		return m_HitCheckDone;
	}

	// ------------------------------------------------------------
	// ExpansionKillFeedModule CalcBlood
	// ------------------------------------------------------------
	protected int CalcBlood(PlayerBase player)
	{
		return CalcPercent(player.GetHealth("", "Blood") - 2500,  2500);
	}
	
	// ------------------------------------------------------------
	// ExpansionKillFeedModule CalcPercent
	// ------------------------------------------------------------
	protected int CalcPercent(float value, float max)
	{
		return (Math.Round(value) * 100)/max;
	}
		
	// ------------------------------------------------------------
	// ExpansionKillFeedModule OnKilledByCondition
	// ------------------------------------------------------------	
	bool OnKilledByCondition(PlayerBase player, Object source)
	{
		#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
		EXLogPrint( "ExpansionKillFeedModule::OnKilledByCondition - Start" );
		#endif
			
		m_PlayerPrefix = GetPlayerPrefix( player.GetIdentity() );
		#ifdef JM_COT
		m_PlayerSteamWebhook = player.FormatSteamWebhook();
		#endif
				
		m_StatWater = player.GetStatWater();
		m_StatEnergy = player.GetStatEnergy();
		m_StatBlood = CalcBlood(player);
		
		if (m_StatWater && m_StatWater.Get() == 0)
		{
			KillFeedMessage( ExpansionKillFeedMessageType.DEHYDRATION, ExpansionIcons.GetPath("Water"), m_PlayerPrefix );
			s_EventOnPlayerDeath.Invoke(ExpansionPlayerDeathType.DEHYDRATION, player);
			DiscordMessage( ExpansionKillFeedMessageType.DEHYDRATION, m_PlayerSteamWebhook );
			return true;
		} else if (m_StatEnergy && m_StatEnergy.Get() == 0)
		{
			KillFeedMessage( ExpansionKillFeedMessageType.STARVATION, ExpansionIcons.GetPath("Heart"), m_PlayerPrefix);
			s_EventOnPlayerDeath.Invoke(ExpansionPlayerDeathType.STARVATION, player);
			DiscordMessage( ExpansionKillFeedMessageType.STARVATION, m_PlayerSteamWebhook );
			return true;
		} else if (m_StatBlood && m_StatBlood < 0.9)
		{
			KillFeedMessage( ExpansionKillFeedMessageType.BLEEDING, ExpansionIcons.GetPath("Human Skull"), m_PlayerPrefix );
			s_EventOnPlayerDeath.Invoke(ExpansionPlayerDeathType.BLEEDING, player);
			DiscordMessage( ExpansionKillFeedMessageType.BLEEDING, m_PlayerSteamWebhook );
			return true;
		}
		
		#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
		EXLogPrint( "ExpansionKillFeedModule::OnKilledByCondition - End" );
		#endif
			
		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionKillFeedModule OnPlayerSuicide
	// ------------------------------------------------------------	
	void OnPlayerSuicide(PlayerBase player)
	{
		//#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
		EXLogPrint( "ExpansionKillFeedModule::OnPlayerSuicide - Start" );
		//#endif
		
		if (player)
		{
			m_PlayerPrefix = GetPlayerPrefix( player.GetIdentity() );
			#ifdef JM_COT
			m_PlayerSteamWebhook = player.FormatSteamWebhook();
			#endif
					
			// Check if player has something in hands
			m_ItemEntity = EntityAI.Cast( player.GetHumanInventory().GetEntityInHands() );
			if ( m_ItemEntity )
			{
				// If item in hands is kind of weapon/meele-weapon
				if ( m_ItemEntity.IsWeapon() || m_ItemEntity.IsMeleeWeapon() )
				{
					ItemBase item = ItemBase.Cast( player.GetHumanInventory().GetEntityInHands() );
					m_DisplayName = "";
					m_DisplayName = item.GetType();
					
					if (m_DisplayName && m_DisplayName != "")
					{
						string icon;
						if ( m_ItemEntity.IsMeleeWeapon() )
						{
							icon = ExpansionIcons.GetPath("Knife");
						} else if ( m_ItemEntity.IsWeapon() )
						{
							icon = ExpansionIcons.GetPath("Skull 1");
						}
						else
						{
							icon = ExpansionIcons.GetPath("Skull 1");
						}
						
						KillFeedMessage( ExpansionKillFeedMessageType.SUICIDE, icon, m_PlayerPrefix, m_DisplayName );
						s_EventOnPlayerDeath.Invoke(ExpansionPlayerDeathType.SUICIDE, player);
						DiscordMessage( ExpansionKillFeedMessageType.SUICIDE, m_PlayerSteamWebhook, m_DisplayName );
					}
				}
			}
			
			//#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
			EXLogPrint("ExpansionKillFeedModule::OnPlayerSuicide - Hierarchy Parent: " + player.GetHierarchyParent().ToString());
			EXLogPrint("ExpansionKillFeedModule::OnPlayerSuicide - Hierarchy Root: " + player.GetHierarchyRoot().ToString());
			//#endif
				
			/*array<string> names;
			IEntity child;
			PlayerBase childPB;
			int i;
			string passanger_names = "";
			string current_name;
			
			//! Check for transport
			HumanCommandVehicle hcv = player.GetCommand_Vehicle();
			if( hcv )
			{
				Transport transport = hcv.GetTransport();
				if( transport )
				{
					CarScript car = CarScript.Cast(transport);
					if(car)
					{
						if( car.IsCar() )
						{
							#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
							EXLogPrint("ExpansionKillFeedModule::OnPlayerSuicide - HumanCommandVehicle Car BOOM: " + car.ClassName());
							#endif
							if( car.IsExploded() )
							{
								#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
								EXLogPrint("ExpansionKillFeedModule::OnPlayerSuicide - CarScript GetChildren: " + car.GetChildren());
								#endif
								m_DisplayName = "";
								m_DisplayName = car.ClassName();
								
								names = new array<string>;
								child = car.GetChildren();
								while( child != NULL && child.GetSibling() != NULL )
								{
									child = child.GetSibling();
									if(child.IsInherited(PlayerBase))
									{
										childPB = PlayerBase.Cast(child);
										if(childPB.GetIdentity() != player.GetIdentity())
											names.Insert( GetPlayerPrefix( childPB.GetIdentity() ) );
									}
								}
								
								if(names.Count() > 0)
								{
									for (i = 0; i < names.Count(); i++)
									{
										if (i != names.Count())
										{
											current_name = names[i];				
											passanger_names = passanger_names + current_name + ", ";
										} else
										{
											current_name = names[i];				
											passanger_names = passanger_names + current_name;
										}
									}
									
									KillFeedMessage( ExpansionKillFeedMessageType.CAR_CRASH_CREW, ExpansionIcons.GetPath("Helicopter"), m_PlayerPrefix, m_DisplayName, passanger_names );
									s_EventOnPlayerDeath.Invoke(ExpansionPlayerDeathType.CAR_CRASH, player, null, car);
									DiscordMessage( ExpansionKillFeedMessageType.CAR_CRASH_CREW, m_PlayerSteamWebhook, m_DisplayName, passanger_names );
								} else
								{
									KillFeedMessage( ExpansionKillFeedMessageType.CAR_CRASH, ExpansionIcons.GetPath("Helicopter"), m_PlayerPrefix, m_DisplayName );
									s_EventOnPlayerDeath.Invoke(ExpansionPlayerDeathType.CAR_CRASH, player, null, car);
									DiscordMessage( ExpansionKillFeedMessageType.CAR_CRASH, m_PlayerSteamWebhook, m_DisplayName );
								}
							}
						} else if( car.IsHelicopter() )
						{
							#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
							EXLogPrint("ExpansionKillFeedModule::OnPlayerSuicide - HumanCommandVehicle Heli BOOM: " + car.ClassName());
							#endif
							ExpansionHelicopterScript exheli = ExpansionHelicopterScript.Cast( car );
							if( exheli && exheli.IsExploded() )
							{
								#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
								EXLogPrint("ExpansionKillFeedModule::OnPlayerSuicide - ExpansionHelicopterScript GetChildren: " + exheli.GetChildren());
								#endif
								m_DisplayName = "";
								m_DisplayName = exheli.ClassName();
								
								names = new array<string>;
								child = exheli.GetChildren();
								while( child != NULL && child.GetSibling() != NULL )
								{
									child = child.GetSibling();
									if(child.IsInherited(PlayerBase))
									{
										childPB = PlayerBase.Cast(child);
										if(childPB.GetIdentity() != player.GetIdentity())
											names.Insert( GetPlayerPrefix( childPB.GetIdentity() ) );
									}
								}
								
								if(names.Count() > 0)
								{
									for (i = 0; i < names.Count(); i++)
									{
										if (i != names.Count())
										{
											current_name = names[i];				
											passanger_names = passanger_names + current_name + ", ";
										} else
										{
											current_name = names[i];				
											passanger_names = passanger_names + current_name;
										}
									}
									
									KillFeedMessage( ExpansionKillFeedMessageType.HELI_CRASH_CREW, ExpansionIcons.GetPath("Helicopter"), m_PlayerPrefix, m_DisplayName, passanger_names );
									s_EventOnPlayerDeath.Invoke(ExpansionPlayerDeathType.HELI_CRASH, player, null, exheli);
									DiscordMessage( ExpansionKillFeedMessageType.HELI_CRASH_CREW, m_PlayerSteamWebhook, m_DisplayName, passanger_names );
								} else
								{
									KillFeedMessage( ExpansionKillFeedMessageType.HELI_CRASH, ExpansionIcons.GetPath("Helicopter"), m_PlayerPrefix, m_DisplayName );
									s_EventOnPlayerDeath.Invoke(ExpansionPlayerDeathType.HELI_CRASH, player, null, exheli);
									DiscordMessage( ExpansionKillFeedMessageType.HELI_CRASH, m_PlayerSteamWebhook, m_DisplayName );
								}
							}
						} else if( car.IsBoat() )
						{
							#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
							EXLogPrint("ExpansionKillFeedModule::OnPlayerSuicide - HumanCommandVehicle Boat BOOM: " + car.ClassName());
							#endif
							ExpansionBoatScript exboat = ExpansionBoatScript.Cast( car );
							if( exboat && exboat.IsExploded() )
							{
								#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
								EXLogPrint("ExpansionKillFeedModule::OnPlayerSuicide - ExpansionHelicopterScript GetChildren: " + exboat.GetChildren());
								#endif
								m_DisplayName = "";
								m_DisplayName = exboat.ClassName();
								
								names = new array<string>;
								child = exboat.GetChildren();
								while( child != NULL && child.GetSibling() != NULL )
								{
									child = child.GetSibling();
									if(child.IsInherited(PlayerBase))
									{
										childPB = PlayerBase.Cast(child);
										if(childPB.GetIdentity() != player.GetIdentity())
											names.Insert( GetPlayerPrefix( childPB.GetIdentity() ) );
									}
								}
								
								if(names.Count() > 0)
								{
									for (i = 0; i < names.Count(); i++)
									{
										if (i != names.Count())
										{
											current_name = names[i];				
											passanger_names = passanger_names + current_name + ", ";
										} else
										{
											current_name = names[i];				
											passanger_names = passanger_names + current_name;
										}
									}
									
									KillFeedMessage( ExpansionKillFeedMessageType.HELI_CRASH_CREW, ExpansionIcons.GetPath("Helicopter"), m_PlayerPrefix, m_DisplayName, passanger_names );
									s_EventOnPlayerDeath.Invoke(ExpansionPlayerDeathType.HELI_CRASH, player, null, exboat);
									DiscordMessage( ExpansionKillFeedMessageType.HELI_CRASH_CREW, m_PlayerSteamWebhook, m_DisplayName, passanger_names );
								} else
								{
									KillFeedMessage( ExpansionKillFeedMessageType.HELI_CRASH, ExpansionIcons.GetPath("Helicopter"), m_PlayerPrefix, m_DisplayName );
									s_EventOnPlayerDeath.Invoke(ExpansionPlayerDeathType.HELI_CRASH, player, null, exboat);
									DiscordMessage( ExpansionKillFeedMessageType.HELI_CRASH, m_PlayerSteamWebhook, m_DisplayName );
								}
							}
						}
					}
				}
			}*/
		}
			
		//#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
		EXLogPrint( "ExpansionKillFeedModule::OnPlayerSuicide - End" );
		//#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionKillFeedModule OnKilledByWeapon
	// ------------------------------------------------------------	
	void OnKilledByWeapon(PlayerBase player, Object source)
	{
		#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
		EXLogPrint( "ExpansionKillFeedModule::OnKilledByWeapon - Start" );
		#endif
			
		m_PlayerPrefix = GetPlayerPrefix( player.GetIdentity() );
		#ifdef JM_COT
		m_PlayerSteamWebhook = player.FormatSteamWebhook();
		#endif
		
		m_Source = PlayerBase.Cast( EntityAI.Cast( source ).GetHierarchyParent() );
		m_PlayerPrefix2 = "";
		m_PlayerSteamWebhook2 = "";
		m_DisplayName = "";
		m_DisplayName = source.GetType();
		
		if (m_Source && m_Source.IsInherited(PlayerBase))
		{
			#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
			EXLogPrint("ExpansionKillFeedModule::OnKilledByWeapon - m_Source : " + m_Source.ToString());
			#endif
			m_PlayerPrefix2 = this.GetPlayerPrefix( m_Source.GetIdentity() );
			#ifdef JM_COT
			m_PlayerSteamWebhook2 = m_Source.FormatSteamWebhook();
			#endif
			
			if( player.GetIdentity() != m_Source.GetIdentity() )
			{
				if( source.IsMeleeWeapon() )
				{						
					KillFeedMessage( ExpansionKillFeedMessageType.MELEWEAPON, ExpansionIcons.GetPath("Knife"), m_PlayerPrefix, m_PlayerPrefix2, m_DisplayName);
					s_EventOnPlayerDeath.Invoke(ExpansionPlayerDeathType.WEAPON, player, m_Source, source);
					DiscordMessage( ExpansionKillFeedMessageType.MELEWEAPON, m_PlayerSteamWebhook, m_PlayerSteamWebhook2, m_DisplayName );
				} 
				else
				{
					float distance = vector.Distance( player.GetPosition(), m_Source.GetPosition() );
					KillFeedMessage( ExpansionKillFeedMessageType.WEAPON, ExpansionIcons.GetPath("Gun"), m_PlayerPrefix, m_PlayerPrefix2, m_DisplayName, Math.Round(distance).ToString());
					s_EventOnPlayerDeath.Invoke(ExpansionPlayerDeathType.WEAPON, player, m_Source, source);
					DiscordMessage( ExpansionKillFeedMessageType.WEAPON, m_PlayerSteamWebhook, m_PlayerSteamWebhook2, m_DisplayName, Math.Round(distance).ToString() );
				}
			}
		} else if (source.IsInherited(ItemBase))
		{
			ItemBase item = ItemBase.Cast(source);
			if(item)
			{
				if( item.IsInherited(Grenade_Base) )
				{
					#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
					EXLogPrint("ExpansionKillFeedModule::OnKilledByWeapon - m_Source == NULL || Grenade_Base");
					#endif
					KillFeedMessage( ExpansionKillFeedMessageType.WEAPON_EXPLOSION, ExpansionIcons.GetPath("Grenade"), m_PlayerPrefix, m_DisplayName);
					s_EventOnPlayerDeath.Invoke(ExpansionPlayerDeathType.EXPLOSION, player, null, item);
					DiscordMessage( ExpansionKillFeedMessageType.WEAPON_EXPLOSION, m_PlayerSteamWebhook, m_DisplayName );
				}
			}
		}		
		
		#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
		EXLogPrint( "ExpansionKillFeedModule::OnKilledByWeapon - End" );
		#endif
	} 
	
	// ------------------------------------------------------------
	// ExpansionKillFeedModule OnKilledByPlayer
	// ------------------------------------------------------------	
	void OnKilledByPlayer(PlayerBase player, Object source)
	{
		#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
		EXLogPrint( "ExpansionKillFeedModule::OnKilledByPlayer - Start" );
		#endif
			
		m_PlayerPrefix = GetPlayerPrefix( player.GetIdentity() );
		#ifdef JM_COT
		m_PlayerSteamWebhook = player.FormatSteamWebhook();
		#endif
		
		m_Source = PlayerBase.Cast(EntityAI.Cast(source));
		m_PlayerPrefix2 = "";
		m_PlayerSteamWebhook2 = "";
		
		if (m_Source && m_Source.IsInherited(PlayerBase))
		{
			if( player.GetIdentity() != m_Source.GetIdentity() )
			{
				m_PlayerPrefix2 = this.GetPlayerPrefix( m_Source.GetIdentity() );
				#ifdef JM_COT
				m_PlayerSteamWebhook2 = m_Source.FormatSteamWebhook();
				#endif
				
				if(m_PlayerPrefix2 != "" || m_PlayerSteamWebhook2 != "") // Got player name
				{
					KillFeedMessage( ExpansionKillFeedMessageType.BAREHANDS, ExpansionIcons.GetPath("Grab"), m_PlayerPrefix, m_PlayerPrefix2 );	
					s_EventOnPlayerDeath.Invoke(ExpansionPlayerDeathType.BAREHANDS, player, m_Source);
					DiscordMessage( ExpansionKillFeedMessageType.BAREHANDS, m_PlayerSteamWebhook, m_PlayerSteamWebhook2 );
				}
			}
		}
		
		#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
		EXLogPrint( "ExpansionKillFeedModule::OnKilledByPlayer - End" );
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionKillFeedModule OnKilledByZombie
	// ------------------------------------------------------------	
	void OnKilledByZombie(PlayerBase player, Object source)
	{
		#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
		EXLogPrint( "ExpansionKillFeedModule::OnKilledByZombie - Start" );
		#endif
		
		m_PlayerPrefix = GetPlayerPrefix( player.GetIdentity() );
		#ifdef JM_COT
		m_PlayerSteamWebhook = player.FormatSteamWebhook();			
		#endif
			
		ZombieBase zombie = ZombieBase.Cast(EntityAI.Cast(source));
		if(zombie)
		{
			KillFeedMessage( ExpansionKillFeedMessageType.INFECTED, ExpansionIcons.GetPath("Infected 2"), m_PlayerPrefix );
			s_EventOnPlayerDeath.Invoke(ExpansionPlayerDeathType.INFECTED, player, null, zombie);
			DiscordMessage( ExpansionKillFeedMessageType.INFECTED, m_PlayerSteamWebhook );
		}
		
		#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
		EXLogPrint( "ExpansionKillFeedModule::OnKilledByZombie - End" );
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionKillFeedModule OnKilledByAnimal
	// ------------------------------------------------------------	
	void OnKilledByAnimal(PlayerBase player, Object source)
	{
		#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
		EXLogPrint( "ExpansionKillFeedModule::OnKilledByAnimal - Start" );
		#endif
		
		m_PlayerPrefix = GetPlayerPrefix( player.GetIdentity() );
		#ifdef JM_COT
		m_PlayerSteamWebhook = player.FormatSteamWebhook();			
		#endif
			
		m_DisplayName = "";
		m_DisplayName = source.ClassName();
		
		AnimalBase animal = AnimalBase.Cast(EntityAI.Cast(source));
		if(animal)
		{
			string icon;
			if( animal.IsKindOf( "Animal_UrsusArctos" ) )
			{
				icon = ExpansionIcons.GetPath("Bear");
			} else
			{
				icon = ExpansionIcons.GetPath("Claw");
			}
			
			KillFeedMessage( ExpansionKillFeedMessageType.ANIMAL, icon, m_PlayerPrefix, m_DisplayName );
			s_EventOnPlayerDeath.Invoke(ExpansionPlayerDeathType.ANIMAL, player, null, animal);
			DiscordMessage( ExpansionKillFeedMessageType.ANIMAL, m_PlayerSteamWebhook, m_DisplayName );
		}
		
		#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
		EXLogPrint( "ExpansionKillFeedModule::OnKilledByAnimal - End" );
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionKillFeedModule OnKilledUnknown
	// ------------------------------------------------------------	
	void OnKilledUnknown(PlayerBase player, Object source)
	{
		#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
		EXLogPrint( "ExpansionKillFeedModule::OnKilledUnknown - Start" );
		#endif
		
		m_PlayerPrefix = GetPlayerPrefix( player.GetIdentity() );
		#ifdef JM_COT
		m_PlayerSteamWebhook = player.FormatSteamWebhook();	
		#endif
			
		if (source)
		{
			m_DisplayName = "";
			m_DisplayName = source.ClassName();
			
			KillFeedMessage( ExpansionKillFeedMessageType.KILLED_UNKNOWN, ExpansionIcons.GetPath("Skull 1"), m_PlayerPrefix, m_DisplayName );
			s_EventOnPlayerDeath.Invoke(ExpansionPlayerDeathType.UNKNOWN, player, null, source);
			DiscordMessage( ExpansionKillFeedMessageType.KILLED_UNKNOWN, m_PlayerSteamWebhook, m_DisplayName );
		} else
		{
			KillFeedMessage( ExpansionKillFeedMessageType.DIED_UNKNOWN, ExpansionIcons.GetPath("Skull 1"), m_PlayerPrefix );
			s_EventOnPlayerDeath.Invoke(ExpansionPlayerDeathType.UNKNOWN, player, null, source);
			DiscordMessage( ExpansionKillFeedMessageType.DIED_UNKNOWN, m_PlayerSteamWebhook );
		}
			
		#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
		EXLogPrint( "ExpansionKillFeedModule::OnKilledUnknown - End" );
		#endif
	}

	// ------------------------------------------------------------
	// ExpansionKillFeedModule KillFeedMessage
	// Called on Server
	// ------------------------------------------------------------	
	private void KillFeedMessage( ExpansionKillFeedMessageType type, string icon, string param1 = "", string param2 = "", string param3 = "", string param4 = "")
	{
		#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
		EXLogPrint( "ExpansionKillFeedModule::Message - Start" );
		#endif
		
		if ( GetGame().IsServer() )
		{
			if( !KillFeedCheckServerSettings(type) )
				return;
	
			ExpansionKillFeedMessageMetaData kill_data = new ExpansionKillFeedMessageMetaData(type, icon, param1, param2, param3, param4);
			
			ScriptRPC message_rpc = new ScriptRPC();
			message_rpc.Write( kill_data );
			message_rpc.Send( null, ExpansionKillFeedModuleRPC.SendMessage, true );
				
			ExpansionLogKillfeed(kill_data);
		}

		#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
		EXLogPrint( "ExpansionKillFeedModule::Message - End" );
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionKillFeedModule KillFeedCheckServerSetting
	// ------------------------------------------------------------
	private bool KillFeedCheckServerSettings( ExpansionKillFeedMessageType type )
	{
		#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
		EXLogPrint( "ExpansionKillFeedModule::MessageServerSettingCheck - Start" );
		#endif

		switch ( type )
		{
			case ExpansionKillFeedMessageType.FALL:
				if( GetExpansionSettings().GetNotification().KillFeedFall )
					return true;
				break;
			case ExpansionKillFeedMessageType.CAR_HIT_DRIVER:
				if( GetExpansionSettings().GetNotification().KillFeedCarHitDriver )
					return true;
				break;
			case ExpansionKillFeedMessageType.CAR_HIT_NODRIVER:
				if( GetExpansionSettings().GetNotification().KillFeedCarHitNoDriver )
					return true;
				break;
			case ExpansionKillFeedMessageType.CAR_CRASH:
				if( GetExpansionSettings().GetNotification().KillFeedCarCrash )
					return true;
				break;
			case ExpansionKillFeedMessageType.CAR_CRASH_CREW:
				if( GetExpansionSettings().GetNotification().KillFeedCarCrashCrew )
					return true;
				break;
			case ExpansionKillFeedMessageType.HELI_HIT_DRIVER:
				if( GetExpansionSettings().GetNotification().KillFeedHeliHitDriver )
					return true;
				break;
			case ExpansionKillFeedMessageType.HELI_HIT_NODRIVER:
				if( GetExpansionSettings().GetNotification().KillFeedHeliHitNoDriver )
					return true;
				break;
			case ExpansionKillFeedMessageType.HELI_CRASH:
				if( GetExpansionSettings().GetNotification().KillFeedHeliCrash )
					return true;
				break;
			case ExpansionKillFeedMessageType.HELI_CRASH_CREW:
				if( GetExpansionSettings().GetNotification().KillFeedHeliCrashCrew )
					return true;
				break;
			/*case ExpansionKillFeedMessageType.BOAT_CRASH:
				if( GetExpansionSettings().GetNotification().KillFeedBoatCrash )
					return true;
				break;
			case ExpansionKillFeedMessageType.BOAT_CRASH_CREW:
				if( GetExpansionSettings().GetNotification().KillFeedBoatCrashCrew )
					return true;
				break;*/
			case ExpansionKillFeedMessageType.BARBEDWIRE:
				if( GetExpansionSettings().GetNotification().KillFeedBarbedWire )
					return true;
				break;
			case ExpansionKillFeedMessageType.FIRE:
				if( GetExpansionSettings().GetNotification().KillFeedFire )
					return true;
				break;
			/*case ExpansionKillFeedMessageType.SPECIAL_EXPLOSION:
				if( GetExpansionSettings().GetNotification().KillFeedSpecialExplosion )
					return true;
				break;*/
			case ExpansionKillFeedMessageType.WEAPON_EXPLOSION:
				if( GetExpansionSettings().GetNotification().KillFeedWeaponExplosion )
					return true;
				break;
			case ExpansionKillFeedMessageType.DEHYDRATION:
				if( GetExpansionSettings().GetNotification().KillFeedDehydration )
					return true;
				break;
			case ExpansionKillFeedMessageType.STARVATION:
				if( GetExpansionSettings().GetNotification().KillFeedStarvation )
					return true;
				break;
			case ExpansionKillFeedMessageType.BLEEDING:
				if( GetExpansionSettings().GetNotification().KillFeedBleeding )
					return true;
				break;
			case ExpansionKillFeedMessageType.SUICIDE:
				if( GetExpansionSettings().GetNotification().KillFeedSuicide )
					return true;
				break;
			case ExpansionKillFeedMessageType.WEAPON:
				if( GetExpansionSettings().GetNotification().KillFeedWeapon )
					return true;
				break;
			case ExpansionKillFeedMessageType.MELEWEAPON:
				if( GetExpansionSettings().GetNotification().KillFeedMeleeWeapon )
					return true;
				break;
			case ExpansionKillFeedMessageType.BAREHANDS:
				if( GetExpansionSettings().GetNotification().KillFeedBarehands )
					return true;
				break;
			case ExpansionKillFeedMessageType.INFECTED:
				if( GetExpansionSettings().GetNotification().KillFeedInfected )
					return true;
				break;
			case ExpansionKillFeedMessageType.ANIMAL:
				if( GetExpansionSettings().GetNotification().KillFeedAnimal )
					return true;
				break;
			case ExpansionKillFeedMessageType.KILLED_UNKNOWN:
				if( GetExpansionSettings().GetNotification().KillFeedKilledUnknown )
					return true;
				break;
			case ExpansionKillFeedMessageType.DIED_UNKNOWN:
				if( GetExpansionSettings().GetNotification().KillFeedDiedUnknown )
					return true;
				break;
		}

		return false;

		#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
		EXLogPrint( "ExpansionKillFeedModule::MessageServerSettingCheck - End" );
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionKillFeedModule DiscordMessage
	// ------------------------------------------------------------	
	private void DiscordMessage(ExpansionKillFeedMessageType type, string param1 = "", string param2 = "", string param3 = "", string param4 = "")
	{
		#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
		EXLogPrint( "ExpansionKillFeedModule::DiscordMessage - Start" );
		#endif
		
		if ( GetExpansionSettings().GetNotification().EnableKillFeedDiscordMsg )
		{
			#ifdef JM_COT

			JMWebhookDiscordMessage discord_message = m_Webhook.CreateDiscordMessage();
	   		JMWebhookDiscordEmbed discord_embed = discord_message.GetEmbed();
	
			string message = SetMessage(type);
						
			string path;
			if ( IsClassName( param1, path ) )
			{
				GetGame().ConfigGetText( path + " " + param1 + " displayName", param1 );
			}
			
			if ( IsClassName( param2, path ) )
			{
				GetGame().ConfigGetText( path + " " + param2 + " displayName", param2 );
			}
			
			if ( IsClassName( param3, path ) )
			{
				GetGame().ConfigGetText( path + " " + param3 + " displayName", param3 );
			}
			
			if ( IsClassName( param4, path ) )
			{
				GetGame().ConfigGetText( path + " " + param4 + " displayName", param4 );
			}

			StringLocaliser localizer = new StringLocaliser( message, "\"" + param1 + "\"", "\"" + param2 + "\"", "\"" + param3 + "\"", "\"" + param4 + "\"" );		
			
			#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
			EXLogPrint( "ExpansionKillFeedModule::DiscordMessage - Localizer text: " + localizer.Format() );
			#endif
			
			discord_embed.SetColor( 16711680 ); // 0xFF0000
			discord_embed.SetAuthor( "DayZ Expansion", "https://steamcommunity.com/sharedfiles/filedetails/?id=2116151222", "https://cdn.cloudflare.steamstatic.com/steamcommunity/public/images/avatars/98/980723d8611aa9b8b71eca92e5f911167489a785_full.jpg" );
			discord_embed.AddField( "Kill-Feed", localizer.Format() );
			
			m_Webhook.Post( "Killfeed", discord_message );

			#else

			Error("Killfeed Discord message requires COT (Community Online Tools)");

			#endif
		}
		
		#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
		EXLogPrint( "ExpansionKillFeedModule::DiscordMessage - End" );
		#endif
	}
		
	// ------------------------------------------------------------
	// ExpansionKillFeedModule SetMessage
	// ------------------------------------------------------------	
	private string SetMessage(ExpansionKillFeedMessageType type)
	{
		#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
		EXLogPrint( "ExpansionKillFeedModule::SetMessage - Start" );
		#endif
		
		string message;
		switch ( type )
		{
			case ExpansionKillFeedMessageType.FALL:
				message = "STR_EXPANSION_KILLFEED_PLAYER_DIED_FALLING";
				break;
			case ExpansionKillFeedMessageType.CAR_HIT_DRIVER:
				message = "STR_EXPANSION_KILLFEED_PLAYER_KILLED_CRASH_CAR";
				break;
			case ExpansionKillFeedMessageType.CAR_HIT_NODRIVER:
				message = "STR_EXPANSION_KILLFEED_PLAYER_CRASHED_CAR";
				break;
			case ExpansionKillFeedMessageType.CAR_CRASH:
				message = "STR_EXPANSION_KILLFEED_PLAYER_CRASHED_CAR";
				break;
			case ExpansionKillFeedMessageType.CAR_CRASH_CREW:
				message = "%1 cant drive a %2 and exploded. Killing %3";
				break;
			case ExpansionKillFeedMessageType.HELI_HIT_DRIVER:
				message = "STR_EXPANSION_KILLFEED_PLAYER_KILLED_CRASH_HELI";
				break;
			case ExpansionKillFeedMessageType.HELI_HIT_NODRIVER:
				message = "STR_EXPANSION_KILLFEED_PLAYER_CRASHED_HELI";
				break;
			case ExpansionKillFeedMessageType.HELI_CRASH:
				message = "STR_EXPANSION_KILLFEED_PLAYER_CRASHED_HELI";
				break;
			case ExpansionKillFeedMessageType.HELI_CRASH_CREW:
				message = "%1 cant fly a %2 and exploded. Killing %3";
				break;
			case ExpansionKillFeedMessageType.BOAT_CRASH:
				message = "%1 crashed with his %2 boat.";
				break;
			case ExpansionKillFeedMessageType.BOAT_CRASH_CREW:
				message = "%1 cant drive a %2 and exploded. Killing %3";
				break;
			case ExpansionKillFeedMessageType.BARBEDWIRE:
				message = "STR_EXPANSION_KILLFEED_PLAYER_BARBED_WIRE";
				break;
			case ExpansionKillFeedMessageType.FIRE:
				message = "STR_EXPANSION_KILLFEED_PLAYER_DIED_FIRE";
				break;
			/*case ExpansionKillFeedMessageType.SPECIAL_EXPLOSION:
				message = "STR_EXPANSION_KILLFEED_PLAYER_DIED_EXPLOSION_WEAPON";
				break;*/
			case ExpansionKillFeedMessageType.WEAPON_EXPLOSION:
				message = "STR_EXPANSION_KILLFEED_PLAYER_DIED_EXPLOSION";
				break;
			case ExpansionKillFeedMessageType.DEHYDRATION:
				message = "STR_EXPANSION_KILLFEED_PLAYER_DIED_DEHYDRATION";
				break;
			case ExpansionKillFeedMessageType.STARVATION:
				message = "STR_EXPANSION_KILLFEED_PLAYER_DIED_STARVATION";
				break;
			case ExpansionKillFeedMessageType.BLEEDING:
				message = "STR_EXPANSION_KILLFEED_PLAYER_DIED_BLEEDING";
				break;
			case ExpansionKillFeedMessageType.SUICIDE:
				message = "STR_EXPANSION_KILLFEED_PLAYER_DIED_SUICIDE";
				break;
			case ExpansionKillFeedMessageType.WEAPON:
				message = "STR_EXPANSION_KILLFEED_PLAYER_KILLED_PLAYER_WEAPON";
				break;
			case ExpansionKillFeedMessageType.MELEWEAPON:
				message = "STR_EXPANSION_KILLFEED_PLAYER_KILLED_PLAYER_MELE";
				break;
			case ExpansionKillFeedMessageType.BAREHANDS:
				message = "STR_EXPANSION_KILLFEED_PLAYER_KILLED_PLAYE_BAREHANDS";
				break;
			case ExpansionKillFeedMessageType.INFECTED:
				message = "STR_EXPANSION_KILLFEED_PLAYER_KILLED_ZOMBIE";
				break;
			case ExpansionKillFeedMessageType.ANIMAL:
				message = "STR_EXPANSION_KILLFEED_PLAYER_KILLED_ANIMAL";
				break;
			case ExpansionKillFeedMessageType.KILLED_UNKNOWN:
				message = "STR_EXPANSION_KILLFEED_PLAYER_KILLED_UNKNOWN";
				break;
			case ExpansionKillFeedMessageType.DIED_UNKNOWN:
				message = "STR_EXPANSION_KILLFEED_PLAYER_DIED_UNKNOWN";
				break;
		}
		
		#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
		EXLogPrint( "ExpansionKillFeedModule::SetMessage - End and message: " + message);
		#endif
		
		return message;
	}
	
	// ------------------------------------------------------------
	// ExpansionKillFeedModule GetPlayerPrefix
	// ------------------------------------------------------------	
	private string GetPlayerPrefix( PlayerIdentity identity )  // player name
	{
		if ( identity == NULL )
			return "UNKNOWN PLAYER";

		return identity.GetName();
	}
		
	// ------------------------------------------------------------
	// ExpansionKillFeedModule GetRPCMin
	// ------------------------------------------------------------	
	override int GetRPCMin()
	{
		return ExpansionKillFeedModuleRPC.INVALID;
	}
	
	// ------------------------------------------------------------
	// ExpansionKillFeedModule GetRPCMax
	// ------------------------------------------------------------
	override int GetRPCMax()
	{
		return ExpansionKillFeedModuleRPC.COUNT;
	}
	
	// ------------------------------------------------------------
	// ExpansionKillFeedModule OnRPC
	// ------------------------------------------------------------
	override void OnRPC(Class sender, CF_EventArgs args)
	{
		super.OnRPC(sender, args);

		auto rpc = CF_EventRPCArgs.Cast(args);

		switch ( rpc.ID )
		{
		case ExpansionKillFeedModuleRPC.SendMessage:
			RPC_SendMessage( rpc.Sender, rpc.Context );
			break;
		}
	}
	
	// ------------------------------------------------------------
	// ExpansionKillFeedModule RPC_SendMessage
	// Called on all Clients
	// ------------------------------------------------------------
	private void RPC_SendMessage(PlayerIdentity sender, ParamsReadContext ctx )
	{
		#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
		EXLogPrint( "ExpansionKillFeedModule::RPC_SendMessage - Start" );
		#endif
		
		ExpansionKillFeedMessageMetaData kill_data = new ExpansionKillFeedMessageMetaData( ExpansionKillFeedMessageType.UNKNOWN, "" );
		ctx.Read(kill_data);
		
		if (kill_data)
		{
			if ( GetExpansionSettings().GetNotification().KillFeedMessageType == ExpansionAnnouncementType.NOTIFICATION )
			{
				ExpansionNotification("STR_EXPANSION_KILLFEED_TITLE", GetKillMessage(kill_data), kill_data.Icon, COLOR_EXPANSION_NOTIFICATION_EXPANSION, 5, ExpansionNotificationType.KILLFEED).Create();
			} else if ( GetExpansionSettings().GetNotification().KillFeedMessageType == ExpansionAnnouncementType.CHAT )
			{
				GetGame().GetMission().OnEvent( ChatMessageEventTypeID, new ChatMessageEventParams( ExpansionChatChannels.CCSystem, "", "#STR_EXPANSION_KILLFEED_TITLE" + " - " + GetKillMessage(kill_data).Format(), "" ) );
			}
		}
		
		#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
		EXLogPrint( "ExpansionKillFeedModule::RPC_SendMessage - End" );
		#endif
	}
	
	// ------------------------------------------------------------
	// ExpansionKillFeedModule IsClassName
	// ------------------------------------------------------------
	private bool IsClassName(string classname, out string config_path)
	{
		string path = "cfgVehicles";
		if ( GetGame().ConfigIsExisting( path + " " + classname ) )
		{
			config_path = "cfgVehicles";
			return true;
		}
		
		path = "cfgWeapons";
		if ( GetGame().ConfigIsExisting( path + " " + classname ) )
		{
			config_path = "cfgWeapons";
			return true;
		}
		
		path = "cfgMagazines";
		if ( GetGame().ConfigIsExisting( path + " " + classname ) )
		{
			config_path = "cfgMagazines";
			return true;
		}
		
		path = "cfgNonAIVehicles";
		if ( GetGame().ConfigIsExisting( path + " " + classname ) )
		{
			config_path = "cfgNonAIVehicles";
			return true;
		}
		
		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionKillFeedModule GetKillMessage
	// ------------------------------------------------------------	
	private StringLocaliser GetKillMessage(ExpansionKillFeedMessageMetaData kill_data)
	{
			if (!kill_data)
				return NULL;
			
		string path;
		if ( IsClassName( kill_data.FeedParam1, path ) )
		{
			if (path != string.Empty)
				GetGame().ConfigGetText( path + " " + kill_data.FeedParam1 + " displayName", kill_data.FeedParam2 );
		}
		
		#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
		EXLogPrint("ExpansionKillFeedModule::GetKillMessage - kill_data.FeedParam1: " + kill_data.FeedParam1);
		#endif
		
		if ( IsClassName( kill_data.FeedParam2, path ) )
		{
			if (path != string.Empty)
				GetGame().ConfigGetText( path + " " + kill_data.FeedParam2 + " displayName", kill_data.FeedParam2 );
		}
			
		#ifdef EXPANSION_KILLFEED_MODULE_DEBUG	
		EXLogPrint("ExpansionKillFeedModule::GetKillMessage - kill_data.FeedParam2: " + kill_data.FeedParam2);
		#endif

		if ( IsClassName( kill_data.FeedParam3, path ) )
		{
			if (path != string.Empty)
				GetGame().ConfigGetText( path + " " + kill_data.FeedParam3 + " displayName", kill_data.FeedParam3 );
		}
			
		#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
		EXLogPrint("ExpansionKillFeedModule::GetKillMessage - kill_data.FeedParam3: " + kill_data.FeedParam3);
		#endif
		
		if ( IsClassName( kill_data.FeedParam4, path ) )
		{
			if (path != string.Empty)
				GetGame().ConfigGetText( path + " " + kill_data.FeedParam4 + " displayName", kill_data.FeedParam4 );
		}
		
		#ifdef EXPANSION_KILLFEED_MODULE_DEBUG		
		EXLogPrint("ExpansionKillFeedModule::GetKillMessage - kill_data.FeedParam4: " + kill_data.FeedParam4);
		#endif
			
		return new StringLocaliser(kill_data.Message, kill_data.FeedParam1, kill_data.FeedParam2, kill_data.FeedParam3, kill_data.FeedParam4);
	}
		
	// ------------------------------------------------------------
	// ExpansionKillFeedModule GetKillLogMessage
	// ------------------------------------------------------------	
	private string GetKillLogMessage(ExpansionKillFeedMessageMetaData kill_data)
	{
			if (!kill_data)
				return "";
			
		string path;
		if ( IsClassName( kill_data.FeedParam1, path ) )
		{
			if (path != string.Empty)
				GetGame().ConfigGetText( path + " " + kill_data.FeedParam1 + " displayName", kill_data.FeedParam2 );
		}
		
		#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
		EXLogPrint("ExpansionKillFeedModule::GetKillLogMessage - kill_data.FeedParam1: " + kill_data.FeedParam1);
		#endif
		
		if ( IsClassName( kill_data.FeedParam2, path ) )
		{
			if (path != string.Empty)
				GetGame().ConfigGetText( path + " " + kill_data.FeedParam2 + " displayName", kill_data.FeedParam2 );
		}
			
		#ifdef EXPANSION_KILLFEED_MODULE_DEBUG	
		EXLogPrint("ExpansionKillFeedModule::GetKillLogMessage - kill_data.FeedParam2: " + kill_data.FeedParam2);
		#endif

		if ( IsClassName( kill_data.FeedParam3, path ) )
		{
			if (path != string.Empty)
				GetGame().ConfigGetText( path + " " + kill_data.FeedParam3 + " displayName", kill_data.FeedParam3 );
		}
			
		#ifdef EXPANSION_KILLFEED_MODULE_DEBUG
		EXLogPrint("ExpansionKillFeedModule::GetKillLogMessage - kill_data.FeedParam3: " + kill_data.FeedParam3);
		#endif
		
		if ( IsClassName( kill_data.FeedParam4, path ) )
		{
			if (path != string.Empty)
				GetGame().ConfigGetText( path + " " + kill_data.FeedParam4 + " displayName", kill_data.FeedParam4 );
		}
		
		#ifdef EXPANSION_KILLFEED_MODULE_DEBUG		
		EXLogPrint("ExpansionKillFeedModule::GetKillLogMessage - kill_data.FeedParam4: " + kill_data.FeedParam4);
		#endif
			
		return Widget.TranslateString( string.Format(kill_data.Message, kill_data.FeedParam1, kill_data.FeedParam2, kill_data.FeedParam3, kill_data.FeedParam4) );
	}
		
	// ------------------------------------------------------------
	// ExpansionKillFeedModule ExpansionLogKillfeed
	// ------------------------------------------------------------
	private void ExpansionLogKillfeed(ExpansionKillFeedMessageMetaData kill_data)
	{
		if (!kill_data)
			return;

		if (GetExpansionSettings().GetLog().Killfeed)
			GetExpansionSettings().GetLog().PrintLog("[Killfeed] " + GetKillLogMessage(kill_data));
	}
}
