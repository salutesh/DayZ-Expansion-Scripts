/**
 * ExpansionWorldObjectsModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class ExpansionWorldObjectsModule
{
	static ref ScriptInvoker SI_LampEnable = new ScriptInvoker();
	static ref ScriptInvoker SI_LampDisable = new ScriptInvoker();

	private autoptr array< vector > m_LightGenerators;

	private string m_WorldName;
 	
	// ------------------------------------------------------------
	// ExpansionWorldObjectsModule Constructor
	// ------------------------------------------------------------
	void ExpansionWorldObjectsModule()
	{
		m_LightGenerators = new array< vector >;
	}
	
	// ------------------------------------------------------------
	// Expansion OnInit
	// ------------------------------------------------------------
	override void OnInit()
	{
		super.OnInit();
		
		EnableMissionLoaded();
	}
 	
 	override void OnMissionStart(Class sender, CF_EventArgs args)
 	{
		super.OnMissionStart(sender, args);

		if (!GetGame().IsServer())
			return;

		m_WorldName = AdjustWorldName( g_Game.GetWorldName() );

		if (GetExpansionSettings().GetGeneral().Mapping && GetExpansionSettings().GetGeneral().Mapping.UseCustomMappingModule)
		{
			foreach (string name: GetExpansionSettings().GetGeneral().Mapping.Mapping)
			{
				LoadObjectsFile(EXPANSION_MAPPING_FOLDER + m_WorldName + "/" + name + EXPANSION_MAPPING_EXT);
			}

		}
	}

	// ------------------------------------------------------------
	// Expansion OnMissionLoaded
	// ------------------------------------------------------------
	override void OnMissionLoaded(Class sender, CF_EventArgs args)
	{
		super.OnMissionLoaded(sender, args);

		if ( !IsMissionOffline() && IsMissionClient() )
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Send( NULL, ExpansionWorldObjectsModuleRPC.Load, true );
		}
	}
	
	// ------------------------------------------------------------
	// Expansion AdjustWorldName
	// ------------------------------------------------------------	
	private string AdjustWorldName( string name )
	{
		string nName = name;
		nName.ToLower();
		if ( nName.Contains( "chernarusplus" ) )
		{
			return "chernarusplus";
		}

		return nName;
	}
	
	// ------------------------------------------------------------
	// Expansion OnRPC
	// ------------------------------------------------------------	
	override void OnRPC(Class sender, CF_EventArgs args)
	{
		super.OnRPC(sender, args);

		auto rpc = CF_EventRPCArgs.Cast(args);

		switch ( rpc.ID )
		{
		case ExpansionWorldObjectsModuleRPC.TurnOn:
			RPC_TurnOn( rpc.Context, rpc.Sender, rpc.Target );
			break;
		case ExpansionWorldObjectsModuleRPC.TurnOff:
			RPC_TurnOff( rpc.Context, rpc.Sender, rpc.Target );
			break;
		case ExpansionWorldObjectsModuleRPC.Load:
			RPC_Load( rpc.Context, rpc.Sender, rpc.Target );
			break;
		}
	}
	
	// ------------------------------------------------------------
	// Expansion RPC_TurnOn
	// ------------------------------------------------------------	
	private void RPC_TurnOn( ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		if ( IsMissionHost() )
			return;

		vector position;
		if ( ctx.Read( position ) )
		{
			m_LightGenerators.Insert( position );
			SI_LampEnable.Invoke( position );
		}
	}
	
	// ------------------------------------------------------------
	// Expansion RPC_TurnOff
	// ------------------------------------------------------------	
	private void RPC_TurnOff( ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		if ( IsMissionHost() )
			return;

		vector position;
		if ( ctx.Read( position ) )
		{
			m_LightGenerators.RemoveItem( position );
			SI_LampDisable.Invoke( position );
		}
	}
	
	// ------------------------------------------------------------
	// Expansion RPC_Load
	// ------------------------------------------------------------	
	private void RPC_Load( ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		if ( IsMissionHost() )
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write( m_LightGenerators );
			rpc.Send( NULL, ExpansionWorldObjectsModuleRPC.Load, true, senderRPC );
		} else
		{
			ctx.Read( m_LightGenerators );

			for ( int i = 0; i < m_LightGenerators.Count(); i++ )
			{
				SI_LampEnable.Invoke( m_LightGenerators[i] );
			}
		}
	}
	
	// ------------------------------------------------------------
	// Expansion TurnOnGenerator
	// ------------------------------------------------------------	
	void TurnOnGenerator( vector position )
	{
		#ifdef EXPANSION_MAPPING_MODULE_DEBUG
		EXLogPrint("TurnOnGenerator - Start - " + position );
		#endif

		m_LightGenerators.Insert( position );

		if ( IsMissionOffline() )
		{
			SI_LampEnable.Invoke( position );
		} else
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write( position );
			rpc.Send( NULL, ExpansionWorldObjectsModuleRPC.TurnOn, true, NULL );
		}

		#ifdef EXPANSION_MAPPING_MODULE_DEBUG
		EXLogPrint("TurnOnGenerator - End - " + position );
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion TurnOffGenerator
	// ------------------------------------------------------------	
	void TurnOffGenerator( vector position )
	{
		#ifdef EXPANSION_MAPPING_MODULE_DEBUG
		EXLogPrint("TurnOffGenerator - Start - " + position );
		#endif

		m_LightGenerators.RemoveItem( position );

		if ( IsMissionOffline() )
		{
			SI_LampDisable.Invoke( position );
		} 
		else
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write( position );
			rpc.Send( NULL, ExpansionWorldObjectsModuleRPC.TurnOff, true, NULL );
		}

		#ifdef EXPANSION_MAPPING_MODULE_DEBUG
		EXLogPrint("TurnOffGenerator - End - " + position );
		#endif
	}
};