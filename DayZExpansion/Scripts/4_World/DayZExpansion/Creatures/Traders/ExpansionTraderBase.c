/**
 * ExpansionTraderBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionTraderBase
 * @brief		This class handle trader base entity
 **/
class ExpansionTraderBase extends DayZPlayer
{
	private static ref set< ExpansionTraderBase > m_allTraders = new set< ExpansionTraderBase >;

	protected ref TInputActionMap m_InputActionMap;
	protected ref ExpansionMarketTraderZone m_TraderZone;
	protected ref ExpansionMarketTrader m_Trader;

	bool m_ActionsInitialize;

	private ref ExpansionHumanCommandTrader_ST m_CommandTraderTable;

	// ------------------------------------------------------------
	// Constructor
	// ------------------------------------------------------------
	void ExpansionTraderBase()
	{
		#ifdef EXPANSIONEXLOGPRINT
		Print("ExpansionTraderBase::ExpansionTraderBase - Start");
		#endif
		
		if ( IsMissionHost() )
		{
			SetAllowDamage( false );
		}

		m_allTraders.Insert( this );

		GetGame().GetCallQueue( CALL_CATEGORY_GAMEPLAY ).CallLater( this.OnInit, 250 );
		
		#ifdef EXPANSIONEXLOGPRINT
		Print("ExpansionTraderBase::ExpansionTraderBase - End");
		#endif
	}

	// ------------------------------------------------------------
	// Destructor
	// ------------------------------------------------------------
	void ~ExpansionTraderBase()
	{
		#ifdef EXPANSIONEXLOGPRINT
		Print("ExpansionTraderBase::~ExpansionTraderBase - Start");
		#endif

		delete m_CommandTraderTable;
		
		int idx = m_allTraders.Find( this );
		if ( idx >= 0 )
		{
			m_allTraders.Remove( idx );
		}

		#ifdef EXPANSIONEXLOGPRINT
		Print("ExpansionTraderBase::~ExpansionTraderBase - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion GetAll
	// ------------------------------------------------------------
	static set< ExpansionTraderBase > GetAll()
	{
		return m_allTraders;
	}

	// --------------------------------------------------
	// User Actions
	//---------------------------------------------------
	void SetActions()
	{
		AddAction( ExpansionActionOpenTraderMenu );
		//AddAction( ExpansionActionOpenTraderMenuNew );
	}

	void InitializeActions()
	{
		m_InputActionMap = new TInputActionMap;
		SetActions();
	}
	
	override void GetActions(typename action_input_type, out array<ActionBase_Basic> actions)
	{
		if(!m_ActionsInitialize)
		{
			m_ActionsInitialize = true;
			InitializeActions();
		}
		
		actions = m_InputActionMap.Get(action_input_type);
	}
	
	void AddAction(typename actionName)
	{
		ActionBase action = ActionManagerBase.GetAction(actionName);

		typename ai = action.GetInputType();
		if(!ai)
		{
			m_ActionsInitialize = false;
			return;
		}
		
		ref array<ActionBase_Basic> action_array = m_InputActionMap.Get( ai );
		
		if(!action_array)
		{
			action_array = new array<ActionBase_Basic>;
			m_InputActionMap.Insert(ai, action_array);
		}
		action_array.Insert(action); 
	}
	
	void RemoveAction(typename actionName)
	{
		ActionBase action = ActionManagerBase.GetAction(actionName);
		typename ai = action.GetInputType();
		ref array<ActionBase_Basic> action_array = m_InputActionMap.Get( ai );
		
		if(action_array)
		{
			for(int i = 0; i < action_array.Count(); i++)
			{
				if(action == action_array.Get(i))
				{
					action_array.Remove(i);
				}
			}
			action_array = new array<ActionBase_Basic>;
			m_InputActionMap.Insert(ai, action_array);
		}
		action_array.Insert(action); 
	}

	// ------------------------------------------------------------
	// Expansion OnInit
	// ------------------------------------------------------------
	void OnInit()
	{	
		#ifdef EXPANSIONEXLOGPRINT
		Print("ExpansionTraderBase::OnInit - Start");
		#endif
		
		if ( IsMissionHost() )
		{
			OnServerInit();
		} else
		{
			OnClientInit();
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		Print("ExpansionTraderBase::OnInit - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion OnServerInit
	// ------------------------------------------------------------
	void OnServerInit()
	{
		#ifdef EXPANSIONEXLOGPRINT
		Print("ExpansionTraderBase::OnServerInit - Start");
		#endif

		m_Trader = GetExpansionSettings().GetMarket().GetMarketTrader( GetType() );
		m_TraderZone = GetExpansionSettings().GetMarket().GetTraderZoneByPosition( GetPosition() );

		#ifdef EXPANSIONEXLOGPRINT
		Print( "Type: " + GetType() );
		#endif

		#ifdef EXPANSIONEXLOGPRINT
		Print( "Position: " + GetPosition() );
		#endif

		#ifdef EXPANSIONEXLOGPRINT
		Print( m_Trader );
		#endif

		#ifdef EXPANSIONEXLOGPRINT
		Print( "Zone[" + m_TraderZone + "]: " + m_TraderZone.m_ZoneName + " - " + m_TraderZone.m_DisplayName );
		#endif

		#ifdef EXPANSIONEXLOGPRINT
		Print( "" );
		#endif
		#ifdef EXPANSIONEXLOGPRINT
		Print( "" );
		#endif

		m_TraderZone.DebugPrint();
		
		#ifdef EXPANSIONEXLOGPRINT
		Print( "" );
		#endif
		#ifdef EXPANSIONEXLOGPRINT
		Print( "" );
		#endif

		UpdateTraderZone();
		
		#ifdef EXPANSIONEXLOGPRINT
		Print("ExpansionTraderBase::OnServerInit - End");
		#endif
	}

	// ------------------------------------------------------------
	// Expansion OnClientInit
	// ------------------------------------------------------------
	void OnClientInit()
	{
		#ifdef EXPANSIONEXLOGPRINT
		Print("ExpansionTraderBase::OnClientInit - Start");
		#endif

		RequestTraderObject();
		
		#ifdef EXPANSIONEXLOGPRINT
		Print("ExpansionTraderBase::OnClientInit - End");
		#endif
	}

	void RequestTraderObject()
	{
		if ( IsMissionOffline() )
			return;

		ScriptRPC rpc = new ScriptRPC();
		rpc.Send( this, ExpansionMarketRPC.TraderObject, true, NULL );
	}

	override void OnRPC( PlayerIdentity sender, int rpc_type, ParamsReadContext ctx )
	{
		super.OnRPC( sender, rpc_type, ctx );

		if ( rpc_type <= ExpansionMarketRPC.INVALID )
			return;
		if ( rpc_type >= ExpansionMarketRPC.COUNT )
			return;
		
		switch ( rpc_type )
		{
		case ExpansionMarketRPC.TraderObject:
			RPC_TraderObject( ctx, sender );
			break;
		}
	}

	private void RPC_TraderObject( ParamsReadContext ctx, PlayerIdentity sender )
	{
		#ifdef EXPANSIONEXLOGPRINT
		Print( "ExpansionMarketRPC::RPC_TraderObject" );
		#endif

		if ( IsMissionHost() )
		{
			ScriptRPC rpc = new ScriptRPC();
			#ifdef EXPANSIONEXLOGPRINT
			Print( m_Trader );
			#endif

			rpc.Write( m_Trader );
			rpc.Send( this, ExpansionMarketRPC.TraderObject, true, sender );
			return;
		}

		if ( !ctx.Read( m_Trader ) )
		{
			Error("Error reading param");
		}

		#ifdef EXPANSIONEXLOGPRINT
		Print( m_Trader );
		#endif
	}

	// ------------------------------------------------------------
	// Expansion UpdateTraderZone
	// ------------------------------------------------------------
	void UpdateTraderZone()
	{
		#ifdef EXPANSIONEXLOGPRINT
		Print("ExpansionTraderBase::UpdateTraderZone - Start");
		#endif
		
		if ( !m_TraderZone )
		{
			#ifdef EXPANSIONEXLOGPRINT
			Print("ExpansionTraderBase::UpdateTraderZone - Error");
			#endif

			return;
		}

		#ifdef EXPANSIONEXLOGPRINT
		Print( "Zone[" + m_TraderZone + "]: " + m_TraderZone.m_ZoneName + " - " + m_TraderZone.m_DisplayName );
		#endif

		for ( int i = 0; i < m_Trader.Items.Count(); i++ )
		{
			if ( !m_TraderZone.ItemExists( m_Trader.Items[i] ) )
			{
				#ifdef EXPANSIONEXLOGPRINT
				Print( "Item Didn't Exist in Zone: " + m_Trader.Items[i] );
				#endif

				m_TraderZone.SetStock( m_Trader.Items[i], 0 );
			} else
			{
				#ifdef EXPANSIONEXLOGPRINT
				Print( "Item Exists in Zone: " + m_Trader.Items[i] );
				#endif
			}
		}

		#ifdef EXPANSIONEXLOGPRINT
		Print( "" );
		#endif
		
		#ifdef EXPANSIONEXLOGPRINT
		Print( "" );
		#endif

		m_TraderZone.DebugPrint();
		
		#ifdef EXPANSIONEXLOGPRINT
		Print("ExpansionTraderBase::UpdateTraderZone - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// IsInventoryVisible
	// ------------------------------------------------------------
	override bool IsInventoryVisible()
	{
		return false;
	}
	
	// ------------------------------------------------------------
	// CanDisplayCargo
	// ------------------------------------------------------------
	override bool CanDisplayCargo()
	{
		return false;
	}

	// ------------------------------------------------------------
	// CanReceiveItemIntoHands
	// ------------------------------------------------------------
	override bool CanReceiveItemIntoHands(EntityAI item_to_hands)
	{
		return false;
	}
	
	// ------------------------------------------------------------
	// CanPutIntoHands
	// ------------------------------------------------------------
	override bool CanPutIntoHands(EntityAI parent)
	{
		return false;
	}
	
	// ------------------------------------------------------------
	// GetNetworkSerialization
	// ------------------------------------------------------------
	void GetNetworkSerialization( out ref array< ref ExpansionMarketNetworkItem > list )
	{
		#ifdef EXPANSIONEXLOGPRINT
		Print( "ExpansionTraderBase::GetNetworkSerialization - Start -" + m_TraderZone );
		#endif

		if ( m_TraderZone )
		{
			m_TraderZone.GetNetworkSerialization( GetTraderMarket(), list );
			return;
		}
		
		if ( !list )
		{
			list = new ref array< ref ExpansionMarketNetworkItem >;
		}
		
		#ifdef EXPANSIONEXLOGPRINT
		Print( "ExpansionTraderBase::GetNetworkSerialization - End -" + m_TraderZone );
		#endif
	}

	// ------------------------------------------------------------
	// Expansion GetTraderZone
	// ------------------------------------------------------------
	ExpansionMarketTraderZone GetTraderZone()
	{
		return m_TraderZone;
	}

	// ------------------------------------------------------------
	// Expansion GetTraderMarket
	// ------------------------------------------------------------
	ref ExpansionMarketTrader GetTraderMarket()
	{
		return m_Trader;
	}
	
	// ------------------------------------------------------------
	// Expansion HasVehicleSpawnPosition
	// ------------------------------------------------------------
	bool HasVehicleSpawnPosition( string className, out vector spawnPosition )
	{
		array< vector > positions;

		if ( GetGame().IsKindOf( className, "ExpansionBoatScript" ) )
		{
			positions = GetExpansionSettings().GetMarket().WaterSpawnPositions;
		} else if ( GetGame().IsKindOf( className, "ExpansionHelicopterScript" ) )
		{
			positions = GetExpansionSettings().GetMarket().AirSpawnPositions;
		} else if ( GetGame().IsKindOf( className, "ExpansionPlaneScript" ) )
		{
			positions = GetExpansionSettings().GetMarket().AirSpawnPositions;
		} else
		{
			positions = GetExpansionSettings().GetMarket().LandSpawnPositions;
		}

		if ( !positions )
			return false;

		for ( int i = 0; i < positions.Count(); i++ )
		{
			if ( vector.Distance( positions[i], GetPosition() ) > 30.0 )
				continue;
			
			if ( !VehicleSpawnPositionFree( positions[i] ) )
				continue;
			
			spawnPosition = positions[i];
			return true;
		}

		return false;
	}
	
	// ------------------------------------------------------------
	// Expansion VehicleSpawnPositionFree
	// ------------------------------------------------------------
	private bool VehicleSpawnPositionFree( vector pos )
	{
		return true;
	}

	override void CommandHandler( float pDt, int pCurrentCommandID, bool pCurrentCommandFinished ) 
	{
		if ( pCurrentCommandID == DayZPlayerConstants.COMMANDID_SCRIPT )
		{
			HumanCommandScript hcs = GetCommand_Script();	
		
			ExpansionHumanCommandTrader traderCommand;
			if ( Class.CastTo( traderCommand, hcs ) )
			{
				return;
			}
		}

		if ( m_CommandTraderTable == NULL )
			m_CommandTraderTable = new ExpansionHumanCommandTrader_ST( this );
		
		StartCommand_Script( new ExpansionHumanCommandTrader( this, m_CommandTraderTable ) );
	}
}