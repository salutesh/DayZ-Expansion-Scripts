/**
 * MissionServer.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		MissionServer
 * @brief		
 **/
modded class MissionServer
{
	ref ExpansionStartingClothing m_ExpansionClothing;
	
	int m_topexp = -1;
	int m_shoesexp = -1;
	int m_bottomexp = -1;
	int m_headgearexp = -1;
	int m_glassesexp = -1;
	int m_maskexp = -1;
	int m_vestexp = -1;
	int m_glovesexp = -1;
	int m_beltexp = -1;
	int m_bandexp = -1;
	int m_backpackexp = -1;
	
	// ------------------------------------------------------------
	// MissionServer Constructor
	// ------------------------------------------------------------
	void MissionServer()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionServer::MissionServer - Start");
		#endif

		CreateDayZExpansion();

		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionServer::MissionServer - End");
		#endif
	}

	// ------------------------------------------------------------
	// MissionServer Destructor
	// ------------------------------------------------------------
	void ~MissionServer()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionServer::~MissionServer - Start");
		#endif
		
		DestroyDayZExpansion();
		DestroyNotificationSystem();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionServer::~MissionServer - End");
		#endif
	}

	// ------------------------------------------------------------
	// OnMissionStart
	// ------------------------------------------------------------
	override void OnMissionStart()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionServer::OnMissionStart - Start");
		#endif
		
		super.OnMissionStart();
		
		GetDayZExpansion().OnMissionStart();	

		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionServer::OnMissionStart - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// OnMissionLoaded
	// ------------------------------------------------------------
	override void OnMissionLoaded()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionServer::OnMissionLoaded - Start");
		#endif
		
		super.OnMissionLoaded();
		
		GetDayZExpansion().OnMissionLoaded();

		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionServer::OnMissionLoaded - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// OnMissionFinish
	// ------------------------------------------------------------
	override void OnMissionFinish()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionServer::OnMissionFinish - Start");
		#endif
	   
		super.OnMissionFinish();

		//! Save settings on mission finish
		g_exGlobalSettings.Save();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionServer::OnMissionFinish - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// OnClientReconnectEvent
	// ------------------------------------------------------------
	override void OnClientReconnectEvent( PlayerIdentity identity, PlayerBase player )
	{
		super.OnClientReconnectEvent( identity, player );

		g_exGlobalSettings.Send( identity );
	}	
	
	// ------------------------------------------------------------
	// InvokeOnConnect
	// ------------------------------------------------------------
	override void InvokeOnConnect( PlayerBase player, PlayerIdentity identity )
	{
		g_exGlobalSettings.Send( identity );
		
		PlayerBase.AddPlayer( player );
		
		//! Do after, because some modules use PlayerIdentity setup inside AddPlayer of PlayerBase class
		super.InvokeOnConnect( player, identity );
	}

	// ------------------------------------------------------------
	// PlayerDisconnected
	// ------------------------------------------------------------
	override void PlayerDisconnected( PlayerBase player, PlayerIdentity identity, string uid )
	{
		PlayerBase.RemovePlayer( uid );
		
		super.PlayerDisconnected( player, identity, uid );
	}
	
	// ------------------------------------------------------------
	// OnClientNewEvent
	// ------------------------------------------------------------
	override PlayerBase OnClientNewEvent(PlayerIdentity identity, vector pos, ParamsReadContext ctx)
	{
		if (GetExpansionSettings().GetSpawn().UseCustomClothing)
		{
			#ifdef EXPANSIONEXPRINT
			EXPrint("MissionServer::OnClientNewEvent - UseCustomClothing == TRUE");
			#endif
			
			string characterName;
			
			if (m_skin == -1)
			{
				characterName = GetGame().CreateRandomPlayer();
			}
			else
			{
				characterName = GetGame().ListAvailableCharacters().Get(m_skin);
			}
			
			if (CreateCharacter(identity, pos, ctx, characterName))
			{
				#ifdef EXPANSIONEXPRINT
				EXPrint("MissionServer::OnClientNewEvent - Equip character with custom clothing!");
				#endif
				EquipCharacter();
			}
		}
		else
		{
			#ifdef EXPANSIONEXPRINT
			EXPrint("MissionServer::OnClientNewEvent - UseCustomClothing == FALSE");
			#endif
			super.OnClientNewEvent(identity, pos, ctx);
		}
		
		return m_player;
	}
	
	// ------------------------------------------------------------
	// Override EquipCharacter
	// ------------------------------------------------------------
	override void EquipCharacter()
	{
		if ( GetExpansionSettings().GetSpawn().UseCustomClothing )
		{
			#ifdef EXPANSIONEXPRINT
			EXPrint("MissionServer::EquipCharacter - UseCustomClothing == TRUE");
			#endif
			m_ExpansionClothing = GetExpansionSettings().GetSpawn().StartingClothing;
			if (m_ExpansionClothing)
			{
				if (m_ExpansionClothing.Headgear.Count() > 0)
				{
					if (m_ExpansionClothing.Headgear.Count() == 1)
					{
						m_headgearexp = 0;
					}
					else if (m_ExpansionClothing.Headgear.Count() > 1)
					{
						m_headgearexp = Math.RandomIntInclusive( 0, m_ExpansionClothing.Headgear.Count() );
					}
					
					#ifdef EXPANSIONEXPRINT
					EXPrint("MissionServer::EquipCharacter - m_headgearexp: " + m_headgearexp.ToString());
					#endif
				}
				
				if (m_ExpansionClothing.Glasses.Count() > 0)
				{
					if (m_ExpansionClothing.Glasses.Count() == 1)
					{
						m_glassesexp = 0;
					}
					else if (m_ExpansionClothing.Glasses.Count() > 1)
					{
						m_glassesexp = Math.RandomIntInclusive( 0, m_ExpansionClothing.Glasses.Count() );
					}
					
					#ifdef EXPANSIONEXPRINT
					EXPrint("MissionServer::EquipCharacter - m_glassesexp: " + m_glassesexp.ToString());
					#endif
				}
				
				if (m_ExpansionClothing.Masks.Count() > 0)
				{
					if (m_ExpansionClothing.Masks.Count() == 1)
					{
						m_maskexp = 0;
					}
					else if (m_ExpansionClothing.Masks.Count() > 1)
					{
						m_maskexp = Math.RandomIntInclusive( 0, m_ExpansionClothing.Masks.Count() );
					}
					
					#ifdef EXPANSIONEXPRINT
					EXPrint("MissionServer::EquipCharacter - m_maskexp: " + m_maskexp.ToString());
					#endif
				}
				
				if (m_ExpansionClothing.Tops.Count() > 0)
				{
					if (m_ExpansionClothing.Tops.Count() == 1)
					{
						m_topexp = 0;
					}
					else if (m_ExpansionClothing.Tops.Count() > 1)
					{
						m_topexp = Math.RandomIntInclusive( 0, m_ExpansionClothing.Tops.Count() );
					}
					
					#ifdef EXPANSIONEXPRINT
					EXPrint("MissionServer::EquipCharacter - m_topexp: " + m_topexp.ToString());
					#endif
				}
				
				if (m_ExpansionClothing.Vests.Count() > 0)
				{
					if (m_ExpansionClothing.Vests.Count() == 1)
					{
						m_vestexp = 0;
					}
					else if (m_ExpansionClothing.Vests.Count() > 1)
					{
						m_vestexp = Math.RandomIntInclusive( 0, m_ExpansionClothing.Vests.Count() );
					}
					
					#ifdef EXPANSIONEXPRINT
					EXPrint("MissionServer::EquipCharacter - m_vestexp: " + m_vestexp.ToString());
					#endif
				}
				
				if (m_ExpansionClothing.Gloves.Count() > 0)
				{
					if (m_ExpansionClothing.Gloves.Count() == 1)
					{
						m_glovesexp = 0;
					}
					else if (m_ExpansionClothing.Gloves.Count() > 1)
					{
						m_glovesexp = Math.RandomIntInclusive( 0, m_ExpansionClothing.Gloves.Count() );
					}
					
					#ifdef EXPANSIONEXPRINT
					EXPrint("MissionServer::EquipCharacter - m_glovesexp: " + m_glovesexp.ToString());
					#endif
				}
				
				if (m_ExpansionClothing.Pants.Count() > 0)
				{
					if (m_ExpansionClothing.Pants.Count() == 1)
					{
						m_bottomexp = 0;
					}
					else if (m_ExpansionClothing.Pants.Count() > 1)
					{
						m_bottomexp = Math.RandomIntInclusive( 0, m_ExpansionClothing.Pants.Count() );
					}
					
					#ifdef EXPANSIONEXPRINT
					EXPrint("MissionServer::EquipCharacter - m_bottomexp: " + m_bottomexp.ToString());
					#endif
				}
				
				if (m_ExpansionClothing.Belts.Count() > 0)
				{
					if (m_ExpansionClothing.Belts.Count() == 1)
					{
						m_beltexp = 0;
					}
					else if (m_ExpansionClothing.Belts.Count() > 1)
					{
						m_beltexp = Math.RandomIntInclusive( 0, m_ExpansionClothing.Belts.Count() );
					}
					
					#ifdef EXPANSIONEXPRINT
					EXPrint("MissionServer::EquipCharacter - m_beltexp: " + m_beltexp.ToString());
					#endif
				}
				
				if (m_ExpansionClothing.Shoes.Count() > 0)
				{
					if (m_ExpansionClothing.Shoes.Count() == 1)
					{
						m_shoesexp = 0;
					}
					else if (m_ExpansionClothing.Shoes.Count() > 1)
					{
						m_shoesexp = Math.RandomIntInclusive( 0, m_ExpansionClothing.Shoes.Count() );
					}
					
					#ifdef EXPANSIONEXPRINT
					EXPrint("MissionServer::EquipCharacter - m_shoesexp: " + m_shoesexp.ToString());
					#endif
				}
				
				if (m_ExpansionClothing.Armbands.Count() > 0)
				{
					if (m_ExpansionClothing.Armbands.Count() == 1)
					{
						m_bandexp = 0;
					}
					else if (m_ExpansionClothing.Armbands.Count() > 1)
					{
						m_bandexp = Math.RandomIntInclusive( 0, m_ExpansionClothing.Armbands.Count() );
					}
					
					#ifdef EXPANSIONEXPRINT
					EXPrint("MissionServer::EquipCharacter - m_bandexp: " + m_bandexp.ToString());
					#endif
				}
				
				if (m_ExpansionClothing.Backpacks.Count() > 0)
				{
					if (m_ExpansionClothing.Backpacks.Count() == 1)
					{
						m_backpackexp = 0;
					}
					else if (m_ExpansionClothing.Backpacks.Count() > 1)
					{
						m_backpackexp = Math.RandomIntInclusive( 0, m_ExpansionClothing.Backpacks.Count() );
					}
					
					#ifdef EXPANSIONEXPRINT
					EXPrint("MissionServer::EquipCharacter - m_backpackexp: " + m_backpackexp.ToString());
					#endif
				}
				
				if (m_headgearexp != -1)
				{
					#ifdef EXPANSIONEXPRINT
					EXPrint("MissionServer::EquipCharacter - m_ExpansionClothing.Headgear: " + m_ExpansionClothing.Headgear.Get( m_headgearexp ));
					#endif
					m_player.GetInventory().CreateInInventory( m_ExpansionClothing.Headgear.Get( m_headgearexp ) );
				}
				
				if (m_glassesexp != -1)
				{
					#ifdef EXPANSIONEXPRINT
					EXPrint("MissionServer::EquipCharacter - m_ExpansionClothing.Glasses: " + m_ExpansionClothing.Glasses.Get( m_glassesexp ));
					#endif
					m_player.GetInventory().CreateInInventory( m_ExpansionClothing.Glasses.Get( m_glassesexp ) );
				}
				
				if (m_maskexp != -1)
				{
					#ifdef EXPANSIONEXPRINT
					EXPrint("MissionServer::EquipCharacter - m_ExpansionClothing.Masks: " + m_ExpansionClothing.Masks.Get( m_maskexp ));
					#endif
					m_player.GetInventory().CreateInInventory( m_ExpansionClothing.Masks.Get( m_maskexp ) );
				}
				
				if (m_topexp != -1)
				{
					#ifdef EXPANSIONEXPRINT
					EXPrint("MissionServer::EquipCharacter - m_ExpansionClothing.Tops: " + m_ExpansionClothing.Tops.Get( m_topexp ));
					#endif
					m_player.GetInventory().CreateInInventory( m_ExpansionClothing.Tops.Get( m_topexp ) );
				}
				
				if (m_vestexp != -1)
				{
					#ifdef EXPANSIONEXPRINT
					EXPrint("MissionServer::EquipCharacter - m_ExpansionClothing.Vests: " + m_ExpansionClothing.Vests.Get( m_vestexp ));
					#endif
					m_player.GetInventory().CreateInInventory( m_ExpansionClothing.Vests.Get( m_vestexp ) );
				}
				
				if (m_glovesexp != -1)
				{
					#ifdef EXPANSIONEXPRINT
					EXPrint("MissionServer::EquipCharacter - m_ExpansionClothing.Gloves: " + m_ExpansionClothing.Gloves.Get( m_glovesexp ));
					#endif
					m_player.GetInventory().CreateInInventory( m_ExpansionClothing.Gloves.Get( m_glovesexp ) );
				}
				
				if (m_bottomexp != -1)
				{
					#ifdef EXPANSIONEXPRINT
					EXPrint("MissionServer::EquipCharacter - m_ExpansionClothing.Pants: " + m_ExpansionClothing.Pants.Get( m_bottomexp ));
					#endif
					m_player.GetInventory().CreateInInventory( m_ExpansionClothing.Pants.Get( m_bottomexp ) );
				}
				
				if (m_beltexp != -1)
				{
					#ifdef EXPANSIONEXPRINT
					EXPrint("MissionServer::EquipCharacter - m_ExpansionClothing.Belts: " + m_ExpansionClothing.Belts.Get( m_beltexp ));
					#endif
					m_player.GetInventory().CreateInInventory( m_ExpansionClothing.Belts.Get( m_beltexp ) );
				}
				
				if (m_shoesexp != -1)
				{
					#ifdef EXPANSIONEXPRINT
					EXPrint("MissionServer::EquipCharacter - m_ExpansionClothing.Shoes: " + m_ExpansionClothing.Shoes.Get( m_shoesexp ));
					#endif
					m_player.GetInventory().CreateInInventory( m_ExpansionClothing.Shoes.Get( m_shoesexp ) );
				}
				
				if (m_bandexp != -1)
				{
					#ifdef EXPANSIONEXPRINT
					EXPrint("MissionServer::EquipCharacter - m_ExpansionClothing.Armbands: " + m_ExpansionClothing.Armbands.Get( m_bandexp ));
					#endif
					m_player.GetInventory().CreateInInventory( m_ExpansionClothing.Armbands.Get( m_bandexp ) );
				}
				
				if (m_backpackexp != -1)
				{
					#ifdef EXPANSIONEXPRINT
					EXPrint("MissionServer::EquipCharacter - m_ExpansionClothing.Backpacks: " + m_ExpansionClothing.Backpacks.Get( m_backpackexp ));
					#endif
					m_player.GetInventory().CreateInInventory( m_ExpansionClothing.Backpacks.Get( m_backpackexp ) );
				}
			}
			
			StartingEquipSetup(m_player, false);
		}
		else
		{
			#ifdef EXPANSIONEXPRINT
			EXPrint("MissionServer::EquipCharacter - UseCustomClothing == FALSE");
			#endif
			
			EntityAI item;
			EntityAI item2;
			EntityAI item3;
			
			//! Creates clothes from DayZIntroScene's m_demoUnit
			if ( m_top != -1 && m_bottom != -1 && m_shoes != -1 /*&& m_skin != -1*/ )
			{
				item = m_player.GetInventory().CreateInInventory( topsArray.Get( m_top ) );
				item2 = m_player.GetInventory().CreateInInventory( pantsArray.Get( m_bottom ) );
				item3 = m_player.GetInventory().CreateInInventory( shoesArray.Get( m_shoes ) );
				
				StartingEquipSetup(m_player, true);
			}
			//! Creates random starting clothes - fallback
			else
			{
				item = m_player.GetInventory().CreateInInventory( topsArray.GetRandomElement() );
				item2 = m_player.GetInventory().CreateInInventory( pantsArray.GetRandomElement() );
				item3 = m_player.GetInventory().CreateInInventory( shoesArray.GetRandomElement() );
				
				StartingEquipSetup(m_player, false);
			}
		}
	}

	// ------------------------------------------------------------
	// SetStartingGear
	// ------------------------------------------------------------
	void SetStartingGear(PlayerBase player)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionServer::SetStartingGear - Start");
		#endif

		int i;
		
		if ( GetExpansionSettings() && GetExpansionSettings().GetSpawn() )
		{
			ref ExpansionStartingGear gear = GetExpansionSettings().GetSpawn().StartingGear;
			
			if ( gear.UsingUpperGear && gear.UpperGear )
			{
				EntityAI itemTop = player.FindAttachmentBySlotName("Body");
				
				string classNameTop = itemTop.ClassName();
				if ( classNameTop != "" )
				{
					GetGame().ObjectDelete( itemTop );
					itemTop = player.GetInventory().CreateInInventory( classNameTop );
				}
				
				if ( itemTop )
				{
					for ( i = 0; i < gear.UpperGear.Count(); i++ )
					{
						itemTop.GetInventory().CreateInInventory( gear.UpperGear[i] );
					}
				}
			}
			
			if ( gear.UsingPantsGear && gear.PantsGear )
			{
				EntityAI itemPants = player.FindAttachmentBySlotName("Pants");
				
				string classNamePants = itemPants.ClassName();
				if ( classNamePants != "" )
				{
					GetGame().ObjectDelete( itemTop );
					itemPants = player.GetInventory().CreateInInventory( classNamePants );
				}
				
				if ( itemPants )
				{					
					for ( i = 0; i < gear.PantsGear.Count(); i++ )
					{
						itemPants.GetInventory().CreateInInventory( gear.PantsGear[i] );
					}
				}
			}
			
			if ( gear.UsingBackpackGear && gear.BackpackGear )
			{
				EntityAI itemBag = player.FindAttachmentBySlotName("Backpack");
				if ( itemBag )
				{					
					for ( i = 0; i < gear.BackpackGear.Count(); i++ )
					{
						itemBag.GetInventory().CreateInInventory( gear.BackpackGear[i] );
					}
				}
			}
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("MissionServer::SetStartingGear - End");
		#endif
	}
	

	// ------------------------------------------------------------
	// Override HandleBody
	// ------------------------------------------------------------
	override void HandleBody(PlayerBase player)
	{
		player.SetAllowDamage(true);
		
		super.HandleBody(player);
	}
	
	// ------------------------------------------------------------
	// Override OnClientRespawnEvent
	// ------------------------------------------------------------
	override void OnClientRespawnEvent(PlayerIdentity identity, PlayerBase player)
	{
		if (player)
			player.SetAllowDamage(true);
		
		super.OnClientRespawnEvent(identity, player);
	}
}
