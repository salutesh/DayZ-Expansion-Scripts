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

		Expansion_RegisterBothRPC("RPC_Load");
		Expansion_RegisterClientRPC("RPC_TurnOn");
		Expansion_RegisterClientRPC("RPC_TurnOff");
	}
 	
 	override void OnMissionStart(Class sender, CF_EventArgs args)
 	{
		super.OnMissionStart(sender, args);

		if (!GetGame().IsServer())
			return;

		m_WorldName = ExpansionStatic.GetCanonicalWorldName();

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
			auto rpc = Expansion_CreateRPC("RPC_Load");
			rpc.Expansion_Send(true );
		}
	}
	
	// ------------------------------------------------------------
	// Expansion RPC_TurnOn
	// ------------------------------------------------------------	
	private void RPC_TurnOn(PlayerIdentity senderRPC, Object target, ParamsReadContext ctx)
	{
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
	private void RPC_TurnOff(PlayerIdentity senderRPC, Object target, ParamsReadContext ctx)
	{
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
	private void RPC_Load(PlayerIdentity senderRPC, Object target, ParamsReadContext ctx)
	{
		if ( IsMissionHost() )
		{
			auto rpc = Expansion_CreateRPC("RPC_Load");
			rpc.Write( m_LightGenerators );
			rpc.Expansion_Send(true, senderRPC);
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
			auto rpc = Expansion_CreateRPC("RPC_TurnOn");
			rpc.Write( position );
			rpc.Expansion_Send(true);
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
			auto rpc = Expansion_CreateRPC("RPC_TurnOff");
			rpc.Write( position );
			rpc.Expansion_Send(true);
		}

		#ifdef EXPANSION_MAPPING_MODULE_DEBUG
		EXLogPrint("TurnOffGenerator - End - " + position );
		#endif
	}
};