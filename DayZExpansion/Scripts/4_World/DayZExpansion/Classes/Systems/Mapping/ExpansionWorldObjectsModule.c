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

	private autoptr array< vector > m_LightGenerators = {};

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

		string worldName = ExpansionStatic.GetCanonicalWorldName();

		if (GetExpansionSettings().GetGeneral().Mapping && GetExpansionSettings().GetGeneral().Mapping.UseCustomMappingModule)
		{
			foreach (string name: GetExpansionSettings().GetGeneral().Mapping.Mapping)
			{
				LoadObjectsFile(EXPANSION_MAPPING_FOLDER + worldName + "/" + name + EXPANSION_MAPPING_EXT);
			}

		}
	}

	override void OnMissionLoaded(Class sender, CF_EventArgs args)
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.MAPPING, this);
	#endif

		super.OnMissionLoaded(sender, args);

		if ( !IsMissionOffline() && IsMissionClient() )
		{
			auto rpc = Expansion_CreateRPC("RPC_Load");
			rpc.Expansion_Send(true );
		}
	}
	
	private void RPC_TurnOn(PlayerIdentity senderRPC, Object target, ParamsReadContext ctx)
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.MAPPING, this);
	#endif

		vector position;
		if ( ctx.Read( position ) )
		{
			m_LightGenerators.Insert( position );
			SI_LampEnable.Invoke( position );
		}
	}
	
	private void RPC_TurnOff(PlayerIdentity senderRPC, Object target, ParamsReadContext ctx)
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.MAPPING, this);
	#endif

		vector position;
		if ( ctx.Read( position ) )
		{
			m_LightGenerators.RemoveItem( position );
			SI_LampDisable.Invoke( position );
		}
	}
	
	private void RPC_Load(PlayerIdentity senderRPC, Object target, ParamsReadContext ctx)
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.MAPPING, this);
	#endif

		int count;
		vector position;

		if ( IsMissionHost() )
		{
			auto rpc = Expansion_CreateRPC("RPC_Load");

			count = m_LightGenerators.Count();

			rpc.Write(count);

			for (int i = 0; i < count; ++i)
			{
				position = m_LightGenerators[i];
				rpc.Write(position[0]);
				rpc.Write(position[1]);
				rpc.Write(position[2]);
			}

			rpc.Expansion_Send(true, senderRPC);
		}
		else
		{
			m_LightGenerators.Clear();

			if (!ctx.Read(count))
				return;

			while (count--)
			{
				float x, y, z;

				if (!ctx.Read(x))
					return;

				if (!ctx.Read(y))
					return;

				if (!ctx.Read(z))
					return;

				position = Vector(x, y, z);
				m_LightGenerators.Insert(position);
				SI_LampEnable.Invoke(position);
			}
		}
	}
	
	void TurnOnGenerator( vector position )
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.MAPPING, this);
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
	}
	
	void TurnOffGenerator( vector position )
	{
	#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.MAPPING, this);
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
	}
};
