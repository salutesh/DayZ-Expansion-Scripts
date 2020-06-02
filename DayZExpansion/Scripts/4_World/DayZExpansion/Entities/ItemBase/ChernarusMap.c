/**
 * ChernarusMap.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

enum ExpansionChernarusMapItemRPC
{
	NONE,
	RequestItemData = 20620,
	SendItemData = 20621,
	COUNT
}

modded class ChernarusMap
{
	string m_MapItemName;
	
	// ------------------------------------------------------------
	// ChernarusMap Consturctor
	// ------------------------------------------------------------
	void ChernarusMap()
	{		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ChernarusMap::ChernarusMap - Start");
		#endif
		
		m_MapItemName = ConfigGetString("displayName");
		
		string worldName;
		GetGame().GetWorldName( worldName );
		
		if ( m_MapItemName != worldName && worldName != "" )
			m_MapItemName = worldName + " Map";
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ChernarusMap::ChernarusMap - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ChernarusMap GetDisplayName
	// ------------------------------------------------------------
	override string GetDisplayName()
	{
		return m_MapItemName;
	}
	
	// ------------------------------------------------------------
	// ChernarusMap PlayerInventoryCheckLocal
	// ------------------------------------------------------------
	private void PlayerInventoryCheckLocal()
	{
		if ( IsMissionClient() )
		{
			//! Get player who has this item
			if ( GetHierarchyRootPlayer() && GetHierarchyRootPlayer().IsKindOf("SurvivorBase") )
			{
				PlayerBase player = PlayerBase.Cast( GetHierarchyRootPlayer() );
				if ( player )
				{
					player.SetHasItemMap( true );
				}
			}
		}
	}
	
	// ------------------------------------------------------------
	// ChernarusMap EEInventoryIn
	// ------------------------------------------------------------	
	override void EEInventoryIn(Man newParentMan, EntityAI diz, EntityAI newParent)
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ChernarusMap::EEInventoryIn - Start");
		#endif
		
		super.EEInventoryIn( newParentMan, diz, newParent );
		
		if ( newParentMan && newParentMan.IsInherited( SurvivorBase ) )
		{
			if ( IsMissionClient() )
			{
				PlayerBase player = PlayerBase.Cast( newParentMan);
				
				if ( !player )
				{
					#ifdef EXPANSIONEXLOGPRINT
					EXLogPrint("ChernarusMap::EEInventoryIn - player: " + player.ToString());
					#endif

					return;
				}
				
				player.SetHasItemMap( true );
			}
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ChernarusMap::EEInventoryIn - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ChernarusMap EEInventoryOut
	// ------------------------------------------------------------	
	override void EEInventoryOut(Man oldParentMan, EntityAI diz, EntityAI newParent)
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ChernarusMap::EEInventoryOut - Start");
		#endif
		
		super.EEInventoryOut( oldParentMan, diz, newParent );
		
		if ( oldParentMan && oldParentMan.IsInherited( SurvivorBase ) )
		{
			if ( IsMissionClient() )
			{
				PlayerBase player = PlayerBase.Cast( oldParentMan );
				
				if ( !player )
				{
					#ifdef EXPANSIONEXLOGPRINT
					EXLogPrint("ChernarusMap::EEInventoryOut - player: " + player.ToString());
					#endif
					
					return;
				}
				
				player.SetHasItemMap( false );
			}
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ChernarusMap::EEInventoryOut - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// ChernarusMap EEInit
	// ------------------------------------------------------------
	override void EEInit()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ChernarusMap::EEInit - Start");
		#endif
		
		super.EEInit();
		
		PlayerInventoryCheckLocal();
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ChernarusMap::EEInit - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion OnRPC
	// ------------------------------------------------------------
	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext  ctx) 
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ChernarusMap::OnRPC - Start");
		#endif
		
		super.OnRPC(sender, rpc_type, ctx);
		
		switch (rpc_type)
		{
			case ExpansionChernarusMapItemRPC.RequestItemData:
			{
				RPC_RequestItemData( ctx, sender );
				break;
			}
			case ExpansionChernarusMapItemRPC.SendItemData:
			{
				RPC_SendItemData( ctx, sender );
				break;
			}
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ChernarusMap::OnRPC - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion RPC_SendItemData
	// Called on client
	// ------------------------------------------------------------
	private void RPC_SendItemData( ref ParamsReadContext ctx, PlayerIdentity senderRPC )
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ChernarusMap::RPC_SendItemData - Start");
		#endif
		
		if ( !ctx.Read( m_MapItemName ) )
			return;
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ChernarusMap::RPC_SendItemData - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion RPC_RequestItemData
	// Called on server
	// ------------------------------------------------------------
	private void RPC_RequestItemData( ref ParamsReadContext ctx, PlayerIdentity senderRPC )
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ChernarusMap::RPC_RequestItemData - Start");
		#endif
		
		ScriptRPC rpc = new ScriptRPC();
		rpc.Write( m_MapItemName );
		rpc.Send( this, ExpansionChernarusMapItemRPC.SendItemData, true, senderRPC );
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ChernarusMap::RPC_RequestItemData - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion RequestItemData
	// Called on client
	// ------------------------------------------------------------
	void RequestItemData()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ChernarusMap::RequestItemData - Start");
		#endif
		
		if ( IsMissionClient() )
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Send( this, ExpansionChernarusMapItemRPC.RequestItemData, true, NULL );
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ChernarusMap::RequestItemData - End");
		#endif
	}
}