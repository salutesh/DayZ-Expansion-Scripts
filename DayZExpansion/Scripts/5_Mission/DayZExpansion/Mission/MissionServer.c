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
		if (GetExpansionSettings() && GetExpansionSettings().GetSpawn().StartingClothing.UseCustomClothing )
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
	// SetRandomHealth
	// ------------------------------------------------------------
	void SetRandomHealth(EntityAI itemEnt)
	{
		if ( itemEnt )
		{
			int rndHlt = Math.RandomInt(55,100);
			itemEnt.SetHealth("","",rndHlt);
		}
	}
	
	// ------------------------------------------------------------
	// Override StartingEquipSetup
	// ------------------------------------------------------------
	override void StartingEquipSetup(PlayerBase player, bool clothesChosen)
	{
		if ( GetExpansionSettings() && GetExpansionSettings().GetSpawn().StartingGear.UseStartingGear )
		{
			SetStartingGear(player);
		}
		else
		{
			EntityAI itemClothing;
			EntityAI itemEnt;
			ItemBase itemBs;
			float rand;

			itemClothing = player.FindAttachmentBySlotName( "Body" );
			if ( itemClothing )
			{
				SetRandomHealth( itemClothing );
				
				itemEnt = itemClothing.GetInventory().CreateInInventory( "Rag" );
				if ( Class.CastTo( itemBs, itemEnt ) )
					itemBs.SetQuantity( 4 );

				SetRandomHealth( itemEnt );

				string chemlightArray[] = { "Chemlight_White", "Chemlight_Yellow", "Chemlight_Green", "Chemlight_Red" };
				int rndIndex = Math.RandomInt( 0, 4 );
				itemEnt = itemClothing.GetInventory().CreateInInventory( chemlightArray[rndIndex] );
				SetRandomHealth( itemEnt );

				rand = Math.RandomFloatInclusive( 0.0, 1.0 );
				if ( rand < 0.35 )
					itemEnt = player.GetInventory().CreateInInventory( "Apple" );
				else if ( rand > 0.65 )
					itemEnt = player.GetInventory().CreateInInventory( "Pear" );
				else
					itemEnt = player.GetInventory().CreateInInventory( "Plum" );

				SetRandomHealth( itemEnt );
			}
			
			itemClothing = player.FindAttachmentBySlotName( "Legs" );
			if ( itemClothing )
				SetRandomHealth( itemClothing );
			
			itemClothing = player.FindAttachmentBySlotName( "Feet" );
			if ( itemClothing )
				SetRandomHealth( itemClothing );
		}
	}
	
	bool ExpansionEquipCharacter(PlayerBase player)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZExpansion::EquipCharacter - UseCustomClothing == TRUE");
		#endif
		
		ExpansionStartingClothing startingClothing;
		if (Class.CastTo(startingClothing, GetExpansionSettings().GetSpawn().StartingClothing))
		{
			player.GetInventory().CreateAttachmentEx(startingClothing.Headgear.GetRandomElement(), InventorySlots.HEADGEAR);
			player.GetInventory().CreateAttachmentEx(startingClothing.Glasses.GetRandomElement(), InventorySlots.EYEWEAR);
			player.GetInventory().CreateAttachmentEx(startingClothing.Masks.GetRandomElement(), InventorySlots.MASK);
			player.GetInventory().CreateAttachmentEx(startingClothing.Tops.GetRandomElement(), InventorySlots.BODY);
			player.GetInventory().CreateAttachmentEx(startingClothing.Vests.GetRandomElement(), InventorySlots.VEST);
			player.GetInventory().CreateAttachmentEx(startingClothing.Gloves.GetRandomElement(), InventorySlots.GLOVES);
			player.GetInventory().CreateAttachmentEx(startingClothing.Pants.GetRandomElement(), InventorySlots.LEGS);
			player.GetInventory().CreateAttachmentEx(startingClothing.Belts.GetRandomElement(), InventorySlots.HIPS);
			player.GetInventory().CreateAttachmentEx(startingClothing.Shoes.GetRandomElement(), InventorySlots.FEET);
			player.GetInventory().CreateAttachmentEx(startingClothing.Armbands.GetRandomElement(), InventorySlots.ARMBAND);
			player.GetInventory().CreateAttachmentEx(startingClothing.Backpacks.GetRandomElement(), InventorySlots.BACK);
			
			return true;
		}
		
		return false;
	}
	
	// ------------------------------------------------------------
	// Override EquipCharacter
	// ------------------------------------------------------------
	override void EquipCharacter()
	{
		if ( GetExpansionSettings() && GetExpansionSettings().GetSpawn().StartingClothing.UseCustomClothing )
		{
			if (ExpansionEquipCharacter(m_player))	
				StartingEquipSetup(m_player, true);
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
			if ( m_top != -1 && m_bottom != -1 && m_shoes != -1 && m_skin != -1 )
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
			ExpansionStartingGear gear;
			if ( Class.CastTo(gear, GetExpansionSettings().GetSpawn().StartingGear) )
			{
				if ( gear.UsingUpperGear )
				{
					EntityAI itemTop = player.FindAttachmentBySlotName("Body");
					
					if ( itemTop )
					{
						for ( i = 0; i < gear.UpperGear.Count(); i++ )
						{
							itemTop.GetInventory().CreateInInventory( gear.UpperGear[i] );
						}
					}
				}
				
				if ( gear.UsingPantsGear )
				{
					EntityAI itemPants = player.FindAttachmentBySlotName("Pants");
					
					if ( itemPants )
					{					
						for ( i = 0; i < gear.PantsGear.Count(); i++ )
						{
							itemPants.GetInventory().CreateInInventory( gear.PantsGear[i] );
						}
					}
				}
				
				if ( gear.UsingBackpackGear )
				{
					EntityAI itemBag = player.FindAttachmentBySlotName("Back");
					if ( itemBag )
					{					
						for ( i = 0; i < gear.BackpackGear.Count(); i++ )
						{
							itemBag.GetInventory().CreateInInventory( gear.BackpackGear[i] );
						}
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
