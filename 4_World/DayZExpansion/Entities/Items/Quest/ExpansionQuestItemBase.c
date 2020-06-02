/**
 * ExpansionQuestItemBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

enum ExpansionQuestItemRPC
{
	NONE,
	RequestItemData = 20600,
	SendItemData = 20601,
	COUNT
}

class ExpansionQuestItemBase extends GiftBox_Base
{	
	//ExpansionQuest m_Quest;
	bool m_IsInInventory;
	string m_QuestItemName;
	string m_QuestItemDescription;
	
	bool m_IsDeliveryItem;
	bool m_IsDelivered;
	
	int m_MapMarker;
	
	// ------------------------------------------------------------
	// ExpansionQuestItemBase Constructor
	// ------------------------------------------------------------
    void ExpansionQuestItemBase()
    {
		m_IsInInventory = false;
		m_QuestItemName = ConfigGetString("displayName");
		m_QuestItemDescription = ConfigGetString("descriptionShort");
		
		m_IsDeliveryItem = true;
		m_IsDelivered = false;
    }
		
	// ------------------------------------------------------------
	// Override GetDisplayName
	// ------------------------------------------------------------
	override string GetDisplayName()
	{
		return m_QuestItemName;
	}
	
	// ------------------------------------------------------------
	// Override GetDisplayName
	// ------------------------------------------------------------
	override string GetTooltip()
	{
		return m_QuestItemDescription;
	}
	
	// ------------------------------------------------------------
	// Override IsContainer
	// ------------------------------------------------------------
	override bool IsContainer()
	{
		return true;
	}
	
	// ------------------------------------------------------------
	// Override IsHeavyBehaviour
	// ------------------------------------------------------------
	override bool IsHeavyBehaviour()
	{
        return false;
    }

	// ------------------------------------------------------------
	// Override IsTwoHandedBehaviour
	// ------------------------------------------------------------
    override bool IsTwoHandedBehaviour()
	{
        return true;
    }
		
	// ------------------------------------------------------------
	// ExpansionQuestItemBase CanPutInCargo
	// ------------------------------------------------------------
	override bool CanPutInCargo( EntityAI parent )
	{
		if( !super.CanPutInCargo(parent) ) {return false;}	
		if ( parent != this && ( this.GetType() != parent.GetType() ) )
		{
			return true;
		}
		
		return false;
	}
	
	// ------------------------------------------------------------
	// ExpansionCarKeys EEInventoryIn
	// ------------------------------------------------------------	
	override void EEInventoryIn(Man newParentMan, EntityAI diz, EntityAI newParent)
	{
		super.EEInventoryIn( newParentMan, diz, newParent );
	}
	
	// ------------------------------------------------------------
	// ExpansionCarKeys EEInventoryOut
	// ------------------------------------------------------------	
	override void EEInventoryOut(Man oldParentMan, EntityAI diz, EntityAI newParent)
	{
		super.EEInventoryOut( oldParentMan, diz, newParent );
	}
	
	// ------------------------------------------------------------
	// ExpansionQuestItemBase OnItemLocationChanged
	// ------------------------------------------------------------
	override void OnItemLocationChanged( EntityAI old_owner, EntityAI new_owner ) 
	{
		super.OnItemLocationChanged( old_owner, new_owner );
		
		if ( new_owner && new_owner.IsInherited(SurvivorBase) || old_owner && old_owner.IsInherited(SurvivorBase) )
		{
			if ( IsMissionClient() )
			{
				RequestItemData();
			}		
		}
	}
	
	// ------------------------------------------------------------
	// Expansion EECargoIn
	//! Event called on item when a item has been placed into its cargo inventory, passes the item as a parameter
	// ------------------------------------------------------------	
	override void EECargoIn(EntityAI item)
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionQuestItemBase::EECargoIn - Start");
		#endif
		
		super.EECargoIn(item);
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionQuestItemBase::EECargoIn - Added item to cargo: " + item.GetType() );
		#endif
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionQuestItemBase::EECargoIn - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion EECargoIn
	//! Event called on item when a item has been removed from its cargo inventory, passes the item as a parameter
	// ------------------------------------------------------------	
	override void EECargoOut(EntityAI item)
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionQuestItemBase::EECargoOut - Start");
		#endif
		
		super.EECargoOut(item);
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionQuestItemBase::EECargoIn - Removed item from cargo: " + item.GetType() );
		#endif
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionQuestItemBase::EECargoOut - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion EEInit
	// ------------------------------------------------------------
	override void EEInit()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionQuestItemBase::EEInit - Start");
		#endif
		
		super.EEInit();
		
		if ( IsMissionClient() )
		{
			RequestItemData();
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("ExpansionQuestItemBase::EEInit - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion EEDelete
	// ------------------------------------------------------------
	override void EEDelete(EntityAI parent)
	{
		super.EEDelete(parent);	
	}
	
	// ------------------------------------------------------------
	// Expansion SetActions
	// ------------------------------------------------------------
	override void SetActions()
	{
		super.SetActions();
	}
	
	// ------------------------------------------------------------
	// Expansion IsQuestItem
	// ------------------------------------------------------------
	override bool IsQuestItem()
	{
		return true;
	}
	
	// ------------------------------------------------------------
	// Expansion OnRPC
	// ------------------------------------------------------------
	override void OnRPC(PlayerIdentity sender, int rpc_type, ParamsReadContext  ctx) 
	{
		super.OnRPC(sender, rpc_type, ctx);
		
		switch (rpc_type)
		{
			case ExpansionQuestItemRPC.RequestItemData:
			{
				RPC_RequestItemData( ctx, sender );
				break;
			}
			case ExpansionQuestItemRPC.SendItemData:
			{
				RPC_SendItemData( ctx, sender );
				break;
			}
		}
	}
	
	// ------------------------------------------------------------
	// Expansion RPC_SendItemData
	// Called on client
	// ------------------------------------------------------------
	private void RPC_SendItemData( ref ParamsReadContext ctx, PlayerIdentity senderRPC )
	{
		//if ( !ctx.Read( m_Quest ) )
		//	return;
		
		if ( !ctx.Read( m_QuestItemName ) )
			return;
		
		if ( !ctx.Read( m_QuestItemDescription ) )
			return;
	}
	
	// ------------------------------------------------------------
	// Expansion RPC_RequestItemData
	// Called on server
	// ------------------------------------------------------------
	private void RPC_RequestItemData( ref ParamsReadContext ctx, PlayerIdentity senderRPC )
	{
		ScriptRPC rpc = new ScriptRPC();
		//rpc.Write( m_Quest );
		rpc.Write( m_QuestItemName );
		rpc.Write( m_QuestItemDescription );
		rpc.Send( this, ExpansionQuestItemRPC.SendItemData, true, senderRPC );
	}
	
	// ------------------------------------------------------------
	// Expansion SendItemData
	// Called on client
	// ------------------------------------------------------------
	void RequestItemData()
	{
		if ( IsMissionClient() )
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Send( this, ExpansionQuestItemRPC.RequestItemData, true, NULL );
		}
	}
	
	// ------------------------------------------------------------
	// Expansion OnStoreLoad
	// ------------------------------------------------------------
	override bool OnStoreLoad(ParamsReadContext ctx, int version)
	{   
		if ( !super.OnStoreLoad(ctx, version) )
			return false;

		//if ( !ctx.Read( m_Quest ) )
		//	return false;
		
		if ( !ctx.Read( m_QuestItemName ) )
			return false;
		
		if ( !ctx.Read( m_QuestItemDescription ) )
			return false;
		
		return true;
	}

	// ------------------------------------------------------------
	// Expansion OnStoreSave
	// ------------------------------------------------------------
	override void OnStoreSave(ParamsWriteContext ctx)
	{
		super.OnStoreSave(ctx);
		
		//ctx.Write( m_Quest );
		ctx.Write( m_QuestItemName );
		ctx.Write( m_QuestItemDescription );
	}
	
	// ------------------------------------------------------------
	// Expansion OnInventoryEnter
	// ------------------------------------------------------------
	//! Event called on item when it is placed in the player(Man) inventory, passes the owner as a parameter
	//  Called on server and client
	override void OnInventoryEnter(Man player)
	{
		super.OnInventoryEnter(player);
		
		//! Print("ExpansionQuestItemBase::OnInventoryEnter - Item has been added to player invetory of player: " + player.GetIdentityName() );
		
		m_IsInInventory = true;
		
		PlayerBase playerBase;
		if ( IsMissionHost() )
			playerBase = PlayerBase.Cast( player );
		else
			playerBase = PlayerBase.Cast( GetGame().GetPlayer() );

		if ( !playerBase || !playerBase.GetIdentity() )
			return;

		
	}
	
	// ------------------------------------------------------------
	// Expansion OnInventoryExit
	// ------------------------------------------------------------
	//! Event called on item when it is removed from the player(Man) inventory, passes the old owner as a parameter
	//  Called on server and client
	override void OnInventoryExit(Man player)
	{
		super.OnInventoryExit(player);
		
		//! Print("ExpansionQuestItemBase::OnInventoryExit - Item has been removed from player invetory of player: " + player.GetIdentityName() );
				
		m_IsInInventory = false;
		
		PlayerBase playerBase;
		if ( IsMissionHost() )
			playerBase = PlayerBase.Cast( player );
		else
			playerBase = PlayerBase.Cast( GetGame().GetPlayer() );

		if ( !playerBase || !playerBase.GetIdentity() )
			return;

		
	}
	
	// ------------------------------------------------------------
	// Expansion IsInPlayerInvetory
	// ------------------------------------------------------------
	bool IsInPlayerInvetory()
	{
		return m_IsInInventory;
	}
	
	// ------------------------------------------------------------
	// Expansion SetQuestItemName
	// ------------------------------------------------------------
	void SetQuestItemName(string name)
	{
		m_QuestItemName = name;
	}
	
	// ------------------------------------------------------------
	// Expansion SetQuestItemDesc
	// ------------------------------------------------------------
	void SetQuestItemDesc(string desc)
	{
		m_QuestItemDescription = desc;
	}
	// ------------------------------------------------------------
	// Expansion IsDeliveryItem
	// ------------------------------------------------------------	
	bool IsDeliveryItem()
	{
		return m_IsDeliveryItem;
	}
	
	// ------------------------------------------------------------
	// Expansion IsDelivered
	// ------------------------------------------------------------
	bool IsDelivered()
	{
		return m_IsDelivered;
	}
	
	// ------------------------------------------------------------
	// Expansion SetDelivered
	// ------------------------------------------------------------	
	void SetDelivered(bool state)
	{
		m_IsDelivered = state;
	}
	
	// ------------------------------------------------------------
	// Expansion SetQuest
	// ------------------------------------------------------------	
	/*
	void SetQuest(ExpansionQuest quest)
	{
		m_Quest = quest;
	}
	*/
}